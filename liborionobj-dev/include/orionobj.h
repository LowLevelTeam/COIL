#ifndef ORIONOBJ_H
#define ORIONOBJ_H

#include <stdint.h>
#include <stddef.h>

// Orion Object Format Magic Number
#define ORION_MAGIC 0x4F52494F  // "ORIO" in ASCII

// Format version
#define ORION_VERSION_CURRENT 1

// Object types
typedef enum {
  ORION_OBJ_NONE = 0,
  ORION_OBJ_REL,        // Relocatable object
  ORION_OBJ_EXEC,       // Executable
  ORION_OBJ_DYN,        // Shared object
  ORION_OBJ_CORE        // Core dump
} OrionObjectType;

// Section types
typedef enum {
  ORION_SEC_NULL = 0,          // Null section
  ORION_SEC_ORIONPP_TEXT,      // Orion++ executable code
  ORION_SEC_ORIONVAR_TEXT,     // Orion variant executable code
  ORION_SEC_NATIVE_TEXT,       // Native machine code
  ORION_SEC_DATA,              // Initialized data
  ORION_SEC_BSS,               // Uninitialized data
  ORION_SEC_SYMTAB,            // Symbol table
  ORION_SEC_STRTAB,            // String table
  ORION_SEC_RELA,              // Relocation entries with addends
  ORION_SEC_DYNAMIC,           // Dynamic linking information
  ORION_SEC_NOTE,              // Note section
  ORION_SEC_DEBUG              // Debug information
} OrionSectionType;

// Section flags
typedef enum {
  ORION_SEC_FLAG_WRITE = 0x1,     // Writable
  ORION_SEC_FLAG_ALLOC = 0x2,     // Occupies memory during execution
  ORION_SEC_FLAG_EXEC = 0x4,      // Executable
  ORION_SEC_FLAG_MERGE = 0x10,    // Can be merged
  ORION_SEC_FLAG_STRINGS = 0x20,  // Contains null-terminated strings
  ORION_SEC_FLAG_GROUP = 0x200    // Member of a section group
} OrionSectionFlags;

// Target architectures (for native code only)
typedef enum {
  ORION_TARGET_NONE = 0,
  ORION_TARGET_X86,
  ORION_TARGET_X86_32,
  ORION_TARGET_X86_64,
  ORION_TARGET_ARM_THUMB,
  ORION_TARGET_ARM,
  ORION_TARGET_ARM64,
  ORION_TARGET_RISCV32,
  ORION_TARGET_RISCV64,
  ORION_TARGET_RISCV128
} OrionTargetArch;

// Orion variant types
typedef enum {
  ORION_VAR_NONE = 0,
  ORION_VAR_X86,
  ORION_VAR_ARM,
  ORION_VAR_RISCV6
} OrionVariantType;

// Symbol binding
typedef enum {
  ORION_SYM_LOCAL = 0,
  ORION_SYM_GLOBAL = 1,
  ORION_SYM_WEAK = 2
} OrionSymbolBinding;

// Symbol types
typedef enum {
  ORION_SYM_NOTYPE = 0,
  ORION_SYM_OBJECT = 1,
  ORION_SYM_FUNC = 2,
  ORION_SYM_SECTION = 3,
  ORION_SYM_FILE = 4
} OrionSymbolType;

// Object header
typedef struct {
  uint32_t magic;               // Magic number (ORION_MAGIC)
  uint8_t version;              // Format version
  uint8_t obj_type;             // Object type (OrionObjectType)
  uint8_t target_arch;          // Target architecture (for native code)
  uint8_t variant_type;         // Orion variant type
  uint32_t flags;               // Object flags
  uint64_t entry_point;         // Entry point address (if executable)
  uint32_t section_header_offset; // Offset to section header table
  uint32_t section_count;       // Number of sections
  uint32_t section_header_size; // Size of section header entry
  uint32_t string_table_index;  // Index of section name string table
  uint32_t reserved[4];         // Reserved for future use
} OrionObjectHeader;

// Section header
typedef struct {
  uint32_t name_offset;         // Offset into string table
  uint32_t type;                // Section type (OrionSectionType)
  uint64_t flags;               // Section flags (OrionSectionFlags)
  uint64_t addr;                // Section virtual address
  uint64_t offset;              // Offset in file
  uint64_t size;                // Section size in bytes
  uint32_t link;                // Link to another section
  uint32_t info;                // Additional section information
  uint64_t alignment;           // Section alignment
  uint64_t entry_size;          // Entry size if section holds table
} OrionSectionHeader;

// Symbol table entry
typedef struct {
  uint32_t name_offset;         // Offset into string table
  uint8_t info;                 // Symbol type and binding (packed)
  uint8_t other;                // Symbol visibility
  uint16_t section_index;       // Section header table index
  uint64_t value;               // Symbol value
  uint64_t size;                // Symbol size
} OrionSymbol;

// Relocation entry with addend
typedef struct {
  uint64_t offset;              // Offset where to apply relocation
  uint64_t info;                // Relocation type and symbol index
  int64_t addend;               // Constant addend
} OrionRelocationEntry;

// Internal structures (exposed for library use)
typedef struct OrionSection {
  char* name;
  OrionSectionHeader header;
  void* data;
  size_t data_size;
  size_t data_capacity;
} OrionSection;

typedef struct OrionObject {
  OrionObjectHeader header;
  OrionSection** sections;
  size_t section_count;
  size_t section_capacity;
  
  OrionSymbol* symbols;
  size_t symbol_count;
  size_t symbol_capacity;
  
  char* string_table;
  size_t string_table_size;
  size_t string_table_capacity;
  
  uint32_t section_name_table_index;
} OrionObject;

// Object manipulation functions
OrionObject* orion_obj_create(OrionObjectType type, OrionTargetArch target);
OrionObject* orion_obj_load(const char* filename);
int orion_obj_save(OrionObject* obj, const char* filename);
void orion_obj_free(OrionObject* obj);

// Section manipulation functions
OrionSection* orion_obj_add_section(OrionObject* obj, const char* name, OrionSectionType type);
OrionSection* orion_obj_get_section(OrionObject* obj, const char* name);
OrionSection* orion_obj_get_section_by_index(OrionObject* obj, uint32_t index);
uint32_t orion_obj_get_section_count(OrionObject* obj);

// Section data functions
int orion_section_set_data(OrionSection* section, const void* data, size_t size);
void* orion_section_get_data(OrionSection* section);
size_t orion_section_get_size(OrionSection* section);
void orion_section_set_flags(OrionSection* section, uint64_t flags);
uint64_t orion_section_get_flags(OrionSection* section);
void orion_section_set_alignment(OrionSection* section, uint64_t alignment);

// Symbol table functions
int orion_obj_add_symbol(OrionObject* obj, const char* name, OrionSymbolType type, 
                         OrionSymbolBinding binding, uint16_t section_index, 
                         uint64_t value, uint64_t size);
OrionSymbol* orion_obj_get_symbol(OrionObject* obj, const char* name);
OrionSymbol* orion_obj_get_symbol_by_index(OrionObject* obj, uint32_t index);
uint32_t orion_obj_get_symbol_count(OrionObject* obj);

// String table functions
uint32_t orion_obj_add_string(OrionObject* obj, const char* str);
const char* orion_obj_get_string(OrionObject* obj, uint32_t offset);

// Utility functions
#define ORION_SYM_BIND(info) ((info) >> 4)
#define ORION_SYM_TYPE(info) ((info) & 0xf)
#define ORION_SYM_INFO(bind, type) (((bind) << 4) + ((type) & 0xf))

// Error codes
typedef enum {
  ORION_OK = 0,
  ORION_ERR_INVALID_MAGIC,
  ORION_ERR_INVALID_VERSION,
  ORION_ERR_INVALID_FORMAT,
  ORION_ERR_FILE_NOT_FOUND,
  ORION_ERR_PERMISSION_DENIED,
  ORION_ERR_OUT_OF_MEMORY,
  ORION_ERR_INVALID_SECTION,
  ORION_ERR_INVALID_SYMBOL,
  ORION_ERR_BUFFER_TOO_SMALL
} OrionError;

// Get last error
OrionError orion_get_last_error(void);
const char* orion_get_error_string(OrionError error);

#endif // ORIONOBJ_H