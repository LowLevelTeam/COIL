# Arithmetic Instructions (0x60-0x8F)

## Purpose

This document defines the arithmetic instructions in COIL, which perform mathematical operations on numeric data across different data types. These instructions provide the foundation for computational operations in COIL programs.

## Key Concepts

- **Basic Arithmetic**: Core mathematical operations
- **Type-Determined Behavior**: How operations adapt to different data types
- **Special Arithmetic Operations**: Advanced mathematical functions
- **Comparison Operations**: How values are compared
- **Numeric Conversions**: How values are converted between numeric types

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x60   | ADD      | `ADD dest, left, right[, TYPE_PARAM5]` | Addition |
| 0x61   | SUB      | `SUB dest, left, right[, TYPE_PARAM5]` | Subtraction |
| 0x62   | MUL      | `MUL dest, left, right[, TYPE_PARAM5]` | Multiplication |
| 0x63   | DIV      | `DIV dest, left, right[, TYPE_PARAM5]` | Division |
| 0x64   | MOD      | `MOD dest, left, right[, TYPE_PARAM5]` | Modulo |
| 0x65   | INC      | `INC dest[, TYPE_PARAM5]` | Increment by 1 |
| 0x66   | DEC      | `DEC dest[, TYPE_PARAM5]` | Decrement by 1 |
| 0x67   | NEG      | `NEG dest, src[, TYPE_PARAM5]` | Negate (change sign) |
| 0x68   | ABS      | `ABS dest, src[, TYPE_PARAM5]` | Absolute value |
| 0x69   | SQRT     | `SQRT dest, src[, TYPE_PARAM5]` | Square root |
| 0x6A   | FMA      | `FMA dest, a, b, c[, TYPE_PARAM5]` | Fused multiply-add (a*b+c) |
| 0x6B   | CEIL     | `CEIL dest, src[, TYPE_PARAM5]` | Round up to integer |
| 0x6C   | FLOOR    | `FLOOR dest, src[, TYPE_PARAM5]` | Round down to integer |
| 0x6D   | ROUND    | `ROUND dest, src[, TYPE_PARAM5]` | Round to nearest integer |
| 0x6E   | TRUNC    | `TRUNC dest, src[, TYPE_PARAM5]` | Truncate to integer |
| 0x7B   | MIN      | `MIN dest, left, right[, TYPE_PARAM5]` | Minimum value |
| 0x7C   | MAX      | `MAX dest, left, right[, TYPE_PARAM5]` | Maximum value |

## Detailed Descriptions

### ADD (0x60)
Add two values.

#### Syntax
```
ADD dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
ADD dest, src[, TYPE_PARAM5=condition]  ; Equivalent to ADD dest, dest, src
```

#### Binary Encoding
```
0x60                  ; Opcode for ADD
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[left_type]           ; Type of left operand (if three-operand form)
[left_value]          ; Left operand value (if three-operand form)
[right_type]          ; Type of right operand
[right_value]         ; Right operand value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (three-operand form)
ADD result, a, b

; Binary
0x60      ; ADD
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"

; Assembly (two-operand form)
ADD counter, 1

; Binary
0x60      ; ADD
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
0x1320    ; TYPE_UNT32+IMM
0x01000000 ; Value 1
```

### SUB (0x61)
Subtract the second value from the first.

#### Syntax
```
SUB dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SUB dest, src[, TYPE_PARAM5=condition]  ; Equivalent to SUB dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
SUB result, a, b       ; result = a - b

; Assembly (two-operand form)
SUB counter, 1        ; counter = counter - 1
```

### MUL (0x62)
Multiply two values.

#### Syntax
```
MUL dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MUL dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MUL dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
MUL result, a, b       ; result = a * b

; Assembly (two-operand form)
MUL value, 2          ; value = value * 2
```

### DIV (0x63)
Divide the first value by the second.

#### Syntax
```
DIV dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
DIV dest, src[, TYPE_PARAM5=condition]  ; Equivalent to DIV dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
DIV result, a, b       ; result = a / b

; Assembly (two-operand form)
DIV value, 10         ; value = value / 10
```

### INC (0x65)
Increment a value by 1.

#### Syntax
```
INC dest[, TYPE_PARAM5=condition]
```

#### Example
```
; Assembly
INC counter           ; counter += 1
```

### DEC (0x66)
Decrement a value by 1.

#### Syntax
```
DEC dest[, TYPE_PARAM5=condition]
```

#### Example
```
; Assembly
DEC counter           ; counter -= 1
```

### SQRT (0x69)
Compute the square root.

#### Syntax
```
SQRT dest, src[, TYPE_PARAM5=condition]
```

#### Example
```
; Assembly
SQRT result, value    ; result = √value
```

### FMA (0x6A)
Fused multiply-add: (a * b) + c with a single rounding step.

#### Syntax
```
FMA dest, a, b, c[, TYPE_PARAM5=condition]
```

#### Example
```
; Assembly
FMA result, a, b, c   ; result = (a * b) + c
```

### MIN (0x7B)
Find the minimum of two values.

#### Syntax
```
MIN dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MIN dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MIN dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
MIN result, a, b       ; result = min(a, b)

; Assembly (two-operand form)
MIN value, 100        ; value = min(value, 100)
```

### MAX (0x7C)
Find the maximum of two values.

#### Syntax
```
MAX dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MAX dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MAX dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
MAX result, a, b       ; result = max(a, b)

; Assembly (two-operand form)
MAX value, 0          ; value = max(value, 0)
```

## Type-Dependent Behavior

The behavior of arithmetic instructions depends on operand types:

1. **Integer Operations**:
   - Based on two's complement arithmetic
   - Divide-by-zero causes an error
   - Overflow behavior is type-dependent (wrap for unsigned, implementation-defined for signed)

2. **Floating-Point Operations**:
   - Follow IEEE 754 arithmetic rules
   - Support special values (NaN, Infinity)
   - Default rounding mode is round-to-nearest-even

3. **Mixed Type Operations**:
   - Operand promotion rules apply
   - Explicit type conversion recommended for clarity

## Execution Flags

Arithmetic operations typically set the following flags:
- **Zero Flag (Z)**: Set if the result is zero
- **Sign Flag (S)**: Set if the result is negative
- **Overflow Flag (O)**: Set if the operation overflows
- **Carry Flag (C)**: Set for unsigned overflow

These flags can be used by subsequent conditional operations.

## Common Arithmetic Patterns

### Counter Loop

```
; Loop with counter
VAR TYPE_INT32, i, 0
VAR TYPE_INT32, limit, 10

loop_start:
  CMP i, limit
  BR_GE loop_end
  
  ; Loop body...
  
  INC i
  BR loop_start
loop_end:
```

### Absolute Difference

```
; Calculate |a - b|
VAR TYPE_INT32, a, 5
VAR TYPE_INT32, b, 10
VAR TYPE_INT32, diff

; Compute difference
SUB diff, a, b

; Take absolute value
ABS diff
```

### Polynomial Evaluation

```
; Evaluate a*x^2 + b*x + c
VAR TYPE_FP32, a, 1.0
VAR TYPE_FP32, b, 2.0
VAR TYPE_FP32, c, 3.0
VAR TYPE_FP32, x, 2.5
VAR TYPE_FP32, result

; Compute x^2
VAR TYPE_FP32, x_squared
MUL x_squared, x, x

; Compute a*x^2
VAR TYPE_FP32, term1
MUL term1, a, x_squared

; Compute b*x
VAR TYPE_FP32, term2
MUL term2, b, x

; Sum all terms
ADD result, term1, term2
ADD result, result, c
```

### Clamping a Value

```
; Clamp value to [min_val, max_val]
VAR TYPE_INT32, value, 42
VAR TYPE_INT32, min_val, 0
VAR TYPE_INT32, max_val, 100
VAR TYPE_INT32, clamped

; First ensure value >= min_val
MAX clamped, value, min_val

; Then ensure result <= max_val
MIN clamped, clamped, max_val
```

## Optimization Opportunities

COIL processors may optimize arithmetic operations in various ways:

1. **Constant Folding**: Evaluate constant expressions at compile time
2. **Strength Reduction**: Replace complex operations with simpler ones
   - Multiply by power of 2 → Shift left
   - Divide by power of 2 → Shift right (for unsigned)
3. **Common Subexpression Elimination**: Compute shared expressions once
4. **Instruction Combining**: Combine multiple operations (e.g., using FMA)
5. **Vector Processing**: Use SIMD instructions for parallel arithmetic

## Related Documentation

- [CASM Instructions](../../assembly/instructions.md) - General guide to CASM instructions
- [Type System](../../systems/type-system.md) - Type rules for arithmetic operations
- [Bit Manipulation](bit-manipulation.md) - Bit-level operations
- [Vector Operations](vector.md) - SIMD arithmetic operations