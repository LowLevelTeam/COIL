/**
 * @file src/executor.c
 * @brief Enhanced Orion++ instruction executor implementation with conditional branches
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
        case ORIONPP_OP_ISA_BREQ: return ovm_exec_breq(vm, instr);
        case ORIONPP_OP_ISA_BRNEQ: return ovm_exec_brneq(vm, instr);
        case ORIONPP_OP_ISA_BRGT: return ovm_exec_brgt(vm, instr);
        case ORIONPP_OP_ISA_BRGE: return ovm_exec_brge(vm, instr);
        case ORIONPP_OP_ISA_BRLT: return ovm_exec_brlt(vm, instr);
        case ORIONPP_OP_ISA_BRLE: return ovm_exec_brle(vm, instr);
        case ORIONPP_OP_ISA_BRZ: return ovm_exec_brz(vm, instr);
        case ORIONPP_OP_ISA_BRNZ: return ovm_exec_brnz(vm, instr);
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
    case ORIONPP_TYPE_C:
      if (value->bytesize >= sizeof(char)) {
        var->value.i64 = *(char*)value->bytes;
        var->is_initialized = true;
      } else {
        ovm_error(vm, "Invalid character constant size");
        return -1;
      }
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

// Conditional branch instruction implementations
int ovm_exec_breq(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BREQ instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BREQ instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BREQ instruction");
    return -1;
  }
  
  // Validate variables are initialized
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result == 0, label_id);
}

int ovm_exec_brneq(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BRNEQ instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRNEQ instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BRNEQ instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result != 0, label_id);
}

int ovm_exec_brgt(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BRGT instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRGT instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BRGT instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result > 0, label_id);
}

int ovm_exec_brge(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BRGE instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRGE instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BRGE instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result >= 0, label_id);
}

int ovm_exec_brlt(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BRLT instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRLT instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BRLT instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result < 0, label_id);
}

int ovm_exec_brle(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "BRLE instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t left_id, right_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &right_id) != 0 ||
      ovm_extract_label_id(&instr->values[2], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRLE instruction");
    return -1;
  }
  
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!left || !right) {
    ovm_error(vm, "Variables not found in BRLE instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, left);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Left operand not initialized");
    return -1;
  }
  
  validation = ovm_validate_variable_initialization(vm, right);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Right operand not initialized");
    return -1;
  }
  
  int comparison_result;
  if (ovm_compare_variables(vm, left, right, &comparison_result) != 0) {
    return -1;
  }
  
  return ovm_branch_if_condition(vm, comparison_result <= 0, label_id);
}

int ovm_exec_brz(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "BRZ instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t var_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &var_id) != 0 ||
      ovm_extract_label_id(&instr->values[1], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRZ instruction");
    return -1;
  }
  
  VMVariable* var = ovm_get_variable(vm, var_id);
  if (!var) {
    ovm_error(vm, "Variable %u not found", var_id);
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, var);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Variable not initialized");
    return -1;
  }
  
  bool is_zero = false;
  switch (var->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
    case ORIONPP_TYPE_C:
      is_zero = (var->value.i64 == 0);
      break;
    default:
      ovm_error(vm, "Invalid variable type for BRZ instruction");
      return -1;
  }
  
  return ovm_branch_if_condition(vm, is_zero, label_id);
}

int ovm_exec_brnz(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "BRNZ instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t var_id;
  orionpp_label_id_t label_id;
  
  if (ovm_extract_variable_id(&instr->values[0], &var_id) != 0 ||
      ovm_extract_label_id(&instr->values[1], &label_id) != 0) {
    ovm_error(vm, "Invalid operands in BRNZ instruction");
    return -1;
  }
  
  VMVariable* var = ovm_get_variable(vm, var_id);
  if (!var) {
    ovm_error(vm, "Variable %u not found", var_id);
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, var);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Variable not initialized");
    return -1;
  }
  
  bool is_not_zero = false;
  switch (var->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
    case ORIONPP_TYPE_C:
      is_not_zero = (var->value.i64 != 0);
      break;
    default:
      ovm_error(vm, "Invalid variable type for BRNZ instruction");
      return -1;
  }
  
  return ovm_branch_if_condition(vm, is_not_zero, label_id);
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
            case ORIONPP_TYPE_C:
              printf("%c\n", (char)arg->value.i64);
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
  
  vm->pc++;
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

// Arithmetic operations (keeping existing implementations)
int ovm_exec_add(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "ADD instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in ADD instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in ADD instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_ADD);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for ADD operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 + right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_sub(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "SUB instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in SUB instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in SUB instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_SUB);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for SUB operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 - right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_mul(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "MUL instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in MUL instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in MUL instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_MUL);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for MUL operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 * right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_div(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "DIV instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in DIV instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in DIV instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_DIV);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for DIV operation");
    return -1;
  }
  
  if (right->value.i64 == 0) {
    ovm_error(vm, "Division by zero");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 / right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_mod(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "MOD instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in MOD instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in MOD instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_MOD);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for MOD operation");
    return -1;
  }
  
  if (right->value.i64 == 0) {
    ovm_error(vm, "Division by zero in modulo operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 % right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_inc(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "INC instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, operand_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &operand_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in INC instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* operand = ovm_get_variable(vm, operand_id);
  
  if (!dest || !operand) {
    ovm_error(vm, "Variables not found in INC instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Operand variable not initialized");
    return -1;
  }
  
  dest->value.i64 = operand->value.i64 + 1;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_dec(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "DEC instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, operand_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &operand_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in DEC instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* operand = ovm_get_variable(vm, operand_id);
  
  if (!dest || !operand) {
    ovm_error(vm, "Variables not found in DEC instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Operand variable not initialized");
    return -1;
  }
  
  dest->value.i64 = operand->value.i64 - 1;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_incp(OrionVM* vm, const orinopp_instruction_t* instr) {
  // Post-increment: return original value, then increment
  if (instr->value_count < 2) {
    ovm_error(vm, "INC++ instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, operand_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &operand_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in INC++ instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* operand = ovm_get_variable(vm, operand_id);
  
  if (!dest || !operand) {
    ovm_error(vm, "Variables not found in INC++ instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Operand variable not initialized");
    return -1;
  }
  
  dest->value.i64 = operand->value.i64;
  operand->value.i64++;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_decp(OrionVM* vm, const orinopp_instruction_t* instr) {
  // Post-decrement: return original value, then decrement
  if (instr->value_count < 2) {
    ovm_error(vm, "DEC++ instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, operand_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &operand_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in DEC++ instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* operand = ovm_get_variable(vm, operand_id);
  
  if (!dest || !operand) {
    ovm_error(vm, "Variables not found in DEC++ instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Operand variable not initialized");
    return -1;
  }
  
  dest->value.i64 = operand->value.i64;
  operand->value.i64--;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_and(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "AND instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in AND instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in AND instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_AND);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for AND operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 & right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_or(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "OR instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in OR instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in OR instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_OR);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for OR operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 | right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_xor(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "XOR instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in XOR instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in XOR instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_XOR);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for XOR operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 ^ right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_not(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 2) {
    ovm_error(vm, "NOT instruction requires 2 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, operand_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &operand_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in NOT instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* operand = ovm_get_variable(vm, operand_id);
  
  if (!dest || !operand) {
    ovm_error(vm, "Variables not found in NOT instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Operand variable not initialized");
    return -1;
  }
  
  dest->value.i64 = ~operand->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_shl(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "SHL instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in SHL instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in SHL instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_SHL);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for SHL operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 << right->value.i64;
  dest->is_initialized = true;
  return 0;
}

int ovm_exec_shr(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (instr->value_count < 3) {
    ovm_error(vm, "SHR instruction requires 3 operands");
    return -1;
  }
  
  orionpp_variable_id_t dest_id, left_id, right_id;
  if (ovm_extract_variable_id(&instr->values[0], &dest_id) != 0 ||
      ovm_extract_variable_id(&instr->values[1], &left_id) != 0 ||
      ovm_extract_variable_id(&instr->values[2], &right_id) != 0) {
    ovm_error(vm, "Invalid variable IDs in SHR instruction");
    return -1;
  }
  
  VMVariable* dest = ovm_get_variable(vm, dest_id);
  VMVariable* left = ovm_get_variable(vm, left_id);
  VMVariable* right = ovm_get_variable(vm, right_id);
  
  if (!dest || !left || !right) {
    ovm_error(vm, "Variables not found in SHR instruction");
    return -1;
  }
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, ORIONPP_OP_ISA_SHR);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Type validation failed for SHR operation");
    return -1;
  }
  
  dest->value.i64 = left->value.i64 >> right->value.i64;
  dest->is_initialized = true;
  return 0;
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

int ovm_compare_variables(OrionVM* vm, VMVariable* left, VMVariable* right, int* result) {
  if (!vm || !left || !right || !result) return -1;
  
  // Check types are compatible for comparison
  if (!ovm_types_compatible(left->type, right->type)) {
    ovm_error(vm, "Type mismatch in variable comparison");
    return -1;
  }
  
  // Perform comparison based on type
  switch (left->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
    case ORIONPP_TYPE_C:
      if (left->value.i64 < right->value.i64) {
        *result = -1;
      } else if (left->value.i64 > right->value.i64) {
        *result = 1;
      } else {
        *result = 0;
      }
      break;
    case ORIONPP_TYPE_STRING:
      if (left->value.str && right->value.str) {
        *result = strcmp(left->value.str, right->value.str);
      } else if (left->value.str) {
        *result = 1;
      } else if (right->value.str) {
        *result = -1;
      } else {
        *result = 0;
      }
      break;
    default:
      ovm_error(vm, "Unsupported type for comparison");
      return -1;
  }
  
  return 0;
}

int ovm_branch_if_condition(OrionVM* vm, bool condition, orionpp_label_id_t label_id) {
  if (!vm) return -1;
  
  if (condition) {
    size_t target = ovm_find_label(vm, label_id);
    if (target == SIZE_MAX) {
      ovm_error(vm, "Label %u not found", label_id);
      return -1;
    }
    
    ValidationResult validation = ovm_validate_label_jump(vm, label_id);
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

int ovm_perform_binary_op(OrionVM* vm, VMVariable* dest, VMVariable* left, VMVariable* right, orionpp_opcode_module_t op) {
  if (!vm || !dest || !left || !right) return -1;
  
  ValidationResult validation = ovm_validate_type_operation(vm, left, right, op);
  if (validation != OVM_VALID) {
    return -1;
  }
  
  switch (op) {
    case ORIONPP_OP_ISA_ADD:
      dest->value.i64 = left->value.i64 + right->value.i64;
      break;
    case ORIONPP_OP_ISA_SUB:
      dest->value.i64 = left->value.i64 - right->value.i64;
      break;
    case ORIONPP_OP_ISA_MUL:
      dest->value.i64 = left->value.i64 * right->value.i64;
      break;
    case ORIONPP_OP_ISA_DIV:
      if (right->value.i64 == 0) {
        ovm_error(vm, "Division by zero");
        return -1;
      }
      dest->value.i64 = left->value.i64 / right->value.i64;
      break;
    case ORIONPP_OP_ISA_MOD:
      if (right->value.i64 == 0) {
        ovm_error(vm, "Division by zero in modulo");
        return -1;
      }
      dest->value.i64 = left->value.i64 % right->value.i64;
      break;
    case ORIONPP_OP_ISA_AND:
      dest->value.i64 = left->value.i64 & right->value.i64;
      break;
    case ORIONPP_OP_ISA_OR:
      dest->value.i64 = left->value.i64 | right->value.i64;
      break;
    case ORIONPP_OP_ISA_XOR:
      dest->value.i64 = left->value.i64 ^ right->value.i64;
      break;
    case ORIONPP_OP_ISA_SHL:
      dest->value.i64 = left->value.i64 << right->value.i64;
      break;
    case ORIONPP_OP_ISA_SHR:
      dest->value.i64 = left->value.i64 >> right->value.i64;
      break;
    default:
      ovm_error(vm, "Unsupported binary operation");
      return -1;
  }
  
  dest->is_initialized = true;
  return 0;
}

int ovm_perform_unary_op(OrionVM* vm, VMVariable* dest, VMVariable* operand, orionpp_opcode_module_t op) {
  if (!vm || !dest || !operand) return -1;
  
  ValidationResult validation = ovm_validate_variable_initialization(vm, operand);
  if (validation != OVM_VALID) {
    return -1;
  }
  
  switch (op) {
    case ORIONPP_OP_ISA_NOT:
      dest->value.i64 = ~operand->value.i64;
      break;
    case ORIONPP_OP_ISA_INC:
      dest->value.i64 = operand->value.i64 + 1;
      break;
    case ORIONPP_OP_ISA_DEC:
      dest->value.i64 = operand->value.i64 - 1;
      break;
    case ORIONPP_OP_ISA_INCp:
      dest->value.i64 = operand->value.i64;
      operand->value.i64++;
      break;
    case ORIONPP_OP_ISA_DECp:
      dest->value.i64 = operand->value.i64;
      operand->value.i64--;
      break;
    default:
      ovm_error(vm, "Unsupported unary operation");
      return -1;
  }
  
  dest->is_initialized = true;
  return 0;
}

int ovm_convert_value(VMVariable* dest, const VMVariable* src, orionpp_type_t target_type) {
  if (!dest || !src || !src->is_initialized) return -1;
  
  // Simple type conversion
  if (src->type == target_type) {
    // Same type - direct copy
    switch (src->type) {
      case ORIONPP_TYPE_WORD:
      case ORIONPP_TYPE_SIZE:
      case ORIONPP_TYPE_C:
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