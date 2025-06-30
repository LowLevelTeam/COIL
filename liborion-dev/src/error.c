#include "orionpp/error.h"

static const char *error_messages[] = {
  "Success",
  "Invalid argument",
  "Out of memory",
  "Buffer overflow",
  "File error",
  "Invalid instruction",
  "Invalid type",
  "Arena full",
  "Arena not initialized",
  "Invalid magic number",
  "Unsupported language version",
  "Unsupported feature",
  "Unknown error"
};

const char *orionpp_strerr(orionpp_error_t err) {
  if (err < 0 || err >= ORIONPP_ERROR_UNKNOWN) {
    return error_messages[ORIONPP_ERROR_UNKNOWN];
  }
  return error_messages[err];
}