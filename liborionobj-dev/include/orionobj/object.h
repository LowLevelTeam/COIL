#ifndef ORIONOBJ_OBJECT_H
#define ORIONOBJ_OBJECT_H

#include "orionobj/types.h"
#include "orionobj/section.h"
#include "orionobj/symbol.h"
#include "orionobj/strtab.h"

// Main object structure
struct orionobj_object {
  orionobj_header_t header;           // Object header
  orionobj_string_table_t* strings;   // String table for all strings
  
  // Sections
  orionobj_section_t** sections;
  uint32_t section_count;
  uint32_t section_capacity;
  
  // Symbols
  orionobj_symbol_t** symbols;
  uint32_t symbol_count;
  uint32_t symbol_capacity;
  
  // Memory management
  orionobj_allocator_t allocator;
};

// Object lifecycle management
orionobj_result_t orionobj_object_create(orionobj_object_t** object,
                                          orionobj_format_t format,
                                          orionobj_arch_t architecture,
                                          const orionobj_allocator_t* allocator);

void orionobj_object_destroy(orionobj_object_t* object);

// Object properties
orionobj_format_t orionobj_object_get_format(const orionobj_object_t* object);
orionobj_arch_t orionobj_object_get_architecture(const orionobj_object_t* object);
orionobj_string_table_t* orionobj_object_get_string_table(orionobj_object_t* object);

// Section management
orionobj_result_t orionobj_object_add_section(orionobj_object_t* object,
                                               orionobj_section_type_t type,
                                               const char* name,
                                               orionobj_section_flags_t flags,
                                               orionobj_section_t** section);

orionobj_section_t* orionobj_object_get_section(const orionobj_object_t* object,
                                                 uint32_t index);

orionobj_section_t* orionobj_object_find_section(const orionobj_object_t* object,
                                                  const char* name);

uint32_t orionobj_object_get_section_count(const orionobj_object_t* object);

// Symbol management
orionobj_result_t orionobj_object_add_symbol(orionobj_object_t* object,
                                              const char* name,
                                              orionobj_symbol_binding_t binding,
                                              orionobj_symbol_type_t type,
                                              uint32_t section_index,
                                              uint64_t value,
                                              uint64_t size,
                                              orionobj_symbol_t** symbol);

orionobj_symbol_t* orionobj_object_get_symbol(const orionobj_object_t* object,
                                               uint32_t index);

orionobj_symbol_t* orionobj_object_find_symbol(const orionobj_object_t* object,
                                                const char* name);

uint32_t orionobj_object_get_symbol_count(const orionobj_object_t* object);

// Object I/O
orionobj_result_t orionobj_object_read_from_file(orionobj_object_t** object,
                                                  const char* filename,
                                                  const orionobj_allocator_t* allocator);

orionobj_result_t orionobj_object_write_to_file(const orionobj_object_t* object,
                                                 const char* filename);

orionobj_result_t orionobj_object_read_from_stream(orionobj_object_t** object,
                                                    FILE* stream,
                                                    const orionobj_allocator_t* allocator);

orionobj_result_t orionobj_object_write_to_stream(const orionobj_object_t* object,
                                                   FILE* stream);

// Object validation
orionobj_result_t orionobj_object_validate(const orionobj_object_t* object);

// Utility functions
const char* orionobj_format_name(orionobj_format_t format);
const char* orionobj_arch_name(orionobj_arch_t arch);
const char* orionobj_error_string(orionobj_error_t error);

#endif // ORIONOBJ_OBJECT_H