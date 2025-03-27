# COIL Overview

## Purpose

This document provides a high-level introduction to the Computer Oriented Intermediate Language (COIL), explaining its core design principles, key features, and overall architecture. It serves as an entry point to the COIL documentation.

## What is COIL?

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. COIL bridges the gap between high-level programming languages and hardware-specific machine code through its type-determined instruction philosophy.

## Core Design Principles

### Type-Determined Instruction Philosophy

COIL's most distinctive characteristic is that instructions derive their behavior from the types of their operands, rather than having separate opcodes for each operation variant. This approach provides:

- **Compact instruction set** with fewer opcodes
- **Consistent behavior** across different data types
- **Extensibility** without opcode explosion
- **Simplified instruction decoding**

For example, a single `ADD` instruction works across integers, floating-point values, and vectors, with behavior determined by operand types.

### Binary Format Primacy

COIL is fundamentally a binary instruction format. The text representation (COIL Assembly or CASM) is a human-readable interface to this binary format, but the binary encoding is the authoritative definition.

### Architecture Independence

COIL maintains a clean separation between:

- **Universal operations** (available on all platforms)
- **Processing unit specific operations** (cleanly isolated)
- **Architecture specific operations** (explicitly marked)
- **Implementation specific extensions** (clearly designated)

This separation ensures maximum portability while still allowing direct access to hardware-specific features when needed.

### Performance-First Approach

While providing abstractions, COIL never sacrifices performance:

- No abstraction penalties for universal operations
- Direct mapping to hardware capabilities where possible
- Optimization hints through type extensions
- Explicit control over performance-critical aspects

## Key Features

### Variable System

COIL's variable system abstracts over registers and memory:

- Variables are declared with specific types
- Variables are scoped for automatic lifetime management
- The COIL processor optimally allocates variables to registers or memory
- Variables can be promoted or demoted as needed for performance

```
; Example of variable usage
SCOPEE
    VAR TYPE_INT32, counter, 0
    ADD counter, counter, 1
SCOPEL  ; counter is automatically released here
```

### ABI System

The Application Binary Interface (ABI) system eliminates the need for manual register handling during function calls:

- Parameter passing is automated based on ABI definitions
- Return values are managed according to ABI rules
- Register preservation is handled by the processor
- Multiple ABIs can coexist in the same program

```
; Example of ABI usage
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

### Multi-Processor Design

COIL is designed with a clear progression plan for supporting multiple processing unit types:

- **Version 1**: Focus on CPU processing units
- **Version 2**: Standard library implementation
- **Version 3**: Comprehensive multi-device support (GPUs, TPUs, etc.)

## Version Evolution Strategy

COIL evolves through distinct versions with clear boundaries:

### Version 1 (Current)
- Core CPU-focused instruction set
- Comprehensive type system
- Variable system and scoping
- ABI mechanism
- Binary formats

### Version 2 (Planned)
- Standard library
- Extended system interfaces
- Enhanced optimization support

### Version 3 (Future)
- Multi-device operations
- Cross-device memory management
- Heterogeneous processing

## Specification Structure

The COIL specification is organized into the following sections:

- **Core Concepts**: Fundamental principles and mechanisms
- **Assembly**: CASM language definition and usage
- **Instruction Set**: Complete instruction definitions
- **Systems**: Key subsystems like types, variables, and ABIs
- **Implementation**: Requirements and formats for implementation
- **Reference**: Comprehensive reference materials
- **Examples**: Sample COIL programs

## Getting Started

To get started with COIL:

1. Familiarize yourself with the core concepts in this overview
2. Learn the CASM syntax for writing COIL programs
3. Understand the type system, which is fundamental to COIL's operation
4. Explore examples to see COIL in action
5. Refer to the implementation guides when creating COIL tools

## Related Documentation

- [Design Philosophy](design-philosophy.md) - Detailed exploration of COIL's design principles
- [CASM Syntax](../assembly/syntax.md) - Learn how to write COIL assembly
- [Type System](../systems/type-system.md) - Understand COIL's type system
- [Binary Format](../binary-format.md) - Details of the binary encoding
- [Examples](../../examples/basic.md) - Basic examples of COIL programs