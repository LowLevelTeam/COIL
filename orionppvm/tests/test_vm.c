/**
 * @file tests/test_vm.c
 * @brief Test program for the Orion++ Virtual Machine
 */

#include "vm.h"
#include "executor.h"
#include "validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test helper functions
static void test_vm_init_destroy() {
  printf("Testing VM initialization and destruction...\n");
  
  OrionVM vm;
  int result = ovm_init(&vm);
  assert(result == 0);
  assert(vm.instructions != NULL);
  assert(vm.variables != NULL);
  assert(vm.labels != NULL);
  assert(vm.call_stack != NULL);
  assert(vm.instruction_count == 0);
  assert(vm.variable_count == 0);
  assert(vm.label_count == 0);
  assert(vm.call_depth == 0);
  assert(vm.running == false);
  assert(vm.error == false);
  
  ovm_destroy(&vm);
  printf("✓ VM initialization and destruction test passed\n");
}

static void test_variable_management() {
  printf("Testing variable management...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  
  // Test creating variables
  VMVariable* var1 = ovm_create_variable(&vm, 1, ORIONPP_TYPE_WORD);
  assert(var1 != NULL);
  assert(var1->id == 1);
  assert(var1->type == ORIONPP_TYPE_WORD);
  assert(var1->is_initialized == false);
  
  VMVariable* var2 = ovm_create_variable(&vm, 2, ORIONPP_TYPE_STRING);
  assert(var2 != NULL);
  assert(var2->id == 2);
  assert(var2->type == ORIONPP_TYPE_STRING);
  
  // Test finding variables
  VMVariable* found = ovm_get_variable(&vm, 1);
  assert(found == var1);
  
  found = ovm_get_variable(&vm, 999);
  assert(found == NULL);
  
  // Test setting variable values
  int32_t int_value = 42;
  int result = ovm_set_variable_value(&vm, 1, &int_value, sizeof(int_value));
  assert(result == 0);
  assert(var1->is_initialized == true);
  assert(var1->value.i64 == 42);
  
  const char* str_value = "Hello, World!";
  result = ovm_set_variable_value(&vm, 2, str_value, strlen(str_value));
  assert(result == 0);
  assert(var2->is_initialized == true);
  assert(strcmp(var2->value.str, "Hello, World!") == 0);
  
  ovm_destroy(&vm);
  printf("✓ Variable management test passed\n");
}

static void test_label_management() {
  printf("Testing label management...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  
  // Test registering labels
  int result = ovm_register_label(&vm, 100, 5);
  assert(result == 0);
  
  result = ovm_register_label(&vm, 200, 10);
  assert(result == 0);
  
  // Test finding labels
  size_t addr = ovm_find_label(&vm, 100);
  assert(addr == 5);
  
  addr = ovm_find_label(&vm, 200);
  assert(addr == 10);
  
  addr = ovm_find_label(&vm, 999);
  assert(addr == SIZE_MAX);
  
  ovm_destroy(&vm);
  printf("✓ Label management test passed\n");
}

static void test_validation() {
  printf("Testing validation system...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  
  // Test validation levels
  ovm_set_validation_level(&vm, OVM_VALIDATE_STRICT);
  assert(ovm_get_validation_level(&vm) == OVM_VALIDATE_STRICT);
  
  // Test validation results
  const char* msg = ovm_validation_result_to_string(OVM_VALID);
  assert(strcmp(msg, "Valid") == 0);
  
  msg = ovm_validation_result_to_string(OVM_DIVISION_BY_ZERO);
  assert(strcmp(msg, "Division by zero") == 0);
  
  // Test variable validation
  VMVariable* var = ovm_create_variable(&vm, 1, ORIONPP_TYPE_WORD);
  ValidationResult result = ovm_validate_variable_initialization(&vm, var);
  assert(result == OVM_UNINITIALIZED_VARIABLE);
  
  var->is_initialized = true;
  var->value.i64 = 10;
  result = ovm_validate_variable_initialization(&vm, var);
  assert(result == OVM_VALID);
  
  // Test division validation
  result = ovm_validate_division(&vm, var);
  assert(result == OVM_VALID);
  
  var->value.i64 = 0;
  result = ovm_validate_division(&vm, var);
  assert(result == OVM_DIVISION_BY_ZERO);
  
  ovm_destroy(&vm);
  printf("✓ Validation system test passed\n");
}

static void test_value_extraction() {
  printf("Testing value extraction...\n");
  
  // Test variable ID extraction
  uint32_t var_id = 123;
  orinopp_value_t value;
  value.root = ORIONPP_TYPE_VARID;
  value.bytes = (char*)&var_id;
  value.bytesize = sizeof(var_id);
  
  orionpp_variable_id_t extracted_id;
  int result = ovm_extract_variable_id(&value, &extracted_id);
  assert(result == 0);
  assert(extracted_id == 123);
  
  // Test invalid extraction
  value.root = ORIONPP_TYPE_STRING;
  result = ovm_extract_variable_id(&value, &extracted_id);
  assert(result == -1);
  
  // Test label ID extraction
  uint32_t label_id = 456;
  value.root = ORIONPP_TYPE_LABELID;
  value.bytes = (char*)&label_id;
  value.bytesize = sizeof(label_id);
  
  orionpp_label_id_t extracted_label;
  result = ovm_extract_label_id(&value, &extracted_label);
  assert(result == 0);
  assert(extracted_label == 456);
  
  // Test integer extraction
  int32_t int_val = 789;
  value.root = ORIONPP_TYPE_WORD;
  value.bytes = (char*)&int_val;
  value.bytesize = sizeof(int_val);
  
  int64_t extracted_int;
  result = ovm_extract_integer(&value, &extracted_int);
  assert(result == 0);
  assert(extracted_int == 789);
  
  // Test string extraction
  const char* str = "test string";
  value.root = ORIONPP_TYPE_STRING;
  value.bytes = (char*)str;
  value.bytesize = strlen(str);
  
  char* extracted_str;
  result = ovm_extract_string(&value, &extracted_str);
  assert(result == 0);
  assert(strcmp(extracted_str, "test string") == 0);
  free(extracted_str);
  
  printf("✓ Value extraction test passed\n");
}

static void test_simple_program() {
  printf("Testing simple program execution...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  ovm_set_debug_mode(&vm, true, stdout);
  
  // Create a simple program manually
  // var 0 WORD
  // const 0 WORD 42
  // ret 0
  
  vm.instruction_count = 3;
  vm.instructions = realloc(vm.instructions, 3 * sizeof(orinopp_instruction_t));
  
  // Instruction 0: var 0 WORD
  orinopp_instruction_t* instr = &vm.instructions[0];
  instr->root = ORIONPP_OP_ISA;
  instr->child = ORIONPP_OP_ISA_VAR;
  instr->value_count = 2;
  instr->values = malloc(2 * sizeof(orinopp_value_t));
  
  uint32_t var_id = 0;
  instr->values[0].root = ORIONPP_TYPE_VARID;
  instr->values[0].child = 0;
  instr->values[0].bytes = malloc(sizeof(var_id));
  memcpy(instr->values[0].bytes, &var_id, sizeof(var_id));
  instr->values[0].bytesize = sizeof(var_id);
  
  instr->values[1].root = ORIONPP_TYPE_WORD;
  instr->values[1].child = 0;
  instr->values[1].bytes = NULL;
  instr->values[1].bytesize = 0;
  
  // Instruction 1: const 0 WORD 42
  instr = &vm.instructions[1];
  instr->root = ORIONPP_OP_ISA;
  instr->child = ORIONPP_OP_ISA_CONST;
  instr->value_count = 3;
  instr->values = malloc(3 * sizeof(orinopp_value_t));
  
  instr->values[0].root = ORIONPP_TYPE_VARID;
  instr->values[0].child = 0;
  instr->values[0].bytes = malloc(sizeof(var_id));
  memcpy(instr->values[0].bytes, &var_id, sizeof(var_id));
  instr->values[0].bytesize = sizeof(var_id);
  
  instr->values[1].root = ORIONPP_TYPE_WORD;
  instr->values[1].child = 0;
  instr->values[1].bytes = NULL;
  instr->values[1].bytesize = 0;
  
  int32_t const_val = 42;
  instr->values[2].root = ORIONPP_TYPE_WORD;
  instr->values[2].child = 0;
  instr->values[2].bytes = malloc(sizeof(const_val));
  memcpy(instr->values[2].bytes, &const_val, sizeof(const_val));
  instr->values[2].bytesize = sizeof(const_val);
  
  // Instruction 2: ret 0
  instr = &vm.instructions[2];
  instr->root = ORIONPP_OP_ISA;
  instr->child = ORIONPP_OP_ISA_RET;
  instr->value_count = 1;
  instr->values = malloc(1 * sizeof(orinopp_value_t));
  
  instr->values[0].root = ORIONPP_TYPE_VARID;
  instr->values[0].child = 0;
  instr->values[0].bytes = malloc(sizeof(var_id));
  memcpy(instr->values[0].bytes, &var_id, sizeof(var_id));
  instr->values[0].bytesize = sizeof(var_id);
  
  // Validate and run program
  ValidationResult validation = ovm_validate_program(&vm);
  if (validation != OVM_VALID) {
    printf("Program validation failed: %s\n", ovm_validation_result_to_string(validation));
  }
  
  printf("Running simple program...\n");
  int result = ovm_run(&vm);
  
  if (result == 0) {
    printf("Program executed successfully\n");
    printf("Return value: %lld\n", (long long)vm.return_value.value.i64);
    assert(vm.return_value.is_initialized);
    assert(vm.return_value.value.i64 == 42);
  } else {
    printf("Program execution failed: %s\n", ovm_get_error(&vm));
  }
  
  ovm_destroy(&vm);
  printf("✓ Simple program test passed\n");
}

static void test_type_system() {
  printf("Testing type system...\n");
  
  // Test type checking functions
  assert(ovm_is_numeric_type(ORIONPP_TYPE_WORD) == true);
  assert(ovm_is_numeric_type(ORIONPP_TYPE_SIZE) == true);
  assert(ovm_is_numeric_type(ORIONPP_TYPE_STRING) == false);
  
  assert(ovm_is_integer_type(ORIONPP_TYPE_WORD) == true);
  assert(ovm_is_integer_type(ORIONPP_TYPE_STRING) == false);
  
  assert(ovm_is_string_type(ORIONPP_TYPE_STRING) == true);
  assert(ovm_is_string_type(ORIONPP_TYPE_WORD) == false);
  
  // Test type compatibility
  assert(ovm_types_compatible(ORIONPP_TYPE_WORD, ORIONPP_TYPE_WORD) == true);
  assert(ovm_types_compatible(ORIONPP_TYPE_WORD, ORIONPP_TYPE_SIZE) == true);
  assert(ovm_types_compatible(ORIONPP_TYPE_WORD, ORIONPP_TYPE_STRING) == false);
  
  // Test type conversion
  OrionVM vm;
  ovm_init(&vm);
  
  VMVariable* src = ovm_create_variable(&vm, 1, ORIONPP_TYPE_WORD);
  VMVariable* dest = ovm_create_variable(&vm, 2, ORIONPP_TYPE_SIZE);
  
  src->value.i64 = 123;
  src->is_initialized = true;
  
  int result = ovm_convert_value(dest, src, ORIONPP_TYPE_SIZE);
  assert(result == 0);
  assert(dest->is_initialized == true);
  assert(dest->value.i64 == 123);
  
  ovm_destroy(&vm);
  printf("✓ Type system test passed\n");
}

static void test_error_handling() {
  printf("Testing error handling...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  
  // Test error setting and getting
  assert(ovm_has_error(&vm) == false);
  assert(strlen(ovm_get_error(&vm)) == 0);
  
  ovm_error(&vm, "Test error: %d", 42);
  assert(ovm_has_error(&vm) == true);
  assert(strstr(ovm_get_error(&vm), "Test error: 42") != NULL);
  assert(vm.running == false);
  
  ovm_destroy(&vm);
  printf("✓ Error handling test passed\n");
}

static void test_memory_safety() {
  printf("Testing memory safety...\n");
  
  OrionVM vm;
  ovm_init(&vm);
  
  // Test memory usage tracking
  size_t initial_memory = vm.memory_used;
  assert(initial_memory > 0);
  
  // Test memory limit validation
  ValidationResult result = ovm_validate_memory_usage(&vm);
  assert(result == OVM_VALID);
  
  // Test memory allocation validation
  result = ovm_validate_memory_allocation(&vm, 1000);
  assert(result == OVM_VALID);
  
  result = ovm_validate_memory_allocation(&vm, OVM_MAX_MEMORY_SIZE);
  assert(result == OVM_MEMORY_LIMIT_EXCEEDED);
  
  // Test variable count limits
  result = ovm_validate_variable_count(&vm);
  assert(result == OVM_VALID);
  
  // Test call depth limits
  result = ovm_validate_call_depth(&vm);
  assert(result == OVM_VALID);
  
  ovm_destroy(&vm);
  printf("✓ Memory safety test passed\n");
}

int main() {
  printf("Running Orion++ Virtual Machine Tests\n");
  printf("=====================================\n\n");
  
  test_vm_init_destroy();
  test_variable_management();
  test_label_management();
  test_validation();
  test_value_extraction();
  test_simple_program();
  test_type_system();
  test_error_handling();
  test_memory_safety();
  
  printf("\n=====================================\n");
  printf("All VM tests completed successfully! ✓\n");
  
  return 0;
}