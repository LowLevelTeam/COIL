# COIL Debug Information Format

## Overview
The debug information format enables debuggers, profilers, and other tools to map between COIL binary code and the original CASM source, inspect variables, and analyze program behavior.

## Storage Format
Debug information is stored in a dedicated debug section referenced from the COIL object header:

```
struct CoilHeader {
    // Standard header fields...
    uint32_t debug_offset;   // Offset to debug section (0 if none)
}

struct DebugSection {
    uint32_t version;        // Debug format version
    uint32_t tables_offset;  // Offset to debug tables
    uint32_t strings_offset; // Offset to string table
}
```

## Debug Tables
The debug information is organized into specialized tables:

```
struct DebugTables {
    uint32_t source_table_offset;   // Source file information
    uint32_t line_table_offset;     // Line number mapping
    uint32_t type_table_offset;     // Type information
    uint32_t variable_table_offset; // Variable information
    uint32_t scope_table_offset;    // Scope information
    uint32_t function_table_offset; // Function information
}
```

### Source Table
Maps file IDs to source file names:

```
struct SourceTable {
    uint32_t count;           // Number of source files
    SourceEntry entries[];    // Array of source entries
}

struct SourceEntry {
    uint32_t file_id;         // File identifier
    uint32_t name_offset;     // Offset into string table for filename
    uint32_t content_hash;    // Hash of file contents
}
```

### Line Table
Maps binary offsets to source lines:

```
struct LineTable {
    uint32_t count;           // Number of line entries
    LineEntry entries[];      // Array of line entries
}

struct LineEntry {
    uint32_t offset;          // Offset into COIL binary
    uint32_t file_id;         // Source file ID
    uint32_t line;            // Source line number
    uint32_t column;          // Source column number (optional)
    uint8_t  flags;           // Line entry flags
}
```

Line entry flags:
- `0x01`: Statement start
- `0x08`: Function start
- `0x10`: Function end

### Variable Table
Tracks all variables in the program:

```
struct VariableTable {
    uint32_t count;           // Number of variable entries
    VariableEntry entries[];  // Array of variable entries
}

struct VariableEntry {
    uint32_t name_offset;     // Name in string table
    uint32_t type_index;      // Index into type table
    uint32_t scope_index;     // Index into scope table
    uint32_t location_type;   // How the variable is stored
    uint32_t location_data;   // Location-specific data
    uint32_t start_offset;    // Binary offset where variable becomes valid
    uint32_t end_offset;      // Binary offset where variable becomes invalid
    uint16_t var_id;          // Variable ID in CASM
    uint16_t flags;           // Variable flags
}
```

Location types:
- `0x01`: Register
- `0x02`: Stack (BP-relative)
- `0x03`: Stack (SP-relative)
- `0x04`: Static memory

### Type Table
Provides detailed type information:

```
struct TypeTable {
    uint32_t count;           // Number of type entries
    TypeEntry entries[];      // Array of type entries
}

struct TypeEntry {
    uint16_t type_id;         // COIL type ID
    uint16_t flags;           // Type flags
    uint32_t name_offset;     // Name in string table
    uint32_t size;            // Size in bytes
    uint32_t alignment;       // Alignment requirement
    uint32_t parent_type;     // Parent type for derived types
    uint32_t elements;        // Element count for arrays
    uint32_t fields;          // Field count for structs
    uint32_t field_list;      // Offset to field list
}
```

### Function Table
Provides information about functions:

```
struct FunctionTable {
    uint32_t count;           // Number of function entries
    FunctionEntry entries[];  // Array of function entries
}

struct FunctionEntry {
    uint32_t name_offset;     // Name in string table
    uint32_t type_index;      // Index into type table
    uint32_t scope_index;     // Index into scope table
    uint32_t start_offset;    // Binary offset of function start
    uint32_t end_offset;      // Binary offset of function end
    uint32_t param_count;     // Number of parameters
    uint32_t param_list;      // Offset to parameter list
    uint32_t abi_index;       // ABI used by this function
    uint16_t flags;           // Function flags
}
```

## String Table
The string table stores all strings referenced by debug information entries:

```
struct StringTable {
    uint32_t size;            // Total size of string table
    char     data[];          // Null-terminated strings
}
```

Strings are referenced by their byte offset from the beginning of the string table.

## Compression Options
For large debug information sections, compression can be applied:

```
struct CompressedDebugSection {
    uint32_t compression_type; // 0 = none, 1 = zlib, 2 = lz4
    uint32_t uncompressed_size; // Size when decompressed
    uint8_t  compressed_data[]; // Compressed debug section
}
```

## Debug Information Stripping
For production builds, debug information can be:
1. **Fully Stripped**: All debug information removed
2. **Partially Stripped**: Keep function boundaries and names only
3. **Separated**: Placed in separate files (.coild) for later use

## Implementation Requirements
Implementations must:
1. Preserve source-level information during assembly and linking
2. Maintain debug information accuracy through optimization passes
3. Support separating debug information from the executable code