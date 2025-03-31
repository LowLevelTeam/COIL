# COIL Specification Summary

## What is COIL?

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges high-level programming languages and hardware-specific machine code, enabling efficient cross-platform execution.

## Key Features

- **Type-Determined Instructions**: Instructions derive behavior from operand types, reducing instruction set complexity
- **Architecture Independence**: Clear separation between universal and platform-specific code
- **Variable System**: Abstract management of registers and memory
- **ABI System**: Automated function call conventions
- **Performance-First**: Native performance capabilities with useful abstractions

## Version Roadmap

### Version 1: Core Specification (Current)
- Complete CPU-oriented instruction set
- Comprehensive type system
- Binary formats (COIL object and output)
- ABI mechanisms
- Single-device focus

### Version 2: Standard Library (Planned)
- Standard library for common operations
- System interface API
- Optimization framework
- Full backward compatibility with v1

### Version 3: Multi-Device Support (Future)
- Support for GPUs, TPUs, DSPs, FPGAs, and other accelerators
- Device management and selection
- Cross-device memory management
- Heterogeneous computing model

## Technical Overview

### Binary Format
- 8-bit opcode
- 8-bit operand count
- Type-specific operand encoding
- Symbol table for efficient representation

### Processor Model
- Processor agnostic design
- Support for multiple processor architectures
- Each COIL object specifies target processor

### Memory Model
- Linear address space
- Scope-based management
- Clear memory ordering rules
- Support for atomic operations

## LLM-Friendly Parsing Guidelines

- Type IDs are consistently represented as hexadecimal values (e.g., 0x01)
- Instruction opcodes are consistently 8-bit
- Operand formats follow predictable patterns
- Structure is hierarchical with clear categorization
- Examples demonstrate standard patterns

## Implementation References

- COIL processor: Reads and executes COIL binary instructions
- CASM assembler: Converts CASM text to COIL binary format
- COIL linker: Combines multiple COIL objects into executable form

## Specification Format

This specification document follows semantic versioning:
- Major version: Incompatible changes to instruction set or binary format
- Minor version: Backwards-compatible additions
- Patch version: Clarifications and documentation improvements

The current specification version is 1.0.0, establishing core functionality for single-processor targets.