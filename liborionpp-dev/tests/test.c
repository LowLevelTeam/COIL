#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "orionpp/module.h"
#include "orionpp/binary.h"
#include "orionpp/text.h"
#include "orionpp/value.h"
#include "orionpp/instr.h"
#include "orionpp/strtab.h"

// Test framework
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

#define TEST(name) static void test_##name(void)
#define RUN_TEST(name) do { \
  printf("Running test: " #name "... "); \
  fflush(stdout); \
  test_count++; \
  test_##name(); \
  printf("PASSED\n"); \
  test_passed++; \
} while(0)

#define ASSERT(condition) do { \
  if (!(condition)) { \
    printf("FAILED\n  Assertion failed: %s at line %d\n", #condition, __LINE__); \
    test_failed++; \
    return; \
  } \
} while(0)

#define ASSERT_OK(result) ASSERT(ORIONPP_IS_OK(result))
#define ASSERT_ERROR(result) ASSERT(ORIONPP_IS_ERROR(result))
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)
#define ASSERT_NULL(ptr) ASSERT((ptr) == NULL)
#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_NEQ(a, b) ASSERT((a) != (b))
#define ASSERT_STR_EQ(a, b) ASSERT(strcmp((a), (b)) == 0)

// Test helper functions
static void cleanup_files(void) {
  remove("test_output.hopp");
  remove("test_output.orion");
  remove("test_loaded.hopp");
}

// String table tests
TEST(string_table_create_destroy) {
  orionpp_string_table_t* table = NULL;
  orionpp_result_t result = orionpp_string_table_create(&table, NULL);
  ASSERT_OK(result);
  ASSERT_NOT_NULL(table);
  
  orionpp_string_table_destroy(table);
}

TEST(string_table_add_get) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  uint32_t offset1, offset2, offset3;
  ASSERT_OK(orionpp_string_table_add(table, "hello", &offset1));
  ASSERT_OK(orionpp_string_table_add(table, "world", &offset2));
  ASSERT_OK(orionpp_string_table_add(table, "hello", &offset3)); // Duplicate
  
  ASSERT_EQ(offset1, offset3); // Should deduplicate
  ASSERT_NEQ(offset1, offset2);
  
  ASSERT_STR_EQ(orionpp_string_table_get(table, offset1), "hello");
  ASSERT_STR_EQ(orionpp_string_table_get(table, offset2), "world");
  
  ASSERT_EQ(orionpp_string_table_get_length(table, offset1), 5);
  ASSERT_EQ(orionpp_string_table_get_length(table, offset2), 5);
  
  ASSERT(orionpp_string_table_is_valid_offset(table, offset1));
  ASSERT(!orionpp_string_table_is_valid_offset(table, 99999));
  
  orionpp_string_table_destroy(table);
}

TEST(string_table_add_length) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  uint32_t offset;
  ASSERT_OK(orionpp_string_table_add_length(table, "hello world", 5, &offset));
  ASSERT_STR_EQ(orionpp_string_table_get(table, offset), "hello");
  
  orionpp_string_table_destroy(table);
}

// Value tests
TEST(value_variable) {
  orionpp_value_t value = orionpp_value_variable(42);
  ASSERT(orionpp_value_is_variable(&value));
  ASSERT(!orionpp_value_is_symbol(&value));
  ASSERT(!orionpp_value_is_numeric(&value));
  ASSERT_EQ(orionpp_value_get_variable_id(&value), 42);
  ASSERT(orionpp_value_is_valid(&value));
}

TEST(value_numeric) {
  orionpp_value_t value = orionpp_value_numeric(ORIONPP_BASE_HEX, 0xFF);
  ASSERT(orionpp_value_is_numeric(&value));
  ASSERT(!orionpp_value_is_variable(&value));
  ASSERT_EQ(orionpp_value_get_numeric_value(&value), 0xFF);
  ASSERT_EQ(orionpp_value_get_numeric_base(&value), ORIONPP_BASE_HEX);
  ASSERT(orionpp_value_is_valid(&value));
}

TEST(value_symbol) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  orionpp_value_t value;
  ASSERT_OK(orionpp_value_symbol_from_string(&value, "test_symbol", table));
  ASSERT(orionpp_value_is_symbol(&value));
  ASSERT(orionpp_value_is_valid(&value));
  
  orionpp_string_table_destroy(table);
}

TEST(value_label) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  orionpp_value_t value;
  ASSERT_OK(orionpp_value_label_from_string(&value, "loop", 1, table));
  ASSERT(orionpp_value_is_label(&value));
  ASSERT(orionpp_value_is_valid(&value));
  
  orionpp_string_table_destroy(table);
}

TEST(value_string) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  orionpp_value_t value;
  ASSERT_OK(orionpp_value_string_from_string(&value, "test string", table));
  ASSERT(orionpp_value_is_string(&value));
  ASSERT(orionpp_value_is_valid(&value));
  
  orionpp_string_table_destroy(table);
}

TEST(value_array) {
  orionpp_value_t elements[3] = {
    orionpp_value_variable(0),
    orionpp_value_variable(1), 
    orionpp_value_numeric(ORIONPP_BASE_DECIMAL, 42)
  };
  
  orionpp_value_t* array_value = NULL;
  ASSERT_OK(orionpp_value_array_create(&array_value, elements, 3, NULL));
  ASSERT_NOT_NULL(array_value);
  ASSERT(orionpp_value_is_array(array_value));
  ASSERT(orionpp_value_is_valid(array_value));
  ASSERT_EQ(array_value->array.count, 3);
  
  orionpp_value_array_destroy(array_value, NULL);
}

TEST(value_copy) {
  orionpp_value_t src = orionpp_value_variable(123);
  orionpp_value_t dest;
  
  ASSERT_OK(orionpp_value_copy(&dest, &src, NULL));
  ASSERT(orionpp_value_is_variable(&dest));
  ASSERT_EQ(orionpp_value_get_variable_id(&dest), 123);
}

TEST(value_parse_from_text) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  orionpp_value_t value;
  
  // Test variable parsing
  ASSERT_OK(orionpp_value_parse_from_text(&value, "$42", table, NULL));
  ASSERT(orionpp_value_is_variable(&value));
  ASSERT_EQ(orionpp_value_get_variable_id(&value), 42);
  
  // Test numeric parsing
  ASSERT_OK(orionpp_value_parse_from_text(&value, "%x10", table, NULL));
  ASSERT(orionpp_value_is_numeric(&value));
  ASSERT_EQ(orionpp_value_get_numeric_value(&value), 0x10);
  ASSERT_EQ(orionpp_value_get_numeric_base(&value), ORIONPP_BASE_HEX);
  
  // Test label parsing
  ASSERT_OK(orionpp_value_parse_from_text(&value, "+.loop", table, NULL));
  ASSERT(orionpp_value_is_label(&value));
  
  orionpp_string_table_destroy(table);
}

// Instruction tests
TEST(instruction_create_destroy) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, NULL));
  ASSERT_NOT_NULL(instr);
  ASSERT_EQ(instr->feature, ORIONPP_FEATURE_ISA);
  ASSERT_EQ(instr->opcode, ORIONPP_ISA_ADD);
  ASSERT(orionpp_instruction_is_valid(instr));
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_feature_validation) {
  orionpp_instruction_t* instr = NULL;
  
  // Valid combinations
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, NULL));
  orionpp_instruction_destroy(instr, NULL);
  
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MOV, NULL));
  orionpp_instruction_destroy(instr, NULL);
  
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ABI, ORIONPP_ABI_ARGS, NULL));
  orionpp_instruction_destroy(instr, NULL);
  
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_HINT, ORIONPP_HINT_SYMEND, NULL));
  orionpp_instruction_destroy(instr, NULL);
  
  // Invalid combinations (use opcodes that don't collide with valid ones)
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_OBJ, 99, NULL));  // Invalid OBJ opcode
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, 88, NULL));  // Invalid ISA opcode
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ABI, 77, NULL));  // Invalid ABI opcode
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_HINT, 66, NULL)); // Invalid HINT opcode
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_NONE, 1, NULL));  // Invalid feature
  ASSERT_ERROR(orionpp_instruction_create(&instr, 255, 1, NULL));                   // Invalid feature
}

TEST(instruction_obj_section) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, NULL));
  
  ASSERT_OK(orionpp_instruction_set_obj_section(instr, ORIONPP_SECTION_TEXT,
                                                 ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC,
                                                 123));
  
  ASSERT_EQ(instr->obj_section.section_type, ORIONPP_SECTION_TEXT);
  ASSERT_EQ(instr->obj_section.section_flags, ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC);
  ASSERT_EQ(instr->obj_section.name_offset, 123);
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_obj_symbol) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SYMBOL, NULL));
  
  ASSERT_OK(orionpp_instruction_set_obj_symbol(instr, ORIONPP_SYMBOL_GLOBAL,
                                                ORIONPP_SYMBOL_FUNC, 456));
  
  ASSERT_EQ(instr->obj_symbol.binding, ORIONPP_SYMBOL_GLOBAL);
  ASSERT_EQ(instr->obj_symbol.symbol_type, ORIONPP_SYMBOL_FUNC);
  ASSERT_EQ(instr->obj_symbol.name_offset, 456);
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_isa_let) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_LET, NULL));
  
  ASSERT_OK(orionpp_instruction_set_isa_let(instr, 99));
  ASSERT_EQ(instr->isa_let.variable_id, 99);
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_isa_mov) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MOV, NULL));
  
  orionpp_value_t dest = orionpp_value_variable(1);
  orionpp_value_t src = orionpp_value_numeric(ORIONPP_BASE_DECIMAL, 42);
  
  ASSERT_OK(orionpp_instruction_set_isa_mov(instr, &dest, &src));
  ASSERT(orionpp_value_is_variable(&instr->isa_mov.dest));
  ASSERT(orionpp_value_is_numeric(&instr->isa_mov.src));
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_isa_binary_op) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, NULL));
  
  orionpp_value_t dest = orionpp_value_variable(2);
  orionpp_value_t src1 = orionpp_value_variable(0);
  orionpp_value_t src2 = orionpp_value_variable(1);
  
  ASSERT_OK(orionpp_instruction_set_isa_binary_op(instr, &dest, &src1, &src2));
  ASSERT(orionpp_value_is_variable(&instr->isa_binary_op.dest));
  ASSERT(orionpp_value_is_variable(&instr->isa_binary_op.src1));
  ASSERT(orionpp_value_is_variable(&instr->isa_binary_op.src2));
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_abi_declaration) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CALLEE, NULL));
  
  ASSERT_OK(orionpp_instruction_set_abi_declaration(instr, ORIONPP_ABI_CALLEE_TYPE, 789));
  ASSERT_EQ(instr->abi_declaration.abi_type, ORIONPP_ABI_CALLEE_TYPE);
  ASSERT_EQ(instr->abi_declaration.abi_name_offset, 789);
  
  orionpp_instruction_destroy(instr, NULL);
}

TEST(instruction_abi_args_rets) {
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_instruction_create(&instr, ORIONPP_FEATURE_ABI, ORIONPP_ABI_ARGS, NULL));
  
  orionpp_value_t args[2] = {
    orionpp_value_variable(0),
    orionpp_value_variable(1)
  };
  
  ASSERT_OK(orionpp_instruction_set_abi_args_rets(instr, args, 2, NULL));
  ASSERT_EQ(instr->abi_args_rets.count, 2);
  ASSERT_NOT_NULL(instr->abi_args_rets.values);
  
  orionpp_instruction_destroy(instr, NULL);
}

// Module tests
TEST(module_create_destroy) {
  orionpp_module_t* module = NULL;
  orionpp_features_t features = ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA;
  
  ASSERT_OK(orionpp_module_create(&module, features, NULL));
  ASSERT_NOT_NULL(module);
  ASSERT_EQ(module->features, features);
  ASSERT_NOT_NULL(module->strings);
  ASSERT_EQ(module->instruction_count, 0);
  ASSERT_EQ(module->version_major, ORIONPP_VERSION_MAJOR);
  
  orionpp_module_destroy(module);
}

TEST(module_feature_management) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_OBJ, NULL));
  
  ASSERT(orionpp_module_has_feature(module, ORIONPP_FEATURE_OBJ));
  ASSERT(!orionpp_module_has_feature(module, ORIONPP_FEATURE_ISA));
  
  ASSERT_OK(orionpp_module_enable_feature(module, ORIONPP_FEATURE_ISA));
  ASSERT(orionpp_module_has_feature(module, ORIONPP_FEATURE_ISA));
  
  ASSERT_OK(orionpp_module_disable_feature(module, ORIONPP_FEATURE_OBJ));
  ASSERT(!orionpp_module_has_feature(module, ORIONPP_FEATURE_OBJ));
  
  orionpp_module_destroy(module);
}

TEST(module_instruction_management) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_ISA, NULL));
  
  orionpp_instruction_t* instr1 = NULL;
  orionpp_instruction_t* instr2 = NULL;
  
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, &instr1));
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 1);
  ASSERT_EQ(orionpp_module_get_instruction(module, 0), instr1);
  
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SUB, &instr2));
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 2);
  
  // Test insertion
  orionpp_instruction_t* instr3 = NULL;
  ASSERT_OK(orionpp_module_insert_instruction(module, 1, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MUL, &instr3));
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 3);
  ASSERT_EQ(orionpp_module_get_instruction(module, 1), instr3);
  
  // Test removal
  ASSERT_OK(orionpp_module_remove_instruction(module, 1));
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 2);
  
  // Test feature validation
  ASSERT_ERROR(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &instr1));
  
  orionpp_module_destroy(module);
}

TEST(module_validation) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_ISA, NULL));
  
  // Empty module should be valid
  ASSERT_OK(orionpp_module_validate(module));
  
  // Add valid instruction
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, &instr));
  ASSERT_OK(orionpp_module_validate(module));
  
  orionpp_module_destroy(module);
}

TEST(module_stats) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA, NULL));
  
  // Add some instructions
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &instr));
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, &instr));
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, &instr));
  
  orionpp_module_stats_t stats;
  ASSERT_OK(orionpp_module_get_stats(module, &stats));
  
  ASSERT_EQ(stats.total_instructions, 3);
  ASSERT_EQ(stats.obj_instructions, 1);
  ASSERT_EQ(stats.isa_instructions, 2);
  ASSERT_EQ(stats.abi_instructions, 0);
  ASSERT_EQ(stats.hint_instructions, 0);
  
  orionpp_module_destroy(module);
}

// Complex module creation test
TEST(create_complex_module) {
  orionpp_module_t* module = NULL;
  orionpp_features_t features = ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA | 
                                ORIONPP_FEATURES_ABI | ORIONPP_FEATURES_HINT;
  
  ASSERT_OK(orionpp_module_create(&module, features, NULL));
  
  orionpp_string_table_t* strings = orionpp_module_get_string_table(module);
  ASSERT_NOT_NULL(strings);
  
  // Add string table entries
  uint32_t text_offset, add_offset, sys_offset;
  ASSERT_OK(orionpp_string_table_add(strings, ".text", &text_offset));
  ASSERT_OK(orionpp_string_table_add(strings, "add", &add_offset));
  ASSERT_OK(orionpp_string_table_add(strings, "sys", &sys_offset));
  
  // Create .text section
  orionpp_instruction_t* text_section = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &text_section));
  ASSERT_OK(orionpp_instruction_set_obj_section(text_section, ORIONPP_SECTION_TEXT,
                                                 ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC,
                                                 text_offset));
  
  // Create add symbol
  orionpp_instruction_t* add_symbol = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SYMBOL, &add_symbol));
  ASSERT_OK(orionpp_instruction_set_obj_symbol(add_symbol, ORIONPP_SYMBOL_GLOBAL,
                                                ORIONPP_SYMBOL_FUNC, add_offset));
  
  // ABI declaration
  orionpp_instruction_t* abi_decl = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CALLEE, &abi_decl));
  ASSERT_OK(orionpp_instruction_set_abi_declaration(abi_decl, ORIONPP_ABI_CALLEE_TYPE, sys_offset));
  
  // ABI arguments
  orionpp_value_t args[2] = {
    orionpp_value_variable(0),
    orionpp_value_variable(1)
  };
  orionpp_instruction_t* abi_args = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ABI, ORIONPP_ABI_ARGS, &abi_args));
  ASSERT_OK(orionpp_instruction_set_abi_args_rets(abi_args, args, 2, NULL));
  
  // ABI returns
  orionpp_value_t rets[1] = { orionpp_value_variable(2) };
  orionpp_instruction_t* abi_rets = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ABI, ORIONPP_ABI_RETS, &abi_rets));
  ASSERT_OK(orionpp_instruction_set_abi_args_rets(abi_rets, rets, 1, NULL));
  
  // ABI setup
  orionpp_instruction_t* abi_setup = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ABI, ORIONPP_ABI_SETUP, &abi_setup));
  
  // Scope enter
  orionpp_instruction_t* scope_enter = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_ENTER, &scope_enter));
  
  // Let variable
  orionpp_instruction_t* let_var = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_LET, &let_var));
  ASSERT_OK(orionpp_instruction_set_isa_let(let_var, 3));
  
  // Move instruction
  orionpp_instruction_t* mov_instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MOV, &mov_instr));
  orionpp_value_t dest = orionpp_value_variable(3);
  orionpp_value_t src = orionpp_value_numeric(ORIONPP_BASE_DECIMAL, 10);
  ASSERT_OK(orionpp_instruction_set_isa_mov(mov_instr, &dest, &src));
  
  // Add instruction
  orionpp_instruction_t* add_instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, &add_instr));
  orionpp_value_t add_dest = orionpp_value_variable(2);
  orionpp_value_t add_src1 = orionpp_value_variable(0);
  orionpp_value_t add_src2 = orionpp_value_variable(1);
  ASSERT_OK(orionpp_instruction_set_isa_binary_op(add_instr, &add_dest, &add_src1, &add_src2));
  
  // Scope leave
  orionpp_instruction_t* scope_leave = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_LEAVE, &scope_leave));
  
  // ABI cleanup
  orionpp_instruction_t* abi_cleanup = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CLEANUP, &abi_cleanup));
  
  // Return
  orionpp_instruction_t* ret_instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, &ret_instr));
  
  // Symbol end hint
  orionpp_instruction_t* symend = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_HINT, ORIONPP_HINT_SYMEND, &symend));
  
  // Validate the module
  ASSERT_OK(orionpp_module_validate(module));
  
  // Check instruction count
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 14);
  
  orionpp_module_destroy(module);
}

// Binary I/O tests
TEST(binary_io_roundtrip) {
  orionpp_module_t* original = NULL;
  orionpp_features_t features = ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA;
  
  ASSERT_OK(orionpp_module_create(&original, features, NULL));
  
  // Add some content
  orionpp_string_table_t* strings = orionpp_module_get_string_table(original);
  uint32_t test_offset;
  ASSERT_OK(orionpp_string_table_add(strings, "test", &test_offset));
  
  orionpp_instruction_t* section = NULL;
  ASSERT_OK(orionpp_module_add_instruction(original, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &section));
  ASSERT_OK(orionpp_instruction_set_obj_section(section, ORIONPP_SECTION_TEXT,
                                                 ORIONPP_SECTION_FLAG_ALLOC, test_offset));
  
  orionpp_instruction_t* add_instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(original, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, &add_instr));
  orionpp_value_t dest = orionpp_value_variable(0);
  orionpp_value_t src1 = orionpp_value_variable(1);
  orionpp_value_t src2 = orionpp_value_variable(2);
  ASSERT_OK(orionpp_instruction_set_isa_binary_op(add_instr, &dest, &src1, &src2));
  
  ASSERT_OK(orionpp_module_validate(original));
  
  // Write to binary
  FILE* file = fopen("test_output.orion", "wb");
  ASSERT_NOT_NULL(file);
  ASSERT_OK(orionpp_module_write_binary(original, file));
  fclose(file);
  
  // Read back from binary
  file = fopen("test_output.orion", "rb");
  ASSERT_NOT_NULL(file);
  orionpp_module_t* loaded = NULL;
  ASSERT_OK(orionpp_module_read_binary(&loaded, file, NULL));
  fclose(file);
  
  // Validate loaded module
  ASSERT_OK(orionpp_module_validate(loaded));
  
  // Compare key properties
  ASSERT_EQ(original->features, loaded->features);
  ASSERT_EQ(orionpp_module_get_instruction_count(original), orionpp_module_get_instruction_count(loaded));
  ASSERT_EQ(original->version_major, loaded->version_major);
  ASSERT_EQ(original->version_minor, loaded->version_minor);
  ASSERT_EQ(original->version_patch, loaded->version_patch);
  
  // Clean up
  orionpp_module_destroy(original);
  orionpp_module_destroy(loaded);
}

// Text output tests
TEST(text_output) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA, NULL));
  
  // Add some content
  orionpp_string_table_t* strings = orionpp_module_get_string_table(module);
  uint32_t text_offset;
  ASSERT_OK(orionpp_string_table_add(strings, ".text", &text_offset));
  
  orionpp_instruction_t* section = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &section));
  ASSERT_OK(orionpp_instruction_set_obj_section(section, ORIONPP_SECTION_TEXT,
                                                 ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC,
                                                 text_offset));
  
  orionpp_instruction_t* ret_instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, &ret_instr));
  
  // Write to text format
  FILE* file = fopen("test_output.hopp", "w");
  ASSERT_NOT_NULL(file);
  
  orionpp_text_options_t options = orionpp_default_text_options;
  options.show_comments = true;
  options.show_line_numbers = false;
  
  ASSERT_OK(orionpp_module_write_text(module, file, &options));
  fclose(file);
  
  // Check that file was created and has some content
  file = fopen("test_output.hopp", "r");
  ASSERT_NOT_NULL(file);
  
  char buffer[1024];
  size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
  buffer[bytes_read] = '\0';
  fclose(file);
  
  ASSERT(bytes_read > 0);
  ASSERT(strstr(buffer, "FEATURES") != NULL);
  ASSERT(strstr(buffer, "obj.section") != NULL);
  ASSERT(strstr(buffer, "isa.ret") != NULL);
  
  orionpp_module_destroy(module);
}

TEST(text_stats_output) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_ISA, NULL));
  
  orionpp_instruction_t* instr = NULL;
  ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_NOP, &instr));
  
  // Capture stats output
  FILE* file = fopen("test_stats.txt", "w");
  ASSERT_NOT_NULL(file);
  ASSERT_OK(orionpp_module_write_stats(module, file));
  fclose(file);
  
  // Verify file exists and has content
  file = fopen("test_stats.txt", "r");
  ASSERT_NOT_NULL(file);
  char buffer[512];
  size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
  buffer[bytes_read] = '\0';
  fclose(file);
  
  ASSERT(bytes_read > 0);
  ASSERT(strstr(buffer, "Statistics") != NULL);
  ASSERT(strstr(buffer, "Total: 1") != NULL);
  
  remove("test_stats.txt");
  orionpp_module_destroy(module);
}

// Error handling tests
TEST(error_handling) {
  // Test null pointer errors
  ASSERT_ERROR(orionpp_module_create(NULL, 0, NULL));
  ASSERT_ERROR(orionpp_string_table_create(NULL, NULL));
  ASSERT_ERROR(orionpp_instruction_create(NULL, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, NULL));
  
  // Test invalid feature combinations
  orionpp_instruction_t* instr = NULL;
  ASSERT_ERROR(orionpp_instruction_create(&instr, ORIONPP_FEATURE_NONE, 1, NULL));
  ASSERT_ERROR(orionpp_instruction_create(&instr, 255, 1, NULL));
  
  // Test feature not enabled in module
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_ISA, NULL));
  ASSERT_ERROR(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &instr));
  
  orionpp_module_destroy(module);
}

// Edge cases and stress tests
TEST(empty_module_operations) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, 0, NULL)); // No features enabled
  
  ASSERT_OK(orionpp_module_validate(module));
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 0);
  ASSERT_NULL(orionpp_module_get_instruction(module, 0));
  
  // Should not be able to add any instructions
  orionpp_instruction_t* instr = NULL;
  ASSERT_ERROR(orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &instr));
  
  // But should be able to write empty module
  FILE* file = fopen("empty_test.hopp", "w");
  ASSERT_NOT_NULL(file);
  ASSERT_OK(orionpp_module_write_text(module, file, NULL));
  fclose(file);
  
  remove("empty_test.hopp");
  orionpp_module_destroy(module);
}

TEST(large_string_table) {
  orionpp_string_table_t* table = NULL;
  ASSERT_OK(orionpp_string_table_create(&table, NULL));
  
  // Add many strings to test growth and deduplication
  char buffer[64];
  uint32_t offsets[1000];
  
  for (int i = 0; i < 1000; i++) {
    snprintf(buffer, sizeof(buffer), "string_%d", i);
    ASSERT_OK(orionpp_string_table_add(table, buffer, &offsets[i]));
  }
  
  // Test deduplication by adding the same strings again
  uint32_t dup_offsets[1000];
  for (int i = 0; i < 1000; i++) {
    snprintf(buffer, sizeof(buffer), "string_%d", i);
    ASSERT_OK(orionpp_string_table_add(table, buffer, &dup_offsets[i]));
    ASSERT_EQ(offsets[i], dup_offsets[i]); // Should be deduplicated
  }
  
  // Verify all strings are retrievable
  for (int i = 0; i < 1000; i++) {
    const char* retrieved = orionpp_string_table_get(table, offsets[i]);
    ASSERT_NOT_NULL(retrieved);
    snprintf(buffer, sizeof(buffer), "string_%d", i);
    ASSERT_STR_EQ(retrieved, buffer);
  }
  
  orionpp_string_table_destroy(table);
}

TEST(instruction_capacity_growth) {
  orionpp_module_t* module = NULL;
  ASSERT_OK(orionpp_module_create(&module, ORIONPP_FEATURES_ISA, NULL));
  
  // Add many instructions to test capacity growth
  for (int i = 0; i < 100; i++) {
    orionpp_instruction_t* instr = NULL;
    ASSERT_OK(orionpp_module_add_instruction(module, ORIONPP_FEATURE_ISA, ORIONPP_ISA_NOP, &instr));
  }
  
  ASSERT_EQ(orionpp_module_get_instruction_count(module), 100);
  
  // Verify all instructions are accessible
  for (int i = 0; i < 100; i++) {
    orionpp_instruction_t* instr = orionpp_module_get_instruction(module, i);
    ASSERT_NOT_NULL(instr);
    ASSERT_EQ(instr->feature, ORIONPP_FEATURE_ISA);
    ASSERT_EQ(instr->opcode, ORIONPP_ISA_NOP);
  }
  
  orionpp_module_destroy(module);
}

// Main test runner
int main(void) {
  printf("Running OrionPP Library Test Suite\n");
  printf("===================================\n\n");
  
  cleanup_files();
  
  // String table tests
  RUN_TEST(string_table_create_destroy);
  RUN_TEST(string_table_add_get);
  RUN_TEST(string_table_add_length);
  
  // Value tests
  RUN_TEST(value_variable);
  RUN_TEST(value_numeric);
  RUN_TEST(value_symbol);
  RUN_TEST(value_label);
  RUN_TEST(value_string);
  RUN_TEST(value_array);
  RUN_TEST(value_copy);
  RUN_TEST(value_parse_from_text);
  
  // Instruction tests
  RUN_TEST(instruction_create_destroy);
  RUN_TEST(instruction_feature_validation);
  RUN_TEST(instruction_obj_section);
  RUN_TEST(instruction_obj_symbol);
  RUN_TEST(instruction_isa_let);
  RUN_TEST(instruction_isa_mov);
  RUN_TEST(instruction_isa_binary_op);
  RUN_TEST(instruction_abi_declaration);
  RUN_TEST(instruction_abi_args_rets);
  
  // Module tests
  RUN_TEST(module_create_destroy);
  RUN_TEST(module_feature_management);
  RUN_TEST(module_instruction_management);
  RUN_TEST(module_validation);
  RUN_TEST(module_stats);
  RUN_TEST(create_complex_module);
  
  // I/O tests
  RUN_TEST(binary_io_roundtrip);
  RUN_TEST(text_output);
  RUN_TEST(text_stats_output);
  
  // Error handling tests
  RUN_TEST(error_handling);
  
  // Edge cases and stress tests
  RUN_TEST(empty_module_operations);
  RUN_TEST(large_string_table);
  RUN_TEST(instruction_capacity_growth);
  
  cleanup_files();
  
  printf("\n===================================\n");
  printf("Test Results:\n");
  printf("  Total tests: %d\n", test_count);
  printf("  Passed:      %d\n", test_passed);
  printf("  Failed:      %d\n", test_failed);
  
  if (test_failed == 0) {
    printf("\n🎉 All tests passed!\n");
    return 0;
  } else {
    printf("\n❌ %d test(s) failed!\n", test_failed);
    return 1;
  }
}