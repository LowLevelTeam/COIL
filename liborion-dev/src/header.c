/**
* @file header.c
* @brief Header validation and initalization implementation
*/

#include "orionpp/header.h"


// Current library version
static const orionpp_byte_t version_major = 1;
static const orionpp_byte_t version_minor = 0;
static const orionpp_byte_t version_patch = 0;

orionpp_error_t orionpp_header_init(orionpp_header_t *header) {
  if (!header) return ORIONPP_ERROR_INVALID_ARGUMENT;
  
  header->magic0 = ORIONPP_MAGIC0;
  header->magic1 = ORIONPP_MAGIC1;
  header->magic2 = ORIONPP_MAGIC2;
  header->magic3 = ORIONPP_MAGIC3;
  header->major = version_major;
  header->minor = version_minor;
  header->patch = version_patch;
  header->features = 0;
  header->typetab = 0;
  header->abitab = 0;
  header->extrntab = 0
  header->intrntab = 0;
  header->strtab = 0;
  header->code = 0;

  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_header_validate(const orionpp_header_t *header) {
  if (!header) return ORIONPP_ERROR_INVALID_ARGUMENT;
  
  const orionpp_byte_t magic[] = ORIONPP_MAGIC;
  
  // Validate magic number
  if (header->magic0 != ORIONPP_MAGIC0 || header->magic1 != ORIONPP_MAGIC1 || header->magic2 != ORIONPP_MAGIC2 || header->magic3 != ORIONPP_MAGIC3) {
    return ORIONPP_ERROR_INVALID_MAGIC;
  }
  
  // Validate version compatibility (patch version ignored)
  if (header->major > version_major || (header->major == version_major && header->minor > version_minor)) {
    return ORIONPP_ERROR_INVALID_VERSION;
  }
  
  return ORIONPP_ERROR_GOOD;
}
