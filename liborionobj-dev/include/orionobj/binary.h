#ifndef ORIONOBJ_BINARY_H
#define ORIONOBJ_BINARY_H

#include "orionobj/types.h"
#include "orionobj/object.h"

// Binary format detection
orionobj_result_t orionobj_binary_detect_format(FILE* file, orionobj_format_t* format);

// Header validation and reading
orionobj_result_t orionobj_binary_read_header(FILE* file, orionobj_header_t* header);
orionobj_result_t orionobj_binary_write_header(FILE* file, const orionobj_header_t* header);

// Version compatibility checking
bool orionobj_binary_is_version_compatible(uint16_t file_major, uint16_t file_minor,
                                            uint16_t lib_major, uint16_t lib_minor);

// Generic binary I/O operations
orionobj_result_t orionobj_binary_read_object(orionobj_object_t** object,
                                               FILE* file,
                                               const orionobj_allocator_t* allocator);

orionobj_result_t orionobj_binary_write_object(const orionobj_object_t* object,
                                                FILE* file);

// OrionPP format handlers
orionobj_result_t orionobj_binary_read_orionpp(orionobj_object_t** object,
                                                FILE* file,
                                                const orionobj_allocator_t* allocator);

orionobj_result_t orionobj_binary_write_orionpp(const orionobj_object_t* object,
                                                 FILE* file);

// Orion format handlers
orionobj_result_t orionobj_binary_read_orion(orionobj_object_t** object,
                                              FILE* file,
                                              const orionobj_allocator_t* allocator);

orionobj_result_t orionobj_binary_write_orion(const orionobj_object_t* object,
                                               FILE* file);

// Orion-specific binary format structures
typedef struct {
  uint32_t magic;                   // ORIONOBJ_MAGIC_ORION
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t version_patch;
  orionobj_arch_t architecture;     // Orion variant
  uint32_t flags;                   // Orion-specific flags
  uint32_t isa_metadata_size;       // Size of ISA metadata
  uint32_t abi_info_size;           // Size of ABI information
  uint32_t feature_flags;           // Enabled Orion features
  uint32_t reserved[4];             // Reserved for future use
} orionobj_orion_header_t;

// Orion feature flags
#define ORIONOBJ_ORION_FEATURE_VECTORIZATION    (1U << 0)
#define ORIONOBJ_ORION_FEATURE_SIMD             (1U << 1)
#define ORIONOBJ_ORION_FEATURE_ATOMIC           (1U << 2)
#define ORIONOBJ_ORION_FEATURE_FLOATING_POINT   (1U << 3)

// Binary I/O utilities
orionobj_result_t orionobj_binary_read_uint32(FILE* file, uint32_t* value, bool big_endian);
orionobj_result_t orionobj_binary_write_uint32(FILE* file, uint32_t value, bool big_endian);

orionobj_result_t orionobj_binary_read_buffer(FILE* file, uint8_t* buffer, uint32_t size);
orionobj_result_t orionobj_binary_write_buffer(FILE* file, const uint8_t* buffer, uint32_t size);

// Binary validation
orionobj_result_t orionobj_binary_validate_header(const orionobj_header_t* header);

#endif // ORIONOBJ_BINARY_H