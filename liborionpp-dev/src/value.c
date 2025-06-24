#include "orionpp/value.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

orionpp_value_t orionpp_value_variable(uint32_t variable_id) {
  orionpp_value_t value = {0};
  value.type = ORIONPP_VALUE_VARIABLE;
  value.variable_id = variable_id;
  return value;
}

orionpp_value_t orionpp_value_symbol(uint32_t name_offset, uint32_t name_length) {
  orionpp_value_t value = {0};
  value.type = ORIONPP_VALUE_SYMBOL;
  value.symbol.name_offset = name_offset;
  value.symbol.name_length = name_length;
  return value;
}

orionpp_value_t orionpp_value_numeric(orionpp_numeric_base_t base, uint64_t numeric_value) {
  orionpp_value_t value = {0};
  value.type = ORIONPP_VALUE_NUMERIC;
  value.numeric.base = base;
  value.numeric.value = numeric_value;
  return value;
}

orionpp_value_t orionpp_value_label(uint32_t name_offset, uint32_t name_length, int8_t direction) {
  orionpp_value_t value = {0};
  value.type = ORIONPP_VALUE_LABEL;
  value.label.name_offset = name_offset;
  value.label.name_length = name_length;
  value.label.direction = direction;
  return value;
}

orionpp_value_t orionpp_value_string(uint32_t offset, uint32_t length) {
  orionpp_value_t value = {0};
  value.type = ORIONPP_VALUE_STRING;
  value.string.offset = offset;
  value.string.length = length;
  return value;
}

orionpp_result_t orionpp_value_array_create(orionpp_value_t** value,
                                             orionpp_value_t* elements,
                                             uint32_t count,
                                             const orionpp_allocator_t* allocator) {
  if (!value || (!elements && count > 0)) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_value_t* array_value = allocator->malloc(sizeof(orionpp_value_t));
  if (!array_value) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  array_value->type = ORIONPP_VALUE_ARRAY;
  array_value->array.count = count;
  
  if (count > 0) {
    array_value->array.values = allocator->malloc(sizeof(orionpp_value_t) * count);
    if (!array_value->array.values) {
      allocator->free(array_value);
      return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
    }
    
    // Deep copy elements
    for (uint32_t i = 0; i < count; i++) {
      orionpp_result_t copy_result = orionpp_value_copy(&array_value->array.values[i], &elements[i], allocator);
      if (ORIONPP_IS_ERROR(copy_result)) {
        // Clean up partial array
        for (uint32_t j = 0; j < i; j++) {
          orionpp_value_free(&array_value->array.values[j], allocator);
        }
        allocator->free(array_value->array.values);
        allocator->free(array_value);
        return copy_result;
      }
    }
  } else {
    array_value->array.values = NULL;
  }
  
  *value = array_value;
  return ORIONPP_OK_PTR(array_value);
}

void orionpp_value_array_destroy(orionpp_value_t* value, const orionpp_allocator_t* allocator) {
  if (!value || value->type != ORIONPP_VALUE_ARRAY) return;
  if (!allocator) allocator = &orionpp_default_allocator;
  
  if (value->array.values) {
    for (uint32_t i = 0; i < value->array.count; i++) {
      orionpp_value_free(&value->array.values[i], allocator);
    }
    allocator->free(value->array.values);
  }
  
  allocator->free(value);
}

bool orionpp_value_is_valid(const orionpp_value_t* value) {
  if (!value) return false;
  
  switch (value->type) {
    case ORIONPP_VALUE_VARIABLE:
    case ORIONPP_VALUE_SYMBOL:
    case ORIONPP_VALUE_NUMERIC:
    case ORIONPP_VALUE_LABEL:
    case ORIONPP_VALUE_STRING:
      return true;
    case ORIONPP_VALUE_ARRAY:
      return value->array.count == 0 || value->array.values != NULL;
    default:
      return false;
  }
}

bool orionpp_value_is_variable(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_VARIABLE;
}

bool orionpp_value_is_symbol(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_SYMBOL;
}

bool orionpp_value_is_numeric(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_NUMERIC;
}

bool orionpp_value_is_label(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_LABEL;
}

bool orionpp_value_is_array(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_ARRAY;
}

bool orionpp_value_is_string(const orionpp_value_t* value) {
  return value && value->type == ORIONPP_VALUE_STRING;
}

uint32_t orionpp_value_get_variable_id(const orionpp_value_t* value) {
  if (!orionpp_value_is_variable(value)) return 0;
  return value->variable_id;
}

uint64_t orionpp_value_get_numeric_value(const orionpp_value_t* value) {
  if (!orionpp_value_is_numeric(value)) return 0;
  return value->numeric.value;
}

orionpp_numeric_base_t orionpp_value_get_numeric_base(const orionpp_value_t* value) {
  if (!orionpp_value_is_numeric(value)) return ORIONPP_BASE_DECIMAL;
  return value->numeric.base;
}

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

orionpp_result_t orionpp_value_write_text(const orionpp_value_t* value,
                                           const orionpp_string_table_t* string_table,
                                           FILE* file) {
  if (!value || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (value->type) {
    case ORIONPP_VALUE_VARIABLE:
      fprintf(file, "$%u", value->variable_id);
      break;
      
    case ORIONPP_VALUE_SYMBOL: {
      const char* name = orionpp_string_table_get(string_table, value->symbol.name_offset);
      if (name) {
        fprintf(file, "@%s@", name);
      } else {
        fprintf(file, "@<invalid>@");
      }
      break;
    }
    
    case ORIONPP_VALUE_NUMERIC: {
      char base_char;
      const char* format;
      switch (value->numeric.base) {
        case ORIONPP_BASE_BINARY:  base_char = 'b'; format = "%llx"; break;
        case ORIONPP_BASE_OCTAL:   base_char = 'o'; format = "%llo"; break;
        case ORIONPP_BASE_DECIMAL: base_char = 'd'; format = "%llu"; break;
        case ORIONPP_BASE_HEX:     base_char = 'x'; format = "%llx"; break;
        default: base_char = 'd'; format = "%llu"; break;
      }
      fprintf(file, "%%%c", base_char);
      fprintf(file, format, value->numeric.value);
      break;
    }
    
    case ORIONPP_VALUE_LABEL: {
      const char* name = orionpp_string_table_get(string_table, value->label.name_offset);
      if (name) {
        if (value->label.direction > 0) {
          fprintf(file, "+.%s", name);
        } else if (value->label.direction < 0) {
          fprintf(file, "-.%s", name);
        } else {
          fprintf(file, ".%s", name);
        }
      } else {
        fprintf(file, ".<invalid>");
      }
      break;
    }
    
    case ORIONPP_VALUE_STRING: {
      const char* str = orionpp_string_table_get(string_table, value->string.offset);
      if (str) {
        fprintf(file, "\"%s\"", str);
      } else {
        fprintf(file, "\"<invalid>\"");
      }
      break;
    }
    
    case ORIONPP_VALUE_ARRAY:
      fprintf(file, "[");
      for (uint32_t i = 0; i < value->array.count; i++) {
        if (i > 0) fprintf(file, ", ");
        orionpp_value_write_text(&value->array.values[i], string_table, file);
      }
      fprintf(file, "]");
      break;
      
    default:
      fprintf(file, "<unknown_value>");
      break;
  }
  
  return ORIONPP_OK_INT(0);
}

// Parse numeric value from string (handles %x123, %d456, etc.)
static orionpp_result_t parse_numeric_value(const char* str, orionpp_value_t* value) {
  if (!str || str[0] != '%') return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
  
  char base_char = str[1];
  const char* number_str = str + 2;
  
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
    return parse_numeric_value(text, value);
  }
  
  if (*text == '.' || *text == '+' || *text == '-') {
    // Label: .label, +.label, -.label
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
  
  return ORIONPP_ERROR(ORIONPP_ERROR_PARSE_ERROR);
}

orionpp_result_t orionpp_value_copy(orionpp_value_t* dest,
                                     const orionpp_value_t* src,
                                     const orionpp_allocator_t* allocator) {
  if (!dest || !src) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  *dest = *src; // Copy basic data
  
  // Handle deep copy for arrays
  if (src->type == ORIONPP_VALUE_ARRAY && src->array.count > 0) {
    dest->array.values = allocator->malloc(sizeof(orionpp_value_t) * src->array.count);
    if (!dest->array.values) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
    
    for (uint32_t i = 0; i < src->array.count; i++) {
      orionpp_result_t result = orionpp_value_copy(&dest->array.values[i], &src->array.values[i], allocator);
      if (ORIONPP_IS_ERROR(result)) {
        // Clean up partial copy
        for (uint32_t j = 0; j < i; j++) {
          orionpp_value_free(&dest->array.values[j], allocator);
        }
        allocator->free(dest->array.values);
        return result;
      }
    }
  }
  
  return ORIONPP_OK_INT(0);
}

void orionpp_value_free(orionpp_value_t* value, const orionpp_allocator_t* allocator) {
  if (!value) return;
  if (!allocator) allocator = &orionpp_default_allocator;
  
  if (value->type == ORIONPP_VALUE_ARRAY && value->array.values) {
    for (uint32_t i = 0; i < value->array.count; i++) {
      orionpp_value_free(&value->array.values[i], allocator);
    }
    allocator->free(value->array.values);
    value->array.values = NULL;
    value->array.count = 0;
  }
}