#ifndef ORIONOBJ_SECTION_H
#define ORIONOBJ_SECTION_H

#include "orionobj/types.h"

// Section structure
struct orionobj_section {
  orionobj_section_type_t type;     // Section type
  uint32_t name_offset;             // Offset in string table
  orionobj_section_flags_t flags;   // Section flags
  uint64_t virtual_address;         // Virtual address when loaded
  uint64_t file_offset;             // Offset in file
  uint64_t size;                    // Section size
  uint64_t alignment;               // Required alignment
  
  // Section data
  uint8_t* data;
  uint64_t data_size;
  uint64_t data_capacity;
  
  // Memory management
  orionobj_allocator_t allocator;
};

// Section creation and destruction
orionobj_result_t orionobj_section_create(orionobj_section_t** section,
                                           orionobj_section_type_t type,
                                           const char* name,
                                           orionobj_section_flags_t flags,
                                           const orionobj_allocator_t* allocator);

void orionobj_section_destroy(orionobj_section_t* section);

// Section properties
orionobj_section_type_t orionobj_section_get_type(const orionobj_section_t* section);
orionobj_section_flags_t orionobj_section_get_flags(const orionobj_section_t* section);
uint64_t orionobj_section_get_size(const orionobj_section_t* section);
uint64_t orionobj_section_get_alignment(const orionobj_section_t* section);

orionobj_result_t orionobj_section_set_flags(orionobj_section_t* section,
                                              orionobj_section_flags_t flags);
orionobj_result_t orionobj_section_set_alignment(orionobj_section_t* section,
                                                  uint64_t alignment);

// Section data management
orionobj_result_t orionobj_section_set_data(orionobj_section_t* section,
                                             const uint8_t* data,
                                             uint64_t size);

orionobj_result_t orionobj_section_append_data(orionobj_section_t* section,
                                                const uint8_t* data,
                                                uint64_t size);

const uint8_t* orionobj_section_get_data(const orionobj_section_t* section);
uint64_t orionobj_section_get_data_size(const orionobj_section_t* section);

// Data type convenience functions
orionobj_result_t orionobj_section_write_uint32(orionobj_section_t* section,
                                                 uint64_t offset,
                                                 uint32_t value,
                                                 bool big_endian);

orionobj_result_t orionobj_section_read_uint32(const orionobj_section_t* section,
                                                uint64_t offset,
                                                uint32_t* value,
                                                bool big_endian);

// Section validation
orionobj_result_t orionobj_section_validate(const orionobj_section_t* section);

// Section utilities
bool orionobj_section_is_executable(const orionobj_section_t* section);
bool orionobj_section_is_writable(const orionobj_section_t* section);
bool orionobj_section_is_allocatable(const orionobj_section_t* section);

const char* orionobj_section_type_name(orionobj_section_type_t type);

#endif // ORIONOBJ_SECTION_H