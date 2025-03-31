# Introduction to COIL

## What is COIL?

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. 

COIL serves as a bridge between high-level programming languages and hardware-specific machine code. It provides a consistent binary representation that can be efficiently translated to various target architectures.

## Core Concepts

### Binary Format Primacy

COIL is fundamentally a **binary instruction format**. While a text representation (COIL Assembly or CASM) is provided for human interaction, the binary format is the actual "language" of COIL.

### Type-Determined Instructions

In COIL, instructions derive their behavior from the types of their operands. This means:

- The same `ADD` instruction works on integers, floating-point values, or vectors
- No need for separate opcode variants (like ADDI, ADDF, ADDV)
- Behavior is determined by examining operand types at runtime

### Universal vs. Platform-Specific

COIL clearly separates:

- **Universal operations** (opcodes 0x00-0xBF): Available on all platforms
- **Platform-specific operations** (opcodes 0xC0-0xFE): For specific architectures
- **Extensions** (opcode 0xFF): For implementation-specific features

## Processor Agnostic Design

COIL is designed to be fundamentally processor-agnostic:

- Does not assume a CPU as the core/host device
- Can target any processor type (CPU, GPU, QPU, FPGA, etc.) as the primary device
- Each COIL object specifies its target processor via the PROC directive
- Multiple processor types can be supported within a single program through appropriate sections

Any processor type can be the "core" device in a COIL-based system, not just CPUs. For example, a quantum processing unit (QPU) or FPGA could be the primary device with CPUs serving auxiliary functions.

## COIL Processing Model

A COIL processor:

1. **Reads** COIL binary instructions
2. **Validates** instruction and operand types
3. **Dispatches** to appropriate implementation based on types
4. **Executes** the operation, using native capabilities where available
5. **Manages** resources according to the variable and ABI systems

## Versions of COIL

The COIL design evolves through distinct versions:

- **Version 1** (Current): Focus on instruction set, types, variables, and ABI
- **Version 2** (Planned): Standard library, system interfaces, optimization
- **Version 3** (Future): Multi-device operations, cross-device memory management

## Future Format Directions

While the current COIL format is designed for general-purpose portability, future specialized formats may include:

- **CBC (COIL Byte Code)**: A more compact, streamlined format optimized for interpreters and JIT compilers with faster parsing and smaller size at the expense of some flexibility
- **Device-Specific Optimized Forms**: Pre-optimized formats for specific device classes

## Relationship to CASM

COIL Assembly (CASM) is the text representation of COIL binary format. It provides:

- Human-readable syntax for writing COIL programs
- Direct correlation to the underlying binary format
- Additional assembly-time directives and conveniences

CASM is to COIL what assembly language is to machine code - a more human-friendly representation of the same underlying concepts.