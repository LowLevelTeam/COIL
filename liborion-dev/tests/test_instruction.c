#include "test_framework.h"
#include "orionpp/instruction.h"

/**
 * @file test_instruction.c
 * @brief Instruction handling tests
 * 
 * Tests instruction creation, formatting, serialization, and I/O operations.
 */

void test_instruction_core() {
  TEST_START("Instruction Core Operations");
  
  orionpp_instruction_t instr;
  orionpp_error_t err;
  
  TEST_CASE("Instruction initialization");
  err = orionpp_instruction_init(&instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Init should succeed");
  
  TEST_CASE("Instruction copy");
  orionpp_instruction_t src, dest;
  orionpp_instruction_init(&src);
  src.opcode.root = ORIONPP_OPCODE_ISA;
  src.opcode.module_ = ORIONPP_OP_ISA_ADD;
  
  err = orionpp_instruction_copy(&dest, &src);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Copy should succeed");
  TEST_ASSERT(dest.opcode.root == src.opcode.root && 
              dest.opcode.module_ == src.opcode.module_, "Copy should preserve opcode");
  
  TEST_CASE("Instruction destruction");
  err = orionpp_instruction_destroy(&instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Destruction should succeed");
  
  orionpp_instruction_destroy(&src);
  orionpp_instruction_destroy(&dest);
}

void test_instruction_formatting() {
  TEST_START("Instruction Formatting");
  
  char buf[256];
  size_t len;
  
  TEST_CASE("Opcode formatting");
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_ADD};
  len = orionpp_string_opcode(buf, sizeof(buf), &opcode);
  TEST_ASSERT(len > 0 && strstr(buf, "ISA.ADD") != NULL, "Should format ISA.ADD opcode");
  
  TEST_CASE("Type formatting");
  orionpp_type_t type = {
    .base = {ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32},
    .count = 0,
    .types = NULL
  };
  len = orionpp_string_type(buf, sizeof(buf), &type);
  TEST_ASSERT(len > 0 && strstr(buf, "int32") != NULL, "Should format int32 type");
  
  TEST_CASE("Value formatting");
  int32_t test_value = 42;
  orionpp_value_t value;
  orionpp_error_t err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                          &test_value, sizeof(test_value));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  len = orionpp_string_value(buf, sizeof(buf), &value);
  TEST_ASSERT(len > 0 && strstr(buf, "42") != NULL && strstr(buf, "int32") != NULL, 
              "Should format value with type");
  
  if (value.value) free(value.value);
  
  TEST_CASE("Instruction formatting");
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 0,
    .values = NULL
  };
  len = orionpp_string_instr(buf, sizeof(buf), &instr);
  TEST_ASSERT(len > 0 && strstr(buf, "ISA.ADD") != NULL, "Should format instruction");
}

void test_instruction_serialization() {
  TEST_START("Instruction Serialization");
  
  orionpp_instruction_t src, dest;
  orionpp_error_t err;
  char buffer[1024];
  
  TEST_CASE("Simple instruction serialization");
  orionpp_instruction_init(&src);
  src.opcode.root = ORIONPP_OPCODE_ISA;
  src.opcode.module_ = ORIONPP_OP_ISA_NOP;
  src.value_count = 0;
  src.values = NULL;
  
  err = orionpp_serialize_instr(buffer, sizeof(buffer), &src);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Serialization should succeed");
  
  err = orionpp_deserialize_instr(buffer, sizeof(buffer), &dest);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Deserialization should succeed");
  TEST_ASSERT(dest.opcode.root == src.opcode.root && 
              dest.opcode.module_ == src.opcode.module_, "Should preserve opcode");
  
  orionpp_instruction_destroy(&src);
  orionpp_instruction_destroy(&dest);
  
  TEST_CASE("Instruction with values serialization");
  orionpp_instruction_init(&src);
  src.opcode.root = ORIONPP_OPCODE_ISA;
  src.opcode.module_ = ORIONPP_OP_ISA_MOV;
  src.value_count = 1;
  src.values = malloc(sizeof(orionpp_value_t));
  
  int32_t test_data = 123;
  err = create_test_value(&src.values[0], ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &test_data, sizeof(test_data));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  err = orionpp_serialize_instr(buffer, sizeof(buffer), &src);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Serialization with values should succeed");
  
  err = orionpp_deserialize_instr(buffer, sizeof(buffer), &dest);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Deserialization with values should succeed");
  
  TEST_ASSERT(dest.value_count == src.value_count, "Should preserve value count");
  if (dest.values && dest.values[0].value && src.values[0].value) {
    int32_t *dest_data = (int32_t*)dest.values[0].value;
    int32_t *src_data = (int32_t*)src.values[0].value;
    TEST_ASSERT(*dest_data == *src_data, "Should preserve value data");
  }
  
  orionpp_instruction_destroy(&src);
  orionpp_instruction_destroy(&dest);
}

void test_instruction_errors() {
  TEST_START("Instruction Error Conditions");
  
  orionpp_instruction_t instr;
  orionpp_error_t err;
  char buf[64];
  
  TEST_CASE("NULL pointer handling");
  err = orionpp_instruction_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Init with NULL should fail");
  
  err = orionpp_instruction_destroy(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Destroy with NULL should fail");
  
  size_t len = orionpp_string_opcode(NULL, sizeof(buf), NULL);
  TEST_ASSERT(len == 0, "String formatting with NULL should fail");
  
  TEST_CASE("Buffer overflow protection");
  orionpp_instruction_init(&instr);
  instr.opcode.root = ORIONPP_OPCODE_ISA;
  instr.opcode.module_ = ORIONPP_OP_ISA_ADD;
  instr.value_count = 1;
  instr.values = malloc(sizeof(orionpp_value_t));
  
  // Create a large value to trigger overflow
  size_t large_size = 1024;
  void *large_data = malloc(large_size);
  memset(large_data, 0xAB, large_size);
  
  err = create_test_value(&instr.values[0], ORIONPP_TYPE_INT, ORIONPP_TYPE_UNT8, 
                          large_data, large_size);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Large value creation should succeed");
  
  char small_buf[10];
  err = orionpp_serialize_instr(small_buf, sizeof(small_buf), &instr);
  TEST_ASSERT(err == ORIONPP_ERROR_BUFFER_OVERFLOW, "Should detect buffer overflow");
  
  free(large_data);
  orionpp_instruction_destroy(&instr);
}