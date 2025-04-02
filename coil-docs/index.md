# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Core Design Philosophy

COIL is built on these foundational concepts:

### Type-Determined Instructions

Instructions derive their behavior from operand types, allowing a single opcode to handle different data types:

```c
ADD result, a, b  // Works for integers, floats, or vectors based on operand types
```

This approach provides:
- A compact instruction set
- Consistent behavior patterns across types
- Natural extensibility as new types are added

### Architecture Independence

COIL maintains a clean separation between:

- **Universal Operations** (0x00-0x9F): Work identically across all platforms
- **PU Operations** (0xA0-0xBF): Work across identical processing units
- **Architecture-Specific Operations** (0xC0-0xDF): Tailored to specific architectures
- **Mode-Specific Operations** (0xE0-0xEF): Tailored to specific architecture modes
- **Directive Operations** (0xF0-0xFE): Direct communication with the COIL Processor
- **Extension Operations** (0xFF): Vendor-specific features

### Instruction Sets

COIL is organized into three main instruction sets:
- [ISA Universal](./isa-u/index.md): Core instructions for all platforms
- [ISA Extended](./isa-e/index.md): Platform-specific instructions
- [ISA Compiler](./isa-c/index.md): Directives for the COIL processor

## COIL in the Toolchain

COIL serves as an intermediate representation in a complete toolchain:

```
Source Language (.*/.casm) → Frontend Compiler → COIL (.coil) →

.coil → COIL Processor → COIL Compiled Code (.ccc) → COIL Linker → Executable/Library

.coil → COIL Processor → .acoil (COIL Library)

.coil → COIL Processor → .dcoil (COIL Dynamic Library)
```

The COIL ecosystem includes:
- **CASM**: Text representation of COIL for direct programming
- **COIL**: COIL Code stored in an object format using default instruction set
- **CCC**: COIL Compiled Code stored in an object format (contains either CBC for JIT and interpretation or native machine code)

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## Related Components

- [Binary Format](./core/binary-format.md) - COIL binary encoding specification
- [Type System](./types/type-system.md) - Complete type system reference
- [ISA Universal](./isa-u/index.md) - Universal instruction set
- [ISA Extended](./isa-e/index.md) - Extended instruction set
- [ISA Compiler](./isa-c/index.md) - Compiler directives
- [CBC](../cbc-docs/index.md) - COIL Byte Code
- [CASM](../casm-docs/index.md) - COIL Assembly Language