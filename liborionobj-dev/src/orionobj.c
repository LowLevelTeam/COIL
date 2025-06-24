#include "orionobj/orionobj.h"
#include <stdio.h>
#include <stdbool.h>

orionobj_result_t orionobj_init(void) {
  // Currently no global initialization needed
  return ORIONOBJ_OK_INT(0);
}

void orionobj_cleanup(void) {
  // Currently no global cleanup needed
}

void orionobj_get_version(uint16_t* major, uint16_t* minor, uint32_t* patch) {
  if (major) *major = ORIONOBJ_VERSION_MAJOR;
  if (minor) *minor = ORIONOBJ_VERSION_MINOR;
  if (patch) *patch = ORIONOBJ_VERSION_PATCH;
}

const char* orionobj_get_version_string(void) {
  static char version_string[32];
  static bool initialized = false;
  
  if (!initialized) {
    int ret = snprintf(version_string, sizeof(version_string), "%u.%u.%u",
                       (unsigned int)ORIONOBJ_VERSION_MAJOR, 
                       (unsigned int)ORIONOBJ_VERSION_MINOR, 
                       (unsigned int)ORIONOBJ_VERSION_PATCH);
    (void)ret; // Suppress unused variable warning
    initialized = true;
  }
  
  return version_string;
}