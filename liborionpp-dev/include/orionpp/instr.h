#ifndef ORIONPP_INSTRUCTION_H
#define ORIONPP_INSTRUCTION_H

#include "orionpp/types.h"
#include "orionpp/strtab.h"

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

// Instruction data setters (type-safe)
orionpp_result_t orionpp_instruction_set_obj_section(orionpp_instruction_t* instruction,
                                                      orionpp_section_type_t type,
                                                      orionpp_section_flags_t flags,
                                                      uint32_t name_offset);

orionpp_result_t orionpp_instruction_set_obj_symbol(orionpp_instruction_t* instruction,
                                                     orionpp_symbol_binding_t binding,
                                                     orionpp_symbol_type_t type,
                                                     uint32_t name_offset);

orionpp_result_t orionpp_instruction_set_isa_let(orionpp_instruction_t* instruction,
                                                  uint32_t variable_id);

orionpp_result_t orionpp_instruction_set_isa_mov(orionpp_instruction_t* instruction,
                                                  const orionpp_value_t* dest,
                                                  const orionpp_value_t* src);

orionpp_result_t orionpp_instruction_set_isa_binary_op(orionpp_instruction_t* instruction,
                                                        const orionpp_value_t* dest,
                                                        const orionpp_value_t* src1,
                                                        const orionpp_value_t* src2);

orionpp_result_t orionpp_instruction_set_abi_declaration(orionpp_instruction_t* instruction,
                                                          orionpp_abi_type_t type,
                                                          uint32_t abi_name_offset);

orionpp_result_t orionpp_instruction_set_abi_args_rets(orionpp_instruction_t* instruction,
                                                        orionpp_value_t* values,
                                                        uint32_t count,
                                                        const orionpp_allocator_t* allocator);

// Instruction serialization/deserialization
orionpp_result_t orionpp_instruction_write_binary(const orionpp_instruction_t* instruction,
                                                   FILE* file);

orionpp_result_t orionpp_instruction_read_binary(orionpp_instruction_t** instruction,
                                                  FILE* file,
                                                  const orionpp_allocator_t* allocator);

// Text format support
orionpp_result_t orionpp_instruction_write_text(const orionpp_instruction_t* instruction,
                                                 const orionpp_string_table_t* string_table,
                                                 FILE* file);

// Instruction introspection
const char* orionpp_instruction_get_name(orionpp_feature_t feature, uint8_t opcode);
const char* orionpp_feature_get_name(orionpp_feature_t feature);

#endif // ORIONPP_INSTRUCTION_H