#ifndef ORIONPP_HEADER_H
#define ORIONPP_HEADER_H

#include <orionpp/error.h>

/**
 * @file header.h
 * @brief Orion++ file format header validation and management
 * 
 * Handles file format headers including magic number validation,
 * version checking, and feature flag management.
 */

// Magic number and version constants
#ifndef ORIONPP_MAGIC_LEN
#define ORIONPP_MAGIC_LEN 4
#endif

#ifndef ORIONPP_MAGIC
#define ORIONPP_MAGIC { 'O', 'P', 'P', 0xD4 }
#endif

typedef unsigned char orionpp_byte_t;
typedef orionpp_byte_t orionpp_feature_t;

enum orionpp_feature {
  ORIONPP_FEATURE_ABI,    // Application Binary Interface Calling Convention
  ORIONPP_FEATURE_CSTL,   // Compile-time standard library only
  ORIONPP_FEATURE_STL,    // Runtime Standard library
  ORIONPP_FEATURE_ORION   // Embedded orion
};

typedef struct orionpp_header {
  orionpp_byte_t magic0;
  orionpp_byte_t magic1;
  orionpp_byte_t magic2;
  orionpp_byte_t magic3;
  orionpp_byte_t features;
  orionpp_byte_t major;
  orionpp_byte_t minor;
  orionpp_byte_t patch;
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

/**
 * @brief Check if header has specific feature enabled
 * @param header Header structure to check
 * @param feature Feature flag to check
 * @return Non-zero if feature is present, 0 otherwise
 */
int orionpp_header_has_feature(const orionpp_header_t *header, orionpp_feature_t feature);

#endif // ORIONPP_HEADER_H