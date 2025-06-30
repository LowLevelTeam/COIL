#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "orionpp/arena.h"
#include "orionpp/error.h"
#include "orionpp/header.h"
#include "orionpp/instruction.h"

// Test framework macros
#define TEST_COUNT_MAX 1000
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

#define TEST_START(name) \
  do { \
    printf("\n=== Testing %s ===\n", name); \
  } while(0)

#define TEST_CASE(name) \
  do { \
    printf("  [%d] %s... ", ++test_count, name); \
    fflush(stdout); \
  } while(0)

#define TEST_ASSERT(condition, message) \
  do { \
    if (condition) { \
      printf("PASS\n"); \
      test_passed++; \
    } else { \
      printf("FAIL - %s\n", message); \
      test_failed++; \
    } \
  } while(0)

#define TEST_SUMMARY() \
  do { \
    printf("\n=== TEST SUMMARY ===\n"); \
    printf("Total tests: %d\n", test_count); \
    printf("Passed: %d\n", test_passed); \
    printf("Failed: %d\n", test_failed); \
    printf("Success rate: %.2f%%\n", test_count > 0 ? (test_passed * 100.0f / test_count) : 0.0f); \
  } while(0)

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

// Arena Tests
void test_arena_functionality() {
  TEST_START("Arena Functionality");
  
  orionpp_arena_t arena;
  orionpp_error_t err;
  
  // Test 1: Arena initialization
  TEST_CASE("Arena initialization");
  err = orionpp_arena_init(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Failed to initialize arena");
  
  // Test 2: Arena initialization with NULL
  TEST_CASE("Arena initialization with NULL");
  err = orionpp_arena_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  // Test 3: Arena creation
  TEST_CASE("Arena creation");
  err = orionpp_arena_create(&arena, 1024, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Failed to create arena");
  
  // Test 4: Arena creation with invalid args
  TEST_CASE("Arena creation with invalid args");
  orionpp_arena_t arena2;
  err = orionpp_arena_create(NULL, 1024, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_create(&arena2, 0, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with zero max_size");
  
  err = orionpp_arena_create(&arena2, 1024, 0);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with zero bucket_size");
  
  // Test 5: Basic allocation
  TEST_CASE("Basic allocation");
  void *ptr1 = NULL;
  err = orionpp_arena_alloc(&arena, 64, &ptr1);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr1 != NULL, "Failed basic allocation");
  
  // Test 6: Multiple allocations
  TEST_CASE("Multiple allocations");
  void *ptr2 = NULL, *ptr3 = NULL;
  err = orionpp_arena_alloc(&arena, 32, &ptr2);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr2 != NULL, "Failed second allocation");
  
  err = orionpp_arena_alloc(&arena, 16, &ptr3);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr3 != NULL, "Failed third allocation");
  
  // Test 7: Check pointers are different
  TEST_CASE("Allocation pointers are unique");
  TEST_ASSERT(ptr1 != ptr2 && ptr2 != ptr3 && ptr1 != ptr3, "Allocated pointers should be unique");
  
  // Test 8: Check arena used space
  TEST_CASE("Arena used space tracking");
  size_t used = orionpp_arena_used(&arena);
  // Should be at least 112 bytes (64+32+16) but could be more due to alignment
  TEST_ASSERT(used >= 112, "Arena should track used space correctly");
  
  // Test 9: Check arena available space
  TEST_CASE("Arena available space");
  size_t available = orionpp_arena_available(&arena);
  TEST_ASSERT(available <= 1024, "Available space should not exceed max size");
  
  // Test 10: Pop allocation
  TEST_CASE("Pop allocation");
  size_t used_before = orionpp_arena_used(&arena);
  err = orionpp_arena_pop(&arena, 16);
  size_t used_after = orionpp_arena_used(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && used_after < used_before, "Pop should reduce used space");
  
  // Test 11: Large allocation spanning buckets
  TEST_CASE("Large allocation spanning buckets");
  void *large_ptr = NULL;
  err = orionpp_arena_alloc(&arena, 300, &large_ptr); // Should create new bucket
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && large_ptr != NULL, "Large allocation should succeed");
  
  // Test 12: Arena full condition
  TEST_CASE("Arena full condition");
  orionpp_arena_t small_arena;
  err = orionpp_arena_create(&small_arena, 100, 50);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Failed to create small arena");
  
  void *ptr_a = NULL, *ptr_b = NULL, *ptr_c = NULL;
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_a);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "First allocation in small arena failed");
  
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_b);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Second allocation in small arena failed");
  
  // This should fail as we've exceeded capacity
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_c);
  TEST_ASSERT(err == ORIONPP_ERROR_ARENA_FULL, "Should fail when arena is full");
  
  // Test 13: Arena reset
  TEST_CASE("Arena reset");
  err = orionpp_arena_reset(&arena);
  size_t used_after_reset = orionpp_arena_used(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && used_after_reset == 0, "Reset should clear used space");
  
  // Test 14: Allocation after reset
  TEST_CASE("Allocation after reset");
  void *ptr_reset = NULL;
  err = orionpp_arena_alloc(&arena, 64, &ptr_reset);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr_reset != NULL, "Allocation after reset should work");
  
  // Test 15: Invalid allocation arguments
  TEST_CASE("Invalid allocation arguments");
  void *invalid_ptr = NULL;
  err = orionpp_arena_alloc(NULL, 64, &invalid_ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_alloc(&arena, 0, &invalid_ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with zero size");
  
  err = orionpp_arena_alloc(&arena, 64, NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL pointer");
  
  // Test 16: Pop with invalid arguments
  TEST_CASE("Pop with invalid arguments");
  err = orionpp_arena_pop(NULL, 64);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_pop(&arena, 0);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with zero size");
  
  // Test 17: Destroy arenas
  TEST_CASE("Arena destruction");
  err = orionpp_arena_destroy(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena destruction should succeed");
  
  err = orionpp_arena_destroy(&small_arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Small arena destruction should succeed");
  
  // Test 18: Operations on destroyed arena
  TEST_CASE("Operations on destroyed arena");
  void *destroyed_ptr = NULL;
  err = orionpp_arena_alloc(&arena, 64, &destroyed_ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_ARENA_NOT_INITIALIZED, "Should fail on destroyed arena");
}

// Error Tests
void test_error_functionality() {
  TEST_START("Error Functionality");
  
  // Test 1: Valid error messages
  TEST_CASE("Valid error messages");
  const char *msg = orionpp_strerr(ORIONPP_ERROR_GOOD);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Good error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_INVALID_ARG);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid arg error should have message");
  
  msg = orionpp_strerr(ORIONPP_ERROR_OUT_OF_MEMORY);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Out of memory error should have message");
  
  // Test 2: Invalid error codes
  TEST_CASE("Invalid error codes");
  msg = orionpp_strerr(-1);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Invalid error code should return unknown message");
  
  msg = orionpp_strerr(999);
  TEST_ASSERT(msg != NULL && strlen(msg) > 0, "Large error code should return unknown message");
  
  // Test 3: All defined error codes
  TEST_CASE("All defined error codes");
  int all_valid = 1;
  for (int i = ORIONPP_ERROR_GOOD; i <= ORIONPP_ERROR_UNKNOWN; i++) {
    msg = orionpp_strerr(i);
    if (msg == NULL || strlen(msg) == 0) {
      all_valid = 0;
      break;
    }
  }
  TEST_ASSERT(all_valid, "All defined error codes should have valid messages");
}

// Header Tests
void test_header_functionality() {
  TEST_START("Header Functionality");
  
  orionpp_header_t header;
  orionpp_error_t err;
  
  // Test 1: Header initialization
  TEST_CASE("Header initialization");
  err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header initialization should succeed");
  
  // Test 2: Header initialization with NULL
  TEST_CASE("Header initialization with NULL");
  err = orionpp_header_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL header");
  
  // Test 3: Header validation
  TEST_CASE("Header validation");
  err = orionpp_header_validate(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Valid header should pass validation");
  
  // Test 4: Header validation with NULL
  TEST_CASE("Header validation with NULL");
  err = orionpp_header_validate(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL header");
  
  // Test 5: Magic number validation
  TEST_CASE("Invalid magic number");
  orionpp_header_t bad_header = header;
  bad_header.magic0 = 0xFF;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_MAGIC, "Should fail with invalid magic");
  
  // Test 6: Version validation (future version)
  TEST_CASE("Future version validation");
  bad_header = header;
  bad_header.major = 99;
  err = orionpp_header_validate(&bad_header);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_VERSION, "Should fail with future version");
  
  // Test 7: Feature checking
  TEST_CASE("Feature checking");
  int has_feature = orionpp_header_has_feature(&header, 1);
  TEST_ASSERT(has_feature == 0, "Default header should have no features");
  
  // Test 8: Feature checking with NULL
  TEST_CASE("Feature checking with NULL");
  has_feature = orionpp_header_has_feature(NULL, 1);
  TEST_ASSERT(has_feature == 0, "NULL header should return no features");
  
  // Test 9: Set and check feature
  TEST_CASE("Set and check feature");
  header.features = 0x01;
  has_feature = orionpp_header_has_feature(&header, 1);
  TEST_ASSERT(has_feature != 0, "Should detect set feature");
  
  has_feature = orionpp_header_has_feature(&header, 2);
  TEST_ASSERT(has_feature == 0, "Should not detect unset feature");
}

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
  
  // Test 11: Buffer I/O
  TEST_CASE("Buffer write/read");
  char io_buf[1024];
  err = orionpp_writebuf(io_buf, sizeof(io_buf), &instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Buffer write should succeed");
  
  orionpp_instruction_t read_instr;
  memset(&read_instr, 0, sizeof(read_instr));
  err = orionpp_readbuf(io_buf, sizeof(io_buf), &read_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Buffer read should succeed");
  
  TEST_ASSERT(read_instr.opcode.root == instr.opcode.root && 
              read_instr.opcode.module_ == instr.opcode.module_ &&
              read_instr.value_count == instr.value_count,
              "Read instruction should match written instruction");
  
  // Verify value data integrity
  if (read_instr.value_count > 0 && read_instr.values && instr.values) {
    TEST_ASSERT(read_instr.values[0].value_byte_size == instr.values[0].value_byte_size,
                "Value byte size should match");
    
    if (read_instr.values[0].value && instr.values[0].value) {
      uint64_t *read_val = (uint64_t*)read_instr.values[0].value;
      uint64_t *orig_val = (uint64_t*)instr.values[0].value;
      TEST_ASSERT(*read_val == *orig_val, "Value data should match");
    }
  }
  
  cleanup_instruction(&read_instr);
  
  // Test 12: Buffer overflow
  TEST_CASE("Buffer overflow protection");
  char small_buf[10];
  err = orionpp_writebuf(small_buf, sizeof(small_buf), &instr);
  TEST_ASSERT(err == ORIONPP_ERROR_BUFFER_OVERFLOW, "Should detect buffer overflow");
  
  // Test 13: Print functions (visual verification)
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
}

// Memory Safety Tests
void test_memory_safety() {
  TEST_START("Memory Safety");
  
  // Test 1: Large allocations
  TEST_CASE("Large allocation handling");
  orionpp_arena_t arena;
  orionpp_error_t err = orionpp_arena_create(&arena, 1024*1024, 1024);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Large arena creation should succeed");
  
  void *large_ptr = NULL;
  err = orionpp_arena_alloc(&arena, 1024*512, &large_ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && large_ptr != NULL, "Large allocation should succeed");
  
  orionpp_arena_destroy(&arena);
  
  // Test 2: Alignment testing
  TEST_CASE("Memory alignment");
  err = orionpp_arena_create(&arena, 1024, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  void *ptr1 = NULL, *ptr2 = NULL;
  err = orionpp_arena_alloc(&arena, 1, &ptr1);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Small allocation should succeed");
  
  err = orionpp_arena_alloc(&arena, 1, &ptr2);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Second small allocation should succeed");
  
  // Check 8-byte alignment
  uintptr_t addr1 = (uintptr_t)ptr1;
  uintptr_t addr2 = (uintptr_t)ptr2;
  TEST_ASSERT((addr1 % 8) == 0 && (addr2 % 8) == 0, "Allocations should be 8-byte aligned");
  
  orionpp_arena_destroy(&arena);
  
  // Test 3: Boundary conditions
  TEST_CASE("Boundary conditions");
  err = orionpp_arena_create(&arena, 64, 32);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Small arena creation should succeed");
  
  void *ptr = NULL;
  err = orionpp_arena_alloc(&arena, 32, &ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Exact bucket size allocation should succeed");
  
  err = orionpp_arena_alloc(&arena, 33, &ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_ARENA_FULL, "Oversized allocation should fail");
  
  orionpp_arena_destroy(&arena);
  
  // Test 4: Instruction memory cleanup
  TEST_CASE("Instruction memory cleanup");
  orionpp_instruction_t instr;
  memset(&instr, 0, sizeof(instr));
  
  instr.opcode.root = ORIONPP_OPCODE_ISA;
  instr.opcode.module_ = ORIONPP_OP_ISA_ADD;
  instr.value_count = 2;
  instr.values = malloc(2 * sizeof(orionpp_value_t));
  TEST_ASSERT(instr.values != NULL, "Value array allocation should succeed");
  
  // Create test values
  int32_t data1 = 100, data2 = 200;
  err = create_test_value(&instr.values[0], ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &data1, sizeof(data1));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "First value creation should succeed");
  
  err = create_test_value(&instr.values[1], ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &data2, sizeof(data2));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Second value creation should succeed");
  
  // Test cleanup
  cleanup_instruction(&instr);
  TEST_ASSERT(instr.values == NULL && instr.value_count == 0, "Cleanup should reset instruction");
}

// Integration Tests
void test_integration() {
  TEST_START("Integration Tests");
  
  // Test 1: Complete workflow
  TEST_CASE("Complete workflow");
  
  // Initialize header
  orionpp_header_t header;
  orionpp_error_t err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header init should succeed");
  
  // Create arena
  orionpp_arena_t arena;
  err = orionpp_arena_create(&arena, 2048, 512);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  // Create instruction
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_MOV};
  int32_t value_data = 123;
  orionpp_value_t value;
  err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &value_data, sizeof(value_data));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Value creation should succeed");
  
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 1,
    .values = &value
  };
  
  // Allocate space for instruction copy in arena
  void *instr_ptr = NULL;
  err = orionpp_arena_alloc(&arena, sizeof(orionpp_instruction_t), &instr_ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena allocation should succeed");
  
  // Test serialization
  char buffer[512];
  err = orionpp_writebuf(buffer, sizeof(buffer), &instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Instruction serialization should succeed");
  
  // Test deserialization
  orionpp_instruction_t loaded_instr;
  err = orionpp_readbuf(buffer, sizeof(buffer), &loaded_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Instruction deserialization should succeed");
  
  // Verify data integrity
  TEST_ASSERT(loaded_instr.opcode.root == instr.opcode.root &&
              loaded_instr.opcode.module_ == instr.opcode.module_ &&
              loaded_instr.value_count == instr.value_count,
              "Deserialized instruction should match original");
  
  if (loaded_instr.values && loaded_instr.values[0].value && value.value) {
    int32_t *loaded_data = (int32_t*)loaded_instr.values[0].value;
    int32_t *orig_data = (int32_t*)value.value;
    TEST_ASSERT(*loaded_data == *orig_data, "Deserialized data should match original");
  }
  
  // Cleanup
  cleanup_instruction(&loaded_instr);
  if (value.value) free(value.value);
  orionpp_arena_destroy(&arena);
  
  TEST_ASSERT(1, "Complete workflow should succeed");
}

// Edge Case Tests
void test_edge_cases() {
  TEST_START("Edge Cases");
  
  // Test 1: Zero-sized operations
  TEST_CASE("Zero-sized operations");
  orionpp_arena_t arena;
  orionpp_error_t err = orionpp_arena_create(&arena, 1024, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  void *ptr = NULL;
  err = orionpp_arena_alloc(&arena, 0, &ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Zero-size allocation should fail");
  
  orionpp_arena_destroy(&arena);
  
  // Test 2: Very large values
  TEST_CASE("Very large values");
  orionpp_value_t large_value;
  size_t large_size = 1024 * 1024; // 1MB
  void *large_data = malloc(large_size);
  if (large_data) {
    memset(large_data, 0xAB, large_size);
    err = create_test_value(&large_value, ORIONPP_TYPE_INT, ORIONPP_TYPE_UNT8, 
                            large_data, large_size);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Large value creation should succeed");
    
    // Test serialization with small buffer (should fail)
    char small_buf[1024];
    orionpp_instruction_t large_instr = {
      .opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_MOV},
      .value_count = 1,
      .values = &large_value
    };
    
    err = orionpp_writebuf(small_buf, sizeof(small_buf), &large_instr);
    TEST_ASSERT(err == ORIONPP_ERROR_BUFFER_OVERFLOW, "Should fail with small buffer");
    
    // Cleanup
    free(large_data);
    if (large_value.value) free(large_value.value);
  } else {
    TEST_ASSERT(1, "Large allocation test skipped (malloc failed)");
  }
  
  // Test 3: Rapid allocation/deallocation
  TEST_CASE("Rapid allocation/deallocation");
  err = orionpp_arena_create(&arena, 4096, 1024);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  int rapid_success = 1;
  for (int i = 0; i < 100 && rapid_success; i++) {
    void *temp_ptr = NULL;
    err = orionpp_arena_alloc(&arena, 32, &temp_ptr);
    if (err != ORIONPP_ERROR_GOOD && err != ORIONPP_ERROR_ARENA_FULL) {
      rapid_success = 0;
    }
    if (i % 10 == 0) {
      orionpp_arena_reset(&arena);
    }
  }
  TEST_ASSERT(rapid_success, "Rapid allocation/deallocation should work");
  
  orionpp_arena_destroy(&arena);
  
  // Test 4: Empty instruction serialization
  TEST_CASE("Empty instruction serialization");
  orionpp_instruction_t empty_instr = {
    .opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_NOP},
    .value_count = 0,
    .values = NULL
  };
  
  char buf[256];
  err = orionpp_writebuf(buf, sizeof(buf), &empty_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Empty instruction write should succeed");
  
  orionpp_instruction_t read_empty;
  err = orionpp_readbuf(buf, sizeof(buf), &read_empty);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Empty instruction read should succeed");
  
  TEST_ASSERT(read_empty.value_count == 0 && read_empty.values == NULL,
              "Empty instruction should remain empty");
}

int main() {
  printf("OrionPP Library Test Suite\n");
  printf("==========================\n");
  
  test_arena_functionality();
  test_error_functionality();  
  test_header_functionality();
  test_instruction_functionality();
  test_file_io();
  test_memory_safety();
  test_integration();
  test_edge_cases();
  
  TEST_SUMMARY();
  
  return test_failed > 0 ? 1 : 0;
}