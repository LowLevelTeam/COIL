# COIL Debug Format

## Purpose

This document defines the debug information format for COIL programs, enabling tools to map between COIL binary instructions and the original source code, provide variable debugging, and support runtime debugging.

## Key Concepts

- **Debug Information Container**: How debug data is stored in COIL objects
- **Line Number Mapping**: How binary instructions map to source code lines
- **Variable Information**: How variables can be inspected during debugging
- **Type Information**: How types are represented for debugging
- **Scope Information**: How variable scopes are tracked during debugging

## Debug Information Container

Debug information is stored in a dedicated section within the COIL object file:

```
SECTION .debug, 0x04  ; Readable but not executable or writable
```

## Debug Information Structure

The debug information section contains:

1. **Header**: Overall metadata about the debug information
2. **Line Number Table**: Maps between instructions and source lines
3. **Source File Table**: Information about original source files
4. **Variable Table**: Debug information for variables
5. **Type Table**: Expanded type information
6. **Scope Table**: Information about lexical scopes
7. **String Table**: Storage for all debug strings

### Debug Header

```c
struct DebugHeader {
    uint32_t format_version;    // Debug format version
    uint32_t line_table_offset; // Offset to line number table
    uint32_t file_table_offset; // Offset to source file table
    uint32_t var_table_offset;  // Offset to variable table
    uint32_t type_table_offset; // Offset to type table
    uint32_t scope_table_offset; // Offset to scope table
    uint32_t string_table_offset; // Offset to debug string table
    uint32_t reserved[2];       // Reserved for future use
}
```

### Line Number Table

Maps between instruction addresses and source locations:

```c
struct LineEntry {
    uint32_t address;     // Instruction address
    uint32_t file_index;  // Index into source file table
    uint32_t line;        // Line number in source file
    uint32_t column;      // Column number (if available)
    uint8_t  flags;       // Additional flags (statement begin, etc.)
    uint8_t  reserved[3]; // Reserved for future use
}
```

### Source File Table

Contains information about the original source files:

```c
struct FileEntry {
    uint32_t name_offset;   // Offset into string table for filename
    uint32_t content_hash;  // Hash of file content for verification
    uint32_t line_count;    // Number of lines in file
    uint32_t timestamp;     // Last modification timestamp
    uint8_t  language;      // Source language identifier
    uint8_t  reserved[3];   // Reserved for future use
}
```

### Variable Table

Contains debugging information for variables:

```c
struct VarEntry {
    uint32_t name_offset;   // Offset into string table for variable name
    uint32_t type_index;    // Index into type table
    uint32_t scope_index;   // Index into scope table
    uint32_t location_type; // How variable is stored (register, memory, etc.)
    uint32_t location_data; // Register number, memory offset, etc.
    uint8_t  flags;         // Variable flags (parameter, local, etc.)
    uint8_t  reserved[3];   // Reserved for future use
}
```

Variable location types:
```
0x01 - REGISTER     // Variable is in a register
0x02 - MEMORY       // Variable is in memory
0x03 - CONSTANT     // Variable is a constant
0x04 - COMPUTED     // Variable location is computed
0x05 - UNAVAILABLE  // Variable is unavailable
```

### Type Table

Extended type information for debugging:

```c
struct TypeEntry {
    uint16_t type_id;       // COIL type ID
    uint16_t flags;         // Type flags
    uint32_t name_offset;   // Offset into string table for type name
    uint32_t size;          // Size in bytes
    uint32_t align;         // Alignment requirement
    uint32_t member_count;  // Number of members (for composite types)
    uint32_t members_offset; // Offset to member list (if composite)
}
```

### Scope Table

Information about lexical scopes:

```c
struct ScopeEntry {
    uint32_t parent_index;  // Index of parent scope (-1 for global)
    uint32_t start_address; // Start address of scope
    uint32_t end_address;   // End address of scope
    uint32_t var_count;     // Number of variables in this scope
    uint32_t first_var_index; // Index of first variable in variable table
}
```

## String Table

All string data (names, paths, etc.) is stored in a string table. Strings are null-terminated and referenced by offset.

## Line Number Encoding

Line numbers are encoded efficiently:

1. **Full Line Entry**: Contains complete information
2. **Delta Line Entry**: Contains only changes from the previous entry

The encoding uses a state machine:

```c
struct LineStateMachine {
    uint32_t address;
    uint32_t file;
    uint32_t line;
    uint32_t column;
    uint8_t  flags;
}
```

## Source Level Debugging Support

The debug format supports:

1. **Breakpoints**: Setting breakpoints on source lines
2. **Single-Stepping**: Step by instruction or source line
3. **Variable Inspection**: Examining variable values
4. **Call Stack Analysis**: Walking up the call stack
5. **Expression Evaluation**: Evaluating expressions in the debug context

## Source Code Embedding

For enhanced debugging, source code can optionally be embedded in the debug information:

```c
struct EmbeddedSource {
    uint32_t file_index;    // Index into source file table
    uint32_t content_offset; // Offset to source content in string table
    uint32_t content_length; // Length of source content
}
```

## Debug Format Versioning

The debug format follows its own versioning scheme:

1. **Major Version**: Incompatible changes to the debug format
2. **Minor Version**: Backwards-compatible additions

The current version is 1.0.

## Location List Encoding

For variables that change location during their lifetime, location lists are used:

```c
struct LocationList {
    uint32_t entry_count;   // Number of location entries
    LocationEntry entries[]; // Array of location entries
}

struct LocationEntry {
    uint32_t start_address; // Start address of range
    uint32_t end_address;   // End address of range
    uint32_t location_type; // How variable is stored
    uint32_t location_data; // Location data
}
```

## Debug Info Size Optimization

To reduce debug information size:

1. **String Deduplication**: Reuse identical strings
2. **Delta Encoding**: Store changes rather than full records
3. **Compression**: Optional compression of debug sections
4. **Separation**: Store debug information in separate files

## Integration with COIL Toolchain

### Debug Information Generation

COIL assemblers and compilers can generate debug information:

```
coilasm -g source.casm -o output.coil
```

### Debug Information Extraction

Debug information can be extracted for analysis:

```
coilobj extract -s .debug input.coil -o debug_info
```

### Debugging Commands

The COIL debugger uses debug information for commands:

```
(coildbg) break source.casm:15    # Set breakpoint at line 15
(coildbg) print variable          # Print variable value
(coildbg) step                    # Single-step by source line
(coildbg) where                   # Show call stack
```

## Implementation Recommendations

COIL processors with debugging support should:

1. **Parse Debug Information**: Load and process debug information
2. **Map Addresses**: Translate between binary addresses and source locations
3. **Variable Context**: Provide access to variable values in appropriate scopes
4. **Command Interface**: Support standard debugger commands
5. **State Inspection**: Allow inspection of processor state

### Variable Location Resolution

To locate a variable during debugging:

1. Determine the current instruction address
2. Find the active scope for that address
3. Look up the variable in the scope's variable table
4. Determine the variable's location (register, memory, etc.)
5. Access the value at that location

## Debug Format Examples

### Simple Line Table Example

```
Address  File  Line  Column  Flags
0x1000    1     10     0       1    ; Start of function
0x1008    1     11     0       0
0x1010    1     12     0       0
0x1018    1     13     0       0
0x1020    1     14     0       0    ; End of function
```

### Variable Location Example

```
Name   Type  Scope  Location      Flags
"i"     3     2     REGISTER(0)    0    ; 'i' in register 0
"j"     3     2     MEMORY(16)     0    ; 'j' at stack offset 16
"k"     3     2     COMPUTED(...)  0    ; 'k' has computed location
```

## Security Considerations

Debug information may contain sensitive data and should be:

1. **Stripped for Production**: Remove from production builds when not needed
2. **Protected**: Give appropriate access controls
3. **Validated**: Check for malicious content in untrusted files

## Related Documentation

- [Object Format](object-format.md) - The container format for debug information
- [Error Handling](error-handling.md) - How errors relate to debugging
- [Toolchain](toolchain.md) - Debugger tool information
- [Variable System](../spec/systems/variable-system.md) - How variables are represented