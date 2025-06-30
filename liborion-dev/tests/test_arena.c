#include "test_framework.h"
#include "orionpp/arena.h"

/**
 * @file test_arena.c
 * @brief Arena allocator tests
 * 
 * Tests core arena functionality including allocation, I/O operations,
 * and error conditions.
 */

void test_arena_core() {
  TEST_START("Arena Core Functionality");
  
  orionpp_arena_t arena;
  orionpp_error_t err;
  
  TEST_CASE("Arena initialization");
  err = orionpp_arena_init(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Init should succeed");
  
  TEST_CASE("Arena creation");
  err = orionpp_arena_create(&arena, 1024, 256);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Creation should succeed");
  
  TEST_CASE("Basic allocation");
  void *ptr1 = NULL;
  err = orionpp_arena_alloc(&arena, 64, &ptr1);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr1 != NULL, "Allocation should succeed");
  
  TEST_CASE("Multiple allocations");
  void *ptr2 = NULL, *ptr3 = NULL;
  err = orionpp_arena_alloc(&arena, 32, &ptr2);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr2 != NULL, "Second allocation should succeed");
  err = orionpp_arena_alloc(&arena, 16, &ptr3);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && ptr3 != NULL, "Third allocation should succeed");
  
  TEST_CASE("Pointer uniqueness");
  TEST_ASSERT(ptr1 != ptr2 && ptr2 != ptr3 && ptr1 != ptr3, "Pointers should be unique");
  
  TEST_CASE("Used space tracking");
  size_t used = orionpp_arena_used(&arena);
  TEST_ASSERT(used >= 112, "Should track used space correctly"); // 64+32+16, aligned
  
  TEST_CASE("Arena reset");
  err = orionpp_arena_reset(&arena);
  size_t used_after_reset = orionpp_arena_used(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && used_after_reset == 0, "Reset should clear used space");
  
  TEST_CASE("Arena destruction");
  err = orionpp_arena_destroy(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Destruction should succeed");
}

void test_arena_io() {
  TEST_START("Arena I/O Operations");
  
  orionpp_arena_t arena;
  orionpp_error_t err;
  
  TEST_CASE("Arena I/O setup");
  err = orionpp_arena_create(&arena, 2048, 512);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  TEST_CASE("Basic write/read");
  const char test_data[] = "Hello, Arena!";
  size_t bytes_written, bytes_read;
  
  err = orionpp_arena_write(&arena, test_data, strlen(test_data), &bytes_written);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_written == strlen(test_data), 
              "Write should succeed");
  
  err = orionpp_arena_rewind(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Rewind should succeed");
  
  char read_buffer[64];
  memset(read_buffer, 0, sizeof(read_buffer));
  err = orionpp_arena_read(&arena, read_buffer, strlen(test_data), &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == strlen(test_data) &&
              strcmp(read_buffer, test_data) == 0, 
              "Read should return correct data");
  
  TEST_CASE("Position tracking");
  size_t position;
  err = orionpp_arena_tell(&arena, &position);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && position == strlen(test_data),
              "Position should be tracked correctly");
  
  TEST_CASE("Seek functionality");
  err = orionpp_arena_seek(&arena, 7); // Position at "Arena!"
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Seek should succeed");
  
  memset(read_buffer, 0, sizeof(read_buffer));
  err = orionpp_arena_read(&arena, read_buffer, 6, &bytes_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == 6 &&
              strncmp(read_buffer, "Arena!", 6) == 0,
              "Read after seek should return correct data");
  
  orionpp_arena_destroy(&arena);
}

void test_arena_errors() {
  TEST_START("Arena Error Conditions");
  
  orionpp_arena_t arena;
  orionpp_error_t err;
  void *ptr = NULL;
  
  TEST_CASE("NULL arena operations");
  err = orionpp_arena_init(NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Init with NULL should fail");
  
  err = orionpp_arena_alloc(NULL, 64, &ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Alloc with NULL arena should fail");
  
  TEST_CASE("Invalid arguments");
  orionpp_arena_init(&arena);
  err = orionpp_arena_alloc(&arena, 0, &ptr);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Zero size allocation should fail");
  
  err = orionpp_arena_alloc(&arena, 64, NULL);
  TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "NULL pointer should fail");
  
  TEST_CASE("Arena full condition");
  orionpp_arena_t small_arena;
  err = orionpp_arena_create(&small_arena, 100, 50);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Small arena creation should succeed");
  
  void *ptr_a = NULL, *ptr_b = NULL, *ptr_c = NULL;
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_a);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "First allocation should succeed");
  
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_b);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Second allocation should succeed");
  
  err = orionpp_arena_alloc(&small_arena, 40, &ptr_c);
  TEST_ASSERT(err == ORIONPP_ERROR_ARENA_FULL, "Should fail when arena is full");
  
  orionpp_arena_destroy(&small_arena);
}