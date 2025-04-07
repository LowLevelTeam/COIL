# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Core Components

The COIL ecosystem consists of three primary components:

### 1. COIL (Computer Oriented Intermediate Language)
The universal binary instruction format that serves as the intermediate representation for COIL-compatible systems.

### 2. CASM (COIL Assembly)
The standard human-readable text representation of COIL code.

### 3. CBC (COIL Byte Code)
A compact, portable binary representation for efficient interpretation and JIT compilation.

## Specification Structure

The COIL documentation is organized into the following sections:

- **Core Specification**: Type system, memory model, execution model, binary format
- **Instruction Sets**: Universal ISA, Extended ISA, Compiler ISA
- **Extensions**: Multi-dimensional types, ABI definitions, preprocessor, processor-specific

## Design Philosophy

COIL is built on these foundational concepts:

### Type-Determined Instructions

Instructions derive their behavior from operand types, allowing a single opcode to handle different data types:

```
ADD result, a, b  // Works for integers, floats, vectors, matrices based on operand types
```

### Architecture Independence

COIL maintains a clean separation between:

- **Universal Operations**: Work identically across all platforms
- **Extended Operations**: Tailored to specific architecture modes
- **Compiler Operations**: Direct communication with the COIL Processor

## Extensions

COIL supports several extensions to enhance functionality:

- **Multi-Dimensional Types**: Vectors, matrices, and tensors with associated operations
- **Preprocessor**: Conditional compilation, definitions, and compile-time constants
- **ABI**: Application Binary Interface definitions for function calling conventions
- **Processor Specific**: Instructions tailored for specific processing units
- **Architecture Specific**: Instructions for specific hardware architectures
- **Composite Types**: Enhanced type operations for structured data

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.
