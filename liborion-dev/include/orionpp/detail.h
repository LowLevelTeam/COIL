/**
* @file detail.h
* @brief Orion++ internal detail
*
* Functionality used internally by the library like global types and error handling.
*/

#ifndef ORIONPP_DETAIL_H
#define ORIONPP_DETAIL_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

typedef uint16_t orionpp_abi_t; // each module should only need 65535 ABIs
typedef uint16_t orionpp_varref_t; // each scope should only need 65535 variables
typedef uint16_t orionpp_labelref_t; // each scope should only need 65535 labels
typedef uint32_t orionpp_typeref_t; // each module should only need 2^32 types
typedef uint32_t orionpp_dataref_t; // each module should only need 2^32 data references
typedef uint32_t orionpp_funcref_t; // each module should only need 2^32 function references
typedef uint32_t orionpp_nameref_t; // each module should only need 2^32 name references

enum orionpp_abi {
  ORIONPP_ABI_NONE = 0,
  ORIONPP_ABI_CABI = 1, // C standard calling convention
};

#endif // ORIONPP_DETAIL_H