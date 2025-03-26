# COIL ISA Overview

## Purpose

This document provides a high-level overview of the COIL Instruction Set Architecture (ISA), explaining its fundamental design principles and organization.

## Core Design Principles

### Type-Determined Instruction Philosophy

COIL's defining characteristic is its type-determined instruction approach. Instructions derive their behavior from operand types rather than having specialized opcodes for different data variants. This approach enables:

- **Compact instruction set**: One opcode for an operation regardless of data type
- **Unified semantics**: Same operation works consistently across different data types
- **Extensibility**: New types can be added without new opcodes
- **Simplified implementation**: Consistent instruction decoding patterns

For example, the single ADD opcode (0x60) works across integers, floating-point values, and vectors, with behavior determined by operand types.

### Binary Format Primacy

COIL is fundamentally a binary instruction format. The text representation (COIL-ASM) is a human-readable interface to this binary format, but the binary encoding is the authoritative definition.

### Architecture Independence

COIL abstracts over architecture-specific details while allowing access to specialized features when needed:
- Universal operations work identically across architectures
- The variable system abstracts over registers
- The ABI system abstracts over calling conventions

## Instruction Format

All COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

## Opcode Space Organization

The COIL opcode space is organized by functional categories:

| Opcode Range | Category | Description |
|--------------|----------|-------------|
| 0x00         | NOP      | No Operation |
| 0x01-0x0F    | Control Flow | Program flow, branches, calls |
| 0x10-0x2F    | Memory Operations | Data movement, variables, memory access |
| 0x30-0x4F    | Reserved | Reserved for multi-device operations (v3) |
| 0x50-0x5F    | Bit Manipulation | Logical operations, shifts, bit-level operations |
| 0x60-0x8F    | Arithmetic | Mathematical operations |
| 0x90-0x9F    | Vector/Array | Operations on multiple data elements |
| 0xA0-0xAF    | Type Operations | Type manipulation, structures, arrays |
| 0xB0-0xBF    | Directives | Processor selection, assembly control |
| 0xC0-0xFE    | Processor-Specific | Architecture/processor-specific instructions |
| 0xFF         | Extensions | Implementation-specific extensions |

## Universal vs. Processor-Specific Instructions

COIL divides instructions into two categories:

1. **Universal Instructions** (0x00-0xBF): Available on all processor types and must be implemented by all COIL processors
2. **Processor-Specific Instructions** (0xC0-0xFF): Available only on specific processors or architectures

## Version Strategy

The COIL ISA is designed to evolve through versions:

- **Version 1**: All universal instructions, CPU-specific processor instructions
- **Version 2**: Enhancements for standard library support
- **Version 3**: Multi-device operations (opcodes 0x30-0x4F)