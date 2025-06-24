#ifndef ORIONPP_VALUE_PARSER_H
#define ORIONPP_VALUE_PARSER_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/value/value.h"
#include "orionpp/string/strtab.h"

// Value parsing from text
orionpp_result_t orionpp_value_parse_from_text(orionpp_value_t* value,
                                                const char* text,
                                                orionpp_string_table_t* string_table,
                                                const orionpp_allocator_t* allocator);

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

// Array parsing utilities
orionpp_result_t orionpp_value_parse_array_from_text(orionpp_value_t* value,
                                                      const char* text,
                                                      orionpp_string_table_t* string_table,
                                                      const orionpp_allocator_t* allocator);

// Numeric parsing utilities
orionpp_result_t orionpp_value_parse_numeric_from_text(orionpp_value_t* value,
                                                        const char* text);

// Label parsing utilities
orionpp_result_t orionpp_value_parse_label_from_text(orionpp_value_t* value,
                                                      const char* text,
                                                      orionpp_string_table_t* string_table,
                                                      const orionpp_allocator_t* allocator);

// String literal parsing utilities
orionpp_result_t orionpp_value_parse_string_literal_from_text(orionpp_value_t* value,
                                                               const char* text,
                                                               orionpp_string_table_t* string_table,
                                                               const orionpp_allocator_t* allocator);

#endif // ORIONPP_VALUE_PARSER_H