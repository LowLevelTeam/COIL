#include "test_common.h"

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
  TEST_ASSERT(err != ORIONPP_ERROR_ARENA_FULL, "Oversized allocation shouldn't fail but reallocate");
  
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
  
  // Test 5: Multiple arena operations with memory pressure
  TEST_CASE("Memory pressure handling");
  const int num_arenas = 10;
  orionpp_arena_t arenas[num_arenas];
  
  // Create multiple arenas
  for (int i = 0; i < num_arenas; i++) {
    err = orionpp_arena_create(&arenas[i], 1024, 128);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Multiple arena creation should succeed");
  }
  
  // Allocate in each arena
  for (int i = 0; i < num_arenas; i++) {
    void *ptrs[8];
    for (int j = 0; j < 8; j++) {
      err = orionpp_arena_alloc(&arenas[i], 64, &ptrs[j]);
      TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Multiple allocations should succeed");
    }
  }
  
  // Cleanup all arenas
  for (int i = 0; i < num_arenas; i++) {
    orionpp_arena_destroy(&arenas[i]);
  }
  
  TEST_ASSERT(1, "Memory pressure test completed successfully");
  
  // Test 6: Value type tree memory management
  TEST_CASE("Complex type memory management");
  orionpp_value_t complex_value;
  memset(&complex_value, 0, sizeof(complex_value));
  
  // Create a pointer type with nested types
  complex_value.type.base.root = ORIONPP_TYPE_QUAL;
  complex_value.type.base.module_ = ORIONPP_TYPE_PTR;
  complex_value.type.count = 1;
  complex_value.type.types = malloc(sizeof(orionpp_type_t));
  TEST_ASSERT(complex_value.type.types != NULL, "Nested type allocation should succeed");
  
  // Set up the nested type
  complex_value.type.types[0].base.root = ORIONPP_TYPE_INT;
  complex_value.type.types[0].base.module_ = ORIONPP_TYPE_INT32;
  complex_value.type.types[0].count = 0;
  complex_value.type.types[0].types = NULL;
  
  // Add some value data
  void *test_ptr = (void*)0xDEADBEEF;
  complex_value.value = malloc(sizeof(void*));
  TEST_ASSERT(complex_value.value != NULL, "Value data allocation should succeed");
  memcpy(complex_value.value, &test_ptr, sizeof(void*));
  complex_value.value_byte_size = sizeof(void*);
  
  // Test serialization/deserialization of complex type
  char buffer[1024];
  orionpp_instruction_t complex_instr = {
    .opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_LEA},
    .value_count = 1,
    .values = &complex_value
  };
  
  err = orionpp_writebuf(buffer, sizeof(buffer), &complex_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Complex instruction serialization should succeed");
  
  orionpp_instruction_t read_complex_instr;
  err = orionpp_readbuf(buffer, sizeof(buffer), &read_complex_instr);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Complex instruction deserialization should succeed");
  
  // Verify the complex type structure
  TEST_ASSERT(read_complex_instr.value_count == 1 && read_complex_instr.values != NULL,
              "Complex instruction should have correct structure");
  
  if (read_complex_instr.values) {
    orionpp_value_t *read_val = &read_complex_instr.values[0];
    TEST_ASSERT(read_val->type.base.root == ORIONPP_TYPE_QUAL &&
                read_val->type.base.module_ == ORIONPP_TYPE_PTR,
                "Complex type should preserve root type");
  }
  
  // Cleanup complex types
  if (complex_value.value) free(complex_value.value);
  if (complex_value.type.types) free(complex_value.type.types);
  cleanup_instruction(&read_complex_instr);
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
  
  // Test 2: Multi-instruction program simulation
  TEST_CASE("Multi-instruction program simulation");
  
  err = orionpp_arena_create(&arena, 4096, 1024);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Program arena creation should succeed");
  
  // Create a series of instructions representing a simple program
  orionpp_instruction_t program[5];
  orionpp_value_t values[5];
  int32_t program_data[] = {10, 20, 30, 40, 50};
  
  for (int i = 0; i < 5; i++) {
    program[i].opcode.root = ORIONPP_OPCODE_ISA;
    program[i].opcode.module_ = (i % 2 == 0) ? ORIONPP_OP_ISA_LET : ORIONPP_OP_ISA_ADD;
    program[i].value_count = 1;
    program[i].values = &values[i];
    
    err = create_test_value(&values[i], ORIONPP_TYPE_INT, ORIONPP_TYPE_INT32, 
                            &program_data[i], sizeof(program_data[i]));
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Program value creation should succeed");
  }
  
  // Write entire program to arena
  for (int i = 0; i < 5; i++) {
    err = orionpp_writearena(&arena, &program[i]);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Program instruction write should succeed");
  }
  
  // Read back and verify program
  orionpp_arena_rewind(&arena);
  orionpp_instruction_t read_program[5];
  
  for (int i = 0; i < 5; i++) {
    err = orionpp_readarena(&arena, &read_program[i]);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Program instruction read should succeed");
    
    TEST_ASSERT(read_program[i].opcode.root == program[i].opcode.root &&
                read_program[i].opcode.module_ == program[i].opcode.module_,
                "Program instruction should match");
  }
  
  // Cleanup program
  for (int i = 0; i < 5; i++) {
    if (values[i].value) free(values[i].value);
    cleanup_instruction(&read_program[i]);
  }
  
  orionpp_arena_destroy(&arena);
  
  // Test 3: Header validation with program
  TEST_CASE("Header validation with program");
  
  err = orionpp_header_init(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header initialization should succeed");
  
  // Set some features
  header.features |= (1 << ORIONPP_FEATURE_ABI);
  header.features |= (1 << ORIONPP_FEATURE_STL);
  
  // Validate header
  err = orionpp_header_validate(&header);
  TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Header with features should validate");
  
  // Check feature detection
  int has_abi = orionpp_header_has_feature(&header, 1 << ORIONPP_FEATURE_ABI);
  int has_stl = orionpp_header_has_feature(&header, 1 << ORIONPP_FEATURE_STL);
  int has_cstl = orionpp_header_has_feature(&header, 1 << ORIONPP_FEATURE_CSTL);
  
  TEST_ASSERT(has_abi && has_stl && !has_cstl, "Feature detection should work correctly");
  
  // Test 4: Full system integration with file I/O
  TEST_CASE("Full system integration with file I/O");
  
#ifndef WIN32
  // Create a complete program file
  int program_file = open("test_program.opp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (program_file != -1) {
    // Write header first
    write(program_file, &header, sizeof(header));
    
    // Create and write a simple instruction
    orionpp_opcode_t final_opcode = {ORIONPP_OPCODE_ISA, ORIONPP_OP_ISA_RET};
    orionpp_instruction_t final_instr = {
      .opcode = final_opcode,
      .value_count = 0,
      .values = NULL
    };
    
    err = orionpp_writef(program_file, &final_instr);
    close(program_file);
    TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Program file write should succeed");
    
    // Read back and verify
    program_file = open("test_program.opp", O_RDONLY);
    if (program_file != -1) {
      orionpp_header_t read_header;
      read(program_file, &read_header, sizeof(read_header));
      
      err = orionpp_header_validate(&read_header);
      TEST_ASSERT(err == ORIONPP_ERROR_GOOD, "Read header should validate");
      
      orionpp_instruction_t read_final_instr;
      err = orionpp_readf(program_file, &read_final_instr);
      close(program_file);
      
      TEST_ASSERT(err == ORIONPP_ERROR_GOOD &&
                  read_final_instr.opcode.root == final_opcode.root &&
                  read_final_instr.opcode.module_ == final_opcode.module_,
                  "Read instruction should match written");
      
      cleanup_instruction(&read_final_instr);
    }
    
    unlink("test_program.opp");
  }
#else
  TEST_ASSERT(1, "Full system integration test skipped on Windows");
#endif
}