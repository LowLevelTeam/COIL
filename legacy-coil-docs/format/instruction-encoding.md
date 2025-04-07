# Instruction Encoding Clarifications

## Overview

This document clarifies the binary encoding of COIL instructions with special attention to edge cases like zero-operand instructions and complex operand types. These specifications ensure consistent implementation across different platforms.

## Basic Instruction Structure

Every COIL instruction follows this structure:

```
[opcode: uint8_t] [extended opcode: optional uint8_t] [operand_count: uint8_t] [[operand: Operand], ...operand_count]
```

## Zero-Operand Instructions

### Definition

Zero-operand instructions are those that take no parameters, such as NOP, RET (without condition), PUSH/POP, etc.

### Encoding Rules

For zero-operand instructions:

1. The operand count byte is **always present** and set to `0x00`
2. No operand data follows the count byte
3. The total instruction length is 2 bytes (opcode + operand count)

### Example: NOP Instruction

The NOP instruction (opcode 0x00) is encoded as:

```
0x00 0x00
```

Where:
- `0x00` is the opcode
- `0x00` is the operand count (zero)

### Example: RET Instruction

The unconditional RET instruction (opcode 0x03) is encoded as:

```
0x03 0x00
```

Where:
- `0x03` is the opcode
- `0x00` is the operand count (zero)

## Fixed-Operand Instructions

Instructions with a fixed number of operands still include the operand count byte, which must match the expected count for that instruction.

### Example: One-Operand Form of NEG

The one-operand form of NEG (opcode 0x47) is encoded as:

```
0x47 0x01 [operand]
```

Where:
- `0x47` is the opcode
- `0x01` is the operand count (one)
- `[operand]` is the encoded operand data

## Extended Opcode Instructions

When using extended opcodes (primary opcode 0xFF):

1. The extended opcode byte is **mandatory**
2. The operand count byte follows the extended opcode
3. The total instruction length is at minimum 3 bytes (primary opcode + extended opcode + operand count)

### Example: Vendor Extension

A vendor-specific instruction with extended opcode 0x42 and no operands:

```
0xFF 0x42 0x00
```

Where:
- `0xFF` is the primary opcode indicating an extension
- `0x42` is the extended opcode (vendor-specific)
- `0x00` is the operand count (zero)

## Operand Count Validation

1. Each instruction has a defined minimum and maximum number of operands
2. Providing fewer than the minimum required operands is an error
3. Providing more than the maximum allowed operands is an error
4. The absolute maximum for any instruction is 255 operands (limit of uint8_t)

### Instruction-Specific Limits

| Instruction | Minimum Operands | Maximum Operands | Notes |
|-------------|------------------|------------------|-------|
| NOP         | 0                | 0                | Always zero operands |
| BR          | 1                | 2                | 1 for unconditional, 2 for conditional |
| ADD         | 2                | 3                | 2 for in-place, 3 for three-operand form |
| MUL         | 2                | 3                | 2 for in-place, 3 for three-operand form |
| NEG         | 1                | 2                | 1 for in-place, 2 for two-operand form |

## Operand Encoding Examples

### Integer Immediate Value

A 32-bit integer immediate value (42):

```
0x04 0x10 0x0000002A
```

Where:
- `0x04` is the type opcode (INT32)
- `0x10` is the type extension (IMMEDIATE flag)
- `0x0000002A` is the value (42 in little-endian)

### Variable Reference

A variable reference to variable ID 1:

```
0x04 0x40 0x0000000000000001
```

Where:
- `0x04` is the type opcode (INT32)
- `0x40` is the type extension (VARIABLE flag)
- `0x0000000000000001` is the variable ID (1 in little-endian)

### Complex Vector Type

A vector of four 32-bit floating-point values:

```
0x33 0x00 0x10 0x00 0x0004
```

Where:
- `0x33` is the type opcode (CVEC)
- `0x00` is the type extension (no flags)
- `0x10 0x00` is the element type (FP32)
- `0x0004` is the element count (4)

## Complete Instruction Examples

### ADD (Three-Operand Form)

Adding two variables (a = b + c):

```
0x40 0x03 [a_operand] [b_operand] [c_operand]
```

Where:
- `0x40` is the opcode (ADD)
- `0x03` is the operand count (three)
- `[a_operand]` is the destination operand
- `[b_operand]` is the first source operand
- `[c_operand]` is the second source operand

### CMP (Two-Operand Form)

Comparing two values:

```
0x04 0x02 [op1_operand] [op2_operand]
```

Where:
- `0x04` is the opcode (CMP)
- `0x02` is the operand count (two)
- `[op1_operand]` is the first operand
- `[op2_operand]` is the second operand

## Implementation Requirements

1. **Operand Count Validation**
   - Implementations must validate that the operand count matches instruction requirements
   - For instructions with variable operand counts, validate against minimum and maximum limits

2. **Consistent Encoding**
   - Always include the operand count byte, even for zero-operand instructions
   - Follow the type extension encoding rules consistently

3. **Error Handling**
   - Report clear errors for invalid operand counts
   - Validate operand types against instruction requirements