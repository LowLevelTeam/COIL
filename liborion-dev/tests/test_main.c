#include "test_common.h"

int main() {
  printf("OrionPP Library Test Suite\n");
  printf("==========================\n");
  printf("Running comprehensive tests across multiple modules...\n");
  
  // Core functionality tests
  test_arena_functionality();
  test_error_functionality();  
  test_header_functionality();
  
  // Instruction and formatting tests
  test_instruction_functionality();
  
  // I/O tests
  test_file_io();
  test_arena_io();
  
  // Advanced tests
  test_memory_safety();
  test_integration();
  test_edge_cases();
  
  // Print final summary
  TEST_SUMMARY();
  
  // Return appropriate exit code
  return assert_failed > 0 ? 1 : 0;
}