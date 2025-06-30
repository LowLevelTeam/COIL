#include "orionpp/header.h"
#include "orionpp/error.h"
#include <string.h>

const orionpp_byte_t version_major = 1;
const orionpp_byte_t version_minor = 0;
const orionpp_byte_t version_patch = 0;

orionpp_error_t orionpp_header_init(orionpp_header_t *header) {
  if (!header) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  const orionpp_byte_t magic[] = ORIONPP_MAGIC;
  
  header->magic0 = magic[0];
  header->magic1 = magic[1];
  header->magic2 = magic[2];
  header->magic3 = magic[3];
  header->features = 0;
  header->major = version_major;
  header->minor = version_minor;
  header->patch = version_patch;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_header_validate(const orionpp_header_t *header) {
  if (!header) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  const orionpp_byte_t magic[] = ORIONPP_MAGIC;
  
  if (header->magic0 != magic[0] || header->magic1 != magic[1] || header->magic2 != magic[2] || header->magic3 != magic[3]) {
    return ORIONPP_ERROR_INVALID_MAGIC;
  }

  if (header->major > version_major || header->minor > version_minor) {
    // patch not checked as it changes bugs which are internal
    return ORIONPP_ERROR_INVALID_VERSION;
  }
  
  return ORIONPP_ERROR_GOOD;
}

int orionpp_header_has_feature(const orionpp_header_t *header, orionpp_feature_t feature) {
  if (!header) {
    return 0;
  }
  
  return (header->features & (orionpp_byte_t)(feature & 0xFF)) != 0;
}