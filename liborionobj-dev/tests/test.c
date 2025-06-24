#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "orionobj/orionobj.h"

static int test_count = 0;
static int test_passed = 0;

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
    exit(1); \
  } \
} while(0)

#define ASSERT_OK(result) ASSERT(ORIONOBJ_IS_OK(result))
#define ASSERT_NOT_NULL(ptr) ASSERT((ptr) != NULL)
#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_STR_EQ(a, b) ASSERT(strcmp((a), (b)) == 0)

TEST(string_table_basic) {
  orionobj_string_table_t* table = NULL;
  ASSERT_OK(orionobj_string_table_create(&table, NULL));
  ASSERT_NOT_NULL(table);

  uint32_t offset1, offset2, offset3;
  ASSERT_OK(orionobj_string_table_add(table, "hello", &offset1));
  ASSERT_OK(orionobj_string_table_add(table, "world", &offset2));
  ASSERT_OK(orionobj_string_table_add(table, "hello", &offset3)); // Duplicate

  ASSERT_EQ(offset1, offset3); // Should deduplicate
  ASSERT_STR_EQ(orionobj_string_table_get(table, offset1), "hello");
  ASSERT_STR_EQ(orionobj_string_table_get(table, offset2), "world");

  orionobj_string_table_destroy(table);
}

TEST(object_creation) {
  orionobj_object_t* object = NULL;
  ASSERT_OK(orionobj_object_create(&object, ORIONOBJ_FORMAT_ORIONPP, 
                                   ORIONOBJ_ARCH_ORION_X86, NULL));
  ASSERT_NOT_NULL(object);
  ASSERT_EQ(orionobj_object_get_format(object), ORIONOBJ_FORMAT_ORIONPP);
  ASSERT_EQ(orionobj_object_get_architecture(object), ORIONOBJ_ARCH_ORION_X86);
  ASSERT_NOT_NULL(orionobj_object_get_string_table(object));

  orionobj_object_destroy(object);
}

TEST(section_management) {
  orionobj_object_t* object = NULL;
  ASSERT_OK(orionobj_object_create(&object, ORIONOBJ_FORMAT_ORIONPP, 
                                   ORIONOBJ_ARCH_ORION_X86, NULL));

  orionobj_section_t* text_section = NULL;
  ASSERT_OK(orionobj_object_add_section(object, ORIONOBJ_SECTION_TEXT, ".text", 
                                         ORIONOBJ_SECTION_FLAG_ALLOC | ORIONOBJ_SECTION_FLAG_EXEC,
                                         &text_section));
  ASSERT_NOT_NULL(text_section);
  ASSERT_EQ(orionobj_object_get_section_count(object), 1);

  orionobj_section_t* data_section = NULL;
  ASSERT_OK(orionobj_object_add_section(object, ORIONOBJ_SECTION_DATA, ".data",
                                         ORIONOBJ_SECTION_FLAG_ALLOC | ORIONOBJ_SECTION_FLAG_WRITE,
                                         &data_section));
  ASSERT_EQ(orionobj_object_get_section_count(object), 2);

  // Test section lookup
  orionobj_section_t* found = orionobj_object_find_section(object, ".text");
  ASSERT_EQ(found, text_section);

  orionobj_object_destroy(object);
}

TEST(symbol_management) {
  orionobj_object_t* object = NULL;
  ASSERT_OK(orionobj_object_create(&object, ORIONOBJ_FORMAT_ORIONPP, 
                                   ORIONOBJ_ARCH_ORION_X86, NULL));

  orionobj_symbol_t* symbol = NULL;
  ASSERT_OK(orionobj_object_add_symbol(object, "main", ORIONOBJ_SYMBOL_GLOBAL,
                                        ORIONOBJ_SYMBOL_FUNC, 1, 0x1000, 128, &symbol));
  ASSERT_NOT_NULL(symbol);
  ASSERT_EQ(orionobj_object_get_symbol_count(object), 1);

  // Test symbol lookup
  orionobj_symbol_t* found = orionobj_object_find_symbol(object, "main");
  ASSERT_EQ(found, symbol);
  ASSERT_EQ(orionobj_symbol_get_value(symbol), 0x1000);
  ASSERT_EQ(orionobj_symbol_get_size(symbol), 128);

  orionobj_object_destroy(object);
}

TEST(basic_io) {
  orionobj_object_t* original = NULL;
  ASSERT_OK(orionobj_object_create(&original, ORIONOBJ_FORMAT_ORIONPP, 
                                   ORIONOBJ_ARCH_ORION_X86, NULL));

  // Add some content
  orionobj_section_t* section = NULL;
  ASSERT_OK(orionobj_object_add_section(original, ORIONOBJ_SECTION_TEXT, ".text",
                                         ORIONOBJ_SECTION_FLAG_ALLOC | ORIONOBJ_SECTION_FLAG_EXEC,
                                         &section));

  orionobj_symbol_t* symbol = NULL;
  ASSERT_OK(orionobj_object_add_symbol(original, "test", ORIONOBJ_SYMBOL_GLOBAL,
                                        ORIONOBJ_SYMBOL_FUNC, 1, 0, 0, &symbol));

  // Write to file
  ASSERT_OK(orionobj_object_write_to_file(original, "test_output.orion"));

  // Read back
  orionobj_object_t* loaded = NULL;
  ASSERT_OK(orionobj_object_read_from_file(&loaded, "test_output.orion", NULL));

  // Verify basic properties
  ASSERT_EQ(orionobj_object_get_format(loaded), ORIONOBJ_FORMAT_ORIONPP);
  ASSERT_EQ(orionobj_object_get_architecture(loaded), ORIONOBJ_ARCH_ORION_X86);

  // Clean up
  orionobj_object_destroy(original);
  orionobj_object_destroy(loaded);
  remove("test_output.orion");
}

TEST(object_validation) {
  orionobj_object_t* object = NULL;
  ASSERT_OK(orionobj_object_create(&object, ORIONOBJ_FORMAT_ORIONPP, 
                                   ORIONOBJ_ARCH_ORION_X86, NULL));

  // Empty object should be valid
  ASSERT_OK(orionobj_object_validate(object));

  // Add content and validate again
  orionobj_section_t* section = NULL;
  ASSERT_OK(orionobj_object_add_section(object, ORIONOBJ_SECTION_TEXT, ".text",
                                         ORIONOBJ_SECTION_FLAG_ALLOC, &section));
  ASSERT_OK(orionobj_object_validate(object));

  orionobj_object_destroy(object);
}

int main(void) {
  printf("Running OrionObj Library Test Suite\n");
  printf("====================================\n\n");

  RUN_TEST(string_table_basic);
  RUN_TEST(object_creation);
  RUN_TEST(section_management);
  RUN_TEST(symbol_management);
  RUN_TEST(basic_io);
  RUN_TEST(object_validation);

  printf("\n====================================\n");
  printf("Test Results:\n");
  printf("  Total tests: %d\n", test_count);
  printf("  Passed:      %d\n", test_passed);

  if (test_passed == test_count) {
    printf("\n🎉 All tests passed!\n");
    return 0;
  } else {
    printf("\n❌ Some tests failed!\n");
    return 1;
  }
}