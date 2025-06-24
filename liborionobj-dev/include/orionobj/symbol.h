#ifndef ORIONOBJ_SYMBOL_H
#define ORIONOBJ_SYMBOL_H

#include "orionobj/types.h"
#include "orionobj/strtab.h"

// Symbol structure
struct orionobj_symbol {
  uint32_t name_offset;                     // Offset in string table
  orionobj_symbol_binding_t binding;        // Symbol binding
  orionobj_symbol_type_t type;              // Symbol type
  uint32_t section_index;                   // Section index (0 = undefined)
  uint64_t value;                           // Symbol value/address
  uint64_t size;                            // Symbol size
  
  // Memory management
  orionobj_allocator_t allocator;
};

// Symbol creation and destruction
orionobj_result_t orionobj_symbol_create(orionobj_symbol_t** symbol,
                                          const char* name,
                                          orionobj_symbol_binding_t binding,
                                          orionobj_symbol_type_t type,
                                          uint32_t section_index,
                                          uint64_t value,
                                          uint64_t size,
                                          orionobj_string_table_t* string_table,
                                          const orionobj_allocator_t* allocator);

void orionobj_symbol_destroy(orionobj_symbol_t* symbol);

// Symbol properties
const char* orionobj_symbol_get_name(const orionobj_symbol_t* symbol,
                                      const orionobj_string_table_t* string_table);

orionobj_symbol_binding_t orionobj_symbol_get_binding(const orionobj_symbol_t* symbol);
orionobj_symbol_type_t orionobj_symbol_get_type(const orionobj_symbol_t* symbol);
uint32_t orionobj_symbol_get_section_index(const orionobj_symbol_t* symbol);
uint64_t orionobj_symbol_get_value(const orionobj_symbol_t* symbol);
uint64_t orionobj_symbol_get_size(const orionobj_symbol_t* symbol);

orionobj_result_t orionobj_symbol_set_value(orionobj_symbol_t* symbol,
                                             uint64_t value);
orionobj_result_t orionobj_symbol_set_size(orionobj_symbol_t* symbol,
                                            uint64_t size);

// Symbol classification
bool orionobj_symbol_is_undefined(const orionobj_symbol_t* symbol);
bool orionobj_symbol_is_global(const orionobj_symbol_t* symbol);
bool orionobj_symbol_is_local(const orionobj_symbol_t* symbol);
bool orionobj_symbol_is_function(const orionobj_symbol_t* symbol);
bool orionobj_symbol_is_object(const orionobj_symbol_t* symbol);

// Symbol validation
orionobj_result_t orionobj_symbol_validate(const orionobj_symbol_t* symbol);

// Symbol utilities
const char* orionobj_symbol_binding_name(orionobj_symbol_binding_t binding);
const char* orionobj_symbol_type_name(orionobj_symbol_type_t type);

#endif // ORIONOBJ_SYMBOL_H