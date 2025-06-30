#include "test_framework.h"
#include "orionpp/arena.h"
#include "orionpp/error.h"
#include "orionpp/header.h"
#include "orionpp/instruction.h"

/**
 * @file test_main.c
 * @brief Main test runner
 * 
 * Runs all test suites and reports results.
 */

// Test function declarations
// Arena tests
void test_arena_core(void);
void test_arena_io(void);
void test_arena_errors(void);

// Error tests
void test_error_messages(void);

// Header tests
void test_header_core(void);
void test_header_validation(void);
void test_header_features(void);
void test_header_errors(void);

// Instruction tests
void test_instruction_core(void);
void test_instruction_formatting(void);
void test_instruction_serialization(void);
void test_instruction_errors(void);

// Integration tests
void test_arena_instruction_integration(void);
void test_complete_workflow(void);
void test_memory_management(void);

int main(void) {
  printf("OrionPP Library Test Suite\n");
  printf("==========================\n");
  printf("Running comprehensive tests for production readiness...\n");
  
  // Arena module tests
  test_arena_core();
  test_arena_io();
  test_arena_errors();
  
  // Error module tests
  test_error_messages();
  
  // Header module tests
  test_header_core();
  test_header_validation();
  test_header_features();
  test_header_errors();
  
  // Instruction module tests
  test_instruction_core();
  test_instruction_formatting();
  test_instruction_serialization();
  test_instruction_errors();
  
  // Integration tests
  test_arena_instruction_integration();
  test_complete_workflow();
  test_memory_management();
  
  // Print final summary
  TEST_SUMMARY();
  
  // Return appropriate exit code
  return assert_failed > 0 ? 1 : 0;
}