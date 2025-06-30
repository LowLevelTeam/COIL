#ifndef __ORIONPP_INSTRUCTION
#define __ORIONPP_INSTRUCTION

#include <stdint.h>
#include <orionpp/arena.h>

// -------------------------------- ENUMS -------------------------------- //

enum orionpp_opcode_root {
  ORIONPP_OPCODE_ISA,
};

enum orionpp_opcode_isa {
  ORIONPP_OP_ISA_LET, // var i {= {value}}
  ORIONPP_OP_ISA_CONST, // const i {= {value}}
  ORIONPP_OP_ISA_MOV, // i = i
  ORIONPP_OP_ISA_LEA, // i = &i
  ORIONPP_OP_ISA_JMP, // uncondtional jump to target
  ORIONPP_OP_ISA_BREQ, // if (x == x)
  ORIONPP_OP_ISA_BRNEQ, // if (x != x)
  ORIONPP_OP_ISA_BRGT, // if (x > y)
  ORIONPP_OP_ISA_BRGE, // if (x >= y) 
  ORIONPP_OP_ISA_BRLT, // if (x < y)
  ORIONPP_OP_ISA_BRLE, // if (x <= y)
  ORIONPP_OP_ISA_BRZ, // if (!x)
  ORIONPP_OP_ISA_BRNZ, // if (x)
  ORIONPP_OP_ISA_CALL, // uncondtional jump to target and save a return address
  ORIONPP_OP_ISA_RET, // return to the address saved by call instruction
  ORIONPP_OP_ISA_LABEL, // can only be used in control flow of a child or the same scope
  ORIONPP_OP_ISA_SCOPE, // enter a scope
  ORIONPP_OP_ISA_SCOPL, // leave a scope 
  ORIONPP_OP_ISA_ADD, // i + i
  ORIONPP_OP_ISA_SUB, // i - i
  ORIONPP_OP_ISA_MUL, // i * i
  ORIONPP_OP_ISA_DIV, // i / i
  ORIONPP_OP_ISA_MOD, // i % i
  ORIONPP_OP_ISA_INC, // ++i
  ORIONPP_OP_ISA_DEC, // --i
  ORIONPP_OP_ISA_INCp, // i++
  ORIONPP_OP_ISA_DECp, // i--
  ORIONPP_OP_ISA_AND, // i & i
  ORIONPP_OP_ISA_OR,  // i | i
  ORIONPP_OP_ISA_XOR, // i ^ i
  ORIONPP_OP_ISA_NOT, // ~i
  ORIONPP_OP_ISA_SHL, // i << i
  ORIONPP_OP_ISA_SHR, // i >> i
};

enum orionpp_opcode_type {
  ORIONPP_TYPE_QUAL,
  ORIONPP_TYPE_INT,
};

enum orionpp_opcode_qual {
  ORIONPP_TYPE_CONST,
  ORIONPP_TYPE_VOLATILE,
  ORIONPP_TYPE_PTR,
};

enum orionpp_opcode_int {
  // basics
  ORIONPP_TYPE_INT8,
  ORIONPP_TYPE_INT16,
  ORIONPP_TYPE_INT32,
  ORIONPP_TYPE_INT64,
  ORIONPP_TYPE_UNT8,
  ORIONPP_TYPE_UNT16,
  ORIONPP_TYPE_UNT32,
  ORIONPP_TYPE_UNT64,
};

// -------------------------------- TYPES -------------------------------- //

#ifdef WIN32
  #include <windows.h>
  typedef HANDLE file_handle_t;
#else
  typedef int file_handle_t;
#endif

typedef uint8_t orionpp_opcode_root_t;
typedef uint8_t orionpp_opcode_module_t;

typedef uint8_t orionpp_type_root_t;
typedef uint8_t orionpp_type_module_t;

// -------------------------------- STRUCTS -------------------------------- //

typedef struct orionpp_type_raw {
  orionpp_type_root_t root;
  orionpp_type_module_t module_; // module is a C++ keyword
} orionpp_type_raw_t;

typedef struct orionpp_type {
  orionpp_type_raw_t base;
  size_t count; // count of types
  struct orionpp_type *types; // optional array of types 
} orionpp_type_t;

typedef struct orionpp_opcode {
  orionpp_opcode_root_t root;
  orionpp_opcode_module_t module_; // module is a C++ keyword
} orionpp_opcode_t;

typedef struct orionpp_value {
  orionpp_type_t type;
  void *value; size_t value_byte_size;
} orionpp_value_t;

typedef struct orinopp_instruction {
  orionpp_opcode_t opcode;
  size_t value_count;
  orionpp_value_t *values;
} orinopp_instruction_t;

// -------------------------------- FUNCS -------------------------------- //

// Debug Helpers
// format ROOT.MODULE
size_t orionpp_string_opcode(char *buf, size_t bufsize, orionpp_opcode_t*); // 0 on error or the size of the string >0
// format TYPE, TYPE<TYPE> // NOT IMPLEMENTED YET(TYPE[x], TYPE{ TYPE, TYPE })
size_t orionpp_string_type(char *buf, size_t bufsize, orionpp_type_t*);
// VALUE: TYPE
size_t orionpp_string_value(char *buf, size_t bufsize, orionpp_value_t*);
// ROOT.MODULE VALUE: TYPE, ...
size_t orionpp_string_instr(char *buf, size_t bufsize, orinopp_instruction_t*);
void orionpp_print_opcode(orionpp_opcode_t*);
void orionpp_print_type(orionpp_type_t*);
void orionpp_print_value(orionpp_value_t*);
void orionpp_print_instr(orinopp_instruction_t*);

// Implementation Helpers
orionpp_error_t orionpp_readf(file_handle_t file, orinopp_instruction_t *dest);
orionpp_error_t orionpp_writef(file_handle_t file, const orinopp_instruction_t *src);
int orionpp_readarena(orionpp_arena_t *arena, orinopp_instruction_t *dest);
int orionpp_writearena(const orionpp_arena_t *arena, const orinopp_instruction_t *src);
orionpp_error_t orionpp_readbuf(const char *buf, size_t bufsize, orinopp_instruction_t *dest);
orionpp_error_t orionpp_writebuf(char *buf, size_t bufsize, const orinopp_instruction_t *src);

#endif // __ORIONPP_INSTRUCTION