#include "orionpp/output/orionobj.h"
#include "orionobj/orionobj.h"
#include <string.h>

const orionpp_orionobj_options_t orionpp_default_orionobj_options = {
  .output_format = ORIONPP_FORMAT_ORIONPP,
  .target_arch = ORIONPP_ARCH_ORION_X86,
  .optimize_strings = true,
  .generate_debug_info = false,
  .entry_point = NULL
};

orionpp_result_t orionpp_section_type_to_orionobj(orionpp_section_type_t pp_type,
                                                   uint32_t* obj_type) {
  if (!obj_type) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (pp_type) {
    case ORIONPP_SECTION_TEXT:
      *obj_type = ORIONOBJ_SECTION_TEXT;
      break;
    case ORIONPP_SECTION_DATA:
      *obj_type = ORIONOBJ_SECTION_DATA;
      break;
    case ORIONPP_SECTION_BSS:
      *obj_type = ORIONOBJ_SECTION_BSS;
      break;
    case ORIONPP_SECTION_RODATA:
      *obj_type = ORIONOBJ_SECTION_RODATA;
      break;
    case ORIONPP_SECTION_SYMTAB:
      *obj_type = ORIONOBJ_SECTION_SYMTAB;
      break;
    case ORIONPP_SECTION_STRTAB:
      *obj_type = ORIONOBJ_SECTION_STRTAB;
      break;
    case ORIONPP_SECTION_RELTAB:
      *obj_type = ORIONOBJ_SECTION_RELTAB;
      break;
    case ORIONPP_SECTION_DEBUG:
      *obj_type = ORIONOBJ_SECTION_DEBUG;
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FORMAT);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_section_flags_to_orionobj(orionpp_section_flags_t pp_flags,
                                                    uint32_t* obj_flags) {
  if (!obj_flags) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  *obj_flags = 0;
  
  if (pp_flags & ORIONPP_SECTION_FLAG_WRITE) {
    *obj_flags |= ORIONOBJ_SECTION_FLAG_WRITE;
  }
  if (pp_flags & ORIONPP_SECTION_FLAG_ALLOC) {
    *obj_flags |= ORIONOBJ_SECTION_FLAG_ALLOC;
  }
  if (pp_flags & ORIONPP_SECTION_FLAG_EXEC) {
    *obj_flags |= ORIONOBJ_SECTION_FLAG_EXEC;
  }
  if (pp_flags & ORIONPP_SECTION_FLAG_MERGE) {
    *obj_flags |= ORIONOBJ_SECTION_FLAG_MERGE;
  }
  if (pp_flags & ORIONPP_SECTION_FLAG_TLS) {
    *obj_flags |= ORIONOBJ_SECTION_FLAG_TLS;
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_symbol_binding_to_orionobj(orionpp_symbol_binding_t pp_binding,
                                                     uint32_t* obj_binding) {
  if (!obj_binding) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (pp_binding) {
    case ORIONPP_SYMBOL_LOCAL:
      *obj_binding = ORIONOBJ_SYMBOL_LOCAL;
      break;
    case ORIONPP_SYMBOL_GLOBAL:
      *obj_binding = ORIONOBJ_SYMBOL_GLOBAL;
      break;
    case ORIONPP_SYMBOL_WEAK:
      *obj_binding = ORIONOBJ_SYMBOL_WEAK;
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FORMAT);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_symbol_type_to_orionobj(orionpp_symbol_type_t pp_type,
                                                  uint32_t* obj_type) {
  if (!obj_type) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (pp_type) {
    case ORIONPP_SYMBOL_NONE:
      *obj_type = ORIONOBJ_SYMBOL_NONE;
      break;
    case ORIONPP_SYMBOL_OBJECT:
      *obj_type = ORIONOBJ_SYMBOL_OBJECT;
      break;
    case ORIONPP_SYMBOL_FUNC:
      *obj_type = ORIONOBJ_SYMBOL_FUNC;
      break;
    case ORIONPP_SYMBOL_SECTION:
      *obj_type = ORIONOBJ_SYMBOL_SECTION;
      break;
    case ORIONPP_SYMBOL_FILE:
      *obj_type = ORIONOBJ_SYMBOL_FILE;
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FORMAT);
  }
  
  return ORIONPP_OK_INT(0);
}

// Convert OrionPP architecture to OrionObj architecture
static orionpp_result_t convert_architecture(orionpp_arch_t pp_arch, orionobj_arch_t* obj_arch) {
  if (!obj_arch) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (pp_arch) {
    case ORIONPP_ARCH_ORION_X86:
      *obj_arch = ORIONOBJ_ARCH_ORION_X86;
      break;
    case ORIONPP_ARCH_ORION_ARM:
      *obj_arch = ORIONOBJ_ARCH_ORION_ARM;
      break;
    case ORIONPP_ARCH_ORION_RISCV:
      *obj_arch = ORIONOBJ_ARCH_ORION_RISCV;
      break;
    case ORIONPP_ARCH_X86_32:
      *obj_arch = ORIONOBJ_ARCH_X86_32;
      break;
    case ORIONPP_ARCH_X86_64:
      *obj_arch = ORIONOBJ_ARCH_X86_64;
      break;
    case ORIONPP_ARCH_ARM_32:
      *obj_arch = ORIONOBJ_ARCH_ARM_32;
      break;
    case ORIONPP_ARCH_ARM_64:
      *obj_arch = ORIONOBJ_ARCH_ARM_64;
      break;
    case ORIONPP_ARCH_RISCV_32:
      *obj_arch = ORIONOBJ_ARCH_RISCV_32;
      break;
    case ORIONPP_ARCH_RISCV_64:
      *obj_arch = ORIONOBJ_ARCH_RISCV_64;
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_ARCH);
  }
  
  return ORIONPP_OK_INT(0);
}

// Convert OrionPP format to OrionObj format
static orionpp_result_t convert_format(orionpp_format_t pp_format, orionobj_format_t* obj_format) {
  if (!obj_format) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (pp_format) {
    case ORIONPP_FORMAT_ORIONPP:
      *obj_format = ORIONOBJ_FORMAT_ORIONPP;
      break;
    case ORIONPP_FORMAT_ORION:
      *obj_format = ORIONOBJ_FORMAT_ORION;
      break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FORMAT);
  }
  
  return ORIONPP_OK_INT(0);
}

// Generate a simple byte array from ISA instructions for demonstration
static orionpp_result_t generate_simple_code(const orionpp_module_t* module,
                                              uint8_t** code_data,
                                              uint32_t* code_size,
                                              const orionpp_allocator_t* allocator) {
  if (!module || !code_data || !code_size) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Simple approach: generate 4 bytes per ISA instruction
  uint32_t isa_count = 0;
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (inst && inst->feature == ORIONPP_FEATURE_ISA) {
      isa_count++;
    }
  }
  
  if (isa_count == 0) {
    *code_data = NULL;
    *code_size = 0;
    return ORIONPP_OK_INT(0);
  }
  
  *code_size = isa_count * 4; // 4 bytes per instruction
  *code_data = allocator->malloc(*code_size);
  if (!*code_data) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  uint32_t code_offset = 0;
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (inst && inst->feature == ORIONPP_FEATURE_ISA) {
      // Generate a simple 4-byte encoding: feature, opcode, 0, 0
      (*code_data)[code_offset++] = inst->feature;
      (*code_data)[code_offset++] = inst->opcode;
      (*code_data)[code_offset++] = 0;
      (*code_data)[code_offset++] = 0;
    }
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_generate_code_from_isa(const orionpp_module_t* module,
                                                 orionobj_section_t* text_section,
                                                 const orionpp_orionobj_options_t* options) {
  if (!module || !text_section) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!options) options = &orionpp_default_orionobj_options;
  
  uint8_t* code_data;
  uint32_t code_size;
  orionpp_result_t result = generate_simple_code(module, &code_data, &code_size, &orionpp_default_allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  if (code_size > 0) {
    orionobj_result_t obj_result = orionobj_section_set_data(text_section, code_data, code_size);
    orionpp_default_allocator.free(code_data);
    
    if (ORIONOBJ_IS_ERROR(obj_result)) {
      return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
    }
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_resolve_symbols_and_labels(const orionpp_module_t* module,
                                                     orionobj_object_t* object) {
  if (!module || !object) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Simple implementation: just add symbols as we find them
  // In a real implementation, this would resolve addresses and references
  uint64_t current_address = 0x1000; // Start at some base address
  
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (!inst) continue;
    
    if (inst->feature == ORIONPP_FEATURE_OBJ && inst->opcode == ORIONPP_OBJ_SYMBOL) {
      const char* symbol_name = orionpp_string_table_get(module->strings, inst->obj_symbol.name_offset);
      if (symbol_name) {
        orionobj_symbol_binding_t binding;
        orionobj_symbol_type_t type;
        
        orionpp_result_t binding_result = orionpp_symbol_binding_to_orionobj(inst->obj_symbol.binding, &binding);
        orionpp_result_t type_result = orionpp_symbol_type_to_orionobj(inst->obj_symbol.symbol_type, &type);
        
        if (ORIONPP_IS_OK(binding_result) && ORIONPP_IS_OK(type_result)) {
          orionobj_symbol_t* symbol;
          orionobj_result_t result = orionobj_object_add_symbol(object, symbol_name, binding, type,
                                                                1, current_address, 0, &symbol);
          if (ORIONOBJ_IS_OK(result)) {
            current_address += 64; // Assume 64 bytes per symbol for now
          }
        }
      }
    }
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_compile_to_orionobj(const orionpp_module_t* module,
                                                     orionobj_object_t** object,
                                                     const orionpp_orionobj_options_t* options,
                                                     const orionpp_allocator_t* allocator) {
  if (!module || !object) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!options) options = &orionpp_default_orionobj_options;
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Convert types
  orionobj_format_t obj_format;
  orionobj_arch_t obj_arch;
  
  orionpp_result_t format_result = convert_format(options->output_format, &obj_format);
  if (ORIONPP_IS_ERROR(format_result)) return format_result;
  
  orionpp_result_t arch_result = convert_architecture(options->target_arch, &obj_arch);
  if (ORIONPP_IS_ERROR(arch_result)) return arch_result;
  
  // Create OrionObj object
  orionobj_allocator_t obj_allocator = {
    .malloc = allocator->malloc,
    .realloc = allocator->realloc,
    .free = allocator->free
  };
  
  orionobj_object_t* obj;
  orionobj_result_t create_result = orionobj_object_create(&obj, obj_format, obj_arch, &obj_allocator);
  if (ORIONOBJ_IS_ERROR(create_result)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  // Process OrionPP instructions and build object
  orionobj_section_t* current_section = NULL;
  
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (!inst) continue;
    
    if (inst->feature == ORIONPP_FEATURE_OBJ) {
      switch (inst->opcode) {
        case ORIONPP_OBJ_SECTION: {
          const char* section_name = orionpp_string_table_get(module->strings, inst->obj_section.name_offset);
          if (section_name) {
            orionobj_section_type_t section_type;
            orionobj_section_flags_t section_flags;
            
            orionpp_result_t type_result = orionpp_section_type_to_orionobj(inst->obj_section.section_type, &section_type);
            orionpp_result_t flags_result = orionpp_section_flags_to_orionobj(inst->obj_section.section_flags, &section_flags);
            
            if (ORIONPP_IS_OK(type_result) && ORIONPP_IS_OK(flags_result)) {
              orionobj_result_t add_result = orionobj_object_add_section(obj, section_type, section_name, 
                                                                          section_flags, &current_section);
              if (ORIONOBJ_IS_ERROR(add_result)) {
                orionobj_object_destroy(obj);
                return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
              }
            }
          }
          break;
        }
        
        case ORIONPP_OBJ_BYTE:
        case ORIONPP_OBJ_WORD:
        case ORIONPP_OBJ_DWORD:
        case ORIONPP_OBJ_QWORD: {
          // Add data to current section
          if (current_section && orionpp_value_is_numeric(&inst->obj_data.value)) {
            uint64_t value = orionpp_value_get_numeric_value(&inst->obj_data.value);
            uint8_t data[8];
            uint32_t data_size = 0;
            
            switch (inst->opcode) {
              case ORIONPP_OBJ_BYTE:
                data[0] = (uint8_t)value;
                data_size = 1;
                break;
              case ORIONPP_OBJ_WORD:
                data[0] = (uint8_t)value;
                data[1] = (uint8_t)(value >> 8);
                data_size = 2;
                break;
              case ORIONPP_OBJ_DWORD:
                data[0] = (uint8_t)value;
                data[1] = (uint8_t)(value >> 8);
                data[2] = (uint8_t)(value >> 16);
                data[3] = (uint8_t)(value >> 24);
                data_size = 4;
                break;
              case ORIONPP_OBJ_QWORD:
                data[0] = (uint8_t)value;
                data[1] = (uint8_t)(value >> 8);
                data[2] = (uint8_t)(value >> 16);
                data[3] = (uint8_t)(value >> 24);
                data[4] = (uint8_t)(value >> 32);
                data[5] = (uint8_t)(value >> 40);
                data[6] = (uint8_t)(value >> 48);
                data[7] = (uint8_t)(value >> 56);
                data_size = 8;
                break;
            }
            
            orionobj_section_append_data(current_section, data, data_size);
          }
          break;
        }
      }
    }
  }
  
  // Generate code from ISA instructions if we have a text section
  orionobj_section_t* text_section = orionobj_object_find_section(obj, ".text");
  if (text_section) {
    orionpp_result_t code_result = orionpp_generate_code_from_isa(module, text_section, options);
    if (ORIONPP_IS_ERROR(code_result)) {
      orionobj_object_destroy(obj);
      return code_result;
    }
  }
  
  // Resolve symbols and labels
  orionpp_result_t symbol_result = orionpp_resolve_symbols_and_labels(module, obj);
  if (ORIONPP_IS_ERROR(symbol_result)) {
    orionobj_object_destroy(obj);
    return symbol_result;
  }
  
  // Set entry point if specified
  if (options->entry_point) {
    orionobj_symbol_t* entry_symbol = orionobj_object_find_symbol(obj, options->entry_point);
    if (entry_symbol) {
      // Set entry point in object (this would need to be added to OrionObj API)
      // For now, just note that we found the entry point
    }
  }
  
  *object = obj;
  return ORIONPP_OK_PTR(obj);
}

orionpp_result_t orionpp_module_write_orionobj(const orionpp_module_t* module,
                                                const char* filename,
                                                const orionpp_orionobj_options_t* options) {
  if (!module || !filename) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!options) options = &orionpp_default_orionobj_options;
  
  orionobj_object_t* object;
  orionpp_result_t compile_result = orionpp_module_compile_to_orionobj(module, &object, options, &orionpp_default_allocator);
  if (ORIONPP_IS_ERROR(compile_result)) return compile_result;
  
  orionobj_result_t write_result = orionobj_object_write_to_file(object, filename);
  orionobj_object_destroy(object);
  
  if (ORIONOBJ_IS_ERROR(write_result)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_write_orionobj_stream(const orionpp_module_t* module,
                                                       FILE* file,
                                                       const orionpp_orionobj_options_t* options) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!options) options = &orionpp_default_orionobj_options;
  
  orionobj_object_t* object;
  orionpp_result_t compile_result = orionpp_module_compile_to_orionobj(module, &object, options, &orionpp_default_allocator);
  if (ORIONPP_IS_ERROR(compile_result)) return compile_result;
  
  orionobj_result_t write_result = orionobj_object_write_to_stream(object, file);
  orionobj_object_destroy(object);
  
  if (ORIONOBJ_IS_ERROR(write_result)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  return ORIONPP_OK_INT(0);
}