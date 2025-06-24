#ifndef ORIONPP_INSTR_INSTR_H
#define ORIONPP_INSTR_INSTR_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/value/value.h"

// Instruction structure
struct orionpp_instruction {
  orionpp_feature_t feature;    // First opcode (feature module)
  uint8_t opcode;               // Second opcode (instruction within module)
  uint16_t flags;               // Instruction-specific flags
  
  // Instruction data based on feature and opcode
  union {
    // OBJ feature instructions
    struct {
      orionpp_section_type_t section_type;
      orionpp_section_flags_t section_flags;
      uint32_t name_offset;     // String table offset
    } obj_section;
    
    struct {
      orionpp_symbol_binding_t binding;
      orionpp_symbol_type_t symbol_type;
      uint32_t name_offset;     // String table offset
    } obj_symbol;
    
    struct {
      orionpp_value_t value;
    } obj_data;
    
    // ISA feature instructions
    struct {
      uint32_t variable_id;
    } isa_let;
    
    struct {
      orionpp_value_t dest;
      orionpp_value_t src;
    } isa_mov;
    
    struct {
      orionpp_value_t dest;
      orionpp_value_t src1;
      orionpp_value_t src2;
    } isa_binary_op; // For add, sub, mul, div, and, or, xor, shl, shr
    
    struct {
      orionpp_value_t dest;
      orionpp_value_t src;
    } isa_unary_op; // For not, neg, etc.
    
    struct {
      orionpp_value_t label;
      orionpp_value_t left;
      orionpp_value_t right;
    } isa_branch;
    
    struct {
      orionpp_value_t target;
    } isa_jump;
    
    struct {
      orionpp_value_t label;
    } isa_label;
    
    struct {
      orionpp_value_t target;
    } isa_call;
    
    // ABI feature instructions
    struct {
      orionpp_abi_type_t abi_type;  // CALLEE or CALLER
      uint32_t abi_name_offset;     // String table offset for ABI name
    } abi_declaration;
    
    struct {
      uint32_t count;
      orionpp_value_t* values;
    } abi_args_rets;
    
    // HINT feature instructions
    struct {
      uint32_t hint_data_offset;    // String table offset for hint data
    } hint_generic;
  };
};

// Instruction management
orionpp_result_t orionpp_instruction_create(orionpp_instruction_t** instruction,
                                             orionpp_feature_t feature,
                                             uint8_t opcode,
                                             const orionpp_allocator_t* allocator);

void orionpp_instruction_destroy(orionpp_instruction_t* instruction,
                                 const orionpp_allocator_t* allocator);

// Instruction validation
bool orionpp_instruction_is_valid(const orionpp_instruction_t* instruction);
bool orionpp_instruction_feature_supports_opcode(orionpp_feature_t feature, uint8_t opcode);

// Instruction utilities
const char* orionpp_instruction_get_name(orionpp_feature_t feature, uint8_t opcode);
const char* orionpp_feature_get_name(orionpp_feature_t feature);

#endif // ORIONPP_INSTR_INSTR_H