#include "test_common.h"

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

void test_arena_io() {
  TEST_START("Arena I/O");
  
  orionpp_arena_t arena;
  orionpp_error_t err;
  
  // Test 1: Arena IO setup
  TEST_CASE("Arena IO setup");
  err = orionpp_arena_create(&arena, 2048, 512);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  // Test 2: Basic arena write/read
  TEST_CASE("Basic arena write/read");
  const char test_data[] = "Hello, Arena!";
  size_t bytes_written, bytes_read;
  
  err = orionpp_arena_write(&arena, test_data, strlen(test_data), &bytes_written);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_written == strlen(test_data), 
              "Arena write should succeed");
  
  // Rewind for reading
  err = orionpp_arena_rewind(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena rewind should succeed");
  
  char read_buffer[64];
  memset(read_buffer, 0, sizeof(read_buffer));
  err = orionpp_arena_read(&arena, read_buffer, strlen(test_data), &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == strlen(test_data) &&
              strcmp(read_buffer, test_data) == 0, 
              "Arena read should return correct data");
  
  // Test 3: Position tracking
  TEST_CASE("Arena position tracking");
  size_t position;
  err = orionpp_arena_tell(&arena, &position);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && position == strlen(test_data),
              "Arena position should be tracked correctly");
  
  // Test 4: Seek functionality
  TEST_CASE("Arena seek functionality");
  err = orionpp_arena_seek(&arena, 7); // Position at "Arena!"
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena seek should succeed");
  
  memset(read_buffer, 0, sizeof(read_buffer));
  err = orionpp_arena_read(&arena, read_buffer, 6, &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == 6 &&
              strncmp(read_buffer, "Arena!", 6) == 0,
              "Arena read after seek should return correct data");
  
  // Test 5: Instruction arena IO
  TEST_CASE("Instruction arena IO");
  orionpp_arena_reset(&arena);
  
  // Create test instruction
  orionpp_opcode_t opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_ADD};
  int32_t test_value = 42;
  orionpp_value_t value;
  err = create_test_value(&value, ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &test_value, sizeof(test_value));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  orionpp_instruction_t instr = {
    .opcode = opcode,
    .value_count = 1,
    .values = &value
  };
  
  // Write instruction to arena
  err = orionpp_writearena(&arena, &instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena instruction write should succeed");
  
  // Rewind and read back
  orionpp_arena_rewind(&arena);
  orionpp_instruction_t read_instr;
  err = orionpp_readarena(&arena, &read_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena instruction read should succeed");
  
  // Verify instruction data
  TEST_ASSERT(read_instr.opcode.root == instr.opcode.root &&
              read_instr.opcode.module_ == instr.opcode.module_ &&
              read_instr.value_count == instr.value_count,
              "Read instruction should match written instruction");
  
  if (read_instr.values && read_instr.values[0].value && value.value) {
    int32_t *read_data = (int32_t*)read_instr.values[0].value;
    int32_t *orig_data = (int32_t*)value.value;
    TEST_ASSERT(*read_data == *orig_data, "Instruction value data should match");
  }
  
  // Test 6: Large data spanning buckets
  TEST_CASE("Large data spanning buckets");
  orionpp_arena_reset(&arena);
  
  // Create large data that spans multiple buckets
  char large_data[1000];
  for (int i = 0; i < 1000; i++) {
    large_data[i] = (char)(i % 256);
  }
  
  err = orionpp_arena_write(&arena, large_data, sizeof(large_data), &bytes_written);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_written == sizeof(large_data),
              "Large arena write should succeed");
  
  orionpp_arena_rewind(&arena);
  char large_read_buffer[1000];
  err = orionpp_arena_read(&arena, large_read_buffer, sizeof(large_read_buffer), &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == sizeof(large_data) &&
              memcmp(large_data, large_read_buffer, sizeof(large_data)) == 0,
              "Large arena read should return correct data");
  
  // Test 7: Arena IO error conditions
  TEST_CASE("Arena IO error conditions");
  err = orionpp_arena_read(NULL, read_buffer, 10, &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_write(NULL, test_data, 10, &bytes_written);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_seek(NULL, 0);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  err = orionpp_arena_tell(NULL, &position);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Should fail with NULL arena");
  
  // Cleanup
  cleanup_instruction(&read_instr);
  if (value.value) free(value.value);
  orionpp_arena_destroy(&arena);
}