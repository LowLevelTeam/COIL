# COIL Binary Format Details

## Overview

This document provides definitive details on the COIL binary format. The specification is designed to be unambiguous, consistent, and implementation-friendly while maintaining broad platform support.

## Instruction Encoding

Every COIL instruction consists of:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

### Opcode Field (8 bits)
- **0x00-0xBF**: Universal instructions (available on all processor types)
- **0xC0-0xFE**: Processor-specific instructions
- **0xFF**: Implementation-specific extensions

### Operand Count Field (8 bits)
- Indicates the number of operands that follow (0 to 255)
- Most instructions have a fixed operand count per opcode

## Operand Format

Each operand follows a standard format:

```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

### Type Field (16 bits)
- **First 8 bits**: Main Type - Primary type category
- **Second 8 bits**: Type Extensions - Qualifiers (const, volatile, immediate)

## Value Encoding

Different types encode their values differently:

### Immediate Values
- Integer: Little-endian encoded with size based on type
- Float: IEEE 754 format
- String: Length-prefixed UTF-8 encoding

### References
- Variable references: `0x9000` followed by variable ID (8 or 16 bits)
- Symbol references: `0x9100` followed by symbol table index (16 bits)
- Register references: `0x9200` followed by register ID (8 bits)

## File Format

### COIL Object File (.coil)

Header structure:
```
struct CoilHeader {
    char     magic[4];        // "COIL"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t symbol_offset;   // Offset to symbol table
    uint32_t section_offset;  // Offset to section table
    uint32_t reloc_offset;    // Offset to relocation table
    uint32_t file_size;       // Total file size
}
```

### COIL Output Object File (.coilo)

Similar to COIL object file, but with resolved relocations and possibly optimized format.

## Symbol Table

```
struct SymbolTable {
    uint32_t count;           // Number of symbols
    Symbol   entries[];       // Array of symbol entries
}

struct Symbol {
    uint16_t name_length;     // Length of symbol name
    char     name[];          // Symbol name (not null-terminated)
    uint32_t attributes;      // Symbol attributes
    uint32_t value;           // Symbol value (address or data)
    uint16_t section_index;   // Index of containing section
    uint8_t  processor_type;  // Target processor type
}
```

Symbol attributes (bitfield):
- `0x01`: Global symbol
- `0x02`: Local symbol
- `0x04`: Weak symbol
- `0x08`: Function symbol
- `0x10`: Data symbol
- `0x20`: External reference

## Section Table

```
struct SectionTable {
    uint32_t count;           // Number of sections
    Section  entries[];       // Array of section entries
}

struct Section {
    uint16_t name_index;      // Index into symbol table for name
    uint32_t attributes;      // Section attributes
    uint32_t offset;          // Offset from start of file
    uint32_t size;            // Size in bytes
    uint32_t address;         // Virtual address (0 if not set)
    uint32_t alignment;       // Required alignment
    uint8_t  processor_type;  // Target processor type
}
```

Section attributes (bitfield):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)

## Relocation Table

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

## Binary Format Example

```
; COIL header
"COIL"                 ; Magic number
0x01, 0x00, 0x00       ; Version 1.0.0
0x01                   ; Object file flag
0x00000020             ; Symbol table offset
0x0000004C             ; Section table offset
0x00000064             ; Relocation table offset
0x0000007C             ; File size

; MOV instruction example - MOV #1, 42
0x10                   ; Opcode for MOV
0x02                   ; Two operands
0x9000                 ; TYPE_VAR
0x01                   ; Variable ID 1
0x1320                 ; TYPE_UNT32+IMM
0x2A000000             ; Value 42 (little-endian)
```

## Format Compatibility and Versioning

The binary format includes version information to allow for future extensions while maintaining compatibility:

1. Major version changes indicate incompatible format changes
2. Minor version changes indicate backward-compatible additions
3. Processors must check version compatibility

## Implementation Requirements

A conforming COIL processor must:
1. Correctly parse all fields in the binary format
2. Validate type compatibility for operations
3. Correctly interpret all universal opcodes (0x00-0xBF)
4. Implement proper error handling for invalid instructions
5. Respect memory ordering and alignment requirements