#ifndef ORIONPP_VALUE_H
#define ORIONPP_VALUE_H

#include "orionpp/types.h"
#include "orionpp/strtab.h"

// Value creation functions
orionpp_value_t orionpp_value_variable(uint32_t variable_id);
orionpp_value_t orionpp_value_symbol(uint32_t name_offset, uint32_t name_length);
orionpp_value_t orionpp_value_numeric(orionpp_numeric_base_t base, uint64_t value);
orionpp_value_t orionpp_value_label(uint32_t name_offset, uint32_t name_length, int8_t direction);
orionpp_value_t orionpp_value_string(uint32_t offset, uint32_t length);

// Array value creation
orionpp_result_t orionpp_value_array_create(orionpp_value_t** value,
                                             orionpp_value_t* elements,
                                             uint32_t count,
                                             const orionpp_allocator_t* allocator);

void orionpp_value_array_destroy(orionpp_value_t* value, const orionpp_allocator_t* allocator);

// Value validation
bool orionpp_value_is_valid(const orionpp_value_t* value);
bool orionpp_value_is_variable(const orionpp_value_t* value);
bool orionpp_value_is_symbol(const orionpp_value_t* value);
bool orionpp_value_is_numeric(const orionpp_value_t* value);
bool orionpp_value_is_label(const orionpp_value_t* value);
bool orionpp_value_is_array(const orionpp_value_t* value);
bool orionpp_value_is_string(const orionpp_value_t* value);

// Value access helpers
uint32_t orionpp_value_get_variable_id(const orionpp_value_t* value);
uint64_t orionpp_value_get_numeric_value(const orionpp_value_t* value);
orionpp_numeric_base_t orionpp_value_get_numeric_base(const orionpp_value_t* value);

// String table integration
orionpp_result_t orionpp_value_symbol_from_string(orionpp_value_t* value,
                                                   const char* symbol_name,
                                                   orionpp_string_table_t* string_table);

orionpp_result_t orionpp_value_label_from_string(orionpp_value_t* value,
                                                  const char* label_name,
                                                  int8_t direction,
                                                  orionpp_string_table_t* string_table);

orionpp_result_t orionpp_value_string_from_string(orionpp_value_t* value,
                                                   const char* str,
                                                   orionpp_string_table_t* string_table);

// Value serialization
orionpp_result_t orionpp_value_write_binary(const orionpp_value_t* value, FILE* file);
orionpp_result_t orionpp_value_read_binary(orionpp_value_t* value, 
                                           FILE* file,
                                           const orionpp_allocator_t* allocator);

// Text representation
orionpp_result_t orionpp_value_write_text(const orionpp_value_t* value,
                                           const orionpp_string_table_t* string_table,
                                           FILE* file);

// Value parsing from text
orionpp_result_t orionpp_value_parse_from_text(orionpp_value_t* value,
                                                const char* text,
                                                orionpp_string_table_t* string_table,
                                                const orionpp_allocator_t* allocator);

// Value copying (deep copy for arrays)
orionpp_result_t orionpp_value_copy(orionpp_value_t* dest,
                                     const orionpp_value_t* src,
                                     const orionpp_allocator_t* allocator);

void orionpp_value_free(orionpp_value_t* value, const orionpp_allocator_t* allocator);

#endif // ORIONPP_VALUE_H