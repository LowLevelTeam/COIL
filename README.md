# Computer Oriented Intermediate Language (COIL)

## Overview

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. COIL bridges the gap between high-level programming languages and hardware-specific machine code through its type-determined instruction philosophy.

## Core Principles

### Type-Determined Instruction Philosophy

Instructions in COIL derive their behavior from the types of their operands, rather than having numerous specialized opcodes. This allows for:

- Compact binary representation
- Consistent behavior across data types
- Extensibility without opcode explosion
- Simplified instruction decoding

### Architecture Independence

COIL maintains a clean separation between:

- Universal operations (available on all platforms)
- Processing unit specific operations
- Architecture specific operations
- Implementation specific extensions

This separation ensures maximum portability while still allowing direct access to hardware-specific features when needed.

### Performance-First Approach

COIL is designed to compile efficiently to native code without sacrificing performance:

- No abstraction penalties for universal operations
- Direct mapping to hardware capabilities where possible
- Optimization hints through type extensions
- Explicit control over performance-critical aspects

## Version 1 Specification

COIL v1 focuses on:
- Complete CPU-oriented instruction set
- Comprehensive type system
- Foundational binary formats
- Application Binary Interface (ABI) mechanisms

The v1 specification is structured to allow future extensions to support other processing units (GPUs, TPUs, etc.) while maintaining backward compatibility.

## Directory Structure

- `/spec/v1/` - Version 1 specification
  - `core.md` - Core concepts and instruction format
  - `types.md` - Type system specification
  - `isa/` - Instruction Set Architecture
  - `abi.md` - Application Binary Interface
  - `obj.md` - Object format
  - `reg.md` - Register system
  - `config.md` - Configuration format
  - `debug.md` - Debug information format
- `/examples/v1/` - Example COIL programs for v1

## Future Versions

- **Version 2:** Standard library implementation
- **Version 3:** Multi-device support (GPUs, TPUs, etc.)

## The Team Behind COIL

The Low Level Team is a group of developers who contribute to creating a new framework under the COIL specification. We focus on creating clean and efficient software that enables development across all levels of the stack, from assembly replacement to high-level applications.

## License

COIL is released into the public domain under the Unlicense. See the LICENSE file for details.