#include "test_common.h"

// Test framework globals
int test_count = 0;
int assert_total = 0;
int assert_passed = 0;
int assert_failed = 0;

// Helper function to clean up instruction memory
void cleanup_instruction(orionpp_instruction_t *instr) {
  if (!instr) return;
  
  if (instr->values) {
    for (size_t i = 0; i < instr->value_count; i++) {
      if (instr->values[i].value) {
        free(instr->values[i].value);
        instr->values[i].value = NULL;
      }
      if (instr->values[i].type.types) {
        free(instr->values[i].type.types);
        instr->values[i].type.types = NULL;
      }
    }
    free(instr->values);
    instr->values = NULL;
  }
  instr->value_count = 0;
}

// Helper function to create a test value
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