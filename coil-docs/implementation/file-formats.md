# File Formats

## Overview

The COIL ecosystem uses several standardized file formats for different stages of the compilation and execution process. This document defines these formats and their purposes.

## CASM Source Format (`.casm`)

CASM files are plain text files containing COIL assembly language code:

```
; Example CASM file
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture

SECTION .text, 0x01 | 0x04
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_INT32, 42
    ; ...
    SCOPEL
    RET
```

Key characteristics:
- Text-based format
- Line-oriented syntax
- Comments begin with semicolon (`;`)
- Labels end with colon (`:`)
- Directives in uppercase (`PROC`, `SECTION`, etc.)
- Support for various numeric formats (`42`, `0xFF`, `0b1010`)

## COIL Object Format (`.coil`)

COIL object files contain compiled but unlinked code:

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
- `0x01`: Object file format
- `0x04`: Contains debug information
- `0x08`: Big-endian encoding (default is little-endian)

Symbol table format:
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

Section table format:
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

## COIL Output Object Format (`.coilo`)

COILO files contain processed COIL objects with native opcodes for the target architecture:

```
struct CoilOHeader {
    char     magic[4];        // "CILO"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t symbol_offset;   // Offset to symbol table
    uint32_t section_offset;  // Offset to section table
    uint32_t meta_offset;     // Offset to metadata section
    uint32_t file_size;       // Total file size
}
```

COILO flags:
- `0x01`: Contains native code
- `0x02`: Contains CBC sections
- `0x04`: Contains debug information
- `0x08`: Big-endian encoding (default is little-endian)

COILO files can contain:
1. Native opcodes for the target architecture
2. COIL code sections for delayed processing
3. CBC bytecode sections for interpretation
4. Metadata for linking and execution

COILO files are **not** directly executable. They must be processed by an OS-specific linker to create executables (.exe, .out, etc.) or libraries (.dll, .so, etc.).

## CBC Bytecode Format (`.cbc`)

CBC bytecode files contain interpreter-friendly code:

```
struct CBCHeader {
    char     magic[4];        // "CBC\0"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t constant_offset; // Offset to constant pool
    uint32_t code_offset;     // Offset to code section
    uint32_t metadata_offset; // Offset to metadata (0 if none)
    uint32_t file_size;       // Total file size
}
```

CBC flags:
- `0x01`: Contains debug information
- `0x02`: Uses extended constant pool
- `0x04`: Contains metadata
- `0x08`: Big-endian encoding (default is little-endian)

## Debug Information Format (`.coild`)

Debug information can be stored in separate `.coild` files:

```
struct CoilDHeader {
    char     magic[4];        // "CILD"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t debug_offset;    // Offset to debug info
    uint32_t symbols_offset;  // Offset to symbol table
    uint32_t strings_offset;  // Offset to string table
    uint32_t file_size;       // Total file size
}
```

Debug information includes:
- Source file mappings
- Line number tables
- Variable information
- Type information
- Scope information
- Function information

## Target Definition Format (`.target`)

Target definition files describe device capabilities:

```
# x86-64.target
DEVICE 0x01, 0x01, 0x03      # CPU, x86, 64-bit
  CAPS 0x01 | 0x02 | 0x04    # Capabilities
  MEM 0x01                   # Memory model
  FEAT SSE, SSE2, SSE3, AVX  # Features
  # ...
ENDDEV
```

Target files define:
- Device type, architecture, and mode
- Supported capabilities and features
- Register sets
- ABI definitions
- Memory models
- Specific limitations

## Binary Encoding Examples

### COIL Instruction Encoding

```
; MOV instruction (MOV #1, 42)
0x10                   ; Opcode for MOV
0x02                   ; Two operands
0x9000 0x01            ; TYPE_VAR + Variable ID 1
0x1320 0x2A000000      ; TYPE_UNT32+IMM + Value 42 (little-endian)
```

### CBC Instruction Encoding

```
; var.i32 #1, 42
2E 14 01 0000002A      ; Opcode, type+format, var ID, value
```

## Format Compatibility

File format compatibility follows these principles:

1. **Version compatibility**: Newer tools can read older formats
2. **Format evolution**: Formats may evolve with backward compatibility
3. **Extension mechanism**: Formats include extension mechanisms
4. **Tool interoperability**: Different tools can exchange formats

When formats change:
- Minor version updates for backward-compatible changes
- Major version updates for incompatible changes
- Flags indicate special format features

## Format Validation

Validation tools ensure format correctness:

```
coilvalidate input.coil
coilvalidate input.coilo
cbcvalidate input.cbc
```

These tools check:
- Magic numbers and version compatibility
- Structure integrity
- Reference validity
- Content compliance with specifications