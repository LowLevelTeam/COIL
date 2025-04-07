# Universal Instruction Set Overview

## Purpose

The Universal Instruction Set (ISA-U) provides a platform-independent foundation for COIL programs. These instructions work identically across all processing units, architectures, and modes, ensuring maximum portability while maintaining performance.

## Instruction Categories

The Universal ISA is organized into the following categories:

| Opcode Range | Category | Purpose |
|--------------|----------|---------|
| 0x00-0x1F | Control Flow | Program execution control |
| 0x20-0x3F | Memory Operations | Memory management and access |
| 0x40-0x5F | Arithmetic Operations | Mathematical computation |
| 0x60-0x7F | Bit Manipulation | Bitwise operations |
| 0x80-0x8F | Vector/Array Operations | Vector and array processing |
| 0x90-0x9F | Special Operations | Specialized instructions |
| 0xE0-0xEF | Type Operations | Direct functionality for types |

## Type-Determined Behavior

Universal instructions derive their behavior from operand types, allowing a single opcode to function across different data types. For example:

```
ADD result, a, b
```

This single instruction works for integers, floats, vectors, matrices, and tensors based on the operand types.

## Execution Model

Universal instructions operate within a unified execution model:

1. **Variable System**: Abstracts hardware-specific registers and memory
2. **Type System**: Determines instruction behavior based on operand types
3. **Control Flow**: Platform-agnostic execution control

## Implementation Requirements

For a processing unit to be COIL-compliant, it must implement all Universal ISA instructions with their defined semantics. This ensures that code using only Universal instructions will execute identically across all COIL platforms.

## Related Components

- [Control Flow](./control-flow.md) - Execution flow instructions
- [Memory Operations](./memory-operations.md) - Memory management instructions
- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Bit Manipulation](./bit-manipulation.md) - Bitwise operation instructions
- [Vector Operations](./vector-operations.md) - Vector processing instructions
- [Type Operations](./type.md) - Type Operations
- [Type System](../types/type-system.md) - Complete type system reference