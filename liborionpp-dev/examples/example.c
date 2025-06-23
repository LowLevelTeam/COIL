#include "../include/orionpp.h"
#include "../include/orionpp_writer.h"
#include <stdio.h>

void create_human_example(void) {
  printf("Creating Orion++ human format example...\n");
  
  FILE *output = fopen("example_human.hopp", "w");
  if (!output) {
    printf("Failed to create output file\n");
    return;
  }
  
  OrionPPWriter *writer = orionpp_writer_create(output, ORIONPP_FORMAT_HUMAN);
  if (!writer) {
    printf("Failed to create writer\n");
    fclose(output);
    return;
  }
  
  // Begin program
  orionpp_writer_begin_program(writer);
  
  // Create a simple function: int add(int a, int b) { return a + b; }
  orionpp_writer_begin_function(writer, "add", 2, ORIONPP_TYPE_I32);
  
  // Create parameter variables
  uint32_t param_a = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "a", ORIONPP_TYPE_I32);
  uint32_t param_b = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "b", ORIONPP_TYPE_I32);
  
  // Create local variables
  uint32_t result = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "result", ORIONPP_TYPE_I32);
  
  // Emit function prologue
  orionpp_writer_emit_abi_callee_enter(writer);
  orionpp_writer_emit_enter(writer);
  
  // Get arguments
  orionpp_writer_emit_abi_callee_getarg(writer, param_a, 0);
  orionpp_writer_emit_abi_callee_getarg(writer, param_b, 1);
  
  // Perform addition
  orionpp_writer_emit_add(writer, result, param_a, param_b);
  
  // Set return value
  orionpp_writer_emit_abi_callee_setret(writer, result, 0);
  
  // Function epilogue
  orionpp_writer_emit_leave(writer);
  orionpp_writer_emit_abi_callee_leave(writer);
  orionpp_writer_emit_ret(writer);
  
  orionpp_writer_end_function(writer);
  
  // Create main function
  orionpp_writer_begin_function(writer, "main", 0, ORIONPP_TYPE_I32);
  
  uint32_t a_val = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "a_val", ORIONPP_TYPE_I32);
  uint32_t b_val = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "b_val", ORIONPP_TYPE_I32);
  uint32_t sum = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "sum", ORIONPP_TYPE_I32);
  
  orionpp_writer_emit_abi_callee_enter(writer);
  orionpp_writer_emit_enter(writer);
  
  // Initialize values
  orionpp_writer_emit_const(writer, a_val, 10);
  orionpp_writer_emit_const(writer, b_val, 20);
  
  // Call add function (simplified - would need full ABI setup)
  orionpp_writer_emit_call(writer, "add");
  
  // Return 0
  orionpp_writer_emit_const(writer, sum, 0);
  orionpp_writer_emit_abi_callee_setret(writer, sum, 0);
  
  orionpp_writer_emit_leave(writer);
  orionpp_writer_emit_abi_callee_leave(writer);
  orionpp_writer_emit_ret(writer);
  
  orionpp_writer_end_function(writer);
  
  orionpp_writer_end_program(writer);
  orionpp_writer_finalize(writer);
  
  orionpp_writer_destroy(writer);
  fclose(output);
  
  printf("Created example_human.hopp\n");
}

void create_binary_example(void) {
  printf("Creating Orion++ binary format example...\n");
  
  FILE *output = fopen("example_binary.orionpp", "wb");
  if (!output) {
    printf("Failed to create output file\n");
    return;
  }
  
  OrionPPWriter *writer = orionpp_writer_create(output, ORIONPP_FORMAT_BINARY);
  if (!writer) {
    printf("Failed to create writer\n");
    fclose(output);
    return;
  }
  
  // Begin program
  orionpp_writer_begin_program(writer);
  
  // Create the same add function for binary format
  orionpp_writer_begin_function(writer, "add", 2, ORIONPP_TYPE_I32);
  
  uint32_t param_a = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "a", ORIONPP_TYPE_I32);
  uint32_t param_b = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "b", ORIONPP_TYPE_I32);
  uint32_t result = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "result", ORIONPP_TYPE_I32);
  
  orionpp_writer_emit_abi_callee_enter(writer);
  orionpp_writer_emit_enter(writer);
  orionpp_writer_emit_abi_callee_getarg(writer, param_a, 0);
  orionpp_writer_emit_abi_callee_getarg(writer, param_b, 1);
  orionpp_writer_emit_add(writer, result, param_a, param_b);
  orionpp_writer_emit_abi_callee_setret(writer, result, 0);
  orionpp_writer_emit_leave(writer);
  orionpp_writer_emit_abi_callee_leave(writer);
  orionpp_writer_emit_ret(writer);
  
  orionpp_writer_end_function(writer);
  
  orionpp_writer_end_program(writer);
  orionpp_writer_finalize(writer);
  
  orionpp_writer_destroy(writer);
  fclose(output);
  
  printf("Created example_binary.orionpp\n");
}

int main(void) {
  printf("Orion++ Library Example\n");
  printf("=====================\n\n");
  
  create_human_example();
  create_binary_example();
  
  printf("\nExample completed successfully!\n");
  printf("Files created:\n");
  printf("  example_human.hopp - Human-readable Orion++ assembly\n");
  printf("  example_binary.orionpp - Binary Orion++ format\n");
  
  return 0;
}