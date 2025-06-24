#include <stdio.h>
#include <stdlib.h>
#include "orionpp/orionpp.h"

int main(void) {
  printf("OrionPP Simple Example\n");
  printf("======================\n\n");
  
  // Initialize the library
  orionpp_result_t init_result = orionpp_init();
  if (ORIONPP_IS_ERROR(init_result)) {
    printf("Failed to initialize OrionPP: %s\n", orionpp_error_string(init_result.error));
    return 1;
  }
  
  printf("OrionPP Version: %s\n\n", orionpp_get_version_string());
  
  // Create a module with OBJ, ISA, ABI, and HINT features
  orionpp_module_t* module = NULL;
  orionpp_features_t features = ORIONPP_FEATURES_OBJ | ORIONPP_FEATURES_ISA | 
                                ORIONPP_FEATURES_ABI | ORIONPP_FEATURES_HINT;
  
  orionpp_result_t create_result = orionpp_module_create(&module, features, NULL);
  if (ORIONPP_IS_ERROR(create_result)) {
    printf("Failed to create module: %s\n", orionpp_error_string(create_result.error));
    orionpp_cleanup();
    return 1;
  }
  
  printf("Created module with features: OBJ, ISA, ABI, HINT\n");
  
  // Get string table for adding strings
  orionpp_string_table_t* strings = orionpp_module_get_string_table(module);
  
  // Create .text section using builder function
  orionpp_instruction_t* text_section = NULL;
  orionpp_result_t section_result = orionpp_instruction_build_obj_section(
    &text_section, ORIONPP_SECTION_TEXT, 
    ORIONPP_SECTION_FLAG_ALLOC | ORIONPP_SECTION_FLAG_EXEC,
    ".text", strings, NULL);
  
  if (ORIONPP_IS_OK(section_result)) {
    orionpp_module_add_instruction(module, text_section);
    printf("Added .text section\n");
  }
  
  // Create add function symbol using builder function
  orionpp_instruction_t* add_symbol = NULL;
  orionpp_result_t symbol_result = orionpp_instruction_build_obj_symbol(
    &add_symbol, ORIONPP_SYMBOL_GLOBAL, ORIONPP_SYMBOL_FUNC, 
    "add", strings, NULL);
  
  if (ORIONPP_IS_OK(symbol_result)) {
    orionpp_module_add_instruction(module, add_symbol);
    printf("Added 'add' function symbol\n");
  }
  
  // Add ABI declaration
  orionpp_instruction_t* abi_callee = NULL;
  orionpp_result_t abi_create = orionpp_instruction_create(&abi_callee, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CALLEE, NULL);
  if (ORIONPP_IS_OK(abi_create)) {
    uint32_t sys_offset;
    orionpp_string_table_add(strings, "system", &sys_offset);
    orionpp_instruction_set_abi_declaration(abi_callee, ORIONPP_ABI_CALLEE_TYPE, sys_offset);
    orionpp_module_add_instruction(module, abi_callee);
    printf("Added ABI callee declaration\n");
  }
  
  // Add ABI arguments using builder
  orionpp_instruction_t* abi_args = NULL;
  orionpp_result_t args_create = orionpp_instruction_create(&abi_args, ORIONPP_FEATURE_ABI, ORIONPP_ABI_ARGS, NULL);
  if (ORIONPP_IS_OK(args_create)) {
    orionpp_value_t args[2] = {
      orionpp_value_variable(0),
      orionpp_value_variable(1)
    };
    orionpp_instruction_set_abi_args_rets(abi_args, args, 2, NULL);
    orionpp_module_add_instruction(module, abi_args);
    printf("Added ABI arguments: $0, $1\n");
  }
  
  // Add ABI returns
  orionpp_instruction_t* abi_rets = NULL;
  orionpp_result_t rets_create = orionpp_instruction_create(&abi_rets, ORIONPP_FEATURE_ABI, ORIONPP_ABI_RETS, NULL);
  if (ORIONPP_IS_OK(rets_create)) {
    orionpp_value_t rets[1] = { orionpp_value_variable(2) };
    orionpp_instruction_set_abi_args_rets(abi_rets, rets, 1, NULL);
    orionpp_module_add_instruction(module, abi_rets);
    printf("Added ABI returns: $2\n");
  }
  
  // Add ABI setup
  orionpp_instruction_t* abi_setup = NULL;
  orionpp_result_t setup_create = orionpp_instruction_create(&abi_setup, ORIONPP_FEATURE_ABI, ORIONPP_ABI_SETUP, NULL);
  if (ORIONPP_IS_OK(setup_create)) {
    orionpp_module_add_instruction(module, abi_setup);
    printf("Added ABI setup\n");
  }
  
  // Add scope enter
  orionpp_instruction_t* scope_enter = NULL;
  orionpp_result_t enter_create = orionpp_instruction_create(&scope_enter, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_ENTER, NULL);
  if (ORIONPP_IS_OK(enter_create)) {
    orionpp_module_add_instruction(module, scope_enter);
    printf("Added scope enter\n");
  }
  
  // Add variable declaration: isa.let $3
  orionpp_instruction_t* let_var = NULL;
  orionpp_result_t let_create = orionpp_instruction_create(&let_var, ORIONPP_FEATURE_ISA, ORIONPP_ISA_LET, NULL);
  if (ORIONPP_IS_OK(let_create)) {
    orionpp_instruction_set_isa_let(let_var, 3);
    orionpp_module_add_instruction(module, let_var);
    printf("Added variable declaration: $3\n");
  }
  
  // Add move instruction using builder: isa.mov $3, %d10
  orionpp_instruction_t* mov_instr = NULL;
  orionpp_result_t mov_result = orionpp_instruction_build_isa_mov_var_const(&mov_instr, 3, 10, NULL);
  if (ORIONPP_IS_OK(mov_result)) {
    orionpp_module_add_instruction(module, mov_instr);
    printf("Added move instruction: $3 = 10\n");
  }
  
  // Add addition instruction using builder: isa.add $2, $0, $1
  orionpp_instruction_t* add_instr = NULL;
  orionpp_result_t add_result = orionpp_instruction_build_isa_add_vars(&add_instr, 2, 0, 1, NULL);
  if (ORIONPP_IS_OK(add_result)) {
    orionpp_module_add_instruction(module, add_instr);
    printf("Added addition: $2 = $0 + $1\n");
  }
  
  // Add scope leave
  orionpp_instruction_t* scope_leave = NULL;
  orionpp_result_t leave_create = orionpp_instruction_create(&scope_leave, ORIONPP_FEATURE_ISA, ORIONPP_ISA_SCOPE_LEAVE, NULL);
  if (ORIONPP_IS_OK(leave_create)) {
    orionpp_module_add_instruction(module, scope_leave);
    printf("Added scope leave\n");
  }
  
  // Add ABI cleanup
  orionpp_instruction_t* abi_cleanup = NULL;
  orionpp_result_t cleanup_create = orionpp_instruction_create(&abi_cleanup, ORIONPP_FEATURE_ABI, ORIONPP_ABI_CLEANUP, NULL);
  if (ORIONPP_IS_OK(cleanup_create)) {
    orionpp_module_add_instruction(module, abi_cleanup);
    printf("Added ABI cleanup\n");
  }
  
  // Add return
  orionpp_instruction_t* ret_instr = NULL;
  orionpp_result_t ret_create = orionpp_instruction_create(&ret_instr, ORIONPP_FEATURE_ISA, ORIONPP_ISA_RET, NULL);
  if (ORIONPP_IS_OK(ret_create)) {
    orionpp_module_add_instruction(module, ret_instr);
    printf("Added return\n");
  }
  
  // Add symbol end hint
  orionpp_instruction_t* symend = NULL;
  orionpp_result_t hint_create = orionpp_instruction_create(&symend, ORIONPP_FEATURE_HINT, ORIONPP_HINT_SYMEND, NULL);
  if (ORIONPP_IS_OK(hint_create)) {
    orionpp_module_add_instruction(module, symend);
    printf("Added symbol end hint\n");
  }
  
  printf("\nModule created with %u instructions\n", orionpp_module_get_instruction_count(module));
  
  // Validate the module
  orionpp_result_t validate_result = orionpp_module_validate(module);
  if (ORIONPP_IS_OK(validate_result)) {
    printf("Module validation: PASSED\n");
  } else {
    printf("Module validation: FAILED (%s)\n", orionpp_error_string(validate_result.error));
  }
  
  // Get module statistics
  orionpp_module_stats_t stats;
  orionpp_result_t stats_result = orionpp_module_get_stats(module, &stats);
  if (ORIONPP_IS_OK(stats_result)) {
    printf("\nModule Statistics:\n");
    printf("  Total instructions: %u\n", stats.total_instructions);
    printf("  OBJ instructions:   %u\n", stats.obj_instructions);
    printf("  ISA instructions:   %u\n", stats.isa_instructions);
    printf("  ABI instructions:   %u\n", stats.abi_instructions);
    printf("  HINT instructions:  %u\n", stats.hint_instructions);
    printf("  String table size:  %u bytes\n", stats.string_table_size);
    printf("  Estimated binary:   %u bytes\n", stats.estimated_binary_size);
  }
  
  // Write text output
  printf("\nWriting text output to 'simple_example.hopp'...\n");
  FILE* text_file = fopen("simple_example.hopp", "w");
  if (text_file) {
    orionpp_result_t text_result = orionpp_module_write_text(module, text_file, NULL);
    fclose(text_file);
    if (ORIONPP_IS_OK(text_result)) {
      printf("Text output: SUCCESS\n");
    } else {
      printf("Text output: FAILED (%s)\n", orionpp_error_string(text_result.error));
    }
  }
  
  // Write binary output
  printf("Writing binary output to 'simple_example.orionpp'...\n");
  FILE* binary_file = fopen("simple_example.orionpp", "wb");
  if (binary_file) {
    orionpp_result_t binary_result = orionpp_module_write_binary(module, binary_file);
    fclose(binary_file);
    if (ORIONPP_IS_OK(binary_result)) {
      printf("Binary output: SUCCESS\n");
    } else {
      printf("Binary output: FAILED (%s)\n", orionpp_error_string(binary_result.error));
    }
  }
  
  // Write OrionObj output
  printf("Writing OrionObj output to 'simple_example.orion'...\n");
  orionpp_orionobj_options_t obj_options = orionpp_default_orionobj_options;
  obj_options.target_arch = ORIONPP_ARCH_ORION_X86;
  obj_options.entry_point = "add";
  
  orionpp_result_t obj_result = orionpp_module_write_orionobj(module, "simple_example.orion", &obj_options);
  if (ORIONPP_IS_OK(obj_result)) {
    printf("OrionObj output: SUCCESS\n");
  } else {
    printf("OrionObj output: FAILED (%s)\n", orionpp_error_string(obj_result.error));
  }
  
  // Clean up
  orionpp_module_destroy(module);
  orionpp_cleanup();
  
  printf("\nSimple example completed successfully!\n");
  printf("Generated files:\n");
  printf("  - simple_example.hopp    (text format)\n");
  printf("  - simple_example.orionpp (binary format)\n");
  printf("  - simple_example.orion   (object format)\n");
  
  return 0;
}