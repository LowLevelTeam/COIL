# COIL Binary Format

## Overview
The COIL binary format is the native representation of COIL programs. This document provides the concrete specification needed to implement COIL processors, assemblers, and linkers.

## File Format

### COIL Header
Every COIL binary file begins with this header:

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
    uint32_t debug_offset;    // Offset to debug info (0 if none)
    uint32_t file_size;       // Total file size
}
```

Format flags:
- `0x01`: Object file (.coil)
- `0x02`: Output object file (.coilo)
- `0x04`: Contains debug information
- `0x08`: Big-endian encoding (default is little-endian)

### Instruction Encoding
Every COIL instruction follows this structure:
```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Opcode ranges:
- `0x00-0xBF`: Universal instructions (available on all processor types)
- `0xC0-0xFE`: Processor-specific instructions
- `0xFF`: Implementation-specific extensions

### Operand Format
Each operand follows this format:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

Type field (16 bits) consists of:
- First 8 bits: Main Type (primary type category)
- Second 8 bits: Type Extensions (qualifiers like const, volatile, immediate)

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

Symbol attributes (bit flags):
- `0x0001`: Global symbol (visible outside the file)
- `0x0002`: Weak symbol (can be overridden)
- `0x0004`: Local symbol (file scope only)
- `0x0008`: Function symbol
- `0x0010`: Data symbol
- `0x0020`: Absolute symbol (fixed address)
- `0x0040`: Common symbol (uninitialized)
- `0x0080`: Exported symbol

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

Section attributes (bit flags):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)
- `0x20`: Linked section (contains relocations)
- `0x40`: Discardable (can be removed)

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

Relocation types:
- `0x01`: Absolute (fill with symbol value)
- `0x02`: Relative (fill with symbol value - current location)
- `0x03`: PC-relative (for branch instructions)
- `0x04`: Section-relative
- `0x05`: Symbol+Addend

## File Types

### COIL Object File (.coil)
- Used during compilation and linking
- Contains relocations and unresolved symbols
- Can be linked with other COIL object files
- Usually not directly executable

### COIL Output Object File (.coilo)
- Final executable format after linking
- All relocations are resolved
- All symbols are resolved
- Can be directly loaded and executed

## Example: Simple Instruction Encoding

MOV instruction (`MOV #1, 42`):
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

## Implementation Requirements

A compliant COIL processor must:
1. Correctly parse the file header and all tables
2. Validate that all referenced symbols and sections exist
3. Enforce memory protection based on section attributes
4. Implement all universal instructions (0x00-0xBF)
5. Either implement or reject processor-specific instructions
6. Handle all specified relocation types
7. Ensure proper memory alignment for all data types