#ifndef ORIONOBJ_TYPES_H
#define ORIONOBJ_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Version information
#define ORIONOBJ_VERSION_MAJOR 1
#define ORIONOBJ_VERSION_MINOR 0
#define ORIONOBJ_VERSION_PATCH 0

// Magic numbers for different object formats
#define ORIONOBJ_MAGIC_ORIONPP    0x4F50504E  // "OPPN" (OrionPP Native)
#define ORIONOBJ_MAGIC_ORION      0x4F524E58  // "ORNX" (Orion Extended)

// Forward declarations
typedef struct orionobj_object       orionobj_object_t;
typedef struct orionobj_section      orionobj_section_t;
typedef struct orionobj_symbol       orionobj_symbol_t;
typedef struct orionobj_string_table orionobj_string_table_t;

// Object format types
typedef enum {
  ORIONOBJ_FORMAT_UNKNOWN = 0,
  ORIONOBJ_FORMAT_ORIONPP,      // OrionPP binary format
  ORIONOBJ_FORMAT_ORION,        // Orion variations (x86, arm, riscv, etc.)
  ORIONOBJ_FORMAT_MAX = 255
} orionobj_format_t;

// Target architecture types
typedef enum {
  ORIONOBJ_ARCH_UNKNOWN = 0,
  
  // Orion variations
  ORIONOBJ_ARCH_ORION_X86,
  ORIONOBJ_ARCH_ORION_ARM,
  ORIONOBJ_ARCH_ORION_RISCV,
  
  // Native architectures (for cross-compilation support)
  ORIONOBJ_ARCH_X86_32,
  ORIONOBJ_ARCH_X86_64,
  ORIONOBJ_ARCH_ARM_32,
  ORIONOBJ_ARCH_ARM_64,
  ORIONOBJ_ARCH_RISCV_32,
  ORIONOBJ_ARCH_RISCV_64,
  
  ORIONOBJ_ARCH_MAX = 0xFFFF
} orionobj_arch_t;

// Section types
typedef enum {
  ORIONOBJ_SECTION_TEXT = 0,      // Executable code
  ORIONOBJ_SECTION_DATA,          // Initialized data
  ORIONOBJ_SECTION_BSS,           // Uninitialized data
  ORIONOBJ_SECTION_RODATA,        // Read-only data
  ORIONOBJ_SECTION_SYMTAB,        // Symbol table
  ORIONOBJ_SECTION_STRTAB,        // String table
  ORIONOBJ_SECTION_RELTAB,        // Relocation table
  ORIONOBJ_SECTION_DEBUG,         // Debug information
  ORIONOBJ_SECTION_ORION_META,    // Orion metadata
  ORIONOBJ_SECTION_MAX = 0xFF
} orionobj_section_type_t;

// Section flags
typedef enum {
  ORIONOBJ_SECTION_FLAG_WRITE = 1 << 0,   // Writable
  ORIONOBJ_SECTION_FLAG_ALLOC = 1 << 1,   // Allocatable
  ORIONOBJ_SECTION_FLAG_EXEC  = 1 << 2,   // Executable
  ORIONOBJ_SECTION_FLAG_MERGE = 1 << 3,   // Mergeable
  ORIONOBJ_SECTION_FLAG_TLS   = 1 << 4,   // Thread-local storage
} orionobj_section_flags_t;

// Symbol binding
typedef enum {
  ORIONOBJ_SYMBOL_LOCAL = 0,
  ORIONOBJ_SYMBOL_GLOBAL,
  ORIONOBJ_SYMBOL_WEAK,
  ORIONOBJ_SYMBOL_BINDING_MAX = 0xFF
} orionobj_symbol_binding_t;

// Symbol type
typedef enum {
  ORIONOBJ_SYMBOL_NONE = 0,
  ORIONOBJ_SYMBOL_OBJECT,
  ORIONOBJ_SYMBOL_FUNC,
  ORIONOBJ_SYMBOL_SECTION,
  ORIONOBJ_SYMBOL_FILE,
  ORIONOBJ_SYMBOL_TYPE_MAX = 0xFF
} orionobj_symbol_type_t;

// Error codes
typedef enum {
  ORIONOBJ_OK = 0,
  ORIONOBJ_ERROR_NULL_POINTER = -1,
  ORIONOBJ_ERROR_OUT_OF_MEMORY = -2,
  ORIONOBJ_ERROR_INVALID_FORMAT = -3,
  ORIONOBJ_ERROR_INVALID_ARCH = -4,
  ORIONOBJ_ERROR_IO_ERROR = -5,
  ORIONOBJ_ERROR_PARSE_ERROR = -6,
  ORIONOBJ_ERROR_CORRUPT_DATA = -7,
  ORIONOBJ_ERROR_NOT_FOUND = -8,
} orionobj_error_t;

// Result type for functions that can fail
typedef struct {
  orionobj_error_t error;
  union {
    void* ptr;
    int64_t integer;
    bool boolean;
  } value;
} orionobj_result_t;

// Macros for result handling
#define ORIONOBJ_OK_PTR(__ptr) ((orionobj_result_t){.error = ORIONOBJ_OK, .value.ptr = (void*)(__ptr)})
#define ORIONOBJ_OK_INT(val) ((orionobj_result_t){.error = ORIONOBJ_OK, .value.integer = (val)})
#define ORIONOBJ_OK_BOOL(val) ((orionobj_result_t){.error = ORIONOBJ_OK, .value.boolean = (val)})
#define ORIONOBJ_ERROR(err) ((orionobj_result_t){.error = (err), .value.ptr = NULL})

#define ORIONOBJ_IS_OK(result) ((result).error == ORIONOBJ_OK)
#define ORIONOBJ_IS_ERROR(result) ((result).error != ORIONOBJ_OK)

// Memory allocation function pointers
typedef void* (*orionobj_malloc_fn)(size_t size);
typedef void* (*orionobj_realloc_fn)(void* ptr, size_t size);
typedef void (*orionobj_free_fn)(void* ptr);

typedef struct {
  orionobj_malloc_fn malloc;
  orionobj_realloc_fn realloc;
  orionobj_free_fn free;
} orionobj_allocator_t;

// Default allocator
extern const orionobj_allocator_t orionobj_default_allocator;

// Object header structure
typedef struct {
  uint32_t magic;                 // Format magic number
  orionobj_format_t format;       // Object format type
  orionobj_arch_t architecture;   // Target architecture
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t version_patch;
  uint32_t flags;                 // Format-specific flags
  uint32_t string_table_size;     // Size of string table
  uint32_t section_count;         // Number of sections
  uint32_t symbol_count;          // Number of symbols
  uint32_t entry_point;           // Entry point (if applicable)
  uint32_t reserved[4];           // Reserved for future use
} orionobj_header_t;

// String constants (extern declarations)
extern const char* orionobj_format_names[];
extern const char* orionobj_arch_names[];
extern const char* orionobj_section_type_names[];
extern const char* orionobj_symbol_binding_names[];
extern const char* orionobj_symbol_type_names[];

#endif // ORIONOBJ_TYPES_H