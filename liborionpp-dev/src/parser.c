#include "orionpp/value/parser.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

orionpp_result_t orionpp_value_symbol_from_string(orionpp_value_t* value,
                                                   const char* symbol_name,
                                                   orionpp_string_table_t* string_table) {
  if (!value || !symbol_name || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  
  uint32_t offset;
  orionpp_result_t result = orionpp_string_table_add(string_table, symbol_name, &offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  *value = orionpp_value_symbol(offset, strlen(symbol_name));
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_value_label_from_string(orionpp_value_t* value,
                                                  const char* label_name,
                                                  int8_t direction,
                                                  orionpp_string_table_t* string_table) {
  if (!value || !label_name || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  
  uint32_t offset;
  orionpp_result_t result = orionpp_string_table_add(string_table, label_name, &offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  *value = orionpp_value_label(offset, strlen(label_name), direction);
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_value_string_from_string(orionpp_value_t* value,
                                                   const char* str,
                                                   orionpp_string_table_t* string_table) {
  if (!value || !str || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  
  uint32_t offset;
  orionpp_result_t result = orionpp_string_table_add(string_table, str, &offset);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  *value = orionpp_value_string(offset, strlen(str));
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_value_parse_numeric_from_text(orionpp_value_t* value,
                                                        const char* text) {
  if (!value || !text || text[0] != '%') {
    return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  
  char base_char = text[1];
  const char* number_str = text + 2;
  
  orionpp_numeric_base_t base;
  int parse_base;
  
  switch (base_char) {
    case 'b': base = ORIONPP_BASE_BINARY;  parse_base = 2;  break;
    case 'o': base = ORIONPP_BASE_OCTAL;   parse_base = 8;  break;
    case 'd': base = ORIONPP_BASE_DECIMAL; parse_base = 10; break;
    case 'x': base = ORIONPP_BASE_HEX;     parse_base = 16; break;
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  
  char* endptr;
  uint64_t numeric_value = strtoull(number_str, &endptr, parse_base);
  
  if (endptr == number_str || *endptr != '\0') {
    return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  
  *value = orionpp_value_numeric(base, numeric_value);
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_value_parse_label_from_text(orionpp_value_t* value,
                                                      const char* text,
                                                      orionpp_string_table_t* string_table,
                                                      const orionpp_allocator_t* allocator) {
  if (!value || !text || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  int8_t direction = 0;
  const char* label_start = text;
  
  if (*text == '+') {
    direction = 1;
    label_start++;
  } else if (*text == '-') {
    direction = -1;
    label_start++;
  }
  
  if (*label_start != '.') return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  label_start++; // Skip the '.'
  
  // Find end of label (whitespace or special characters)
  const char* label_end = label_start;
  while (*label_end && !isspace(*label_end) && *label_end != ',' && *label_end != ']' && *label_end != ')') {
    label_end++;
  }
  
  size_t len = label_end - label_start;
  if (len == 0) return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  
  char* label_name = allocator->malloc(len + 1);
  if (!label_name) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  memcpy(label_name, label_start, len);
  label_name[len] = '\0';
  
  orionpp_result_t result = orionpp_value_label_from_string(value, label_name, direction, string_table);
  allocator->free(label_name);
  return result;
}

orionpp_result_t orionpp_value_parse_string_literal_from_text(orionpp_value_t* value,
                                                               const char* text,
                                                               orionpp_string_table_t* string_table,
                                                               const orionpp_allocator_t* allocator) {
  if (!value || !text || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  if (*text != '"') return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  
  const char* start = text + 1;
  const char* end = strchr(start, '"');
  if (!end) return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  
  size_t len = end - start;
  char* str_content = allocator->malloc(len + 1);
  if (!str_content) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  memcpy(str_content, start, len);
  str_content[len] = '\0';
  
  orionpp_result_t result = orionpp_value_string_from_string(value, str_content, string_table);
  allocator->free(str_content);
  return result;
}

orionpp_result_t orionpp_value_parse_array_from_text(orionpp_value_t* value,
                                                      const char* text,
                                                      orionpp_string_table_t* string_table,
                                                      const orionpp_allocator_t* allocator) {
  if (!value || !text || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Array parsing is complex - for now, just return an error
  // This would need a full tokenizer/parser to implement properly
  (void)text; // Suppress unused parameter warning
  return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
}

orionpp_result_t orionpp_value_parse_from_text(orionpp_value_t* value,
                                                const char* text,
                                                orionpp_string_table_t* string_table,
                                                const orionpp_allocator_t* allocator) {
  if (!value || !text || !string_table) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Skip whitespace
  while (isspace(*text)) text++;
  
  if (*text == '$') {
    // Variable: $123
    char* endptr;
    uint32_t var_id = strtoul(text + 1, &endptr, 10);
    if (endptr == text + 1) return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
    *value = orionpp_value_variable(var_id);
    return ORIONPP_OK_INT(0);
  }
  
  if (*text == '@') {
    // Symbol: @symbol@
    const char* start = text + 1;
    const char* end = strstr(start, "@");
    if (!end) return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
    
    size_t len = end - start;
    char* symbol_name = allocator->malloc(len + 1);
    if (!symbol_name) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
    
    memcpy(symbol_name, start, len);
    symbol_name[len] = '\0';
    
    orionpp_result_t result = orionpp_value_symbol_from_string(value, symbol_name, string_table);
    allocator->free(symbol_name);
    return result;
  }
  
  if (*text == '%') {
    // Numeric: %x123, %d456, etc.
    return orionpp_value_parse_numeric_from_text(value, text);
  }
  
  if (*text == '.' || *text == '+' || *text == '-') {
    // Label: .label, +.label, -.label
    return orionpp_value_parse_label_from_text(value, text, string_table, allocator);
  }
  
  if (*text == '"') {
    // String literal: "string"
    return orionpp_value_parse_string_literal_from_text(value, text, string_table, allocator);
  }
  
  if (*text == '[') {
    // Array: [val1, val2, ...]
    return orionpp_value_parse_array_from_text(value, text, string_table, allocator);
  }
  
  return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
}