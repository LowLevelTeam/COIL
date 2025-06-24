#include "orionobj/types.h"
#include <stdlib.h>
#include <string.h>

// Default allocator implementation
static void* default_malloc(size_t size) { 
  return malloc(size); 
}

static void* default_realloc(void* ptr, size_t size) { 
  return realloc(ptr, size); 
}

static void default_free(void* ptr) { 
  free(ptr); 
}

const orionobj_allocator_t orionobj_default_allocator = {
  .malloc = default_malloc,
  .realloc = default_realloc,
  .free = default_free
};

// String constants for format names
const char* orionobj_format_names[] = {
  [ORIONOBJ_FORMAT_UNKNOWN] = "unknown",
  [ORIONOBJ_FORMAT_ORIONPP] = "orionpp",
  [ORIONOBJ_FORMAT_ORION] = "orion",
};

const char* orionobj_arch_names[] = {
  [ORIONOBJ_ARCH_UNKNOWN] = "unknown",
  [ORIONOBJ_ARCH_ORION_X86] = "orion.x86",
  [ORIONOBJ_ARCH_ORION_ARM] = "orion.arm",
  [ORIONOBJ_ARCH_ORION_RISCV] = "orion.riscv",
  [ORIONOBJ_ARCH_X86_32] = "x86-32",
  [ORIONOBJ_ARCH_X86_64] = "x86-64",
  [ORIONOBJ_ARCH_ARM_32] = "arm32",
  [ORIONOBJ_ARCH_ARM_64] = "arm64",
  [ORIONOBJ_ARCH_RISCV_32] = "riscv32",
  [ORIONOBJ_ARCH_RISCV_64] = "riscv64",
};

const char* orionobj_section_type_names[] = {
  [ORIONOBJ_SECTION_TEXT] = ".text",
  [ORIONOBJ_SECTION_DATA] = ".data",
  [ORIONOBJ_SECTION_BSS] = ".bss",
  [ORIONOBJ_SECTION_RODATA] = ".rodata",
  [ORIONOBJ_SECTION_SYMTAB] = ".symtab",
  [ORIONOBJ_SECTION_STRTAB] = ".strtab",
  [ORIONOBJ_SECTION_RELTAB] = ".reltab",
  [ORIONOBJ_SECTION_DEBUG] = ".debug",
  [ORIONOBJ_SECTION_ORION_META] = ".orion_meta",
};

const char* orionobj_symbol_binding_names[] = {
  [ORIONOBJ_SYMBOL_LOCAL] = "LOCAL",
  [ORIONOBJ_SYMBOL_GLOBAL] = "GLOBAL",
  [ORIONOBJ_SYMBOL_WEAK] = "WEAK",
};

const char* orionobj_symbol_type_names[] = {
  [ORIONOBJ_SYMBOL_NONE] = "NONE",
  [ORIONOBJ_SYMBOL_OBJECT] = "OBJECT",
  [ORIONOBJ_SYMBOL_FUNC] = "FUNC",
  [ORIONOBJ_SYMBOL_SECTION] = "SECTION",
  [ORIONOBJ_SYMBOL_FILE] = "FILE",
};

const char* orionobj_format_name(orionobj_format_t format) {
  switch (format) {
    case ORIONOBJ_FORMAT_UNKNOWN: return orionobj_format_names[ORIONOBJ_FORMAT_UNKNOWN];
    case ORIONOBJ_FORMAT_ORIONPP: return orionobj_format_names[ORIONOBJ_FORMAT_ORIONPP];
    case ORIONOBJ_FORMAT_ORION: return orionobj_format_names[ORIONOBJ_FORMAT_ORION];
    default: return "unknown";
  }
}

const char* orionobj_arch_name(orionobj_arch_t arch) {
  switch (arch) {
    case ORIONOBJ_ARCH_UNKNOWN: return orionobj_arch_names[ORIONOBJ_ARCH_UNKNOWN];
    case ORIONOBJ_ARCH_ORION_X86: return orionobj_arch_names[ORIONOBJ_ARCH_ORION_X86];
    case ORIONOBJ_ARCH_ORION_ARM: return orionobj_arch_names[ORIONOBJ_ARCH_ORION_ARM];
    case ORIONOBJ_ARCH_ORION_RISCV: return orionobj_arch_names[ORIONOBJ_ARCH_ORION_RISCV];
    case ORIONOBJ_ARCH_X86_32: return orionobj_arch_names[ORIONOBJ_ARCH_X86_32];
    case ORIONOBJ_ARCH_X86_64: return orionobj_arch_names[ORIONOBJ_ARCH_X86_64];
    case ORIONOBJ_ARCH_ARM_32: return orionobj_arch_names[ORIONOBJ_ARCH_ARM_32];
    case ORIONOBJ_ARCH_ARM_64: return orionobj_arch_names[ORIONOBJ_ARCH_ARM_64];
    case ORIONOBJ_ARCH_RISCV_32: return orionobj_arch_names[ORIONOBJ_ARCH_RISCV_32];
    case ORIONOBJ_ARCH_RISCV_64: return orionobj_arch_names[ORIONOBJ_ARCH_RISCV_64];
    default: return "unknown";
  }
}

const char* orionobj_error_string(orionobj_error_t error) {
  switch (error) {
    case ORIONOBJ_OK: return "Success";
    case ORIONOBJ_ERROR_NULL_POINTER: return "Null pointer";
    case ORIONOBJ_ERROR_OUT_OF_MEMORY: return "Out of memory";
    case ORIONOBJ_ERROR_INVALID_FORMAT: return "Invalid format";
    case ORIONOBJ_ERROR_INVALID_ARCH: return "Invalid architecture";
    case ORIONOBJ_ERROR_IO_ERROR: return "I/O error";
    case ORIONOBJ_ERROR_PARSE_ERROR: return "Parse error";
    case ORIONOBJ_ERROR_CORRUPT_DATA: return "Corrupt data";
    case ORIONOBJ_ERROR_NOT_FOUND: return "Not found";
    default: return "Unknown error";
  }
}