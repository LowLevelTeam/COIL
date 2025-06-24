#ifndef ORIONPP_VALUE_VALUE_H
#define ORIONPP_VALUE_VALUE_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"

// Value types
typedef enum {
  ORIONPP_VALUE_NONE = 0,
  ORIONPP_VALUE_VARIABLE = 1,    // $n
  ORIONPP_VALUE_SYMBOL = 2,      // @symbol@
  ORIONPP_VALUE_NUMERIC = 3,     // %base[digits]
  ORIONPP_VALUE_ARRAY = 4,       // [values...]
  ORIONPP_VALUE_LABEL = 5,       // .label / +.label / -.label
  ORIONPP_VALUE_STRING = 6,      // "string"
} orionpp_value_type_t;

// Numeric base types
typedef enum {
  ORIONPP_BASE_BINARY = 2,
  ORIONPP_BASE_OCTAL = 8,
  ORIONPP_BASE_DECIMAL = 10,
  ORIONPP_BASE_HEX = 16,
} orionpp_numeric_base_t;

// Value structure
struct orionpp_value {
  orionpp_value_type_t type;
  union {
    // Variable reference: $n
    uint32_t variable_id;
    
    // Symbol reference: @symbol@
    struct {
      uint32_t name_offset;     // Offset into string table
      uint32_t name_length;
    } symbol;
    
    // Numeric literal: %base[digits]
    struct {
      orionpp_numeric_base_t base;
      uint64_t value;
    } numeric;
    
    // Array of values: [v1, v2, ...]
    struct {
      uint32_t count;
      orionpp_value_t* values;
    } array;
    
    // Label reference: .label, +.label, -.label
    struct {
      uint32_t name_offset;     // Offset into string table
      uint32_t name_length;
      int8_t direction;         // -1 = backward, 0 = local, +1 = forward
    } label;
    
    // String literal: "string"
    struct {
      uint32_t offset;          // Offset into string table
      uint32_t length;
    } string;
  };
};

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

// Value copying (deep copy for arrays)
orionpp_result_t orionpp_value_copy(orionpp_value_t* dest,
                                     const orionpp_value_t* src,
                                     const orionpp_allocator_t* allocator);

void orionpp_value_free(orionpp_value_t* value, const orionpp_allocator_t* allocator);

#endif // ORIONPP_VALUE_VALUE_H