#include "orionpp/value/value.h"
#include <string.h>
#include <stdlib.h>

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