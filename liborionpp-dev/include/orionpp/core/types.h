#ifndef ORIONPP_CORE_TYPES_H
#define ORIONPP_CORE_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Version information
#define ORIONPP_VERSION_MAJOR 1
#define ORIONPP_VERSION_MINOR 0
#define ORIONPP_VERSION_PATCH 0
#define ORIONPP_BINARY_MAGIC 0x4F50504E  // "OPPN" (OrionPP New)

// Forward declarations
typedef struct orionpp_module orionpp_module_t;
typedef struct orionpp_instruction orionpp_instruction_t;
typedef struct orionpp_value orionpp_value_t;
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
  ORIONPP_ISA_MAX = 255
} orionpp_isa_opcode_t;

// ABI feature instructions (second opcode byte)
typedef enum {
  ORIONPP_ABI_CALLEE = 1,
  ORIONPP_ABI_CALLER = 2,
  ORIONPP_ABI_ARGS = 3,
  ORIONPP_ABI_RETS = 4,
  ORIONPP_ABI_SETUP = 5,
  ORIONPP_ABI_CLEANUP = 6,
  ORIONPP_ABI_MAX = 255
} orionpp_abi_opcode_t;

// HINT feature instructions (second opcode byte)
typedef enum {
  ORIONPP_HINT_SYMEND = 1,
  ORIONPP_HINT_OPTIMIZE = 2,
  ORIONPP_HINT_UNROLL = 3,
  ORIONPP_HINT_INLINE = 4,
  ORIONPP_HINT_NOINLINE = 5,
  ORIONPP_HINT_MAX = 255
} orionpp_hint_opcode_t;

// Section types (from OrionObj integration)
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

// Section flags (from OrionObj integration)
typedef enum {
  ORIONPP_SECTION_FLAG_WRITE = 1 << 0,
  ORIONPP_SECTION_FLAG_ALLOC = 1 << 1,
  ORIONPP_SECTION_FLAG_EXEC = 1 << 2,
  ORIONPP_SECTION_FLAG_MERGE = 1 << 3,
  ORIONPP_SECTION_FLAG_STRINGS = 1 << 4,
  ORIONPP_SECTION_FLAG_TLS = 1 << 5,
} orionpp_section_flags_t;

// Symbol binding (from OrionObj integration)
typedef enum {
  ORIONPP_SYMBOL_LOCAL = 0,
  ORIONPP_SYMBOL_GLOBAL = 1,
  ORIONPP_SYMBOL_WEAK = 2,
  ORIONPP_SYMBOL_COMMON = 3,
} orionpp_symbol_binding_t;

// Symbol type (from OrionObj integration)
typedef enum {
  ORIONPP_SYMBOL_NONE = 0,
  ORIONPP_SYMBOL_OBJECT = 1,
  ORIONPP_SYMBOL_FUNC = 2,
  ORIONPP_SYMBOL_SECTION = 3,
  ORIONPP_SYMBOL_FILE = 4,
  ORIONPP_SYMBOL_TLS = 5,
} orionpp_symbol_type_t;

// Architecture types (from OrionObj integration)
typedef enum {
  ORIONPP_ARCH_UNKNOWN = 0,
  ORIONPP_ARCH_ORION_X86,
  ORIONPP_ARCH_ORION_ARM,
  ORIONPP_ARCH_ORION_RISCV,
  ORIONPP_ARCH_X86_32,
  ORIONPP_ARCH_X86_64,
  ORIONPP_ARCH_ARM_32,
  ORIONPP_ARCH_ARM_64,
  ORIONPP_ARCH_RISCV_32,
  ORIONPP_ARCH_RISCV_64,
  ORIONPP_ARCH_MAX = 0xFFFF
} orionpp_arch_t;

// Object format types (from OrionObj integration)
typedef enum {
  ORIONPP_FORMAT_UNKNOWN = 0,
  ORIONPP_FORMAT_ORIONPP,
  ORIONPP_FORMAT_ORION,
  ORIONPP_FORMAT_MAX = 255
} orionpp_format_t;

// ABI types
typedef enum {
  ORIONPP_ABI_CALLEE_TYPE = 0,
  ORIONPP_ABI_CALLER_TYPE = 1,
} orionpp_abi_type_t;

#endif // ORIONPP_CORE_TYPES_H