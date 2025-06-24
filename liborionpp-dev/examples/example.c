#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orionpp/module.h"
#include "orionpp/binary.h"
#include "orionpp/text.h"
#include "orionpp/value.h"

// Example: Create a simple add function module
static orionpp_result_t create_add_function_module(orionpp_module_t** module) {
  orionpp_result_t result;
  
  // Create module with all features enabled
  orionpp_features_t features = ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA | 
                                ORIONPP_FEATURES_ABI | ORIONPP_FEATURES_HINT;
  
  result = orionpp_module_create(module, features, NULL);
  if (ORIONPP_IS_ERROR(result)) {
    return result;
  }
  
  orionpp_module_t* mod = *module;
  orionpp_string_table_t* strings = orionpp_module_get_string_table(mod);
  
  // Add string table entries
  uint32_t text_section_offset, add_symbol_offset, abi_offset;
  orionpp_string_table_add(strings, ".text", &text_section_offset);
  orionpp_string_table_add(strings, "add", &add_symbol_offset);
  orionpp_string_table_add(strings, "sys", &abi_offset);
  
  // Create .text section
  orionpp_instruction_t* text_section;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &text_section);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_obj_section(text_section, ORIONPP_SECTION_TEXT, 
                                               ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC,
                                               text_section_offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Create add symbol
  orionpp_instruction_t* add_symbol;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SYMBOL, &add_symbol);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_obj_symbol(add_symbol, ORIONPP_SYMBOL_GLOBAL, 
                                              ORIONPP_SYMBOL_FUNC, add_symbol_offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // ABI declaration
  orionpp_instruction_t* abi_decl;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CALLEE, &abi_decl);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_abi_declaration(abi_decl, ORIONPP_ABI_CALLEE_TYPE, abi_offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // ABI arguments: [0, 1]
  orionpp_value_t args[2] = {
    orionpp_value_variable(0),
    orionpp_value_variable(1)
  };
  
  orionpp_instruction_t* abi_args;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ABI, ORIONPP_ABI_ARGS, &abi_args);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_abi_args_rets(abi_args, args, 2, NULL);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // ABI returns: [2]
  orionpp_value_t rets[1] = {
    orionpp_value_variable(2)
  };
  
  orionpp_instruction_t* abi_rets;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ABI, ORIONPP_ABI_RETS, &abi_rets);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_abi_args_rets(abi_rets, rets, 1, NULL);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // ABI setup
  orionpp_instruction_t* abi_setup;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ABI, ORIONPP_ABI_SETUP, &abi_setup);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Scope enter
  orionpp_instruction_t* scope_enter;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_ENTER, &scope_enter);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Let $3 (local variable)
  orionpp_instruction_t* let_3;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_LET, &let_3);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_isa_let(let_3, 3);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // mov $3, %d10
  orionpp_instruction_t* mov_3;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_MOV, &mov_3);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  orionpp_value_t dest_3 = orionpp_value_variable(3);
  orionpp_value_t src_10 = orionpp_value_numeric(ORIONPP_BASE_DECIMAL, 10);
  
  result = orionpp_instruction_set_isa_mov(mov_3, &dest_3, &src_10);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // add $2, $0, $1
  orionpp_instruction_t* add_instr;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_ADD, &add_instr);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  orionpp_value_t dest_2 = orionpp_value_variable(2);
  orionpp_value_t src1_0 = orionpp_value_variable(0);
  orionpp_value_t src2_1 = orionpp_value_variable(1);
  
  result = orionpp_instruction_set_isa_binary_op(add_instr, &dest_2, &src1_0, &src2_1);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Scope leave
  orionpp_instruction_t* scope_leave;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_LEAVE, &scope_leave);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // ABI cleanup
  orionpp_instruction_t* abi_cleanup;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CLEANUP, &abi_cleanup);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Return
  orionpp_instruction_t* ret_instr;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, &ret_instr);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Symbol end hint
  orionpp_instruction_t* symend_hint;
  result = orionpp_module_add_instruction(mod, ORIONPP_FEATURE_HINT, ORIONPP_HINT_SYMEND, &symend_hint);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  return ORIONPP_OK_PTR(mod);
}

// Example: Create a data section with hello world string
static orionpp_result_t add_data_section(orionpp_module_t* module) {
  orionpp_result_t result;
  orionpp_string_table_t* strings = orionpp_module_get_string_table(module);
  
  // Add string table entries
  uint32_t data_section_offset, hello_symbol_offset;
  orionpp_string_table_add(strings, ".data", &data_section_offset);
  orionpp_string_table_add(strings, "hello_world_str", &hello_symbol_offset);
  
  // Create .data section
  orionpp_instruction_t* data_section;
  result = orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SECTION, &data_section);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_obj_section(data_section, ORIONPP_SECTION_DATA, 
                                               ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_WRITE,
                                               data_section_offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Create hello_world_str symbol
  orionpp_instruction_t* hello_symbol;
  result = orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_SYMBOL, &hello_symbol);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  result = orionpp_instruction_set_obj_symbol(hello_symbol, ORIONPP_SYMBOL_GLOBAL, 
                                              ORIONPP_SYMBOL_OBJECT, hello_symbol_offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Add bytes for "Hello, World!\0"
  const char* hello_str = "Hello, World!";
  for (size_t i = 0; i < strlen(hello_str); i++) {
    orionpp_instruction_t* byte_instr;
    result = orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_BYTE, &byte_instr);
    if (ORIONPP_IS_ERROR(result)) return result;
    
    byte_instr->obj_data.value = orionpp_value_numeric(ORIONPP_BASE_HEX, (uint8_t)hello_str[i]);
  }
  
  // Add null terminator
  orionpp_instruction_t* null_byte;
  result = orionpp_module_add_instruction(module, ORIONPP_FEATURE_OBJ, ORIONPP_OBJ_BYTE, &null_byte);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  null_byte->obj_data.value = orionpp_value_numeric(ORIONPP_BASE_HEX, 0);
  
  // Symbol end hint
  orionpp_instruction_t* symend_hint;
  result = orionpp_module_add_instruction(module, ORIONPP_FEATURE_HINT, ORIONPP_HINT_SYMEND, &symend_hint);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  return ORIONPP_OK_INT(0);
}

static void print_error(const char* context, orionpp_result_t result) {
  const char* error_msg = "Unknown error";
  
  switch (result.error) {
    case ORIONPP_ERROR_NULL_POINTER:      error_msg = "Null pointer"; break;
    case ORIONPP_ERROR_OUT_OF_MEMORY:     error_msg = "Out of memory"; break;
    case ORIONPP_ERROR_INVALID_FEATURE:   error_msg = "Invalid feature"; break;
    case ORIONPP_ERROR_INVALID_INSTRUCTION: error_msg = "Invalid instruction"; break;
    case ORIONPP_ERROR_IO_ERROR:          error_msg = "I/O error"; break;
    case ORIONPP_ERROR_PARSE_ERROR:       error_msg = "Parse error"; break;
    case ORIONPP_ERROR_VERSION_MISMATCH:  error_msg = "Version mismatch"; break;
    case ORIONPP_ERROR_CORRUPT_DATA:      error_msg = "Corrupt data"; break;
  }
  
  fprintf(stderr, "Error in %s: %s\n", context, error_msg);
}

int main(int argc, char* argv[]) {
  orionpp_result_t result;
  orionpp_module_t* module = NULL;
  
  printf("OrionPP Library Example\n");
  printf("======================\n\n");
  
  // Create the add function module
  printf("1. Creating add function module...\n");
  result = create_add_function_module(&module);
  if (ORIONPP_IS_ERROR(result)) {
    print_error("create_add_function_module", result);
    return 1;
  }
  
  // Add data section
  printf("2. Adding data section...\n");
  result = add_data_section(module);
  if (ORIONPP_IS_ERROR(result)) {
    print_error("add_data_section", result);
    orionpp_module_destroy(module);
    return 1;
  }
  
  // Validate the module
  printf("3. Validating module...\n");
  result = orionpp_module_validate(module);
  if (ORIONPP_IS_ERROR(result)) {
    print_error("orionpp_module_validate", result);
    orionpp_module_destroy(module);
    return 1;
  }
  printf("   Module is valid!\n\n");
  
  // Print module statistics
  printf("4. Module Statistics:\n");
  result = orionpp_module_write_stats(module, stdout);
  if (ORIONPP_IS_ERROR(result)) {
    print_error("orionpp_module_write_stats", result);
  }
  printf("\n");
  
  // Write module to text format
  printf("5. Writing module to text format...\n");
  FILE* text_file = fopen("example_output.hopp", "w");
  if (!text_file) {
    fprintf(stderr, "Failed to open example_output.hopp for writing\n");
    orionpp_module_destroy(module);
    return 1;
  }
  
  orionpp_text_options_t text_options = orionpp_default_text_options;
  text_options.show_comments = true;
  text_options.show_line_numbers = false;
  
  result = orionpp_module_write_text(module, text_file, &text_options);
  fclose(text_file);
  
  if (ORIONPP_IS_ERROR(result)) {
    print_error("orionpp_module_write_text", result);
    orionpp_module_destroy(module);
    return 1;
  }
  printf("   Text format written to: example_output.hopp\n");
  
  // Write module to binary format
  printf("6. Writing module to binary format...\n");
  FILE* binary_file = fopen("example_output.orion", "wb");
  if (!binary_file) {
    fprintf(stderr, "Failed to open example_output.orion for writing\n");
    orionpp_module_destroy(module);
    return 1;
  }
  
  result = orionpp_module_write_binary(module, binary_file);
  fclose(binary_file);
  
  if (ORIONPP_IS_ERROR(result)) {
    print_error("orionpp_module_write_binary", result);
    orionpp_module_destroy(module);
    return 1;
  }
  printf("   Binary format written to: example_output.orion\n");
  
  // Test reading the binary format back
  printf("7. Reading binary format back...\n");
  binary_file = fopen("example_output.orion", "rb");
  if (!binary_file) {
    fprintf(stderr, "Failed to open example_output.orion for reading\n");
    orionpp_module_destroy(module);
    return 1;
  }
  
  orionpp_module_t* loaded_module = NULL;
  result = orionpp_module_read_binary(&loaded_module, binary_file, NULL);
  fclose(binary_file);
  
  if (ORIONPP_IS_ERROR(result)) {
    print_error("orionpp_module_read_binary", result);
    orionpp_module_destroy(module);
    return 1;
  }
  
  // Validate the loaded module
  result = orionpp_module_validate(loaded_module);
  if (ORIONPP_IS_ERROR(result)) {
    print_error("validate loaded module", result);
    orionpp_module_destroy(module);
    orionpp_module_destroy(loaded_module);
    return 1;
  }
  printf("   Loaded module is valid!\n");
  
  // Compare instruction counts
  uint32_t orig_count = orionpp_module_get_instruction_count(module);
  uint32_t loaded_count = orionpp_module_get_instruction_count(loaded_module);
  
  if (orig_count == loaded_count) {
    printf("   Instruction counts match: %u instructions\n", orig_count);
  } else {
    printf("   Warning: Instruction count mismatch (orig: %u, loaded: %u)\n", 
           orig_count, loaded_count);
  }
  
  // Write loaded module to text for comparison
  printf("8. Writing loaded module to text format...\n");
  text_file = fopen("example_loaded.hopp", "w");
  if (text_file) {
    result = orionpp_module_write_text(loaded_module, text_file, &text_options);
    fclose(text_file);
    
    if (ORIONPP_IS_OK(result)) {
      printf("   Loaded module text written to: example_loaded.hopp\n");
    }
  }
  
  // Clean up
  orionpp_module_destroy(module);
  orionpp_module_destroy(loaded_module);
  
  printf("\nExample completed successfully!\n");
  printf("Generated files:\n");
  printf("  - example_output.hopp (text format)\n");
  printf("  - example_output.orion (binary format)\n");
  printf("  - example_loaded.hopp (loaded from binary)\n");
  
  return 0;
}