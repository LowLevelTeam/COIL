#include "orionpp/instr.h"
#include <string.h>
#include <stdlib.h>

// Instruction name tables
static const char* obj_instruction_names[] = {
  [0] = "unknown",
  [ORIONPP_OBJ_SECTION] = "obj.section",
  [ORIONPP_OBJ_SYMBOL] = "obj.sym",
  [ORIONPP_OBJ_BYTE] = ".byte",
  [ORIONPP_OBJ_WORD] = ".word",
  [ORIONPP_OBJ_DWORD] = ".dword",
  [ORIONPP_OBJ_QWORD] = ".qword",
  [ORIONPP_OBJ_ALIGN] = ".align",
};

static const char* isa_instruction_names[] = {
  [0] = "unknown",
  [ORIONPP_ISA_SCOPE_ENTER] = "isa.scope_enter",
  [ORIONPP_ISA_SCOPE_LEAVE] = "isa.scope_leave",
  [ORIONPP_ISA_LET] = "isa.let",
  [ORIONPP_ISA_MOV] = "isa.mov",
  [ORIONPP_ISA_CONST] = "isa.const",
  [ORIONPP_ISA_ADD] = "isa.add",
  [ORIONPP_ISA_SUB] = "isa.sub",
  [ORIONPP_ISA_MUL] = "isa.mul",
  [ORIONPP_ISA_DIV] = "isa.div",
  [ORIONPP_ISA_AND] = "isa.and",
  [ORIONPP_ISA_OR] = "isa.or",
  [ORIONPP_ISA_XOR] = "isa.xor",
  [ORIONPP_ISA_NOT] = "isa.not",
  [ORIONPP_ISA_SHL] = "isa.shl",
  [ORIONPP_ISA_SHR] = "isa.shr",
  [ORIONPP_ISA_CMP] = "isa.cmp",
  [ORIONPP_ISA_BR_EQ] = "isa.br.eq",
  [ORIONPP_ISA_BR_NE] = "isa.br.ne",
  [ORIONPP_ISA_BR_LT] = "isa.br.lt",
  [ORIONPP_ISA_BR_LE] = "isa.br.le",
  [ORIONPP_ISA_BR_GT] = "isa.br.gt",
  [ORIONPP_ISA_BR_GE] = "isa.br.ge",
  [ORIONPP_ISA_JMP] = "isa.jmp",
  [ORIONPP_ISA_LABEL] = "isa.label",
  [ORIONPP_ISA_CALL] = "isa.call",
  [ORIONPP_ISA_RET] = "isa.ret",
  [ORIONPP_ISA_NOP] = "isa.nop",
};

static const char* abi_instruction_names[] = {
  [0] = "unknown",
  [ORIONPP_ABI_CALLEE] = "abi.callee",
  [ORIONPP_ABI_CALLER] = "abi.caller",
  [ORIONPP_ABI_ARGS] = "abi.args",
  [ORIONPP_ABI_RETS] = "abi.rets",
  [ORIONPP_ABI_SETUP] = "abi.setup",
  [ORIONPP_ABI_CLEANUP] = "abi.cleanup",
};

static const char* hint_instruction_names[] = {
  [0] = "unknown",
  [ORIONPP_HINT_SYMEND] = "hint.symend",
  [ORIONPP_HINT_OPTIMIZE] = "hint.optimize",
  [ORIONPP_HINT_UNROLL] = "hint.unroll",
  [ORIONPP_HINT_INLINE] = "hint.inline",
  [ORIONPP_HINT_NOINLINE] = "hint.noinline",
};

static const char* feature_names[] = {
  [ORIONPP_FEATURE_NONE] = "none",
  [ORIONPP_FEATURE_OBJ] = "OBJ",
  [ORIONPP_FEATURE_ISA] = "ISA",
  [ORIONPP_FEATURE_ABI] = "ABI",
  [ORIONPP_FEATURE_HINT] = "HINT",
};

orionpp_result_t orionpp_instruction_create(orionpp_instruction_t** instruction,
                                             orionpp_feature_t feature,
                                             uint8_t opcode,
                                             const orionpp_allocator_t* allocator) {
  if (!instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  if (!orionpp_instruction_feature_supports_opcode(feature, opcode)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  orionpp_instruction_t* inst = allocator->malloc(sizeof(orionpp_instruction_t));
  if (!inst) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  memset(inst, 0, sizeof(orionpp_instruction_t));
  inst->feature = feature;
  inst->opcode = opcode;
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

void orionpp_instruction_destroy(orionpp_instruction_t* instruction,
                                 const orionpp_allocator_t* allocator) {
  if (!instruction) return;
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Free any allocated arrays within the instruction
  if (instruction->feature == ORIONPP_FEATURE_ABI) {
    if (instruction->opcode == ORIONPP_ABI_ARGS || instruction->opcode == ORIONPP_ABI_RETS) {
      if (instruction->abi_args_rets.values) {
        allocator->free(instruction->abi_args_rets.values);
      }
    }
  }
  
  allocator->free(instruction);
}

bool orionpp_instruction_feature_supports_opcode(orionpp_feature_t feature, uint8_t opcode) {
  switch (feature) {
    case ORIONPP_FEATURE_OBJ:
      return opcode > 0 && opcode < ORIONPP_OBJ_MAX;
    case ORIONPP_FEATURE_ISA:
      return opcode > 0 && opcode < ORIONPP_ISA_MAX;
    case ORIONPP_FEATURE_ABI:
      return opcode > 0 && opcode < ORIONPP_ABI_MAX;
    case ORIONPP_FEATURE_HINT:
      return opcode > 0 && opcode < ORIONPP_HINT_MAX;
    default:
      return false;
  }
}

bool orionpp_instruction_is_valid(const orionpp_instruction_t* instruction) {
  if (!instruction) return false;
  return orionpp_instruction_feature_supports_opcode(instruction->feature, instruction->opcode);
}

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
  
  memcpy(new_values, values, sizeof(orionpp_value_t) * count);
  
  instruction->abi_args_rets.values = new_values;
  instruction->abi_args_rets.count = count;
  
  return ORIONPP_OK_INT(0);
}

const char* orionpp_instruction_get_name(orionpp_feature_t feature, uint8_t opcode) {
  switch (feature) {
    case ORIONPP_FEATURE_OBJ:
      if (opcode < sizeof(obj_instruction_names) / sizeof(obj_instruction_names[0])) {
        return obj_instruction_names[opcode];
      }
      break;
    case ORIONPP_FEATURE_ISA:
      if (opcode < sizeof(isa_instruction_names) / sizeof(isa_instruction_names[0])) {
        return isa_instruction_names[opcode];
      }
      break;
    case ORIONPP_FEATURE_ABI:
      if (opcode < sizeof(abi_instruction_names) / sizeof(abi_instruction_names[0])) {
        return abi_instruction_names[opcode];
      }
      break;
    case ORIONPP_FEATURE_HINT:
      if (opcode < sizeof(hint_instruction_names) / sizeof(hint_instruction_names[0])) {
        return hint_instruction_names[opcode];
      }
      break;
  }
  return "unknown";
}

const char* orionpp_feature_get_name(orionpp_feature_t feature) {
  if (feature < sizeof(feature_names) / sizeof(feature_names[0])) {
    return feature_names[feature];
  }
  return "unknown";
}