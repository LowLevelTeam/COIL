# Instruction Flag Effects

## Overview

This document defines the behavior of condition flags in the COIL architecture. Flags provide essential information about the results of operations and enable conditional execution. Consistent flag behavior is critical for deterministic program execution across different implementations.

## Condition Flag Register

The COIL condition flag register contains the following flags:

| Flag | Symbol | Bit Position | Description |
|------|--------|--------------|-------------|
| Zero | Z | 0 | Set when result is zero |
| Sign | S | 1 | Set when result is negative |
| Carry | C | 2 | Set when operation produces a carry |
| Overflow | V | 3 | Set when operation produces an overflow |
| Divide-by-Zero | D | 4 | Set when division by zero occurs |
| NaN | N | 5 | Set when operation produces Not-a-Number |
| Parity | P | 6 | Set when result has even parity |

## Flag Behavior by Instruction Category

This section defines which flags are affected by each instruction and how they are set.

### Control Flow Instructions

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| NOP | - | - | - | - | - | - | - | No flags affected |
| BR  | - | - | - | - | - | - | - | No flags affected |
| CALL | - | - | - | - | - | - | - | No flags affected |
| RET | - | - | - | - | - | - | - | No flags affected |
| CMP | ✓ | ✓ | ✓ | ✓ | - | ✓ | - | Set based on (op1 - op2) |
| TEST | ✓ | ✓ | 0 | 0 | - | - | ✓ | Set based on (op1 & op2) |

### Memory Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| MOV | - | - | - | - | - | - | - | No flags affected |
| PUSH | - | - | - | - | - | - | - | No flags affected |
| POP | - | - | - | - | - | - | - | No flags affected |
| LEA | - | - | - | - | - | - | - | No flags affected |
| SCOPE | - | - | - | - | - | - | - | No flags affected |
| SCOPL | - | - | - | - | - | - | - | No flags affected |
| VAR | - | - | - | - | - | - | - | No flags affected |
| XCHG | - | - | - | - | - | - | - | No flags affected |
| CAS | ✓ | - | - | - | - | - | - | Z set if exchange occurred |

### Arithmetic Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| ADD | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| SUB | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| MUL | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| DIV | ✓ | ✓ | - | ✓ | ✓ | ✓ | ✓ | D set if divisor is zero |
| MOD | ✓ | ✓ | - | ✓ | ✓ | ✓ | ✓ | D set if divisor is zero |
| INC | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| DEC | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| NEG | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Set based on result |
| ABS | ✓ | 0 | - | ✓ | - | ✓ | ✓ | S always cleared (result ≥ 0) |
| SQRT | ✓ | 0 | - | - | - | ✓ | ✓ | N set if operand < 0 |
| CEIL | ✓ | ✓ | - | - | - | ✓ | ✓ | Set based on result |
| FLOR | ✓ | ✓ | - | - | - | ✓ | ✓ | Set based on result |
| ROND | ✓ | ✓ | - | - | - | ✓ | ✓ | Set based on result |

### Bit Manipulation Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| AND | ✓ | ✓ | 0 | 0 | - | - | ✓ | C,V cleared; Z,S,P set based on result |
| OR  | ✓ | ✓ | 0 | 0 | - | - | ✓ | C,V cleared; Z,S,P set based on result |
| XOR | ✓ | ✓ | 0 | 0 | - | - | ✓ | C,V cleared; Z,S,P set based on result |
| NOT | ✓ | ✓ | - | - | - | - | ✓ | Z,S,P set based on result |
| SHL | ✓ | ✓ | ✓ | ✓ | - | - | ✓ | C set from last bit shifted out |
| SHR | ✓ | ✓ | ✓ | 0 | - | - | ✓ | C set from last bit shifted out |
| SAL | ✓ | ✓ | ✓ | ✓ | - | - | ✓ | C set from last bit shifted out |
| SAR | ✓ | ✓ | ✓ | 0 | - | - | ✓ | C set from last bit shifted out |

### Vector Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| GETE | ✓ | ✓ | - | - | - | ✓ | - | Set based on extracted element |
| SETE | - | - | - | - | - | - | - | No flags affected |
| DOT | ✓ | ✓ | - | ✓ | - | ✓ | - | Set based on dot product result |
| CROSS | ✓ | - | - | - | - | - | - | Z set if result is zero vector |
| NORM | ✓ | - | - | - | ✓ | ✓ | - | Z set if input is zero vector |
| LEN | ✓ | 0 | - | - | - | - | - | Z set if input is zero vector |
| SHUF | - | - | - | - | - | - | - | No flags affected |
| EXTRACT | - | - | - | - | - | - | - | No flags affected |
| INSERT | - | - | - | - | - | - | - | No flags affected |
| TRANS | - | - | - | - | - | - | - | No flags affected |
| INV | ✓ | - | - | - | ✓ | - | - | Z set if matrix is singular |
| DET | ✓ | ✓ | - | - | - | - | - | Z set if determinant is zero |
| ROW | - | - | - | - | - | - | - | No flags affected |
| COL | - | - | - | - | - | - | - | No flags affected |
| DIAG | - | - | - | - | - | - | - | No flags affected |

## Flag Setting Rules

### Zero Flag (Z)

- Set when the result is exactly zero
- For vector/matrix operations, set when all elements are zero
- For boolean operations, set when result is false (0)
- Not affected by operations that don't produce a result

### Sign Flag (S)

- Set when the most significant bit of the result is 1 (result is negative)
- For vector/matrix operations, set based on the sign of the first element
- For unsigned operations, always cleared
- Not affected by operations that don't produce a scalar result

### Carry Flag (C)

- Set when an arithmetic operation produces a carry out of the most significant bit
- Set when a shift/rotate operation shifts out a 1 bit
- Cleared by logical operations
- Not affected by operations that don't involve carries

### Overflow Flag (V)

- Set when a signed arithmetic operation produces a result too large for the destination
- Set when shifting a signed value changes the sign bit inappropriately
- Cleared by logical operations
- Not affected by operations where overflow is impossible

### Divide-by-Zero Flag (D)

- Set when a division operation has a divisor of zero
- Set when a matrix inversion operation encounters a singular matrix
- Not affected by non-division operations

### NaN Flag (N)

- Set when a floating-point operation produces Not-a-Number
- Set when an invalid operation is attempted (sqrt of negative number, etc.)
- Not affected by integer operations

### Parity Flag (P)

- Set when the result has an even number of set bits (even parity)
- Cleared when the result has an odd number of set bits (odd parity)
- Only affected by operations that produce a scalar result
- Not affected by operations on non-scalar types

## Type-Dependent Flag Behavior

Flag behavior varies based on operand types:

### Integer Operations

- Zero, Sign, Carry, Overflow flags set according to standard integer arithmetic
- NaN flag always cleared
- Parity flag set based on result bits

### Floating-Point Operations

- Zero flag set if result is exactly 0.0
- Sign flag set if result is negative
- Overflow flag set if result exceeds representable range
- NaN flag set if result is NaN
- Carry and Parity flags not typically affected

### Vector/Matrix Operations

- Zero flag set if all elements are zero
- Other flags typically reflect the operation on the first element or are aggregated across elements

## Conditional Execution

Flags enable conditional execution through the BR, CALL, and RET instructions with condition codes:

| Condition Code | Mnemonic | Flag Expression | Description |
|----------------|----------|-----------------|-------------|
| 0x00 | EQ | Z = 1 | Equal (Zero) |
| 0x01 | NEQ | Z = 0 | Not Equal (Not Zero) |
| 0x02 | GT | Z = 0 & S = V | Greater Than |
| 0x03 | GTE | Z = 1 or S = V | Greater Than or Equal |
| 0x04 | LT | S ≠ V | Less Than |
| 0x05 | LTE | Z = 1 or S ≠ V | Less Than or Equal |
| 0x06 | O | V = 1 | Overflow |
| 0x07 | NO | V = 0 | No Overflow |
| 0x08 | S | S = 1 | Sign (Negative) |
| 0x09 | NS | S = 0 | Not Sign (Not Negative) |
| 0x0A | Z | Z = 1 | Zero |
| 0x0B | NZ | Z = 0 | Not Zero |
| 0x0C | C | C = 1 | Carry |
| 0x0D | NC | C = 0 | No Carry |
| 0x0E | D | D = 1 | Divide by Zero |
| 0x0F | ND | D = 0 | No Divide by Zero |
| 0x10 | N | N = 1 | NaN Result |
| 0x11 | NN | N = 0 | Not NaN Result |
| 0x12 | P | P = 1 | Even Parity |
| 0x13 | NP | P = 0 | Odd Parity |

## Flag Preservation

Instructions that do not affect specific flags must preserve their previous values. Implementations must not reset flag values unless explicitly specified.

## Related Components

- [Control Flow](../isa-u/control-flow.md) - Instructions that use flags for conditional execution
- [Arithmetic Operations](../isa-u/arithmetic-operations.md) - Operations that set flags
- [Binary Format](../format/binary-format.md) - Binary encoding of instructions