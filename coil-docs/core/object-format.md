# COIL Object Format

## Overview

COIL uses two primary object formats:

1. **COIL Object Format (.coil)**: Intermediate format for linkage
2. **COIL Output Object Format (.coilo)**: Executable format after linking

Both formats share a common structure but differ in their purpose and content. This document describes the binary layout and organization of these formats.

## Common Header Structure

Both formats begin with a header:

```
struct CoilHeader {
    char     magic[4];     // "COIL"
    uint8_t  major;        // Major version
    uint8_t  minor;        // Minor version
    uint8_t  patch;        // Patch version
    uint8_t  flags;        // Format flags
    uint32_t symbol_offset; // Offset to symbol table
    uint32_t section_offset; // Offset to section table
    uint32_t reloc_offset;  // Offset to relocation table (0 for .coilo)
    uint32_t file_size;    // Total file size
}
```

Flag values:
- `0x01`: Object file (.coil)
- `0x02`: Output file (.coilo)
- `0x04`: Contains debug information
- `0x08`: Core processor (CPU) targeted
- `0x10`: Extended processors targeted (v3)

## Symbol Table

The symbol table is essential for COIL's operation:

```
struct SymbolTable {
    uint32_t count;         // Number of symbols
    Symbol   entries[];     // Array of symbol entries
}

struct Symbol {
    uint16_t name_length;   // Length of symbol name
    char     name[];        // Symbol name (not null-terminated)
    uint32_t attributes;    // Symbol attributes
    uint32_t value;         // Symbol value (address or data)
    uint16_t section_index; // Index of containing section
}
```

Symbol attributes (bitfield):
- `0x01`: Global symbol
- `0x02`: Local symbol
- `0x04`: Weak symbol
- `0x08`: Function symbol
- `0x10`: Data symbol
- `0x20`: External reference

### Symbol Referencing

Symbols are referenced by their index in the symbol table to eliminate string duplication:

1. The first symbol has index 0
2. In COIL binary, TYPE_SYM (0x91) is followed by a symbol index
3. All references to a symbol use the same index

For example:
```
0x9100 0x0A         ; Reference to symbol with index 10
```

## Section Table

The section table defines memory regions:

```
struct SectionTable {
    uint32_t count;         // Number of sections
    Section  entries[];     // Array of section entries
}

struct Section {
    uint16_t name_index;    // Index into symbol table for name
    uint32_t attributes;    // Section attributes
    uint32_t offset;        // Offset from start of file
    uint32_t size;          // Size in bytes
    uint32_t address;       // Virtual address (0 if not set)
    uint32_t alignment;     // Required alignment
}
```

Section attributes (bitfield):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)

## Relocation Table (COIL Object Format Only)

The relocation table is used during linking:

```
struct RelocationTable {
    uint32_t count;           // Number of relocations
    Relocation entries[];     // Array of relocation entries
}

struct Relocation {
    uint32_t offset;          // Offset within section
    uint16_t symbol_index;    // Index into symbol table
    uint16_t section_index;   // Index into section table
    uint8_t  type;            // Relocation type
    uint8_t  size;            // Relocation size
}
```

Relocation types:
- `0x01`: Absolute
- `0x02`: Relative
- `0x03`: PC-relative
- `0x04`: Section-relative

## Debug Information (Optional)

If the header flags indicate debug information is present, a debug section follows:

```
struct DebugTable {
    uint32_t count;         // Number of debug entries
    Debug    entries[];     // Array of debug entries
}

struct Debug {
    uint16_t type;          // Debug entry type
    uint32_t length;        // Length of debug data
    uint8_t  data[];        // Debug data
}
```

Debug entry types:
- `0x01`: Line number information
- `0x02`: Variable information
- `0x03`: Type information
- `0x04`: Source file information

## COIL Object Format vs. COIL Output Object Format

Key differences between .coil and .coilo:

### COIL Object Format (.coil)
- Contains relocations
- May have unresolved external symbols
- Designed for linking
- Preserves section independence
- May require symbol resolution

### COIL Output Object Format (.coilo)
- No relocations (all addresses resolved)
- All symbols are resolved
- Ready for execution
- Contains platform-specific optimizations
- May include entry point information

## Binary Encoding Example

A minimal COIL object file:

```
; Header
"COIL"                 ; Magic number
0x01, 0x00, 0x00       ; Version 1.0.0
0x01                   ; Object file flag
0x00000020             ; Symbol table offset
0x0000004C             ; Section table offset
0x00000064             ; Relocation table offset
0x0000007C             ; File size

; Symbol table (at offset 0x20)
0x00000002             ; 2 symbols
0x04 "main"            ; Symbol 1: "main"
0x01                   ; Global attribute
0x00000000             ; Value
0x0001                 ; Section index
0x05 "hello"           ; Symbol 2: "hello"
0x10                   ; Data attribute
0x00000000             ; Value
0x0002                 ; Section index

; Section table (at offset 0x4C)
0x00000002             ; 2 sections
0xFFFF                 ; No name
0x01 | 0x04            ; Executable, readable
0x00000070             ; Offset
0x0000000C             ; Size
0x00000000             ; Address
0x00000001             ; Alignment
0xFFFF                 ; No name
0x02 | 0x04 | 0x08     ; Writable, readable, initialized
0x0000007C             ; Offset
0x00000000             ; Size
0x00000000             ; Address
0x00000001             ; Alignment

; Relocation table (at offset 0x64)
0x00000001             ; 1 relocation
0x00000006             ; Offset
0x0002                 ; Symbol index
0x0002                 ; Section index
0x01                   ; Absolute type
0x04                   ; 4 bytes

; Code section (at offset 0x70)
0x10 0x02 ...          ; MOV instruction
0x02 0x01 ...          ; BR instruction
```

## Best Practices

1. **Symbol Table Management**:
   - Use symbol table indices to reference symbols
   - Deduplicate strings by using symbol table
   - Keep symbol names meaningful for debugging

2. **Section Organization**:
   - Group related functionality in the same section
   - Use section attributes appropriately
   - Align sections according to their requirements

3. **File Size Optimization**:
   - Use the symbol table to reduce string duplication
   - Place related functionality in sequential order
   - Use compact representations where possible