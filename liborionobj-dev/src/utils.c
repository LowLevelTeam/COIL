#include "../include/orionobj.h"
#include <stdio.h>
#include <string.h>

// Convenience functions for creating common object configurations

OrionObject* orion_create_orionpp_object(void) {
  OrionObject* obj = orion_obj_create(ORION_OBJ_REL, ORION_TARGET_NONE);
  if (!obj) return NULL;
  
  // Add standard Orion++ sections
  OrionSection* text = orion_obj_add_section(obj, ".orionpp.text", ORION_SEC_ORIONPP_TEXT);
  if (text) {
    orion_section_set_flags(text, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_EXEC);
    orion_section_set_alignment(text, 16);
  }
  
  OrionSection* data = orion_obj_add_section(obj, ".orionpp.data", ORION_SEC_ORIONPP_DATA);
  if (data) {
    orion_section_set_flags(data, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(data, 8);
  }
  
  OrionSection* bss = orion_obj_add_section(obj, ".orionpp.bss", ORION_SEC_ORIONPP_BSS);
  if (bss) {
    orion_section_set_flags(bss, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(bss, 8);
  }
  
  // Add symbol table
  OrionSection* symtab = orion_obj_add_section(obj, ".symtab", ORION_SEC_SYMTAB);
  if (symtab) {
    orion_section_set_alignment(symtab, 8);
  }
  
  // Add string table
  OrionSection* strtab = orion_obj_add_section(obj, ".strtab", ORION_SEC_STRTAB);
  if (strtab) {
    orion_section_set_alignment(strtab, 1);
  }
  
  return obj;
}

OrionObject* orion_create_orion_variant_object(OrionVariantType variant) {
  OrionObject* obj = orion_obj_create(ORION_OBJ_REL, ORION_TARGET_NONE);
  if (!obj) return NULL;
  
  obj->header.variant_type = variant;
  
  // Add standard Orion variant sections
  const char* prefix;
  switch (variant) {
    case ORION_VAR_X86: prefix = ".orion.x86"; break;
    case ORION_VAR_ARM: prefix = ".orion.arm"; break;
    case ORION_VAR_RISCV6: prefix = ".orion.riscv6"; break;
    default: prefix = ".orion"; break;
  }
  
  char section_name[64];
  
  snprintf(section_name, sizeof(section_name), "%s.text", prefix);
  OrionSection* text = orion_obj_add_section(obj, section_name, ORION_SEC_ORIONVAR_TEXT);
  if (text) {
    orion_section_set_flags(text, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_EXEC);
    orion_section_set_alignment(text, 16);
  }
  
  snprintf(section_name, sizeof(section_name), "%s.data", prefix);
  OrionSection* data = orion_obj_add_section(obj, section_name, ORION_SEC_ORIONVAR_DATA);
  if (data) {
    orion_section_set_flags(data, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(data, 8);
  }
  
  snprintf(section_name, sizeof(section_name), "%s.bss", prefix);
  OrionSection* bss = orion_obj_add_section(obj, section_name, ORION_SEC_ORIONVAR_BSS);
  if (bss) {
    orion_section_set_flags(bss, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(bss, 8);
  }
  
  // Add symbol table
  OrionSection* symtab = orion_obj_add_section(obj, ".symtab", ORION_SEC_SYMTAB);
  if (symtab) {
    orion_section_set_alignment(symtab, 8);
  }
  
  // Add string table
  OrionSection* strtab = orion_obj_add_section(obj, ".strtab", ORION_SEC_STRTAB);
  if (strtab) {
    orion_section_set_alignment(strtab, 1);
  }
  
  return obj;
}

OrionObject* orion_create_native_object(OrionTargetArch target) {
  OrionObject* obj = orion_obj_create(ORION_OBJ_REL, target);
  if (!obj) return NULL;
  
  // Add standard native sections
  OrionSection* text = orion_obj_add_section(obj, ".text", ORION_SEC_NATIVE_TEXT);
  if (text) {
    orion_section_set_flags(text, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_EXEC);
    orion_section_set_alignment(text, 16);
  }
  
  OrionSection* data = orion_obj_add_section(obj, ".data", ORION_SEC_NATIVE_DATA);
  if (data) {
    orion_section_set_flags(data, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(data, 8);
  }
  
  OrionSection* bss = orion_obj_add_section(obj, ".bss", ORION_SEC_NATIVE_BSS);
  if (bss) {
    orion_section_set_flags(bss, ORION_SEC_FLAG_ALLOC | ORION_SEC_FLAG_WRITE);
    orion_section_set_alignment(bss, 8);
  }
  
  // Add symbol table
  OrionSection* symtab = orion_obj_add_section(obj, ".symtab", ORION_SEC_SYMTAB);
  if (symtab) {
    orion_section_set_alignment(symtab, 8);
  }
  
  // Add string table
  OrionSection* strtab = orion_obj_add_section(obj, ".strtab", ORION_SEC_STRTAB);
  if (strtab) {
    orion_section_set_alignment(strtab, 1);
  }
  
  return obj;
}

int orion_add_function_symbol(OrionObject* obj, const char* name, 
                              OrionSymbolBinding binding, uint16_t section_index,
                              uint64_t addr, uint64_t size) {
  return orion_obj_add_symbol(obj, name, ORION_SYM_FUNC, binding, section_index, addr, size);
}

int orion_add_data_symbol(OrionObject* obj, const char* name,
                          OrionSymbolBinding binding, uint16_t section_index,
                          uint64_t addr, uint64_t size) {
  return orion_obj_add_symbol(obj, name, ORION_SYM_OBJECT, binding, section_index, addr, size);
}

int orion_add_global_function(OrionObject* obj, const char* name, uint16_t section_index,
                              uint64_t addr, uint64_t size) {
  return orion_add_function_symbol(obj, name, ORION_SYM_GLOBAL, section_index, addr, size);
}

int orion_add_local_function(OrionObject* obj, const char* name, uint16_t section_index,
                             uint64_t addr, uint64_t size) {
  return orion_add_function_symbol(obj, name, ORION_SYM_LOCAL, section_index, addr, size);
}

int orion_add_global_data(OrionObject* obj, const char* name, uint16_t section_index,
                          uint64_t addr, uint64_t size) {
  return orion_add_data_symbol(obj, name, ORION_SYM_GLOBAL, section_index, addr, size);
}

int orion_add_local_data(OrionObject* obj, const char* name, uint16_t section_index,
                         uint64_t addr, uint64_t size) {
  return orion_add_data_symbol(obj, name, ORION_SYM_LOCAL, section_index, addr, size);
}

// Helper to get section index by name
uint32_t orion_get_section_index(OrionObject* obj, const char* name) {
  if (!obj || !name) return 0;
  
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->name && strcmp(section->name, name) == 0) {
      return i;
    }
  }
  return 0; // Return null section index if not found
}

// Print object information for debugging
void orion_print_object_info(OrionObject* obj) {
  if (!obj) {
    printf("NULL object\n");
    return;
  }
  
  printf("Orion Object:\n");
  printf("  Type: %d\n", obj->header.obj_type);
  printf("  Target: %d\n", obj->header.target_arch);
  printf("  Variant: %d\n", obj->header.variant_type);
  printf("  Sections: %u\n", obj->header.section_count);
  printf("  Entry Point: 0x%lx\n", obj->header.entry_point);
  printf("  String Table Size: %zu\n", obj->string_table_size);
  
  printf("\nSections:\n");
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    printf("  [%2u] %-20s Type: %2u Size: %8zu Flags: 0x%lx\n",
           i, section->name ? section->name : "<NULL>",
           section->header.type, section->data_size, section->header.flags);
  }
  
  printf("\nSymbols: %zu\n", obj->symbol_count);
  for (size_t i = 0; i < obj->symbol_count; i++) {
    OrionSymbol* sym = &obj->symbols[i];
    const char* name = orion_obj_get_string(obj, sym->name_offset);
    printf("  [%2zu] %-20s Bind: %u Type: %u Section: %u Value: 0x%lx Size: %lu\n",
           i, name ? name : "<NULL>",
           ORION_SYM_BIND(sym->info), ORION_SYM_TYPE(sym->info),
           sym->section_index, sym->value, sym->size);
  }
}

// Helper to determine if object contains specific IR type
int orion_has_orionpp(OrionObject* obj) {
  if (!obj) return 0;
  
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->header.type >= ORION_SEC_ORIONPP_TEXT && 
        section->header.type <= ORION_SEC_ORIONPP_BSS) {
      return 1;
    }
  }
  return 0;
}

int orion_has_orion_variant(OrionObject* obj) {
  if (!obj) return 0;
  
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->header.type >= ORION_SEC_ORIONVAR_TEXT && 
        section->header.type <= ORION_SEC_ORIONVAR_BSS) {
      return 1;
    }
  }
  return 0;
}

int orion_has_native(OrionObject* obj) {
  if (!obj) return 0;
  
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->header.type >= ORION_SEC_NATIVE_TEXT && 
        section->header.type <= ORION_SEC_NATIVE_BSS) {
      return 1;
    }
  }
  return 0;
}

// Get target name string
const char* orion_get_target_name(OrionTargetArch target) {
  switch (target) {
    case ORION_TARGET_NONE: return "none";
    case ORION_TARGET_X86: return "x86";
    case ORION_TARGET_X86_64: return "x86_64";
    case ORION_TARGET_ARM: return "arm";
    case ORION_TARGET_ARM64: return "arm64";
    case ORION_TARGET_RISCV32: return "riscv32";
    case ORION_TARGET_RISCV64: return "riscv64";
    case ORION_TARGET_RISCV128: return "riscv128";
    default: return "unknown";
  }
}

// Get variant name string
const char* orion_get_variant_name(OrionVariantType variant) {
  switch (variant) {
    case ORION_VAR_NONE: return "none";
    case ORION_VAR_X86: return "x86";
    case ORION_VAR_ARM: return "arm";
    case ORION_VAR_RISCV6: return "riscv6";
    default: return "unknown";
  }
}

// Check if section type is a BSS section
int orion_is_bss_section(OrionSectionType type) {
  return (type == ORION_SEC_ORIONPP_BSS ||
          type == ORION_SEC_ORIONVAR_BSS ||
          type == ORION_SEC_NATIVE_BSS);
}