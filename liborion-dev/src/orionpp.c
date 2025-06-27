/**
 * @file src/orionpp.c
 * @brief Implementation of orion++ functionality
 */

#include "orionpp/orionpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#ifdef WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <fcntl.h>
#endif

// -------------------------------- HELPERS -------------------------------- //

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
void orionpp_parse(const char *buf, char *val, size_t valsize);
void orionpp_parse_type(const char *buf, orionpp_ftype_t *value);


void orionpp_string_opcode(char *buf, size_t bufsize, orionpp_opcode_t opcode, orionpp_opcode_module_t module_opcode) {
  (void)bufsize; // TODO- Implement Safety
  switch (opcode) {
    case ORIONPP_OP_ISA:
      memcpy(buf, "ISA.", 4);
      buf += 4;
      switch (module_opcode) {
        case ORIONPP_OP_ISA_VAR: memcpy(buf, "VAR", 4); break;
        case ORIONPP_OP_ISA_CONST: memcpy(buf, "CONST", 6); break;
        case ORIONPP_OP_ISA_MOV: memcpy(buf, "MOV", 4); break;
        case ORIONPP_OP_ISA_LEA: memcpy(buf, "LEA", 4); break;
        case ORIONPP_OP_ISA_LABEL: memcpy(buf, "LABEL", 6); break;
        case ORIONPP_OP_ISA_JMP: memcpy(buf, "JMP", 4); break;
        case ORIONPP_OP_ISA_BREQ: memcpy(buf, "BR.EQ", 6); break;
        case ORIONPP_OP_ISA_BRNEQ: memcpy(buf, "BR.NEQ", 7); break;
        case ORIONPP_OP_ISA_BRGT: memcpy(buf, "BR.GT", 6); break;
        case ORIONPP_OP_ISA_BRGE: memcpy(buf, "BR.GE", 6); break; 
        case ORIONPP_OP_ISA_BRLT: memcpy(buf, "BR.LT", 6); break;
        case ORIONPP_OP_ISA_BRLE: memcpy(buf, "BR.LE", 6); break;
        case ORIONPP_OP_ISA_BRZ: memcpy(buf, "BR.Z", 5); break;
        case ORIONPP_OP_ISA_BRNZ: memcpy(buf, "BR.NZ", 6); break;
        case ORIONPP_OP_ISA_CALL: memcpy(buf, "CALL", 5); break;
        case ORIONPP_OP_ISA_RET: memcpy(buf, "RET", 4); break;
        case ORIONPP_OP_ISA_ADD: memcpy(buf, "ADD", 4); break;
        case ORIONPP_OP_ISA_SUB: memcpy(buf, "SUB", 4); break;
        case ORIONPP_OP_ISA_MUL: memcpy(buf, "MUL", 4); break;
        case ORIONPP_OP_ISA_DIV: memcpy(buf, "DIV", 4); break;
        case ORIONPP_OP_ISA_MOD: memcpy(buf, "MOD", 4); break;
        case ORIONPP_OP_ISA_INC: memcpy(buf, "INC", 4); break;
        case ORIONPP_OP_ISA_DEC: memcpy(buf, "DEC", 4); break;
        case ORIONPP_OP_ISA_INCp: memcpy(buf, "INC++", 6); break;
        case ORIONPP_OP_ISA_DECp: memcpy(buf, "DEC++", 6); break;
        case ORIONPP_OP_ISA_AND: memcpy(buf, "AND", 4); break;
        case ORIONPP_OP_ISA_OR: memcpy(buf, "OR", 3); break;
        case ORIONPP_OP_ISA_XOR: memcpy(buf, "XOR", 4); break;
        case ORIONPP_OP_ISA_NOT: memcpy(buf, "NOT", 4); break;
        case ORIONPP_OP_ISA_SHL: memcpy(buf, "SHL", 4); break;
        case ORIONPP_OP_ISA_SHR: memcpy(buf, "SHR", 4); break;
        default: memcpy(buf, "UNKN", 5); break;
      };
      break;
    case ORIONPP_OP_OBJ:
      memcpy(buf, "OBJ.", 4);
      buf += 4;
      switch (module_opcode) {
        case ORIONPP_OP_OBJ_SECT: memcpy(buf, "SECT", 5); break;
        case ORIONPP_OP_OBJ_SYM: memcpy(buf, "SYM", 4); break;
        case ORIONPP_OP_OBJ_RAW: memcpy(buf, "RAW", 4); break;
        case ORIONPP_OP_OBJ_RESV: memcpy(buf, "RESV", 5); break;
        default: memcpy(buf, "UNKN", 5); break;
      };
      break;
    case ORIONPP_OP_HINT:
      memcpy(buf, "HINT.", 5);
      buf += 5;
      switch (module_opcode) {
        case ORIONPP_OP_HINT_FUNCEND: memcpy(buf, "FUNCEND", 8); break;
        default: memcpy(buf, "UNKN", 5); break;
      };
      break;
    case ORIONPP_OP_TYPE:
      memcpy(buf, "TYPE.", 5);
      buf += 5;
      switch (module_opcode) {
        case ORIONPP_OP_TYPE_DEF: memcpy(buf, "DEF", 4); break;
        default: memcpy(buf, "UNKN", 5); break;
      };
      break;
    case ORIONPP_OP_ABI:
      memcpy(buf, "ABI.", 4);
      buf += 4;
      switch (module_opcode) {
        case ORIONPP_OP_ABI_ESETUP: memcpy(buf, "ESETUP", 7); break;
        case ORIONPP_OP_ABI_ECLEANUP: memcpy(buf, "ECLEANUP", 9); break;
        case ORIONPP_OP_ABI_GETARG: memcpy(buf, "GETARG", 7); break;
        case ORIONPP_OP_ABI_SETRET: memcpy(buf, "SETRET", 7); break;
        case ORIONPP_OP_ABI_RSETUP: memcpy(buf, "RSETUP", 7); break;
        case ORIONPP_OP_ABI_RCLEANUP: memcpy(buf, "RCLEANUP", 9); break;
        case ORIONPP_OP_ABI_SETARG: memcpy(buf, "SETARG", 7); break;
        case ORIONPP_OP_ABI_GETRET: memcpy(buf, "GETRET", 7); break;
        default: memcpy(buf, "UNKN", 5); break;
      };
      break;
    case ORIONPP_OP_END: memcpy(buf, "END", 4); break;
    default: memcpy(buf, "UNKN", 5);
  }
}
void orionpp_string_type(char *buf, size_t bufsize, orionpp_type_t type, orionpp_type_module_t module_type) {
  (void)bufsize; // TODO- Implement Safety 
  switch (type) {
    case ORIONPP_TYPE_VARID: memcpy(buf, "VARID", 6); break;
    case ORIONPP_TYPE_LABELID: memcpy(buf, "LABELID", 8); break;
    case ORIONPP_TYPE_SYMBOL: memcpy(buf, "SYMBOL", 7); break;
    case ORIONPP_TYPE_STRING: memcpy(buf, "STRING", 7); break;
    case ORIONPP_TYPE_WORD: memcpy(buf, "WORD", 5); break;
    case ORIONPP_TYPE_C:
      *buf = 'C';
      ++buf;
      switch (module_type) {
        case ORIONPP_TYPE_C_BOOL: memcpy(buf, "BOOL", 5); break;          
        case ORIONPP_TYPE_C_BOOL_FALSE: memcpy(buf, "BOOL_FALSE", 11); break; 
        case ORIONPP_TYPE_C_BOOL_TRUE: memcpy(buf, "BOOL_TRUE", 10); break;  
        case ORIONPP_TYPE_C_CHAR: memcpy(buf, "CHAR", 5); break;
        case ORIONPP_TYPE_C_CHAR_MIN: memcpy(buf, "CHAR_MIN", 9); break;
        case ORIONPP_TYPE_C_CHAR_MAX: memcpy(buf, "CHAR_MAX", 9); break;
        case ORIONPP_TYPE_C_SCHAR: memcpy(buf, "SCHAR", 6); break;
        case ORIONPP_TYPE_C_SCHAR_MIN: memcpy(buf, "SCHAR_MIN", 10); break;
        case ORIONPP_TYPE_C_SCHAR_MAX: memcpy(buf, "SCHAR_MAX", 10); break;
        case ORIONPP_TYPE_C_UCHAR: memcpy(buf, "UCHAR", 6); break;
        case ORIONPP_TYPE_C_UCHAR_MIN: memcpy(buf, "UCHAR_MIN", 10); break;
        case ORIONPP_TYPE_C_UCHAR_MAX: memcpy(buf, "UCHAR_MAX", 10); break;
        case ORIONPP_TYPE_C_SHORT: memcpy(buf, "SHORT", 6); break;
        case ORIONPP_TYPE_C_SHORT_MIN: memcpy(buf, "SHORT_MIN", 10); break;
        case ORIONPP_TYPE_C_SHORT_MAX: memcpy(buf, "SHORT_MAX", 10); break;
        case ORIONPP_TYPE_C_USHORT: memcpy(buf, "USHORT", 7); break;
        case ORIONPP_TYPE_C_USHORT_MIN: memcpy(buf, "USHORT_MIN", 11); break;
        case ORIONPP_TYPE_C_USHORT_MAX: memcpy(buf, "USHORT_MAX", 11); break;
        case ORIONPP_TYPE_C_INT: memcpy(buf, "INT", 4); break;
        case ORIONPP_TYPE_C_INT_MIN: memcpy(buf, "INT_MIN", 8); break;
        case ORIONPP_TYPE_C_INT_MAX: memcpy(buf, "INT_MAX", 8); break;
        case ORIONPP_TYPE_C_UINT: memcpy(buf, "UINT", 5); break;
        case ORIONPP_TYPE_C_UINT_MIN: memcpy(buf, "UINT_MIN", 9); break;
        case ORIONPP_TYPE_C_UINT_MAX: memcpy(buf, "UINT_MAX", 9); break;
        case ORIONPP_TYPE_C_LONG: memcpy(buf, "LONG", 5); break;
        case ORIONPP_TYPE_C_LONG_MIN: memcpy(buf, "LONG_MIN", 9); break;
        case ORIONPP_TYPE_C_LONG_MAX: memcpy(buf, "LONG_MAX", 9); break;
        case ORIONPP_TYPE_C_ULONG: memcpy(buf, "ULONG", 6); break;
        case ORIONPP_TYPE_C_ULONG_MIN: memcpy(buf, "ULONG_MIN", 10); break;
        case ORIONPP_TYPE_C_ULONG_MAX: memcpy(buf, "ULONG_MAX", 10); break;
        case ORIONPP_TYPE_C_LLONG: memcpy(buf, "LLONG", 6); break;
        case ORIONPP_TYPE_C_LLONG_MIN: memcpy(buf, "LLONG_MIN", 10); break;
        case ORIONPP_TYPE_C_LLONG_MAX: memcpy(buf, "LLONG_MAX", 10); break;
        case ORIONPP_TYPE_C_ULLONG: memcpy(buf, "ULLONG", 7); break;
        case ORIONPP_TYPE_C_ULLONG_MIN: memcpy(buf, "ULLONG_MIN", 11); break;
        case ORIONPP_TYPE_C_ULLONG_MAX: memcpy(buf, "ULLONG_MAX", 11); break;
        case ORIONPP_TYPE_C_FLOAT: memcpy(buf, "FLOAT", 6); break;
        case ORIONPP_TYPE_C_FLOAT_MIN: memcpy(buf, "FLOAT_MIN", 10); break;  
        case ORIONPP_TYPE_C_FLOAT_MAX: memcpy(buf, "FLOAT_MAX", 10); break;  
        case ORIONPP_TYPE_C_DOUBLE: memcpy(buf, "DOUBLE", 7); break;
        case ORIONPP_TYPE_C_DOUBLE_MIN: memcpy(buf, "DOUBLE_MIN", 11); break;
        case ORIONPP_TYPE_C_DOUBLE_MAX: memcpy(buf, "DOUBLE_MAX", 11); break;
        case ORIONPP_TYPE_C_LDOUBLE: memcpy(buf, "LDOUBLE", 8); break;
        case ORIONPP_TYPE_C_LDOUBLE_MIN: memcpy(buf, "LDOUBLE_MIN", 12); break;
        case ORIONPP_TYPE_C_LDOUBLE_MAX: memcpy(buf, "LDOUBLE_MAX", 12); break;
        case ORIONPP_TYPE_C_SIZE_T: memcpy(buf, "SIZE_T", 7); break;
        case ORIONPP_TYPE_C_SIZE_T_MIN: memcpy(buf, "SIZE_T_MIN", 11); break;
        case ORIONPP_TYPE_C_SIZE_T_MAX: memcpy(buf, "SIZE_T_MAX", 11); break;
        case ORIONPP_TYPE_C_PTRDIFF_T: memcpy(buf, "PTRDIFF_T", 10); break;
        case ORIONPP_TYPE_C_PTRDIFF_T_MIN: memcpy(buf, "PTRDIFF_T_MIN", 14); break;
        case ORIONPP_TYPE_C_PTRDIFF_T_MAX: memcpy(buf, "PTRDIFF_T_MAX", 14); break;
        case ORIONPP_TYPE_C_INTMAX_T: memcpy(buf, "INTMAX_T", 9); break;
        case ORIONPP_TYPE_C_INTMAX_T_MIN: memcpy(buf, "INTMAX_T_MIN", 13); break;  
        case ORIONPP_TYPE_C_INTMAX_T_MAX: memcpy(buf, "INTMAX_T_MAX", 13); break;  
        case ORIONPP_TYPE_C_UINTMAX_T: memcpy(buf, "UINTMAX_T", 10); break;
        case ORIONPP_TYPE_C_UINTMAX_T_MIN: memcpy(buf, "UINTMAX_T_MIN", 14); break;
        case ORIONPP_TYPE_C_UINTMAX_T_MAX: memcpy(buf, "UINTMAX_T_MAX", 14); break;
        case ORIONPP_TYPE_C_INTPTR_T: memcpy(buf, "INTPTR_T", 9); break;
        case ORIONPP_TYPE_C_INTPTR_T_MIN: memcpy(buf, "INTPTR_T_MIN", 13); break;  
        case ORIONPP_TYPE_C_INTPTR_T_MAX: memcpy(buf, "INTPTR_T_MAX", 13); break;  
        case ORIONPP_TYPE_C_UINTPTR_T: memcpy(buf, "UINTPTR_T", 10); break;
        case ORIONPP_TYPE_C_UINTPTR_T_MIN: memcpy(buf, "UINTPTR_T_MIN", 14); break;
        case ORIONPP_TYPE_C_UINTPTR_T_MAX: memcpy(buf, "UINTPTR_T_MAX", 14); break;
        case ORIONPP_TYPE_C_WCHAR_T: memcpy(buf, "WCHAR_T", 8); break;
        case ORIONPP_TYPE_C_WCHAR_T_MIN: memcpy(buf, "WCHAR_T_MIN", 12); break;
        case ORIONPP_TYPE_C_WCHAR_T_MAX: memcpy(buf, "WCHAR_T_MAX", 12); break;
        case ORIONPP_TYPE_C_DEC32: memcpy(buf, "DEC32", 6); break;
        case ORIONPP_TYPE_C_DEC32_MIN: memcpy(buf, "DEC32_MIN", 10); break;
        case ORIONPP_TYPE_C_DEC32_MAX: memcpy(buf, "DEC32_MAX", 10); break;
        case ORIONPP_TYPE_C_DEC64: memcpy(buf, "DEC64", 6); break;
        case ORIONPP_TYPE_C_DEC64_MIN: memcpy(buf, "DEC64_MIN", 10); break;
        case ORIONPP_TYPE_C_DEC64_MAX: memcpy(buf, "DEC64_MAX", 10); break;
        case ORIONPP_TYPE_C_DEC128: memcpy(buf, "DEC128", 7); break;
        case ORIONPP_TYPE_C_DEC128_MIN: memcpy(buf, "DEC128_MIN", 11); break;
        case ORIONPP_TYPE_C_DEC128_MAX: memcpy(buf, "DEC128_MAX", 11); break;
        case ORIONPP_TYPE_C_INT8: memcpy(buf, "INT8", 5); break;
        case ORIONPP_TYPE_C_UINT8: memcpy(buf, "UINT8", 6); break;
        case ORIONPP_TYPE_C_INT16: memcpy(buf, "INT16", 6); break;
        case ORIONPP_TYPE_C_UINT16: memcpy(buf, "UINT16", 7); break;
        case ORIONPP_TYPE_C_INT32: memcpy(buf, "INT32", 6); break;
        case ORIONPP_TYPE_C_UINT32: memcpy(buf, "UINT32", 7); break;
        case ORIONPP_TYPE_C_INT64: memcpy(buf, "INT64", 6); break;
        case ORIONPP_TYPE_C_UINT64: memcpy(buf, "UINT64", 7); break;
        case ORIONPP_TYPE_C_PTR: memcpy(buf, "PTR", 4); break;
        case ORIONPP_TYPE_C_CONST: memcpy(buf, "CONST", 6); break; 
        case ORIONPP_TYPE_C_VOLATILE: memcpy(buf, "VOLATILE", 9); break; 
        case ORIONPP_TYPE_C_RESTRICT: memcpy(buf, "RESTRICT", 9); break; 
        case ORIONPP_TYPE_C_ATOMIC: memcpy(buf, "ATOMIC", 7); break; 
        case ORIONPP_TYPE_C_COMPLEX: memcpy(buf, "COMPLEX", 8); break; 
        case ORIONPP_TYPE_C_IMAGINARY: memcpy(buf, "IMAGINARY", 10); break; 
        case ORIONPP_TYPE_C_REGISTER: memcpy(buf, "REGISTER", 9); break;
        case ORIONPP_TYPE_C_STRUCT: memcpy(buf, "STRUCT", 7); break;
        case ORIONPP_TYPE_C_UNION: memcpy(buf, "UNION", 6); break;
        case ORIONPP_TYPE_C_STRUCTREF: memcpy(buf, "STRUCTREF", 10); break;
        case ORIONPP_TYPE_C_UNIONREF: memcpy(buf, "UNIONREF", 9); break;
        case ORIONPP_TYPE_C_TYPEREF: memcpy(buf, "TYPEREF", 8); break;
        case ORIONPP_TYPE_C_ARRAY: memcpy(buf, "ARRAY", 6); break;
        case ORIONPP_TYPE_C_VLARRAY: memcpy(buf, "VLARRAY", 8); break;
        case ORIONPP_TYPE_C_VOID: memcpy(buf, "VOID", 5); break;
        case ORIONPP_TYPE_C_VARADIC: memcpy(buf, "VARADIC", 8); break;
        case ORIONPP_TYPE_C_FUNCTION: memcpy(buf, "FUNCTION", 9); break;
        default: memcpy(buf, "INVL", 5);
      };
      break;
    case ORIONPP_TYPE_END: memcpy(buf, "END", 4); break;
    default: memcpy(buf, "UNKN", 5);
  }
}

void orionpp_parse_value_simple(const char *buf, orionpp_value_t *val, size_t bytesize) {
  if (orionpp_peak_end() == 1) {
    val->val = arena_alloc(arena, bytesize);
    orionpp_parse(buf, val->val, bytesize);
    val->valsize = bytesize;
  } else {
    val->val = NULL;
    val->valsize = 0;
  }
}
void orionpp_parse_value_complex(const char *buf, orionpp_ftype_t *type); // followed by constant data (like _BitInt<N>)
void orionpp_parse_value_qualifer(const char *buf, orionpp_ftype_t *qualifier, orionpp_ftype_t *type);
void orionpp_parse_value_complex_qualifer(const char *buf, orionpp_ftype_t *qualifier, orionpp_ftype_t *type); // like Arrary<T>[N]
void orionpp_parse_value_list(const char *buf, orionpp_ftype_t *qualifier, orionpp_ftype_t **type);
void orionpp_parse_value_func(const char *buf, orionpp_ftype_t *qualifier, orionpp_ftype_t **rets, orionpp_ftype_t **params);

// parse loaded orionpp into memory
void orionpp_parse_value(orion_arena_t *arena, orionpp_header_t *header, const char *buf, orionpp_value_t *val) {
  orionpp_parse_type(buf, &val->type);

  switch (val->type.type) {
    case ORIONPP_TYPE_VARID:
    case ORIONPP_TYPE_LABELID:
    case ORIONPP_TYPE_SYMBOL:
    case ORIONPP_TYPE_STRING:
      orionpp_parse_value_simple(buf, val, 4);
      break;
    case ORIONPP_TYPE_WORD:
      orionpp_parse_value_simple(buf, val, header->wordbytesize);
    case ORIONPP_TYPE_C:
      switch (val->type.module_type) {
        case ORIONPP_TYPE_C_BOOL:
          orionpp_parse_value_simple(buf, val, 1);
          break;
        case ORIONPP_TYPE_C_BOOL_FALSE:
          val->val = arena_alloc(arena, 1);
          *(bool*)val->val = false;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_BOOL_TRUE:
          val->val = arena_alloc(arena, 1);
          *(bool*)val->val = true;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_CHAR:
          orionpp_parse_value_simple(buf, val, 1);
          break;
        case ORIONPP_TYPE_C_CHAR_MIN:
          val->val = arena_alloc(arena, 1);
          *(char*)val->val = CHAR_MIN;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_CHAR_MAX:
          val->val = arena_alloc(arena, 1);
          *(char*)val->val = CHAR_MAX;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_SCHAR:
          orionpp_parse_value_simple(buf, val, 1);
          break;
        case ORIONPP_TYPE_C_SCHAR_MIN:
          val->val = arena_alloc(arena, 1);
          *(signed char*)val->val = SCHAR_MIN;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_SCHAR_MAX:
          val->val = arena_alloc(arena, 1);
          *(signed char*)val->val = SCHAR_MAX;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_UCHAR:
          orionpp_parse_value_simple(buf, val, 1);
          break;
        case ORIONPP_TYPE_C_UCHAR_MIN:
          val->val = arena_alloc(arena, 1);
          *(unsigned char*)val->val = 0;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_UCHAR_MAX:
          val->val = arena_alloc(arena, 1);
          *(unsigned char*)val->val = UCHAR_MAX;
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_SHORT:
          orionpp_parse_value_simple(buf, val, 2);
          break;
        case ORIONPP_TYPE_C_SHORT_MIN:
          val->val = arena_alloc(arena, 2);
          *(short*)val->val = SHRT_MIN;
          val->valsize = 2;
          break;
        case ORIONPP_TYPE_C_SHORT_MAX:
          val->val = arena_alloc(arena, 2);
          *(short*)val->val = SHRT_MAX;
          val->valsize = 2;
          break;
        case ORIONPP_TYPE_C_USHORT:
          orionpp_parse_value_simple(buf, val, 2);
          break;
        case ORIONPP_TYPE_C_USHORT_MIN:
          val->val = arena_alloc(arena, 2);
          *(unsigned short*)val->val = 0;
          val->valsize = 2;
          break;
        case ORIONPP_TYPE_C_USHORT_MAX:
          val->val = arena_alloc(arena, 2);
          *(unsigned short*)val->val = USHRT_MAX;
          val->valsize = 2;
          break;
        case ORIONPP_TYPE_C_INT:
          orionpp_parse_value_simple(buf, val, max(header->wordbytesize / 2, 2)); // int has to be at least 16 bit according to c standard but on 64 bit its expected to be 32 bit.
          break;
        case ORIONPP_TYPE_C_INT_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize / 2, 2));
          *(int*)val->val = INT_MIN;
          val->valsize = max(header->wordbytesize / 2, 2);
          break;
        case ORIONPP_TYPE_C_INT_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize / 2, 2));
          *(int*)val->val = INT_MAX;
          val->valsize = max(header->wordbytesize / 2, 2);
          break;
        case ORIONPP_TYPE_C_UINT:
          orionpp_parse_value_simple(buf, val, max(header->wordbytesize / 2, 2));
          break;
        case ORIONPP_TYPE_C_UINT_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize / 2, 2));
          *(unsigned int*)val->val = 0;
          val->valsize = max(header->wordbytesize / 2, 2);
          break;
        case ORIONPP_TYPE_C_UINT_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize / 2, 2));
          *(unsigned int*)val->val = UINT_MAX;
          val->valsize = max(header->wordbytesize / 2, 2);
          break;
        case ORIONPP_TYPE_C_LONG:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          orionpp_parse(buf, val->val, max(header->wordbytesize, 4));
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_LONG_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          *(long*)val->val = LONG_MIN;
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_LONG_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          *(long*)val->val = LONG_MAX;
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_ULONG:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          orionpp_parse(buf, val->val, max(header->wordbytesize, 4));
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_ULONG_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          *(unsigned long*)val->val = 0;
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_ULONG_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize, 4));
          *(unsigned long*)val->val = ULONG_MAX;
          val->valsize = max(header->wordbytesize, 4);
          break;
        case ORIONPP_TYPE_C_LLONG:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          orionpp_parse(buf, val->val, max(header->wordbytesize, 8));
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_LLONG_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          *(long long*)val->val = LLONG_MIN;
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_LLONG_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          *(long long*)val->val = LLONG_MAX;
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_ULLONG:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          orionpp_parse(buf, val->val, max(header->wordbytesize, 8));
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_ULLONG_MIN:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          *(unsigned long long*)val->val = 0;
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_ULLONG_MAX:
          val->val = arena_alloc(arena, max(header->wordbytesize, 8));
          *(unsigned long long*)val->val = ULLONG_MAX;
          val->valsize = max(header->wordbytesize, 8);
          break;
        case ORIONPP_TYPE_C_FLOAT:
          val->val = arena_alloc(arena, 4); // float has to be 4 bytes according to orion deal with it. (C doesn't specify)
          orionpp_parse(buf, val->val, 4);
          val->valsize = 4;
          break;
        case ORIONPP_TYPE_C_FLOAT_MIN:
          val->val = arena_alloc(arena, 4);
          *(float*)val->val = FLT_MIN;
          val->valsize = 4;
          break;
        case ORIONPP_TYPE_C_FLOAT_MAX:
          val->val = arena_alloc(arena, 4);
          *(float*)val->val = FLT_MAX;
          val->valsize = 4;
          break;
        case ORIONPP_TYPE_C_DOUBLE:
          val->val = arena_alloc(arena, 8); // double has to be 8 bytes according to orion deal with it. (C doesn't specify)
          orionpp_parse(buf, val->val, 8);
          val->valsize = 8;
          break;
        case ORIONPP_TYPE_C_DOUBLE_MIN:
          val->val = arena_alloc(arena, 8);
          *(double*)val->val = DBL_MIN;
          val->valsize = 8;
          break;
        case ORIONPP_TYPE_C_DOUBLE_MAX:
          val->val = arena_alloc(arena, 8);
          *(double*)val->val = DBL_MAX;
          val->valsize = 8;
          break;
        case ORIONPP_TYPE_C_LDOUBLE:
          val->val = arena_alloc(arena, 16); // long double has to be 16 bytes according to orion deal with it. (C doesn't specify) (could be 10 bytes but just ignore the other bytes then)
          orionpp_parse(buf, val->val, 16);
          val->valsize = 16;
          break;
        case ORIONPP_TYPE_C_LDOUBLE_MIN:
          val->val = arena_alloc(arena, 16);
          *(long double*)val->val = LDBL_MIN;
          val->valsize = 16;
          break;
        case ORIONPP_TYPE_C_LDOUBLE_MAX:
          val->val = arena_alloc(arena, 16);
          *(long double*)val->val = LDBL_MAX;
          val->valsize = 16;
          break;
        case ORIONPP_TYPE_C_WCHAR_T:
          val->val = arena_alloc(arena, sizeof(wchar_t));
          orionpp_parse(buf, val->val, sizeof(wchar_t));
          val->valsize = sizeof(wchar_t);
          break;
        case ORIONPP_TYPE_C_WCHAR_T_MIN:
          val->val = arena_alloc(arena, sizeof(wchar_t));
          *(wchar_t*)val->val = WCHAR_MIN;
          val->valsize = sizeof(wchar_t);
          break;
        case ORIONPP_TYPE_C_WCHAR_T_MAX:
          val->val = arena_alloc(arena, sizeof(wchar_t));
          *(wchar_t*)val->val = WCHAR_MAX;
          val->valsize = sizeof(wchar_t);
          break;
        case ORIONPP_TYPE_C_DEC32:
          val->val = arena_alloc(arena, 4);
          orionpp_parse(buf, val->val, 4);
          val->valsize = 4;
          break;
        case ORIONPP_TYPE_C_DEC64:
          val->val = arena_alloc(arena, 8);
          orionpp_parse(buf, val->val, 8);
          val->valsize = 8;
          break;
        case ORIONPP_TYPE_C_DEC128:
          val->val = arena_alloc(arena, 16);
          orionpp_parse(buf, val->val, 16);
          val->valsize = 16;
          break;
        case ORIONPP_TYPE_C_INT8:
        case ORIONPP_TYPE_C_UINT8:
          val->val = arena_alloc(arena, 1);
          orionpp_parse(buf, val->val, 1);
          val->valsize = 1;
          break;
        case ORIONPP_TYPE_C_INT16:
        case ORIONPP_TYPE_C_UINT16:
          val->val = arena_alloc(arena, 2);
          orionpp_parse(buf, val->val, 2);
          val->valsize = 2;
          break;
        case ORIONPP_TYPE_C_INT32:
        case ORIONPP_TYPE_C_UINT32:
          val->val = arena_alloc(arena, 4);
          orionpp_parse(buf, val->val, 4);
          val->valsize = 4;
          break;
        case ORIONPP_TYPE_C_INT64:
        case ORIONPP_TYPE_C_UINT64:
          val->val = arena_alloc(arena, 8);
          orionpp_parse(buf, val->val, 8);
          val->valsize = 8;
          break;
        case ORIONPP_TYPE_C_PTR:
          val->val = arena_alloc(arena, header->addrbytesize);
          orionpp_parse(buf, val->val, header->addrbytesize);
          val->valsize = header->addrbytesize;
          break;
        case ORIONPP_TYPE_C_CONST:
        case ORIONPP_TYPE_C_VOLATILE:
        case ORIONPP_TYPE_C_RESTRICT:
        case ORIONPP_TYPE_C_ATOMIC:
        case ORIONPP_TYPE_C_REGISTER:
          val->val = arena_alloc(arena, sizeof(orionpp_value_t));
          orionpp_parse_value(arena, header, buf, (orionpp_value_t*)val->val); // qualifiers are followed by another value
          val->valsize = sizeof(orionpp_ftype_t);
          break;
          
        // Complex types would need special handling
        case ORIONPP_TYPE_C_COMPLEX:
        case ORIONPP_TYPE_C_IMAGINARY:
        case ORIONPP_TYPE_C_STRUCT:
        case ORIONPP_TYPE_C_UNION:
        case ORIONPP_TYPE_C_STRUCTREF:
        case ORIONPP_TYPE_C_UNIONREF:
        case ORIONPP_TYPE_C_TYPEREF:
        case ORIONPP_TYPE_C_ARRAY:
        case ORIONPP_TYPE_C_VLARRAY:
        case ORIONPP_TYPE_C_FUNCTION:
          // These would require more complex parsing logic
          val->val = NULL;
          val->valsize = 0;
          break;
          
        case ORIONPP_TYPE_C_VOID:
          val->val = NULL;
          val->valsize = 0;
          break;
          
        case ORIONPP_TYPE_C_VARADIC:
          val->val = NULL;
          val->valsize = 0;
          break;
          
        default:
          val->val = NULL;
          val->valsize = 0;
          break;
      }
      break;
    default:
      val->val = NULL;
      val->valsize = 0;
  }
}



