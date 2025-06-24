#include "orionobj/binary.h"
#include <string.h>

orionobj_result_t orionobj_binary_detect_format(FILE* file, orionobj_format_t* format) {
  if (!file || !format) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  uint32_t magic;
  long pos = ftell(file);
  
  if (fread(&magic, sizeof(uint32_t), 1, file) != 1) {
    fseek(file, pos, SEEK_SET);
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }
  
  fseek(file, pos, SEEK_SET);

  if (magic == ORIONOBJ_MAGIC_ORIONPP) {
    *format = ORIONOBJ_FORMAT_ORIONPP;
    return ORIONOBJ_OK_INT(0);
  } else if (magic == ORIONOBJ_MAGIC_ORION) {
    *format = ORIONOBJ_FORMAT_ORION;
    return ORIONOBJ_OK_INT(0);
  }

  *format = ORIONOBJ_FORMAT_UNKNOWN;
  return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
}

orionobj_result_t orionobj_binary_read_header(FILE* file, orionobj_header_t* header) {
  if (!file || !header) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (fread(header, sizeof(orionobj_header_t), 1, file) != 1) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return orionobj_binary_validate_header(header);
}

orionobj_result_t orionobj_binary_write_header(FILE* file, const orionobj_header_t* header) {
  if (!file || !header) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (fwrite(header, sizeof(orionobj_header_t), 1, file) != 1) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return ORIONOBJ_OK_INT(0);
}

bool orionobj_binary_is_version_compatible(uint16_t file_major, uint16_t file_minor,
                                            uint16_t lib_major, uint16_t lib_minor) {
  // Major version must match exactly
  if (file_major != lib_major) return false;
  
  // Minor version must be <= library version (backward compatibility)
  if (file_minor > lib_minor) return false;
  
  return true;
}

orionobj_result_t orionobj_binary_validate_header(const orionobj_header_t* header) {
  if (!header) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (header->magic != ORIONOBJ_MAGIC_ORIONPP && header->magic != ORIONOBJ_MAGIC_ORION) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_CORRUPT_DATA);
  }

  if (!orionobj_binary_is_version_compatible(header->version_major, header->version_minor,
                                             ORIONOBJ_VERSION_MAJOR, ORIONOBJ_VERSION_MINOR)) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_binary_read_object(orionobj_object_t** object,
                                               FILE* file,
                                               const orionobj_allocator_t* allocator) {
  if (!object || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  orionobj_format_t format;
  orionobj_result_t result = orionobj_binary_detect_format(file, &format);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  switch (format) {
    case ORIONOBJ_FORMAT_ORIONPP:
      return orionobj_binary_read_orionpp(object, file, allocator);
    case ORIONOBJ_FORMAT_ORION:
      return orionobj_binary_read_orion(object, file, allocator);
    default:
      return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }
}

orionobj_result_t orionobj_binary_write_object(const orionobj_object_t* object,
                                                FILE* file) {
  if (!object || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  switch (object->header.format) {
    case ORIONOBJ_FORMAT_ORIONPP:
      return orionobj_binary_write_orionpp(object, file);
    case ORIONOBJ_FORMAT_ORION:
      return orionobj_binary_write_orion(object, file);
    default:
      return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }
}

orionobj_result_t orionobj_binary_read_orionpp(orionobj_object_t** object,
                                                FILE* file,
                                                const orionobj_allocator_t* allocator) {
  if (!object || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  orionobj_header_t header;
  orionobj_result_t result = orionobj_binary_read_header(file, &header);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  orionobj_object_t* obj;
  result = orionobj_object_create(&obj, header.format, header.architecture, allocator);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  obj->header = header;

  // Read string table
  orionobj_string_table_destroy(obj->strings);
  result = orionobj_string_table_read_binary(&obj->strings, file, header.string_table_size, allocator);
  if (ORIONOBJ_IS_ERROR(result)) {
    orionobj_object_destroy(obj);
    return result;
  }

  // TODO: Read sections and symbols (basic implementation)
  
  *object = obj;
  return ORIONOBJ_OK_PTR(obj);
}

orionobj_result_t orionobj_binary_write_orionpp(const orionobj_object_t* object,
                                                 FILE* file) {
  if (!object || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  // Update header with current counts
  orionobj_header_t header = object->header;
  header.string_table_size = orionobj_string_table_get_size(object->strings);
  header.section_count = object->section_count;
  header.symbol_count = object->symbol_count;

  orionobj_result_t result = orionobj_binary_write_header(file, &header);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  // Write string table
  result = orionobj_string_table_write_binary(object->strings, file);
  if (ORIONOBJ_IS_ERROR(result)) return result;

  // TODO: Write sections and symbols (basic implementation)

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_binary_read_orion(orionobj_object_t** object,
                                              FILE* file,
                                              const orionobj_allocator_t* allocator) {
  // For now, treat Orion format the same as OrionPP
  return orionobj_binary_read_orionpp(object, file, allocator);
}

orionobj_result_t orionobj_binary_write_orion(const orionobj_object_t* object,
                                               FILE* file) {
  // For now, treat Orion format the same as OrionPP
  return orionobj_binary_write_orionpp(object, file);
}

orionobj_result_t orionobj_binary_read_uint32(FILE* file, uint32_t* value, bool big_endian) {
  if (!file || !value) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  uint8_t bytes[4];
  if (fread(bytes, 1, 4, file) != 4) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  if (big_endian) {
    *value = ((uint32_t)bytes[0] << 24) |
             ((uint32_t)bytes[1] << 16) |
             ((uint32_t)bytes[2] << 8) |
             (uint32_t)bytes[3];
  } else {
    *value = (uint32_t)bytes[0] |
             ((uint32_t)bytes[1] << 8) |
             ((uint32_t)bytes[2] << 16) |
             ((uint32_t)bytes[3] << 24);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_binary_write_uint32(FILE* file, uint32_t value, bool big_endian) {
  if (!file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  uint8_t bytes[4];
  if (big_endian) {
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
  } else {
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
  }

  if (fwrite(bytes, 1, 4, file) != 4) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_binary_read_buffer(FILE* file, uint8_t* buffer, uint32_t size) {
  if (!file || !buffer) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (fread(buffer, 1, size, file) != size) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_binary_write_buffer(FILE* file, const uint8_t* buffer, uint32_t size) {
  if (!file || !buffer) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (fwrite(buffer, 1, size, file) != size) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return ORIONOBJ_OK_INT(0);
}