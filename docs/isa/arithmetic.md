# Arithmetic Instructions (0x60-0x8F)

## Purpose

This document defines the arithmetic instructions in COIL, which perform mathematical operations on numeric data across different data types.

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

#### Assembly Syntax
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

#### Assembly Syntax
```
SUB dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SUB dest, src[, TYPE_PARAM5=condition]  ; Equivalent to SUB dest, dest, src
```

#### Binary Encoding
```
0x61                  ; Opcode for SUB
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
SUB result, a, b

; Binary
0x61      ; SUB
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"

; Assembly (two-operand form)
SUB counter, 1

; Binary
0x61      ; SUB
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
0x1320    ; TYPE_UNT32+IMM
0x01000000 ; Value 1
```

### MUL (0x62)
Multiply two values.

#### Assembly Syntax
```
MUL dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MUL dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MUL dest, dest, src
```

#### Binary Encoding
```
0x62                  ; Opcode for MUL
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
MUL result, a, b

; Binary
0x62      ; MUL
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"

; Assembly (two-operand form)
MUL value, 2

; Binary
0x62      ; MUL
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1320    ; TYPE_UNT32+IMM
0x02000000 ; Value 2
```

### DIV (0x63)
Divide the first value by the second.

#### Assembly Syntax
```
DIV dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
DIV dest, src[, TYPE_PARAM5=condition]  ; Equivalent to DIV dest, dest, src
```

#### Binary Encoding
```
0x63                  ; Opcode for DIV
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
DIV result, a, b

; Binary
0x63      ; DIV
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"
```

### INC (0x65)
Increment a value by 1.

#### Assembly Syntax
```
INC dest[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x65                  ; Opcode for INC
0x01/0x02             ; One or two operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
INC counter

; Binary
0x65      ; INC
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
```

### DEC (0x66)
Decrement a value by 1.

#### Assembly Syntax
```
DEC dest[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x66                  ; Opcode for DEC
0x01/0x02             ; One or two operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
DEC counter

; Binary
0x66      ; DEC
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
```

### SQRT (0x69)
Compute the square root.

#### Assembly Syntax
```
SQRT dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x69                  ; Opcode for SQRT
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
SQRT result, value

; Binary
0x69      ; SQRT
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
```

### FMA (0x6A)
Fused multiply-add: (a * b) + c with a single rounding step.

#### Assembly Syntax
```
FMA dest, a, b, c[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x6A                  ; Opcode for FMA
0x04/0x05             ; Four or five operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[a_type]              ; Type of first multiplicand
[a_value]             ; First multiplicand value
[b_type]              ; Type of second multiplicand
[b_value]             ; Second multiplicand value
[c_type]              ; Type of addend
[c_value]             ; Addend value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
FMA result, a, b, c

; Binary
0x6A      ; FMA
0x04      ; Four operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "c"
```

### MIN (0x7B)
Find the minimum of two values.

#### Assembly Syntax
```
MIN dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MIN dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MIN dest, dest, src
```

#### Binary Encoding
```
0x7B                  ; Opcode for MIN
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
MIN result, a, b

; Binary
0x7B      ; MIN
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"

; Assembly (two-operand form)
MIN value, 100      ; Clamp upper limit to 100

; Binary
0x7B      ; MIN
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1320    ; TYPE_UNT32+IMM
0x64000000 ; Value 100
```

### MAX (0x7C)
Find the maximum of two values.

#### Assembly Syntax
```
MAX dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
MAX dest, src[, TYPE_PARAM5=condition]  ; Equivalent to MAX dest, dest, src
```

#### Binary Encoding
```
0x7C                  ; Opcode for MAX
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
MAX result, a, b

; Binary
0x7C      ; MAX
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"

; Assembly (two-operand form)
MAX value, 0         ; Ensure non-negative value

; Binary
0x7C      ; MAX
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1320    ; TYPE_UNT32+IMM
0x00000000 ; Value 0
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

Binary encoding (key parts):
```
0x65      ; INC
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "i"
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

Binary encoding (key parts):
```
0x61      ; SUB
0x03      ; Three operands
0x9000    ; TYPE_VAR (diff)
[id]      ; Variable ID
0x9000    ; TYPE_VAR (a)
[id]      ; Variable ID
0x9000    ; TYPE_VAR (b)
[id]      ; Variable ID

0x68      ; ABS
0x01      ; One operand
0x9000    ; TYPE_VAR (diff)
[id]      ; Variable ID
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