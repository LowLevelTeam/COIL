#include "orionpp/instr/instr.h"
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
        // Free individual values first
        for (uint32_t i = 0; i < instruction->abi_args_rets.count; i++) {
          orionpp_value_free(&instruction->abi_args_rets.values[i], allocator);
        }
        allocator->free(instruction->abi_args_rets.values);
      }
    }
  }
  
  // Free any values in other instruction types
  switch (instruction->feature) {
    case ORIONPP_FEATURE_OBJ:
      if (instruction->opcode >= ORIONPP_OBJ_BYTE && instruction->opcode <= ORIONPP_OBJ_QWORD) {
        orionpp_value_free(&instruction->obj_data.value, allocator);
      }
      break;
    case ORIONPP_FEATURE_ISA:
      switch (instruction->opcode) {
        case ORIONPP_ISA_MOV:
          orionpp_value_free(&instruction->isa_mov.dest, allocator);
          orionpp_value_free(&instruction->isa_mov.src, allocator);
          break;
        case ORIONPP_ISA_ADD:
        case ORIONPP_ISA_SUB:
        case ORIONPP_ISA_MUL:
        case ORIONPP_ISA_DIV:
        case ORIONPP_ISA_AND:
        case ORIONPP_ISA_OR:
        case ORIONPP_ISA_XOR:
        case ORIONPP_ISA_SHL:
        case ORIONPP_ISA_SHR:
          orionpp_value_free(&instruction->isa_binary_op.dest, allocator);
          orionpp_value_free(&instruction->isa_binary_op.src1, allocator);
          orionpp_value_free(&instruction->isa_binary_op.src2, allocator);
          break;
        case ORIONPP_ISA_NOT:
          orionpp_value_free(&instruction->isa_unary_op.dest, allocator);
          orionpp_value_free(&instruction->isa_unary_op.src, allocator);
          break;
        case ORIONPP_ISA_BR_EQ:
        case ORIONPP_ISA_BR_NE:
        case ORIONPP_ISA_BR_LT:
        case ORIONPP_ISA_BR_LE:
        case ORIONPP_ISA_BR_GT:
        case ORIONPP_ISA_BR_GE:
          orionpp_value_free(&instruction->isa_branch.label, allocator);
          orionpp_value_free(&instruction->isa_branch.left, allocator);
          orionpp_value_free(&instruction->isa_branch.right, allocator);
          break;
        case ORIONPP_ISA_JMP:
          orionpp_value_free(&instruction->isa_jump.target, allocator);
          break;
        case ORIONPP_ISA_LABEL:
          orionpp_value_free(&instruction->isa_label.label, allocator);
          break;
        case ORIONPP_ISA_CALL:
          orionpp_value_free(&instruction->isa_call.target, allocator);
          break;
      }
      break;
  }
  
  allocator->free(instruction);
}

bool orionpp_instruction_feature_supports_opcode(orionpp_feature_t feature, uint8_t opcode) {
  switch (feature) {
    case ORIONPP_FEATURE_OBJ:
      switch (opcode) {
        case ORIONPP_OBJ_SECTION:
        case ORIONPP_OBJ_SYMBOL:
        case ORIONPP_OBJ_BYTE:
        case ORIONPP_OBJ_WORD:
        case ORIONPP_OBJ_DWORD:
        case ORIONPP_OBJ_QWORD:
        case ORIONPP_OBJ_ALIGN:
          return true;
        default:
          return false;
      }
      
    case ORIONPP_FEATURE_ISA:
      switch (opcode) {
        case ORIONPP_ISA_SCOPE_ENTER:
        case ORIONPP_ISA_SCOPE_LEAVE:
        case ORIONPP_ISA_LET:
        case ORIONPP_ISA_MOV:
        case ORIONPP_ISA_CONST:
        case ORIONPP_ISA_ADD:
        case ORIONPP_ISA_SUB:
        case ORIONPP_ISA_MUL:
        case ORIONPP_ISA_DIV:
        case ORIONPP_ISA_AND:
        case ORIONPP_ISA_OR:
        case ORIONPP_ISA_XOR:
        case ORIONPP_ISA_NOT:
        case ORIONPP_ISA_SHL:
        case ORIONPP_ISA_SHR:
        case ORIONPP_ISA_CMP:
        case ORIONPP_ISA_BR_EQ:
        case ORIONPP_ISA_BR_NE:
        case ORIONPP_ISA_BR_LT:
        case ORIONPP_ISA_BR_LE:
        case ORIONPP_ISA_BR_GT:
        case ORIONPP_ISA_BR_GE:
        case ORIONPP_ISA_JMP:
        case ORIONPP_ISA_LABEL:
        case ORIONPP_ISA_CALL:
        case ORIONPP_ISA_RET:
        case ORIONPP_ISA_NOP:
          return true;
        default:
          return false;
      }
      
    case ORIONPP_FEATURE_ABI:
      switch (opcode) {
        case ORIONPP_ABI_CALLEE:
        case ORIONPP_ABI_CALLER:
        case ORIONPP_ABI_ARGS:
        case ORIONPP_ABI_RETS:
        case ORIONPP_ABI_SETUP:
        case ORIONPP_ABI_CLEANUP:
          return true;
        default:
          return false;
      }
      
    case ORIONPP_FEATURE_HINT:
      switch (opcode) {
        case ORIONPP_HINT_SYMEND:
        case ORIONPP_HINT_OPTIMIZE:
        case ORIONPP_HINT_UNROLL:
        case ORIONPP_HINT_INLINE:
        case ORIONPP_HINT_NOINLINE:
          return true;
        default:
          return false;
      }
      
    default:
      return false;
  }
}

bool orionpp_instruction_is_valid(const orionpp_instruction_t* instruction) {
  if (!instruction) return false;
  return orionpp_instruction_feature_supports_opcode(instruction->feature, instruction->opcode);
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