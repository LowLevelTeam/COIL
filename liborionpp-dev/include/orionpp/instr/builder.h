#ifndef ORIONPP_INSTR_BUILDER_H
#define ORIONPP_INSTR_BUILDER_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/instr/instr.h"
#include "orionpp/value/value.h"

// Instruction data setters (type-safe)
orionpp_result_t orionpp_instruction_set_obj_section(orionpp_instruction_t* instruction,
                                                      orionpp_section_type_t type,
                                                      orionpp_section_flags_t flags,
                                                      uint32_t name_offset);

orionpp_result_t orionpp_instruction_set_obj_symbol(orionpp_instruction_t* instruction,
                                                     orionpp_symbol_binding_t binding,
                                                     orionpp_symbol_type_t type,
                                                     uint32_t name_offset);

orionpp_result_t orionpp_instruction_set_obj_data(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* value);

orionpp_result_t orionpp_instruction_set_isa_let(orionpp_instruction_t* instruction,
                                                  uint32_t variable_id);

orionpp_result_t orionpp_instruction_set_isa_mov(orionpp_instruction_t* instruction,
                                                  const orionpp_value_t* dest,
                                                  const orionpp_value_t* src);

orionpp_result_t orionpp_instruction_set_isa_binary_op(orionpp_instruction_t* instruction,
                                                        const orionpp_value_t* dest,
                                                        const orionpp_value_t* src1,
                                                        const orionpp_value_t* src2);

orionpp_result_t orionpp_instruction_set_isa_unary_op(orionpp_instruction_t* instruction,
                                                       const orionpp_value_t* dest,
                                                       const orionpp_value_t* src);

orionpp_result_t orionpp_instruction_set_isa_branch(orionpp_instruction_t* instruction,
                                                     const orionpp_value_t* label,
                                                     const orionpp_value_t* left,
                                                     const orionpp_value_t* right);

orionpp_result_t orionpp_instruction_set_isa_jump(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* target);

orionpp_result_t orionpp_instruction_set_isa_label(orionpp_instruction_t* instruction,
                                                    const orionpp_value_t* label);

orionpp_result_t orionpp_instruction_set_isa_call(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* target);

orionpp_result_t orionpp_instruction_set_abi_declaration(orionpp_instruction_t* instruction,
                                                          orionpp_abi_type_t type,
                                                          uint32_t abi_name_offset);

orionpp_result_t orionpp_instruction_set_abi_args_rets(orionpp_instruction_t* instruction,
                                                        orionpp_value_t* values,
                                                        uint32_t count,
                                                        const orionpp_allocator_t* allocator);

orionpp_result_t orionpp_instruction_set_hint_generic(orionpp_instruction_t* instruction,
                                                       uint32_t hint_data_offset);

// High-level builder functions
orionpp_result_t orionpp_instruction_build_obj_section(orionpp_instruction_t** instruction,
                                                        orionpp_section_type_t type,
                                                        orionpp_section_flags_t flags,
                                                        const char* name,
                                                        orionpp_string_table_t* string_table,
                                                        const orionpp_allocator_t* allocator);

orionpp_result_t orionpp_instruction_build_obj_symbol(orionpp_instruction_t** instruction,
                                                       orionpp_symbol_binding_t binding,
                                                       orionpp_symbol_type_t type,
                                                       const char* name,
                                                       orionpp_string_table_t* string_table,
                                                       const orionpp_allocator_t* allocator);

orionpp_result_t orionpp_instruction_build_isa_mov_var_const(orionpp_instruction_t** instruction,
                                                             uint32_t dest_var,
                                                             uint64_t const_value,
                                                             const orionpp_allocator_t* allocator);

orionpp_result_t orionpp_instruction_build_isa_add_vars(orionpp_instruction_t** instruction,
                                                         uint32_t dest_var,
                                                         uint32_t src1_var,
                                                         uint32_t src2_var,
                                                         const orionpp_allocator_t* allocator);

#endif // ORIONPP_INSTR_BUILDER_H