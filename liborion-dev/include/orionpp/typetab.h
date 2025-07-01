/**
* @file strtab.h
* @brief Orion++ Type Table
*/

#ifndef ORIONPP_TYPETAB_H
#define ORIONPP_TYPETAB_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>

// -------------------------------- Type system -------------------------------- //
enum orionpp_type {
  ORIONPP_TYPE_PRIM, // primitive / fixed-width
  ORIONPP_TYPE_QUAL, // qualifers
  ORIONPP_TYPE_MACH, // machine
  ORIONPP_TYPE_COMP, // composite
};

enum orionpp_type_int {
  ORIONPP_TYPE_PRIM_I8, ORIONPP_TYPE_PRIM_I16, ORIONPP_TYPE_PRIM_I32, ORIONPP_TYPE_PRIM_I64,
  ORIONPP_TYPE_PRIM_U8, ORIONPP_TYPE_PRIM_U16, ORIONPP_TYPE_PRIM_U32, ORIONPP_TYPE_PRIM_U64,
};

enum orionpp_type_qual {
  ORIONPP_TYPE_QUAL_CONST,
  ORIONPP_TYPE_QUAL_VOLATILE,
  ORIONPP_TYPE_QUAL_PTR,
};

enum orionpp_type_mach {
  ORIONPP_TYPE_MACH_PTR,
};

enum orionpp_type_comp {
  ORIONPP_TYPE_COMP_PACK, // structure
  ORIONPP_TYPE_COMP_STRUCT, // padded structure
  ORIONPP_TYPE_COMP_UNION, // overlapping structure
};

// type table
// [INBUILT UNT16_MAX]
// [USER UNT32_MAX]
typedef uint32_t orionpp_type_t; // reference into the type table

size_t orionpp_type_sizeof(orionpp_type_t type);

#endif // ORIONPP_TYPETAB_H