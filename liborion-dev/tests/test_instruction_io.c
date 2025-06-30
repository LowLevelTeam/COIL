#include "test_common.h"

// Instruction Tests
void test_instruction_functionality() {
  TEST_START("Instruction Functionality");
  
  // Test 1: Opcode string formatting
  TEST_CASE("Opcode string formatting");
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_ADD};
  char buf[64];
  size_t len = orionpp_string_opcode(buf, sizeof(buf), &opcode);
  TEST_ASSERT(len > 0 && strstr(buf, "ISA.ADD") != NULL, "Should format ISA.ADD opcode");
  
  // Test 2: Invalid opcode formatting
  TEST_CASE("Invalid opcode formatting");
  orionpp_opcode_t bad_opcode = {99, 99};
  len = orionpp_string_opcode(buf, sizeof(buf), &bad_opcode);
  TEST_ASSERT(len > 0 && strstr(buf, "UNKNOWN") != NULL, "Should format unknown opcode");
  
  // Test 3: Opcode formatting with NULL
  TEST_CASE("Opcode formatting with NULL");
  len = orionpp_string_opcode(NULL, sizeof(buf), &opcode);
  TEST_ASSERT(len == 0, "Should fail with NULL buffer");
  
  len = orionpp_string_opcode(buf, sizeof(buf), NULL);
  TEST_ASSERT(len == 0, "Should fail with NULL opcode");
  
  len = orionpp_string_opcode(buf, 0, &opcode);
  TEST_ASSERT(len == 0, "Should fail with zero buffer size");
  
  // Test 4: Type string formatting
  TEST_CASE("Type string formatting");
  orionpp_type_t type = {
    .base = {ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32},
    .count = 0,
    .types = NULL
  };
  len = orionpp_string_type(buf, sizeof(buf), &type);
  TEST_ASSERT(len > 0 && strstr(buf, "int32") != NULL, "Should format int32 type");
  
  // Test 5: Qualifier type formatting
  TEST_CASE("Qualifier type formatting");
  orionpp_type_t ptr_type = {
    .base = {ORIONPP_TYPE_QUAL, ORIONPP_TYPE_PTR},
    .count = 0,
    .types = NULL
  };
  len = orionpp_string_type(buf, sizeof(buf), &ptr_type);
  TEST_ASSERT(len > 0 && strstr(buf, "ptr") != NULL, "Should format ptr type");
  
  // Test 6: Value string formatting
  TEST_CASE("Value string formatting");
  int32_t test_value = 42;
  orionpp_value_t value;
  orionpp_error_t err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                          &test_value, sizeof(test_value));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  len = orionpp_string_value(buf, sizeof(buf), &value);
  TEST_ASSERT(len > 0 && strstr(buf, "42") != NULL && strstr(buf, "int32") != NULL, 
              "Should format value with type");
  
  // Clean up
  if (value.value) free(value.value);
  
  // Test 7: NULL value formatting
  TEST_CASE("NULL value formatting");
  orionpp_value_t null_value;
  err = create_test_value(&null_value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, NULL, 0);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "NULL test value creation should succeed");
  
  len = orionpp_string_value(buf, sizeof(buf), &null_value);
  TEST_ASSERT(len > 0 && strstr(buf, "NULL") != NULL, "Should format NULL value");
  
  // Test 8: Different integer types
  TEST_CASE("Different integer types");
  uint64_t test_u64 = 0xDEADBEEFCAFEBABE;
  orionpp_value_t u64_value;
  err = create_test_value(&u64_value, ORIONPP_TYPE_INT, ORIONPP_TYPE_UNT64, 
                          &test_u64, sizeof(test_u64));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "uint64 test value creation should succeed");
  
  len = orionpp_string_value(buf, sizeof(buf), &u64_value);
  TEST_ASSERT(len > 0 && strstr(buf, "uint64") != NULL, "Should format uint64 type");
  
  // Test 9: Instruction formatting
  TEST_CASE("Instruction formatting");
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 1,
    .values = &u64_value
  };
  char large_buf[512];
  len = orionpp_string_instr(large_buf, sizeof(large_buf), &instr);
  TEST_ASSERT(len > 0 && strstr(large_buf, "ISA.ADD") != NULL && strstr(large_buf, "uint64") != NULL,
              "Should format complete instruction");
  
  // Test 10: Instruction with multiple values
  TEST_CASE("Instruction with multiple values");
  int32_t test_value2 = 123;
  orionpp_value_t value2;
  err = create_test_value(&value2, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &test_value2, sizeof(test_value2));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Second test value creation should succeed");
  
  orionpp_value_t values[2] = {u64_value, value2};
  orionpp_instruction_t multi_instr = {
    .opcode = opcode,
    .value_count = 2,
    .values = values
  };
  len = orionpp_string_instr(large_buf, sizeof(large_buf), &multi_instr);
  TEST_ASSERT(len > 0 && strstr(large_buf, "ISA.ADD") != NULL && 
              strstr(large_buf, "123") != NULL && strstr(large_buf, "uint64") != NULL,
              "Should format instruction with multiple values");
  
  // Test 11: Print functions (visual verification)
  TEST_CASE("Print functions");
  printf("\n    Visual verification of print functions:\n");
  printf("    "); orionpp_print_opcode(&opcode);
  printf("    "); orionpp_print_type(&u64_value.type);
  printf("    "); orionpp_print_value(&u64_value);
  printf("    "); orionpp_print_instr(&instr);
  TEST_ASSERT(1, "Print functions executed (check output manually)");
  
  // Clean up test values
  if (u64_value.value) free(u64_value.value);
  if (value2.value) free(value2.value);
}

// File I/O Tests
void test_file_io() {
  TEST_START("File I/O");
  
#ifdef WIN32
  // Windows file I/O test
  TEST_CASE("Windows file I/O");
  HANDLE file = CreateFile("test_instruction.bin", GENERIC_WRITE, 0, NULL, 
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file == INVALID_HANDLE_VALUE) {
    TEST_ASSERT(0, "Failed to create test file");
    return;
  }
  
  // Create test instruction
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_MOV};
  int32_t test_data = 456;
  orionpp_value_t value;
  orionpp_error_t err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                          &test_data, sizeof(test_data));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 1,
    .values = &value
  };
  
  // Write instruction
  err = orionpp_writef(file, &instr);
  CloseHandle(file);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "File write should succeed");
  
  // Read instruction back
  file = CreateFile("test_instruction.bin", GENERIC_READ, 0, NULL, 
                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file == INVALID_HANDLE_VALUE) {
    TEST_ASSERT(0, "Failed to open test file for reading");
    if (value.value) free(value.value);
    return;
  }
  
  orionpp_instruction_t read_instr;
  err = orionpp_readf(file, &read_instr);
  CloseHandle(file);
  
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "File read should succeed");
  
  // Verify data
  TEST_ASSERT(read_instr.opcode.root == instr.opcode.root &&
              read_instr.opcode.module_ == instr.opcode.module_ &&
              read_instr.value_count == instr.value_count,
              "Read instruction should match written");
  
  if (read_instr.values && read_instr.values[0].value) {
    int32_t *read_data = (int32_t*)read_instr.values[0].value;
    TEST_ASSERT(*read_data == test_data, "Data should match");
  }
  
  // Cleanup
  cleanup_instruction(&read_instr);
  if (value.value) free(value.value);
  DeleteFile("test_instruction.bin");
  
#else
  // Unix file I/O test
  TEST_CASE("Unix file I/O");
  int file = open("test_instruction.bin", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (file == -1) {
    TEST_ASSERT(0, "Failed to create test file");
    return;
  }
  
  // Create test instruction
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_MOV};
  int32_t test_data = 456;
  orionpp_value_t value;
  orionpp_error_t err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                          &test_data, sizeof(test_data));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 1,
    .values = &value
  };
  
  // Write instruction
  err = orionpp_writef(file, &instr);
  close(file);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "File write should succeed");
  
  // Read instruction back
  file = open("test_instruction.bin", O_RDONLY);
  if (file == -1) {
    TEST_ASSERT(0, "Failed to open test file for reading");
    if (value.value) free(value.value);
    return;
  }
  
  orionpp_instruction_t read_instr;
  err = orionpp_readf(file, &read_instr);
  close(file);
  
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "File read should succeed");
  
  // Verify data
  TEST_ASSERT(read_instr.opcode.root == instr.opcode.root &&
              read_instr.opcode.module_ == instr.opcode.module_ &&
              read_instr.value_count == instr.value_count,
              "Read instruction should match written");
  
  if (read_instr.values && read_instr.values[0].value) {
    int32_t *read_data = (int32_t*)read_instr.values[0].value;
    TEST_ASSERT(*read_data == test_data, "Data should match");
  }
  
  // Cleanup
  cleanup_instruction(&read_instr);
  if (value.value) free(value.value);
  unlink("test_instruction.bin");
#endif

  // Test buffer I/O
  TEST_CASE("Buffer write/read");
  {
    orionpp_opcode_t buf_opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_ADD};
    int32_t buf_test_value = 789;
    orionpp_value_t buf_value;
    orionpp_error_t buf_err = create_test_value(&buf_value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                                &buf_test_value, sizeof(buf_test_value));
    TEST_ASSERT(buf_err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
    
    orionpp_instruction_t buf_instr = {
      .opcode = buf_opcode,
      .value_count = 1,
      .values = &buf_value
    };
    
    char io_buf[1024];
    buf_err = orionpp_writebuf(io_buf, sizeof(io_buf), &buf_instr);
    TEST_ASSERT(buf_err == ORIONPP_ERROR_GOOD, "Buffer write should succeed");
    
    orionpp_instruction_t buf_read_instr;
    memset(&buf_read_instr, 0, sizeof(buf_read_instr));
    buf_err = orionpp_readbuf(io_buf, sizeof(io_buf), &buf_read_instr);
    TEST_ASSERT(buf_err == ORIONPP_ERROR_GOOD, "Buffer read should succeed");
    
    TEST_ASSERT(buf_read_instr.opcode.root == buf_instr.opcode.root && 
                buf_read_instr.opcode.module_ == buf_instr.opcode.module_ &&
                buf_read_instr.value_count == buf_instr.value_count,
                "Read instruction should match written instruction");
    
    // Verify value data integrity
    if (buf_read_instr.value_count > 0 && buf_read_instr.values && buf_instr.values) {
      TEST_ASSERT(buf_read_instr.values[0].value_byte_size == buf_instr.values[0].value_byte_size,
                  "Value byte size should match");
      
      if (buf_read_instr.values[0].value && buf_instr.values[0].value) {
        int32_t *read_val = (int32_t*)buf_read_instr.values[0].value;
        int32_t *orig_val = (int32_t*)buf_instr.values[0].value;
        TEST_ASSERT(*read_val == *orig_val, "Value data should match");
      }
    }
    
    cleanup_instruction(&buf_read_instr);
    if (buf_value.value) free(buf_value.value);
  }

  // Test buffer overflow
  TEST_CASE("Buffer overflow protection");
  {
    orionpp_opcode_t overflow_opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_ADD};
    int32_t overflow_test_value = 999;
    orionpp_value_t overflow_value;
    orionpp_error_t overflow_err = create_test_value(&overflow_value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                                                     &overflow_test_value, sizeof(overflow_test_value));
    TEST_ASSERT(overflow_err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
    
    orionpp_instruction_t overflow_instr = {
      .opcode = overflow_opcode,
      .value_count = 1,
      .values = &overflow_value
    };
    
    char small_buf[10];
    overflow_err = orionpp_writebuf(small_buf, sizeof(small_buf), &overflow_instr);
    TEST_ASSERT(overflow_err == ORIONPP_ERROR_BUFFER_OVERFLOW, "Should detect buffer overflow");
    
    if (overflow_value.value) free(overflow_value.value);
  }
}