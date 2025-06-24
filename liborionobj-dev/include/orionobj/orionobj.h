#ifndef ORIONOBJ_H
#define ORIONOBJ_H

// Main header that includes all necessary OrionObj components
#include "orionobj/types.h"
#include "orionobj/object.h"
#include "orionobj/section.h"
#include "orionobj/symbol.h"
#include "orionobj/strtab.h"
#include "orionobj/binary.h"

// Library initialization and cleanup
orionobj_result_t orionobj_init(void);
void orionobj_cleanup(void);

// Version information
void orionobj_get_version(uint16_t* major, uint16_t* minor, uint32_t* patch);
const char* orionobj_get_version_string(void);

#endif // ORIONOBJ_H