#include "orionpp/orionpp.h"
#include "orionobj/orionobj.h"
#include <stdio.h>
#include <stdbool.h>

orionpp_result_t orionpp_init(void) {
  // Initialize OrionObj library first
  orionobj_result_t obj_result = orionobj_init();
  if (ORIONOBJ_IS_ERROR(obj_result)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Currently no additional global initialization needed for OrionPP
  return ORIONPP_OK_INT(0);
}

void orionpp_cleanup(void) {
  // Clean up OrionObj library
  orionobj_cleanup();
  
  // Currently no additional global cleanup needed for OrionPP
}

void orionpp_get_version(uint16_t* major, uint16_t* minor, uint32_t* patch) {
  if (major) *major = ORIONPP_VERSION_MAJOR;
  if (minor) *minor = ORIONPP_VERSION_MINOR;
  if (patch) *patch = ORIONPP_VERSION_PATCH;
}

const char* orionpp_get_version_string(void) {
  static char version_string[32];
  static bool initialized = false;
  
  if (!initialized) {
    int ret = snprintf(version_string, sizeof(version_string), "%u.%u.%u",
                       (unsigned int)ORIONPP_VERSION_MAJOR, 
                       (unsigned int)ORIONPP_VERSION_MINOR, 
                       (unsigned int)ORIONPP_VERSION_PATCH);
    (void)ret; // Suppress unused variable warning
    initialized = true;
  }
  
  return version_string;
}