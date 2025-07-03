/**
* @file header.h
* @brief Orion++ file format header validation and initalization
*/

#ifndef ORIONPP_HEADER_H
#define ORIONPP_HEADER_H

#include <orionpp/error.h>

#ifndef ORIONPP_MAGIC
#define ORIONPP_MAGIC { 'O', 'P', 'P', 0xD4 }
#define ORIONPP_MAGIC0 'O'
#define ORIONPP_MAGIC1 'P'
#define ORIONPP_MAGIC2 'P'
#define ORIONPP_MAGIC3 0xD4
#endif

typedef uint64_t orionpp_feature_t;

enum orionpp_feature {
  ORIONPP_FEATURE_CSTL = (1 << 0),   // Compile-time standard library only
  ORIONPP_FEATURE_STL = (1 << 1),    // Runtime Standard library
  ORIONPP_FEATURE_ORION = (1 << 2)   // Embedded orion
  // bits 4-63 reserved
};

typedef struct orionpp_header {
  orionpp_byte_t magic0;
  orionpp_byte_t magic1;
  orionpp_byte_t magic2;
  orionpp_byte_t magic3;
  orionpp_byte_t reserved;
  orionpp_byte_t major;
  orionpp_byte_t minor;
  orionpp_byte_t patch;
  orionpp_feature_t features;
  uint64_t typetab;
  uint64_t strtab;
  uint64_t datatab;
  uint64_t codetab;
  uint64_t extrntab;
  uint64_t intrntab;
} orionpp_header_t;

/**
* @brief Initialize header with default values
* @param header Header structure to initialize
* @return Error code
*/
orionpp_error_t orionpp_header_init(orionpp_header_t *header);

/**
* @brief Validate header format and version compatibility
* @param header Header structure to validate
* @return Error code
*/
orionpp_error_t orionpp_header_validate(const orionpp_header_t *header);

#endif // ORIONPP_HEADER_H