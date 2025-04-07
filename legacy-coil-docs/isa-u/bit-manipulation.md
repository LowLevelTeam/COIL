# Bit Manipulation Instructions

## Overview

Bit manipulation instructions operate at the bit level, enabling logical operations, shifts, and rotations on data. These operations provide the foundation for bitwise programming, flags manipulation, and low-level data transformation.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x60   | AND      | Bitwise AND |
| 0x61   | OR       | Bitwise OR |
| 0x62   | XOR      | Bitwise XOR |
| 0x63   | NOT      | Bitwise NOT |
| 0x64   | SHL      | Shift left logical |
| 0x65   | SHR      | Shift right logical |
| 0x66   | SAL      | Shift arithmetic left |
| 0x67   | SAR      | Shift arithmetic right |

## Detailed Specifications

### AND (0x60)

Performs a bitwise AND operation.

**Encoding:**
```
// Three-operand form (dest = src1 & src2)
[0x60][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest &= src)
[0x60][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Performs bitwise AND between the operands
- For integral types: Standard bitwise AND
- For vector types: Element-wise bitwise AND
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)

### OR (0x61)

Performs a bitwise OR operation.

**Encoding:**
```
// Three-operand form (dest = src1 | src2)
[0x61][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest |= src)
[0x61][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Performs bitwise OR between the operands
- For integral types: Standard bitwise OR
- For vector types: Element-wise bitwise OR
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)

### XOR (0x62)

Performs a bitwise XOR (exclusive OR) operation.

**Encoding:**
```
// Three-operand form (dest = src1 ^ src2)
[0x62][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest ^= src)
[0x62][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Performs bitwise XOR between the operands
- For integral types: Standard bitwise XOR
- For vector types: Element-wise bitwise XOR
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)

### NOT (0x63)

Performs a bitwise complement (NOT) operation.

**Encoding:**
```
// Two-operand form (dest = ~src)
[0x63][0x02][dest: Operand][src: Operand]

// One-operand form (dest = ~dest)
[0x63][0x01][dest: Operand]
```

**Behavior:**
- Inverts all bits in the operand
- For integral types: Standard bitwise complement
- For vector types: Element-wise bitwise complement
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)

### SHL (0x64)

Performs a logical left shift.

**Encoding:**
```
// Three-operand form (dest = src1 << src2)
[0x64][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest <<= src)
[0x64][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Shifts all bits in the operand left by the specified count
- Fills vacated bits with zeros
- For integral types: Standard logical shift left
- For vector types: Element-wise shift left
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign, overflow)
- If shift count is greater than bit width, result is defined by implementation

### SHR (0x65)

Performs a logical right shift.

**Encoding:**
```
// Three-operand form (dest = src1 >> src2)
[0x65][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest >>= src)
[0x65][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Shifts all bits in the operand right by the specified count
- Fills vacated bits with zeros
- For integral types: Standard logical shift right
- For vector types: Element-wise shift right
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)
- If shift count is greater than bit width, result is defined by implementation

### SAL (0x66)

Performs an arithmetic left shift.

**Encoding:**
```
// Three-operand form (dest = src1 <<< src2)
[0x66][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest <<<= src)
[0x66][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Shifts all bits in the operand left by the specified count
- Fills vacated bits with zeros
- Functionally identical to SHL, included for completeness
- For integral types: Standard arithmetic shift left
- For vector types: Element-wise shift left
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign, overflow)
- If shift count is greater than bit width, result is defined by implementation

### SAR (0x67)

Performs an arithmetic right shift.

**Encoding:**
```
// Three-operand form (dest = src1 >>> src2)
[0x67][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest >>>= src)
[0x67][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Shifts all bits in the operand right by the specified count
- Preserves the sign bit (copies it into vacated positions)
- For integral types: Standard arithmetic shift right
- For vector types: Element-wise arithmetic shift right
- For floating-point types: Implementation-defined or error
- Sets flags based on result (zero, sign)
- If shift count is greater than bit width, result is defined by implementation

## Bitwise Operations on Complex Types

For complex types (vectors, matrices, etc.), bitwise operations generally apply element-wise:

| Type Category | Behavior |
|---------------|----------|
| Vector | Element-wise bitwise operation |
| Matrix | Element-wise bitwise operation |
| Composite | Element-wise or implementation-defined |

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Vector Operations](./vector-operations.md) - Vector processing instructions
- [Type System](../types/type-system.md) - Complete type system reference