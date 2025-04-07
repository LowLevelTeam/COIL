# COIL Object Format

## Overview

The COIL Object Format defines the structure and layout of COIL binary objects, providing a consistent representation for compiled COIL programs, libraries, and modules. This format enables linking, loading, and execution across different platforms.

## File Structure

COIL object files have the following structure:

```
┌────────────────┐
│ File Header    │
├────────────────┤
│ Section Table  │
├────────────────┤
│ Symbol Table   │
├────────────────┤
│ String Table   │
├────────────────┤
│ Relocation Table│
├────────────────┤
│ Sections...    │
└────────────────┘
```

## File Header

The file header identifies the file as a COIL object and provides essential metadata:

```c
struct CoilObjectHeader {
    char     magic[4];         // "COIL"
    uint32_t version;          // Version in format 0xMMmmpp (Major, minor, patch)
    uint32_t flags;            // File flags
    uint32_t target_pu;        // Target processing unit
    uint32_t target_arch;      // Target architecture
    uint32_t target_mode;      // Target mode
    uint64_t entry_point;      // Entry point (or 0 if not executable)
    uint32_t section_count;    // Number of sections
    uint32_t symbol_count;     // Number of symbols
    uint32_t reloc_count;      // Number of relocations
    uint64_t section_offset;   // Offset to section table
    uint64_t symbol_offset;    // Offset to symbol table
    uint64_t string_offset;    // Offset to string table
    uint64_t reloc_offset;     // Offset to relocation table
    uint8_t  endianness;       // 0 = little-endian, 1 = big-endian
    uint8_t  padding[7];       // Reserved for future use, must be 0
}
```

### Flag Values

File flags define key object characteristics:

| Flag    | Value  | Description |
|---------|--------|-------------|
| EXEC    | 0x0001 | Executable file |
| SHARED  | 0x0002 | Shared object (DSO/DLL) |
| PIC     | 0x0004 | Position-independent code |
| DEBUG   | 0x0008 | Contains debug information |
| RELOC   | 0x0010 | Relocatable object |
| CPU     | 0x0020 | Contains CPU-specific code |
| GPU     | 0x0040 | Contains GPU-specific code |
| NPU     | 0x0080 | Contains NPU-specific code |
| DSP     | 0x0100 | Contains DSP-specific code |

Flags can be combined with bitwise OR.

### Processing Unit, Architecture, and Mode

These fields identify the target environment:

- **Target PU**: Identifies the processing unit (0x01 for CPU, 0x02 for GPU, etc.)
- **Target Architecture**: Identifies the specific architecture (0x01 for x86, 0x02 for ARM, etc.)
- **Target Mode**: Identifies specific mode (0x01 for 32-bit, 0x02 for 64-bit, etc.)

## Section Table

The section table defines the file sections:

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

### Section Types

| Type Value | Name     | Description |
|------------|----------|-------------|
| 0x00       | NULL     | Unused/null section |
| 0x01       | TEXT     | Executable code (.text) |
| 0x02       | DATA     | Initialized data (.data) |
| 0x03       | RODATA   | Read-only data (.rodata) |
| 0x04       | BSS      | Uninitialized data (.bss) |
| 0x05       | SYMTAB   | Symbol table (.symtab) |
| 0x06       | STRTAB   | String table (.strtab) |
| 0x07       | REL      | Relocation entries (.rel) |
| 0x08       | RELA     | Relocation entries with addend (.rela) |
| 0x09       | INIT     | Initialization code (.init) |
| 0x0A       | FINI     | Finalization code (.fini) |
| 0x0B       | DYNAMIC  | Dynamic linking information (.dynamic) |
| 0x0C       | DEBUG    | Debugging information (.debug) |
| 0x0D       | COMMENT  | Comments (.comment) |

### Section Flags

| Flag Value | Name      | Description |
|------------|-----------|-------------|
| 0x0001     | WRITE     | Section is writable |
| 0x0002     | ALLOC     | Section occupies memory at runtime |
| 0x0004     | EXEC      | Section contains executable instructions |
| 0x0008     | MERGE     | Section may be merged |
| 0x0010     | STRINGS   | Section contains null-terminated strings |
| 0x0020     | INFO      | Section does not contribute to the image |
| 0x0040     | LINK      | Section contains link order information |
| 0x0080     | OS_SPEC   | OS-specific processing required |
| 0x0100     | GROUP     | Section is member of a group |
| 0x0200     | TLS       | Section contains thread-local storage |

Flags can be combined with bitwise OR.

## Symbol Table

The symbol table defines symbols and their attributes:

```c
struct SymbolEntry {
    uint32_t name_idx;   // Index into string table
    uint32_t section_idx; // Section containing the symbol (0=undefined)
    uint64_t value;      // Symbol value (offset or address)
    uint64_t size;       // Symbol size in bytes
    uint16_t type;       // Symbol type
    uint16_t bind;       // Symbol binding
    uint16_t visibility; // Symbol visibility
    uint16_t reserved;   // Reserved for future use, must be 0
}
```

### Symbol Types

| Type Value | Name     | Description |
|------------|----------|-------------|
| 0x00       | NOTYPE   | No type specified |
| 0x01       | FUNC     | Function |
| 0x02       | OBJECT   | Data object (variable) |
| 0x03       | SECTION  | Section |
| 0x04       | FILE     | Source file |
| 0x05       | COMMON   | Common data object |
| 0x06       | TLS      | Thread-local storage object |

### Symbol Binding

| Bind Value | Name     | Description |
|------------|----------|-------------|
| 0x00       | LOCAL    | Not visible outside the object file |
| 0x01       | GLOBAL   | Visible to all objects being combined |
| 0x02       | WEAK     | Like global, but with lower precedence |
| 0x03       | UNIQUE   | Global with guaranteed unique definition |

### Symbol Visibility

| Visibility Value | Name     | Description |
|------------------|----------|-------------|
| 0x00             | DEFAULT  | Default visibility rules apply |
| 0x01             | HIDDEN   | Symbol not visible outside the object |
| 0x02             | PROTECTED| Symbol visible but not preemptable |
| 0x03             | INTERNAL | Symbol visible but with implementation-specific meaning |

## String Table

The string table stores null-terminated strings referenced by other tables:

- Simple list of null-terminated strings
- First byte is always null (index 0 = empty string)
- Other tables reference strings by offset into this table

## Relocation Table

The relocation table defines how references should be updated during linking:

```c
struct RelocationEntry {
    uint64_t offset;    // Location to be modified
    uint32_t symbol_idx; // Symbol referenced
    uint32_t type;      // Relocation type
    uint64_t addend;    // Constant addend (RELA only)
}
```

### Relocation Types

| Type Value | Name     | Description | Formula |
|------------|----------|-------------|---------|
| 0x00       | NONE     | No relocation | - |
| 0x01       | ABS      | Absolute reference | S + A |
| 0x02       | REL      | PC-relative reference | S + A - P |
| 0x03       | GOT      | Global offset table | G + A |
| 0x04       | PLT      | Procedure linkage table | L + A |
| 0x05       | COPY     | Copy symbol at runtime | - |
| 0x06       | GLOB_DAT | Set GOT entry to address | S |
| 0x07       | JMP_SLOT | Set PLT entry to address | S |

Where:
- S = Symbol value
- A = Addend value
- P = Position (offset) being modified
- G = GOT offset of symbol
- L = PLT offset of symbol

## Data Alignment and Padding

COIL object files follow specific alignment requirements:

1. **Section Alignment**: Each section is aligned according to its `align` field
2. **Header Alignment**: The file header is aligned to 8 bytes
3. **Table Alignment**: All tables are aligned to 8 bytes
4. **Padding Bytes**: All padding bytes must be set to 0

## Version Compatibility

Version compatibility follows these rules:

1. **Major Version**: Incompatible changes, requires updated tools
2. **Minor Version**: New features, backward compatible
3. **Patch Version**: Bug fixes, fully compatible

## Implementation Requirements

A compliant COIL object format implementation must:

1. **Validation**: Verify the integrity and correctness of object files
2. **Endianness Handling**: Correctly handle both little and big endian formats
3. **Version Check**: Verify compatibility with supported versions
4. **Error Detection**: Report clear errors for malformed object files
5. **Section Processing**: Process all required sections according to specification
