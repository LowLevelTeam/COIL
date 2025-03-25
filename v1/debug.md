# COIL Debug Format

## 1. Introduction

The COIL Debug Format provides a standardized way to include debugging information within COIL objects. This information enables debuggers, development tools, and COIL processors to map between COIL code and source code, inspect variables, and provide a rich debugging experience.

## 2. Debug Information Structure

Debug information in COIL is organized into several components:

### 2.1 Source File Mapping

Maps between COIL instructions and original source code:
- Source file names and paths
- Line and column number information
- Source code inclusion relationships

### 2.2 Symbol Information

Extended information about symbols:
- Variable names and types
- Function parameters and return types
- Symbol locations (register, stack, memory)
- Symbol scope and lifetime

### 2.3 Type Information

Detailed type information for debugging:
- Complete structure and union layouts
- Array dimensions and element types
- Typedef relationships
- Type compatibility information

### 2.4 Call Stack Information

Data to support call stack unwinding:
- Frame pointer relationships
- Register save locations
- Exception handling information

### 2.5 Scope Information

Defines lexical scopes within the code:
- Block boundaries
- Variable visibility
- Scope relationships (nested scopes)

## 3. Binary Format

Debug information is stored within the COIL object format:

```
+-----------------+
| Debug Section   |
+-----------------+
| Source Mapping  |
+-----------------+
| Symbol Table    |
+-----------------+
| Type Table      |
+-----------------+
| Stack Info      |
+-----------------+
| Scope Table     |
+-----------------+
```

Each table has an appropriate index structure for efficient lookup.

## 4. Source Mapping Format

Source mapping provides a bidirectional mapping between COIL instructions and source code:

```
struct SourceLocation {
  uint32_t file_index;    // Index into file table
  uint32_t line;          // Source line number
  uint32_t column;        // Source column number
  uint32_t coil_offset;   // Offset into COIL code section
  uint32_t length;        // Length of this mapping
};
```

## 5. Debug Information Generation

Debug information may be generated from:
1. Original source code compilation
2. COIL assembly source
3. Annotations within COIL binaries

Compilers and COIL processors may automatically generate this information when requested.

## 6. Embedded Debugging Support

COIL includes direct support for debugging operations:

### 6.1 Breakpoint Mechanism

COIL processors may implement breakpoints through:
- Software breakpoint instructions
- Hardware breakpoint registers
- Debugger-controlled execution

### 6.2 Variable Inspection

Debug information allows for:
- Variable location tracking
- Value extraction from registers or memory
- Type-appropriate value display

### 6.3 Call Stack Traversal

Debug information supports:
- Frame pointer traversal
- Register state recovery
- Function parameter recovery

## 7. Debug Formats for Different Targets

### 7.1 Native Debugging

When targeting native execution, the COIL debug format may be translated to platform-specific formats:
- DWARF for Unix-like systems
- PDB for Windows
- Custom formats for embedded systems

### 7.2 COIL Virtual Machine Debugging

For COIL virtual machine targets, the debug format may be used directly to provide debugging capabilities within the VM environment.

## 8. Implementation Requirements

A COIL v1 processor must:

1. Be able to parse and utilize the debug format when present
2. Optionally generate debug information when requested
3. Pass through debug information to output formats when appropriate
4. Support at least basic source mapping information

## 9. Best Practices

1. Include debug information during development builds
2. Strip or omit debug information for production releases unless needed
3. Use consistent source file paths to ensure reproducible debugging
4. Provide both line and column information for precise debugging
5. Include complete type information for complex data structures