# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for direct device interaction and maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code through a type-determined instruction model, without operating system dependencies.

## Key Components

The COIL ecosystem consists of three primary components:

### 1. COIL (Computer Oriented Intermediate Language)
The universal binary instruction format that serves as the intermediate representation for COIL-compatible systems. COIL interacts directly with target devices without operating system dependencies.

### 2. CASM (COIL Assembly)
The standard human-readable text representation of COIL code, serving as both a low-level programming language and assembly language for COIL.

### 3. CBC (COIL Byte Code)
A specialized format designed for debugging, JIT compilation, and heterogeneous computing support. CBC is not the primary compilation target but serves specific purposes within the COIL ecosystem.

## Core Philosophy

COIL is built on several fundamental principles:

### Direct Device Interaction
COIL programs interact directly with hardware, without operating system dependencies:
- No dependency on OS abstractions in core specification
- Direct access to device features based on capabilities
- OS services accessed through standard library, not core instructions

### Type-Determined Instructions
Instructions derive their behavior from operand types, allowing a single opcode to represent multiple operations:

```
ADD result, a, b  // Works for integers, floats, vectors, matrices based on operand types
```

### Architectural Independence
COIL maintains a clean separation between:

- **Universal Operations**: Work identically across all platforms
- **Extended Operations**: Tailored to specific architecture modes
- **Compiler Operations**: Direct communication with the COIL Processor

### Capability-Based Design
COIL uses abstract capabilities that can be checked at compile time:
- COIL capabilities represent abstract features (e.g., atomic operations)
- Native features represent hardware-specific capabilities
- Configuration determines available capabilities

## Documentation Structure

This repository contains comprehensive documentation organized as follows:

### Core Specification
- [Overview](/coil-docs/overview.md) - Core principles and concepts
- [Type System](/coil-docs/core/type-system.md) - Data representation and type behaviors
- [Binary Format](/coil-docs/core/binary-format.md) - Encoding format for instructions
- [Instruction Set](/coil-docs/core/instruction-set.md) - Universal instruction reference
- [Memory Model](/coil-docs/core/memory-model.md) - Memory organization and access patterns
- [Atomic Operations](/coil-docs/core/atomic-operations.md) - Atomic operations specification
- [Configuration Format](/coil-docs/core/config-format.md) - Capability configuration details
- [Device Interaction](/coil-docs/core/device-interaction.md) - Direct hardware interaction model
- [Flags](/coil-docs/core/flags.md) - Condition flags and their behaviors
- [Linking](/coil-docs/core/linking.md) - Object linking and loading specification
- [Object Format](/coil-docs/core/object-format.md) - Binary object file structure
- [Execution Model](/coil-docs/core/execution-model.md) - Program execution semantics

### Processing Units
- [CPU](/coil-docs/pu/cpu.md) - CPU-specific instruction set and features
- [GPU](/coil-docs/pu/gpu.md) - GPU-specific instruction set and features
- [NPU](/coil-docs/pu/npu.md) - NPU-specific instruction set and features

### Extensions
- [Multi-Dimensional Types](/coil-docs/extensions/multidim.md) - Vectors, matrices, and tensors
- [Composite Types](/coil-docs/extensions/composite.md) - Structures, unions, and complex data types
- [ABI](/coil-docs/extensions/abi.md) - Application Binary Interface specifications
- [Preprocessor](/coil-docs/extensions/preprocessor.md) - Conditional compilation and compile-time features
- [Overview](/coil-docs/extensions/overview.md) - Extensions framework introduction

### CASM (COIL Assembly)
- [Overview](/casm-docs/index.md) - Introduction to CASM
- [Syntax](/casm-docs/syntax.md) - CASM syntax guide
- [Directives](/casm-docs/directives.md) - Assembler directives

### CBC (COIL Byte Code)
- [Overview](/cbc-docs/index.md) - Introduction to CBC
- [Format](/cbc-docs/format.md) - CBC binary format
- [Execution](/cbc-docs/execution.md) - CBC execution model

### Standard Library
- [Overview](/std-docs/README.md) - Standard library functions and modules
- [System](/std-docs/system.md) - System and OS interaction
- [Math](/std-docs/math.md) - Mathematical functions and operations

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.

## Contributing

Guidelines for contributing to the COIL specification can be found in [CONTRIBUTING.md](/CONTRIBUTING.md).

## Roadmap

For planned development and future directions, see [ROADMAP.md](/ROADMAP.md).