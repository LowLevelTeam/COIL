// Updated instruction.c with complete debug helpers

#include "orionpp/instruction.h"
#include "orionpp/error.h"
#include "orionpp/arena.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef WIN32
  #include <io.h>
#else
  #include <unistd.h>
#endif

static const char *opcode_isa_strings[] = {
  "LET", "CONST", "MOV", "LEA", "JMP", "BREQ", "BRNEQ", "BRGT", "BRGE",
  "BRLT", "BRLE", "BRZ", "BRNZ", "CALL", "RET", "LABEL", "SCOPE", "SCOPL",
  "ADD", "SUB", "MUL", "DIV", "MOD", "INC", "DEC", "INCp", "DECp",
  "AND", "OR", "XOR", "NOT", "SHL", "SHR"
};

static const char *type_qual_strings[] = {
  "const", "volatile", "ptr"
};

static const char *type_int_strings[] = {
  "int8", "int16", "int32", "int64",
  "uint8", "uint16", "uint32", "uint64"
};

// Debug Helpers
size_t orionpp_string_opcode(char *buf, size_t bufsize, orionpp_opcode_t *opcode) {
  if (!buf || bufsize == 0 || !opcode) {
    return 0;
  }
  
  int written = -1;
  if (opcode->root == ORIONPP_OPCODE_ISA && 
      opcode->module_ < (sizeof(opcode_isa_strings) / sizeof(opcode_isa_strings[0]))) {
    const char *opstr = opcode_isa_strings[opcode->module_];
    written = snprintf(buf, bufsize, "ISA.%s", opstr);
  } else {
    written = snprintf(buf, bufsize, "UNKNOWN(%d.%d)", opcode->root, opcode->module_);
  }
  
  return (written > 0 && (size_t)written < bufsize) ? (size_t)written : 0;
}

// Helper function to recursively format types
static size_t format_type_recursive(char *buf, size_t bufsize, const orionpp_type_t *type, int depth) {
  if (!buf || bufsize == 0 || !type || depth > 10) { // Prevent infinite recursion
    return 0;
  }
  
  int written = -1;
  
  if (type->base.root == ORIONPP_TYPE_QUAL && 
      type->base.module_ < (sizeof(type_qual_strings) / sizeof(type_qual_strings[0]))) {
    const char *typestr = type_qual_strings[type->base.module_];
    
    if (type->count > 0 && type->types) {
      // Format: qualifier<inner_type>
      written = snprintf(buf, bufsize, "%s<", typestr);
      if (written < 0 || (size_t)written >= bufsize) {
        return 0;
      }
      
      size_t total_written = written;
      
      // Format each inner type
      for (size_t i = 0; i < type->count && total_written < bufsize; i++) {
        if (i > 0) {
          int comma_written = snprintf(buf + total_written, bufsize - total_written, ", ");
          if (comma_written < 0 || total_written + comma_written >= bufsize) {
            return 0;
          }
          total_written += comma_written;
        }
        
        size_t inner_len = format_type_recursive(buf + total_written, 
                                                 bufsize - total_written, 
                                                 &type->types[i], 
                                                 depth + 1);
        if (inner_len == 0) {
          return 0;
        }
        total_written += inner_len;
      }
      
      // Close the bracket
      int close_written = snprintf(buf + total_written, bufsize - total_written, ">");
      if (close_written < 0 || total_written + close_written >= bufsize) {
        return 0;
      }
      total_written += close_written;
      
      return total_written;
    } else {
      // Format: qualifier (without inner type specified)
      written = snprintf(buf, bufsize, "%s", typestr);
    }
  } else if (type->base.root == ORIONPP_TYPE_INT && 
             type->base.module_ < (sizeof(type_int_strings) / sizeof(type_int_strings[0]))) {
    const char *typestr = type_int_strings[type->base.module_];
    written = snprintf(buf, bufsize, "%s", typestr);
  } else {
    written = snprintf(buf, bufsize, "UNKNOWN_TYPE(%d.%d)", 
                       type->base.root, type->base.module_);
  }

  return (written > 0 && (size_t)written < bufsize) ? (size_t)written : 0;
}

size_t orionpp_string_type(char *buf, size_t bufsize, orionpp_type_t *type) {
  if (!buf || bufsize == 0 || !type) {
    return 0;
  }
  
  return format_type_recursive(buf, bufsize, type, 0);
}

size_t orionpp_string_value(char *buf, size_t bufsize, orionpp_value_t *value) {
  if (!buf || bufsize == 0 || !value) {
    return 0;
  }
  
  // Format as "VALUE: TYPE"
  char type_buf[256];
  size_t type_len = orionpp_string_type(type_buf, sizeof(type_buf), &value->type);
  if (type_len == 0) {
    int written = snprintf(buf, bufsize, "INVALID_TYPE");
    return (written > 0 && (size_t)written < bufsize) ? (size_t)written : 0;
  }
  
  char value_str[256] = "NULL";
  int print_invalid = 1;
  
  if (value->value && value->value_byte_size > 0) {
    void *data = value->value;
    size_t datasize = value->value_byte_size;

    if (value->type.base.root == ORIONPP_TYPE_INT && 
        value->type.base.module_ < (sizeof(type_int_strings) / sizeof(type_int_strings[0]))) {
      print_invalid = 0;
      
      switch (value->type.base.module_) {
        case ORIONPP_TYPE_INT8:
          if (datasize == 1) snprintf(value_str, sizeof(value_str), "%hhd", *((int8_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_INT16:
          if (datasize == 2) snprintf(value_str, sizeof(value_str), "%hd", *((int16_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_INT32:
          if (datasize == 4) snprintf(value_str, sizeof(value_str), "%d", *((int32_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_INT64:
          if (datasize == 8) snprintf(value_str, sizeof(value_str), "%ld", *((int64_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_UNT8:
          if (datasize == 1) snprintf(value_str, sizeof(value_str), "%hhu", *((uint8_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_UNT16:
          if (datasize == 2) snprintf(value_str, sizeof(value_str), "%hu", *((uint16_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_UNT32:
          if (datasize == 4) snprintf(value_str, sizeof(value_str), "%u", *((uint32_t*)data));
          else print_invalid = 1;
          break;
        case ORIONPP_TYPE_UNT64:
          if (datasize == 8) snprintf(value_str, sizeof(value_str), "%lu", *((uint64_t*)data));
          else print_invalid = 1;
          break;
        default:
          print_invalid = 1;
      }
    } else if (value->type.base.root == ORIONPP_TYPE_QUAL && 
               value->type.base.module_ == ORIONPP_TYPE_PTR) {
      print_invalid = 0;
      if (datasize == sizeof(void*)) {
        snprintf(value_str, sizeof(value_str), "%p", *((void**)data));
      } else {
        print_invalid = 1;
      }
    }

    if (print_invalid) {
      snprintf(value_str, sizeof(value_str), "INVALID_VALUE(size=%zu)", datasize);
    }
  }

  int written = snprintf(buf, bufsize, "%s: %s", value_str, type_buf);
  return (written > 0 && (size_t)written < bufsize) ? (size_t)written : 0;
}

size_t orionpp_string_instr(char *buf, size_t bufsize, orinopp_instruction_t *instr) {
  if (!buf || bufsize == 0 || !instr) {
    return 0;
  }
  
  // Get opcode string
  size_t opcode_len = orionpp_string_opcode(buf, bufsize, &instr->opcode);
  if (opcode_len == 0) {
    return 0;
  }
  
  size_t total_written = opcode_len;
  
  // Add values
  if (instr->value_count > 0 && instr->values) {
    int written = snprintf(buf + total_written, bufsize - total_written, " ");
    if (written < 0 || total_written + written >= bufsize) {
      return 0;
    }
    total_written += written;
    
    for (size_t i = 0; i < instr->value_count; i++) {
      if (i > 0) {
        written = snprintf(buf + total_written, bufsize - total_written, ", ");
        if (written < 0 || total_written + written >= bufsize) {
          return 0;
        }
        total_written += written;
      }
      
      size_t value_len = orionpp_string_value(buf + total_written, 
                                              bufsize - total_written, 
                                              &instr->values[i]);
      if (value_len == 0) {
        return 0;
      }
      total_written += value_len;
    }
  }

  return total_written;
}

void orionpp_print_opcode(orionpp_opcode_t *opcode) {
  if (!opcode) {
    printf("Opcode: NULL\n");
    return;
  }
  
  char buf[64];
  if (orionpp_string_opcode(buf, sizeof(buf), opcode)) {
    printf("Opcode: %s\n", buf);
  } else {
    printf("Opcode: [Format Error]\n");
  }
}

void orionpp_print_type(orionpp_type_t *type) {
  if (!type) {
    printf("Type: NULL\n");
    return;
  }
  
  char buf[512];
  if (orionpp_string_type(buf, sizeof(buf), type)) {
    printf("Type: %s (count=%zu)\n", buf, type->count);
  } else {
    printf("Type: [Format Error]\n");
  }
}

void orionpp_print_value(orionpp_value_t *value) {
  if (!value) {
    printf("Value: NULL\n");
    return;
  }
  
  char buf[1024];
  if (orionpp_string_value(buf, sizeof(buf), value)) {
    printf("Value: %s\n", buf);
  } else {
    printf("Value: [Format Error]\n");
  }
}

void orionpp_print_instr(orinopp_instruction_t *instr) {
  if (!instr) {
    printf("Instruction: NULL\n");
    return;
  }
  
  char buf[4096];
  if (orionpp_string_instr(buf, sizeof(buf), instr)) {
    printf("Instruction: %s\n", buf);
  } else {
    printf("Instruction: [Format Error]\n");
  }
}

orionpp_error_t orionpp_readf(file_handle_t file, orinopp_instruction_t *dest) {
  if (!dest) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Read opcode
#ifdef WIN32
  DWORD bytes_read;
  if (!ReadFile(file, &dest->opcode, sizeof(dest->opcode), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read value count
  if (!ReadFile(file, &dest->value_count, sizeof(dest->value_count), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (read(file, &dest->opcode, sizeof(dest->opcode)) != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  if (read(file, &dest->value_count, sizeof(dest->value_count)) != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // Allocate and read values if any
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
#ifdef WIN32
    if (!ReadFile(file, dest->values, dest->value_count * sizeof(orionpp_value_t), 
                  &bytes_read, NULL) ||
        bytes_read != dest->value_count * sizeof(orionpp_value_t)) {
      free(dest->values);
      return ORIONPP_ERROR_FILE_ERROR;
    }
#else
    if (read(file, dest->values, dest->value_count * sizeof(orionpp_value_t)) != 
        (ssize_t)(dest->value_count * sizeof(orionpp_value_t))) {
      free(dest->values);
      return ORIONPP_ERROR_FILE_ERROR;
    }
#endif
  } else {
    dest->values = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writef(file_handle_t file, const orinopp_instruction_t *src) {
  if (!src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Write opcode
#ifdef WIN32
  DWORD bytes_written;
  if (!WriteFile(file, &src->opcode, sizeof(src->opcode), &bytes_written, NULL) ||
      bytes_written != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Write value count
  if (!WriteFile(file, &src->value_count, sizeof(src->value_count), &bytes_written, NULL) ||
      bytes_written != sizeof(src->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (write(file, &src->opcode, sizeof(src->opcode)) != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  if (write(file, &src->value_count, sizeof(src->value_count)) != sizeof(src->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // Write values if any
  if (src->value_count > 0 && src->values) {
#ifdef WIN32
    if (!WriteFile(file, src->values, src->value_count * sizeof(orionpp_value_t), 
                   &bytes_written, NULL) ||
        bytes_written != src->value_count * sizeof(orionpp_value_t)) {
      return ORIONPP_ERROR_FILE_ERROR;
    }
#else
    if (write(file, src->values, src->value_count * sizeof(orionpp_value_t)) != 
        (ssize_t)(src->value_count * sizeof(orionpp_value_t))) {
      return ORIONPP_ERROR_FILE_ERROR;
    }
#endif
  }
  
  return ORIONPP_ERROR_GOOD;
}

// Arena I/O helpers
int orionpp_readarena(orionpp_arena_t *arena, orinopp_instruction_t *dest) {
  // This would need arena read position tracking
  // For now, return error indicating not implemented
  (void)arena;
  (void)dest;
  return ORIONPP_ERROR_UNSUPPORTED_FEATURE;
}

int orionpp_writearena(const orionpp_arena_t *arena, const orinopp_instruction_t *src) {
  // This would need arena write position tracking
  // For now, return error indicating not implemented
  (void)arena;
  (void)src;
  return ORIONPP_ERROR_UNSUPPORTED_FEATURE;
}

// Buffer I/O helpers
orionpp_error_t orionpp_readbuf(char *buf, size_t bufsize, orinopp_instruction_t *dest) {
  if (!buf || !dest || bufsize < sizeof(orionpp_opcode_t) + sizeof(size_t)) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  size_t offset = 0;
  
  // Read opcode
  memcpy(&dest->opcode, buf + offset, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Read value count
  memcpy(&dest->value_count, buf + offset, sizeof(size_t));
  offset += sizeof(size_t);
  
  // Check if buffer has enough space for values
  if (offset + dest->value_count * sizeof(orionpp_value_t) > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Read values if any
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    memcpy(dest->values, buf + offset, dest->value_count * sizeof(orionpp_value_t));
  } else {
    dest->values = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writebuf(char *buf, size_t bufsize, const orinopp_instruction_t *src) {
  if (!buf || !src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  size_t needed_size = sizeof(orionpp_opcode_t) + sizeof(size_t) + 
                       src->value_count * sizeof(orionpp_value_t);
  
  if (bufsize < needed_size) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  size_t offset = 0;
  
  // Write opcode
  memcpy(buf + offset, &src->opcode, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Write value count
  memcpy(buf + offset, &src->value_count, sizeof(size_t));
  offset += sizeof(size_t);
  
  // Write values if any
  if (src->value_count > 0 && src->values) {
    memcpy(buf + offset, src->values, src->value_count * sizeof(orionpp_value_t));
  }
  
  return ORIONPP_ERROR_GOOD;
}