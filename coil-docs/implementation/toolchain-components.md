# Toolchain Components

## Overview

The COIL Toolchain consists of several core components that work together to transform CASM source code into executable programs. This document defines these components and their relationships.

## Core Components

### CASM Assembler

**Purpose**: Converts CASM source code to COIL object files.

**Inputs**: 
- `.casm` files (COIL Assembly source)
- Header files and include directives

**Outputs**:
- `.coil` object files (COIL binary format)

**Responsibilities**:
- Parse CASM syntax
- Validate instruction semantics
- Resolve local symbols
- Generate binary encoding
- Create relocation information
- Include debug information (when requested)

### COIL Processor

**Purpose**: Processes COIL object files into architecture-specific object files.

**Inputs**:
- `.coil` object files
- Target specifications

**Outputs**:
- `.coilo` output object files (containing native opcodes)

**Responsibilities**:
- Type-based instruction processing
- Target-specific code generation
- Optimization passes
- Variable allocation
- ABI implementation
- Multi-device code generation

### OS-Specific Linker

**Purpose**: Links COILO output objects into final executable formats.

**Inputs**:
- `.coilo` output object files
- Libraries and other linked objects

**Outputs**:
- OS-specific executables (`.exe`, `.out`, etc.)
- Shared libraries (`.dll`, `.so`, etc.)
- Static libraries (`.lib`, `.a`, etc.)

**Responsibilities**:
- Final symbol resolution
- Section placement
- OS-specific format generation
- Entry point setup
- Runtime linkage

### CBC Compiler

**Purpose**: Converts COIL objects to CBC bytecode for interpretation.

**Inputs**:
- `.coil` object files

**Outputs**:
- `.cbc` bytecode files
- CBC sections within `.coilo` files

**Responsibilities**:
- Convert COIL instructions to CBC format
- Build constant pools
- Create function tables
- Generate metadata
- Optimize for interpretation

### CBC Interpreter

**Purpose**: Executes CBC bytecode directly.

**Inputs**:
- `.cbc` bytecode files
- CBC sections from `.coilo` files

**Outputs**:
- Program execution

**Responsibilities**:
- Interpret CBC instructions
- Manage runtime memory
- Track variables
- Implement control flow
- Handle errors
- (Optional) JIT compilation

## Toolchain Flows

### Native Compilation Flow

Standard compilation for maximum performance:

```
[.casm files] → CASM Assembler → [.coil objects] → COIL Processor → [.coilo objects] → OS Linker → [Executable]
```

### Interpreted Flow

For dynamic languages or environments requiring interpretation:

```
[.casm files] → CASM Assembler → [.coil objects] → CBC Compiler → [.cbc bytecode] → CBC Interpreter → [Execution]
```

### Hybrid Flow

Mixed native and interpreted execution:

```
[.casm files] → CASM Assembler → [.coil objects] → COIL Processor → [.coilo with CBC sections] → OS Linker → [Executable]
                                                                                               ↓
                                                                       [CBC sections] → CBC Interpreter
```

## Component Interfaces

### File Format Interfaces

Components communicate through standardized file formats:

1. **CASM to COIL Assembler**: CASM source text (`.casm`)
2. **COIL Assembler to COIL Processor**: COIL object format (`.coil`)
3. **COIL Processor to OS Linker**: COILO output object format (`.coilo`)
4. **COIL Processor to CBC Compiler**: COIL object format (`.coil`)
5. **CBC Compiler to CBC Interpreter**: CBC bytecode format (`.cbc`)

### Command Line Interfaces

Each component provides a standard command-line interface:

```
casm [options] input.casm -o output.coil
coilp [options] input.coil -o output.coilo
```

## Implementation Requirements

A compliant COIL toolchain must:

1. Follow all file format specifications
2. Maintain compatibility between components
3. Support all universal instructions
4. Provide consistent error reporting
5. Enable appropriate optimization levels
6. Preserve debug information when requested

## Related Components

- [File Formats](/coil-docs/implementation/file-formats.md) - Detailed format specifications
- [Command Interfaces](/coil-docs/implementation/command-interfaces.md) - Command-line interface details
- [Binary Format](/coil-docs/core/binary-format.md) - COIL binary encoding format