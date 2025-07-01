#include "orionpp/error.h"

/**
 * @file error.c
 * @brief Error handling implementation
 * 
 * Provides human-readable error messages for all library error codes.
 */

static const char *error_messages[] = {
  "GOOD",
  "INVALID_ARGUMENT",
  "NOMEM",
  "BUFFER_OVERFLOW",
  "INVALID_INSTRUCTION",
  "INVALID_TYPE",
  "INVALID_VALUE",
  "INVALID_MAGIC",
  "INVALID_VERSION",
  "UNSUPPORTED_FEATURE",
  "UNKNOWN"
};

const char *orionpp_strerr(orionpp_error_t err) {
  if (err < 0 || err >= ORIONPP_ERROR_UNKNOWN) {
    return error_messages[ORIONPP_ERROR_UNKNOWN];
  }
  return error_messages[err];
}