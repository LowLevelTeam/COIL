# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Documentation Version 1.1.0

This documentation represents version 1.1.0 of the COIL specification, with comprehensive updates addressing implementation challenges encountered in libcoil-dev. Key improvements include:

1. **Binary Format Clarifications**
   - Precise encoding rules for all instruction patterns
   - Detailed examples for edge cases (zero-operand instructions, etc.)
   - Explicit endianness handling rules
   - Complete operand encoding specification

2. **Type System Enhancements**
   - Comprehensive type data encoding examples
   - Explicit matrix and tensor type definitions
   - Nesting limits for complex types
   - Detailed binary encoding examples

3. **Instruction Behavior**
   - Complete type combination matrices for operations
   - Standardized flag effect documentation for all instructions
   - Explicit error handling specifications
   - Linear algebra semantics formalization

4. **Object Format and Linking**
   - Detailed section merging algorithms
   - Precise symbol resolution rules
   - Comprehensive relocation type specifications
   - Platform adaptation guidelines

5. **Memory Model Implementation**
   - Explicit memory protection mechanisms
   - Stack frame organization and management
   - Detailed alignment requirements
   - Error handling for memory operations

6. **ABI Improvements**
   - Variadic function support
   - Parameter passing specifications
   - Return value handling rules
   - Register usage conventions

7. **Configuration Schema**
   - Formal validation schema
   - Required vs. optional settings
   - Platform-specific extension mechanisms
   - Configuration inheritance

## Core Components

The COIL ecosystem consists of three primary components:

### 1. COIL (Computer Oriented Intermediate Language)

The universal binary instruction format that serves as the machine code for COIL-compatible systems. This is the core binary representation that:

- Provides a type-determined instruction set
- Separates universal operations from architecture-specific ones
- Enables portable code across diverse processing units

### 2. CASM (COIL Assembly)

The standard human-readable text representation of COIL code. CASM is:

- A direct, one-to-one mapping to COIL binary instructions
- Used for direct programming, debugging, and learning
- Assembled into COIL binary format by CASM assemblers

### 3. CBC (COIL Byte Code)

A compact, portable binary representation derived from COIL, designed for efficient interpretation and just-in-time compilation. CBC:

- Enables runtime device selection for heterogeneous computing
- Is optimized for interpretation and JIT compilation
- Contains specialized formats for different processing units

## Component Relationships

```
┌───────────────────────┐
│   Source Languages    │
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│        CASM           │◄────── Human Assembly
│                       │        Programming
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│        COIL           │◄────── Universal Binary Format
│                       │        (Machine Code)
└───────────┬───────────┘
            │
         ┌──┴──┐
         │     │
         ▼     ▼
┌─────────────────┐ ┌─────────────────┐
│      CBC        │ │   Native Code    │
│ (for JIT/interp)│ │ (compiled COIL)  │
└─────────────────┘ └─────────────────┘
```

## Key Features

- **Type-Determined Instructions**: Instructions derive behavior from operand types
- **Architecture Independence**: Clean separation between universal and platform-specific code
- **Variable System**: Abstract management of registers and memory
- **ABI System**: Automated function call conventions
- **Performance-First**: No abstraction penalties for core operations
- **Heterogeneous Computing**: Support for multiple processing unit types

## Documentation Structure

The COIL documentation is organized as follows:

### Core Concepts
- [COIL Introduction](/coil-docs/index.md) - Overview of COIL philosophy
- [Type System](/coil-docs/types/type-system.md) - Understanding COIL's type system
- [Type-Determined Behavior](/coil-docs/types/type-determined-behavior.md) - How instructions adapt to types
- [Memory Model](/coil-docs/core/memory-model.md) - Memory management and organization

### Instruction Sets
- [Universal ISA Overview](/coil-docs/isa-u/overview.md) - Core instruction set
- [Extended ISA Overview](/coil-docs/isa-e/overview.md) - Platform-specific instructions
- [Compiler ISA](/coil-docs/isa-c/index.md) - Compiler directives

### Binary Formats
- [Binary Format](/coil-docs/format/binary-format.md) - COIL instruction encoding
- [Object Format](/coil-docs/format/object-format.md) - COIL object file structure

### Configuration & Linking
- [Configuration Format](/coil-docs/core/config-format.md) - Target specification
- [Linking Model](/coil-docs/core/linking-model.md) - Module combination process

## Implementation Guidance

For implementers of COIL processors and tools:

1. **Instruction Encoding**: Follow the precise encoding rules in the binary format specification, paying particular attention to operand count handling and type extension encoding.

2. **Memory Management**: Implement the memory protection model as specified, with clear error handling for violations.

3. **Type System**: Support all type categories with their precise binary representations and respect the nesting limits.

4. **Linking Process**: Follow the detailed section merging and symbol resolution algorithms to ensure consistent behavior across implementations.

5. **Configuration**: Validate configuration files against the formal schema and respect the platform-specific extension mechanism.

6. **ABI Management**: Implement the variadic function support and parameter passing rules as specified to ensure interoperability.

7. **Flag Effects**: Follow the comprehensive flag effect documentation to ensure consistent behavior across instructions.

## Version Information

Current specification version: 1.1.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions and clarifications
- **Patch**: Documentation improvements and minor clarifications

## Contributing

For information on contributing to the COIL specification, please see [CONTRIBUTING.md](/CONTRIBUTING.md).

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.