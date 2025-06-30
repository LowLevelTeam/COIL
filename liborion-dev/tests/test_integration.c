#include "test_framework.h"
#include "orionpp/arena.h"
#include "orionpp/header.h"
#include "orionpp/instruction.h"

/**
 * @file test_integration.c
 * @brief Integration tests
 * 
 * Tests interaction between different library components.
 */

void test_arena_instruction_integration() {
  TEST_START("Arena-Instruction Integration");
  
  orionpp_arena_t arena;
  orionpp_instruction_t src_instr, dest_instr;
  orionpp_error_t err;
  
  TEST_CASE("Arena-instruction I/O setup");
  err = orionpp_arena_create(&arena, 4096, 1024);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  orionpp_instruction_init(&src_instr);
  src_instr.opcode.root = ORIONPP_OPCODE_ISA;
  src_instr.opcode.module_ = ORIONPP_OP_ISA_ADD;
  src_instr.value_count = 1;
  src_instr.values = malloc(sizeof(orionpp_value_t));
  
  int32_t test_data = 42;
  err = create_test_value(&src_instr.values[0], ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                          &test_data, sizeof(test_data));
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
  
  TEST_CASE("Write instruction to arena");
  err = orionpp_write_arena(&arena, &src_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena write should succeed");
  
  TEST_CASE("Read instruction from arena");
  orionpp_arena_rewind(&arena);
  err = orionpp_read_arena(&arena, &dest_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena read should succeed");
  
  TEST_CASE("Verify instruction integrity");
  TEST_ASSERT(dest_instr.opcode.root == src_instr.opcode.root &&
              dest_instr.opcode.module_ == src_instr.opcode.module_ &&
              dest_instr.value_count == src_instr.value_count,
              "Instruction should be preserved");
  
  if (dest_instr.values && dest_instr.values[0].value && src_instr.values[0].value) {
    int32_t *dest_data = (int32_t*)dest_instr.values[0].value;
    int32_t *src_data = (int32_t*)src_instr.values[0].value;
    TEST_ASSERT(*dest_data == *src_data, "Value data should be preserved");
  }
  
  orionpp_instruction_destroy(&src_instr);
  orionpp_instruction_destroy(&dest_instr);
  orionpp_arena_destroy(&arena);
}

void test_complete_workflow() {
  TEST_START("Complete Workflow");
  
  orionpp_header_t header;
  orionpp_arena_t arena;
  orionpp_instruction_t instructions[3], read_instructions[3];
  orionpp_error_t err;
  
  TEST_CASE("Complete system setup");
  
  // Initialize header
  err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header init should succeed");
  
  // Set some features
  header.features |= (1 << ORIONPP_FEATURE_ABI);
  header.features |= (1 << ORIONPP_FEATURE_STL);
  
  // Create arena
  err = orionpp_arena_create(&arena, 8192, 2048);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
  
  // Create a program with multiple instructions
  int32_t values[] = {10, 20, 30};
  orionpp_opcode_module_t opcodes[] = {ORIONPP_OP_ISA_LET, ORIONPP_OP_ISA_ADD, ORIONPP_OP_ISA_RET};
  
  for (int i = 0; i < 3; i++) {
    orionpp_instruction_init(&instructions[i]);
    instructions[i].opcode.root = ORIONPP_OPCODE_ISA;
    instructions[i].opcode.module_ = opcodes[i];
    
    if (i < 2) { // Only first two instructions have values
      instructions[i].value_count = 1;
      instructions[i].values = malloc(sizeof(orionpp_value_t));
      err = create_test_value(&instructions[i].values[0], ORIONPP_TYPE_INT, 
                              ORIONPP_TYPE_INT32, &values[i], sizeof(values[i]));
      TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Test value creation should succeed");
    } else {
      instructions[i].value_count = 0;
      instructions[i].values = NULL;
    }
  }
  
  TEST_CASE("Write header to arena");
  size_t header_written;
  err = orionpp_arena_write(&arena, &header, sizeof(header), &header_written);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && header_written == sizeof(header),
              "Header write should succeed");
  
  TEST_CASE("Write program to arena");
  for (int i = 0; i < 3; i++) {
    err = orionpp_write_arena(&arena, &instructions[i]);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Instruction write should succeed");
  }
  
  TEST_CASE("Read and validate complete program");
  orionpp_arena_rewind(&arena);
  
  // Read header
  orionpp_header_t read_header;
  size_t header_read;
  err = orionpp_arena_read(&arena, &read_header, sizeof(read_header), &header_read);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD && header_read == sizeof(read_header),
              "Header read should succeed");
  
  // Validate header
  err = orionpp_header_validate(&read_header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Read header should be valid");
  
  // Check features
  int has_abi = orionpp_header_has_feature(&read_header, ORIONPP_FEATURE_ABI);
  int has_stl = orionpp_header_has_feature(&read_header, ORIONPP_FEATURE_STL);
  TEST_ASSERT(has_abi && has_stl, "Features should be preserved");
  
  // Read instructions
  for (int i = 0; i < 3; i++) {
    err = orionpp_read_arena(&arena, &read_instructions[i]);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Instruction read should succeed");
    
    TEST_ASSERT(read_instructions[i].opcode.root == instructions[i].opcode.root &&
                read_instructions[i].opcode.module_ == instructions[i].opcode.module_,
                "Instruction opcode should be preserved");
    
    TEST_ASSERT(read_instructions[i].value_count == instructions[i].value_count,
                "Instruction value count should be preserved");
  }
  
  TEST_CASE("Verify program integrity");
  char format_buf[256];
  int formatting_works = 1;
  
  for (int i = 0; i < 3; i++) {
    size_t len = orionpp_string_instr(format_buf, sizeof(format_buf), &read_instructions[i]);
    if (len == 0) {
      formatting_works = 0;
      break;
    }
  }
  
  TEST_ASSERT(formatting_works, "All instructions should format correctly");
  
  // Cleanup
  for (int i = 0; i < 3; i++) {
    orionpp_instruction_destroy(&instructions[i]);
    orionpp_instruction_destroy(&read_instructions[i]);
  }
  orionpp_arena_destroy(&arena);
}

void test_memory_management() {
  TEST_START("Memory Management Integration");
  
  orionpp_arena_t arena;
  orionpp_instruction_t instructions[10];
  orionpp_error_t err;
  
  TEST_CASE("Multiple arena operations");
  err = orionpp_arena_create(&arena, 16384, 4096);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Large arena creation should succeed");
  
  // Create multiple instructions with complex types
  for (int i = 0; i < 10; i++) {
    orionpp_instruction_init(&instructions[i]);
    instructions[i].opcode.root = ORIONPP_OPCODE_ISA;
    instructions[i].opcode.module_ = ORIONPP_OP_ISA_MOV;
    instructions[i].value_count = 2;
    instructions[i].values = malloc(2 * sizeof(orionpp_value_t));
    
    // Create two values per instruction
    int32_t value1 = i * 10;
    uint64_t value2 = i * 100ULL;
    
    err = create_test_value(&instructions[i].values[0], ORIONPP_TYPE_INT, 
                            ORIONPP_TYPE_INT32, &value1, sizeof(value1));
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Value 1 creation should succeed");
    
    err = create_test_value(&instructions[i].values[1], ORIONPP_TYPE_INT, 
                            ORIONPP_TYPE_UNT64, &value2, sizeof(value2));
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Value 2 creation should succeed");
    
    // Write to arena
    err = orionpp_write_arena(&arena, &instructions[i]);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena write should succeed");
  }
  
  TEST_CASE("Arena utilization check");
  size_t used = orionpp_arena_used(&arena);
  size_t available = orionpp_arena_available(&arena);
  TEST_ASSERT(used > 0 && available < 16384, "Arena should be utilized");
  
  TEST_CASE("Arena reset and reuse");
  err = orionpp_arena_reset(&arena);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena reset should succeed");
  
  size_t used_after_reset = orionpp_arena_used(&arena);
  TEST_ASSERT(used_after_reset == 0, "Arena should be empty after reset");
  
  // Cleanup
  for (int i = 0; i < 10; i++) {
    orionpp_instruction_destroy(&instructions[i]);
  }
  orionpp_arena_destroy(&arena);
}