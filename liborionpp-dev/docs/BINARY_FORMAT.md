# Orion++ Binary Format Specification

This document defines the complete binary file format (.orionpp) used for production deployment and distribution of Orion++ intermediate representation.

## Table of Contents

1. [Format Overview](#format-overview)
2. [File Layout](#file-layout)
3. [Header Structure](#header-structure)
4. [Data Sections](#data-sections)
5. [Instruction Encoding](#instruction-encoding)
6. [Loading and Validation](#loading-and-validation)
7. [Implementation Guidelines](#implementation-guidelines)

## Format Overview

### Design Goals

- **Compact Storage**: 60-80% smaller than human-readable format
- **Fast Loading**: Memory-mappable with minimal parsing
- **Random Access**: O(1) access to functions and symbols
- **Platform Independent**: Consistent across architectures and operating systems
- **Version Extensible**: Forward and backward compatibility support

### File Characteristics

- **Endianness**: Little-endian throughout
- **Alignment**: All structures naturally aligned
- **Magic Number**: `0x4F50504F` ("OPPO" in ASCII)
- **Current Version**: 2
- **Maximum File Size**: 4GB (32-bit offsets)

## File Layout

```
┌─────────────────────────────────────┐ Offset 0
│ File Header (64 bytes)              │
├─────────────────────────────────────┤
│ String Table (variable)             │
├─────────────────────────────────────┤
│ Symbol Table (variable)             │
├─────────────────────────────────────┤
│ Function Table (variable)           │
├─────────────────────────────────────┤
│ Variable Table (variable)           │
├─────────────────────────────────────┤
│ Instruction Data (variable)         │
└─────────────────────────────────────┘ EOF
```

### Section Ordering

Sections must appear in the specified order to enable streaming processing and validation.

## Header Structure

### Header Layout

```c
typedef struct OrionPPHeader {
  uint32_t magic;                   // 0x4F50504F ("OPPO")
  uint16_t version;                 // Format version (2)
  uint16_t flags;                   // Format flags
  uint32_t string_table_offset;     // Offset to string table
  uint32_t string_table_size;       // Size in bytes
  uint32_t symbol_table_offset;     // Offset to symbol table
  uint32_t symbol_table_size;       // Size in bytes
  uint32_t function_table_offset;   // Offset to function table
  uint32_t function_table_size;     // Size in bytes
  uint32_t variable_table_offset;   // Offset to variable table
  uint32_t variable_table_size;     // Size in bytes
  uint32_t instruction_data_offset; // Offset to instruction data
  uint32_t instruction_data_size;   // Size in bytes
  uint32_t entry_point;             // Entry function index
  uint32_t next_var_id;             // Next available variable ID
  uint32_t reserved[2];             // Reserved for future use
} __attribute__((packed)) OrionPPHeader;
```

### Header Fields

| Field | Type | Description |
|-------|------|-------------|
| `magic` | uint32_t | Must be `0x4F50504F` |
| `version` | uint16_t | Format version (currently 2) |
| `flags` | uint16_t | Format flags (see below) |
| `*_offset` | uint32_t | Byte offset from file start |
| `*_size` | uint32_t | Section size in bytes |
| `entry_point` | uint32_t | Index of entry function |
| `next_var_id` | uint32_t | Next variable ID to allocate |
| `reserved` | uint32_t[2] | Must be zero |

### Format Flags

| Bit | Name | Description |
|-----|------|-------------|
| 0 | `COMPRESSED` | File uses compression |
| 1 | `DEBUG_INFO` | Contains debug information |
| 2 | `OPTIMIZED` | Has been optimized |
| 3-15 | Reserved | Must be zero |

### Header Validation

```c
bool validate_header(const OrionPPHeader* header) {
  // Check magic number
  if (header->magic != 0x4F50504F) return false;
  
  // Check version compatibility
  if (header->version < 2 || header->version > 2) return false;
  
  // Validate reserved fields
  if (header->reserved[0] != 0 || header->reserved[1] != 0) return false;
  
  // Check section ordering and bounds
  uint32_t prev_end = sizeof(OrionPPHeader);
  if (header->string_table_offset < prev_end) return false;
  
  prev_end = header->string_table_offset + header->string_table_size;
  if (header->symbol_table_offset < prev_end) return false;
  
  // Continue validation for all sections...
  return true;
}
```

## Data Sections

### String Table

The string table contains all textual data referenced by offset throughout the file.

#### String Table Layout

```
┌─────────────────────────────────────┐ Offset 0
│ \0                                  │ (null string)
├─────────────────────────────────────┤ Offset 1
│ "main\0"                            │
├─────────────────────────────────────┤ Offset 6
│ "add\0"                             │
├─────────────────────────────────────┤ Offset 10
│ "global_variable\0"                 │
└─────────────────────────────────────┘
```

#### String Table Properties

- **Encoding**: UTF-8
- **Termination**: Null-terminated strings
- **Alignment**: No padding between strings
- **First String**: Always null string at offset 0
- **Deduplication**: Identical strings share same offset

#### String Table Implementation

```c
typedef struct StringTable {
  char* data;
  uint32_t size;
  uint32_t capacity;
} StringTable;

uint32_t string_table_add(StringTable* table, const char* str) {
  // Check for existing string
  for (uint32_t i = 0; i < table->size; ) {
    if (strcmp(&table->data[i], str) == 0) {
      return i;  // Return existing offset
    }
    i += strlen(&table->data[i]) + 1;
  }
  
  // Add new string
  uint32_t offset = table->size;
  size_t len = strlen(str) + 1;
  memcpy(&table->data[offset], str, len);
  table->size += len;
  return offset;
}
```

### Symbol Table

Contains symbol definitions with metadata for linking and debugging.

#### Symbol Table Entry

```c
typedef struct OrionPPSymbol {
  uint32_t name_offset;     // Offset into string table
  uint8_t type;             // Symbol type
  uint8_t visibility;       // Symbol visibility
  uint16_t section;         // Section identifier
  uint32_t value;           // Symbol value/address
  uint32_t size;            // Symbol size in bytes
} __attribute__((packed)) OrionPPSymbol;
```

#### Symbol Types

| Value | Name | Description |
|-------|------|-------------|
| 0x01 | `FUNC` | Function symbol |
| 0x02 | `OBJECT` | Data object |
| 0x03 | `SECTION` | Section symbol |

#### Symbol Visibility

| Value | Name | Description |
|-------|------|-------------|
| 0x00 | `LOCAL` | File scope only |
| 0x01 | `GLOBAL` | External linkage |
| 0x02 | `WEAK` | Weak linkage (can be overridden) |

### Function Table

Contains function metadata and instruction references.

#### Function Table Entry

```c
typedef struct OrionPPFunction {
  uint32_t symbol_index;        // Index into symbol table
  uint16_t param_count;         // Number of parameters
  uint16_t local_count;         // Number of local variables
  uint32_t instruction_offset;  // Offset into instruction data
  uint32_t instruction_count;   // Number of instruction bytes
  uint8_t abi_type;             // ABI type
  uint8_t return_type;          // Return type
  uint16_t flags;               // Function flags
  uint32_t first_var_id;        // First variable ID
  uint32_t last_var_id;         // Last variable ID
} __attribute__((packed)) OrionPPFunction;
```

#### ABI Types

| Value | Name | Description |
|-------|------|-------------|
| 0x01 | `C` | C calling convention |
| 0x02 | `CUSTOM` | Custom calling convention |

#### Data Types

| Value | Name | Description |
|-------|------|-------------|
| 0x00 | `VOID` | No value |
| 0x01 | `I32` | 32-bit signed integer |

#### Function Flags

| Bit | Name | Description |
|-----|------|-------------|
| 0 | `INLINE` | Inline function hint |
| 1 | `PURE` | Pure function (no side effects) |
| 2 | `LEAF` | Leaf function (no calls) |
| 3-15 | Reserved | Must be zero |

### Variable Table

Contains variable metadata for debugging and optimization.

#### Variable Table Entry

```c
typedef struct OrionPPVariable {
  uint32_t id;              // Unique variable ID
  uint8_t type;             // Variable type
  uint8_t data_type;        // Data type
  uint16_t flags;           // Variable flags
  uint32_t name_offset;     // Debug name (string table offset)
  uint32_t scope_id;        // Scope identifier
} __attribute__((packed)) OrionPPVariable;
```

#### Variable Types

| Value | Name | Description |
|-------|------|-------------|
| 0x00 | `TEMPORARY` | Compiler temporary |
| 0x01 | `LOCAL` | Local variable |
| 0x02 | `PARAMETER` | Function parameter |
| 0x03 | `GLOBAL` | Global variable |

## Instruction Encoding

### Instruction Format

Instructions use variable-length encoding for compactness:

```
┌─────────┬─────────┬─────────────────────────────┐
│ Opcode  │ Flags   │ Operands (variable length)  │
│ 8 bits  │ 8 bits  │ 0-N operands                │
└─────────┴─────────┴─────────────────────────────┘
```

### Operand Encoding

Each operand is encoded with a type prefix:

```c
#define OPERAND_IMMEDIATE   0x00  // 00xxxxxx - immediate value
#define OPERAND_VARIABLE    0x40  // 01xxxxxx - variable ID
#define OPERAND_LABEL       0x80  // 10xxxxxx - label offset
#define OPERAND_SYMBOL      0xC0  // 11xxxxxx - symbol reference
```

#### Operand Formats

**Immediate Operand (6 bytes)**
```
┌─────────┬─────────────────────────────────────┐
│ 0x00    │ 32-bit immediate value (little-endian) │
└─────────┴─────────────────────────────────────┘
```

**Variable Operand (5 bytes)**
```
┌─────────┬─────────────────────────────────────┐
│ 0x40    │ 32-bit variable ID (little-endian)    │
└─────────┴─────────────────────────────────────┘
```

**Label Operand (5 bytes)**
```
┌─────────┬─────────────────────────────────────┐
│ 0x80    │ 32-bit offset (little-endian)        │
└─────────┴─────────────────────────────────────┘
```

**Symbol Operand (5 bytes)**
```
┌─────────┬─────────────────────────────────────┐
│ 0xC0    │ 32-bit string table offset           │
└─────────┴─────────────────────────────────────┘
```

### Instruction Set

#### Core Instructions

| Opcode | Name | Operands | Description |
|--------|------|----------|-------------|
| 0x00 | `NOP` | None | No operation |
| 0x01 | `ENTER` | None | Enter scope |
| 0x02 | `LEAVE` | None | Exit scope |
| 0x03 | `RET` | None | Return from function |

#### Variable Operations

| Opcode | Name | Operands | Description |
|--------|------|----------|-------------|
| 0x10 | `LET` | var, var | Variable assignment |
| 0x11 | `CONST` | var, imm | Load constant |
| 0x12 | `MOV` | var, var | Move variable |

#### Arithmetic Operations

| Opcode | Name | Operands | Description |
|--------|------|----------|-------------|
| 0x20 | `ADD` | var, var, var | Addition |
| 0x21 | `SUB` | var, var, var | Subtraction |
| 0x22 | `MUL` | var, var, var | Multiplication |
| 0x23 | `DIV` | var, var, var | Division |
| 0x24 | `MOD` | var, var, var | Modulo |
| 0x25 | `NEG` | var, var | Negation |

#### Control Flow

| Opcode | Name | Operands | Description |
|--------|------|----------|-------------|
| 0x40 | `JMP` | label | Unconditional jump |
| 0x41 | `CALL` | symbol | Function call |
| 0x42 | `BEQ` | var, var, label | Branch if equal |
| 0x43 | `BNE` | var, var, label | Branch if not equal |
| 0x44 | `BLT` | var, var, label | Branch if less |
| 0x45 | `BLE` | var, var, label | Branch if less/equal |
| 0x46 | `BGT` | var, var, label | Branch if greater |
| 0x47 | `BGE` | var, var, label | Branch if greater/equal |
| 0x48 | `BEQZ` | var, label | Branch if zero |
| 0x49 | `BNEZ` | var, label | Branch if not zero |

### Instruction Examples

#### Simple Assignment
```
Human: isa.const $1, 42
Binary: 0x11 0x00 0x40 0x01 0x00 0x00 0x00 0x00 0x2A 0x00 0x00 0x00
        │    │    │    └─ Variable ID 1
        │    │    └────── Variable operand type
        │    └─────────── Flags (unused)
        └──────────────── CONST opcode
                          └─ Immediate value 42
```

#### Function Call
```
Human: isa.call @"add"
Binary: 0x41 0x00 0xC0 0x05 0x00 0x00 0x00
        │    │    │    └─ String table offset 5
        │    │    └────── Symbol operand type
        │    └─────────── Flags (unused)
        └──────────────── CALL opcode
```

## Loading and Validation

### Loading Process

1. **Memory Mapping**: Map file into memory for fast access
2. **Header Validation**: Verify magic number, version, and structure
3. **Section Validation**: Check section offsets and sizes
4. **Symbol Resolution**: Build symbol lookup tables
5. **Function Indexing**: Create function access structures

### Memory Layout

```c
typedef struct OrionContext {
  void* file_data;                  // Memory-mapped file
  size_t file_size;                 // File size in bytes
  OrionPPHeader* header;            // Header pointer
  char* string_table;               // String table pointer
  OrionPPSymbol* symbol_table;      // Symbol table pointer
  OrionPPFunction* function_table;  // Function table pointer
  OrionPPVariable* variable_table;  // Variable table pointer
  uint8_t* instruction_data;        // Instruction data pointer
  
  // Derived data structures
  SymbolIndex* symbol_index;        // Fast symbol lookup
  FunctionIndex* function_index;    // Fast function lookup
} OrionContext;
```

### Validation Checklist

- [ ] Magic number is correct (`0x4F50504F`)
- [ ] Version is supported (currently 2)
- [ ] File size matches content
- [ ] All section offsets are valid
- [ ] All section sizes are valid
- [ ] String table is null-terminated
- [ ] All string offsets are valid
- [ ] All symbol references are valid
- [ ] All variable IDs are within range
- [ ] All instruction opcodes are valid
- [ ] All operand types are valid

### Error Handling

```c
typedef enum {
  ORION_OK = 0,
  ORION_ERROR_INVALID_MAGIC,
  ORION_ERROR_UNSUPPORTED_VERSION,
  ORION_ERROR_CORRUPTED_FILE,
  ORION_ERROR_INVALID_OFFSET,
  ORION_ERROR_INVALID_SIZE,
  ORION_ERROR_INVALID_STRING,
  ORION_ERROR_INVALID_SYMBOL,
  ORION_ERROR_INVALID_INSTRUCTION,
  ORION_ERROR_OUT_OF_MEMORY
} OrionResult;
```

## Implementation Guidelines

### Memory Management

- **File Mapping**: Use `mmap()` on Unix, `MapViewOfFile()` on Windows
- **Read-Only Access**: Map files as read-only for safety
- **Error Handling**: Gracefully handle mapping failures
- **Resource Cleanup**: Properly unmap files when done

### Performance Considerations

- **Lazy Loading**: Load sections on demand for large files
- **Caching**: Cache frequently accessed symbols and functions
- **Alignment**: Ensure proper alignment for SIMD operations
- **Prefetching**: Prefetch instruction data for sequential access

### Platform Portability

- **Endianness**: All data is little-endian regardless of platform
- **Alignment**: Use packed structures to avoid alignment issues
- **File I/O**: Use standard library functions for portability
- **Memory Mapping**: Provide fallback for platforms without mmap

### Example Implementation

```c
OrionContext* orion_load_file(const char* filename) {
  OrionContext* ctx = calloc(1, sizeof(OrionContext));
  if (!ctx) return NULL;
  
  // Open and map file
  int fd = open(filename, O_RDONLY);
  if (fd < 0) goto error;
  
  struct stat st;
  if (fstat(fd, &st) < 0) goto error;
  ctx->file_size = st.st_size;
  
  ctx->file_data = mmap(NULL, ctx->file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (ctx->file_data == MAP_FAILED) goto error;
  close(fd);
  
  // Validate and setup pointers
  if (!setup_context_pointers(ctx)) goto error;
  if (!validate_file_structure(ctx)) goto error;
  if (!build_indices(ctx)) goto error;
  
  return ctx;
  
error:
  if (fd >= 0) close(fd);
  orion_free_context(ctx);
  return NULL;
}
```

This specification provides complete implementation guidance for efficient Orion++ binary format processing.