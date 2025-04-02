# Introduction to COIL

## Overview

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed to enable portable, high-performance code across diverse computing architectures. COIL bridges the gap between high-level programming languages and hardware-specific machine code.

## Purpose

COIL addresses several key challenges in modern computing:

1. **Hardware Diversity**: Computing devices span CPUs, GPUs, FPGAs, and specialized accelerators
2. **Portability vs. Performance**: Traditional approaches sacrifice one for the other
3. **Complexity of Optimization**: Manually optimizing for each platform is labor-intensive
4. **Binary Compatibility**: Ensuring code works across different architectures

## Core Design Philosophy

COIL is built on these foundational concepts:

### Type-Determined Instructions

Instructions derive their behavior from operand types, allowing a single opcode to handle different data types:

```
ADD result, a, b  ; Works for integers, floats, or vectors based on operand types
```

This approach provides:
- A compact instruction set
- Consistent behavior patterns across types
- Natural extensibility as new types are added

This approach causes:
- slower compiling / interpreting

### Architecture Independence

COIL maintains a clean separation between:
- **Universal Operations** (0x00-0x9F): Guaranteed to work identically across all platforms;
  1. Control Flow (0x00-0x1F)
  2. Memory Operations (0x20-0x3F)
  3. Arithmetic Operations (0x40-0x5F)
  4. Bit Manipulation (0x60-0x7F)
  5. Vector/Array Operations (0x80-x8F)
  6. Special Operations (0x90-0x9F)
- **PU Operations** (0xA0-0x..): Guaranteed to work identically across identical processing units (CPU, GPU, ...);
- **Architecture-Specific Operations** (0x..-0x..): Tailored to specific architectures (x86, ARM, ...)
- **Mode-Specific Operations** (0x..-0xDF): Tailored to specific architecture modes (16 bit, 32 bit, ...)
- **Type Operations**: (0xE0-0xEF): Type Operations
- **Directive Operations** (0xF0-FE): Direct communication with the COIL Processor
- **Extension Operations** (0xFF): For vendor-specific features followed by another vendor specified opcode


Separated into three main instruction sets as follows:
- [ISA Universal](./isa-u/index.md)
- [ISA Extended](./isa-e/index.md)
- [ISA Compiler](./isa-c/index.md)

## COIL in the Toolchain

COIL serves as an intermediate representation in a complete toolchain:

```
Source Langauge (.*/.casm) → Frontend Compiler → COIL (.coil) →

.coil → COIL Processor → COIL Compiled Code (.ccc) → COIL Linker → Executable/Library

.coil → COIL Processor → .acoil (COIL Library)

.coil → COIL Processor → .dcoil (COIL Dynamic Library)
```

The COIL ecosystem includes:
- **CASM**: Text representation of COIL for direct programming
- **COIL**: COIL Code stored in an object format using default instruction set
- **CCC**: COIL Compiled Code stored in an object format (contains either CBC (COIL Byte Code) for JIT and interpretation or native machine code)


## Related Components

- [CBC](../cbc-docs/index.md) - COIL Byte Code
- [CASM](../casm-docs/index.md) - COIL for Humans
- [ISA-U](./isa-u/index.md) - Universal ISA
- [ISA-E](./isa-e/index.md) - Extended ISA
- [ISA-C](./isa-c/index.md) - Compiler ISA
