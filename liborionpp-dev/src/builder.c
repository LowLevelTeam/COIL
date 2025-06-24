#include "orionpp/instr/builder.h"
#include "orionpp/string/strtab.h"
#include <string.h>

orionpp_result_t orionpp_instruction_set_obj_section(orionpp_instruction_t* instruction,
                                                      orionpp_section_type_t type,
                                                      orionpp_section_flags_t flags,
                                                      uint32_t name_offset) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_OBJ || instruction->opcode != ORIONPP_OBJ_SECTION) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->obj_section.section_type = type;
  instruction->obj_section.section_flags = flags;
  instruction->obj_section.name_offset = name_offset;
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_obj_symbol(orionpp_instruction_t* instruction,
                                                     orionpp_symbol_binding_t binding,
                                                     orionpp_symbol_type_t type,
                                                     uint32_t name_offset) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_OBJ || instruction->opcode != ORIONPP_OBJ_SYMBOL) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->obj_symbol.binding = binding;
  instruction->obj_symbol.symbol_type = type;
  instruction->obj_symbol.name_offset = name_offset;
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_obj_data(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* value) {
  if (!instruction || !value) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_OBJ) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  // Validate opcode is a data instruction
  switch (instruction->opcode) {
    case ORIONPP_OBJ_BYTE:
    case ORIONPP_OBJ_WORD:
    case ORIONPP_OBJ_DWORD:
    case ORIONPP_OBJ_QWORD:
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->obj_data.value = *value;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_let(orionpp_instruction_t* instruction,
                                                  uint32_t variable_id) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA || instruction->opcode != ORIONPP_ISA_LET) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_let.variable_id = variable_id;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_mov(orionpp_instruction_t* instruction,
                                                  const orionpp_value_t* dest,
                                                  const orionpp_value_t* src) {
  if (!instruction || !dest || !src) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA || instruction->opcode != ORIONPP_ISA_MOV) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_mov.dest = *dest;
  instruction->isa_mov.src = *src;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_binary_op(orionpp_instruction_t* instruction,
                                                        const orionpp_value_t* dest,
                                                        const orionpp_value_t* src1,
                                                        const orionpp_value_t* src2) {
  if (!instruction || !dest || !src1 || !src2) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  // Validate opcode is a binary operation
  switch (instruction->opcode) {
    case ORIONPP_ISA_ADD:
    case ORIONPP_ISA_SUB:
    case ORIONPP_ISA_MUL:
    case ORIONPP_ISA_DIV:
    case ORIONPP_ISA_AND:
    case ORIONPP_ISA_OR:
    case ORIONPP_ISA_XOR:
    case ORIONPP_ISA_SHL:
    case ORIONPP_ISA_SHR:
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_binary_op.dest = *dest;
  instruction->isa_binary_op.src1 = *src1;
  instruction->isa_binary_op.src2 = *src2;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_unary_op(orionpp_instruction_t* instruction,
                                                       const orionpp_value_t* dest,
                                                       const orionpp_value_t* src) {
  if (!instruction || !dest || !src) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  // Validate opcode is a unary operation
  switch (instruction->opcode) {
    case ORIONPP_ISA_NOT:
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_unary_op.dest = *dest;
  instruction->isa_unary_op.src = *src;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_branch(orionpp_instruction_t* instruction,
                                                     const orionpp_value_t* label,
                                                     const orionpp_value_t* left,
                                                     const orionpp_value_t* right) {
  if (!instruction || !label || !left || !right) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  // Validate opcode is a branch operation
  switch (instruction->opcode) {
    case ORIONPP_ISA_BR_EQ:
    case ORIONPP_ISA_BR_NE:
    case ORIONPP_ISA_BR_LT:
    case ORIONPP_ISA_BR_LE:
    case ORIONPP_ISA_BR_GT:
    case ORIONPP_ISA_BR_GE:
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_branch.label = *label;
  instruction->isa_branch.left = *left;
  instruction->isa_branch.right = *right;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_jump(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* target) {
  if (!instruction || !target) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA || instruction->opcode != ORIONPP_ISA_JMP) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_jump.target = *target;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_label(orionpp_instruction_t* instruction,
                                                    const orionpp_value_t* label) {
  if (!instruction || !label) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA || instruction->opcode != ORIONPP_ISA_LABEL) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_label.label = *label;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_isa_call(orionpp_instruction_t* instruction,
                                                   const orionpp_value_t* target) {
  if (!instruction || !target) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ISA || instruction->opcode != ORIONPP_ISA_CALL) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->isa_call.target = *target;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_abi_declaration(orionpp_instruction_t* instruction,
                                                          orionpp_abi_type_t type,
                                                          uint32_t abi_name_offset) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_ABI) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  if (instruction->opcode != ORIONPP_ABI_CALLEE && instruction->opcode != ORIONPP_ABI_CALLER) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  instruction->abi_declaration.abi_type = type;
  instruction->abi_declaration.abi_name_offset = abi_name_offset;
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_abi_args_rets(orionpp_instruction_t* instruction,
                                                        orionpp_value_t* values,
                                                        uint32_t count,
                                                        const orionpp_allocator_t* allocator) {
  if (!instruction || (!values && count > 0)) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  if (instruction->feature != ORIONPP_FEATURE_ABI) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  if (instruction->opcode != ORIONPP_ABI_ARGS && instruction->opcode != ORIONPP_ABI_RETS) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  // Free existing values if any
  if (instruction->abi_args_rets.values) {
    for (uint32_t i = 0; i < instruction->abi_args_rets.count; i++) {
      orionpp_value_free(&instruction->abi_args_rets.values[i], allocator);
    }
    allocator->free(instruction->abi_args_rets.values);
  }
  
  if (count == 0) {
    instruction->abi_args_rets.values = NULL;
    instruction->abi_args_rets.count = 0;
    return ORIONPP_OK_INT(0);
  }
  
  // Allocate and copy values
  orionpp_value_t* new_values = allocator->malloc(sizeof(orionpp_value_t) * count);
  if (!new_values) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  for (uint32_t i = 0; i < count; i++) {
    orionpp_result_t result = orionpp_value_copy(&new_values[i], &values[i], allocator);
    if (ORIONPP_IS_ERROR(result)) {
      // Clean up partial array
      for (uint32_t j = 0; j < i; j++) {
        orionpp_value_free(&new_values[j], allocator);
      }
      allocator->free(new_values);
      return result;
    }
  }
  
  instruction->abi_args_rets.values = new_values;
  instruction->abi_args_rets.count = count;
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_set_hint_generic(orionpp_instruction_t* instruction,
                                                       uint32_t hint_data_offset) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (instruction->feature != ORIONPP_FEATURE_HINT) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  instruction->hint_generic.hint_data_offset = hint_data_offset;
  return ORIONPP_OK_INT(0);
}

// High-level builder functions
orionpp_result_t orionpp_instruction_build_obj_section(orionpp_instruction_t** instruction,
                                                        orionpp_section_type_t type,
                                                        orionpp_section_flags_t flags,
                                                        const char* name,
                                                        orionpp_string_table_t* string_table,
                                                        const orionpp_allocator_t* allocator) {
  if (!instruction || !name || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_instruction_t* inst;
  orionpp_result_t result = orionpp_instruction_create(&inst, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  uint32_t name_offset;
  result = orionpp_string_table_add(string_table, name, &name_offset);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  result = orionpp_instruction_set_obj_section(inst, type, flags, name_offset);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_instruction_build_obj_symbol(orionpp_instruction_t** instruction,
                                                       orionpp_symbol_binding_t binding,
                                                       orionpp_symbol_type_t type,
                                                       const char* name,
                                                       orionpp_string_table_t* string_table,
                                                       const orionpp_allocator_t* allocator) {
  if (!instruction || !name || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_instruction_t* inst;
  orionpp_result_t result = orionpp_instruction_create(&inst, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SYMBOL, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  uint32_t name_offset;
  result = orionpp_string_table_add(string_table, name, &name_offset);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  result = orionpp_instruction_set_obj_symbol(inst, binding, type, name_offset);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_instruction_build_isa_mov_var_const(orionpp_instruction_t** instruction,
                                                             uint32_t dest_var,
                                                             uint64_t const_value,
                                                             const orionpp_allocator_t* allocator) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_instruction_t* inst;
  orionpp_result_t result = orionpp_instruction_create(&inst, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MOV, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  orionpp_value_t dest = orionpp_value_variable(dest_var);
  orionpp_value_t src = orionpp_value_numeric(ORIONPP_BASE_DECIMAL, const_value);
  
  result = orionpp_instruction_set_isa_mov(inst, &dest, &src);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_instruction_build_isa_add_vars(orionpp_instruction_t** instruction,
                                                         uint32_t dest_var,
                                                         uint32_t src1_var,
                                                         uint32_t src2_var,
                                                         const orionpp_allocator_t* allocator) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_instruction_t* inst;
  orionpp_result_t result = orionpp_instruction_create(&inst, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  orionpp_value_t dest = orionpp_value_variable(dest_var);
  orionpp_value_t src1 = orionpp_value_variable(src1_var);
  orionpp_value_t src2 = orionpp_value_variable(src2_var);
  
  result = orionpp_instruction_set_isa_binary_op(inst, &dest, &src1, &src2);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_instruction_destroy(inst, allocator);
    return result;
  }
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}