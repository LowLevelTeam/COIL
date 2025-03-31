# COIL Object Format

## Overview

COIL uses two primary object formats:

1. **COIL Object Format (.coil)**: Intermediate format for linkage
2. **COIL Output Object Format (.coilo)**: Executable or library format after linking

Both formats are inspired by ELF (Executable and Linkable Format) for efficiency and CUDA's PTX intermediate representation for flexibility. This design enables both high performance and portable execution across diverse architectures.

## Format Capabilities

### COIL Object Format (.coil)
- Intermediate representation for compilation stages
- Contains relocations and potentially unresolved symbols
- Designed for linking processes
- Can target any processor type (CPU, GPU, QPU, FPGA, etc.)

### COIL Output Object Format (.coilo)
- Can function as:
  - Executable program
  - Static library (concatenation of multiple COIL objects)
  - Dynamic/shared library
  - Device-specific binary (like a kernel for specialized hardware)
- Has resolved relocations and symbols
- May contain optimization metadata

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
    uint32_t reloc_offset;  // Offset to relocation table (0 for executable .coilo)
    uint32_t file_size;    // Total file size
}
```

Flag values:
- `0x01`: Object file (.coil)
- `0x02`: Output file (.coilo)
- `0x04`: Contains debug information
- `0x08`: Library file (static or dynamic)
- `0x10`: Contains multiple processor targets
- `0x20`: Contains initialization code

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
    uint8_t  processor_type; // Target processor type
}
```

Symbol attributes (bitfield):
- `0x01`: Global symbol
- `0x02`: Local symbol
- `0x04`: Weak symbol
- `0x08`: Function symbol
- `0x10`: Data symbol
- `0x20`: External reference
- `0x40`: Exported symbol (for libraries)
- `0x80`: Initialization function

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
    uint8_t  processor_type; // Target processor type
}
```

Section attributes (bitfield):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)
- `0x20`: Device-specific section
- `0x40`: Shared section (accessible by multiple processors)

## Library Format Support

COIL object formats support creating libraries:

### Static Libraries
- Multiple .coilo objects concatenated with a library header
- Symbol export table for quick reference
- Platform-independent representation
- Can contain code for multiple processor types

### Dynamic Libraries
- Similar to executable .coilo but with export table
- Contains initialization code in a special section
- Supports lazy symbol resolution
- Can be loaded at runtime by COIL-aware loaders

### Library Header
```
struct LibraryHeader {
    uint32_t object_count;        // Number of contained objects
    uint32_t export_table_offset; // Offset to export table
    uint32_t dependency_offset;   // Offset to dependency information
    uint32_t object_offsets[];    // Array of offsets to each object
}
```

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

## Multi-Processor Support

COIL objects can contain code for multiple processor types:

- Each section specifies its target processor
- Multiple sections may target different processors
- Unified symbol table with processor-specific attributes
- Cross-processor references use standard relocation mechanisms

Example of multi-processor code layout:
```
[Header]
[Symbol Table]
[Section Table]
[CPU Code Section]
[GPU Code Section]
[FPGA Code Section]
[Shared Data Section]
[Relocation Table (for .coil only)]
```

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
0x01                   ; Processor type (CPU)
0x05 "hello"           ; Symbol 2: "hello"
0x10                   ; Data attribute
0x00000000             ; Value
0x0002                 ; Section index
0x01                   ; Processor type (CPU)

; Section table (at offset 0x4C)
0x00000002             ; 2 sections
0xFFFF                 ; No name
0x01 | 0x04            ; Executable, readable
0x00000070             ; Offset
0x0000000C             ; Size
0x00000000             ; Address
0x00000001             ; Alignment
0x01                   ; Processor type (CPU)
0xFFFF                 ; No name
0x02 | 0x04 | 0x08     ; Writable, readable, initialized
0x0000007C             ; Offset
0x00000000             ; Size
0x00000000             ; Address
0x00000001             ; Alignment
0x01                   ; Processor type (CPU)

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

## Relationship to Future Formats

While the current COIL object format serves general-purpose needs, future specialized formats may include:

- **CBC (COIL Byte Code)**: An optimized bytecode format for interpreters and JITs with faster parsing and smaller size
- **Device-Specific Formats**: Specialized variations optimized for particular device classes

These formats would be derived from the core COIL format while providing additional optimizations for specific use cases.