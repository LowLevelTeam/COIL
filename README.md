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

### Multi-Processor Design

COIL is designed from the ground up to support multiple processing unit types:

- Version 1: Focus on CPU processing units
- Version 2: Standard library implementation
- Version 3: Comprehensive multi-device support (GPUs, TPUs, etc.)

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

## Key Features

### Variable System

COIL's variable system abstracts over registers and memory:
- Variables are declared with specific types
- Variables can be scoped for automatic lifetime management
- The COIL processor optimally allocates variables to registers or memory
- Variables can be promoted or demoted as needed for performance

### ABI System

The Application Binary Interface (ABI) system eliminates the need for manual register handling during function calls:
- Parameter passing is automated based on ABI definitions
- Return values are managed according to ABI rules
- Register preservation is handled by the processor
- Multiple ABIs can coexist in the same program

### Binary Format Primacy

COIL is fundamentally a **binary instruction format**. While a text representation (COIL-ASM) is provided for documentation and human interaction, the binary format is the actual "language" of COIL.

## Specification Structure

The COIL specification is organized into the following sections:

- **[Core Concepts](spec/core/)**: Fundamental principles and mechanisms
- **[Instruction Set Architecture](spec/isa/)**: Complete instruction definitions
- **[Text Format](spec/text-format/)**: COIL Assembly language definition
- **[Multi-Processor Support](spec/multi-processor/)**: Processing unit abstraction
- **[File Formats](spec/formats/)**: Object and debug format specifications
- **[Reference Materials](reference/)**: Comprehensive reference documentation
- **[Examples](examples/)**: Example COIL programs

## Version Information

Current specification version: 1.0.0

Specification versions follow semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## License

COIL is released into the public domain under the Unlicense. See the [LICENSE](LICENSE) file for details.

## Low Level Team
The Low Level Team is a group of anonymous developers who contribute to creating a new framework under a single specification. The main contributors and core members of COIL exists entirely of LLT Members.

We at the Low Level Team pride ourselves on making clean and easy to use software for developers by developers. Focusing on creating a new language framework from replacing assembly and linkers to switching out C and python. We don't like the status quo so we made our own.

All LLT projects are built in some way on top of COIL. Utilizing the COIL framework as the backend for the development language of choice. We welcome a future built by our own hands, build around these specifications to put your own hands in play.

Your work is appreciated by The Low Level Team.

## COIL Projects by LLT
COIL Assembler - Parse .casm into .coil
COIL VM - Interpret .coil
COIL Processor - Parse .coil into .coilo
COIL Linker - Link .coilo into (.exe, .out, ...)
COIL Library - COIL standard library
