#ifndef __ORIONPP_HEADER
#define __ORIONPP_HEADER

#include <orionpp/error.h>

// -------------------------------- DEFS -------------------------------- //
#ifndef ORIONPP_MAGIC_LEN
  #define ORIONPP_MAGIC_LEN 4
#endif
#ifndef ORIONPP_MAGIC
  #define ORIONPP_MAGIC { 'O', 'P', 'P', 0xD4 } // Orion Plus Plus 0xD4 (i play the london... i am sorry)
#endif

// -------------------------------- ENUMS -------------------------------- //

enum orionpp_feature {
  // For seamless updates to the standard in the future
  ORIONPP_FEATURE_ABI, // ABICC (Application Binary Inteface Calling Convention)
  ORIONPP_FEATURE_CSTL, // Copmile time standard library only
  ORIONPP_FEATURE_STL, // Runtime Standard library
  ORIONPP_FEATURE_ORION, // embedded orion
};

// -------------------------------- TYPES -------------------------------- //

typedef unsigned char orionpp_byte_t;
typedef orionpp_byte_t orionpp_feature_t;

// -------------------------------- STRUCTS -------------------------------- //

typedef struct orionpp_header {
  orionpp_byte_t magic0;
  orionpp_byte_t magic1;
  orionpp_byte_t magic2;
  orionpp_byte_t magic3;
  orionpp_byte_t features;
  orionpp_byte_t major;
  orionpp_byte_t minor;
  orionpp_byte_t patch;
} orionpp_header_t;

// -------------------------------- FUNCS -------------------------------- //

orionpp_error_t orionpp_header_init(orionpp_header_t *header);
orionpp_error_t orionpp_header_validate(const orionpp_header_t *header);
int orionpp_header_has_feature(const orionpp_header_t *header, orionpp_feature_t feature);

#endif // __ORIONPP_HEADER