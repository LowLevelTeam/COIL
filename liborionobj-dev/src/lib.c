#include "../include/orionobj.h"
#include "../include/orionobj_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Global error state
static OrionError g_last_error = ORION_OK;

// Helper functions
static void set_error(OrionError error) {
  g_last_error = error;
}

static uint32_t add_string_to_table(char** table, size_t* size, size_t* capacity, const char* str) {
  if (!str) return 0;
  
  // Initialize table with null byte at offset 0 if empty
  if (*size == 0) {
    if (*capacity == 0) {
      *capacity = 256;
      *table = malloc(*capacity);
      if (!*table) {
        set_error(ORION_ERR_OUT_OF_MEMORY);
        return 0;
      }
    }
    (*table)[0] = '\0';
    *size = 1;
  }
  
  size_t len = strlen(str) + 1;
  uint32_t offset = *size;
  
  // Check if we need to expand the table
  if (*size + len > *capacity) {
    size_t new_capacity = (*capacity == 0) ? 256 : *capacity * 2;
    while (new_capacity < *size + len) {
      new_capacity *= 2;
    }
    
    char* new_table = realloc(*table, new_capacity);
    if (!new_table) {
      set_error(ORION_ERR_OUT_OF_MEMORY);
      return 0;
    }
    *table = new_table;
    *capacity = new_capacity;
  }
  
  memcpy(*table + *size, str, len);
  *size += len;
  return offset;
}

static const char* get_string_from_table(const char* table, size_t table_size, uint32_t offset) {
  if (offset >= table_size) return NULL;
  return table + offset;
}

// Object manipulation functions
OrionObject* orion_obj_create(OrionObjectType type, OrionTargetArch target) {
  OrionObject* obj = calloc(1, sizeof(OrionObject));
  if (!obj) {
    set_error(ORION_ERR_OUT_OF_MEMORY);
    return NULL;
  }
  
  // Initialize header
  obj->header.magic = ORION_MAGIC;
  obj->header.version = ORION_VERSION_CURRENT;
  obj->header.obj_type = type;
  obj->header.target_arch = target;
  obj->header.variant_type = ORION_VAR_NONE;
  obj->header.flags = 0;
  obj->header.entry_point = 0;
  obj->header.section_header_size = sizeof(OrionSectionHeader);
  
  // Add null section (index 0 is always null)
  OrionSection* null_section = orion_obj_add_section(obj, "", ORION_SEC_NULL);
  if (!null_section) {
    free(obj);
    return NULL;
  }
  
  // Add section name string table
  OrionSection* section_names = orion_obj_add_section(obj, ".shstrtab", ORION_SEC_STRTAB);
  if (!section_names) {
    orion_obj_free(obj);
    return NULL;
  }
  obj->section_name_table_index = 1; // Index of section name table
  obj->header.string_table_index = 1;
  
  // Initialize string table with null byte at offset 0
  if (!obj->string_table) {
    obj->string_table = malloc(256);
    if (obj->string_table) {
      obj->string_table[0] = '\0';
      obj->string_table_size = 1;
      obj->string_table_capacity = 256;
    }
  }
  
  set_error(ORION_OK);
  return obj;
}

OrionObject* orion_obj_load(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    set_error(ORION_ERR_FILE_NOT_FOUND);
    return NULL;
  }
  
  OrionObjectHeader header;
  if (fread(&header, sizeof(header), 1, file) != 1) {
    fclose(file);
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  if (header.magic != ORION_MAGIC) {
    fclose(file);
    set_error(ORION_ERR_INVALID_MAGIC);
    return NULL;
  }
  
  if (header.version != ORION_VERSION_CURRENT) {
    fclose(file);
    set_error(ORION_ERR_INVALID_VERSION);
    return NULL;
  }
  
  OrionObject* obj = calloc(1, sizeof(OrionObject));
  if (!obj) {
    fclose(file);
    set_error(ORION_ERR_OUT_OF_MEMORY);
    return NULL;
  }
  
  obj->header = header;
  
  // Allocate sections
  obj->section_capacity = header.section_count;
  obj->sections = calloc(header.section_count, sizeof(OrionSection*));
  if (!obj->sections) {
    free(obj);
    fclose(file);
    set_error(ORION_ERR_OUT_OF_MEMORY);
    return NULL;
  }
  
  // Read section headers
  fseek(file, header.section_header_offset, SEEK_SET);
  for (uint32_t i = 0; i < header.section_count; i++) {
    OrionSection* section = calloc(1, sizeof(OrionSection));
    if (!section) {
      orion_obj_free(obj);
      fclose(file);
      set_error(ORION_ERR_OUT_OF_MEMORY);
      return NULL;
    }
    
    if (fread(&section->header, sizeof(OrionSectionHeader), 1, file) != 1) {
      free(section);
      orion_obj_free(obj);
      fclose(file);
      set_error(ORION_ERR_INVALID_FORMAT);
      return NULL;
    }
    
    obj->sections[i] = section;
    obj->section_count++;
    
    // Read section data if present (BSS sections don't have file data)
    if (section->header.size > 0 && !orion_is_bss_section(section->header.type)) {
      long current_pos = ftell(file);
      fseek(file, section->header.offset, SEEK_SET);
      
      section->data = malloc(section->header.size);
      if (!section->data) {
        orion_obj_free(obj);
        fclose(file);
        set_error(ORION_ERR_OUT_OF_MEMORY);
        return NULL;
      }
      
      if (fread(section->data, section->header.size, 1, file) != 1) {
        orion_obj_free(obj);
        fclose(file);
        set_error(ORION_ERR_INVALID_FORMAT);
        return NULL;
      }
      
      section->data_size = section->header.size;
      section->data_capacity = section->header.size;
      
      fseek(file, current_pos, SEEK_SET);
    }
  }
  
  // Load string table and section names
  if (header.string_table_index < header.section_count) {
    OrionSection* strtab = obj->sections[header.string_table_index];
    if (strtab && strtab->data) {
      obj->string_table = malloc(strtab->data_size);
      if (obj->string_table) {
        memcpy(obj->string_table, strtab->data, strtab->data_size);
        obj->string_table_size = strtab->data_size;
        obj->string_table_capacity = strtab->data_size;
        
        // Set section names
        for (uint32_t i = 0; i < obj->section_count; i++) {
          OrionSection* section = obj->sections[i];
          const char* name = get_string_from_table(obj->string_table, obj->string_table_size, 
                                                   section->header.name_offset);
          if (name) {
            section->name = strdup(name);
          }
        }
      }
    }
  }
  
  // Load symbol table
  for (uint32_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->header.type == ORION_SEC_SYMTAB && section->data && section->data_size > 0) {
      size_t symbol_count = section->data_size / sizeof(OrionSymbol);
      obj->symbols = malloc(section->data_size);
      if (obj->symbols) {
        memcpy(obj->symbols, section->data, section->data_size);
        obj->symbol_count = symbol_count;
        obj->symbol_capacity = symbol_count;
      }
      break;
    }
  }
  
  fclose(file);
  set_error(ORION_OK);
  return obj;
}

int orion_obj_save(OrionObject* obj, const char* filename) {
  if (!obj || !filename) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return -1;
  }
  
  // Update string tables in their sections before saving
  OrionSection* shstrtab = orion_obj_get_section(obj, ".shstrtab");
  if (shstrtab && obj->string_table_size > 0) {
    orion_section_set_data(shstrtab, obj->string_table, obj->string_table_size);
  }
  
  OrionSection* strtab = orion_obj_get_section(obj, ".strtab");
  if (strtab && obj->string_table_size > 0) {
    orion_section_set_data(strtab, obj->string_table, obj->string_table_size);
  }
  
  // Update symbol table in its section before saving
  OrionSection* symtab = orion_obj_get_section(obj, ".symtab");
  if (symtab && obj->symbol_count > 0) {
    orion_section_set_data(symtab, obj->symbols, obj->symbol_count * sizeof(OrionSymbol));
  }
  
  FILE* file = fopen(filename, "wb");
  if (!file) {
    set_error(ORION_ERR_PERMISSION_DENIED);
    return -1;
  }
  
  // Update header
  obj->header.section_count = obj->section_count;
  obj->header.section_header_offset = sizeof(OrionObjectHeader);
  
  // Write header
  if (fwrite(&obj->header, sizeof(OrionObjectHeader), 1, file) != 1) {
    fclose(file);
    set_error(ORION_ERR_INVALID_FORMAT);
    return -1;
  }
  
  // Calculate section data offsets
  uint64_t data_offset = sizeof(OrionObjectHeader) + 
                         (obj->section_count * sizeof(OrionSectionHeader));
  
  for (size_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->data_size > 0 && !orion_is_bss_section(section->header.type)) {
      section->header.offset = data_offset;
      section->header.size = section->data_size;
      data_offset += section->data_size;
      
      // Align to 8 bytes
      data_offset = (data_offset + 7) & ~7;
    } else {
      section->header.offset = 0;
      if (orion_is_bss_section(section->header.type)) {
        section->header.size = section->data_size;
      } else {
        section->header.size = 0;
      }
    }
  }
  
  // Write section headers
  for (size_t i = 0; i < obj->section_count; i++) {
    if (fwrite(&obj->sections[i]->header, sizeof(OrionSectionHeader), 1, file) != 1) {
      fclose(file);
      set_error(ORION_ERR_INVALID_FORMAT);
      return -1;
    }
  }
  
  // Write section data
  for (size_t i = 0; i < obj->section_count; i++) {
    OrionSection* section = obj->sections[i];
    if (section->data_size > 0 && !orion_is_bss_section(section->header.type)) {
      // Seek to correct position
      fseek(file, section->header.offset, SEEK_SET);
      
      if (fwrite(section->data, section->data_size, 1, file) != 1) {
        fclose(file);
        set_error(ORION_ERR_INVALID_FORMAT);
        return -1;
      }
    }
  }
  
  fclose(file);
  set_error(ORION_OK);
  return 0;
}

void orion_obj_free(OrionObject* obj) {
  if (!obj) return;
  
  for (size_t i = 0; i < obj->section_count; i++) {
    if (obj->sections[i]) {
      free(obj->sections[i]->name);
      free(obj->sections[i]->data);
      free(obj->sections[i]);
    }
  }
  free(obj->sections);
  free(obj->symbols);
  free(obj->string_table);
  free(obj);
}

// Section manipulation functions
OrionSection* orion_obj_add_section(OrionObject* obj, const char* name, OrionSectionType type) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  // Expand sections array if needed
  if (obj->section_count >= obj->section_capacity) {
    size_t new_capacity = (obj->section_capacity == 0) ? 8 : obj->section_capacity * 2;
    OrionSection** new_sections = realloc(obj->sections, new_capacity * sizeof(OrionSection*));
    if (!new_sections) {
      set_error(ORION_ERR_OUT_OF_MEMORY);
      return NULL;
    }
    obj->sections = new_sections;
    obj->section_capacity = new_capacity;
  }
  
  OrionSection* section = calloc(1, sizeof(OrionSection));
  if (!section) {
    set_error(ORION_ERR_OUT_OF_MEMORY);
    return NULL;
  }
  
  section->name = strdup(name ? name : "");
  if (!section->name) {
    free(section);
    set_error(ORION_ERR_OUT_OF_MEMORY);
    return NULL;
  }
  
  // Add name to string table
  uint32_t name_offset = add_string_to_table(&obj->string_table, &obj->string_table_size,
                                             &obj->string_table_capacity, name);
  
  section->header.name_offset = name_offset;
  section->header.type = type;
  section->header.flags = 0;
  section->header.addr = 0;
  section->header.offset = 0;
  section->header.size = 0;
  section->header.link = 0;
  section->header.info = 0;
  section->header.alignment = 1;
  section->header.entry_size = 0;
  
  obj->sections[obj->section_count] = section;
  obj->section_count++;
  
  // Update header section count
  obj->header.section_count = obj->section_count;
  
  set_error(ORION_OK);
  return section;
}

OrionSection* orion_obj_get_section(OrionObject* obj, const char* name) {
  if (!obj || !name) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  for (size_t i = 0; i < obj->section_count; i++) {
    if (obj->sections[i]->name && strcmp(obj->sections[i]->name, name) == 0) {
      set_error(ORION_OK);
      return obj->sections[i];
    }
  }
  
  set_error(ORION_ERR_INVALID_SECTION);
  return NULL;
}

OrionSection* orion_obj_get_section_by_index(OrionObject* obj, uint32_t index) {
  if (!obj || index >= obj->section_count) {
    set_error(ORION_ERR_INVALID_SECTION);
    return NULL;
  }
  
  set_error(ORION_OK);
  return obj->sections[index];
}

uint32_t orion_obj_get_section_count(OrionObject* obj) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return 0;
  }
  
  set_error(ORION_OK);
  return obj->section_count;
}

// Section data functions
int orion_section_set_data(OrionSection* section, const void* data, size_t size) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return -1;
  }
  
  if (size > section->data_capacity) {
    void* new_data = realloc(section->data, size);
    if (!new_data) {
      set_error(ORION_ERR_OUT_OF_MEMORY);
      return -1;
    }
    section->data = new_data;
    section->data_capacity = size;
  }
  
  if (data && size > 0) {
    memcpy(section->data, data, size);
  }
  section->data_size = size;
  
  set_error(ORION_OK);
  return 0;
}

void* orion_section_get_data(OrionSection* section) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return NULL;
  }
  
  set_error(ORION_OK);
  return section->data;
}

size_t orion_section_get_size(OrionSection* section) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return 0;
  }
  
  set_error(ORION_OK);
  return section->data_size;
}

void orion_section_set_flags(OrionSection* section, uint64_t flags) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return;
  }
  
  section->header.flags = flags;
  set_error(ORION_OK);
}

uint64_t orion_section_get_flags(OrionSection* section) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return 0;
  }
  
  set_error(ORION_OK);
  return section->header.flags;
}

void orion_section_set_alignment(OrionSection* section, uint64_t alignment) {
  if (!section) {
    set_error(ORION_ERR_INVALID_SECTION);
    return;
  }
  
  section->header.alignment = alignment;
  set_error(ORION_OK);
}

// Symbol table functions
int orion_obj_add_symbol(OrionObject* obj, const char* name, OrionSymbolType type,
                         OrionSymbolBinding binding, uint16_t section_index,
                         uint64_t value, uint64_t size) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return -1;
  }
  
  // Expand symbols array if needed
  if (obj->symbol_count >= obj->symbol_capacity) {
    size_t new_capacity = (obj->symbol_capacity == 0) ? 16 : obj->symbol_capacity * 2;
    OrionSymbol* new_symbols = realloc(obj->symbols, new_capacity * sizeof(OrionSymbol));
    if (!new_symbols) {
      set_error(ORION_ERR_OUT_OF_MEMORY);
      return -1;
    }
    obj->symbols = new_symbols;
    obj->symbol_capacity = new_capacity;
  }
  
  uint32_t name_offset = add_string_to_table(&obj->string_table, &obj->string_table_size,
                                             &obj->string_table_capacity, name);
  
  OrionSymbol* symbol = &obj->symbols[obj->symbol_count];
  symbol->name_offset = name_offset;
  symbol->info = ORION_SYM_INFO(binding, type);
  symbol->other = 0;
  symbol->section_index = section_index;
  symbol->value = value;
  symbol->size = size;
  
  obj->symbol_count++;
  
  set_error(ORION_OK);
  return 0;
}

OrionSymbol* orion_obj_get_symbol(OrionObject* obj, const char* name) {
  if (!obj || !name) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  for (size_t i = 0; i < obj->symbol_count; i++) {
    const char* sym_name = get_string_from_table(obj->string_table, obj->string_table_size,
                                                 obj->symbols[i].name_offset);
    if (sym_name && strcmp(sym_name, name) == 0) {
      set_error(ORION_OK);
      return &obj->symbols[i];
    }
  }
  
  set_error(ORION_ERR_INVALID_SYMBOL);
  return NULL;
}

OrionSymbol* orion_obj_get_symbol_by_index(OrionObject* obj, uint32_t index) {
  if (!obj || index >= obj->symbol_count) {
    set_error(ORION_ERR_INVALID_SYMBOL);
    return NULL;
  }
  
  set_error(ORION_OK);
  return &obj->symbols[index];
}

uint32_t orion_obj_get_symbol_count(OrionObject* obj) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return 0;
  }
  
  set_error(ORION_OK);
  return obj->symbol_count;
}

// String table functions
uint32_t orion_obj_add_string(OrionObject* obj, const char* str) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return 0;
  }
  
  uint32_t offset = add_string_to_table(&obj->string_table, &obj->string_table_size,
                                        &obj->string_table_capacity, str);
  
  set_error(ORION_OK);
  return offset;
}

const char* orion_obj_get_string(OrionObject* obj, uint32_t offset) {
  if (!obj) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  const char* str = get_string_from_table(obj->string_table, obj->string_table_size, offset);
  if (!str) {
    set_error(ORION_ERR_INVALID_FORMAT);
    return NULL;
  }
  
  set_error(ORION_OK);
  return str;
}

// Error handling
OrionError orion_get_last_error(void) {
  return g_last_error;
}

const char* orion_get_error_string(OrionError error) {
  switch (error) {
    case ORION_OK: return "Success";
    case ORION_ERR_INVALID_MAGIC: return "Invalid magic number";
    case ORION_ERR_INVALID_VERSION: return "Invalid version";
    case ORION_ERR_INVALID_FORMAT: return "Invalid format";
    case ORION_ERR_FILE_NOT_FOUND: return "File not found";
    case ORION_ERR_PERMISSION_DENIED: return "Permission denied";
    case ORION_ERR_OUT_OF_MEMORY: return "Out of memory";
    case ORION_ERR_INVALID_SECTION: return "Invalid section";
    case ORION_ERR_INVALID_SYMBOL: return "Invalid symbol";
    case ORION_ERR_BUFFER_TOO_SMALL: return "Buffer too small";
    default: return "Unknown error";
  }
}