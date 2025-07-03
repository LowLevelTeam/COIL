/**
* @file code.h
* @brief Orion++ instruction set
*/

#ifndef ORIONPP_CODE_H
#define ORIONPP_CODE_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/detail.h>

// -------------------------------- Opcode -------------------------------- //

typedef uint8_t orionpp_opcode_root_t;
typedef uint8_t orionpp_opcode_module_t;

enum orionpp_opcode_root {
  ORIONPP_OPCODE_ISA, // standard instructions
  ORIONPP_OPCODE_ABI, // ABI specific instructions
};

enum orionpp_opcode_isa {
  // Control flow
  ORIONPP_OP_ISA_NOP,     // No operation
  ORIONPP_OP_ISA_JMP,     // Unconditional jump
  ORIONPP_OP_ISA_CALL,    // Function call
  ORIONPP_OP_ISA_RET,     // Return from function
  
  // Conditional branches
  ORIONPP_OP_ISA_BREQ,    // Branch if equal
  ORIONPP_OP_ISA_BRNEQ,   // Branch if not equal  
  ORIONPP_OP_ISA_BRGT,    // Branch if greater than
  ORIONPP_OP_ISA_BRGE,    // Branch if greater than or equal
  ORIONPP_OP_ISA_BRLT,    // Branch if less than
  ORIONPP_OP_ISA_BRLE,    // Branch if less than or equal
  ORIONPP_OP_ISA_BRZ,     // Branch if zero
  ORIONPP_OP_ISA_BRNZ,    // Branch if not zero
  
  // Memory operations
  ORIONPP_OP_ISA_LET,     // Define variable
  ORIONPP_OP_ISA_CONST,   // Define constant
  ORIONPP_OP_ISA_MOV,     // Move/copy value
  ORIONPP_OP_ISA_LEA,     // Load effective address
  
  // Arithmetic operations
  ORIONPP_OP_ISA_ADD,     // Addition
  ORIONPP_OP_ISA_SUB,     // Subtraction
  ORIONPP_OP_ISA_MUL,     // Multiplication
  ORIONPP_OP_ISA_DIV,     // Division
  ORIONPP_OP_ISA_MOD,     // Modulo
  ORIONPP_OP_ISA_INC,     // Increment
  ORIONPP_OP_ISA_DEC,     // Decrement
  ORIONPP_OP_ISA_INCp,    // Post-increment
  ORIONPP_OP_ISA_DECp,    // Post-decrement
  
  // Bitwise operations
  ORIONPP_OP_ISA_AND,     // Bitwise AND
  ORIONPP_OP_ISA_OR,      // Bitwise OR
  ORIONPP_OP_ISA_XOR,     // Bitwise XOR
  ORIONPP_OP_ISA_NOT,     // Bitwise NOT
  ORIONPP_OP_ISA_SHL,     // Shift left
  ORIONPP_OP_ISA_SHR,     // Shift right
  
  // Scope management
  ORIONPP_OP_ISA_LABEL,   // Label definition
  ORIONPP_OP_ISA_SCOPE,   // Enter scope
  ORIONPP_OP_ISA_SCOPL,   // Leave scope
  
  // Target specification
  ORIONPP_OP_ISA_TARGET   // Target architecture specification
};

enum orionpp_opcode_abi {
  ORIONPP_OPCODE_ABI_CALLEE_SETUP, // Setup callee environment
  ORIONPP_OPCODE_ABI_CALLEE_CLEANUP, // Cleanup callee environment
  ORIONPP_OPCODE_ABI_CALLER_SETUP, // Setup callee environment
  ORIONPP_OPCODE_ABI_CALLER_CLEANUP, // Cleanup callee environment
  ORIONPP_OPCODE_ABI_CALLEE_ARG, // Get parameter
  ORIONPP_OPCODE_ABI_CALLEE_VARG, // Get varadic parameter
  ORIONPP_OPCODE_ABI_CALLER_ARG, // Set argument
  ORIONPP_OPCODE_ABI_CALLER_VARG, // Add varadic argument
  ORIONPP_OPCODE_ABI_CALLEE_RET, // Set return value
  ORIONPP_OPCODE_ABI_CALLER_RET, // Get return value
};

typedef struct orionpp_opcode {
  orionpp_opcode_root_t root;
  orionpp_opcode_module_t module;
} orionpp_opcode_t;

// -------------------------------- Instructions -------------------------------- //

enum orionpp_kind {
  ORIONPP_KIND_NONE = 0,  // No Value
  ORIONPP_KIND_IMMEDIATE, // Immediate value
  ORIONPP_KIND_VARIABLE,  // Variable reference
  ORIONPP_KIND_LABEL,     // Label reference
  ORIONPP_KIND_DATA,      // Data reference
  ORIONPP_KIND_FUNC,      // Function reference
};
typedef uint8_t orionpp_kind_t;

enum orionpp_fmtkind {
  ORIONPP_FMT_ERR = 0,  // Invalid Opcode
  ORIONPP_FMT_NULL,     // Null instruction format
  ORIONPP_FMT_DEF,      // Definition instruction format
  ORIONPP_FMT_UNARY,    // Unary instruction format
  ORIONPP_FMT_BINARY,   // Binary instruction format
  ORIONPP_FMT_TENARY,   // Ternary instruction format
};
typedef uint8_t orionpp_fmtkind_t;

typedef struct orionpp_value {
  orionpp_kind_t kind; // Type of value
  union {
    struct { orionpp_typeref_t type; void *value; } immediate; // Immediate value (pointer to data)
    orionpp_varref_t variable; // Variable reference
    orionpp_labelref_t label; // Label reference
    orionpp_dataref_t data; // Data reference
    orionpp_funcref_t func; // Function reference
  } data;
} orionpp_value_t;

typedef struct orionpp_instrfmt_null {
  orionpp_opcode_t opcode; // Opcode for the instruction
} orionpp_instrfmt_null_t;

typedef struct orionpp_instfmt_def {
  orionpp_opcode_t opcode; // Opcode for the instruction
  orionpp_varref_t id; // Variable identifier
  orionpp_typeref_t type; // Type of the variable
  orionpp_value_t value; // Value associated with the instruction
} orionpp_instfmt_def_t;

typedef struct orionpp_instrfmt_unary {
  orionpp_opcode_t opcode; // Opcode for the instruction
  orionpp_value_t argument; // Single argument for the instruction
} orionpp_instrfmt_unary_t;

typedef struct orionpp_instrfmt_binary {
  orionpp_opcode_t opcode; // Opcode for the instruction
  orionpp_value_t arguments[2]; // Two arguments for the instruction
} orionpp_instrfmt_binary_t;

typedef struct orionpp_instrfmt_tenary {
  orionpp_opcode_t opcode; // Opcode for the instruction
  orionpp_value_t arguments[3]; // Three arguments for the instruction
} orionpp_instrfmt_tenary_t;

typedef union orionpp_instrfmt {
  orionpp_instrfmt_null_t null; // Null instruction format
  orionpp_instfmt_def_t def; // Definition instruction format
  orionpp_instrfmt_unary_t unary; // Unary instruction format
  orionpp_instrfmt_binary_t binary; // Binary instruction format
  orionpp_instrfmt_tenary_t tenary; // Ternary instruction format
} orionpp_instrfmt_t;

// -------------------------------- Function -------------------------------- //

// each function has a unique identifier, ABI, return type, argument types, and instructions
// they are used to create seperate translation units for optimization and linking
// a function can be assigned an optional name like "main" or "printf" to help linking
typedef struct orionpp_func {
  orionpp_nameref_t name; // Name of the function
  orionpp_funcref_t id; // Function identifier
  orionpp_abi_t abi; // ABI used by the function
  uint16_t ret_count; // Number of return values
  uint16_t arg_count; // Number of arguments
  orionpp_typeref_t *return_type; // Return type of the function
  orionpp_typeref_t *arg_types; // Array of argument types
  orionpp_instrfmt_t *instructions; // Array of instructions for the function
} orionpp_func_t;

// -------------------------------- API Functions -------------------------------- //

// the format can be determined by the opcode and the kind of instruction
orionpp_fmtkind_t orionpp_getfmtkind(orionpp_opcode_t opcode);

#endif // ORIONPP_CODE_H