#include "orionpp/instruction.h"
#include "orionpp/arena.h"
#include <string.h>
#include <stdlib.h>

/**
 * @file instruction_io.c
 * @brief Instruction I/O operations implementation
 * 
 * Handles reading and writing instructions to files and arenas,
 * using the serialization functions for data conversion.
 */

#ifdef WIN32
  #include <io.h>
#else
  #include <unistd.h>
  #include <fcntl.h>
#endif

// Maximum serialization buffer size (adjust as needed)
#define MAX_INSTR_SIZE (64 * 1024)

orionpp_error_t orionpp_read_file(orionpp_file_t file, orionpp_instruction_t *dest) {
  if (!dest) return ORIONPP_ERROR_INVALID_ARG;
  
  // Read the instruction size first (we'll need to modify the format for this)
  // For now, we'll use a fixed buffer and hope it's large enough
  char buffer[MAX_INSTR_SIZE];
  
#ifdef WIN32
  DWORD bytes_read;
  
  // Read opcode
  if (!ReadFile(file, &dest->opcode, sizeof(dest->opcode), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read value count
  if (!ReadFile(file, &dest->value_count, sizeof(dest->value_count), &bytes_read, NULL) ||
      bytes_read != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // For now, rewind and read the whole instruction using serialization
  SetFilePointer(file, -(LONG)(sizeof(dest->opcode) + sizeof(dest->value_count)), NULL, FILE_CURRENT);
  
  // Read enough data (this is a simplified approach)
  if (!ReadFile(file, buffer, sizeof(buffer), &bytes_read, NULL) || bytes_read == 0) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
#else
  // Read opcode
  if (read(file, &dest->opcode, sizeof(dest->opcode)) != sizeof(dest->opcode)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // Read value count
  if (read(file, &dest->value_count, sizeof(dest->value_count)) != sizeof(dest->value_count)) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  // For now, rewind and read the whole instruction using serialization
  lseek(file, -(off_t)(sizeof(dest->opcode) + sizeof(dest->value_count)), SEEK_CUR);
  
  // Read enough data (this is a simplified approach)
  ssize_t bytes_read = read(file, buffer, sizeof(buffer));
  if (bytes_read <= 0) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  // Deserialize the instruction
  return orionpp_deserialize_instr(buffer, (size_t)bytes_read, dest);
}

orionpp_error_t orionpp_write_file(orionpp_file_t file, const orionpp_instruction_t *src) {
  if (!src) return ORIONPP_ERROR_INVALID_ARG;
  
  char buffer[MAX_INSTR_SIZE];
  
  // Serialize instruction to buffer
  orionpp_error_t err = orionpp_serialize_instr(buffer, sizeof(buffer), src);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Calculate serialized size (this is approximate - we need actual size)
  size_t serialized_size = sizeof(orionpp_opcode_t) + sizeof(size_t);
  
  // Add value sizes (simplified calculation)
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      serialized_size += sizeof(orionpp_type_raw_t) + sizeof(size_t) * 2; // simplified
      serialized_size += src->values[i].value_byte_size;
    }
  }
  
  if (serialized_size > sizeof(buffer)) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
#ifdef WIN32
  DWORD bytes_written;
  if (!WriteFile(file, buffer, (DWORD)serialized_size, &bytes_written, NULL) ||
      bytes_written != serialized_size) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#else
  if (write(file, buffer, serialized_size) != (ssize_t)serialized_size) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
#endif
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_read_arena(struct orionpp_arena *arena, orionpp_instruction_t *dest) {
  if (!arena || !dest) return ORIONPP_ERROR_INVALID_ARG;
  
  char buffer[MAX_INSTR_SIZE];
  size_t bytes_read;
  
  // Read from arena into buffer
  orionpp_error_t err = orionpp_arena_read(arena, buffer, sizeof(buffer), &bytes_read);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  if (bytes_read == 0) {
    return ORIONPP_ERROR_FILE_ERROR; // No data to read
  }
  
  // Deserialize from buffer
  return orionpp_deserialize_instr(buffer, bytes_read, dest);
}

orionpp_error_t orionpp_write_arena(struct orionpp_arena *arena, const orionpp_instruction_t *src) {
  if (!arena || !src) return ORIONPP_ERROR_INVALID_ARG;
  
  char buffer[MAX_INSTR_SIZE];
  
  // Serialize instruction to buffer
  orionpp_error_t err = orionpp_serialize_instr(buffer, sizeof(buffer), src);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  // Calculate actual serialized size (simplified)
  size_t serialized_size = sizeof(orionpp_opcode_t) + sizeof(size_t);
  
  if (src->value_count > 0 && src->values) {
    for (size_t i = 0; i < src->value_count; i++) {
      serialized_size += sizeof(orionpp_type_raw_t) + sizeof(size_t) * 2;
      serialized_size += src->values[i].value_byte_size;
    }
  }
  
  if (serialized_size > sizeof(buffer)) {
    return ORIONPP_ERROR_BUFFER_OVERFLOW;
  }
  
  // Write to arena
  size_t bytes_written;
  err = orionpp_arena_write(arena, buffer, serialized_size, &bytes_written);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  if (bytes_written != serialized_size) {
    return ORIONPP_ERROR_FILE_ERROR;
  }
  
  return ORIONPP_ERROR_GOOD;
}