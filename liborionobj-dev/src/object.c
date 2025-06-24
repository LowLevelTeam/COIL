#include "orionobj/object.h"
#include "orionobj/binary.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_SECTION_CAPACITY 16
#define INITIAL_SYMBOL_CAPACITY 64

orionobj_result_t orionobj_object_create(orionobj_object_t** object,
                                          orionobj_format_t format,
                                          orionobj_arch_t architecture,
                                          const orionobj_allocator_t* allocator) {
  if (!object) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionobj_default_allocator;

  orionobj_object_t* obj = allocator->malloc(sizeof(orionobj_object_t));
  if (!obj) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);

  memset(obj, 0, sizeof(orionobj_object_t));
  obj->allocator = *allocator;

  // Initialize header
  obj->header.magic = (format == ORIONOBJ_FORMAT_ORIONPP) ? ORIONOBJ_MAGIC_ORIONPP : ORIONOBJ_MAGIC_ORION;
  obj->header.format = format;
  obj->header.architecture = architecture;
  obj->header.version_major = ORIONOBJ_VERSION_MAJOR;
  obj->header.version_minor = ORIONOBJ_VERSION_MINOR;
  obj->header.version_patch = ORIONOBJ_VERSION_PATCH;

  // Create string table
  orionobj_result_t result = orionobj_string_table_create(&obj->strings, allocator);
  if (ORIONOBJ_IS_ERROR(result)) {
    allocator->free(obj);
    return result;
  }

  // Initialize sections array
  obj->section_capacity = INITIAL_SECTION_CAPACITY;
  obj->sections = allocator->malloc(sizeof(orionobj_section_t*) * obj->section_capacity);
  if (!obj->sections) {
    orionobj_string_table_destroy(obj->strings);
    allocator->free(obj);
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }
  memset(obj->sections, 0, sizeof(orionobj_section_t*) * obj->section_capacity);

  // Initialize symbols array
  obj->symbol_capacity = INITIAL_SYMBOL_CAPACITY;
  obj->symbols = allocator->malloc(sizeof(orionobj_symbol_t*) * obj->symbol_capacity);
  if (!obj->symbols) {
    allocator->free(obj->sections);
    orionobj_string_table_destroy(obj->strings);
    allocator->free(obj);
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }
  memset(obj->symbols, 0, sizeof(orionobj_symbol_t*) * obj->symbol_capacity);

  *object = obj;
  return ORIONOBJ_OK_PTR(obj);
}

void orionobj_object_destroy(orionobj_object_t* object) {
  if (!object) return;

  // Destroy all sections
  for (uint32_t i = 0; i < object->section_count; i++) {
    if (object->sections[i]) {
      orionobj_section_destroy(object->sections[i]);
    }
  }
  object->allocator.free(object->sections);

  // Destroy all symbols
  for (uint32_t i = 0; i < object->symbol_count; i++) {
    if (object->symbols[i]) {
      orionobj_symbol_destroy(object->symbols[i]);
    }
  }
  object->allocator.free(object->symbols);

  // Destroy string table
  orionobj_string_table_destroy(object->strings);

  // Free object
  object->allocator.free(object);
}

orionobj_format_t orionobj_object_get_format(const orionobj_object_t* object) {
  return object ? object->header.format : ORIONOBJ_FORMAT_UNKNOWN;
}

orionobj_arch_t orionobj_object_get_architecture(const orionobj_object_t* object) {
  return object ? object->header.architecture : ORIONOBJ_ARCH_UNKNOWN;
}

orionobj_string_table_t* orionobj_object_get_string_table(orionobj_object_t* object) {
  return object ? object->strings : NULL;
}

static orionobj_result_t ensure_section_capacity(orionobj_object_t* object, uint32_t needed) {
  if (needed <= object->section_capacity) return ORIONOBJ_OK_INT(0);

  uint32_t new_capacity = object->section_capacity;
  while (new_capacity < needed) {
    new_capacity *= 2;
  }

  orionobj_section_t** new_sections = object->allocator.realloc(
    object->sections, sizeof(orionobj_section_t*) * new_capacity);
  if (!new_sections) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }

  // Initialize new slots
  for (uint32_t i = object->section_capacity; i < new_capacity; i++) {
    new_sections[i] = NULL;
  }

  object->sections = new_sections;
  object->section_capacity = new_capacity;
  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_object_add_section(orionobj_object_t* object,
                                               orionobj_section_type_t type,
                                               const char* name,
                                               orionobj_section_flags_t flags,
                                               orionobj_section_t** section) {
  if (!object || !name || !section) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  }

  orionobj_result_t result = ensure_section_capacity(object, object->section_count + 1);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  orionobj_section_t* sec;
  result = orionobj_section_create(&sec, type, name, flags, &object->allocator);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  // Add name to string table
  uint32_t name_offset;
  result = orionobj_string_table_add(object->strings, name, &name_offset);
  if (ORIONOBJ_IS_ERROR(result)) {
    orionobj_section_destroy(sec);
    return result;
  }

  sec->name_offset = name_offset;
  object->sections[object->section_count] = sec;
  object->section_count++;
  object->header.section_count = object->section_count;

  *section = sec;
  return ORIONOBJ_OK_PTR(sec);
}

orionobj_section_t* orionobj_object_get_section(const orionobj_object_t* object,
                                                 uint32_t index) {
  if (!object || index >= object->section_count) return NULL;
  return object->sections[index];
}

orionobj_section_t* orionobj_object_find_section(const orionobj_object_t* object,
                                                  const char* name) {
  if (!object || !name) return NULL;

  for (uint32_t i = 0; i < object->section_count; i++) {
    const char* section_name = orionobj_string_table_get(object->strings, 
                                                          object->sections[i]->name_offset);
    if (section_name && strcmp(section_name, name) == 0) {
      return object->sections[i];
    }
  }

  return NULL;
}

uint32_t orionobj_object_get_section_count(const orionobj_object_t* object) {
  return object ? object->section_count : 0;
}

static orionobj_result_t ensure_symbol_capacity(orionobj_object_t* object, uint32_t needed) {
  if (needed <= object->symbol_capacity) return ORIONOBJ_OK_INT(0);

  uint32_t new_capacity = object->symbol_capacity;
  while (new_capacity < needed) {
    new_capacity *= 2;
  }

  orionobj_symbol_t** new_symbols = object->allocator.realloc(
    object->symbols, sizeof(orionobj_symbol_t*) * new_capacity);
  if (!new_symbols) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }

  // Initialize new slots
  for (uint32_t i = object->symbol_capacity; i < new_capacity; i++) {
    new_symbols[i] = NULL;
  }

  object->symbols = new_symbols;
  object->symbol_capacity = new_capacity;
  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_object_add_symbol(orionobj_object_t* object,
                                              const char* name,
                                              orionobj_symbol_binding_t binding,
                                              orionobj_symbol_type_t type,
                                              uint32_t section_index,
                                              uint64_t value,
                                              uint64_t size,
                                              orionobj_symbol_t** symbol) {
  if (!object || !name || !symbol) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  }

  orionobj_result_t result = ensure_symbol_capacity(object, object->symbol_count + 1);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  orionobj_symbol_t* sym;
  result = orionobj_symbol_create(&sym, name, binding, type, section_index, 
                                  value, size, object->strings, &object->allocator);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  object->symbols[object->symbol_count] = sym;
  object->symbol_count++;
  object->header.symbol_count = object->symbol_count;

  *symbol = sym;
  return ORIONOBJ_OK_PTR(sym);
}

orionobj_symbol_t* orionobj_object_get_symbol(const orionobj_object_t* object,
                                               uint32_t index) {
  if (!object || index >= object->symbol_count) return NULL;
  return object->symbols[index];
}

orionobj_symbol_t* orionobj_object_find_symbol(const orionobj_object_t* object,
                                                const char* name) {
  if (!object || !name) return NULL;

  for (uint32_t i = 0; i < object->symbol_count; i++) {
    const char* symbol_name = orionobj_symbol_get_name(object->symbols[i], object->strings);
    if (symbol_name && strcmp(symbol_name, name) == 0) {
      return object->symbols[i];
    }
  }

  return NULL;
}

uint32_t orionobj_object_get_symbol_count(const orionobj_object_t* object) {
  return object ? object->symbol_count : 0;
}

orionobj_result_t orionobj_object_read_from_file(orionobj_object_t** object,
                                                  const char* filename,
                                                  const orionobj_allocator_t* allocator) {
  if (!object || !filename) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  FILE* file = fopen(filename, "rb");
  if (!file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);

  orionobj_result_t result = orionobj_object_read_from_stream(object, file, allocator);
  fclose(file);
  return result;
}

orionobj_result_t orionobj_object_write_to_file(const orionobj_object_t* object,
                                                 const char* filename) {
  if (!object || !filename) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  FILE* file = fopen(filename, "wb");
  if (!file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);

  orionobj_result_t result = orionobj_object_write_to_stream(object, file);
  fclose(file);
  return result;
}

orionobj_result_t orionobj_object_read_from_stream(orionobj_object_t** object,
                                                    FILE* stream,
                                                    const orionobj_allocator_t* allocator) {
  if (!object || !stream) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  
  return orionobj_binary_read_object(object, stream, allocator);
}

orionobj_result_t orionobj_object_write_to_stream(const orionobj_object_t* object,
                                                   FILE* stream) {
  if (!object || !stream) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  
  return orionobj_binary_write_object(object, stream);
}

orionobj_result_t orionobj_object_validate(const orionobj_object_t* object) {
  if (!object) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  // Validate header
  if (object->header.format > ORIONOBJ_FORMAT_ORION) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }

  if (object->header.architecture >= ORIONOBJ_ARCH_MAX) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_ARCH);
  }

  // Validate string table
  orionobj_result_t result = orionobj_string_table_validate(object->strings);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  // Validate sections
  for (uint32_t i = 0; i < object->section_count; i++) {
    result = orionobj_section_validate(object->sections[i]);
    if (ORIONOBJ_IS_ERROR(result)) return result;
  }

  // Validate symbols
  for (uint32_t i = 0; i < object->symbol_count; i++) {
    result = orionobj_symbol_validate(object->symbols[i]);
    if (ORIONOBJ_IS_ERROR(result)) return result;
  }

  return ORIONOBJ_OK_INT(0);
}