/**
* @file instrdef.h
* @brief Orion++ instruction set
*/

#ifndef ORIONPP_CODE_H
#define ORIONPP_CODE_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

typedef uint64_t orionpp_typeref_t;

// -------------------------------- Opcode -------------------------------- //
typedef uint8_t orionpp_opcode_root_t;
typedef uint8_t orionpp_opcode_module_t;

enum orionpp_opcode_root {
  ORIONPP_OPCODE_ISA, // standard instructions
  // instructions down here will be mostly related around features and ISA extensions
  // there will not be instructions like vecadd etc... as the instructions are type determined
  // meaning a vector extension will only come with instructions like indexing and extracting plus
  // maybe some masking for control flow but will work with standard ISA instructions for the most part.
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
  ORIONPP_OP_ISA_SHL, ORIONPP_OP_ISA_SHR, ORIONPP_OP_ISA_TARGET
};

typedef struct orionpp_opcode {
  orionpp_opcode_root_t root;
  orionpp_opcode_module_t module_;
} orionpp_opcode_t;

typedef struct orionpp_argument {
  orionpp_typeref_t type;
  void *value;
} orionpp_argument_t;

typedef struct orionpp_instruction_define {
  orionpp_opcode_t opcode;
  uint64_t id; // the id to initalize this reference under
  orionpp_argument_t argument;
} orionpp_instruction_unary_t;

typedef struct orionpp_instruction_unary {
  orionpp_opcode_t opcode;
  orionpp_argument_t argument;
} orionpp_instruction_unary_t;

typedef struct orionpp_instruction_binary {
  orionpp_opcode_t opcode;
  orionpp_argument_t argument[2];
} orionpp_instruction_binary_t;

typedef struct orionpp_instruction_tenary {
  orionpp_opcode_t opcode;
  orionpp_argument_t argument[3];
} orionpp_instruction_tenary_t;

typedef struct orionpp_instruction_quaternary {
  orionpp_opcode_t opcode;
  orionpp_argument_t argument[4];
} orionpp_instruction_quaternary_t;

typedef struct orionpp_instruction_varadic {
  orionpp_opcode_t opcode;
  uint16_t argument_count;
  orionpp_argument_t *argument;
} orionpp_instruction_varadic_t;

int oroinpp_opcode_validate(const orionpp_opcode_t *opcode); // ensure opcode refers to a real available instruction
uint16_t orionpp_opcode_argument_count(const orionpp_opcode_t *opcode); // find opcode expected argument count (0 if varadic)

// Instruction Format
// base: [opcode_root][opcode_module] 
// argument: [typeref][value]
// more information on typeref under ./typetab.h
// example
// [ORIONPP_OPCODE_ISA][ORIONPP_OP_ISA_CONST][u64][1][typeref -> VARREF][1]
// here a constant is defined at local scope reference 1 as its the second variable in the scope
// which is then assigned the value of the firsat variable which is an already initalized runtime variable

#endif // ORIONPP_CODE_H