# COIL Object Format

## Purpose

This document defines the binary object file format used to contain COIL programs, symbols, and metadata. The COIL object format provides a standardized container that enables linking, loading, and execution across different implementations.

## Object Format vs. Instruction Encoding

It's essential to distinguish between two related but distinct concepts:

1. **Instruction Binary Encoding**: The byte-level encoding of individual COIL instructions
2. **COIL Object Format**: The container format that organizes encoded instructions along with metadata

The object format is the "file format" for COIL programs, while instruction encoding defines how individual instructions are represented within that file.

## Primary Object Formats

COIL uses two primary object formats:

1. **COIL Object (.coil)** - Standard format containing COIL instructions, symbols, and metadata
2. **COIL Output Object (.coilo)** - Format containing processor-specific binary code generated from COIL

## COIL Object (.coil)

The COIL object is the standard container for COIL code. It's designed to be portable across different implementations and processor types.

### File Structure

A COIL object file consists of the following components:

```
+------------------+
| File Header      |
+------------------+
| Section Table    |
+------------------+
| Symbol Table     |
+------------------+
| String Table     |
+------------------+
| Type Table       |
+------------------+
| Code Sections    |
+------------------+
| Data Sections    |
+------------------+
| Relocation Table |
+------------------+
| Debug Info       |
+------------------+
| Metadata         |
+------------------+
```

### File Header

The file header identifies the file as a COIL object and provides basic information:

```c
struct CoilHeader {
    char      magic[4];      // "COIL"
    uint8_t   major_version; // Major version (1 for COIL v1)
    uint8_t   minor_version; // Minor version
    uint8_t   patch_version; // Patch version
    uint8_t   flags;         // Flags (executable, relocatable, etc.)
    uint32_t  section_count; // Number of sections
    uint32_t  section_table; // Offset to section table
    uint32_t  symbol_count;  // Number of symbols
    uint32_t  symbol_table;  // Offset to symbol table
    uint32_t  string_table;  // Offset to string table
    uint32_t  type_table;    // Offset to type table
    uint32_t  entry_point;   // Entry point symbol (if executable)
    uint32_t  target_proc;   // Target processor
    uint32_t  target_arch;   // Target architecture
    uint32_t  target_mode;   // Target mode
    uint32_t  reserved[2];   // Reserved for future use
}
```

#### Header Flags

- `0x01` - EXECUTABLE: File is an executable
- `0x02` - RELOCATABLE: File contains relocations
- `0x04` - SHARED: File is a shared object
- `0x08` - DEBUG: File contains debug information

### Section Table

The section table lists all sections in the file:

```c
struct SectionEntry {
    uint32_t name;       // Offset into string table
    uint32_t type;       // Section type
    uint32_t flags;      // Section flags
    uint32_t offset;     // Offset in file
    uint32_t size;       // Size in bytes
    uint32_t addr;       // Virtual address (if loaded)
    uint32_t alignment;  // Alignment requirement
    uint32_t reserved;   // Reserved for future use
}
```

#### Section Types

```
0x00 - NULL       // Unused section entry
0x01 - CODE       // Executable code
0x02 - DATA       // Initialized data
0x03 - BSS        // Uninitialized data (zero-initialized)
0x04 - SYMBOL     // Symbol table
0x05 - STRING     // String table
0x06 - TYPE       // Type information
0x07 - RELOC      // Relocation entries
0x08 - DEBUG      // Debug information
0x09 - COMMENT    // Comment or metadata
0x0A - CUSTOM     // Custom section
```

#### Section Flags

```
0x01 - EXECUTABLE // Section contains executable code
0x02 - WRITABLE   // Section is writable
0x04 - READABLE   // Section is readable
0x08 - INITIALIZED // Section contains initialized data
0x10 - LOADABLE   // Section should be loaded into memory
0x20 - SHARED     // Section can be shared between processes
```

### Symbol Table

The symbol table contains information about all symbols in the file:

```c
struct SymbolEntry {
    uint32_t name;       // Offset into string table
    uint32_t value;      // Symbol value or offset
    uint32_t size;       // Symbol size
    uint8_t  type;       // Symbol type
    uint8_t  binding;    // Symbol binding
    uint8_t  section;    // Section index
    uint8_t  flags;      // Symbol flags
}
```

#### Symbol Types

```
0x00 - NOTYPE    // Unspecified type
0x01 - FUNCTION  // Function or procedure
0x02 - VARIABLE  // Variable or data
0x03 - SECTION   // Section
0x04 - FILE      // Source file
0x05 - ABI       // ABI definition
```

#### Symbol Bindings

```
0x00 - LOCAL     // Local symbol (not visible outside file)
0x01 - GLOBAL    // Global symbol (visible to other files)
0x02 - WEAK      // Weak symbol (can be overridden)
0x03 - EXTERN    // External symbol (defined elsewhere)
```

### String Table

The string table contains null-terminated strings referenced by other tables.

Offsets into the string table are relative to the beginning of the table.

### Type Table

The type table contains definitions of complex types used in the program:

```c
struct TypeEntry {
    uint16_t type_id;    // Type ID
    uint16_t flags;      // Type flags
    uint32_t size;       // Type size in bytes
    uint32_t align;      // Type alignment requirement
    uint32_t elements;   // Number of elements (for arrays)
    uint32_t fields;     // Number of fields (for structures)
    uint32_t data;       // Offset to type-specific data
}
```

Type-specific data varies depending on the type (structure fields, array elements, etc.).

### Code Sections

Code sections contain COIL instructions in their binary format:

```
[Instruction 1][Instruction 2]...[Instruction N]
```

Each instruction follows the standard COIL binary format:
```
[Opcode (8 bits)][Operand Count (8 bits)][Operands...]
```

### Data Sections

Data sections contain initialized data:

```
[Byte 1][Byte 2]...[Byte N]
```

### Relocation Table

The relocation table lists locations in the code or data that need to be adjusted during linking:

```c
struct RelocationEntry {
    uint32_t offset;     // Offset in section
    uint32_t symbol;     // Symbol index
    uint32_t type;       // Relocation type
    uint32_t addend;     // Addend value
}
```

#### Relocation Types

```
0x01 - ABS32     // 32-bit absolute address
0x02 - REL32     // 32-bit relative address
0x03 - ABS64     // 64-bit absolute address
0x04 - REL64     // 64-bit relative address
```

### Debug Information

Debug information is optional and follows the format specified in the Debug Format document. This includes:

- Line number tables
- Variable information
- Type details
- Source file mappings

## COIL Output Object (.coilo)

The COIL Output Object is an optional format containing processor-specific binary code generated from COIL. This format is used by implementations that translate COIL to native code.

### File Structure

A COILO file extends the COIL format:

```
+------------------+
| COILO Header     |
+------------------+
| Section Table    |
+------------------+
| Symbol Table     |
+------------------+
| String Table     |
+------------------+
| Native Code      |
+------------------+
| Native Data      |
+------------------+
| Relocation Table |
+------------------+
| Debug Info       |
+------------------+
| Original COIL    | (Optional)
+------------------+
| Metadata         |
+------------------+
```

### COILO Header

The COILO header extends the COIL header:

```c
struct CoiloHeader {
    char      magic[4];      // "CILO"
    uint8_t   major_version; // Major version
    uint8_t   minor_version; // Minor version
    uint8_t   patch_version; // Patch version
    uint8_t   flags;         // Flags
    uint32_t  section_count; // Number of sections
    uint32_t  section_table; // Offset to section table
    uint32_t  symbol_count;  // Number of symbols
    uint32_t  symbol_table;  // Offset to symbol table
    uint32_t  string_table;  // Offset to string table
    uint32_t  entry_point;   // Entry point symbol
    uint32_t  target_proc;   // Target processor
    uint32_t  target_arch;   // Target architecture
    uint32_t  target_mode;   // Target mode
    uint32_t  orig_coil;     // Offset to original COIL (if present)
    uint32_t  processor_id;  // COIL processor identifier
    uint32_t  opt_level;     // Optimization level
    uint32_t  reserved[2];   // Reserved for future use
}
```

### Native Code and Data

Native code sections contain processor-specific machine code:

```
[Machine Code Byte 1][Machine Code Byte 2]...[Machine Code Byte N]
```

The format depends on the target processor and architecture.

### Original COIL (Optional)

COILO files may optionally include the original COIL code that was used to generate the native code. This can be useful for:
- Re-optimization for different targets
- Decompilation for debugging
- Verification
- JIT compilation for sections that require it

## Other COIL-Related File Formats

### COIL Header (.coilh)

COIL header files contain declarations that can be included in other COIL files. They typically contain:
- Symbol declarations
- ABI definitions
- Type definitions
- Constant definitions

### COIL Archive (.coila)

COIL archive files are collections of multiple COIL objects, similar to static libraries in other systems. They contain:
- Archive header
- Archive symbol table
- Multiple COIL object files

## Object File Generation

COIL object files are typically generated in the following process:

1. **Assembling**: Converting CASM text to COIL object format
   ```
   coilasm source.casm -o output.coil
   ```

2. **Linking**: Combining multiple COIL objects into a single executable or library
   ```
   coillink main.coil lib1.coil lib2.coil -o program.coil
   ```

3. **Processing**: Translating COIL to native code for a specific target
   ```
   coilproc -t x86_64 program.coil -o program.coilo
   ```

## Version Information

Object files include version information in their headers:
- Major version: Incompatible changes (e.g., 1 for COIL v1)
- Minor version: Compatible additions
- Patch version: Bug fixes and clarifications

COIL processors should check version compatibility:
```c
if (header.major_version > SUPPORTED_MAJOR) {
    error("Unsupported COIL version");
} else if (header.major_version == SUPPORTED_MAJOR) {
    if (header.minor_version > SUPPORTED_MINOR) {
        warning("Some features may not be supported");
    }
}
```

## Example File Structure

Here's a hexdump example showing the beginning of a simple COIL object file:

```
0000000: 434f 494c 0100 0001 0500 0000 1c00 0000  COIL............
0000010: 0300 0000 4400 0000 5800 0000 6c00 0000  ....D...X...l...
0000020: 0000 0000 0100 0000 0100 0000 0300 0000  ................
0000030: 0000 0000 0000 0000 0600 0000 2e74 6578  .............tex
0000040: 7400 0100 0000 0500 0000 7c00 0000 3400  t.........|...4.
0000050: 0000 0000 0000 0400 0000 0000 0000 0600  ................
...
```

This shows:
- "COIL" magic number
- Version 1.0.0
- Flags 0x01 (executable)
- 5 sections
- Section table at offset 0x1C
- First section named ".text" of type CODE (0x01)

## Related Documentation

- [Binary Encoding](binary-encoding.md) - Detailed instruction encoding rules
- [Debug Format](debug-format.md) - Debug information specification
- [CASM Syntax](casm-syntax.md) - Text representation of COIL
- [Implementation Requirements](../implementation/requirements.md) - Requirements for COIL processors