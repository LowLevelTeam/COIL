/**
* @file trntab.h
* @brief Orion++ External Imports and Internal Exports Definition Table
*/

#ifndef ORIONPP_TERNTAB_H
#define ORIONPP_TERNTAB_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

enum orionpp_reference_type {
  ORIONPP_REFTYPE_TYPE = 0x00,        // type definition
  ORIONPP_REFTYPE_ABI = 0x01          // ABI definition
  ORIONPP_REFTYPE_FUNCTION = 0x01,    // function
  ORIONPP_REFTYPE_VARIABLE = 0x02,      // object
  ORIONPP_REFTYPE_CONSTANT = 0x03,    // compile time constant initalized elsewhere
};

typedef struct orionpp_extern_entry {
  uint32_t name_offset;              // Required name in string table
  uint8_t identifier_type;           // Type of external reference
  uint64_t identifier;               // local Type/ABI/target identifier to assign this to
  uint32_t info_size;                // Size of additional information
  uint8_t info[];                    // Type-specific information
} orionpp_extern_entry_t;

typedef struct orionpp_intern_entry {
  uint32_t name_offset;              // Required name in string table
  uint8_t identifier_type;           // Type of internal reference
  uint64_t identifier;               // local Type/ABI/target identifier to extenarlize
  uint32_t info_size;                // Size of additional information
  uint8_t info[];                    // Type-specific information
} orionpp_intern_entry_t;

typedef struct orionpp_extrntab {
  uint32_t entry_count;
  orionpp_extern_entry_t *entries;
} orionpp_extrntab_t;

typedef struct orionpp_intrntab {
  uint32_t entry_count;
  orionpp_internal_entry_t *entries;
} orionpp_intrntab_t;

orionpp_error_t orionpp_extrntab_init(orionpp_extrntab_t *table);
void orionpp_extrntab_free(orionpp_extrntab_t *table);
orionpp_error_t orionpp_extrntab_add_entry(orionpp_extrntab_t *table, const orionpp_extern_entry_t *entry);
const orionpp_extern_entry_t *orionpp_extrntab_lookup(const orionpp_extrntab_t *table, const char *name);

orionpp_error_t orionpp_intrntab_init(orionpp_intrntab_t *table);
void orionpp_intrntab_free(orionpp_intrntab_t *table);
orionpp_error_t orionpp_intrntab_add_entry(orionpp_intrntab_t *table, const orionpp_extern_entry_t *entry);
const orionpp_intern_entry_t *orionpp_intrntab_lookup(const orionpp_intrntab_t *table, const char *name);

#endif // ORIONPP_TERNTAB_H