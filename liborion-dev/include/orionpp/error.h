/**
* @file error.h
* @brief Error code definitions and utilities
*/

#ifndef ORIONPP_ERROR_H
#define ORIONPP_ERROR_H

#include <stdint.h>

typedef uint8_t orionpp_byte_t;
typedef int orionpp_error_t;

enum orionpp_error {
  ORIONPP_ERROR_GOOD = 0,
  ORIONPP_ERROR_INVALID_ARGUMENT,
  ORIONPP_ERROR_NOMEM,
  ORIONPP_ERROR_BUFFER_OVERFLOW,
  ORIONPP_ERROR_INVALID_INSTRUCTION,
  ORIONPP_ERROR_INVALID_TYPE,
  ORIONPP_ERROR_INVALID_VALUE,
  ORIONPP_ERROR_INVALID_MAGIC,
  ORIONPP_ERROR_INVALID_VERSION,
  ORIONPP_ERROR_UNSUPPORTED_FEATURE,
  ORIONPP_ERROR_UNKNOWN
};

/**
 * @brief Get human-readable error message
 * @param err Error code
 * @return Null-terminated error message string
 */
const char *orionpp_strerr(orionpp_error_t err);

#endif // ORIONPP_ERROR_H