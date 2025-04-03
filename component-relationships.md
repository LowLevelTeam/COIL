# COIL Ecosystem Component Relationships

## Overview

This document clarifies the relationships between the core components of the COIL ecosystem: COIL (the binary format), CASM (the assembly language), and CBC (the bytecode format). Understanding these relationships is essential for implementers and users of COIL.

## Component Definitions

### COIL (Computer Oriented Intermediate Language)

COIL is the universal binary instruction format that serves as the machine code. It is designed for maximum portability while retaining native performance across diverse processing architectures.

Key characteristics:
- Universal binary format with consistent encoding
- Type-determined instruction behavior
- Three-tiered instruction set architecture (Universal, Extended, Compiler)
- Platform-independent code with platform-specific optimizations

### CASM (COIL Assembly)

CASM is the standard human-readable text representation of COIL. It provides a syntax for writing COIL programs directly.

Key characteristics:
- Human-readable assembly syntax for COIL
- One-to-one mapping with COIL binary instructions
- Used for direct programming, debugging, and learning
- Assembled into COIL binary format

### CBC (COIL Byte Code)

CBC is a compact, portable binary representation derived from COIL, designed for efficient interpretation and just-in-time compilation.

Key characteristics:
- Optimized for interpretation and JIT compilation
- Enables runtime device selection
- More compact than full COIL binary format
- Has specialized formats for different processing units

## Relationship Diagram

```
┌───────────────────────┐
│   Source Languages    │
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│        CASM           │◄────── Human Assembly
│                       │        Programming
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│        COIL           │◄────── Universal Binary Format
│                       │        (Machine Code)
└───────────┬───────────┘
            │
         ┌──┴──┐
         │     │
         ▼     ▼
┌─────────────────┐ ┌─────────────────┐
│      CBC        │ │   Native Code    │
│ (for JIT/interp)│ │ (compiled COIL)  │
└─────────────────┘ └─────────────────┘
```

## Transformation Process

1. **Source to CASM**:
   - Compilers translate high-level languages to CASM
   - Developers can write CASM directly

2. **CASM to COIL**:
   - CASM assemblers convert CASM text to COIL binary format
   - One-to-one mapping between CASM instructions and COIL binary

3. **COIL to Native Code**:
   - COIL processor compiles COIL to native machine code for the host device
   - Maintains all semantics while optimizing for the target architecture

4. **COIL to CBC**:
   - COIL processor can generate CBC for portions designated for non-host execution

## Usage Scenarios

### Compilation Workflow

```
Source Code → CASM → COIL → Native Object Files → Executable
```

### JIT Compilation Workflow

```
Source Code → CASM → COIL → CBC → JIT Compilation → Execution
```

### Direct Programming Workflow

```
Hand-written CASM → COIL → Native Code → Execution
```

### Heterogeneous Computing Workflow

```
Source Code → CASM → COIL → (Native Code for CPU + CBC for GPU) → Execution
```

## Implementation Requirements

For a complete COIL implementation:

1. **CASM Assembler**: Converts CASM to COIL binary format
2. **COIL Processor**: Converts COIL to native code
3. **CBC Generator**: Creates CBC for JIT/interpretation
4. **Runtime Support**: Libraries for CBC execution and heterogeneous computing

## Key Distinctions

- **COIL vs. CASM**: COIL is the binary format; CASM is its human-readable text representation
- **COIL vs. CBC**: COIL is the complete binary format; CBC is an optimized subset for interpretation
- **Native vs. CBC**: Native code is platform-specific compiled code; CBC is for runtime device selection

## Related Components

- [COIL Overview](./coil-docs/index.md) - Detailed information about COIL
- [CASM Overview](./casm-docs/index.md) - Detailed information about CASM
- [CBC Overview](./cbc-docs/index.md) - Detailed information about CBC