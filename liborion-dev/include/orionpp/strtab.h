/**
 * @file strtab.h
 * @brief Orion++ String Table - Efficient string storage and lookup
 */

#ifndef ORIONPP_STRTAB_H
#define ORIONPP_STRTAB_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

typedef struct orionpp_strtab {
  orionpp_offset_t size;  // Total size of string data
  orionpp_offset_t count; // Number of strings
  char *data;             // Null-terminated string data
} orionpp_strtab_t;

orionpp_error_t orionpp_strtab_init(orionpp_strtab_t *table);
void orionpp_strtab_free(orionpp_strtab_t *table);
orionpp_offset_t orionpp_strtab_add(orionpp_strtab_t *table, const char *str);
const char *orionpp_strtab_get(const orionpp_strtab_t *table, orionpp_offset_t offset);
orionpp_error_t orionpp_strtab_validate(const orionpp_strtab_t *table);

#endif // ORIONPP_STRTAB_H