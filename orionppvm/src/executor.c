/**
 * @file src/executor.c
 * @brief Orion++ instruction executor implementation
 */

#include "executor.h"
#include "validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ovm_execute_instruction(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (!vm || !instr) return -1;
  
  switch (instr->root) {
    case ORIONPP_OP_ISA:
      switch (instr->child) {
        case ORIONPP_OP_ISA_VAR: return ovm_exec_var(vm, instr);
        case ORIONPP_OP_ISA_CONST: return ovm_exec_const(vm, instr);
        case ORIONPP_OP_ISA_MOV: return ovm_exec_mov(vm, instr);
        case ORIONPP_OP_ISA_LEA: return ovm_exec_lea(vm, instr);
        case ORIONPP_OP_ISA_LABEL: return ovm_exec_label(vm, instr);
        case ORIONPP_OP_ISA_JMP: return ovm_exec_jmp(vm, instr);
        case ORIONPP_OP_ISA_BR: return ovm_exec_br(vm, instr);
        case ORIONPP_OP_ISA_CALL: return ovm_exec_call(vm, instr);
        case ORIONPP_OP_ISA_RET: return ovm_exec_ret(vm, instr);
        case ORIONPP_OP_ISA_ADD: return ovm_exec_add(vm, instr);
        case ORIONPP_OP_ISA_SUB: return ovm_exec_sub(vm, instr);
        case ORIONPP_OP_ISA_MUL: return ovm_exec_mul(vm, instr);
        case ORIONPP_OP_ISA_DIV: return ovm_exec_div(vm, instr);
        case ORIONPP_OP_ISA_MOD: return ovm_exec_mod(vm, instr);
        case ORIONPP_OP_ISA_INC: return ovm_exec_inc(vm, instr);
        case ORIONPP_OP_ISA_DEC: return ovm_exec_dec(vm, instr);
        case ORIONPP_OP_ISA_INCp: return ovm_exec_incp(vm, instr);
        case ORIONPP_OP_ISA_DECp: return ovm_exec_decp(vm, instr);
        case ORIONPP_OP_ISA_AND: return ovm_exec_and(vm, instr);
        case ORIONPP_OP_ISA_OR: return ovm_exec_or(vm, instr);
        case ORIONPP_OP_ISA_XOR: return ovm_exec_xor(vm, instr);
        case ORIONPP_OP_ISA_NOT: return ovm_exec_not(vm, instr);
        case ORIONPP_OP_ISA_SHL: return ovm_exec_shl(vm, instr);
        case ORIONPP_OP_ISA_SHR: return ovm_exec_shr(vm, instr);
        default:
          ovm_error(vm, "Unknown ISA instruction: %d", instr->child);
          return -1;
      }
    case ORIONPP_OP_HINT:
      return ovm_exec_hint(vm, instr);
    case ORIONPP_OP_TYPE:
    case ORIONPP_OP_ABI:
    case ORIONPP_OP_OBJ:
      // These are metadata instructions - ignore during execution
      return 0;
    default:
      ovm_error(vm, "Unknown instruction opcode: %d", instr->root);
      return -1;
  }
}

int ovm_exec_var(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "VAR instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t var_id;
  if (ovm_extract_variable_id(&instr->values[0], &var_id) != 0) {
    ovm_error(vm, "Invalid variable ID in VAR instruction");
    return -1;
  }
  
  // Check if variable already exists
  if (ovm_get_variable(vm, var_id) != NULL) {
    ovm_error(vm, "Variable %u already declared", var_id);
    return -1;
  }
  
  orionpp_type_t type = instr->values[1].root;
  VMVariable* var = ovm_create_variable(vm, var_id, type);
  if (!var) {
    return -1;
  }
  
  return 0;
}

int ovm_exec_const(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "CONST instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t var_id;
  if (ovm_extract_variable_id(&instr->values[0], &var_id) != 0) {
    ovm_error(vm, "Invalid variable ID in CONST instruction");
    return -1;
  }
  
  orionpp_type_t type = instr->values[1].root;
  VMVariable* var = ovm_get_variable(vm, var_id);
  
  if (!var) {
    var = ovm_create_variable(vm, var_id, type);
    if (!var) return -1;
  }
  
  // Set the constant value
  const orinopp_value_t* value = &instr->values[2];
  switch (type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
      if (value->bytesize >= sizeof(int32_t)) {
        var->value.i64 = *(int32_t*)value->bytes;
        var->is_initialized = true;
      } else {
        ovm_error(vm, "Invalid integer constant size");
        return -1;
      }
      break;
    case ORIONPP_TYPE_STRING:
      if (var->value.str) free(var->value.str);
      var->value.str = malloc(value->bytesize + 1);
      if (!var->value.str) {
        ovm_error(vm, "Out of memory for string constant");
        return -1;
      }
      memcpy(var->value.str, value->bytes, value->bytesize);
      var->value.str[value->bytesize] = '\0';
      var->is_initialized = true;
      break;
    default:
      ovm_error(vm, "Unsupported constant type: %d", type);
      return -1;
  }
  
  return 0;
}

int ovm_exec_mov(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "MOV instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, src_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &src_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in MOV instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* src = ovm_get_variable(vm, src_id);
  
  if (!dest) {
    ovm_error(vm, "Destination variable %u not found", dest_id);
    return -1;
  }
  if (!src) {
    ovm_error(vm, "Source variable %u not found", src_id);
    return -1;
  }
  
  // Validate source is initialized
  ValidationResult validation = ovm_validate_variable_initialization(vm, src);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Source variable not initialized");
    return -1;
  }
  
  // Copy value with type conversion if needed
  return ovm_convert_value(dest, src, dest->type);
}

int ovm_exec_lea(OrionVM* vm, const orinopp_instruction_t* instr) {
  // LEA (Load Effective Address) - simplified implementation
  ovm_error(vm, "LEA instruction not yet implemented");
  return -1;
}

int ovm_exec_label(OrionVM* vm, const orinopp_instruction_t* instr) {
  // Labels are processed during loading, nothing to do at runtime
  return 0;
}

int ovm_exec_jmp(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 1) {
    ovm_error(vm, "JMP instruction requires 1 operand");
    return -1;
  }
  
  orionpp_label_id_t label_id;
  if (ovm_extract_label_id(&instr->values[0], &label_id) != 0) {
    ovm_error(vm, "Invalid label ID in JMP instruction");
    return -1;
  }
  
  size_t target = ovm_find_label(vm, label_id);
  if (target == SIZE_MAX) {
    ovm_error(vm, "Label %u not found", label_id);
    return -1;
  }
  
  // Validate jump target
  ValidationResult validation = ovm_validate_label_jump(vm, label_id);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Invalid jump target");
    return -1;
  }
  
  vm->pc = target;
  return 0;
}

int ovm_exec_br(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "BR instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t cond_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &cond_id) != 0 ||
      ovm_extract_label_id(&instr->values[1], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BR instruction");
    return -1;
  }
  
  VMVariable* cond = ovm_get_variable(vm, cond_id);
  if (!cond) {
    ovm_error(vm, "Condition variable %u not found", cond_id);
    return -1;
  }
  
  // Validate condition is initialized
  ValidationResult validation = ovm_validate_variable_initialization(vm, cond);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Condition variable not initialized");
    return -1;
  }
  
  // Check condition
  bool should_branch = false;
  switch (cond->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
      should_branch = (cond->value.i64 != 0);
      break;
    default:
      ovm_error(vm, "Invalid condition type for BR instruction");
      return -1;
  }
  
  if (should_branch) {
    size_t target = ovm_find_label(vm, label_id);
    if (target == SIZE_MAX) {
      ovm_error(vm, "Label %u not found", label_id);
      return -1;
    }
    
    validation = ovm_validate_label_jump(vm, label_id);
    if (validation != OVM_VALID) {
      ovm_error(vm, "Invalid branch target");
      return -1;
    }
    
    vm->pc = target;
  } else {
    vm->pc++; // Continue to next instruction
  }
  
  return 0;
}

int ovm_exec_call(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "CALL instruction requires at least 2 operands");
    return -1;
  }
  
  // Simple function call implementation
  // For now, we'll just handle built-in functions
  char* function_name;
  if (ovm_extract_string(&instr->values[1], &function_name) != 0) {
    ovm_error(vm, "Invalid function name in CALL instruction");
    return -1;
  }
  
  // Validate call depth
  ValidationResult validation = ovm_validate_call_depth(vm);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Call depth limit exceeded");
    free(function_name);
    return -1;
  }
  
  // For now, we'll implement a simple print function
  if (strcmp(function_name, "print") == 0) {
    // Print function - takes one argument
    if (instr->value_count >= 3) {
      orionpp_variable_id_t arg_id;
      if (ovm_extract_variable_id(&instr->values[2], &arg_id) == 0) {
        VMVariable* arg = ovm_get_variable(vm, arg_id);
        if (arg && arg->is_initialized) {
          switch (arg->type) {
            case ORIONPP_TYPE_WORD:
            case ORIONPP_TYPE_SIZE:
              printf("%lld\n", (long long)arg->value.i64);
              break;
            case ORIONPP_TYPE_STRING:
              printf("%s\n", arg->value.str ? arg->value.str : "(null)");
              break;
            default:
              printf("(unhandled type)\n");
              break;
          }
        }
      }
    }
    
    // Set return value to 0
    orionpp_variable_id_t result_id;
    if (ovm_extract_variable_id(&instr->values[0], &result_id) == 0) {
      VMVariable* result = ovm_get_variable(vm, result_id);
      if (!result) {
        result = ovm_create_variable(vm, result_id, ORIONPP_TYPE_WORD);
      }
      if (result) {
        result->value.i64 = 0;
        result->is_initialized = true;
      }
    }
  } else {
    ovm_error(vm, "Unknown function: %s", function_name);
    free(function_name);
    return -1;
  }
  
  free(function_name);
  return 0;
}

int ovm_exec_ret(OrionVM* vm, const orinopp_instruction_t* instr) {
  // Set return value if provided
  if (instr->value_count > 0) {
    orionpp_variable_id_t ret_id;
    if (ovm_extract_variable_id(&instr->values[0], &ret_id) == 0) {
      VMVariable* ret_var = ovm_get_variable(vm, ret_id);
      if (ret_var && ret_var->is_initialized) {
        vm->return_value = *ret_var;
        // Don't free the string as we're copying the pointer
        if (ret_var->type == ORIONPP_TYPE_STRING && ret_var->value.str) {
          vm->return_value.value.str = strdup(ret_var->value.str);
        }
      }
    }
  }
  
  // Simple implementation - just stop execution
  vm->running = false;
  return 0;
}

// Arithmetic operations implementation
int ovm_exec_add(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_ADD);
}

int ovm_exec_sub(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_SUB);
}

int ovm_exec_mul(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_MUL);
}

int ovm_exec_div(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_DIV);
}

int ovm_exec_mod(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_MOD);
}

int ovm_exec_inc(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_unary_op(vm, NULL, NULL, ORIONPP_OP_ISA_INC);
}

int ovm_exec_dec(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_unary_op(vm, NULL, NULL, ORIONPP_OP_ISA_DEC);
}

int ovm_exec_incp(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_unary_op(vm, NULL, NULL, ORIONPP_OP_ISA_INCp);
}

int ovm_exec_decp(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_unary_op(vm, NULL, NULL, ORIONPP_OP_ISA_DECp);
}

int ovm_exec_and(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_AND);
}

int ovm_exec_or(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_OR);
}

int ovm_exec_xor(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_XOR);
}

int ovm_exec_not(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_unary_op(vm, NULL, NULL, ORIONPP_OP_ISA_NOT);
}

int ovm_exec_shl(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_SHL);
}

int ovm_exec_shr(OrionVM* vm, const orinopp_instruction_t* instr) {
  return ovm_perform_binary_op(vm, NULL, NULL, NULL, ORIONPP_OP_ISA_SHR);
}

int ovm_exec_hint(OrionVM* vm, const orinopp_instruction_t* instr) {
  // Hints are ignored during execution
  return 0;
}

// Helper functions
int ovm_extract_variable_id(const orinopp_value_t* value, orionpp_variable_id_t* id) {
  if (!value || !id || value->root != ORIONPP_TYPE_VARID) return -1;
  if (value->bytesize < sizeof(orionpp_variable_id_t)) return -1;
  
  *id = *(orionpp_variable_id_t*)value->bytes;
  return 0;
}

int ovm_extract_label_id(const orinopp_value_t* value, orionpp_label_id_t* id) {
  if (!value || !id || value->root != ORIONPP_TYPE_LABELID) return -1;
  if (value->bytesize < sizeof(orionpp_label_id_t)) return -1;
  
  *id = *(orionpp_label_id_t*)value->bytes;
  return 0;
}

int ovm_extract_integer(const orinopp_value_t* value, int64_t* result) {
  if (!value || !result) return -1;
  
  switch (value->root) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
      if (value->bytesize >= sizeof(int32_t)) {
        *result = *(int32_t*)value->bytes;
        return 0;
      }
      break;
    default:
      break;
  }
  
  return -1;
}

int ovm_extract_string(const orinopp_value_t* value, char** result) {
  if (!value || !result) return -1;
  if (value->root != ORIONPP_TYPE_STRING && value->root != ORIONPP_TYPE_SYMBOL) return -1;
  
  *result = malloc(value->bytesize + 1);
  if (!*result) return -1;
  
  memcpy(*result, value->bytes, value->bytesize);
  (*result)[value->bytesize] = '\0';
  
  return 0;
}

int ovm_perform_binary_op(OrionVM* vm, VMVariable* dest, VMVariable* left, VMVariable* right, orionpp_opcode_module_t op) {
  // This is a simplified implementation - in practice you'd extract operands from instruction
  ovm_error(vm, "Binary operations not fully implemented");
  return -1;
}

int ovm_perform_unary_op(OrionVM* vm, VMVariable* dest, VMVariable* operand, orionpp_opcode_module_t op) {
  // This is a simplified implementation - in practice you'd extract operands from instruction
  ovm_error(vm, "Unary operations not fully implemented");
  return -1;
}

int ovm_convert_value(VMVariable* dest, const VMVariable* src, orionpp_type_t target_type) {
  if (!dest || !src || !src->is_initialized) return -1;
  
  // Simple type conversion
  if (src->type == target_type) {
    // Same type - direct copy
    switch (src->type) {
      case ORIONPP_TYPE_WORD:
      case ORIONPP_TYPE_SIZE:
        dest->value.i64 = src->value.i64;
        break;
      case ORIONPP_TYPE_STRING:
        if (dest->value.str) free(dest->value.str);
        dest->value.str = src->value.str ? strdup(src->value.str) : NULL;
        break;
      default:
        return -1;
    }
    dest->is_initialized = true;
    return 0;
  }
  
  // Basic type conversions
  if (ovm_is_numeric_type(src->type) && ovm_is_numeric_type(target_type)) {
    dest->value.i64 = src->value.i64;
    dest->is_initialized = true;
    return 0;
  }
  
  return -1;
}

bool ovm_types_compatible(orionpp_type_t type1, orionpp_type_t type2) {
  if (type1 == type2) return true;
  if (ovm_is_numeric_type(type1) && ovm_is_numeric_type(type2)) return true;
  return false;
}