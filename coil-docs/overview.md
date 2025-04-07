# COIL Core Specification

## Overview

The COIL Core Specification defines the fundamental elements of the Computer Oriented Intermediate Language that are required in all compliant implementations. This document provides an overview of these core components.

## Core Components

### Type System

The COIL type system defines how data is represented and manipulated:

- **Fixed-Width Types**: Standard integer and floating-point types
- **Type Extensions**: Modifiers that affect type behavior (const, volatile, atomic)
- **Type-Determined Behavior**: Instructions adapt based on operand types

### Binary Format

COIL uses a compact binary encoding format:

- **Instructions**: `[opcode][operand count][operands...]`
- **Operands**: `[type][type extension][type data][value]`
- **Object Format**: Header, sections, symbols, and relocation information

### Memory Model

The memory model defines how programs interact with memory:

- **Memory Regions**: Code, data, stack
- **Memory Protection**: Read, write, execute permissions
- **Memory Alignment**: Natural alignment requirements for types
- **Endianness**: Byte order handling

### Instruction Sets

COIL organizes instructions into three categories:

1. **Universal ISA (0x00-0x9F)**: Core instructions that work on all platforms
   - Control flow, memory operations, arithmetic, bit manipulation
   - Vector operations, special operations, type operations

2. **Extended ISA (0xA0-0xDF)**: Platform-specific instructions
   - Organized by processing unit, architecture, and mode
   - Only available when targeting the specific platform

3. **Compiler ISA (0xF0-0xFE)**: Directives for the COIL processor
   - Conditional compilation, code organization
   - ABI definitions, compile-time constants

### Linking Model

The linking model defines how COIL object files are combined:

- **Symbol Resolution**: Rules for resolving symbols across modules
- **Section Merging**: How sections are combined during linking
- **Relocation**: How references are updated during linking

## Core Instruction Categories

| Opcode Range | Category | Purpose | Extension |
|--------------|----------|---------|-----------|
| 0x00-0x1F | Control Flow | Program execution control | Core |
| 0x20-0x3F | Memory Operations | Memory management and access | Core |
| 0x40-0x5F | Arithmetic Operations | Mathematical computation | Core |
| 0x60-0x7F | Bit Manipulation | Bitwise operations | Core |
| 0x80-0x8F | Vector/Array Operations | Vector and array processing | Core |
| 0x90-0x9F | Special Operations | Specialized instructions | Core |
| 0xA0-0xDF | Extended Operations | Platform-specific instructions | Processor |
| 0xE0-0xEF | Type Operations | Direct functionality for types | Core |
| 0xF0-0xFE | Compiler Operations | Compilation control | Preprocessor |
| 0xFF | Extension Operations | Vendor-specific features | Vendor |

## Conformance Requirements

For a system to be COIL-compliant, it must:

1. Support all core types and operations
2. Implement the binary format as specified
3. Follow the memory model requirements
4. Provide the universal instruction set
5. Support the linking model for combining modules

## Related Documents

- [Type System](/core/type-system.md) - Detailed type system specification
- [Binary Format](/core/binary-format.md) - Binary encoding details
- [Instruction Set](/core/instruction-set.md) - Universal instruction reference
- [Memory Model](/core/memory-model.md) - Memory organization and management
- [Flag Effects](/core/flag-effects.md) - Condition flag behavior
- [Linking Model](/core/linking-model.md) - Object file linking specification
