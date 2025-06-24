#ifndef ORIONPP_CORE_RESULT_H
#define ORIONPP_CORE_RESULT_H

#include <stdint.h>
#include <stdbool.h>

// Error codes
typedef enum {
  ORIONPP_OK = 0,
  ORIONPP_ERROR_NULL_POINTER = -1,
  ORIONPP_ERROR_OUT_OF_MEMORY = -2,
  ORIONPP_ERROR_INVALID_FEATURE = -3,
  ORIONPP_ERROR_INVALID_INSTRUCTION = -4,
  ORIONPP_ERROR_IO_ERROR = -5,
  ORIONPP_ERROR_PARSE_ERROR = -6,
  ORIONPP_ERROR_VERSION_MISMATCH = -7,
  ORIONPP_ERROR_CORRUPT_DATA = -8,
  ORIONPP_ERROR_NOT_FOUND = -9,
  ORIONPP_ERROR_INVALID_FORMAT = -10,
  ORIONPP_ERROR_INVALID_ARCH = -11,
} orionpp_error_t;

// Result type for functions that can fail
typedef struct {
  orionpp_error_t error;
  union {
    void* ptr;
    int64_t integer;
    bool boolean;
  } value;
} orionpp_result_t;

// Macros for result handling
#define ORIONPP_OK_PTR(__ptr) ((orionpp_result_t){.error = ORIONPP_OK, .value.ptr = (void*)(__ptr)})
#define ORIONPP_OK_INT(val) ((orionpp_result_t){.error = ORIONPP_OK, .value.integer = (val)})
#define ORIONPP_OK_BOOL(val) ((orionpp_result_t){.error = ORIONPP_OK, .value.boolean = (val)})
#define ORIONPP_ERROR(err) ((orionpp_result_t){.error = (err), .value.ptr = NULL})

#define ORIONPP_IS_OK(result) ((result).error == ORIONPP_OK)
#define ORIONPP_IS_ERROR(result) ((result).error != ORIONPP_OK)

// Error string conversion
const char* orionpp_error_string(orionpp_error_t error);

#endif // ORIONPP_CORE_RESULT_H