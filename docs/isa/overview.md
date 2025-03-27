# COIL ISA Overview

## Purpose

This document provides a high-level overview of the COIL Instruction Set Architecture (ISA), explaining its organization and key characteristics. It serves as an introduction to the detailed instruction category documents.

## Instruction Format

All COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

The opcode identifies the operation to perform, while the operand count indicates how many operands follow. The type field for each operand determines how the instruction behaves.

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

### Universal Instruction Categories

#### Control Flow (0x01-0x0F)
Instructions that control program flow including branches, jumps, function calls, and returns. Key instructions include `SYM`, `BR`, `CALL`, `RET`, `CMP`, and `SWITCH`.

#### Memory Operations (0x10-0x2F)
Instructions for memory access, variable management, and data movement. Key instructions include `MOV`, `PUSH`, `POP`, `VAR`, `SCOPEE`, and `SCOPEL`.

#### Bit Manipulation (0x50-0x5F)
Bitwise operations, shifts, and rotations. Key instructions include `AND`, `OR`, `XOR`, `SHL`, `SHR`, and `POPCNT`.

#### Arithmetic (0x60-0x8F)
Mathematical operations including basic arithmetic, comparison, and specialized math. Key instructions include `ADD`, `SUB`, `MUL`, `DIV`, and `FMA`.

#### Vector/Array Operations (0x90-0x9F)
SIMD and vector processing instructions. Key instructions include `VLOAD`, `VSTORE`, `VCMP`, and `VDOT`.

#### Type Operations (0xA0-0xAF)
Type manipulation and access to typed composite data. Key instructions include `CONVERT`, `CAST`, `GET`, and `SET`.

#### Directives (0xB0-0xBF)
Assembly and processor control instructions. Key directives include `PROC`, `ARCH`, `SECTION`, and `ABI`.

### Processor-Specific Instructions (0xC0-0xFF)

These instructions provide access to specialized hardware features. They are divided into:

- **Processing Unit Features** (0xC0-0xCF): Independent of architecture
- **Architecture-Specific Instructions** (0xD0-0xFE): Tied to specific architectures

## Conditional Execution

Many COIL instructions support conditional execution using branch condition codes:

```
BR_EQ label  ; Branch if equal
ADD_GT a, b, c  ; Add if greater than
```

This enables efficient implementation of conditional logic without requiring separate branch instructions.

## ABI Integration

Instructions like `CALL` and `RET` integrate with the ABI system for parameter passing and return value handling:

```
CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
```

This abstracts over the specific calling conventions of different platforms.

## Version Strategy

The COIL ISA is designed to evolve through versions:

- **Version 1**: All universal instructions, CPU-specific processor instructions
- **Version 2**: Enhancements for standard library support
- **Version 3**: Multi-device operations (opcodes 0x30-0x4F)

## Related Documentation

For detailed information on specific instruction categories, see:
- [Control Flow Instructions](control-flow.md)
- [Memory Operations](memory.md)
- [Bit Manipulation Instructions](bit-manipulation.md)
- [Arithmetic Instructions](arithmetic.md)
- [Vector/Array Operations](vector.md)
- [Type Operations](type.md)
- [Directive Instructions](directive.md)
- [Processor-Specific Instructions](processor-specific.md)
- [Binary Encoding](binary-encoding.md)