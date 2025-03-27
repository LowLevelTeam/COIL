# Computer Oriented Intermediate Language (COIL)

## Overview

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. COIL bridges the gap between high-level programming languages and hardware-specific machine code.

## Key Features

- **Type-Determined Instructions**: Instructions derive behavior from operand types
- **Architecture Independence**: Clean separation between universal and platform-specific code
- **Variable System**: Abstract management of registers and memory
- **ABI System**: Automated function call conventions
- **Performance-First**: No abstraction penalties for core operations

## Documentation Structure

The documentation is organized into two main sections:

### COIL Documentation
Documentation about the binary format and core design:

- [Introduction](coil-docs/core/introduction.md) - What is COIL and its purpose
- [Design Principles](coil-docs/core/design-principles.md) - Core design philosophy
- [Type System](coil-docs/types/type-system-overview.md) - How types work in COIL
- [Variable System](coil-docs/systems/variable-system.md) - Variable abstraction
- [ABI System](coil-docs/systems/abi-system.md) - Function calling conventions

### CASM Documentation
Documentation about the assembly language used to write COIL code:

- [Introduction](casm-docs/basics/introduction.md) - What is CASM
- [Syntax Fundamentals](casm-docs/basics/syntax-fundamentals.md) - Basic syntax rules
- [Variables](casm-docs/features/variables.md) - Working with variables
- [Examples](casm-docs/examples/hello-world.md) - Example programs

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## Low Level Team
The Low Level Team is a group of anonymous developers who contribute to creating a new framework under a single specification. The main contributors and core members of COIL exists entirely of LLT Members.

We at the Low Level Team pride ourselves on making clean and easy to use software for developers by developers. Focusing on creating a new language framework from replacing assembly and linkers to switching out C and python. We don't like the status quo so we made our own.

All LLT projects are built in some way on top of COIL. Utilizing the COIL framework as the backend for the development language of choice. We welcome a future built by our own hands, build around these specifications to put your own hands in play.

Your work is appreciated by The Low Level Team.