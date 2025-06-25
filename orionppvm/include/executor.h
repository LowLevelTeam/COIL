/**
 * @file include/executor.h
 * @brief Orion++ instruction executor definitions
 */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "vm.h"

// Instruction execution functions
int ovm_execute_instruction(OrionVM* vm, const orinopp_instruction_t* instr);

// ISA instruction handlers
int ovm_exec_var(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_const(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_mov(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_lea(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_label(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_jmp(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_br(OrionVM* vm, const orinopp_instruction_t* instr);

// Conditional branch instructions
int ovm_exec_breq(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brneq(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brgt(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brge(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brlt(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brle(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brz(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_brnz(OrionVM* vm, const orinopp_instruction_t* instr);

// Call and return
int ovm_exec_call(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_ret(OrionVM* vm, const orinopp_instruction_t* instr);

// Arithmetic operations
int ovm_exec_add(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_sub(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_mul(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_div(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_mod(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_inc(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_dec(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_incp(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_decp(OrionVM* vm, const orinopp_instruction_t* instr);

// Bitwise operations
int ovm_exec_and(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_or(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_xor(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_not(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_shl(OrionVM* vm, const orinopp_instruction_t* instr);
int ovm_exec_shr(OrionVM* vm, const orinopp_instruction_t* instr);

// Hint processing
int ovm_exec_hint(OrionVM* vm, const orinopp_instruction_t* instr);

// Value extraction helpers
int ovm_extract_variable_id(const orinopp_value_t* value, orionpp_variable_id_t* id);
int ovm_extract_label_id(const orinopp_value_t* value, orionpp_label_id_t* id);
int ovm_extract_integer(const orinopp_value_t* value, int64_t* result);
int ovm_extract_string(const orinopp_value_t* value, char** result);

// Arithmetic helpers
int ovm_perform_binary_op(OrionVM* vm, VMVariable* dest, VMVariable* left, VMVariable* right, orionpp_opcode_module_t op);
int ovm_perform_unary_op(OrionVM* vm, VMVariable* dest, VMVariable* operand, orionpp_opcode_module_t op);

// Comparison helpers
int ovm_compare_variables(OrionVM* vm, VMVariable* left, VMVariable* right, int* result);
int ovm_branch_if_condition(OrionVM* vm, bool condition, orionpp_label_id_t label_id);

// Type conversion
int ovm_convert_value(VMVariable* dest, const VMVariable* src, orionpp_type_t target_type);
bool ovm_types_compatible(orionpp_type_t type1, orionpp_type_t type2);

#endif // EXECUTOR_H