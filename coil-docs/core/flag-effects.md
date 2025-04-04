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

## Flag Behavior Definitions

Each flag has specific conditions that determine when it is set or cleared:

### Zero Flag (Z)
- **Set** when the result is exactly zero
- For vector/matrix operations, set only when **all** elements are zero
- For boolean operations, set when the result is false (0)
- **Cleared** when the result is non-zero

### Sign Flag (S)
- **Set** when the most significant bit of the result is 1 (result is negative)
- For vector/matrix operations, reflects the sign of the first element
- For unsigned operations, always cleared
- **Cleared** when the result is zero or positive

### Carry Flag (C)
- **Set** when:
  - An addition generates a carry out of the most significant bit
  - A subtraction requires a borrow beyond the most significant bit
  - A shift/rotate operation shifts out a 1 bit
- **Cleared** when no carry/borrow occurs
- Not affected by logical operations (AND, OR, XOR) which explicitly clear it

### Overflow Flag (V)
- **Set** when the operation produces a result too large for the destination (signed arithmetic overflow)
- For signed addition: Set when operands with the same sign produce a result with different sign
- For signed subtraction: Set when operands with different signs produce a result with sign matching the subtrahend
- **Cleared** when no overflow occurs
- Logical operations (AND, OR, XOR) explicitly clear this flag

### Divide-by-Zero Flag (D)
- **Set** when a division or modulo operation has a divisor of zero
- **Set** when a matrix inversion operation encounters a singular matrix
- **Cleared** by all other operations
- Once set, remains set until cleared by an operation that affects it

### NaN Flag (N)
- **Set** when a floating-point operation produces a Not-a-Number result
- **Set** when an invalid floating-point operation is attempted (sqrt of negative, etc.)
- **Cleared** by operations that don't produce NaN
- Integer operations never set this flag

### Parity Flag (P)
- **Set** when the result has an even number of set bits (even parity)
- **Cleared** when the result has an odd number of set bits (odd parity)
- For vector/matrix operations, typically reflects parity of the first element
- Only affected by operations that produce a scalar integer result

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
| ADD | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if carry out<br>V: set if signed overflow<br>N: set if floating-point NaN<br>P: set if even parity |
| SUB | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if borrow needed<br>V: set if signed overflow<br>N: set if floating-point NaN<br>P: set if even parity |
| MUL | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if upper half is non-zero<br>V: set if result cannot fit<br>N: set if floating-point NaN<br>P: set if even parity |
| DIV | ✓ | ✓ | - | ✓ | ✓ | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>V: set if result overflows<br>D: set if divisor is zero<br>N: set if floating-point NaN<br>P: set if even parity |
| MOD | ✓ | ✓ | - | ✓ | ✓ | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>V: set if result overflows<br>D: set if divisor is zero<br>N: set if floating-point NaN<br>P: set if even parity |
| INC | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if carry out<br>V: set if signed overflow<br>N: set if floating-point NaN<br>P: set if even parity |
| DEC | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if borrow needed<br>V: set if signed overflow<br>N: set if floating-point NaN<br>P: set if even parity |
| NEG | ✓ | ✓ | ✓ | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set if operand is non-zero<br>V: set if operand is minimum value<br>N: set if floating-point NaN<br>P: set if even parity |
| ABS | ✓ | 0 | - | ✓ | - | ✓ | ✓ | Z: set if result is zero<br>S: always cleared (result ≥ 0)<br>V: set if operand is minimum value<br>N: set if floating-point NaN<br>P: set if even parity |
| SQRT | ✓ | 0 | - | - | - | ✓ | ✓ | Z: set if result is zero<br>S: always cleared (result ≥ 0)<br>N: set if operand < 0<br>P: set if even parity |
| CEIL | ✓ | ✓ | - | - | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>N: set if floating-point NaN<br>P: set if even parity |
| FLOR | ✓ | ✓ | - | - | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>N: set if floating-point NaN<br>P: set if even parity |
| ROND | ✓ | ✓ | - | - | - | ✓ | ✓ | Z: set if result is zero<br>S: set if result is negative<br>N: set if floating-point NaN<br>P: set if even parity |

### Bit Manipulation Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| AND | ✓ | ✓ | 0 | 0 | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: always cleared<br>V: always cleared<br>P: set if even parity |
| OR  | ✓ | ✓ | 0 | 0 | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: always cleared<br>V: always cleared<br>P: set if even parity |
| XOR | ✓ | ✓ | 0 | 0 | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: always cleared<br>V: always cleared<br>P: set if even parity |
| NOT | ✓ | ✓ | - | - | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>P: set if even parity |
| SHL | ✓ | ✓ | ✓ | ✓ | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set from last bit shifted out<br>V: set if sign bit changes<br>P: set if even parity |
| SHR | ✓ | ✓ | ✓ | 0 | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set from last bit shifted out<br>V: always cleared<br>P: set if even parity |
| SAL | ✓ | ✓ | ✓ | ✓ | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set from last bit shifted out<br>V: set if sign bit changes<br>P: set if even parity |
| SAR | ✓ | ✓ | ✓ | 0 | - | - | ✓ | Z: set if result is zero<br>S: set if result is negative<br>C: set from last bit shifted out<br>V: always cleared<br>P: set if even parity |

### Vector Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| GETE | ✓ | ✓ | - | - | - | ✓ | - | Z: set if extracted element is zero<br>S: set if extracted element is negative<br>N: set if extracted element is NaN |
| SETE | - | - | - | - | - | - | - | No flags affected |
| DOT | ✓ | ✓ | - | ✓ | - | ✓ | - | Z: set if dot product is zero<br>S: set if dot product is negative<br>V: set if dot product overflows<br>N: set if result is NaN |
| CROSS | ✓ | - | - | - | - | - | - | Z: set if result is zero vector |
| NORM | ✓ | - | - | - | ✓ | ✓ | - | Z: set if input is zero vector<br>D: set if normalization by zero attempted<br>N: set if result contains NaN |
| LEN | ✓ | 0 | - | - | - | - | - | Z: set if input is zero vector<br>S: always cleared (result ≥ 0) |
| SHUF | - | - | - | - | - | - | - | No flags affected |
| EXTRACT | - | - | - | - | - | - | - | No flags affected |
| INSERT | - | - | - | - | - | - | - | No flags affected |
| TRANS | - | - | - | - | - | - | - | No flags affected |
| INV | ✓ | - | - | - | ✓ | - | - | Z: set if determinant is zero<br>D: set if matrix is singular |
| DET | ✓ | ✓ | - | - | - | - | - | Z: set if determinant is zero<br>S: set if determinant is negative |
| ROW | - | - | - | - | - | - | - | No flags affected |
| COL | - | - | - | - | - | - | - | No flags affected |
| DIAG | - | - | - | - | - | - | - | No flags affected |

### Special Operations

| Instruction | Z | S | C | V | D | N | P | Description |
|-------------|---|---|---|---|---|---|---|-------------|
| RNG | - | - | - | - | - | - | - | No flags affected |
| HASH | - | - | - | - | - | - | - | No flags affected |
| ENCRYPT | - | - | - | - | - | - | - | No flags affected |
| DECRYPT | - | - | - | - | - | - | - | No flags affected |
| VERIFY | ✓ | - | - | - | - | - | - | Z: set if verification fails |

## Type-Dependent Flag Behavior

Flag behavior varies based on operand types:

### Integer Operations

- **Zero Flag (Z)**: Set if result is exactly 0
- **Sign Flag (S)**: Set if most significant bit is 1
- **Carry Flag (C)**: Set if operation carries out of the most significant bit
- **Overflow Flag (V)**: Set if signed result cannot be represented in the destination type
- **NaN Flag (N)**: Never set
- **Parity Flag (P)**: Set based on number of 1 bits in the result

### Floating-Point Operations

- **Zero Flag (Z)**: Set if result is exactly 0.0
- **Sign Flag (S)**: Set if result is negative
- **Carry Flag (C)**: Not typically affected
- **Overflow Flag (V)**: Set if result exceeds representable range
- **NaN Flag (N)**: Set if result is NaN
- **Parity Flag (P)**: Not typically affected

### Vector/Matrix Operations

- **Zero Flag (Z)**: Set if all elements are zero
- **Sign Flag (S)**: Typically reflects sign of first element
- **Carry/Overflow Flags (C/V)**: Reflect element-wise operations
- **NaN Flag (N)**: Set if any element is NaN
- **Parity Flag (P)**: Not typically affected

## Conditional Execution

Flags enable conditional execution through the BR, CALL, and RET instructions with condition codes:

| Condition Code | Mnemonic | Description | Flag Expression |
|----------------|----------|-------------|-----------------|
| 0x00 | EQ | Equal (Zero) | Z = 1 |
| 0x01 | NEQ | Not Equal (Not Zero) | Z = 0 |
| 0x02 | GT | Greater Than | Z = 0 & S = V |
| 0x03 | GTE | Greater Than or Equal | Z = 1 or S = V |
| 0x04 | LT | Less Than | S ≠ V |
| 0x05 | LTE | Less Than or Equal | Z = 1 or S ≠ V |
| 0x06 | O | Overflow | V = 1 |
| 0x07 | NO | No Overflow | V = 0 |
| 0x08 | S | Sign (Negative) | S = 1 |
| 0x09 | NS | Not Sign (Not Negative) | S = 0 |
| 0x0A | Z | Zero | Z = 1 |
| 0x0B | NZ | Not Zero | Z = 0 |
| 0x0C | C | Carry | C = 1 |
| 0x0D | NC | No Carry | C = 0 |
| 0x0E | D | Divide by Zero | D = 1 |
| 0x0F | ND | No Divide by Zero | D = 0 |
| 0x10 | N | NaN Result | N = 1 |
| 0x11 | NN | Not NaN Result | N = 0 |
| 0x12 | P | Even Parity | P = 1 |
| 0x13 | NP | Odd Parity | P = 0 |

## Flag Preservation

Instructions that do not affect specific flags must preserve their previous values. Implementations must not reset flag values unless explicitly specified in the instruction behavior.

## Flag Edge Cases

This section clarifies how flags behave in special circumstances:

### Zero-value Operations
- `NEG 0` sets Z=1, S=0, C=0, V=0
- `ABS 0` sets Z=1, S=0, V=0
- `NOT 0` sets Z=0, S depends on data type width

### Minimum Signed Value Operations
- `NEG INT32_MIN (-2147483648)` sets Z=0, S=1, C=1, V=1
- `ABS INT32_MIN (-2147483648)` sets Z=0, S=0, V=1 (result still negative in 2's complement)

### Floating-Point Special Values
- Operations with NaN set N=1 and generally leave other flags unmodified
- Operations with ±Infinity follow IEEE-754 rules and set flags accordingly
- Comparisons with NaN generally set Z=0, S=0, N=1

### Vector/Matrix Dimensions
- Operations on empty vectors (length 0) set Z=1
- Operations on empty matrices (any dimension is 0) set Z=1

## Related Components

- [Control Flow](../isa-u/control-flow.md) - Instructions that use flags for conditional execution
- [Arithmetic Operations](../isa-u/arithmetic-operations.md) - Operations that set flags
- [Binary Format](../format/binary-format.md) - Binary encoding of instructions