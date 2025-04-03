# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

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
│                       │        Programming (OPTIONAL in real implementations just compile to COIL)
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

Workflow example:
1. Developer writes code in CASM or a high-level language that targets COIL
2. CASM is assembled into COIL binary format
3. COIL processor compiles to:
   - Native machine code for the host device
   - CBC for portions targeting other processing units
4. At runtime, CBC is JIT-compiled for the specific device selected

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
- [Type-Determined Behavior](/type-determined-behavior.md) - How instructions adapt to types
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

## Getting Started

To start working with COIL:

1. Understand the [core concepts](/coil-docs/index.md)
2. Learn about the [type system](/coil-docs/types/type-system.md)
3. Explore the [universal instruction set](/coil-docs/isa-u/overview.md)

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## Contributing

For information on contributing to the COIL specification, please see [CONTRIBUTING.md](/CONTRIBUTING.md).

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.