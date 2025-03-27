# Arithmetic Examples in CASM

## Basic Integer Arithmetic

This example demonstrates various arithmetic operations with integer values.

```
; Basic integer arithmetic example
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM integer_arithmetic
    SCOPEE
    ; Declare variables
    VAR TYPE_INT32, a, 10
    VAR TYPE_INT32, b, 20
    VAR TYPE_INT32, result
    
    ; Addition
    ADD result, a, b          ; result = a + b = 30
    
    ; Subtraction
    SUB result, a, b          ; result = a - b = -10
    
    ; Multiplication
    MUL result, a, b          ; result = a * b = 200
    
    ; Division
    DIV result, a, b          ; result = a / b = 0
    DIV result, b, a          ; result = b / a = 2
    
    ; Modulo (remainder)
    MOD result, b, a          ; result = b % a = 0
    MOD result, a, 3          ; result = a % 3 = 1
    
    ; Increment and decrement
    INC a                     ; a += 1 (a is now 11)
    DEC b                     ; b -= 1 (b is now 19)
    
    ; Compound operations
    VAR TYPE_INT32, c, 5
    
    ; Combined operations
    ADD result, a, b          ; result = a + b = 30
    MUL result, result, c     ; result = result * c = 150
    
    ; Chain of operations
    MOV result, 0
    ADD result, a, b          ; result = a + b = 30
    DIV result, result, c     ; result = result / c = 6
    MOD result, result, 5     ; result = result % 5 = 1
    
    SCOPEL
    RET
```

## Floating-Point Arithmetic

This example shows arithmetic operations with floating-point values.

```
; Floating-point arithmetic example
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM float_arithmetic
    SCOPEE
    ; Declare floating-point variables
    VAR TYPE_FP32, x, 3.5
    VAR TYPE_FP32, y, 2.0
    VAR TYPE_FP32, result
    
    ; Basic arithmetic
    ADD result, x, y          ; result = x + y = 5.5
    SUB result, x, y          ; result = x - y = 1.5
    MUL result, x, y          ; result = x * y = 7.0
    DIV result, x, y          ; result = x / y = 1.75
    
    ; Advanced floating-point operations
    VAR TYPE_FP32, radius, 5.0
    VAR TYPE_FP32, pi, 3.14159
    VAR TYPE_FP32, area
    
    ; Calculate circle area: area = π * r²
    VAR TYPE_FP32, radius_squared
    MUL radius_squared, radius, radius  ; r² = 25.0
    MUL area, pi, radius_squared        ; area = π * r² = 78.53975
    
    ; Square root
    VAR TYPE_FP32, square_root
    SQRT square_root, area              ; square_root = √area ≈ 8.86
    
    ; Ceiling, floor, and round
    VAR TYPE_FP32, value, 3.7
    VAR TYPE_FP32, ceil_value
    VAR TYPE_FP32, floor_value
    VAR TYPE_FP32, round_value
    
    CEIL ceil_value, value     ; ceil_value = 4.0
    FLOOR floor_value, value   ; floor_value = 3.0
    ROUND round_value, value   ; round_value = 4.0
    
    SCOPEL
    RET
```

## Mixed-Type Arithmetic with Conversion

This example demonstrates how to perform arithmetic with different types.

```
; Mixed-type arithmetic example
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM mixed_arithmetic
    SCOPEE
    ; Declare variables of different types
    VAR TYPE_INT32, int_value, 42
    VAR TYPE_FP32, float_value, 3.14
    VAR TYPE_INT8, byte_value, 10
    
    ; Temporary variables for results
    VAR TYPE_INT32, int_result
    VAR TYPE_FP32, float_result
    
    ; Type conversion required for mixed operations
    VAR TYPE_FP32, converted_int
    CONVERT converted_int, int_value    ; converted_int = 42.0
    
    ; Now we can add the float and converted int
    ADD float_result, converted_int, float_value
    ; float_result = 42.0 + 3.14 = 45.14
    
    ; Widen the byte to int32 for addition
    VAR TYPE_INT32, widened_byte
    CONVERT widened_byte, byte_value    ; widened_byte = 10
    
    ADD int_result, int_value, widened_byte
    ; int_result = 42 + 10 = 52
    
    ; Convert float to int (truncates)
    VAR TYPE_INT32, truncated_float
    CONVERT truncated_float, float_value
    ; truncated_float = 3
    
    SCOPEL
    RET
```

## Vector Arithmetic

This example demonstrates arithmetic operations with vectors.

```
; Vector arithmetic example
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM vector_arithmetic
    SCOPEE
    ; Declare vector variables (4 x float32)
    VAR TYPE_V128=TYPE_FP32, vec1, (1.0, 2.0, 3.0, 4.0)
    VAR TYPE_V128=TYPE_FP32, vec2, (5.0, 6.0, 7.0, 8.0)
    VAR TYPE_V128=TYPE_FP32, result
    
    ; Vector addition (element-wise)
    ADD result, vec1, vec2
    ; result = (6.0, 8.0, 10.0, 12.0)
    
    ; Vector subtraction (element-wise)
    SUB result, vec2, vec1
    ; result = (4.0, 4.0, 4.0, 4.0)
    
    ; Vector multiplication (element-wise)
    MUL result, vec1, vec2
    ; result = (5.0, 12.0, 21.0, 32.0)
    
    ; Vector division (element-wise)
    DIV result, vec2, vec1
    ; result = (5.0, 3.0, 2.33, 2.0)
    
    ; Vector-scalar operations
    VAR TYPE_FP32, scalar, 2.0
    MUL result, vec1, scalar
    ; result = (2.0, 4.0, 6.0, 8.0)
    
    ; Vector dot product
    VAR TYPE_FP32, dot_product
    VDOT dot_product, vec1, vec2
    ; dot_product = 1.0*5.0 + 2.0*6.0 + 3.0*7.0 + 4.0*8.0 = 70.0
    
    SCOPEL
    RET
```

## Complex Calculation Example

This example demonstrates a more complex calculation (computing the distance between two 3D points).

```
; Distance calculation between two 3D points
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM distance_calculation
    SCOPEE
    ; Define two 3D points
    VAR TYPE_FP32, x1, 1.0
    VAR TYPE_FP32, y1, 2.0
    VAR TYPE_FP32, z1, 3.0
    
    VAR TYPE_FP32, x2, 4.0
    VAR TYPE_FP32, y2, 6.0
    VAR TYPE_FP32, z2, 8.0
    
    ; Calculate differences
    VAR TYPE_FP32, dx, dy, dz
    SUB dx, x2, x1    ; dx = x2 - x1 = 3.0
    SUB dy, y2, y1    ; dy = y2 - y1 = 4.0
    SUB dz, z2, z1    ; dz = z2 - z1 = 5.0
    
    ; Square the differences
    VAR TYPE_FP32, dx_squared, dy_squared, dz_squared
    MUL dx_squared, dx, dx    ; dx² = 9.0
    MUL dy_squared, dy, dy    ; dy² = 16.0
    MUL dz_squared, dz, dz    ; dz² = 25.0
    
    ; Sum the squares
    VAR TYPE_FP32, sum_of_squares
    ADD sum_of_squares, dx_squared, dy_squared
    ADD sum_of_squares, sum_of_squares, dz_squared
    ; sum_of_squares = 9.0 + 16.0 + 25.0 = 50.0
    
    ; Take the square root to get the distance
    VAR TYPE_FP32, distance
    SQRT distance, sum_of_squares
    ; distance = √50 ≈ 7.071
    
    SCOPEL
    RET
```

## Using the Vector Alternative

The same distance calculation can be done more efficiently using vectors:

```
; Distance calculation using vectors
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .text, 0x01 | 0x04
SYM vector_distance
    SCOPEE
    ; Define two 3D points as vectors (using only the first 3 components)
    VAR TYPE_V128=TYPE_FP32, point1, (1.0, 2.0, 3.0, 0.0)
    VAR TYPE_V128=TYPE_FP32, point2, (4.0, 6.0, 8.0, 0.0)
    
    ; Calculate the difference vector
    VAR TYPE_V128=TYPE_FP32, diff
    SUB diff, point2, point1
    ; diff = (3.0, 4.0, 5.0, 0.0)
    
    ; Calculate the dot product of diff with itself (sum of squares)
    VAR TYPE_FP32, sum_of_squares
    VDOT sum_of_squares, diff, diff
    ; sum_of_squares = 3.0² + 4.0² + 5.0² = 50.0
    
    ; Take the square root
    VAR TYPE_FP32, distance
    SQRT distance, sum_of_squares
    ; distance = √50 ≈ 7.071
    
    SCOPEL
    RET
```