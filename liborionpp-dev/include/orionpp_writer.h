#ifndef ORIONPP_WRITER_H
#define ORIONPP_WRITER_H

#include "orionpp.h"

// Writer API for generating Orion++ files (both binary and human-readable)

// Writer context
struct OrionPPWriter {
  OrionPPContext* context;
  FILE* output;
  OrionPPFormat format;
  
  // State tracking
  bool in_function;
  uint32_t current_function_id;
  uint32_t current_scope_id;
  
  // Instruction buffer (for binary format)
  uint8_t* instructions;
  size_t instruction_size;
  size_t instruction_capacity;
  
  // Label tracking
  struct {
    char** names;
    uint32_t* offsets;
    bool* resolved;
    size_t count;
    size_t capacity;
  } labels;
  
  uint32_t label_counter;
};

// Writer lifecycle
OrionPPWriter* orionpp_writer_create(FILE* output, OrionPPFormat format);
void orionpp_writer_destroy(OrionPPWriter* writer);
OrionPPError orionpp_writer_finalize(OrionPPWriter* writer);

// High-level operations
OrionPPError orionpp_writer_begin_program(OrionPPWriter* writer);
OrionPPError orionpp_writer_end_program(OrionPPWriter* writer);

OrionPPError orionpp_writer_begin_function(OrionPPWriter* writer, const char* name,
                                          uint16_t param_count, uint8_t return_type);
OrionPPError orionpp_writer_end_function(OrionPPWriter* writer);

// Variable operations
uint32_t orionpp_writer_create_variable(OrionPPWriter* writer, OrionPPVariableType type,
                                       const char* debug_name, uint8_t data_type);
uint32_t orionpp_writer_create_temp(OrionPPWriter* writer, uint8_t data_type);

// Symbol operations  
uint32_t orionpp_writer_add_symbol(OrionPPWriter* writer, const char* name, uint8_t type,
                                  uint8_t visibility, uint16_t section, uint32_t value, uint32_t size);

// Label operations
const char* orionpp_writer_create_label(OrionPPWriter* writer);
OrionPPError orionpp_writer_place_label(OrionPPWriter* writer, const char* label);

// Instruction emission
OrionPPError orionpp_writer_emit_nop(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_enter(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_leave(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_ret(OrionPPWriter* writer);

// Variable operations
OrionPPError orionpp_writer_emit_let(OrionPPWriter* writer, uint32_t dest, uint32_t src);
OrionPPError orionpp_writer_emit_const(OrionPPWriter* writer, uint32_t dest, int32_t value);
OrionPPError orionpp_writer_emit_mov(OrionPPWriter* writer, uint32_t dest, uint32_t src);

// Arithmetic operations
OrionPPError orionpp_writer_emit_add(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_sub(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_mul(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_div(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_mod(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_neg(OrionPPWriter* writer, uint32_t dest, uint32_t src);

// Bitwise operations
OrionPPError orionpp_writer_emit_and(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_or(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_xor(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_not(OrionPPWriter* writer, uint32_t dest, uint32_t src);
OrionPPError orionpp_writer_emit_shl(OrionPPWriter* writer, uint32_t dest, uint32_t src, uint32_t count);
OrionPPError orionpp_writer_emit_shr(OrionPPWriter* writer, uint32_t dest, uint32_t src, uint32_t count);

// Control flow
OrionPPError orionpp_writer_emit_jmp(OrionPPWriter* writer, const char* label);
OrionPPError orionpp_writer_emit_call(OrionPPWriter* writer, const char* symbol);
OrionPPError orionpp_writer_emit_beq(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_bne(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_blt(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_ble(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_bgt(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_bge(OrionPPWriter* writer, uint32_t src1, uint32_t src2, const char* label);
OrionPPError orionpp_writer_emit_beqz(OrionPPWriter* writer, uint32_t src, const char* label);
OrionPPError orionpp_writer_emit_bnez(OrionPPWriter* writer, uint32_t src, const char* label);

// ABI operations
OrionPPError orionpp_writer_emit_abi_callee_enter(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_abi_callee_leave(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_abi_callee_getarg(OrionPPWriter* writer, uint32_t dest, int32_t index);
OrionPPError orionpp_writer_emit_abi_callee_setret(OrionPPWriter* writer, uint32_t src, int32_t index);
OrionPPError orionpp_writer_emit_abi_caller_setup(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_abi_caller_cleanup(OrionPPWriter* writer);
OrionPPError orionpp_writer_emit_abi_caller_setarg(OrionPPWriter* writer, uint32_t src, int32_t index);
OrionPPError orionpp_writer_emit_abi_caller_getret(OrionPPWriter* writer, uint32_t dest, int32_t index);

// Human format specific (metadata)
OrionPPError orionpp_writer_emit_comment(OrionPPWriter* writer, const char* comment);
OrionPPError orionpp_writer_emit_section(OrionPPWriter* writer, const char* type, const char* name);
OrionPPError orionpp_writer_emit_hint_abi(OrionPPWriter* writer, const char* abi);
OrionPPError orionpp_writer_emit_hint_args(OrionPPWriter* writer, const char* args);
OrionPPError orionpp_writer_emit_hint_rets(OrionPPWriter* writer, const char* rets);
OrionPPError orionpp_writer_emit_hint_endfunc(OrionPPWriter* writer);

// Utility functions
const char* orionpp_writer_variable_to_string(OrionPPWriter* writer, uint32_t var_id);
const char* orionpp_writer_format_symbol(OrionPPWriter* writer, const char* name);

#endif // ORIONPP_WRITER_H