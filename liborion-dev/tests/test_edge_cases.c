#include "test_common.h"

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
  
  // Test 5: Maximum bucket utilization
  TEST_CASE("Maximum bucket utilization");
  err = orionpp_arena_create(&arena, 512, 128);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Small arena creation should succeed");
  
  // Fill exactly one bucket
  void *ptrs[16]; // 128 / 8 = 16 (8-byte aligned allocations)
  int successful_allocs = 0;
  
  for (int i = 0; i < 16; i++) {
    err = orionpp_arena_alloc(&arena, 8, &ptrs[i]);
    if (err == ORIONPP_ERROR_GOOD) {
      successful_allocs++;
    } else {
      break;
    }
  }
  
  TEST_ASSERT(successful_allocs > 0, "Should successfully allocate some memory");
  
  size_t used = orionpp_arena_used(&arena);
  TEST_ASSERT(used <= 128, "Used memory should not exceed bucket size initially");
  
  orionpp_arena_destroy(&arena);
  
  // Test 6: Extreme type nesting (performance test)
  TEST_CASE("Extreme type nesting");
  {
    const int nesting_depth = 5;
    orionpp_value_t nested_value;
    memset(&nested_value, 0, sizeof(nested_value));
    
    // Create a deeply nested type structure
    orionpp_type_t *current_type = &nested_value.type;
    for (int depth = 0; depth < nesting_depth; depth++) {
      current_type->base.root = ORIONPP_TYPE_QUAL;
      current_type->base.module_ = ORIONPP_TYPE_PTR;
      current_type->count = 1;
      
      if (depth < nesting_depth - 1) {
        current_type->types = malloc(sizeof(orionpp_type_t));
        TEST_ASSERT(current_type->types != NULL, "Nested type allocation should succeed");
        memset(current_type->types, 0, sizeof(orionpp_type_t));
        current_type = current_type->types;
      } else {
        // Leaf type
        current_type->types = malloc(sizeof(orionpp_type_t));
        TEST_ASSERT(current_type->types != NULL, "Leaf type allocation should succeed");
        current_type->types->base.root = ORIONPP_TYPE_INT;
        current_type->types->base.module_ = ORIONPP_TYPE_INT32;
        current_type->types->count = 0;
        current_type->types->types = NULL;
      }
    }
    
    // Add some dummy value data
    void *dummy_ptr = (void*)0x12345678;
    nested_value.value = malloc(sizeof(void*));
    TEST_ASSERT(nested_value.value != NULL, "Nested value data allocation should succeed");
    memcpy(nested_value.value, &dummy_ptr, sizeof(void*));
    nested_value.value_byte_size = sizeof(void*);
    
    // Test string formatting of deeply nested type
    char nested_type_buf[1024];
    size_t type_len = orionpp_string_type(nested_type_buf, sizeof(nested_type_buf), &nested_value.type);
    TEST_ASSERT(type_len > 0, "Deeply nested type formatting should succeed");
    
    // Cleanup nested types
    current_type = &nested_value.type;
    for (int depth = 0; depth < nesting_depth; depth++) {
      if (current_type->types) {
        orionpp_type_t *next_type = (depth < nesting_depth - 1) ? current_type->types : NULL;
        free(current_type->types);
        current_type->types = NULL;
        current_type = next_type;
        if (!current_type) break;
      }
    }
    if (nested_value.value) free(nested_value.value);
  }
  
  // Test 7: Invalid seek positions
  TEST_CASE("Invalid seek positions");
  {
    err = orionpp_arena_create(&arena, 1024, 256);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
    
    // Write some data first
    const char *test_data = "Test data for seeking";
    size_t seek_bytes_written;
    err = orionpp_arena_write(&arena, test_data, strlen(test_data), &seek_bytes_written);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena write should succeed");
    
    // Try to seek beyond data
    err = orionpp_arena_seek(&arena, strlen(test_data) + 100);
    TEST_ASSERT(err == ORIONPP_ERROR_INVALID_ARG, "Seek beyond data should fail");
    
    // Valid seek
    err = orionpp_arena_seek(&arena, 5);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Valid seek should succeed");
    
    size_t position;
    err = orionpp_arena_tell(&arena, &position);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD && position == 5, "Position should be correct");
    
    orionpp_arena_destroy(&arena);
  }
  
  // Test 8: Instruction with all opcode types
  TEST_CASE("All opcode types test");
  const orionpp_opcode_module_t all_opcodes[] = {
    ORIONPP_OP_ISA_NOP, ORIONPP_OP_ISA_LET, ORIONPP_OP_ISA_CONST, ORIONPP_OP_ISA_MOV,
    ORIONPP_OP_ISA_LEA, ORIONPP_OP_ISA_JMP, ORIONPP_OP_ISA_BREQ, ORIONPP_OP_ISA_BRNEQ,
    ORIONPP_OP_ISA_BRGT, ORIONPP_OP_ISA_BRGE, ORIONPP_OP_ISA_BRLT, ORIONPP_OP_ISA_BRLE,
    ORIONPP_OP_ISA_BRZ, ORIONPP_OP_ISA_BRNZ, ORIONPP_OP_ISA_CALL, ORIONPP_OP_ISA_RET,
    ORIONPP_OP_ISA_LABEL, ORIONPP_OP_ISA_SCOPE, ORIONPP_OP_ISA_SCOPL, ORIONPP_OP_ISA_ADD,
    ORIONPP_OP_ISA_SUB, ORIONPP_OP_ISA_MUL, ORIONPP_OP_ISA_DIV, ORIONPP_OP_ISA_MOD,
    ORIONPP_OP_ISA_INC, ORIONPP_OP_ISA_DEC, ORIONPP_OP_ISA_INCp, ORIONPP_OP_ISA_DECp,
    ORIONPP_OP_ISA_AND, ORIONPP_OP_ISA_OR, ORIONPP_OP_ISA_XOR, ORIONPP_OP_ISA_NOT,
    ORIONPP_OP_ISA_SHL, ORIONPP_OP_ISA_SHR
  };
  
  int all_opcodes_work = 1;
  char opcode_buf[64];
  
  for (size_t i = 0; i < sizeof(all_opcodes) / sizeof(all_opcodes[0]); i++) {
    orionpp_opcode_t test_opcode = {ORIONPP_OPCODE_ISA, all_opcodes[i]};
    size_t len = orionpp_string_opcode(opcode_buf, sizeof(opcode_buf), &test_opcode);
    if (len == 0) {
      all_opcodes_work = 0;
      break;
    }
  }
  
  TEST_ASSERT(all_opcodes_work, "All ISA opcodes should format correctly");
  
  // Test 9: All type combinations
  TEST_CASE("All type combinations test");
  {
    const orionpp_type_module_t int_types[] = {
      ORIONPP_TYPE_INT8, ORIONPP_TYPE_INT16, ORIONPP_TYPE_INT32, ORIONPP_TYPE_INT64,
      ORIONPP_TYPE_UNT8, ORIONPP_TYPE_UNT16, ORIONPP_TYPE_UNT32, ORIONPP_TYPE_UNT64
    };
    
    const orionpp_type_module_t qual_types[] = {
      ORIONPP_TYPE_CONST, ORIONPP_TYPE_VOLATILE, ORIONPP_TYPE_PTR
    };
    
    int all_types_work = 1;
    char combo_type_buf[128];
    
    // Test all integer types
    for (size_t i = 0; i < sizeof(int_types) / sizeof(int_types[0]); i++) {
      orionpp_type_t test_type = {
        .base = {ORIONPP_TYPE_INT, int_types[i]},
        .count = 0,
        .types = NULL
      };
      size_t len = orionpp_string_type(combo_type_buf, sizeof(combo_type_buf), &test_type);
      if (len == 0) {
        all_types_work = 0;
        break;
      }
    }
    
    // Test all qualifier types
    for (size_t i = 0; i < sizeof(qual_types) / sizeof(qual_types[0]); i++) {
      orionpp_type_t test_type = {
        .base = {ORIONPP_TYPE_QUAL, qual_types[i]},
        .count = 0,
        .types = NULL
      };
      size_t len = orionpp_string_type(combo_type_buf, sizeof(combo_type_buf), &test_type);
      if (len == 0) {
        all_types_work = 0;
        break;
      }
    }
    
    TEST_ASSERT(all_types_work, "All type combinations should format correctly");
  }
  
  // Test 10: Arena IO at bucket boundaries
  TEST_CASE("Arena IO at bucket boundaries");
  {
    err = orionpp_arena_create(&arena, 1024, 100); // Small buckets to test boundaries
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Arena creation should succeed");
    
    // Write data that spans multiple buckets
    char boundary_data[250]; // More than 2 buckets worth
    for (int i = 0; i < 250; i++) {
      boundary_data[i] = (char)(i % 256);
    }
    
    size_t boundary_bytes_written;
    err = orionpp_arena_write(&arena, boundary_data, sizeof(boundary_data), &boundary_bytes_written);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD && boundary_bytes_written == sizeof(boundary_data),
                "Boundary-spanning write should succeed");
    
    // Rewind and read back in smaller chunks
    orionpp_arena_rewind(&arena);
    char read_chunks[5][50];
    size_t total_read = 0;
    
    for (int i = 0; i < 5; i++) {
      size_t bytes_read;
      err = orionpp_arena_read(&arena, read_chunks[i], 50, &bytes_read);
      TEST_ASSERT(err == ORIONPP_ERROR_GOOD && bytes_read == 50,
                  "Chunked read should succeed");
      total_read += bytes_read;
    }
    
    TEST_ASSERT(total_read == 250, "Total read should match written data size");
    
    // Verify data integrity
    int data_matches = 1;
    for (int chunk = 0; chunk < 5; chunk++) {
      for (int byte = 0; byte < 50; byte++) {
        int expected = (chunk * 50 + byte) % 256;
        if (read_chunks[chunk][byte] != (char)expected) {
          data_matches = 0;
          break;
        }
      }
      if (!data_matches) break;
    }
    
    TEST_ASSERT(data_matches, "Boundary-spanning data should maintain integrity");
    
    orionpp_arena_destroy(&arena);
  }
}