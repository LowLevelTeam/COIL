#include "orionpp/core/result.h"

const char* orionpp_error_string(orionpp_error_t error) {
  switch (error) {
    case ORIONPP_OK: return "Success";
    case ORIONPP_ERROR_NULL_POINTER: return "Null pointer";
    case ORIONPP_ERROR_OUT_OF_MEMORY: return "Out of memory";
    case ORIONPP_ERROR_INVALID_FEATURE: return "Invalid feature";
    case ORIONPP_ERROR_INVALID_INSTRUCTION: return "Invalid instruction";
    case ORIONPP_ERROR_IO_ERROR: return "I/O error";
    case ORIONPP_ERROR_PARSE_ERROR: return "Parse error";
    case ORIONPP_ERROR_VERSION_MISMATCH: return "Version mismatch";
    case ORIONPP_ERROR_CORRUPT_DATA: return "Corrupt data";
    case ORIONPP_ERROR_NOT_FOUND: return "Not found";
    case ORIONPP_ERROR_INVALID_FORMAT: return "Invalid format";
    case ORIONPP_ERROR_INVALID_ARCH: return "Invalid architecture";
    default: return "Unknown error";
  }
}