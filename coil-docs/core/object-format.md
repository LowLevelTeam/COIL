# COIL Object Format

## Overview

COIL uses two primary object formats with distinct purposes:

1. **COIL Object Format (.coil)**: 
   - Intermediate format for linkage
   - Contains relocations and potentially unresolved symbols
   - Used during compilation and linking stages
   - Can target any processor type (CPU, GPU, QPU, FPGA, etc.)

2. **COIL Output Object Format (.coilo)**:
   - Final format after linking
   - Contains resolved symbols and relocations
   - Ready for execution or loading
   - Can function as executable, library, or device code

Both formats share a common structure while differing in their intended use and specific features.

## Format Capabilities

### COIL Object Format (.coil)
- **Purpose**: Intermediate representation for compilation stages
- **Key Features**:
  - Contains relocation information
  - Has potentially unresolved symbols
  - Designed for linking processes
  - Can target any processor type
  - Used by compilers and linkers

### COIL Output Object Format (.coilo)
- **Purpose**: Ready-to-use code format after linking
- **Variants**:
  - **Executable**: Directly runnable program
  - **Static Library**: Collection of multiple compiled objects
  - **Dynamic Library**: Shared code for runtime loading
  - **Device Code**: Specialized binary for accelerators
- **Key Features**:
  - Resolved relocations and symbols
  - May contain optimization metadata
  - Can include initialization sections
  - Format is designed for loading efficiency

## Common Header Structure

Both formats begin with a header:

```
struct CoilHeader {
    char     magic[4];        // "COIL"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t symbol_offset;   // Offset to symbol table
    uint32_t section_offset;  // Offset to section table
    uint32_t reloc_offset;    // Offset to relocation table (0 for executable .coilo)
    uint32_t file_size;       // Total file size
}
```

Flag values:
- `0x01`: Object file (.coil)
- `0x02`: Output file (.coilo)
- `0x04`: Contains debug information
- `0x08`: Library file (static or dynamic)
- `0x10`: Contains multiple processor targets
- `0x20`: Contains initialization code
- `0x40`: Contains JIT compilation information
- `0x80`: Requires runtime resolution

## Symbol Table

The symbol table provides information about named entities:

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
    uint8_t  abi_type;      // Associated ABI type
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
- `0x100`: JIT compilation target
- `0x200`: Device-specific symbol

### Symbol Referencing

Symbols are referenced by their index in the symbol table:

1. The first symbol has index 0
2. In COIL binary, TYPE_SYM (0x91) is followed by a symbol index
3. All references to a symbol use the same index

Example:
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
    uint8_t  device_id;     // Target device ID (for multi-device code)
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
- `0x80`: JIT compilation target
- `0x100`: Runtime loadable
- `0x200`: Version-specific

## Device Table (Multi-Device Format)

For multi-device code, a device table is included:

```
struct DeviceTable {
    uint16_t device_count;  // Number of targeted devices
    Device   entries[];     // Array of device entries
}

struct Device {
    uint8_t  device_id;     // Device identifier
    uint8_t  processor_type; // Processor type
    uint16_t section_count; // Number of sections for this device
    uint16_t section_indices[]; // Indices into section table
}
```

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
    uint32_t jit_info_offset;     // Offset to JIT information (0 if none)
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
    uint16_t addend;          // Additional value for relocation
}
```

Relocation types:
- `0x01`: Absolute
- `0x02`: Relative
- `0x03`: PC-relative
- `0x04`: Section-relative
- `0x05`: Device-relative
- `0x06`: JIT-dependent

## JIT Compilation Information

For code that requires runtime compilation:

```
struct JITInfoTable {
    uint32_t count;         // Number of JIT entries
    JITInfo  entries[];     // Array of JIT compilation info
}

struct JITInfo {
    uint16_t section_index; // Section requiring JIT compilation
    uint8_t  reason_code;   // Reason for JIT compilation
    uint8_t  options;       // Compilation options
    uint32_t dependency_offset; // Offset to dependency information
}
```

JIT reason codes:
- `0x01`: Device-specific optimization
- `0x02`: Runtime feature detection
- `0x03`: Platform adaptation
- `0x04`: Library specialization
- `0x05`: Dynamic code generation

## Runtime Resolution Information

For symbols that must be resolved at runtime:

```
struct RuntimeResolutionTable {
    uint32_t count;           // Number of runtime resolutions
    Resolution entries[];     // Array of resolution entries
}

struct Resolution {
    uint16_t symbol_index;    // Symbol requiring resolution
    uint8_t  resolution_type; // How to resolve
    uint8_t  flags;           // Resolution flags
    uint32_t fallback_offset; // Fallback implementation
}
```

Resolution types:
- `0x01`: Standard library lookup
- `0x02`: Device capability check
- `0x03`: Feature-dependent selection
- `0x04`: Runtime linking

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
[Device Table]
[CPU Code Section]
[GPU Code Section]
[FPGA Code Section]
[Shared Data Section]
[JIT Information]
[Relocation Table (for .coil only)]
```

## Loading and Execution Models

COIL objects support different loading and execution models:

### Static Loading
- All symbols resolved at link time
- Fixed memory layout
- No runtime resolution

### Dynamic Loading
- Symbols resolved at load time
- Position-independent code
- Support for shared libraries

### JIT Compilation
- Code sections compiled at runtime
- Device-specific optimization
- Feature-dependent code generation

### Hybrid Model
- Combination of pre-compiled and JIT-compiled sections
- Used for heterogeneous and adaptable environments
- Balances predictability with runtime adaptability