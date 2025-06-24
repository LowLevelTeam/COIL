#ifndef ORIONPP_H
#define ORIONPP_H

// Core systems
#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"

// String management
#include "orionpp/string/strtab.h"

// Value system
#include "orionpp/value/value.h"
#include "orionpp/value/parser.h"

// Instruction system
#include "orionpp/instr/instr.h"
#include "orionpp/instr/builder.h"

// Module system
#include "orionpp/module/module.h"

// Output systems
#include "orionpp/output/text.h"
#include "orionpp/output/binary.h"
#include "orionpp/output/orionobj.h"

// Library initialization and cleanup
orionpp_result_t orionpp_init(void);
void orionpp_cleanup(void);

// Version information
void orionpp_get_version(uint16_t* major, uint16_t* minor, uint32_t* patch);
const char* orionpp_get_version_string(void);

#endif // ORIONPP_H