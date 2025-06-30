#include "orionpp/instruction.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file instruction.c
 * @brief Core instruction operations implementation
 * 
 * Provides basic instruction lifecycle management including
 * initialization, destruction, and copying operations.
 */

static orionpp_error_t copy_type(orionpp_type_t *dest, const orionpp_type_t *src) {
  if (!dest || !src) return ORIONPP_ERROR_INVALID_ARG;
  
  dest->base = src->base;
  dest->count = src->count;
  dest->types = NULL;
  
  if (src->count > 0 && src->types) {
    dest->types = malloc(src->count * sizeof(orionpp_type_t));
    if (!dest->types) return ORIONPP_ERROR_OUT_OF_MEMORY;
    
    for (size_t i = 0; i < src->count; i++) {
      orionpp_error_t err = copy_type(&dest->types[i], &src->types[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->types[j].types) free(dest->types[j].types);
        }
        free(dest->types);
        dest->types = NULL;
        return err;
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

static orionpp_error_t copy_value(orionpp_value_t *dest, const orionpp_value_t *src) {
  if (!dest || !src) return ORIONPP_ERROR_INVALID_ARG;
  
  // Copy type
  orionpp_error_t err = copy_type(&dest->type, &src->type);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Copy value data
  dest->value_byte_size = src->value_byte_size;
  dest->value = NULL;
  
  if (src->value_byte_size > 0 && src->value) {
    dest->value = malloc(src->value_byte_size);
    if (!dest->value) {
      // Cleanup type on error
      if (dest->type.types) {
        for (size_t i = 0; i < dest->type.count; i++) {
          if (dest->type.types[i].types) free(dest->type.types[i].types);
        }
        free(dest->type.types);
      }
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    memcpy(dest->value, src->value, src->value_byte_size);
  }
  
  return ORIONPP_ERROR_GOOD;
}

static void destroy_type(orionpp_type_t *type) {
  if (!type) return;
  
  if (type->types) {
    for (size_t i = 0; i < type->count; i++) {
      destroy_type(&type->types[i]);
    }
    free(type->types);
    type->types = NULL;
  }
  type->count = 0;
}

static void destroy_value(orionpp_value_t *value) {
  if (!value) return;
  
  destroy_type(&value->type);
  if (value->value) {
    free(value->value);
    value->value = NULL;
  }
  value->value_byte_size = 0;
}

orionpp_error_t orionpp_instruction_init(orionpp_instruction_t *instr) {
  if (!instr) return ORIONPP_ERROR_INVALID_ARG;
  
  memset(instr, 0, sizeof(orionpp_instruction_t));
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_instruction_destroy(orionpp_instruction_t *instr) {
  if (!instr) return ORIONPP_ERROR_INVALID_ARG;
  
  if (instr->values) {
    for (size_t i = 0; i < instr->value_count; i++) {
      destroy_value(&instr->values[i]);
    }
    free(instr->values);
    instr->values = NULL;
  }
  
  instr->value_count = 0;
  memset(&instr->opcode, 0, sizeof(instr->opcode));
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_instruction_copy(orionpp_instruction_t *dest, const orionpp_instruction_t *src) {
  if (!dest || !src) return ORIONPP_ERROR_INVALID_ARG;
  
  // Initialize destination
  orionpp_error_t err = orionpp_instruction_init(dest);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Copy opcode
  dest->opcode = src->opcode;
  dest->value_count = src->value_count;
  
  // Copy values if any
  if (src->value_count > 0 && src->values) {
    dest->values = malloc(src->value_count * sizeof(orionpp_value_t));
    if (!dest->values) return ORIONPP_ERROR_OUT_OF_MEMORY;
    
    for (size_t i = 0; i < src->value_count; i++) {
      err = copy_value(&dest->values[i], &src->values[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          destroy_value(&dest->values[j]);
        }
        free(dest->values);
        dest->values = NULL;
        dest->value_count = 0;
        return err;
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}