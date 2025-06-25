/**
 * @file include/validator.h
 * @brief Orion++ runtime safety validator definitions
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "vm.h"

// Validation levels
typedef enum {
  OVM_VALIDATE_NONE = 0,
  OVM_VALIDATE_BASIC = 1,
  OVM_VALIDATE_STRICT = 2,
  OVM_VALIDATE_PARANOID = 3
} ValidationLevel;

// Validation results
typedef enum {
  OVM_VALID = 0,
  OVM_INVALID_INSTRUCTION = 1,
  OVM_INVALID_OPERAND = 2,
  OVM_UNINITIALIZED_VARIABLE = 3,
  OVM_TYPE_MISMATCH = 4,
  OVM_BUFFER_OVERFLOW = 5,
  OVM_STACK_OVERFLOW = 6,
  OVM_DIVISION_BY_ZERO = 7,
  OVM_INVALID_JUMP = 8,
  OVM_MEMORY_LIMIT_EXCEEDED = 9,
  OVM_CALL_DEPTH_EXCEEDED = 10,
  OVM_INVALID_VARIABLE_ID = 11,
  OVM_INVALID_LABEL_ID = 12,
  OVM_INVALID_FUNCTION_CALL = 13
} ValidationResult;

// Program validation (static analysis)
ValidationResult ovm_validate_program(OrionVM* vm);
ValidationResult ovm_validate_instruction(OrionVM* vm, const orinopp_instruction_t* instr, size_t index);
ValidationResult ovm_validate_labels(OrionVM* vm);
ValidationResult ovm_validate_variables(OrionVM* vm);

// Runtime validation (dynamic checks)
ValidationResult ovm_validate_execution_safety(OrionVM* vm);
ValidationResult ovm_validate_variable_access(OrionVM* vm, orionpp_variable_id_t id);
ValidationResult ovm_validate_label_jump(OrionVM* vm, orionpp_label_id_t id);
ValidationResult ovm_validate_function_call(OrionVM* vm, const char* function_name);
ValidationResult ovm_validate_memory_usage(OrionVM* vm);
ValidationResult ovm_validate_stack_usage(OrionVM* vm);

// Arithmetic validation
ValidationResult ovm_validate_division(OrionVM* vm, const VMVariable* divisor);
ValidationResult ovm_validate_type_operation(OrionVM* vm, const VMVariable* var1, const VMVariable* var2, orionpp_opcode_module_t op);
ValidationResult ovm_validate_variable_initialization(OrionVM* vm, const VMVariable* var);

// Bounds checking
ValidationResult ovm_validate_array_access(OrionVM* vm, const VMVariable* array, int64_t index);
ValidationResult ovm_validate_string_access(OrionVM* vm, const char* str, size_t length);
ValidationResult ovm_validate_buffer_write(OrionVM* vm, void* buffer, size_t size, size_t offset);

// Resource limits
ValidationResult ovm_validate_memory_allocation(OrionVM* vm, size_t size);
ValidationResult ovm_validate_call_depth(OrionVM* vm);
ValidationResult ovm_validate_variable_count(OrionVM* vm);

// Type safety
bool ovm_is_numeric_type(orionpp_type_t type);
bool ovm_is_integer_type(orionpp_type_t type);
bool ovm_is_string_type(orionpp_type_t type);
bool ovm_can_convert_types(orionpp_type_t from, orionpp_type_t to);

// Error reporting
const char* ovm_validation_result_to_string(ValidationResult result);
void ovm_report_validation_error(OrionVM* vm, ValidationResult result, const char* context);

// Validation configuration
void ovm_set_validation_level(OrionVM* vm, ValidationLevel level);
ValidationLevel ovm_get_validation_level(OrionVM* vm);

#endif // VALIDATOR_H