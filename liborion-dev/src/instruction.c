#include "orionpp/instruction.h"
#include "orionpp/error.h"
#include "orionpp/arena.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef WIN32
  #include <io.h>
#else
  #include <unistd.h>
#endif

// Helper function to calculate serialized value size
static size_t calculate_value_serialized_size(const orionpp_value_t *value) {
  if (!value) return 0;
  
  size_t size = 0;
  // Type information
  size += sizeof(orionpp_type_raw_t);
  size += sizeof(size_t); // count
  size += sizeof(size_t); // value_byte_size
  
  // Nested types (if any)
  if (value->type.count > 0 && value->type.types) {
    for (size_t i = 0; i < value->type.count; i++) {
      size += sizeof(orionpp_type_raw_t);
      size += sizeof(size_t); // count for nested type
    }
  }
  
  // Actual value data
  size += value->value_byte_size;
  
  return size;
}

// Helper function to serialize a single value
static orionpp_error_t serialize_value(char *buf, size_t bufsize, size_t *offset, const orionpp_value_t *value) {
  if (!buf || !offset || !value) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  size_t needed = calculate_value_serialized_size(value);
  if (*offset + needed > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Serialize type base
  memcpy(buf + *offset, &value->type.base, sizeof(orionpp_type_raw_t));
  *offset += sizeof(orionpp_type_raw_t);
  
  // Serialize type count
  memcpy(buf + *offset, &value->type.count, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Serialize nested types (simplified - just the base for now)
  if (value->type.count > 0 && value->type.types) {
    for (size_t i = 0; i < value->type.count; i++) {
      memcpy(buf + *offset, &value->type.types[i].base, sizeof(orionpp_type_raw_t));
      *offset += sizeof(orionpp_type_raw_t);
      
      size_t nested_count = value->type.types[i].count;
      memcpy(buf + *offset, &nested_count, sizeof(size_t));
      *offset += sizeof(size_t);
    }
  }
  
  // Serialize value byte size
  memcpy(buf + *offset, &value->value_byte_size, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Serialize actual value data
  if (value->value_byte_size > 0 && value->value) {
    memcpy(buf + *offset, value->value, value->value_byte_size);
    *offset += value->value_byte_size;
  }
  
  return ORIONPP_ERROR_GOOD;
}

// Helper function to deserialize a single value
static orionpp_error_t deserialize_value(const char *buf, size_t bufsize, size_t *offset, orionpp_value_t *value) {
  if (!buf || !offset || !value) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Clear the value first
  memset(value, 0, sizeof(orionpp_value_t));
  
  // Check buffer bounds for basic type info
  if (*offset + sizeof(orionpp_type_raw_t) + 2 * sizeof(size_t) > bufsize) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Deserialize type base
  memcpy(&value->type.base, buf + *offset, sizeof(orionpp_type_raw_t));
  *offset += sizeof(orionpp_type_raw_t);
  
  // Deserialize type count
  memcpy(&value->type.count, buf + *offset, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Deserialize nested types (simplified)
  if (value->type.count > 0) {
    if (*offset + value->type.count * (sizeof(orionpp_type_raw_t) + sizeof(size_t)) > bufsize) {
      return ORIONPP_ERROR_BUFFER_OVERFLOW;
    }
    
    value->type.types = malloc(value->type.count * sizeof(orionpp_type_t));
    if (!value->type.types) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    for (size_t i = 0; i < value->type.count; i++) {
      memcpy(&value->type.types[i].base, buf + *offset, sizeof(orionpp_type_raw_t));
      *offset += sizeof(orionpp_type_raw_t);
      
      memcpy(&value->type.types[i].count, buf + *offset, sizeof(size_t));
      *offset += sizeof(size_t);
      
      value->type.types[i].types = NULL; // Simplified - no deeper nesting
    }
  } else {
    value->type.types = NULL;
  }
  
  // Deserialize value byte size
  if (*offset + sizeof(size_t) > bufsize) {
    if (value->type.types) free(value->type.types);
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  memcpy(&value->value_byte_size, buf + *offset, sizeof(size_t));
  *offset += sizeof(size_t);
  
  // Deserialize actual value data
  if (value->value_byte_size > 0) {
    if (*offset + value->value_byte_size > bufsize) {
      if (value->type.types) free(value->type.types);
      return ORIONPP_ERROR_BUFFER_OVERFLOW;
    }
    
    value->value = malloc(value->value_byte_size);
    if (!value->value) {
      if (value->type.types) free(value->type.types);
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    memcpy(value->value, buf + *offset, value->value_byte_size);
    *offset += value->value_byte_size;
  } else {
    value->value = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

// Arena I/O Implementation
orionpp_error_t orionpp_readarena(orionpp_arena_t *arena, orionpp_instruction_t *dest) {
  if (!arena || !dest) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  if (!arena->initialized) {
    return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  }
  
  // Clear destination
  memset(dest, 0, sizeof(orionpp_instruction_t));
  
  size_t bytes_read;
  orionpp_error_t err;
  
  // Read opcode
  err = orionpp_arena_read(arena, &dest->opcode, sizeof(dest->opcode), &bytes_read);
  if (err != ORIONPP_ERROR_GOOD || bytes_read != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read value count
  err = orionpp_arena_read(arena, &dest->value_count, sizeof(dest->value_count), &bytes_read);
  if (err != ORIONPP_ERROR_GOOD || bytes_read != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read values if any
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    for (size_t i = 0; i < dest->value_count; i++) {
      // Read type info
      orionpp_type_raw_t type_base;
      size_t type_count, value_byte_size;
      
      err = orionpp_arena_read(arena, &type_base, sizeof(type_base), &bytes_read);
      if (err != ORIONPP_ERROR_GOOD || bytes_read != sizeof(type_base)) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      err = orionpp_arena_read(arena, &type_count, sizeof(type_count), &bytes_read);
      if (err != ORIONPP_ERROR_GOOD || bytes_read != sizeof(type_count)) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      err = orionpp_arena_read(arena, &value_byte_size, sizeof(value_byte_size), &bytes_read);
      if (err != ORIONPP_ERROR_GOOD || bytes_read != sizeof(value_byte_size)) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      // Set up the value
      dest->values[i].type.base = type_base;
      dest->values[i].type.count = type_count;
      dest->values[i].type.types = NULL; // Simplified for now
      dest->values[i].value_byte_size = value_byte_size;
      
      // Read actual value data
      if (value_byte_size > 0) {
        dest->values[i].value = malloc(value_byte_size);
        if (!dest->values[i].value) {
          // Cleanup on error
          for (size_t j = 0; j < i; j++) {
            if (dest->values[j].value) free(dest->values[j].value);
            if (dest->values[j].type.types) free(dest->values[j].type.types);
          }
          free(dest->values);
          return ORIONPP_ERROR_OUT_OF_MEMORY;
        }
        
        err = orionpp_arena_read(arena, dest->values[i].value, value_byte_size, &bytes_read);
        if (err != ORIONPP_ERROR_GOOD || bytes_read != value_byte_size) {
          // Cleanup on error
          for (size_t j = 0; j <= i; j++) {
            if (dest->values[j].value) free(dest->values[j].value);
            if (dest->values[j].type.types) free(dest->values[j].type.types);
          }
          free(dest->values);
          return ORIONPP_ERROR_FILE_ERROR;
        }
      } else {
        dest->values[i].value = NULL;
      }
    }
  } else {
    dest->values = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writearena(orionpp_arena_t *arena, const orionpp_instruction_t *src) {
  if (!arena || !src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  if (!arena->initialized) {
    return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  }
  
  size_t bytes_written;
  orionpp_error_t err;
  
  // Write opcode
  err = orionpp_arena_write(arena, &src->opcode, sizeof(src->opcode), &bytes_written);
  if (err != ORIONPP_ERROR_GOOD || bytes_written != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Write value count
  err = orionpp_arena_write(arena, &src->value_count, sizeof(src->value_count), &bytes_written);
  if (err != ORIONPP_ERROR_GOOD || bytes_written != sizeof(src->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Write values if any
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      // Write type info
      err = orionpp_arena_write(arena, &src->values[i].type.base, sizeof(orionpp_type_raw_t), &bytes_written);
      if (err != ORIONPP_ERROR_GOOD || bytes_written != sizeof(orionpp_type_raw_t)) {
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      err = orionpp_arena_write(arena, &src->values[i].type.count, sizeof(size_t), &bytes_written);
      if (err != ORIONPP_ERROR_GOOD || bytes_written != sizeof(size_t)) {
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      err = orionpp_arena_write(arena, &src->values[i].value_byte_size, sizeof(size_t), &bytes_written);
      if (err != ORIONPP_ERROR_GOOD || bytes_written != sizeof(size_t)) {
        return ORIONPP_ERROR_FILE_ERROR;
      }
      
      // Write actual value data
      if (src->values[i].value_byte_size > 0 && src->values[i].value) {
        err = orionpp_arena_write(arena, src->values[i].value, src->values[i].value_byte_size, &bytes_written);
        if (err != ORIONPP_ERROR_GOOD || bytes_written != src->values[i].value_byte_size) {
          return ORIONPP_ERROR_FILE_ERROR;
        }
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

// File I/O Implementation (simplified from original)
orionpp_error_t orionpp_readf(file_handle_t file, orionpp_instruction_t *dest) {
  if (!dest) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Clear destination
  memset(dest, 0, sizeof(orionpp_instruction_t));
  
  // Read opcode
#ifdef WIN32
  DWORD bytes_read;
  if (!ReadFile(file, &dest->opcode, sizeof(dest->opcode), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read value count
  if (!ReadFile(file, &dest->value_count, sizeof(dest->value_count), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (read(file, &dest->opcode, sizeof(dest->opcode)) != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  if (read(file, &dest->value_count, sizeof(dest->value_count)) != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // Allocate and read values if any
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    // Read each value with proper serialization
    for (size_t i = 0; i < dest->value_count; i++) {
      // Read type info
      orionpp_type_raw_t type_base;
      size_t type_count, value_byte_size;
      
#ifdef WIN32
      if (!ReadFile(file, &type_base, sizeof(type_base), &bytes_read, NULL) ||
          bytes_read != sizeof(type_base) ||
          !ReadFile(file, &type_count, sizeof(type_count), &bytes_read, NULL) ||
          bytes_read != sizeof(type_count) ||
          !ReadFile(file, &value_byte_size, sizeof(value_byte_size), &bytes_read, NULL) ||
          bytes_read != sizeof(value_byte_size)) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        return ORIONPP_ERROR_FILE_ERROR;
      }
#else
      if (read(file, &type_base, sizeof(type_base)) != sizeof(type_base) ||
          read(file, &type_count, sizeof(type_count)) != sizeof(type_count) ||
          read(file, &value_byte_size, sizeof(value_byte_size)) != sizeof(value_byte_size)) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        return ORIONPP_ERROR_FILE_ERROR;
      }
#endif
      
      // Set up the value
      dest->values[i].type.base = type_base;
      dest->values[i].type.count = type_count;
      dest->values[i].type.types = NULL; // Simplified for now
      dest->values[i].value_byte_size = value_byte_size;
      
      // Read actual value data
      if (value_byte_size > 0) {
        dest->values[i].value = malloc(value_byte_size);
        if (!dest->values[i].value) {
          // Cleanup on error
          for (size_t j = 0; j < i; j++) {
            if (dest->values[j].value) free(dest->values[j].value);
            if (dest->values[j].type.types) free(dest->values[j].type.types);
          }
          free(dest->values);
          return ORIONPP_ERROR_OUT_OF_MEMORY;
        }
        
#ifdef WIN32
        if (!ReadFile(file, dest->values[i].value, value_byte_size, &bytes_read, NULL) ||
            bytes_read != value_byte_size) {
          // Cleanup on error
          for (size_t j = 0; j <= i; j++) {
            if (dest->values[j].value) free(dest->values[j].value);
            if (dest->values[j].type.types) free(dest->values[j].type.types);
          }
          free(dest->values);
          return ORIONPP_ERROR_FILE_ERROR;
        }
#else
        if (read(file, dest->values[i].value, value_byte_size) != (ssize_t)value_byte_size) {
          // Cleanup on error
          for (size_t j = 0; j <= i; j++) {
            if (dest->values[j].value) free(dest->values[j].value);
            if (dest->values[j].type.types) free(dest->values[j].type.types);
          }
          free(dest->values);
          return ORIONPP_ERROR_FILE_ERROR;
        }
#endif
      } else {
        dest->values[i].value = NULL;
      }
    }
  } else {
    dest->values = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writef(file_handle_t file, const orionpp_instruction_t *src) {
  if (!src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Write opcode
#ifdef WIN32
  DWORD bytes_written;
  if (!WriteFile(file, &src->opcode, sizeof(src->opcode), &bytes_written, NULL) ||
      bytes_written != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Write value count
  if (!WriteFile(file, &src->value_count, sizeof(src->value_count), &bytes_written, NULL) ||
      bytes_written != sizeof(src->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (write(file, &src->opcode, sizeof(src->opcode)) != sizeof(src->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  if (write(file, &src->value_count, sizeof(src->value_count)) != sizeof(src->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // Write values if any
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      // Write type info
#ifdef WIN32
      if (!WriteFile(file, &src->values[i].type.base, sizeof(orionpp_type_raw_t), &bytes_written, NULL) ||
          bytes_written != sizeof(orionpp_type_raw_t) ||
          !WriteFile(file, &src->values[i].type.count, sizeof(size_t), &bytes_written, NULL) ||
          bytes_written != sizeof(size_t) ||
          !WriteFile(file, &src->values[i].value_byte_size, sizeof(size_t), &bytes_written, NULL) ||
          bytes_written != sizeof(size_t)) {
        return ORIONPP_ERROR_FILE_ERROR;
      }
#else
      if (write(file, &src->values[i].type.base, sizeof(orionpp_type_raw_t)) != sizeof(orionpp_type_raw_t) ||
          write(file, &src->values[i].type.count, sizeof(size_t)) != sizeof(size_t) ||
          write(file, &src->values[i].value_byte_size, sizeof(size_t)) != sizeof(size_t)) {
        return ORIONPP_ERROR_FILE_ERROR;
      }
#endif
      
      // Write actual value data
      if (src->values[i].value_byte_size > 0 && src->values[i].value) {
#ifdef WIN32
        if (!WriteFile(file, src->values[i].value, src->values[i].value_byte_size, &bytes_written, NULL) ||
            bytes_written != src->values[i].value_byte_size) {
          return ORIONPP_ERROR_FILE_ERROR;
        }
#else
        if (write(file, src->values[i].value, src->values[i].value_byte_size) != (ssize_t)src->values[i].value_byte_size) {
          return ORIONPP_ERROR_FILE_ERROR;
        }
#endif
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}

// Buffer I/O helpers
orionpp_error_t orionpp_readbuf(const char *buf, size_t bufsize, orionpp_instruction_t *dest) {
  if (!buf || !dest || bufsize < sizeof(orionpp_opcode_t) + sizeof(size_t)) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Clear destination
  memset(dest, 0, sizeof(orionpp_instruction_t));
  
  size_t offset = 0;
  
  // Read opcode
  memcpy(&dest->opcode, buf + offset, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Read value count
  memcpy(&dest->value_count, buf + offset, sizeof(size_t));
  offset += sizeof(size_t);
  
  // Read values if any
  if (dest->value_count > 0) {
    dest->values = malloc(dest->value_count * sizeof(orionpp_value_t));
    if (!dest->values) {
      return ORIONPP_ERROR_OUT_OF_MEMORY;
    }
    
    for (size_t i = 0; i < dest->value_count; i++) {
      orionpp_error_t err = deserialize_value(buf, bufsize, &offset, &dest->values[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        // Cleanup on error
        for (size_t j = 0; j < i; j++) {
          if (dest->values[j].value) free(dest->values[j].value);
          if (dest->values[j].type.types) free(dest->values[j].type.types);
        }
        free(dest->values);
        dest->values = NULL;
        dest->value_count = 0;
        return err;
      }
    }
  } else {
    dest->values = NULL;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_writebuf(char *buf, size_t bufsize, const orionpp_instruction_t *src) {
  if (!buf || !src) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  size_t offset = 0;
  
  // Check basic space requirements
  if (bufsize < sizeof(orionpp_opcode_t) + sizeof(size_t)) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Write opcode
  memcpy(buf + offset, &src->opcode, sizeof(orionpp_opcode_t));
  offset += sizeof(orionpp_opcode_t);
  
  // Write value count
  memcpy(buf + offset, &src->value_count, sizeof(size_t));
  offset += sizeof(size_t);
  
  // Write values if any
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      orionpp_error_t err = serialize_value(buf, bufsize, &offset, &src->values[i]);
      if (err != ORIONPP_ERROR_GOOD) {
        return err;
      }
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}