# COIL Binary Format

## Overview

This document specifies the binary encoding format for COIL instructions, operands, and program structure.

## Instruction Encoding

All COIL instructions follow this basic structure:

```
[opcode: uint8_t] [extended opcode: optional uint8_t] [operand_count: uint8_t] [[operand: Operand], ...operand_count]
```

### Opcode

The first byte of every instruction is the primary opcode:

- **0x00-0x9F**: Universal operations
- **0xA0-0xBF**: Processing unit operations
- **0xC0-0xDF**: Architecture-specific operations
- **0xE0-0xEF**: Mode-specific operations
- **0xF0-0xFE**: Directive operations
- **0xFF**: Extension operations

### Extended Opcode

Some instructions require an extended opcode:
- When the primary opcode is 0xFF, the extended opcode specifies the vendor-specific operation
- When additional precision is needed within an instruction category

### Operand Count

The operand count is a single byte that specifies the number of operands that follow.

Notable exceptions:
- Instructions that never accept operands (e.g., NOP) may omit the operand count
- Certain directive operations have fixed operand formats

## Operand Encoding

Operands are encoded as:

```
[type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]
```

### Type Opcode

The type opcode specifies the basic type:
- **0x00-0x0F**: Fixed-width integral types
- **0x10-0x1F**: Fixed-width floating-point types
- **0x20-0x2F**: Fixed-width vector types
- **0x30-0x3F**: Complex types
- **0xA0-0xAF**: Composite types
- **0xF0-0xF9**: Optimized types
- **0xFA-0xFF**: Special types

### Type Extension

The type extension is a bitmap that modifies the type:
```
CONST     = (1 << 0)  // Ensure no modifications
VOLATILE  = (1 << 1)  // Do not optimize expressions with this value
ATOMIC    = (1 << 2)  // Use atomic variant if possible
SATURATE  = (1 << 3)  // Cap at largest value instead of overflow
VARIABLE  = (1 << 6)  // Refers to a variable id
SYMBOL    = (1 << 7)  // Refers to a named memory address
```

### Type Data

Additional data required to fully specify the type:
- For complex types: size information
- For composite types: structure definition
- For vector types: element type and count

### Data

The actual value or reference:
- For immediate values: literal data of the specified type
- For variables: variable ID (uint64_t)
- For symbols: symbol index (uint64_t)

## File Structure

A COIL binary file consists of:

```
[File Header]
[Section Table]
[Symbol Table]
[String Table]
[Code Section]
[Data Section]
[Debug Section]
```

### File Header

```
struct CoilHeader {
    char     magic[4];      // "COIL"
    uint32_t version;       // Version in format 0xMMmmpp (Major.minor.patch)
    uint32_t flags;         // File flags
    uint32_t section_count; // Number of sections
    uint64_t section_offset; // Offset to section table
    uint64_t symbol_offset;  // Offset to symbol table
    uint64_t string_offset;  // Offset to string table
}
```

### Section Table

Each section entry follows:

```
struct SectionEntry {
    uint32_t type;       // Section type
    uint32_t flags;      // Section flags
    uint64_t offset;     // Offset from file start
    uint64_t size;       // Section size in bytes
    uint64_t addr;       // Virtual address (if applicable)
    uint32_t name_idx;   // Index into string table
}
```

Common section types:
- 0x01: Code section
- 0x02: Data section
- 0x03: Read-only data
- 0x04: BSS (uninitialized data)
- 0x05: Debug information

## Related Components

- [Instruction Reference](../instruction-reference.md)
- [Type System](../types/type-system.md)