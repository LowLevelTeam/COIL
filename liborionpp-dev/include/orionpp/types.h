#ifndef ORIONPP_TYPES_H
#define ORIONPP_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Version information
#define ORIONPP_VERSION_MAJOR 1
#define ORIONPP_VERSION_MINOR 0
#define ORIONPP_VERSION_PATCH 0
#define ORIONPP_BINARY_MAGIC 0x4F50504E  // "OPPN" (OrionPP New)

// Forward declarations
typedef struct orionpp_module    orionpp_module_t;
typedef struct orionpp_instruction orionpp_instruction_t;
typedef struct orionpp_value     orionpp_value_t;
typedef struct orionpp_string_table orionpp_string_table_t;

// Feature modules (first opcode byte)
typedef enum {
  ORIONPP_FEATURE_NONE = 0,
  ORIONPP_FEATURE_OBJ  = 1,  // Object file structure
  ORIONPP_FEATURE_ISA  = 2,  // Instruction set architecture
  ORIONPP_FEATURE_ABI  = 3,  // Application binary interface
  ORIONPP_FEATURE_HINT = 4,  // Optimization hints
  // Reserved for future expansion
  ORIONPP_FEATURE_RESERVED_START = 5,
  ORIONPP_FEATURE_USER_START = 192,
  ORIONPP_FEATURE_MAX = 255
} orionpp_feature_t;

// Feature flags (bitmask for enabled features)
typedef uint32_t orionpp_features_t;
#define ORIONPP_FEATURES_OBJ  (1U << ORIONPP_FEATURE_OBJ)
#define ORIONPP_FEATURES_ISA  (1U << ORIONPP_FEATURE_ISA)
#define ORIONPP_FEATURES_ABI  (1U << ORIONPP_FEATURE_ABI)
#define ORIONPP_FEATURES_HINT (1U << ORIONPP_FEATURE_HINT)

// Object feature instructions (second opcode byte)
typedef enum {
  ORIONPP_OBJ_SECTION = 1,
  ORIONPP_OBJ_SYMBOL = 2,
  ORIONPP_OBJ_BYTE = 3,
  ORIONPP_OBJ_WORD = 4,
  ORIONPP_OBJ_DWORD = 5,
  ORIONPP_OBJ_QWORD = 6,
  ORIONPP_OBJ_ALIGN = 7,
  // Reserved for expansion
  ORIONPP_OBJ_MAX = 255
} orionpp_obj_opcode_t;

// ISA feature instructions (second opcode byte)
typedef enum {
  ORIONPP_ISA_SCOPE_ENTER = 1,
  ORIONPP_ISA_SCOPE_LEAVE = 2,
  ORIONPP_ISA_LET = 3,
  ORIONPP_ISA_MOV = 4,
  ORIONPP_ISA_CONST = 5,
  ORIONPP_ISA_ADD = 6,
  ORIONPP_ISA_SUB = 7,
  ORIONPP_ISA_MUL = 8,
  ORIONPP_ISA_DIV = 9,
  ORIONPP_ISA_AND = 10,
  ORIONPP_ISA_OR = 11,
  ORIONPP_ISA_XOR = 12,
  ORIONPP_ISA_NOT = 13,
  ORIONPP_ISA_SHL = 14,
  ORIONPP_ISA_SHR = 15,
  ORIONPP_ISA_CMP = 16,
  ORIONPP_ISA_BR_EQ = 17,
  ORIONPP_ISA_BR_NE = 18,
  ORIONPP_ISA_BR_LT = 19,
  ORIONPP_ISA_BR_LE = 20,
  ORIONPP_ISA_BR_GT = 21,
  ORIONPP_ISA_BR_GE = 22,
  ORIONPP_ISA_JMP = 23,
  ORIONPP_ISA_LABEL = 24,
  ORIONPP_ISA_CALL = 25,
  ORIONPP_ISA_RET = 26,
  ORIONPP_ISA_NOP = 27,
  // Reserved for expansion
  ORIONPP_ISA_MAX = 255
} orionpp_isa_opcode_t;

// ABI feature instructions (second opcode byte)
typedef enum {
  ORIONPP_ABI_CALLEE = 1,        // abi.callee(abi_name)
  ORIONPP_ABI_CALLER = 2,        // abi.caller(abi_name)
  ORIONPP_ABI_ARGS = 3,          // abi.args [...]
  ORIONPP_ABI_RETS = 4,          // abi.rets [...]
  ORIONPP_ABI_SETUP = 5,         // abi.callee.setup / abi.caller.setup
  ORIONPP_ABI_CLEANUP = 6,       // abi.callee.cleanup / abi.caller.cleanup
  // Reserved for expansion
  ORIONPP_ABI_MAX = 255
} orionpp_abi_opcode_t;

// HINT feature instructions (second opcode byte)
typedef enum {
  ORIONPP_HINT_SYMEND = 1,       // hint.symend
  ORIONPP_HINT_OPTIMIZE = 2,     // hint.optimize(level)
  ORIONPP_HINT_UNROLL = 3,       // hint.unroll(count)
  ORIONPP_HINT_INLINE = 4,       // hint.inline
  ORIONPP_HINT_NOINLINE = 5,     // hint.noinline
  // Reserved for expansion
  ORIONPP_HINT_MAX = 255
} orionpp_hint_opcode_t;

// Section types
typedef enum {
  ORIONPP_SECTION_TEXT = 0,
  ORIONPP_SECTION_DATA = 1,
  ORIONPP_SECTION_BSS = 2,
  ORIONPP_SECTION_RODATA = 3,
  ORIONPP_SECTION_SYMTAB = 4,
  ORIONPP_SECTION_STRTAB = 5,
  ORIONPP_SECTION_RELTAB = 6,
  ORIONPP_SECTION_DEBUG = 7,
  ORIONPP_SECTION_USER_START = 128,
  ORIONPP_SECTION_MAX = 255
} orionpp_section_type_t;

// Section flags
typedef enum {
  ORIONPP_SECTION_FLAG_WRITE = 1 << 0,
  ORIONPP_SECTION_FLAG_ALLOC = 1 << 1,
  ORIONPP_SECTION_FLAG_EXEC = 1 << 2,
  ORIONPP_SECTION_FLAG_MERGE = 1 << 3,
  ORIONPP_SECTION_FLAG_STRINGS = 1 << 4,
  ORIONPP_SECTION_FLAG_TLS = 1 << 5,
} orionpp_section_flags_t;

// Symbol binding
typedef enum {
  ORIONPP_SYMBOL_LOCAL = 0,
  ORIONPP_SYMBOL_GLOBAL = 1,
  ORIONPP_SYMBOL_WEAK = 2,
  ORIONPP_SYMBOL_COMMON = 3,
} orionpp_symbol_binding_t;

// Symbol type
typedef enum {
  ORIONPP_SYMBOL_NONE = 0,
  ORIONPP_SYMBOL_OBJECT = 1,
  ORIONPP_SYMBOL_FUNC = 2,
  ORIONPP_SYMBOL_SECTION = 3,
  ORIONPP_SYMBOL_FILE = 4,
  ORIONPP_SYMBOL_TLS = 5,
} orionpp_symbol_type_t;

// Value types
typedef enum {
  ORIONPP_VALUE_NONE = 0,
  ORIONPP_VALUE_VARIABLE = 1,    // $n
  ORIONPP_VALUE_SYMBOL = 2,      // @symbol@
  ORIONPP_VALUE_NUMERIC = 3,     // %base[digits]
  ORIONPP_VALUE_ARRAY = 4,       // [values...]
  ORIONPP_VALUE_LABEL = 5,       // .label / +.label / -.label
  ORIONPP_VALUE_STRING = 6,      // "string"
} orionpp_value_type_t;

// Numeric base types
typedef enum {
  ORIONPP_BASE_BINARY = 2,
  ORIONPP_BASE_OCTAL = 8,
  ORIONPP_BASE_DECIMAL = 10,
  ORIONPP_BASE_HEX = 16,
} orionpp_numeric_base_t;

// ABI types
typedef enum {
  ORIONPP_ABI_CALLEE_TYPE = 0,
  ORIONPP_ABI_CALLER_TYPE = 1,
} orionpp_abi_type_t;

// Value structure
struct orionpp_value {
  orionpp_value_type_t type;
  union {
    // Variable reference: $n
    uint32_t variable_id;
    
    // Symbol reference: @symbol@
    struct {
      uint32_t name_offset;     // Offset into string table
      uint32_t name_length;
    } symbol;
    
    // Numeric literal: %base[digits]
    struct {
      orionpp_numeric_base_t base;
      uint64_t value;
    } numeric;
    
    // Array of values: [v1, v2, ...]
    struct {
      uint32_t count;
      orionpp_value_t* values;
    } array;
    
    // Label reference: .label, +.label, -.label
    struct {
      uint32_t name_offset;     // Offset into string table
      uint32_t name_length;
      int8_t direction;         // -1 = backward, 0 = local, +1 = forward
    } label;
    
    // String literal: "string"
    struct {
      uint32_t offset;          // Offset into string table
      uint32_t length;
    } string;
  };
};

// Binary format header
typedef struct {
  uint32_t magic;               // ORIONPP_BINARY_MAGIC
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t version_patch;
  orionpp_features_t features;  // Enabled features bitmask
  uint32_t string_table_size;
  uint32_t instruction_count;
  uint32_t reserved[4];         // For future expansion
} orionpp_binary_header_t;

// Instruction in binary format
typedef struct {
  uint8_t feature;              // First opcode (feature module)
  uint8_t opcode;               // Second opcode (instruction within module)
  uint16_t flags;               // Instruction-specific flags
  uint32_t data_size;           // Size of following instruction data
  // Followed by instruction-specific data
} orionpp_binary_instruction_t;

// Error codes
typedef enum {
  ORIONPP_OK = 0,
  ORIONPP_ERROR_NULL_POINTER = -1,
  ORIONPP_ERROR_OUT_OF_MEMORY = -2,
  ORIONPP_ERROR_INVALID_FEATURE = -3,
  ORIONPP_ERROR_INVALID_INSTRUCTION = -4,
  ORIONPP_ERROR_IO_ERROR = -5,
  ORIONPP_ERROR_PARSE_ERROR = -6,
  ORIONPP_ERROR_VERSION_MISMATCH = -7,
  ORIONPP_ERROR_CORRUPT_DATA = -8,
} orionpp_error_t;

// Result type for functions that can fail
typedef struct {
  orionpp_error_t error;
  union {
    void* ptr;
    int64_t integer;
    bool boolean;
  } value;
} orionpp_result_t;

// Macros for result handling
#define ORIONPP_OK_PTR(__ptr) ((orionpp_result_t){.error = ORIONPP_OK, .value.ptr = (void*)(__ptr)})
#define ORIONPP_OK_INT(val) ((orionpp_result_t){.error = ORIONPP_OK, .value.integer = (val)})
#define ORIONPP_OK_BOOL(val) ((orionpp_result_t){.error = ORIONPP_OK, .value.boolean = (val)})
#define ORIONPP_ERROR(err) ((orionpp_result_t){.error = (err), .value.ptr = NULL})

#define ORIONPP_IS_OK(result) ((result).error == ORIONPP_OK)
#define ORIONPP_IS_ERROR(result) ((result).error != ORIONPP_OK)

// Memory allocation function pointers (for custom allocators)
typedef void* (*orionpp_malloc_fn)(size_t size);
typedef void* (*orionpp_realloc_fn)(void* ptr, size_t size);
typedef void (*orionpp_free_fn)(void* ptr);

typedef struct {
  orionpp_malloc_fn malloc;
  orionpp_realloc_fn realloc;
  orionpp_free_fn free;
} orionpp_allocator_t;

// Default allocator
extern const orionpp_allocator_t orionpp_default_allocator;

#endif // ORIONPP_TYPES_H