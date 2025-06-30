#include "orionpp/instruction.h"
#include <string.h>
#include <stdlib.h>

/**
 * @file instruction_serialize.c
 * @brief Instruction serialization and deserialization implementation
 * 
 * Handles conversion between instruction structures and binary data
 * for storage and transmission purposes.
 */

static size_t calculate_type_size(const orionpp_type_t *type) {
  size_t size = sizeof(orionpp_type_raw_t) + sizeof(size_t); // base + count
  
  if (type->count > 0 && type->types) {
    for (size_t i = 0; i < type->count; i++) {
      size += calculate_type_size(&type->types[i]);
    }
  }
  
  return size;
}

static size_t calculate_value_size(const orionpp_value_t *value) {
  if (!value) return 0;
  
  size_t size = 0;
  size += calculate_type_size(&value->type);
  size += sizeof(size_t); // value_byte_size
  size += value->value_byte_size; // actual value data
  
  return size;
}

static orionpp_error_t serialize_type(char *buf, size_t bufsize, size_t *offset, const orionpp_type_t *type) {
  if (!buf || !offset || !type) return ORIONPP_ERROR_INVALID_ARG;
  
  // Check space for base type
  if (*offset + sizeof(orionpp_type_raw_t) + sizeof(size_t) > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Serialize base type
  memcpy(buf + *offset, &type->base, sizeof(orionpp_type_raw_t));
  *offset += sizeof(orionpp_type_raw_t);
  
  // Serialize count
  memcpy(buf + *offset, &type->count, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Serialize nested types
  if (type->count > 0 && type->types) {
    for (size_t i = 0; i < type->count; i++) {
      orionpp_error_t err = serialize_type(buf, bufsize, offset, &type->types[i]);
      if (err != ORIONPP_ERROR_GOOD) return err;
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

static orionpp_error_t deserialize_type(const char *buf, size_t bufsize, size_t *offset, orionpp_type_t *type) {
  if (!buf || !offset || !type) return ORIONPP_ERROR_INVALID_ARG;
  
  // Check space for base type
  if (*offset + sizeof(orionpp_type_raw_t) + sizeof(size_t) > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Deserialize base type
  memcpy(&type->base, buf + *offset, sizeof(orionpp_type_raw_t));
  *offset += sizeof(orionpp_type_raw_t);
  
  // Deserialize count
  memcpy(&type->count, buf + *offset, sizeof(size_t));
  *offset += sizeof(size_t);
  
  type->types = NULL;
  
  // Deserialize nested types
  if (type->count > 0) {
    type->types = malloc(type->count * sizeof(orionpp_type_t));
    if (!type->types) return ORIONPP_ERROR_OUT_OF_MEMORY;
    
    for (size_t i = 0; i < type->count; i++) {
      orionpp_error_t err = deserialize_type(buf, bufsize, offset, &type->types[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (type->types[j].types) free(type->types[j].types);
        }
        free(type->types);
        type->types = NULL;
        return err;
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

static orionpp_error_t serialize_value(char *buf, size_t bufsize, size_t *offset, const orionpp_value_t *value) {
  if (!buf || !offset || !value) return ORIONPP_ERROR_INVALID_ARG;
  
  // Serialize type
  orionpp_error_t err = serialize_type(buf, bufsize, offset, &value->type);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Check space for value_byte_size
  if (*offset + sizeof(size_t) > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Serialize value_byte_size
  memcpy(buf + *offset, &value->value_byte_size, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Serialize value data
  if (value->value_byte_size > 0) {
    if (*offset + value->value_byte_size > bufsize) {
      return ORIONPP_ERROR_BUFFER_OVERFLOW;
    }
    
    if (value->value) {
      memcpy(buf + *offset, value->value, value->value_byte_size);
    } else {
      // Zero out the space if no data
      memset(buf + *offset, 0, value->value_byte_size);
    }
    *offset += value->value_byte_size;
  }
  
  return ORIONPP_ERROR_GOOD;
}

static orionpp_error_t deserialize_value(const char *buf, size_t bufsize, size_t *offset, orionpp_value_t *value) {
  if (!buf || !offset || !value) return ORIONPP_ERROR_INVALID_ARG;
  
  memset(value, 0, sizeof(orionpp_value_t));
  
  // Deserialize type
  orionpp_error_t err = deserialize_type(buf, bufsize, offset, &value->type);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Check space for value_byte_size
  if (*offset + sizeof(size_t) > bufsize) {
    // Cleanup type on error
    if (value->type.types) {
      for (size_t i = 0; i < value->type.count; i++) {
        if (value->type.types[i].types) free(value->type.types[i].types);
      }
      free(value->type.types);
    }
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Deserialize value_byte_size
  memcpy(&value->value_byte_size, buf + *offset, sizeof(size_t));
  *offset += sizeof(size_t);
  
  value->value = NULL;
  
  // Deserialize value data
  if (value->value_byte_size > 0) {
    if (*offset + value->value_byte_size > bufsize) {
      // Cleanup type on error
      if (value->type.types) {
        for (size_t i = 0; i < value->type.count; i++) {
          if (value->type.types[i].types) free(value->type.types[i].types);
        }
        free(value->type.types);
      }
      return ORIONPP_ERROR_BUFFER_OVERFLOW;
    }
    
    value->value = malloc(value->value_byte_size);
    if (!value->value) {
      // Cleanup type on error
      if (value->type.types) {
        for (size_t i = 0; i < value->type.count; i++) {
          if (value->type.types[i].types) free(value->type.types[i].types);
        }
        free(value->type.types);
      }
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    memcpy(value->value, buf + *offset, value->value_byte_size);
    *offset += value->value_byte_size;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_serialize_instr(char *buf, size_t bufsize, const orionpp_instruction_t *src) {
  if (!buf || !src) return ORIONPP_ERROR_INVALID_ARG;
  
  size_t offset = 0;
  
  // Check basic space requirements
  if (bufsize < sizeof(orionpp_opcode_t) + sizeof(size_t)) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Serialize opcode
  memcpy(buf + offset, &src->opcode, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Serialize value count
  memcpy(buf + offset, &src->value_count, sizeof(size_t));
  offset += sizeof(size_t);
  
  // Serialize values
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      orionpp_error_t err = serialize_value(buf, bufsize, &offset, &src->values[i]);
      if (err != ORIONPP_ERROR_GOOD) return err;
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_deserialize_instr(const char *buf, size_t bufsize, orionpp_instruction_t *dest) {
  if (!buf || !dest || bufsize < sizeof(orionpp_opcode_t) + sizeof(size_t)) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  memset(dest, 0, sizeof(orionpp_instruction_t));
  size_t offset = 0;
  
  // Deserialize opcode
  memcpy(&dest->opcode, buf + offset, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Deserialize value count
  memcpy(&dest->value_count, buf + offset, sizeof(size_t));
  offset += sizeof(size_t);
  
  dest->values = NULL;
  
  // Deserialize values
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) return ORIONPP_ERROR_OUT_OF_MEMORY;
    
    for (size_t i = 0; i < dest->value_count; i++) {
      orionpp_error_t err = deserialize_value(buf, bufsize, &offset, &dest->values[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) {
            for (size_t k = 0; k < dest->values[j].type.count; k++) {
              if (dest->values[j].type.types[k].types) free(dest->values[j].type.types[k].types);
            }
            free(dest->values[j].type.types);
          }
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