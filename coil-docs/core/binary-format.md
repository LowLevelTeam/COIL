# COIL Binary Format

## Overview

The COIL binary format is the native representation of COIL programs. This document covers instruction encoding, operand formats, and file structures.

## Instruction Encoding

Every COIL instruction follows this structure:
```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

### Opcode Field (8 bits)
- **0x00-0xBF**: Universal instructions (available on all processor types)
- **0xC0-0xFE**: Processor-specific instructions
- **0xFF**: Implementation-specific extensions

### Operand Count Field (8 bits)
- Indicates how many operands follow (0-255)
- Most instructions have a fixed number of operands

## Operand Format

Each operand follows this format:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

### Type Field (16 bits)
- **First 8 bits**: Main Type (primary type category)
- **Second 8 bits**: Type Extensions (qualifiers like const, volatile, immediate)

### Value Encoding

Different types encode values differently:
- **Integers**: Little-endian with size based on type
- **Floats**: IEEE 754 format
- **References**: Encoded as IDs or indices
  - Variable references: `0x9000` followed by variable ID
  - Symbol references: `0x9100` followed by symbol table index
  - Register references: `0x9200` followed by register ID

## File Formats

COIL uses two primary file formats:

### COIL Object File (.coil)

Used for intermediate compilation steps with this header:
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

Final executable format with resolved relocations.

## Symbol and Section Tables

### Symbol Table

Contains information about all named entities:
```
struct SymbolTable {
    uint32_t count;           // Number of symbols
    Symbol   entries[];       // Array of symbol entries
}

struct Symbol {
    uint16_t name_length;     // Length of symbol name
    char     name[];          // Symbol name
    uint32_t attributes;      // Symbol attributes
    uint32_t value;           // Symbol value
    uint16_t section_index;   // Section index
    uint8_t  processor_type;  // Target processor
}
```

### Section Table

Defines memory regions in the program:
```
struct SectionTable {
    uint32_t count;           // Number of sections
    Section  entries[];       // Array of section entries
}

struct Section {
    uint16_t name_index;      // Symbol table index for name
    uint32_t attributes;      // Section attributes
    uint32_t offset;          // Offset from file start
    uint32_t size;            // Size in bytes
    uint32_t address;         // Virtual address
    uint32_t alignment;       // Required alignment
    uint8_t  processor_type;  // Target processor
}
```

Section attributes:
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)

## Relocation Table

Used to resolve symbol references during linking:
```
struct RelocationTable {
    uint32_t count;           // Number of relocations
    Relocation entries[];     // Array of relocation entries
}

struct Relocation {
    uint32_t offset;          // Offset within section
    uint16_t symbol_index;    // Symbol table index
    uint16_t section_index;   // Section table index
    uint8_t  type;            // Relocation type
    uint8_t  size;            // Relocation size
}
```

## Example Binary Encoding

Simple MOV instruction (`MOV #1, 42`):
```
0x10                   ; Opcode for MOV
0x02                   ; Two operands
0x9000 0x01            ; TYPE_VAR + Variable ID 1
0x1320 0x2A000000      ; TYPE_UNT32+IMM + Value 42 (little-endian)
```

ADD instruction (`ADD #1, #2, #3`):
```
0x60                   ; Opcode for ADD
0x03                   ; Three operands
0x9000 0x01            ; TYPE_VAR + Variable ID 1
0x9000 0x02            ; TYPE_VAR + Variable ID 2
0x9000 0x03            ; TYPE_VAR + Variable ID 3
```

## Multi-Processor Support

COIL objects can contain code for multiple processor types:
- Each section specifies its target processor
- Multiple sections may target different processors
- Unified symbol table with processor-specific attributes