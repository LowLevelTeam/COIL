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
[Relocation Table]
[Sections...]
```

### File Header

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

File flags:
- `0x0001`: Executable
- `0x0002`: Shared object
- `0x0004`: Position independent
- `0x0008`: Debug information included
- `0x0010`: Relocatable object
- `0x0020`: Contains CPU-specific code
- `0x0040`: Contains GPU-specific code
- `0x0080`: Contains NPU-specific code
- `0x0100`: Contains DSP-specific code

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

Section types:
- `0x01`: Code section (.text)
- `0x02`: Data section (.data)
- `0x03`: Read-only data (.rodata)
- `0x04`: BSS (uninitialized data, .bss)
- `0x05`: Symbol table (.symtab)
- `0x06`: String table (.strtab)
- `0x07`: Relocation table (.rel)
- `0x08`: Debug information (.debug)
- `0x09`: Comment section (.comment)
- `0x0A`: Note section (.note)
- `0x0B`: Special section (implementation-defined)

Section flags:
- `0x0001`: Writeable
- `0x0002`: Executable
- `0x0004`: Initialized
- `0x0008`: Occupies memory during execution
- `0x0010`: Mergeable
- `0x0020`: Contains null-terminated strings
- `0x0040`: Contains symbol table
- `0x0080`: TLS section
- `0x0100`: Group section

### Symbol Table

The symbol table defines global symbols and their attributes:

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

Symbol types:
- `0x00`: No type
- `0x01`: Function
- `0x02`: Data object
- `0x03`: Section
- `0x04`: File
- `0x05`: Common block
- `0x06`: TLS entity

Symbol binding:
- `0x00`: Local (not visible outside the object file)
- `0x01`: Global (visible to all objects being combined)
- `0x02`: Weak (visible to all objects but with lower precedence)
- `0x03`: Unique (merged with same-named symbols)

Symbol visibility:
- `0x00`: Default (as defined by binding type)
- `0x01`: Internal (treated as if it has local binding)
- `0x02`: Hidden (visible but not exposed to other components)
- `0x03`: Protected (visible but cannot be overridden)

### String Table

The string table contains null-terminated strings referenced by other tables:

```
[String 1]\0[String 2]\0...[String N]\0
```

Strings are referenced by their byte offset from the beginning of the string table. The first byte of the string table (index 0) must be a null byte, allowing index 0 to represent an empty string.

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

Relocation types are architecture-specific, but common types include:
- `0x01`: Absolute 32-bit address
- `0x02`: Absolute 64-bit address
- `0x03`: PC-relative 32-bit offset
- `0x04`: PC-relative 64-bit offset
- `0x05`: GOT entry relative
- `0x06`: PLT entry relative

## Section Merging Rules

When linking multiple object files, sections are merged according to these rules:

1. **Same Name, Same Type**:
   - Sections with the same name and type are merged into a single section
   - Contents are concatenated based on section type
   - Section attributes are combined (most restrictive wins)
   - Alignment is set to maximum of all input section alignments

2. **Code Sections (.text)**:
   - Contents are concatenated
   - Symbol references are adjusted
   - All attributes preserved (executable, read-only)
   - Function alignment is preserved

3. **Data Sections (.data, .rodata)**:
   - Contents are concatenated
   - Symbol references are adjusted
   - Attributes preserved
   - Data alignment is preserved

4. **BSS Sections (.bss)**:
   - Sizes are summed
   - No actual data is written to the output file
   - Alignment is preserved

5. **Special Sections**:
   - .comment: Concatenated
   - .debug: May be preserved or stripped based on linker options
   - .note: Concatenated, with identical notes merged

6. **Section Padding**:
   - Added as needed to maintain alignment requirements
   - Padding uses zero bytes
   - Alignment must be power of 2 (1, 2, 4, 8, 16, etc.)

## Section Alignment Requirements

Sections must be aligned according to these minimum requirements:

1. **Code Sections**: Aligned to instruction boundary (architecture-dependent)
   - x86: 16-byte alignment recommended
   - ARM: 4-byte alignment required
   - Others: As specified by architecture

2. **Data Sections**: Aligned to the maximum natural alignment of contained data
   - 8-byte alignment recommended minimum

3. **BSS Sections**: Same as data sections

4. **Meta Sections** (symbol table, string table, etc.): 8-byte alignment

5. **Custom Alignment**: Sections can specify stricter alignment requirements

## Symbol Resolution

When linking multiple object files, symbols are resolved according to these precedence rules:

1. **Strong Definitions**: Symbols with global binding
2. **Weak Definitions**: Symbols with weak binding
3. **Undefined References**: References to symbols defined in other modules

### Resolution Algorithm

The symbol resolution algorithm follows these precise steps:

1. **Collect all symbols** from all input objects
2. **Categorize symbols** as:
   - Strong definitions (global binding)
   - Weak definitions (weak binding)
   - Undefined references

3. **For each symbol name**:
   1. If there is exactly one strong definition:
      - Use the strong definition
   2. If there are multiple strong definitions:
      - Error: Multiple definitions of the same symbol
   3. If there are no strong definitions but at least one weak definition:
      - Use the first weak definition encountered
   4. If there are only undefined references:
      - If linking executable: Error: Undefined reference
      - If linking shared object: Leave as undefined

4. **Special Cases**:
   - Common symbols (type 0x05): Treated as weak definitions with allocation
   - Unique symbols (binding 0x03): Merged with same-named symbols

### Symbol Conflicts

For symbols with the same name:

1. **Multiple Strong Definitions**: Error condition
   - Halts linking process
   - Reports conflict details (file names, symbol attributes)

2. **Strong + Weak Definitions**: Strong definition always wins
   - Weak definition is discarded
   - No warning by default

3. **Multiple Weak Definitions**: First encountered wins
   - Other definitions are discarded
   - Warning may be issued

4. **Protected Symbols**: Cannot be overridden by external definitions
   - Treated as local for external references
   - Can still be referenced

5. **Hidden Symbols**: Not visible outside their defining component
   - Cannot be referenced from other objects
   - Used to avoid name conflicts

## Relocation Processing

The linker processes relocations in these steps:

1. **For each relocation entry**:
   1. Find the symbol referenced by the relocation
   2. Determine the final address of the symbol
   3. Calculate the final value based on relocation type and addend
   4. Patch the reference with the calculated value

2. **Relocation Types**:
   - Each relocation type has a specific calculation formula
   - Architecture-specific relocation handling is documented in ISA-E documentation
   - A minimum set of relocations is supported on all platforms

3. **Relocation Overflow**:
   - Check if calculated value fits in the target field
   - If not, issue an error (cannot be ignored)

### Relocation Formula Examples

For an x86_64 system:

1. **Absolute 64-bit (R_X86_64_64)**:
   ```
   Value = S (symbol value) + A (addend)
   ```

2. **PC-relative 32-bit (R_X86_64_PC32)**:
   ```
   Value = S (symbol value) + A (addend) - P (current position)
   ```

3. **GOT-relative (R_X86_64_GOT32)**:
   ```
   Value = G (GOT offset) + A (addend) - P (current position)
   ```

## Executable vs. Shared Objects

COIL supports both executable and shared object formats:

### Executable
- Contains a valid entry point (non-zero entry_point in header)
- All symbols are resolved
- All relocations are applied
- Fixed virtual addresses
- Memory layout defined by format

### Shared Object
- No entry point (or marked as dynamic entry)
- May contain unresolved symbols
- May contain run-time relocations
- Position-independent code (PIC)
- Dynamic symbol table for runtime linking

## Object Format Example

Binary representation of a minimal object file:

```
// File Header
"COIL"                // magic
0x010000              // version 1.0.0
0x0001                // flags (executable)
0x00000000            // target_pu (CPU)
0x00000000            // target_arch (x86)
0x00000003            // target_mode (64-bit)
0x0000000000001000    // entry_point
0x00000003            // section_count (3 sections)
0x00000002            // symbol_count (2 symbols)
0x00000000            // reloc_count (0 relocations)
0x0000000000000040    // section_offset
0x0000000000000088    // symbol_offset
0x00000000000000A8    // string_offset
0x00000000000000C0    // reloc_offset
0x00                  // endianness (little-endian)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // padding

// Section Table (3 entries)
0x00000001, 0x00000006, 0x00000000000000D0, 0x0000000000000020, 0x0000000000001000, 0x0000000000000010, 0x00000001, 0x00000000, 0x00000000  // .text
0x00000002, 0x00000005, 0x00000000000000F0, 0x0000000000000010, 0x0000000000002000, 0x0000000000000008, 0x00000007, 0x00000000, 0x00000000  // .data
0x00000004, 0x00000008, 0x0000000000000000, 0x0000000000000100, 0x0000000000003000, 0x0000000000000008, 0x0000000D, 0x00000000, 0x00000000  // .bss

// Symbol Table (2 entries)
0x00000012, 0x00000001, 0x0000000000000000, 0x0000000000000020, 0x0001, 0x0001, 0x0000, 0x0000  // main
0x00000017, 0x00000002, 0x0000000000000000, 0x0000000000000008, 0x0002, 0x0001, 0x0000, 0x0000  // data_var

// String Table
0x00, 0x2E, 0x74, 0x65, 0x78, 0x74, 0x00, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x00, 0x2E, 0x62, 0x73, 0x73, 0x00, 0x6D, 0x61, 0x69, 0x6E, 0x00, 0x64, 0x61, 0x74, 0x61, 0x5F, 0x76, 0x61, 0x72, 0x00  // "\0.text\0.data\0.bss\0main\0data_var\0"

// .text section (machine code)
// 32 bytes of instruction data

// .data section
// 16 bytes of initialized data
```

## Related Components

- [Binary Format](./binary-format.md) - COIL binary encoding specification
- [Linking Model](../core/linking-model.md) - Linking process and resolution
- [Memory Model](../core/memory-model.md) - Memory management and organization