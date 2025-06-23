#include "../include/orionobj.h"
#include "../include/orionobj_utils.h"
#include <stdio.h>
#include <string.h>

void create_orionpp_example(void) {
  printf("Creating Orion++ example...\n");
  
  // Create an Orion++ object
  OrionObject* obj = orion_create_orionpp_object();
  if (!obj) {
    printf("Failed to create object: %s\n", orion_get_error_string(orion_get_last_error()));
    return;
  }
  
  // Get sections
  OrionSection* text_section = orion_obj_get_section(obj, ".orionpp.text");
  OrionSection* data_section = orion_obj_get_section(obj, ".orionpp.data");
  
  if (!text_section || !data_section) {
    printf("Failed to get sections\n");
    orion_obj_free(obj);
    return;
  }
  
  // Add some example Orion++ code
  const char* orionpp_code = 
    "obj.section [EXEC] .text\n"
    "obj.sym[global,func] main:\n"
    "  .hint-abi(c)\n"
    "  .hint-args(null)\n"
    "  .hint-rets(i32)\n"
    "  abi.callee.enter\n"
    "  isa.enter\n"
    "    isa.let a, 10\n"
    "    isa.let b, 20\n"
    "    isa.add result, a, b\n"
    "    abi.callee.setret.0 result\n"
    "  isa.leave\n"
    "  abi.callee.leave\n"
    "  isa.RET\n"
    ".hint-endfunc()\n";
  
  orion_section_set_data(text_section, orionpp_code, strlen(orionpp_code));
  
  // Add some example data
  const char* hello_msg = "Hello from Orion++!\n";
  orion_section_set_data(data_section, hello_msg, strlen(hello_msg) + 1);
  
  // Add symbols
  uint32_t text_index = orion_get_section_index(obj, ".orionpp.text");
  uint32_t data_index = orion_get_section_index(obj, ".orionpp.data");
  
  orion_add_global_function(obj, "main", text_index, 0, strlen(orionpp_code));
  orion_add_global_data(obj, "hello_msg", data_index, 0, strlen(hello_msg) + 1);
  
  // Print object info
  printf("\nOrion++ Object Info:\n");
  orion_print_object_info(obj);
  
  // Save object
  if (orion_obj_save(obj, "example_orionpp.orion") != 0) {
    printf("Failed to save object: %s\n", orion_get_error_string(orion_get_last_error()));
  } else {
    printf("\nSaved Orion++ object to example_orionpp.orion\n");
  }
  
  orion_obj_free(obj);
}

void create_orion_variant_example(void) {
  printf("\nCreating Orion variant example...\n");
  
  // Create an Orion x86 variant object
  OrionObject* obj = orion_create_orion_variant_object(ORION_VAR_X86);
  if (!obj) {
    printf("Failed to create object: %s\n", orion_get_error_string(orion_get_last_error()));
    return;
  }
  
  // Get sections
  OrionSection* text_section = orion_obj_get_section(obj, ".orion.x86.text");
  
  if (!text_section) {
    printf("Failed to get text section\n");
    orion_obj_free(obj);
    return;
  }
  
  // Add some example Orion x86 code (placeholder)
  const char* orion_x86_code = 
    "// Orion x86 variant code\n"
    "movl $10, %eax\n"
    "movl $20, %ebx\n"
    "addl %ebx, %eax\n"
    "ret\n";
  
  orion_section_set_data(text_section, orion_x86_code, strlen(orion_x86_code));
  
  // Add symbol
  uint32_t text_index = orion_get_section_index(obj, ".orion.x86.text");
  orion_add_global_function(obj, "add_function", text_index, 0, strlen(orion_x86_code));
  
  // Print object info
  printf("\nOrion x86 Variant Object Info:\n");
  orion_print_object_info(obj);
  
  // Save object
  if (orion_obj_save(obj, "example_orion_x86.orion") != 0) {
    printf("Failed to save object: %s\n", orion_get_error_string(orion_get_last_error()));
  } else {
    printf("\nSaved Orion x86 object to example_orion_x86.orion\n");
  }
  
  orion_obj_free(obj);
}

void create_native_example(void) {
  printf("\nCreating native example...\n");
  
  // Create a native x86_64 object
  OrionObject* obj = orion_create_native_object(ORION_TARGET_X86_64);
  if (!obj) {
    printf("Failed to create object: %s\n", orion_get_error_string(orion_get_last_error()));
    return;
  }
  
  // Get sections
  OrionSection* text_section = orion_obj_get_section(obj, ".text");
  OrionSection* data_section = orion_obj_get_section(obj, ".data");
  
  if (!text_section || !data_section) {
    printf("Failed to get sections\n");
    orion_obj_free(obj);
    return;
  }
  
  // Add some example native x86_64 machine code (simplified)
  unsigned char native_code[] = {
    0x48, 0xc7, 0xc0, 0x0a, 0x00, 0x00, 0x00,  // mov $10, %rax
    0x48, 0xc7, 0xc3, 0x14, 0x00, 0x00, 0x00,  // mov $20, %rbx
    0x48, 0x01, 0xd8,                          // add %rbx, %rax
    0xc3                                       // ret
  };
  
  orion_section_set_data(text_section, native_code, sizeof(native_code));
  
  // Add some data
  int data_value = 42;
  orion_section_set_data(data_section, &data_value, sizeof(data_value));
  
  // Add symbols
  uint32_t text_index = orion_get_section_index(obj, ".text");
  uint32_t data_index = orion_get_section_index(obj, ".data");
  
  orion_add_global_function(obj, "native_add", text_index, 0, sizeof(native_code));
  orion_add_global_data(obj, "my_data", data_index, 0, sizeof(data_value));
  
  // Print object info
  printf("\nNative x86_64 Object Info:\n");
  orion_print_object_info(obj);
  
  // Save object
  if (orion_obj_save(obj, "example_native.orion") != 0) {
    printf("Failed to save object: %s\n", orion_get_error_string(orion_get_last_error()));
  } else {
    printf("\nSaved native object to example_native.orion\n");
  }
  
  orion_obj_free(obj);
}

void load_and_inspect_example(void) {
  printf("\nLoading and inspecting saved object...\n");
  
  // Load the Orion++ object we saved earlier
  OrionObject* obj = orion_obj_load("example_orionpp.orion");
  if (!obj) {
    printf("Failed to load object: %s\n", orion_get_error_string(orion_get_last_error()));
    return;
  }
  
  printf("\nLoaded Object Info:\n");
  orion_print_object_info(obj);
  
  // Check what IR types are present
  printf("\nIR Analysis:\n");
  printf("  Has Orion++: %s\n", orion_has_orionpp(obj) ? "yes" : "no");
  printf("  Has Orion variant: %s\n", orion_has_orion_variant(obj) ? "yes" : "no");
  printf("  Has native: %s\n", orion_has_native(obj) ? "yes" : "no");
  printf("  Target: %s\n", orion_get_target_name(obj->header.target_arch));
  printf("  Variant: %s\n", orion_get_variant_name(obj->header.variant_type));
  
  // Get and display section content
  OrionSection* text_section = orion_obj_get_section(obj, ".orionpp.text");
  if (text_section) {
    printf("\nOrion++ Text Section Content:\n");
    printf("%.*s\n", (int)orion_section_get_size(text_section), 
           (char*)orion_section_get_data(text_section));
  }
  
  orion_obj_free(obj);
}

int main(void) {
  printf("Orion Object Format Example\n");
  printf("==========================\n");
  
  // Create examples of different object types
  create_orionpp_example();
  create_orion_variant_example();
  create_native_example();
  
  // Load and inspect one of the saved objects
  load_and_inspect_example();
  
  printf("\nExample completed successfully!\n");
  return 0;
}