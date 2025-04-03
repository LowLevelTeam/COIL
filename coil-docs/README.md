# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Key Features

- **Architecture Independence**: Clean separation between universal and platform-specific operations
- **Type-Determined Instructions**: Instructions adapt behavior based on operand types
- **Modular Design**: Separate instruction sets for universal, extended, and compiler operations
- **Comprehensive Type System**: Support for diverse data types from primitives to matrices and tensors
- **Explicit Configuration**: Target-specific optimizations through standardized configuration

## Documentation Structure

The COIL documentation is organized as follows:

## Instruction Set Architecture

COIL is organized into three main instruction sets:

1. **ISA Universal (ISA-U)**: Core instructions that work on all platforms
   - Control flow, memory operations, arithmetic, bit manipulation
   - Type-determined behavior adapts to operand types
   - Maximum portability across all environments

2. **ISA Extended (ISA-E)**: Platform-specific instructions
   - Organized by processing unit, architecture, and mode
   - Enables hardware-specific optimizations
   - Conditional compilation for portability

3. **ISA Compiler (ISA-C)**: Directives for the COIL processor
   - Configuration and compilation control
   - ABI definitions and linking directives
   - Conditional compilation constructs

## Implementation Status

COIL is currently in the specification phase. The documentation is being developed to provide a comprehensive and implementable standard.

## Getting Started

To understand COIL, begin with these core documents:

1. [COIL Introduction](./coil-docs/index.md) - Overview of COIL philosophy
2. [Type System](./coil-docs/type.md) - Understanding COIL's type system
3. [Universal ISA Overview](./coil-docs/isa-u/overview.md) - Core instruction set
4. [Configuration Format](./coil-docs/core/config-format.md) - Target specification

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements