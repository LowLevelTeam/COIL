#include "test_framework.h"
#include "orionpp/instruction.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file test_framework.c
 * @brief Test framework implementation
 * 
 * Provides test statistics tracking and helper functions.
 */

// Test statistics globals
int test_count = 0;
int assert_total = 0;
int assert_passed = 0;
int assert_failed = 0;

void cleanup_instruction(orionpp_instruction_t *instr) {
  if (!instr) return;
  orionpp_instruction_destroy(instr);
}

orionpp_error_t create_test_value(orionpp_value_t *value, orionpp_type_root_t type_root, 
                                  orionpp_type_module_t type_module, void *data, size_t data_size) {
  if (!value) return ORIONPP_ERROR_INVALID_ARG;
  
  memset(value, 0, sizeof(orionpp_value_t));
  
  value->type.base.root = type_root;
  value->type.base.module_ = type_module;
  value->type.count = 0;
  value->type.types = NULL;
  
  if (data && data_size > 0) {
    value->value = malloc(data_size);
    if (!value->value) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    memcpy(value->value, data, data_size);
    value->value_byte_size = data_size;
  } else {
    value->value = NULL;
    value->value_byte_size = 0;
  }
  
  return ORIONPP_ERROR_GOOD;
}