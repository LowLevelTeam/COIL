# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Key Features

- **Type-Determined Instructions**: Instructions derive behavior from operand types
- **Architecture Independence**: Clean separation between universal and platform-specific code
- **Variable System**: Abstract management of registers and memory
- **ABI System**: Automated function call conventions
- **Performance-First**: No abstraction penalties for core operations

## Documentation Structure

The documentation is organized into distinct sections:

### COIL Core Documentation
- [Introduction](/coil-docs/core/introduction.md) - COIL purpose and design
- [Binary Format](/coil-docs/core/binary-format.md) - Specification of COIL binary encoding
- [Instruction Set](/coil-docs/reference/instruction-reference.md) - Complete instruction reference
- [Type System](/coil-docs/types/type-system.md) - Type definitions and operations

### Systems Documentation
- [Memory and Variables](/coil-docs/systems/memory-and-variables.md) - Memory access and variable system
- [ABI System](/coil-docs/systems/abi-system.md) - Function calling conventions
- [Device Architecture](/coil-docs/systems/device-architecture.md) - Device targeting framework

### CASM Documentation
- [CASM Basics](/casm-docs/basics/casm-basics.md) - Assembly language syntax
- [Directives](/casm-docs/features/directives.md) - Assembly control directives
- [Scopes](/casm-docs/features/scopes.md) - Variable scoping and lifetime

### CBC Documentation
- [CBC Overview](/cbc-docs/cbc-overview.md) - COIL Byte Code introduction
- [CBC Format](/cbc-docs/cbc-format.md) - CBC binary encoding format
- [CBC Instruction Set](/cbc-docs/cbc-instruction-set.md) - CBC instructions reference

### Implementation Documentation
- [Toolchain Components](/coil-docs/implementation/toolchain-components.md) - Core components
- [File Formats](/coil-docs/implementation/file-formats.md) - File format specifications
- [Command Interfaces](/coil-docs/implementation/command-interfaces.md) - Command-line interfaces

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