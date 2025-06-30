#ifndef __TEST_COMMON_H
#define __TEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "orionpp/arena.h"
#include "orionpp/error.h"
#include "orionpp/header.h"
#include "orionpp/instruction.h"

// Test framework globals
extern int test_count;
extern int assert_total;
extern int assert_passed;
extern int assert_failed;

// Test framework macros
#define TEST_COUNT_MAX 1000

#define TEST_START(name) \
  do { \
    printf("\n=== Testing %s ===\n", name); \
  } while(0)

#define TEST_CASE(name) \
  do { \
    printf("  [%d] %s...\n", ++test_count, name); \
    fflush(stdout); \
  } while(0)

#define TEST_ASSERT(condition, message) \
  do { \
    ++assert_total; \
    if (condition) { \
      ++assert_passed; \
    } else { \
      printf("FAIL - %s\n", message); \
      ++assert_failed; \
    } \
  } while(0)

#define TEST_SUMMARY() \
  do { \
    printf("\n=== TEST SUMMARY ===\n"); \
    printf("Total tests: %d\n", test_count); \
    printf("Total Asserts: %d\n", assert_total); \
    printf("Total Passed Asserts: %d\n", assert_passed); \
    printf("Failed Failed Asserts: %d\n", assert_failed); \
    printf("Success rate: %.2f%%\n", assert_total > 0 ? (assert_passed * 100.0f / assert_total) : 0.0f); \
  } while(0)

// Test utility functions
void cleanup_instruction(orionpp_instruction_t *instr);
orionpp_error_t create_test_value(orionpp_value_t *value, orionpp_type_root_t type_root, 
                                  orionpp_type_module_t type_module, void *data, size_t data_size);

// Test function declarations
void test_arena_functionality(void);
void test_error_functionality(void);
void test_header_functionality(void);
void test_instruction_functionality(void);
void test_file_io(void);
void test_arena_io(void);
void test_memory_safety(void);
void test_integration(void);
void test_edge_cases(void);

#endif // __TEST_COMMON_H