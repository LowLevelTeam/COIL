/**
 * @file src/orionpp.c
 * @brief Implementation of orion++ functionality
 */

#include "orion/orionpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <fcntl.h>
#endif

// -------------------------------- HELPERS -------------------------------- //

static const char* opcode_to_string(orionpp_opcode_t opcode) {
  switch (opcode) {
    case ORIONPP_OP_ISA: return "ISA";
    case ORIONPP_OP_OBJ: return "OBJ";
    case ORIONPP_OP_HINT: return "HINT";
    case ORIONPP_OP_TYPE: return "TYPE";
    case ORIONPP_OP_ABI: return "ABI";
    case ORIONPP_OP_END: return "END";
    default: return "UNKNOWN";
  }
}

static const char* isa_opcode_to_string(orionpp_opcode_module_t child) {
  switch (child) {
    case ORIONPP_OP_ISA_VAR: return "VAR";
    case ORIONPP_OP_ISA_CONST: return "CONST";
    case ORIONPP_OP_ISA_MOV: return "MOV";
    case ORIONPP_OP_ISA_LEA: return "LEA";
    case ORIONPP_OP_ISA_LABEL: return "LABEL";
    case ORIONPP_OP_ISA_JMP: return "JMP";
    case ORIONPP_OP_ISA_BREQ: return "BR.EQ";
    case ORIONPP_OP_ISA_BRNEQ: return "BR.NEQ";
    case ORIONPP_OP_ISA_BRGT: return "BR.GT";
    case ORIONPP_OP_ISA_BRGE: return "BR.GE"; 
    case ORIONPP_OP_ISA_BRLT: return "BR.LT";
    case ORIONPP_OP_ISA_BRLE: return "BR.LE";
    case ORIONPP_OP_ISA_BRZ: return "BR.Z";
    case ORIONPP_OP_ISA_BRNZ: return "BR.NZ";
    case ORIONPP_OP_ISA_CALL: return "CALL";
    case ORIONPP_OP_ISA_RET: return "RET";
    case ORIONPP_OP_ISA_ADD: return "ADD";
    case ORIONPP_OP_ISA_SUB: return "SUB";
    case ORIONPP_OP_ISA_MUL: return "MUL";
    case ORIONPP_OP_ISA_DIV: return "DIV";
    case ORIONPP_OP_ISA_MOD: return "MOD";
    case ORIONPP_OP_ISA_INC: return "INC";
    case ORIONPP_OP_ISA_DEC: return "DEC";
    case ORIONPP_OP_ISA_INCp: return "INC++";
    case ORIONPP_OP_ISA_DECp: return "DEC++";
    case ORIONPP_OP_ISA_AND: return "AND";
    case ORIONPP_OP_ISA_OR: return "OR";
    case ORIONPP_OP_ISA_XOR: return "XOR";
    case ORIONPP_OP_ISA_NOT: return "NOT";
    case ORIONPP_OP_ISA_SHL: return "SHL";
    case ORIONPP_OP_ISA_SHR: return "SHR";
    default: return "UNKNOWN_ISA";
  }
}

static const char* type_to_string(orionpp_type_t type) {
  switch (type) {
    case ORIONPP_TYPE_VARID: return "VARID";
    case ORIONPP_TYPE_LABELID: return "LABELID";
    case ORIONPP_TYPE_SYMBOL: return "SYMBOL";
    case ORIONPP_TYPE_STRING: return "STRING";
    case ORIONPP_TYPE_WORD: return "WORD";
    case ORIONPP_TYPE_SIZE: return "SIZE";
    case ORIONPP_TYPE_SSIZE: return "SSIZE";
    case ORIONPP_TYPE_C: return "C";
    case ORIONPP_TYPE_END: return "END";
    default: return "UNKNOWN_TYPE";
  }
}

static const char* type_c_to_string(orionpp_type_module_t type) {
  switch (type) {
    case ORIONPP_TYPE_C_BOOL: return "BOOL";          
    case ORIONPP_TYPE_C_BOOL_FALSE: return "BOOL_FALSE"; 
    case ORIONPP_TYPE_C_BOOL_TRUE: return "BOOL_TRUE";  
    case ORIONPP_TYPE_C_CHAR: return "CHAR";
    case ORIONPP_TYPE_C_CHAR_MIN: return "CHAR_MIN";
    case ORIONPP_TYPE_C_CHAR_MAX: return "CHAR_MAX";
    case ORIONPP_TYPE_C_SCHAR: return "SCHAR";
    case ORIONPP_TYPE_C_SCHAR_MIN: return "SCHAR_MIN";
    case ORIONPP_TYPE_C_SCHAR_MAX: return "SCHAR_MAX";
    case ORIONPP_TYPE_C_UCHAR: return "UCHAR";
    case ORIONPP_TYPE_C_UCHAR_MIN: return "UCHAR_MIN";
    case ORIONPP_TYPE_C_UCHAR_MAX: return "UCHAR_MAX";
    case ORIONPP_TYPE_C_SHORT: return "SHORT";
    case ORIONPP_TYPE_C_SHORT_MIN: return "SHORT_MIN";
    case ORIONPP_TYPE_C_SHORT_MAX: return "SHORT_MAX";
    case ORIONPP_TYPE_C_USHORT: return "USHORT";
    case ORIONPP_TYPE_C_USHORT_MIN: return "USHORT_MIN";
    case ORIONPP_TYPE_C_USHORT_MAX: return "USHORT_MAX";
    case ORIONPP_TYPE_C_INT: return "INT";
    case ORIONPP_TYPE_C_INT_MIN: return "INT_MIN";
    case ORIONPP_TYPE_C_INT_MAX: return "INT_MAX";
    case ORIONPP_TYPE_C_UINT: return "UINT";
    case ORIONPP_TYPE_C_UINT_MIN: return "UINT_MIN";
    case ORIONPP_TYPE_C_UINT_MAX: return "UINT_MAX";
    case ORIONPP_TYPE_C_LONG: return "LONG";
    case ORIONPP_TYPE_C_LONG_MIN: return "LONG_MIN";
    case ORIONPP_TYPE_C_LONG_MAX: return "LONG_MAX";
    case ORIONPP_TYPE_C_ULONG: return "ULONG";
    case ORIONPP_TYPE_C_ULONG_MIN: return "ULONG_MIN";
    case ORIONPP_TYPE_C_ULONG_MAX: return "ULONG_MAX";
    case ORIONPP_TYPE_C_LLONG: return "LLONG";
    case ORIONPP_TYPE_C_LLONG_MIN: return "LLONG_MIN";
    case ORIONPP_TYPE_C_LLONG_MAX: return "LLONG_MAX";
    case ORIONPP_TYPE_C_ULLONG: return "ULLONG";
    case ORIONPP_TYPE_C_ULLONG_MIN: return "ULLONG_MIN";
    case ORIONPP_TYPE_C_ULLONG_MAX: return "ULLONG_MAX";
    case ORIONPP_TYPE_C_FLOAT: return "FLOAT";
    case ORIONPP_TYPE_C_FLOAT_MIN: return "FLOAT_MIN";  
    case ORIONPP_TYPE_C_FLOAT_MAX: return "FLOAT_MAX";  
    case ORIONPP_TYPE_C_DOUBLE: return "DOUBLE";
    case ORIONPP_TYPE_C_DOUBLE_MIN: return "DOUBLE_MIN";
    case ORIONPP_TYPE_C_DOUBLE_MAX: return "DOUBLE_MAX";
    case ORIONPP_TYPE_C_LDOUBLE: return "LDOUBLE";
    case ORIONPP_TYPE_C_LDOUBLE_MIN: return "LDOUBLE_MIN";
    case ORIONPP_TYPE_C_LDOUBLE_MAX: return "LDOUBLE_MAX";
    case ORIONPP_TYPE_C_SIZE_T: return "SIZE_T";
    case ORIONPP_TYPE_C_SIZE_T_MIN: return "SIZE_T_MIN";
    case ORIONPP_TYPE_C_SIZE_T_MAX: return "SIZE_T_MAX";
    case ORIONPP_TYPE_C_PTRDIFF_T: return "PTRDIFF_T";
    case ORIONPP_TYPE_C_PTRDIFF_T_MIN: return "PTRDIFF_T_MIN";
    case ORIONPP_TYPE_C_PTRDIFF_T_MAX: return "PTRDIFF_T_MAX";
    case ORIONPP_TYPE_C_INTMAX_T: return "INTMAX_T";
    case ORIONPP_TYPE_C_INTMAX_T_MIN: return "INTMAX_T_MIN";  
    case ORIONPP_TYPE_C_INTMAX_T_MAX: return "INTMAX_T_MAX";  
    case ORIONPP_TYPE_C_UINTMAX_T: return "UINTMAX_T";
    case ORIONPP_TYPE_C_UINTMAX_T_MIN: return "UINTMAX_T_MIN";
    case ORIONPP_TYPE_C_UINTMAX_T_MAX: return "UINTMAX_T_MAX";
    case ORIONPP_TYPE_C_INTPTR_T: return "INTPTR_T";
    case ORIONPP_TYPE_C_INTPTR_T_MIN: return "INTPTR_T_MIN";  
    case ORIONPP_TYPE_C_INTPTR_T_MAX: return "INTPTR_T_MAX";  
    case ORIONPP_TYPE_C_UINTPTR_T: return "UINTPTR_T";
    case ORIONPP_TYPE_C_UINTPTR_T_MIN: return "UINTPTR_T_MIN";
    case ORIONPP_TYPE_C_UINTPTR_T_MAX: return "UINTPTR_T_MAX";
    case ORIONPP_TYPE_C_WCHAR_T: return "WCHAR_T";
    case ORIONPP_TYPE_C_WCHAR_T_MIN: return "WCHAR_T_MIN";
    case ORIONPP_TYPE_C_WCHAR_T_MAX: return "WCHAR_T_MAX";
    case ORIONPP_TYPE_C_DEC32: return "DEC32";
    case ORIONPP_TYPE_C_DEC32_MIN: return "DEC32_MIN";
    case ORIONPP_TYPE_C_DEC32_MAX: return "DEC32_MAX";
    case ORIONPP_TYPE_C_DEC64: return "DEC64";
    case ORIONPP_TYPE_C_DEC64_MIN: return "DEC64_MIN";
    case ORIONPP_TYPE_C_DEC64_MAX: return "DEC64_MAX";
    case ORIONPP_TYPE_C_DEC128: return "DEC128";
    case ORIONPP_TYPE_C_DEC128_MIN: return "DEC128_MIN";
    case ORIONPP_TYPE_C_DEC128_MAX: return "DEC128_MAX";
    case ORIONPP_TYPE_C_INT8: return "INT8";   
    case ORIONPP_TYPE_C_UINT8: return "UINT8";  
    case ORIONPP_TYPE_C_INT16: return "INT16";  
    case ORIONPP_TYPE_C_UINT16: return "UINT16"; 
    case ORIONPP_TYPE_C_INT32: return "INT32";  
    case ORIONPP_TYPE_C_UINT32: return "UINT32"; 
    case ORIONPP_TYPE_C_INT64: return "INT64";  
    case ORIONPP_TYPE_C_UINT64: return "UINT64"; 
    case ORIONPP_TYPE_C_PTR: return "PTR"; 
    case ORIONPP_TYPE_C_CONST: return "CONST"; 
    case ORIONPP_TYPE_C_VOLATILE: return "VOLATILE"; 
    case ORIONPP_TYPE_C_RESTRICT: return "RESTRICT"; 
    case ORIONPP_TYPE_C_ATOMIC: return "ATOMIC"; 
    case ORIONPP_TYPE_C_COMPLEX: return "COMPLEX"; 
    case ORIONPP_TYPE_C_IMAGINARY: return "IMAGINARY"; 
    case ORIONPP_TYPE_C_BITINT: return "BITINT"; 
    case ORIONPP_TYPE_C_REGISTER: return "REGISTER"; 
    case ORIONPP_TYPE_C_STRUCT: return "STRUCT";
    case ORIONPP_TYPE_C_UNION: return "UNION";
    case ORIONPP_TYPE_C_STRUCTREF: return "STRUCTREF";
    case ORIONPP_TYPE_C_UNIONREF: return "UNIONREF";
    case ORIONPP_TYPE_C_TYPEREF: return "TYPEREF";
    case ORIONPP_TYPE_C_ARRAY: return "ARRAY";
    case ORIONPP_TYPE_C_VLARRAY: return "VLARRAY";
    case ORIONPP_TYPE_C_VOID: return "VOID";
    case ORIONPP_TYPE_C_VARADIC: return "VARADIC";
    case ORIONPP_TYPE_C_FUNCTION: return "FUNCTION";
    default: return "!INVALID!";
  }
}


static size_t read_from_file(file_handle_t file, void* buffer, size_t size) {
#ifdef WIN32
  DWORD bytes_read;
  if (!ReadFile(file, buffer, (DWORD)size, &bytes_read, NULL)) {
    return 0;
  }
  return (size_t)bytes_read;
#else
  return read(file, buffer, size);
#endif
}

static size_t write_to_file(file_handle_t file, const void* buffer, size_t size) {
#ifdef WIN32
  DWORD bytes_written;
  if (!WriteFile(file, buffer, (DWORD)size, &bytes_written, NULL)) {
    return 0;
  }
  return (size_t)bytes_written;
#else
  return write(file, buffer, size);
#endif
}

// -------------------------------- IMPLEMENTATIONS -------------------------------- //

void orionpp_dissasemble(orinopp_instruction_t *instr) {
  if (!instr) {
    printf("NULL instruction\n");
    return;
  }

  printf("%s", opcode_to_string(instr->root));
  
  // Print child opcode for ISA instructions
  if (instr->root == ORIONPP_OP_ISA) {
    printf(".%s", isa_opcode_to_string(instr->child));
  } else if (instr->child != 0) {
    printf(".%d", instr->child);
  }

  // Print values
  for (size_t i = 0; i < instr->value_count; i++) {
    printf(" ");
    
    orinopp_value_t *value = &instr->values[i];
    printf("%s", type_to_string(value->root));
    
    if (value->child != 0) {
      if (value->child == ORIONPP_TYPE_C) { printf("%s", type_c_to_string(value->child)); }
      else { printf(".%d", value->child); }
    }
    
    printf(":");
    
    // Print value data based on type
    switch (value->root) {
      case ORIONPP_TYPE_VARID:
      case ORIONPP_TYPE_LABELID:
        if (value->bytesize >= sizeof(uint32_t)) {
          uint32_t id = *(uint32_t*)value->bytes;
          printf("%u", id);
        }
        break;
      case ORIONPP_TYPE_WORD:
      case ORIONPP_TYPE_SIZE:
        if (value->bytesize >= sizeof(uint32_t)) {
          uint32_t word = *(uint32_t*)value->bytes;
          printf("0x%x", word);
        }
        break;
      case ORIONPP_TYPE_SSIZE:
        if (value->bytesize >= sizeof(int32_t)) {
          int32_t ssize = *(int32_t*)value->bytes;
          printf("%d", ssize);
        }
        break;
      case ORIONPP_TYPE_STRING:
      case ORIONPP_TYPE_SYMBOL:
        printf("\"");
        for (size_t j = 0; j < value->bytesize && j < 64; j++) {
          char c = value->bytes[j];
          if (c == '\0') break;
          if (c >= 32 && c <= 126) {
            printf("%c", c);
          } else {
            printf("\\x%02x", (unsigned char)c);
          }
        }
        printf("\"");
        break;
      default:
        printf("0x");
        for (size_t j = 0; j < value->bytesize && j < 16; j++) {
          printf("%02x", (unsigned char)value->bytes[j]);
        }
        break;
    }
  }
  
  printf("\n");
}

void orionpp_readf(file_handle_t file, orinopp_instruction_t *dest) {
  if (!dest) return;
  
  // Initialize destination
  memset(dest, 0, sizeof(orinopp_instruction_t));
  
  // Read root opcode
  if (read_from_file(file, &dest->root, sizeof(dest->root)) != sizeof(dest->root)) {
    return;
  }
  
  // Read child opcode
  if (read_from_file(file, &dest->child, sizeof(dest->child)) != sizeof(dest->child)) {
    return;
  }
  
  // Read value count
  if (read_from_file(file, &dest->value_count, sizeof(dest->value_count)) != sizeof(dest->value_count)) {
    return;
  }
  
  if (dest->value_count == 0) {
    dest->values = NULL;
    return;
  }
  
  // Allocate values array
  dest->values = malloc(dest->value_count * sizeof(orinopp_value_t));
  if (!dest->values) {
    dest->value_count = 0;
    return;
  }
  
  // Read each value
  for (size_t i = 0; i < dest->value_count; i++) {
    orinopp_value_t *value = &dest->values[i];
    
    // Read type information
    if (read_from_file(file, &value->root, sizeof(value->root)) != sizeof(value->root)) {
      break;
    }
    if (read_from_file(file, &value->child, sizeof(value->child)) != sizeof(value->child)) {
      break;
    }
    
    // Read byte size
    if (read_from_file(file, &value->bytesize, sizeof(value->bytesize)) != sizeof(value->bytesize)) {
      break;
    }
    
    // Allocate and read bytes
    if (value->bytesize > 0) {
      char *bytes = malloc(value->bytesize);
      if (!bytes) {
        value->bytesize = 0;
        value->bytes = NULL;
        continue;
      }
      
      if (read_from_file(file, bytes, value->bytesize) != value->bytesize) {
        free(bytes);
        value->bytesize = 0;
        value->bytes = NULL;
        continue;
      }
      
      value->bytes = bytes;
    } else {
      value->bytes = NULL;
    }
  }
}

void orionpp_writef(file_handle_t file, const orinopp_instruction_t *src) {
  if (!src) return;
  
  // Write root opcode
  write_to_file(file, &src->root, sizeof(src->root));
  
  // Write child opcode
  write_to_file(file, &src->child, sizeof(src->child));
  
  // Write value count
  write_to_file(file, &src->value_count, sizeof(src->value_count));
  
  // Write each value
  for (size_t i = 0; i < src->value_count; i++) {
    const orinopp_value_t *value = &src->values[i];
    
    // Write type information
    write_to_file(file, &value->root, sizeof(value->root));
    write_to_file(file, &value->child, sizeof(value->child));
    
    // Write byte size
    write_to_file(file, &value->bytesize, sizeof(value->bytesize));
    
    // Write bytes
    if (value->bytesize > 0 && value->bytes) {
      write_to_file(file, value->bytes, value->bytesize);
    }
  }
}

void orionpp_readbuf(char *buf, size_t bufsize, orinopp_instruction_t *dest) {
  if (!buf || !dest || bufsize == 0) return;
  
  size_t offset = 0;
  
  // Initialize destination
  memset(dest, 0, sizeof(orinopp_instruction_t));
  
  // Read root opcode
  if (offset + sizeof(dest->root) > bufsize) return;
  memcpy(&dest->root, buf + offset, sizeof(dest->root));
  offset += sizeof(dest->root);
  
  // Read child opcode
  if (offset + sizeof(dest->child) > bufsize) return;
  memcpy(&dest->child, buf + offset, sizeof(dest->child));
  offset += sizeof(dest->child);
  
  // Read value count
  if (offset + sizeof(dest->value_count) > bufsize) return;
  memcpy(&dest->value_count, buf + offset, sizeof(dest->value_count));
  offset += sizeof(dest->value_count);
  
  if (dest->value_count == 0) {
    dest->values = NULL;
    return;
  }
  
  // Allocate values array
  dest->values = malloc(dest->value_count * sizeof(orinopp_value_t));
  if (!dest->values) {
    dest->value_count = 0;
    return;
  }
  
  // Read each value
  for (size_t i = 0; i < dest->value_count; i++) {
    orinopp_value_t *value = &dest->values[i];
    
    // Read type information
    if (offset + sizeof(value->root) > bufsize) break;
    memcpy(&value->root, buf + offset, sizeof(value->root));
    offset += sizeof(value->root);
    
    if (offset + sizeof(value->child) > bufsize) break;
    memcpy(&value->child, buf + offset, sizeof(value->child));
    offset += sizeof(value->child);
    
    // Read byte size
    if (offset + sizeof(value->bytesize) > bufsize) break;
    memcpy(&value->bytesize, buf + offset, sizeof(value->bytesize));
    offset += sizeof(value->bytesize);
    
    // Allocate and read bytes
    if (value->bytesize > 0) {
      if (offset + value->bytesize > bufsize) {
        value->bytesize = 0;
        value->bytes = NULL;
        break;
      }
      
      char *bytes = malloc(value->bytesize);
      if (!bytes) {
        value->bytesize = 0;
        value->bytes = NULL;
        continue;
      }
      
      memcpy(bytes, buf + offset, value->bytesize);
      offset += value->bytesize;
      
      value->bytes = bytes;
    } else {
      value->bytes = NULL;
    }
  }
}

void orionpp_writebuf(const char *buf, size_t bufsize, const orinopp_instruction_t *src) {
  if (!buf || !src || bufsize == 0) return;
  
  size_t offset = 0;
  char *write_buf = (char*)buf; // Cast away const for writing
  
  // Write root opcode
  if (offset + sizeof(src->root) > bufsize) return;
  memcpy(write_buf + offset, &src->root, sizeof(src->root));
  offset += sizeof(src->root);
  
  // Write child opcode
  if (offset + sizeof(src->child) > bufsize) return;
  memcpy(write_buf + offset, &src->child, sizeof(src->child));
  offset += sizeof(src->child);
  
  // Write value count
  if (offset + sizeof(src->value_count) > bufsize) return;
  memcpy(write_buf + offset, &src->value_count, sizeof(src->value_count));
  offset += sizeof(src->value_count);
  
  // Write each value
  for (size_t i = 0; i < src->value_count; i++) {
    const orinopp_value_t *value = &src->values[i];
    
    // Write type information
    if (offset + sizeof(value->root) > bufsize) return;
    memcpy(write_buf + offset, &value->root, sizeof(value->root));
    offset += sizeof(value->root);
    
    if (offset + sizeof(value->child) > bufsize) return;
    memcpy(write_buf + offset, &value->child, sizeof(value->child));
    offset += sizeof(value->child);
    
    // Write byte size
    if (offset + sizeof(value->bytesize) > bufsize) return;
    memcpy(write_buf + offset, &value->bytesize, sizeof(value->bytesize));
    offset += sizeof(value->bytesize);
    
    // Write bytes
    if (value->bytesize > 0 && value->bytes) {
      if (offset + value->bytesize > bufsize) return;
      memcpy(write_buf + offset, value->bytes, value->bytesize);
      offset += value->bytesize;
    }
  }
}