# COIL Object Format

## Overview

The COIL Object Format defines the structure of compiled COIL programs, enabling modular development, linking, and execution across different platforms. This format serves as the container for compiled code, data, and metadata necessary for execution.

## File Structure

A COIL object file consists of the following components:

```
[File Header]
[Section Table]
[Symbol Table]
[String Table]
[Sections...]
```

### File Header

The file header identifies the file as a COIL object and provides essential metadata:

```c
struct CoilObjectHeader {
    char     magic[4];         // "COIL"
    uint32_t version;          // Version in format 0xMMmmpp
    uint32_t flags;            // File flags
    uint32_t target_pu;        // Target processing unit
    uint32_t target_arch;      // Target architecture
    uint32_t target_mode;      // Target mode
    uint64_t entry_point;      // Entry point (or 0 if not executable)
    uint32_t section_count;    // Number of sections
    uint32_t symbol_count;     // Number of symbols
    uint64_t section_offset;   // Offset to section table
    uint64_t symbol_offset;    // Offset to symbol table
    uint64_t string_offset;    // Offset to string table
}
```

Common file flags:
- `0x0001`: Executable
- `0x0002`: Shared object
- `0x0004`: Position independent
- `0x0008`: Debug information included

### Section Table

The section table maps the various sections within the file:

```c
struct SectionEntry {
    uint32_t type;       // Section type
    uint32_t flags;      // Section flags
    uint64_t offset;     // Offset from file start
    uint64_t size;       // Section size in bytes
    uint64_t addr;       // Virtual address (if applicable)
    uint64_t align;      // Section alignment
    uint32_t name_idx;   // Index into string table
    uint32_t link;       // Related section (depends on type)
    uint32_t info;       // Additional information
}
```

Common section types:
- `0x01`: Code section
- `0x02`: Data section
- `0x03`: Read-only data
- `0x04`: BSS (uninitialized data)
- `0x05`: Symbol table
- `0x06`: String table
- `0x07`: Relocation table
- `0x08`: Debug information

Section flags:
- `0x0001`: Writeable
- `0x0002`: Executable
- `0x0004`: Initialized
- `0x0008`: Occupies memory during execution

### Symbol Table

The symbol table defines global symbols and their attributes:

```c
struct SymbolEntry {
    uint32_t name_idx;   // Index into string table
    uint32_t section_idx; // Section containing the symbol
    uint64_t value;      // Symbol value (offset or address)
    uint64_t size;       // Symbol size in bytes
    uint16_t type;       // Symbol type
    uint16_t bind;       // Symbol binding
    uint16_t visibility; // Symbol visibility
}
```

Symbol types:
- `0x00`: No type
- `0x01`: Function
- `0x02`: Data object
- `0x03`: Section
- `0x04`: File

Symbol binding:
- `0x00`: Local
- `0x01`: Global
- `0x02`: Weak

Symbol visibility:
- `0x00`: Default
- `0x01`: Internal
- `0x02`: Hidden
- `0x03`: Protected

### String Table

The string table contains null-terminated strings referenced by other tables:

```
[String 1]\0[String 2]\0...[String N]\0
```

Strings are referenced by their byte offset from the beginning of the string table.

### Relocation Table

The relocation table specifies how to adjust references when linking:

```c
struct RelocationEntry {
    uint64_t offset;     // Location to apply relocation
    uint32_t symbol_idx; // Symbol table index
    uint32_t type;       // Relocation type
    int64_t addend;      // Constant to add to symbol value
}
```

Relocation types are architecture-specific and defined in the respective ISA-E documentation.

## Linking Process

The linking process combines multiple COIL object files:

1. **Symbol Resolution**: Matching external references with their definitions
2. **Relocation**: Adjusting addresses based on the final layout
3. **Section Merging**: Combining like sections from different objects
4. **Output Generation**: Creating a final executable or shared object

## Executable vs. Shared Objects

COIL supports both executable and shared object formats:

### Executable
- Contains a valid entry point
- All symbols are resolved
- Fixed virtual addresses

### Shared Object
- No entry point (or marked as dynamic entry)
- May contain unresolved symbols
- Position-independent code
- Dynamic symbol table for runtime linking

## Platform-Specific Considerations

The COIL object format adapts to different platforms:

1. **Endianness**: Object files use the endianness of the target platform
2. **Alignment**: Sections align according to target architecture requirements
3. **Symbol Naming**: Platform-specific name mangling may be applied

## Related Components

- [Binary Format](./binary-format.md) - COIL binary encoding specification
- [Configuration Format](../core/config-format.md) - Target configuration specification
- [Memory Model](../core/memory-model.md) - Memory management and organization