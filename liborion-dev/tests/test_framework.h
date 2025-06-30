#ifndef ORIONPP_TEST_FRAMEWORK_H
#define ORIONPP_TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <orionpp/orionpp.h>

/**
 * @file test_framework.h
 * @brief Simple test framework for OrionPP library
 * 
 * Provides macros and utilities for writing and running tests.
 */

// Test statistics
extern int test_count;
extern int assert_total;
extern int assert_passed;
extern int assert_failed;

// Test framework macros
#define TEST_START(name) \
  do { \
    printf("\n=== Testing %s ===\n", name); \
    fflush(stdout); \
  } while(0)

#define TEST_CASE(name) \
  do { \
    printf("  [%d] %s... ", ++test_count, name); \
    fflush(stdout); \
  } while(0)

#define TEST_ASSERT(condition, message) \
  do { \
    ++assert_total; \
    if (condition) { \
      ++assert_passed; \
      printf("PASS\n"); \
    } else { \
      printf("FAIL - %s\n", message); \
      ++assert_failed; \
    } \
    fflush(stdout); \
  } while(0)

#define TEST_SUMMARY() \
  do { \
    printf("\n=== TEST SUMMARY ===\n"); \
    printf("Total tests: %d\n", test_count); \
    printf("Total asserts: %d\n", assert_total); \
    printf("Passed: %d\n", assert_passed); \
    printf("Failed: %d\n", assert_failed); \
    if (assert_total > 0) { \
      printf("Success rate: %.2f%%\n", (assert_passed * 100.0f / assert_total)); \
    } \
    printf("Status: %s\n", assert_failed == 0 ? "SUCCESS" : "FAILURE"); \
    fflush(stdout); \
  } while(0)

// Test helper function declarations
void cleanup_instruction(orionpp_instruction_t *instr);
orionpp_error_t create_test_value(orionpp_value_t *value, orionpp_type_root_t type_root, 
                                  orionpp_type_module_t type_module, void *data, size_t data_size);

#endif // ORIONPP_TEST_FRAMEWORK_H