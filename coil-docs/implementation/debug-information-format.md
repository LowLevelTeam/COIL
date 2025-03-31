# COIL Debug Information Format

## Overview

This document specifies the debug information format for COIL objects. This format enables debuggers, profilers, and other tools to map between COIL binary code and the original CASM source, inspect variables, and analyze program behavior.

## Debug Information Storage

Debug information in COIL objects is stored in a dedicated debug section with its own format:

```
struct DebugSection {
    uint32_t version;       // Debug format version
    uint32_t tables_offset;  // Offset to debug tables
    uint32_t strings_offset; // Offset to string table
}
```

The debug information is referenced from the COIL object header:

```
struct CoilHeader {
    // ... standard header fields ...
    uint32_t debug_offset;   // Offset to debug section (0 if none)
}
```

## Debug Tables

Debug information is organized into tables, each handling a specific aspect:

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
    uint32_t count;      // Number of source files
    SourceEntry entries[]; // Array of source entries
}

struct SourceEntry {
    uint32_t file_id;    // File identifier
    uint32_t name_offset; // Offset into string table for filename
    uint32_t content_hash; // Hash of file contents (for version checking)
}
```

### Line Table

Maps COIL binary offsets to source lines:

```
struct LineTable {
    uint32_t count;     // Number of line entries
    LineEntry entries[]; // Array of line entries
}

struct LineEntry {
    uint32_t offset;    // Offset into COIL binary
    uint32_t file_id;   // Source file ID
    uint32_t line;      // Source line number
    uint32_t column;    // Source column number (optional)
    uint8_t  flags;     // Line entry flags
}
```

Line entry flags:
- `0x01`: Statement start
- `0x02`: Block start
- `0x04`: Block end
- `0x08`: Function start
- `0x10`: Function end

### Type Table

Provides detailed type information:

```
struct TypeTable {
    uint32_t count;      // Number of type entries
    TypeEntry entries[]; // Array of type entries
}

struct TypeEntry {
    uint16_t type_id;     // COIL type ID
    uint16_t flags;       // Type flags
    uint32_t name_offset; // Offset into string table for type name
    uint32_t size;        // Size in bytes
    uint32_t alignment;   // Alignment requirement
    uint32_t parent_type; // Parent type for derived types
    uint32_t elements;    // Element count for arrays
    uint32_t fields;      // Field count for structs/unions
    uint32_t field_list;  // Offset to field list (for struct/union)
}
```

Type flags:
- `0x0001`: Primitive type
- `0x0002`: Array type
- `0x0004`: Struct type
- `0x0008`: Union type
- `0x0010`: Pointer type
- `0x0020`: Function type
- `0x0040`: Vector type
- `0x0080`: Enum type

Struct/union field entry:
```
struct FieldEntry {
    uint32_t name_offset; // Offset into string table for field name
    uint32_t type_index;  // Index into type table
    uint32_t offset;      // Byte offset within struct/union
    uint16_t size;        // Size in bytes
    uint16_t flags;       // Field flags
}
```

Field flags:
- `0x0001`: Bit field
- `0x0002`: Static field
- `0x0004`: Constant field

### Variable Table

Tracks all variables in the program:

```
struct VariableTable {
    uint32_t count;         // Number of variable entries
    VariableEntry entries[]; // Array of variable entries
}

struct VariableEntry {
    uint32_t name_offset;   // Offset into string table for variable name
    uint32_t type_index;    // Index into type table
    uint32_t scope_index;   // Index into scope table
    uint32_t location_type; // How the variable is stored
    uint32_t location_data; // Location-specific data
    uint32_t start_offset;  // Binary offset where variable becomes valid
    uint32_t end_offset;    // Binary offset where variable becomes invalid
    uint16_t var_id;        // Variable ID in CASM
    uint16_t flags;         // Variable flags
}
```

Location types:
- `0x01`: Register
- `0x02`: Stack (BP-relative)
- `0x03`: Stack (SP-relative)
- `0x04`: Static memory
- `0x05`: Computed (expression-based)

Variable flags:
- `0x0001`: Parameter
- `0x0002`: Local
- `0x0004`: Global
- `0x0008`: Constant
- `0x0010`: Volatile

### Scope Table

Tracks lexical scopes in the program:

```
struct ScopeTable {
    uint32_t count;       // Number of scope entries
    ScopeEntry entries[]; // Array of scope entries
}

struct ScopeEntry {
    uint32_t parent_index; // Index of parent scope (-1 for global)
    uint32_t start_offset; // Binary offset where scope starts
    uint32_t end_offset;   // Binary offset where scope ends
    uint32_t name_offset;  // Offset into string table for scope name (if any)
    uint16_t level;        // Nesting level (0 for global)
    uint16_t flags;        // Scope flags
}
```

Scope flags:
- `0x0001`: Function scope
- `0x0002`: Block scope
- `0x0004`: Loop scope
- `0x0008`: Exception handler scope

### Function Table

Provides information about functions:

```
struct FunctionTable {
    uint32_t count;          // Number of function entries
    FunctionEntry entries[]; // Array of function entries
}

struct FunctionEntry {
    uint32_t name_offset;    // Offset into string table for function name
    uint32_t type_index;     // Index into type table
    uint32_t scope_index;    // Index into scope table
    uint32_t start_offset;   // Binary offset of function start
    uint32_t end_offset;     // Binary offset of function end
    uint32_t param_count;    // Number of parameters
    uint32_t param_list;     // Offset to parameter list
    uint32_t abi_index;      // ABI used by this function
    uint16_t flags;          // Function flags
    uint16_t optimization;   // Optimization level
}
```

Function flags:
- `0x0001`: Global
- `0x0002`: External
- `0x0004`: Inline
- `0x0008`: Noreturn
- `0x0010`: Pure
- `0x0020`: Recursive
- `0x0040`: Entry point

## String Table

The string table stores all strings referenced by debug information entries:

```
struct StringTable {
    uint32_t size;     // Total size of string table
    char     data[];   // Null-terminated strings
}
```

Strings are referenced by their byte offset from the beginning of the string table.

## Debug Information Encoding

### Compact Encoding

To minimize size impact, the debug information uses these techniques:

1. **String Deduplication**: Identical strings stored only once
2. **Tables by Category**: Information grouped by type
3. **Local References**: Indices instead of pointers
4. **Optional Information**: Some fields can be omitted (marked as 0xFFFFFFFF)

### Compression Options

For large debug information sections, compression can be applied:

```
struct CompressedDebugSection {
    uint32_t compression_type; // 0 = none, 1 = zlib, 2 = lz4
    uint32_t uncompressed_size; // Size when decompressed
    uint8_t  compressed_data[]; // Compressed debug section
}
```

## Debug Information Creation

### Source Mapping

The CASM assembler generates source mapping by tracking:

1. The source file and line for each instruction
2. The beginning and end of lexical scopes
3. Variable declarations and lifetimes
4. Type definitions and usage

### Step-By-Step Process

1. CASM assembler parses source files
2. During code generation, it records debug information
3. After binary generation, it builds debug tables
4. The debug section is added to the COIL object

### Line Number Algorithm

Line number information is created using this algorithm:

1. For each instruction, determine source line
2. Record only changes in file/line information
3. Mark special locations (function starts, block boundaries)
4. Store in ascending order by offset

### Type Information Generation

Type information captures complete type details:

1. All base types are registered in the type table
2. Composite types include field information
3. Type relationships are preserved
4. Named types link to their string representation

## Debug Information Usage

### Source-Level Debugging

Enables debuggers to:

1. Set breakpoints at specific source lines
2. Show current source context during execution
3. Provide single-step execution at source level
4. Display call stacks with source information

### Variable Inspection

Allows inspection of variables by:

1. Determining variable location (register, stack, memory)
2. Interpreting memory according to variable type
3. Showing struct/array members and their values
4. Tracking variables as they move between storage locations

### Call Stack Unwinding

Supports reliable call stack traversal:

1. Identifies function boundaries
2. Determines parameter and return conventions (ABI)
3. Locates saved registers and return addresses
4. Maps instruction pointers back to source locations

## Example: Debug Information for a Sample Program

### CASM Source

```
PROC 0x01
ARCH 0x01, 0x03

SECTION .text, 0x01 | 0x04
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR #1, TYPE_INT32     ; total
    VAR #2, TYPE_INT32     ; i
    MOV #1, 0              ; total = 0
    MOV #2, 1              ; i = 1
    
loop_start:
    CMP #2, 10
    BR_GT loop_end
    
    ADD #1, #1, #2         ; total += i
    INC #2                 ; i++
    BR loop_start
    
loop_end:
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #1
    SCOPEL
```

### Generated Debug Information

Line table entries:
```
{ offset: 0x00, file_id: 1, line: 5, flags: 0x08 }  ; Function start
{ offset: 0x05, file_id: 1, line: 6, flags: 0x01 }  ; SCOPEE
{ offset: 0x0A, file_id: 1, line: 7, flags: 0x01 }  ; VAR #1
{ offset: 0x12, file_id: 1, line: 8, flags: 0x01 }  ; VAR #2
{ offset: 0x1A, file_id: 1, line: 9, flags: 0x01 }  ; MOV #1, 0
{ offset: 0x23, file_id: 1, line: 10, flags: 0x01 } ; MOV #2, 1
{ offset: 0x2C, file_id: 1, line: 12, flags: 0x01 } ; loop_start:
{ offset: 0x2C, file_id: 1, line: 13, flags: 0x01 } ; CMP #2, 10
{ offset: 0x35, file_id: 1, line: 14, flags: 0x01 } ; BR_GT loop_end
{ offset: 0x3E, file_id: 1, line: 16, flags: 0x01 } ; ADD #1, #1, #2
{ offset: 0x49, file_id: 1, line: 17, flags: 0x01 } ; INC #2
{ offset: 0x4F, file_id: 1, line: 18, flags: 0x01 } ; BR loop_start
{ offset: 0x58, file_id: 1, line: 20, flags: 0x01 } ; loop_end:
{ offset: 0x58, file_id: 1, line: 21, flags: 0x01 } ; RET
{ offset: 0x65, file_id: 1, line: 22, flags: 0x01 } ; SCOPEL
{ offset: 0x6A, file_id: 1, line: 22, flags: 0x10 } ; Function end
```

Variable entries:
```
{ name_offset: 0x00, type_index: 3, scope_index: 1, location_type: 0x01, 
  location_data: 0x01, start_offset: 0x12, end_offset: 0x65, var_id: 1, 
  flags: 0x0002 }  ; total

{ name_offset: 0x06, type_index: 3, scope_index: 1, location_type: 0x01, 
  location_data: 0x02, start_offset: 0x1A, end_offset: 0x65, var_id: 2, 
  flags: 0x0002 }  ; i
```

Scope entries:
```
{ parent_index: 0xFFFFFFFF, start_offset: 0x00, end_offset: 0x6A, 
  name_offset: 0x08, level: 0, flags: 0x0001 }  ; Function scope

{ parent_index: 0, start_offset: 0x05, end_offset: 0x65, 
  name_offset: 0xFFFFFFFF, level: 1, flags: 0x0002 }  ; Block scope
```

Function entry:
```
{ name_offset: 0x08, type_index: 0, scope_index: 0, 
  start_offset: 0x00, end_offset: 0x6A, param_count: 0, 
  param_list: 0xFFFFFFFF, abi_index: 0, flags: 0x0001, optimization: 0 }
```

String table:
```
"total\0i\0calculate_sum\0"
```

## Implementation Requirements

For COIL objects with debug information, implementations must:

1. Preserve source-level information during assembly and linking
2. Maintain debug information accuracy through optimization passes
3. Separate debug information to allow stripping when not needed
4. Support incremental updates during development

## Debug Information Stripping

For production builds, debug information can be:

1. **Fully Stripped**: All debug information removed
2. **Partially Stripped**: Keep function boundaries and names only
3. **Separated**: Placed in separate files (.coild) for later use

## Future Extensions

Future versions of the COIL debug format may add:

1. **Macro Expansion Information**: Track macro usage
2. **Template Instantiation**: For higher-level language support
3. **Inlining Information**: Track inlined function calls
4. **Optimization Records**: Document optimization decisions
5. **Source Code Embedding**: Include relevant source excerpts