/**
 * @file src/validator.c
 * @brief Orion++ runtime safety validator implementation
 */

#include "validator.h"
#include <stdio.h>
#include <string.h>

static ValidationLevel current_validation_level = OVM_VALIDATE_BASIC;

bool ovm_types_compatible(orionpp_type_t type1, orionpp_type_t type2); // Forward declaration

ValidationResult ovm_validate_program(OrionVM* vm) {
  if (!vm) return OVM_INVALID_INSTRUCTION;
  
  // Validate each instruction
  for (size_t i = 0; i < vm->instruction_count; i++) {
    ValidationResult result = ovm_validate_instruction(vm, &vm->instructions[i], i);
    if (result != OVM_VALID) {
      ovm_report_validation_error(vm, result, "Program validation failed");
      return result;
    }
  }
  
  // Validate labels
  ValidationResult result = ovm_validate_labels(vm);
  if (result != OVM_VALID) {
    return result;
  }
  
  // Validate variables
  result = ovm_validate_variables(vm);
  if (result != OVM_VALID) {
    return result;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_instruction(OrionVM* vm, const orinopp_instruction_t* instr, size_t index) {
  if (!vm || !instr) return OVM_INVALID_INSTRUCTION;
  
  // Basic instruction validation
  switch (instr->root) {
    case ORIONPP_OP_ISA:
      switch (instr->child) {
        case ORIONPP_OP_ISA_VAR:
          if (instr->value_count < 2) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_CONST:
          if (instr->value_count < 3) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_MOV:
          if (instr->value_count < 2) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID || 
              instr->values[1].root != ORIONPP_TYPE_VARID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_JMP:
          if (instr->value_count < 1) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_LABELID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_BR:
          if (instr->value_count < 2) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID ||
              instr->values[1].root != ORIONPP_TYPE_LABELID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_CALL:
          if (instr->value_count < 2) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID ||
              (instr->values[1].root != ORIONPP_TYPE_SYMBOL && 
               instr->values[1].root != ORIONPP_TYPE_STRING)) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_ADD:
        case ORIONPP_OP_ISA_SUB:
        case ORIONPP_OP_ISA_MUL:
        case ORIONPP_OP_ISA_DIV:
        case ORIONPP_OP_ISA_MOD:
        case ORIONPP_OP_ISA_AND:
        case ORIONPP_OP_ISA_OR:
        case ORIONPP_OP_ISA_XOR:
        case ORIONPP_OP_ISA_SHL:
        case ORIONPP_OP_ISA_SHR:
          if (instr->value_count < 3) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID ||
              instr->values[1].root != ORIONPP_TYPE_VARID ||
              instr->values[2].root != ORIONPP_TYPE_VARID) return OVM_INVALID_OPERAND;
          break;
        case ORIONPP_OP_ISA_NOT:
        case ORIONPP_OP_ISA_INC:
        case ORIONPP_OP_ISA_DEC:
        case ORIONPP_OP_ISA_INCp:
        case ORIONPP_OP_ISA_DECp:
          if (instr->value_count < 2) return OVM_INVALID_OPERAND;
          if (instr->values[0].root != ORIONPP_TYPE_VARID ||
              instr->values[1].root != ORIONPP_TYPE_VARID) return OVM_INVALID_OPERAND;
          break;
        default:
          break;
      }
      break;
    case ORIONPP_OP_HINT:
    case ORIONPP_OP_TYPE:
    case ORIONPP_OP_ABI:
    case ORIONPP_OP_OBJ:
      // Metadata instructions are always valid
      break;
    default:
      return OVM_INVALID_INSTRUCTION;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_labels(OrionVM* vm) {
  if (!vm) return OVM_INVALID_LABEL_ID;
  
  // register all labels
  for (size_t i = 0; i < vm->instruction_count; i++) {
    const orinopp_instruction_t* instr = &vm->instructions[i];
    if (instr->root == ORIONPP_OP_ISA && instr->child == ORIONPP_OP_ISA_LABEL) {
      if (instr->value_count > 0 && instr->values[0].root == ORIONPP_TYPE_LABELID) {
        orionpp_label_id_t label_id;
        if (ovm_extract_label_id(&instr->values[0], &label_id) == 0) {
          ovm_register_label(vm, label_id, i);
        }
      }
    }
  }

  // Validate all label references point to valid labels
  for (size_t i = 0; i < vm->instruction_count; i++) {
    const orinopp_instruction_t* instr = &vm->instructions[i];
    
    if (instr->root == ORIONPP_OP_ISA) {
      switch (instr->child) {
        case ORIONPP_OP_ISA_JMP:
        case ORIONPP_OP_ISA_BR:
          for (size_t j = 0; j < instr->value_count; j++) {
            if (instr->values[j].root == ORIONPP_TYPE_LABELID) {
              if (instr->values[j].bytesize < sizeof(orionpp_label_id_t)) {
                return OVM_INVALID_LABEL_ID;
              }
              orionpp_label_id_t label_id = *(orionpp_label_id_t*)instr->values[j].bytes;
              if (ovm_find_label(vm, label_id) == SIZE_MAX) {
                return OVM_INVALID_LABEL_ID;
              }
            }
          }
          break;
        default:
          break;
      }
    }
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_variables(OrionVM* vm) {
  if (!vm) return OVM_INVALID_VARIABLE_ID;
  
  // For now, just check that variable IDs are reasonable
  for (size_t i = 0; i < vm->variable_count; i++) {
    if (vm->variables[i].id >= OVM_MAX_VARIABLES) {
      return OVM_INVALID_VARIABLE_ID;
    }
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_execution_safety(OrionVM* vm) {
  if (!vm) return OVM_INVALID_INSTRUCTION;
  
  // Check memory usage
  ValidationResult result = ovm_validate_memory_usage(vm);
  if (result != OVM_VALID) return result;
  
  // Check stack usage
  result = ovm_validate_stack_usage(vm);
  if (result != OVM_VALID) return result;
  
  // Check program counter bounds
  if (vm->pc >= vm->instruction_count) {
    return OVM_INVALID_JUMP;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_variable_access(OrionVM* vm, orionpp_variable_id_t id) {
  if (!vm) return OVM_INVALID_VARIABLE_ID;
  
  if (id >= OVM_MAX_VARIABLES) {
    return OVM_INVALID_VARIABLE_ID;
  }
  
  VMVariable* var = ovm_get_variable(vm, id);
  if (!var) {
    return OVM_INVALID_VARIABLE_ID;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_label_jump(OrionVM* vm, orionpp_label_id_t id) {
  if (!vm) return OVM_INVALID_LABEL_ID;
  
  size_t target = ovm_find_label(vm, id);
  if (target == SIZE_MAX) {
    return OVM_INVALID_LABEL_ID;
  }
  
  if (target >= vm->instruction_count) {
    return OVM_INVALID_JUMP;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_function_call(OrionVM* vm, const char* function_name) {
  if (!vm || !function_name) return OVM_INVALID_FUNCTION_CALL;
  
  // For now, only allow certain built-in functions
  if (strcmp(function_name, "print") == 0) {
    return OVM_VALID;
  }
  
  // Add more built-in functions as needed
  return OVM_INVALID_FUNCTION_CALL;
}

ValidationResult ovm_validate_memory_usage(OrionVM* vm) {
  if (!vm) return OVM_MEMORY_LIMIT_EXCEEDED;
  
  if (vm->memory_used > OVM_MAX_MEMORY_SIZE) {
    return OVM_MEMORY_LIMIT_EXCEEDED;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_stack_usage(OrionVM* vm) {
  if (!vm) return OVM_STACK_OVERFLOW;
  
  if (vm->call_depth >= OVM_MAX_CALL_DEPTH) {
    return OVM_STACK_OVERFLOW;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_division(OrionVM* vm, const VMVariable* divisor) {
  if (!vm || !divisor) return OVM_DIVISION_BY_ZERO;
  
  if (!divisor->is_initialized) {
    return OVM_UNINITIALIZED_VARIABLE;
  }
  
  switch (divisor->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
      if (divisor->value.i64 == 0) {
        return OVM_DIVISION_BY_ZERO;
      }
      break;
    default:
      return OVM_TYPE_MISMATCH;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_type_operation(OrionVM* vm, const VMVariable* var1, const VMVariable* var2, orionpp_opcode_module_t op) {
  if (!vm || !var1 || !var2) return OVM_TYPE_MISMATCH;
  
  // Check if both variables are initialized
  if (!var1->is_initialized || !var2->is_initialized) {
    return OVM_UNINITIALIZED_VARIABLE;
  }
  
  // Check type compatibility for operation
  if (!ovm_types_compatible(var1->type, var2->type)) {
    return OVM_TYPE_MISMATCH;
  }
  
  // Check for division by zero
  if ((op == ORIONPP_OP_ISA_DIV || op == ORIONPP_OP_ISA_MOD)) {
    ValidationResult result = ovm_validate_division(vm, var2);
    if (result != OVM_VALID) return result;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_variable_initialization(OrionVM* vm, const VMVariable* var) {
  if (!vm || !var) return OVM_UNINITIALIZED_VARIABLE;
  
  if (!var->is_initialized) {
    return OVM_UNINITIALIZED_VARIABLE;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_array_access(OrionVM* vm, const VMVariable* array, int64_t index) {
  if (!vm || !array) return OVM_BUFFER_OVERFLOW;
  
  // Basic array bounds checking - simplified implementation
  if (index < 0) {
    return OVM_BUFFER_OVERFLOW;
  }
  
  // Additional bounds checking would require array size information
  return OVM_VALID;
}

ValidationResult ovm_validate_string_access(OrionVM* vm, const char* str, size_t length) {
  if (!vm || !str) return OVM_BUFFER_OVERFLOW;
  
  // Basic string validation
  for (size_t i = 0; i < length; i++) {
    if (str[i] == '\0' && i < length - 1) {
      // Null terminator found before end
      break;
    }
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_buffer_write(OrionVM* vm, void* buffer, size_t size, size_t offset) {
  if (!vm || !buffer) return OVM_BUFFER_OVERFLOW;
  
  // Basic buffer overflow protection
  if (offset >= size) {
    return OVM_BUFFER_OVERFLOW;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_memory_allocation(OrionVM* vm, size_t size) {
  if (!vm) return OVM_MEMORY_LIMIT_EXCEEDED;
  
  if (vm->memory_used + size > OVM_MAX_MEMORY_SIZE) {
    return OVM_MEMORY_LIMIT_EXCEEDED;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_call_depth(OrionVM* vm) {
  if (!vm) return OVM_CALL_DEPTH_EXCEEDED;
  
  if (vm->call_depth >= OVM_MAX_CALL_DEPTH - 1) {
    return OVM_CALL_DEPTH_EXCEEDED;
  }
  
  return OVM_VALID;
}

ValidationResult ovm_validate_variable_count(OrionVM* vm) {
  if (!vm) return OVM_MEMORY_LIMIT_EXCEEDED;
  
  if (vm->variable_count >= OVM_MAX_VARIABLES - 1) {
    return OVM_MEMORY_LIMIT_EXCEEDED;
  }
  
  return OVM_VALID;
}

bool ovm_is_numeric_type(orionpp_type_t type) {
  switch (type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
    case ORIONPP_TYPE_SSIZE:
      return true;
    default:
      return false;
  }
}

bool ovm_is_integer_type(orionpp_type_t type) {
  return ovm_is_numeric_type(type);
}

bool ovm_is_string_type(orionpp_type_t type) {
  return type == ORIONPP_TYPE_STRING;
}

bool ovm_can_convert_types(orionpp_type_t from, orionpp_type_t to) {
  if (from == to) return true;
  if (ovm_is_numeric_type(from) && ovm_is_numeric_type(to)) return true;
  return false;
}

const char* ovm_validation_result_to_string(ValidationResult result) {
  switch (result) {
    case OVM_VALID: return "Valid";
    case OVM_INVALID_INSTRUCTION: return "Invalid instruction";
    case OVM_INVALID_OPERAND: return "Invalid operand";
    case OVM_UNINITIALIZED_VARIABLE: return "Uninitialized variable";
    case OVM_TYPE_MISMATCH: return "Type mismatch";
    case OVM_BUFFER_OVERFLOW: return "Buffer overflow";
    case OVM_STACK_OVERFLOW: return "Stack overflow";
    case OVM_DIVISION_BY_ZERO: return "Division by zero";
    case OVM_INVALID_JUMP: return "Invalid jump";
    case OVM_MEMORY_LIMIT_EXCEEDED: return "Memory limit exceeded";
    case OVM_CALL_DEPTH_EXCEEDED: return "Call depth exceeded";
    case OVM_INVALID_VARIABLE_ID: return "Invalid variable ID";
    case OVM_INVALID_LABEL_ID: return "Invalid label ID";
    case OVM_INVALID_FUNCTION_CALL: return "Invalid function call";
    default: return "Unknown validation error";
  }
}

void ovm_report_validation_error(OrionVM* vm, ValidationResult result, const char* context) {
  if (!vm) return;
  
  const char* error_msg = ovm_validation_result_to_string(result);
  if (context) {
    ovm_error(vm, "%s: %s", context, error_msg);
  } else {
    ovm_error(vm, "%s", error_msg);
  }
}

void ovm_set_validation_level(OrionVM* vm, ValidationLevel level) {
  if (vm) {
    current_validation_level = level;
  }
}

ValidationLevel ovm_get_validation_level(OrionVM* vm) {
  return current_validation_level;
}