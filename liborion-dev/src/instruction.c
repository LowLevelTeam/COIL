#include "orionpp/instruction.h"
#include "orionpp/error.h"
#include "orionpp/arena.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  if (!buf || bufsize == 0) {
    return 0;
  }
  const char *opstr = "UNKNOWN";
  
  int written = -1;
  if (opcode->root == ORIONPP_OPCODE_ISA && (opcode->module_ < sizeof(opcode_isa_strings) / sizeof(opcode_isa_strings[0]))) {
    opstr = opcode_isa_strings[opcode->module_];
    written = snprintf(buf, bufsize, "ISA.%s", opstr);
  } else {
    written = snprintf(buf, bufsize, "%s", opstr);
  }
  
  return (written > 0 && (size_t)written < bufsize) ? written : 0;
}

size_t orionpp_string_type(char *buf, size_t bufsize, orionpp_type_t *type) {
  if (!buf || bufsize == 0) {
    return 0;
  }
  
  const char *typestr = "UNKNOWN";
  


  int written = -1;
  if (type->base.root == ORIONPP_TYPE_QUAL && (type->base.module_ < sizeof(type_qual_strings) / sizeof(type_qual_strings[0]))) {
    opstr = type_qual_strings[type->base.module_];

    if (type->count == 0) {
      int nextwritten_signed = snprintf(buf, bufsize, "%s<", opstr); // unknown second type
      if (nextwritten_signed < 0) {
        return 0;
      }
      size_t nextwritten = (size_t)nextwritten_signed;

      orionpp_type_t *type = type->types;
      size_t next_type_size = orionpp_string_type(buf + nextwritten, bufsize - nextwritten, type);
      if (next_type_size == 0) {
        return 0;
      }

      snprintf(buf + nextwritten + next_type_size, bufsize - nextwritten - next_type_size, ">", opstr);
    } else {
      written = snprintf(buf, bufsize, "%s<T>", opstr); // unknown second type
    }
  } else if (type->base.root == ORIONPP_TYPE_QUAL && (type->base.module_ < sizeof(type_int_strings) / sizeof(type_int_strings[0]))) {
    opstr = type_int_strings[type->base.module_];
    written = snprintf(buf, bufsize, "%s", opstr);
  } else {
    written = snprintf(buf, bufsize, "%s", opstr);
  }

  return (written > 0 && (size_t)written < bufsize) ? written : 0;
}

size_t orionpp_string_value(char *buf, size_t bufsize, orionpp_value_t *value) {
  if (!buf || bufsize == 0) {
    return 0;
  }
  
  int print_invalid = 1;

  int written = -1;
  if (value->type.base.root == ORIONPP_TYPE_QUAL && (value->type.base.module_ < sizeof(type_qual_strings) / sizeof(type_qual_strings[0]))) {
    // const forwards
    // volatile forwards
    // pointer prints memory address (memory addresses are hardcoded as of the current version to be 8 bytes)
  } else if (value->type.base.root == ORIONPP_TYPE_INT && (value->type.base.module_ < sizeof(type_int_strings) / sizeof(type_int_strings[0]))) {
    print_invalid = 0;
    switch (value->type.base.module_) {
      case ORIONPP_TYPE_INT8:
        if (datasize == 1) written = snprintf(buf, bufsize, "%hhd", *((int8_t)data));
        break;
      case ORIONPP_TYPE_INT16:
        if (datasize == 2) written = snprintf(buf, bufsize, "%hd", *((int16_t)data));
        break;
      case ORIONPP_TYPE_INT32:
        if (datasize == 4) written = snprintf(buf, bufsize, "%d", *((int32_t)data));
        break;
      case ORIONPP_TYPE_INT64:
        if (datasize == 8) written = snprintf(buf, bufsize, "%ld", *((int64_t)data));
        break;
      case ORIONPP_TYPE_UNT8:
        if (datasize == 1) written = snprintf(buf, bufsize, "%hhu", *((uint8_t)data));
        break;
      case ORIONPP_TYPE_UNT16:
        if (datasize == 2) written = snprintf(buf, bufsize, "%hu", *((uint16_t)data));
        break;
      case ORIONPP_TYPE_UNT32:
        if (datasize == 4) written = snprintf(buf, bufsize, "%u", *((uint32_t)data));
        break;
      case ORIONPP_TYPE_UNT64:
        if (datasize == 8) written = snprintf(buf, bufsize, "%lu", *((uint64_t)data));
        break;
      default:
        print_invalid = 1;
    }
  }

  if (print_invalid) {
    const char *invalidmsg = "INVALID;"
    written = snprintf(buf, bufsize, "%s", invalidmsg);
  }

  return (written > 0 && (size_t)written < bufsize) ? written : 0;
}

size_t orionpp_string_instr(char *buf, size_t bufsize, orinopp_instruction_t *instr) {
  if (!buf || bufsize == 0) {
    return 0;
  }
  
  // string opcode

  // string arguments

  return 0;
}

void orionpp_print_opcode(orionpp_opcode_t *opcode) {
  char buf[64];
  if (orionpp_string_opcode(buf, sizeof(buf), opcode)) {
    printf("Opcode: %s\n", buf);
  }
}

void orionpp_print_type(orionpp_type_t *type) {
  char buf[512];
  if (orionpp_string_type(buf, sizeof(buf), type)) {
    printf("Type: %s\n", buf);
  }
}

void orionpp_print_value(orionpp_value_t *value) {
  char buf[1024];
  if (orionpp_string_value(buf, sizeof(buf), value)) {
    printf("Value: %s\n", buf);
  }
}

void orionpp_print_instr(orinopp_instruction_t *instr) {
  char buf[4096];
  if (orionpp_string_instr(buf, sizeof(buf), instr)) {
    printf("Instruction: %s\n", buf);
  }
}

// File I/O helpers
orionpp_error_t orionpp_readf(file_handle_t file, orinopp_instruction_t *dest) {
  if (!dest) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
#ifdef WIN32
  DWORD bytes_read;
  if (!ReadFile(file, &dest->opcode, sizeof(dest->opcode), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (read(file, &dest->opcode, sizeof(dest->opcode)) != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // For simplicity, assume no values for now
  dest->values = NULL;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writef(file_handle_t file, const orinopp_instruction_t *src) {
  if (!src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
#ifdef WIN32
  DWORD bytes_written;
  if (!WriteFile(file, &src->opcode, sizeof(src->opcode), &bytes_written, NULL) ||
      bytes_written != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (write(file, &src->opcode, sizeof(src->opcode)) != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  return ORIONPP_ERROR_GOOD;
}

// Arena I/O helpers
int orionpp_readarena(orionpp_arena_t *arena, orinopp_instruction_t *dest) {
  // This would need arena read position tracking
  // For now, return error
  return ORIONPP_ERROR_INVALID_ARG;
}

int orionpp_writearena(const orionpp_arena_t *arena, const orinopp_instruction_t *src) {
  // This would need arena write position tracking
  // For now, return error
  return ORIONPP_ERROR_INVALID_ARG;
}

// Buffer I/O helpers
orionpp_error_t orionpp_readbuf(char *buf, size_t bufsize, orinopp_instruction_t *dest) {
  if (!buf || !dest || bufsize < sizeof(orionpp_opcode_t)) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  memcpy(&dest->opcode, buf, sizeof(orionpp_opcode_t));
  dest->values = NULL;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writebuf(const char *buf, size_t bufsize, const orinopp_instruction_t *src) {
  if (!buf || !src || bufsize < sizeof(orionpp_opcode_t)) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  memcpy((void*)buf, &src->opcode, sizeof(orionpp_opcode_t));
  
  return ORIONPP_ERROR_GOOD;
}