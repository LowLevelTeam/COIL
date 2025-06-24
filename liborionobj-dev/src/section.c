#include "orionobj/section.h"
#include <string.h>
#include <stdlib.h>

orionobj_result_t orionobj_section_create(orionobj_section_t** section,
                                           orionobj_section_type_t type,
                                           const char* name,
                                           orionobj_section_flags_t flags,
                                           const orionobj_allocator_t* allocator) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionobj_default_allocator;
  
  (void)name; // Suppress unused parameter warning

  orionobj_section_t* sec = allocator->malloc(sizeof(orionobj_section_t));
  if (!sec) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);

  memset(sec, 0, sizeof(orionobj_section_t));
  sec->allocator = *allocator;
  sec->type = type;
  sec->flags = flags;
  sec->alignment = 1; // Default alignment

  *section = sec;
  return ORIONOBJ_OK_PTR(sec);
}

void orionobj_section_destroy(orionobj_section_t* section) {
  if (!section) return;

  if (section->data) {
    section->allocator.free(section->data);
  }

  section->allocator.free(section);
}

orionobj_section_type_t orionobj_section_get_type(const orionobj_section_t* section) {
  return section ? section->type : ORIONOBJ_SECTION_TEXT;
}

orionobj_section_flags_t orionobj_section_get_flags(const orionobj_section_t* section) {
  return section ? section->flags : 0;
}

uint64_t orionobj_section_get_size(const orionobj_section_t* section) {
  return section ? section->size : 0;
}

uint64_t orionobj_section_get_alignment(const orionobj_section_t* section) {
  return section ? section->alignment : 1;
}

orionobj_result_t orionobj_section_set_flags(orionobj_section_t* section,
                                              orionobj_section_flags_t flags) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  section->flags = flags;
  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_section_set_alignment(orionobj_section_t* section,
                                                  uint64_t alignment) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_ARCH); // Not a power of 2
  }
  section->alignment = alignment;
  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_section_set_data(orionobj_section_t* section,
                                             const uint8_t* data,
                                             uint64_t size) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  // Free existing data
  if (section->data) {
    section->allocator.free(section->data);
    section->data = NULL;
    section->data_size = 0;
    section->data_capacity = 0;
  }

  if (size == 0) {
    section->size = 0;
    return ORIONOBJ_OK_INT(0);
  }

  // Allocate new data
  section->data = section->allocator.malloc(size);
  if (!section->data) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }

  if (data) {
    memcpy(section->data, data, size);
  } else {
    memset(section->data, 0, size);
  }

  section->data_size = size;
  section->data_capacity = size;
  section->size = size;

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_section_append_data(orionobj_section_t* section,
                                                const uint8_t* data,
                                                uint64_t size) {
  if (!section || !data) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (size == 0) return ORIONOBJ_OK_INT(0);

  uint64_t new_size = section->data_size + size;

  // Ensure capacity
  if (new_size > section->data_capacity) {
    uint64_t new_capacity = section->data_capacity;
    if (new_capacity == 0) new_capacity = 64;
    while (new_capacity < new_size) {
      new_capacity *= 2;
    }

    uint8_t* new_data = section->allocator.realloc(section->data, new_capacity);
    if (!new_data) {
      return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
    }

    section->data = new_data;
    section->data_capacity = new_capacity;
  }

  // Append data
  memcpy(section->data + section->data_size, data, size);
  section->data_size = new_size;
  section->size = new_size;

  return ORIONOBJ_OK_INT(0);
}

const uint8_t* orionobj_section_get_data(const orionobj_section_t* section) {
  return section ? section->data : NULL;
}

uint64_t orionobj_section_get_data_size(const orionobj_section_t* section) {
  return section ? section->data_size : 0;
}

orionobj_result_t orionobj_section_write_uint32(orionobj_section_t* section,
                                                 uint64_t offset,
                                                 uint32_t value,
                                                 bool big_endian) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (offset + 4 > section->data_size) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_ARCH);

  uint8_t* ptr = section->data + offset;
  if (big_endian) {
    ptr[0] = (value >> 24) & 0xFF;
    ptr[1] = (value >> 16) & 0xFF;
    ptr[2] = (value >> 8) & 0xFF;
    ptr[3] = value & 0xFF;
  } else {
    ptr[0] = value & 0xFF;
    ptr[1] = (value >> 8) & 0xFF;
    ptr[2] = (value >> 16) & 0xFF;
    ptr[3] = (value >> 24) & 0xFF;
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_section_read_uint32(const orionobj_section_t* section,
                                                uint64_t offset,
                                                uint32_t* value,
                                                bool big_endian) {
  if (!section || !value) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (offset + 4 > section->data_size) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_ARCH);

  const uint8_t* ptr = section->data + offset;
  if (big_endian) {
    *value = ((uint32_t)ptr[0] << 24) |
             ((uint32_t)ptr[1] << 16) |
             ((uint32_t)ptr[2] << 8) |
             (uint32_t)ptr[3];
  } else {
    *value = (uint32_t)ptr[0] |
             ((uint32_t)ptr[1] << 8) |
             ((uint32_t)ptr[2] << 16) |
             ((uint32_t)ptr[3] << 24);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_section_validate(const orionobj_section_t* section) {
  if (!section) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (section->type > ORIONOBJ_SECTION_ORION_META) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }
  if (section->alignment == 0 || (section->alignment & (section->alignment - 1)) != 0) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_ARCH);
  }
  return ORIONOBJ_OK_INT(0);
}

bool orionobj_section_is_executable(const orionobj_section_t* section) {
  return section && (section->flags & ORIONOBJ_SECTION_FLAG_EXEC) != 0;
}

bool orionobj_section_is_writable(const orionobj_section_t* section) {
  return section && (section->flags & ORIONOBJ_SECTION_FLAG_WRITE) != 0;
}

bool orionobj_section_is_allocatable(const orionobj_section_t* section) {
  return section && (section->flags & ORIONOBJ_SECTION_FLAG_ALLOC) != 0;
}

const char* orionobj_section_type_name(orionobj_section_type_t type) {
  switch (type) {
    case ORIONOBJ_SECTION_TEXT: return orionobj_section_type_names[ORIONOBJ_SECTION_TEXT];
    case ORIONOBJ_SECTION_DATA: return orionobj_section_type_names[ORIONOBJ_SECTION_DATA];
    case ORIONOBJ_SECTION_BSS: return orionobj_section_type_names[ORIONOBJ_SECTION_BSS];
    case ORIONOBJ_SECTION_RODATA: return orionobj_section_type_names[ORIONOBJ_SECTION_RODATA];
    case ORIONOBJ_SECTION_SYMTAB: return orionobj_section_type_names[ORIONOBJ_SECTION_SYMTAB];
    case ORIONOBJ_SECTION_STRTAB: return orionobj_section_type_names[ORIONOBJ_SECTION_STRTAB];
    case ORIONOBJ_SECTION_RELTAB: return orionobj_section_type_names[ORIONOBJ_SECTION_RELTAB];
    case ORIONOBJ_SECTION_DEBUG: return orionobj_section_type_names[ORIONOBJ_SECTION_DEBUG];
    case ORIONOBJ_SECTION_ORION_META: return orionobj_section_type_names[ORIONOBJ_SECTION_ORION_META];
    default: return "unknown";
  }
}