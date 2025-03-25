# COIL Object Format

## 1. Introduction

The COIL object format defines how COIL code is stored, linked, and executed. There are two primary object formats in the COIL ecosystem:

1. **COIL Object (.coil)** - The primary input format containing COIL instructions, symbols, and metadata
2. **COIL Output Object (.coilo)** - The processed output format containing architecture-specific binary code and linking information

These formats are designed to support the full capabilities of COIL while maintaining compatibility across different implementations and architectures.

## 2. COIL Object (.coil)

The COIL object is the standard container for COIL code. It's designed to be portable across different implementations and architectures.

### 2.1 File Structure

A COIL object file consists of the following sections:

```
+------------------+
| File Header      |
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
| Relocation Info  |
+------------------+
| Debug Info       |
+------------------+
| Metadata         |
+------------------+
```

### 2.2 File Header

The file header includes:
- Magic number: "COIL"
- Format version: Major.Minor.Patch (e.g., 1.0.0)
- Target architecture information (or "universal")
- Section count and offsets
- Entry point symbol (if executable)
- Flags and attributes

### 2.3 Symbol Table

The symbol table contains all symbols defined or referenced in the code:
- Symbol ID
- Symbol name (reference to string table)
- Symbol type and attributes
- Symbol visibility (global, file-local, etc.)
- Symbol value or offset

### 2.4 String Table

The string table contains all string literals used in the code and symbol names:
- Null-terminated strings
- Index-based lookups

### 2.5 Type Table

The type table contains definitions of complex types:
- Structure layouts
- Array dimensions
- User-defined type information

### 2.6 Code Sections

Code sections contain COIL instructions:
- Section name and attributes
- Alignment requirements
- Raw COIL instruction bytes

### 2.7 Data Sections

Data sections contain initialized data:
- Section name and attributes
- Alignment requirements
- Raw data bytes

### 2.8 Relocation Information

Relocation entries describe how to adjust symbol references:
- Symbol references that need adjustment during linking
- Relocation type and offset
- Reference to the symbol being relocated

### 2.9 Debug Information

Optional debug information to support debugging tools:
- Source file mappings
- Line number information
- Type information for variables
- Scope information

### 2.10 Metadata

Additional information about the object:
- Compiler information
- Build settings
- Custom attributes

## 3. COIL Output Object (.coilo)

The COIL Output Object is the result of processing a COIL object through a COIL processor. It contains architecture-specific binary code ready for execution or linking.

### 3.1 File Structure

A COILO file extends the COIL format with additional sections:

```
+------------------+
| COILO Header     |
+------------------+
| Original COIL    | (Optional)
+------------------+
| Symbol Table     |
+------------------+
| String Table     |
+------------------+
| Native Code      |
+------------------+
| Native Data      |
+------------------+
| Relocation Info  |
+------------------+
| Debug Info       |
+------------------+
| Linking Info     |
+------------------+
| Optimization     |
| Metadata         |
+------------------+
```

### 3.2 COILO Header

The COILO header extends the COIL header with:
- Target architecture details (specific CPU model, features)
- Optimization level
- Processor identification

### 3.3 Original COIL

Optionally, the original COIL code may be preserved to allow:
- Re-optimization for different targets
- Decompilation
- Verification
- JIT compilation for sections that require it

### 3.4 Native Code and Data

Sections containing architecture-specific machine code:
- Multiple sections for different functions
- Code alignment and padding
- Architecture-specific optimizations
- Platform-specific data layouts

### 3.5 Linking Information

Extended information for linking:
- External symbol dependencies
- Version requirements
- Import/export specifications
- Weak symbol handling

### 3.6 Optimization Metadata

Information about the optimizations applied:
- Inlining decisions
- Loop transformations
- Vector optimizations
- Register allocation strategy

## 4. Version 3 Extensions

In COIL v3, the object formats will be extended to support multi-device computing:

### 4.1 JIT Sections

Sections containing COIL code for JIT compilation:
- Device-specific code segments
- Fallback implementations
- Runtime specialization hints

### 4.2 Device-Specific Code

Multiple architecture-specific sections for different devices:
- CPU-specific code
- GPU-specific code
- TPU-specific code
- Other accelerator-specific code

## 5. Implementation Requirements

A COIL v1 processor must:

1. Be able to read and write both .coil and .coilo formats
2. Preserve all necessary information during translation
3. Support linking multiple objects together
4. Handle relocations correctly
5. Maintain debug information if requested

## 6. Best Practices

1. Keep objects modular for better linking and optimization
2. Include debug information during development
3. Use consistent section naming
4. Leverage metadata for build system integration
5. Consider future compatibility when extending the formats