#ifndef ORIONPP_INSTRUCTION_H
#define ORIONPP_INSTRUCTION_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

/**
 * @file instruction.h
 * @brief Orion++ instruction set definitions and operations
 * 
 * Defines the instruction format, opcodes, type system, and provides
 * functions for instruction manipulation, formatting, and I/O operations.
 */

// Forward declaration for arena
struct orionpp_arena;

// Platform-specific file handle
#ifdef WIN32
  #include <windows.h>
  typedef HANDLE orionpp_file_t;
#else
  typedef int orionpp_file_t;
#endif

// Opcode definitions
typedef uint8_t orionpp_opcode_root_t;
typedef uint8_t orionpp_opcode_module_t;

enum orionpp_opcode_root {
  ORIONPP_OPCODE_ISA
};

enum orionpp_opcode_isa {
  ORIONPP_OP_ISA_NOP, ORIONPP_OP_ISA_LET, ORIONPP_OP_ISA_CONST, ORIONPP_OP_ISA_MOV,
  ORIONPP_OP_ISA_LEA, ORIONPP_OP_ISA_JMP, ORIONPP_OP_ISA_BREQ, ORIONPP_OP_ISA_BRNEQ,
  ORIONPP_OP_ISA_BRGT, ORIONPP_OP_ISA_BRGE, ORIONPP_OP_ISA_BRLT, ORIONPP_OP_ISA_BRLE,
  ORIONPP_OP_ISA_BRZ, ORIONPP_OP_ISA_BRNZ, ORIONPP_OP_ISA_CALL, ORIONPP_OP_ISA_RET,
  ORIONPP_OP_ISA_LABEL, ORIONPP_OP_ISA_SCOPE, ORIONPP_OP_ISA_SCOPL, ORIONPP_OP_ISA_ADD,
  ORIONPP_OP_ISA_SUB, ORIONPP_OP_ISA_MUL, ORIONPP_OP_ISA_DIV, ORIONPP_OP_ISA_MOD,
  ORIONPP_OP_ISA_INC, ORIONPP_OP_ISA_DEC, ORIONPP_OP_ISA_INCp, ORIONPP_OP_ISA_DECp,
  ORIONPP_OP_ISA_AND, ORIONPP_OP_ISA_OR, ORIONPP_OP_ISA_XOR, ORIONPP_OP_ISA_NOT,
  ORIONPP_OP_ISA_SHL, ORIONPP_OP_ISA_SHR
};

// Type system definitions
typedef uint8_t orionpp_type_root_t;
typedef uint8_t orionpp_type_module_t;

enum orionpp_opcode_type {
  ORIONPP_TYPE_QUAL,
  ORIONPP_TYPE_INT
};

enum orionpp_opcode_qual {
  ORIONPP_TYPE_CONST,
  ORIONPP_TYPE_VOLATILE,
  ORIONPP_TYPE_PTR
};

enum orionpp_opcode_int {
  ORIONPP_TYPE_INT8, ORIONPP_TYPE_INT16, ORIONPP_TYPE_INT32, ORIONPP_TYPE_INT64,
  ORIONPP_TYPE_UNT8, ORIONPP_TYPE_UNT16, ORIONPP_TYPE_UNT32, ORIONPP_TYPE_UNT64
};

// Core data structures
typedef struct orionpp_type_raw {
  orionpp_type_root_t root;
  orionpp_type_module_t module_;
} orionpp_type_raw_t;

typedef struct orionpp_type {
  orionpp_type_raw_t base;
  size_t count;
  struct orionpp_type *types;
} orionpp_type_t;

typedef struct orionpp_opcode {
  orionpp_opcode_root_t root;
  orionpp_opcode_module_t module_;
} orionpp_opcode_t;

typedef struct orionpp_value {
  orionpp_type_t type;
  void *value;
  size_t value_byte_size;
} orionpp_value_t;

typedef struct orionpp_instruction {
  orionpp_opcode_t opcode;
  size_t value_count;
  orionpp_value_t *values;
} orionpp_instruction_t;

// Core instruction operations
orionpp_error_t orionpp_instruction_init(orionpp_instruction_t *instr);
orionpp_error_t orionpp_instruction_destroy(orionpp_instruction_t *instr);
orionpp_error_t orionpp_instruction_copy(orionpp_instruction_t *dest, const orionpp_instruction_t *src);

// Formatting and debug functions
size_t orionpp_string_opcode(char *buf, size_t bufsize, const orionpp_opcode_t *opcode);
size_t orionpp_string_type(char *buf, size_t bufsize, const orionpp_type_t *type);
size_t orionpp_string_value(char *buf, size_t bufsize, const orionpp_value_t *value);
size_t orionpp_string_instr(char *buf, size_t bufsize, const orionpp_instruction_t *instr);

void orionpp_print_opcode(const orionpp_opcode_t *opcode);
void orionpp_print_type(const orionpp_type_t *type);
void orionpp_print_value(const orionpp_value_t *value);
void orionpp_print_instr(const orionpp_instruction_t *instr);

// Serialization functions
orionpp_error_t orionpp_serialize_instr(char *buf, size_t bufsize, const orionpp_instruction_t *src);
orionpp_error_t orionpp_deserialize_instr(const char *buf, size_t bufsize, orionpp_instruction_t *dest);

// File I/O functions
orionpp_error_t orionpp_read_file(orionpp_file_t file, orionpp_instruction_t *dest);
orionpp_error_t orionpp_write_file(orionpp_file_t file, const orionpp_instruction_t *src);

// Arena I/O functions
orionpp_error_t orionpp_read_arena(struct orionpp_arena *arena, orionpp_instruction_t *dest);
orionpp_error_t orionpp_write_arena(struct orionpp_arena *arena, const orionpp_instruction_t *src);

#endif // ORIONPP_INSTRUCTION_H