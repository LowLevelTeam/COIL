# Arithmetic Instructions (0x60-0x8F)

## Purpose

This document defines the arithmetic instructions in COIL, which perform mathematical operations on numeric data. These instructions form a fundamental part of COIL's computational capabilities across all processor types.

## Basic Arithmetic Operations

### ADD (0x60)
Add two values and store the result.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

The ADD instruction can be used with two or three operands:
- Three operands: `ADD dst, src1, src2` (dst = src1 + src2)
- Two operands: `ADD dst, src` (dst = dst + src)

Example:
```
; Three-operand form
VAR TYPE_INT32, result
ADD result, a, b

; Two-operand form (implied first operand)
ADD counter, 1

; ADD with different numeric types
ADD TYPE_FP32, float_result, float_a, float_b

; ADD with condition
ADD_NZ result, a, b
```

Binary encoding:
```
0x60      ; Opcode for ADD
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### SUB (0x61)
Subtract the second value from the first and store the result.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
SUB result, a, b    ; result = a - b

; Two-operand form
SUB counter, 1      ; counter = counter - 1

; SUB with condition
SUB_GT value, 10
```

Binary encoding:
```
0x61      ; Opcode for SUB
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### MUL (0x62)
Multiply two values and store the result.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
MUL result, a, b    ; result = a * b

; Two-operand form
MUL value, 2        ; value = value * 2

; Integer multiplication
MUL TYPE_INT32, int_result, int_a, int_b

; Floating-point multiplication
MUL TYPE_FP64, fp_result, fp_a, fp_b
```

Binary encoding:
```
0x62      ; Opcode for MUL
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### DIV (0x63)
Divide the first value by the second and store the result.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
DIV result, a, b    ; result = a / b

; Two-operand form
DIV value, 2        ; value = value / 2

; Integer division
DIV TYPE_INT32, int_result, int_a, int_b

; Floating-point division
DIV TYPE_FP64, fp_result, fp_a, fp_b
```

Binary encoding:
```
0x63      ; Opcode for DIV
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### MOD (0x64)
Compute the remainder of dividing the first value by the second.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
MOD result, a, b    ; result = a % b

; Two-operand form
MOD value, 10       ; value = value % 10
```

Binary encoding:
```
0x64      ; Opcode for MOD
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### INC (0x65)
Increment a value by 1.

```
Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Increment a variable
INC counter         ; counter = counter + 1

; Conditional increment
INC_NZ counter      ; Increment if not zero
```

Binary encoding:
```
0x65      ; Opcode for INC
0x01/0x02 ; One or two operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; TYPE_PARAM5 (if conditional)
[value2]  ; Condition code (if conditional)
```

### DEC (0x66)
Decrement a value by 1.

```
Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Decrement a variable
DEC counter         ; counter = counter - 1

; Conditional decrement
DEC_GT counter      ; Decrement if greater than
```

Binary encoding:
```
0x66      ; Opcode for DEC
0x01/0x02 ; One or two operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; TYPE_PARAM5 (if conditional)
[value2]  ; Condition code (if conditional)
```

## Unary Operations

### NEG (0x67)
Negate a value (change sign).

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Two-operand form
NEG result, value   ; result = -value

; One-operand form
NEG value           ; value = -value
```

Binary encoding:
```
0x67      ; Opcode for NEG
0x01/0x02/0x03 ; One, two, or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source (if two-operand form)
[value2]  ; Source value (if two-operand form)
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### ABS (0x68)
Compute the absolute value.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Two-operand form
ABS result, value   ; result = |value|

; One-operand form
ABS value           ; value = |value|
```

Binary encoding:
```
0x68      ; Opcode for ABS
0x01/0x02/0x03 ; One, two, or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source (if two-operand form)
[value2]  ; Source value (if two-operand form)
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### SQRT (0x69)
Compute the square root.

```
Operands:
- Destination: Non TYPE_VOID (floating-point type)
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Compute square root
SQRT result, value  ; result = √value

; With explicit types
SQRT TYPE_FP64, double_result, double_value
```

Binary encoding:
```
0x69      ; Opcode for SQRT
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Advanced Arithmetic

### FMA (0x6A)
Fused multiply-add: (a * b) + c with a single rounding step.

```
Operands:
- Destination: Non TYPE_VOID (floating-point type)
- A: Non TYPE_VOID (floating-point type)
- B: Non TYPE_VOID (floating-point type)
- C: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Compute a*b+c with a single rounding
FMA result, a, b, c    ; result = a*b + c

; With explicit type
FMA TYPE_FP32, float_result, float_a, float_b, float_c
```

Binary encoding:
```
0x6A      ; Opcode for FMA
0x04/0x05 ; Four or five operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of A
[value2]  ; A value
[type3]   ; Type of B
[value3]  ; B value
[type4]   ; Type of C
[value4]  ; C value
[type5]   ; TYPE_PARAM5 (if conditional)
[value5]  ; Condition code (if conditional)
```

### MIN (0x7B)
Compute the minimum of two values.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
MIN result, a, b    ; result = min(a, b)

; Two-operand form
MIN value, 100      ; value = min(value, 100)
```

Binary encoding:
```
0x7B      ; Opcode for MIN
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### MAX (0x7C)
Compute the maximum of two values.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
MAX result, a, b    ; result = max(a, b)

; Two-operand form
MAX value, 0        ; value = max(value, 0)
```

Binary encoding:
```
0x7C      ; Opcode for MAX
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Additional Arithmetic Instructions

### CEIL (0x6B)
Round up to the nearest integer.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Ceiling function
CEIL result, value  ; result = ⌈value⌉
```

### FLOOR (0x6C)
Round down to the nearest integer.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Floor function
FLOOR result, value  ; result = ⌊value⌋
```

### ROUND (0x6D)
Round to the nearest integer.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Round to nearest
ROUND result, value  ; result = round(value)
```

### TRUNC (0x6E)
Truncate to integer (round toward zero).

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Truncate to integer
TRUNC result, value  ; result = trunc(value)
```

## Type Behavior

Arithmetic instructions adapt their behavior based on operand types:

1. **Integer Operations**:
   - Based on two's complement arithmetic
   - Overflow behavior depends on signedness
   - Exact rules determined by the specific processor

2. **Floating-Point Operations**:
   - Based on IEEE 754 arithmetic
   - Default rounding mode is round-to-nearest
   - Special value handling (NaN, Infinity) follows IEEE 754

3. **Mixed-Type Operations**:
   - When operands have different types, COIL follows specific promotion rules
   - Generally, operands are promoted to the wider type
   - Explicit conversion may be required for some mixed-type operations

## Common Arithmetic Patterns

### Counter Loop

```
; Initialize counter
VAR TYPE_INT32, i, 0
VAR TYPE_INT32, limit, 10

; Loop
SYM loop_start
CMP i, limit
BR_GE loop_end

; Loop body...

; Increment counter
INC i
BR loop_start
SYM loop_end
```

### Summation

```
; Initialize sum
VAR TYPE_INT32, sum, 0
VAR TYPE_ARRAY=TYPE_INT32, values, (1, 2, 3, 4, 5)
VAR TYPE_INT32, count, 5
VAR TYPE_INT32, i, 0

; Loop to sum values
SYM sum_loop
CMP i, count
BR_GE sum_done

; Get array element and add to sum
VAR TYPE_INT32, element
INDEX element, values, i
ADD sum, sum, element

; Next element
INC i
BR sum_loop
SYM sum_done
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

### Fast Multiplication by Powers of Two

```
; Multiply by power of 2 using SHL (left shift)
VAR TYPE_INT32, value, 5
VAR TYPE_INT32, result

; Multiply by 4 (2^2)
SHL result, value, 2  ; result = 5 * 4 = 20
```

### Efficient Division by Powers of Two

```
; Divide by power of 2 using SHR (right shift)
VAR TYPE_INT32, value, 20
VAR TYPE_INT32, result

; Divide by 4 (2^2)
SHR result, value, 2  ; result = 20 / 4 = 5
```

## Processor-Specific Considerations

While arithmetic instructions are universal across all processor types, implementation details may vary:

1. **Rounding Behavior**:
   - May depend on processor floating-point rounding mode
   - Can be explicitly controlled with additional parameters

2. **Overflow Handling**:
   - Integer overflow behavior may vary
   - Some processors may set status flags or trap

3. **FMA Implementation**:
   - Hardware FMA may not be available on all processors
   - COIL processor may implement it using separate multiply and add

4. **SIMD Acceleration**:
   - Arithmetic on vector types may be accelerated using SIMD hardware
   - Implementation strategy depends on available hardware capabilities

The COIL processor is responsible for mapping arithmetic instructions to the most efficient implementation for the target processor while maintaining consistent behavior across platforms.