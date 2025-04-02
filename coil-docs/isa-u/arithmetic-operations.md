# Arithmetic Operations

## Overview

Arithmetic operations perform mathematical calculations on data values. COIL provides a comprehensive set of arithmetic instructions that work across all supported data types, with behavior determined by the operand types.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x40   | ADD      | Addition |
| 0x41   | SUB      | Subtraction |
| 0x42   | MUL      | Multiplication |
| 0x43   | DIV      | Division |
| 0x44   | MOD      | Modulo/Remainder |
| 0x45   | INC      | Increment |
| 0x46   | DEC      | Decrement |
| 0x47   | NEG      | Negate value |
| 0x48   | ABS      | Absolute value |
| 0x49   | SQRT     | Square root |
| 0x4A   | CEIL     | Ceiling function |
| 0x4B   | FLOR     | Floor function |
| 0x4C   | ROND     | Round to nearest |

## Detailed Specifications

### ADD (0x40)

Adds two values.

**Encoding:**
```
// Three-operand form (dest = src1 + src2)
[0x40][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest += src)
[0x40][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- For integral types: Standard binary addition with overflow according to type
- For floating-point types: IEEE-754 addition
- For vector types: Element-wise addition
- For matrix types: Element-wise addition
- Sets flags based on result (zero, sign, overflow, etc.)
- If SATURATE flag is set on the type, saturates instead of wrapping

### SUB (0x41)

Subtracts one value from another.

**Encoding:**
```
// Three-operand form (dest = src1 - src2)
[0x41][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest -= src)
[0x41][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- For integral types: Standard binary subtraction with underflow according to type
- For floating-point types: IEEE-754 subtraction
- For vector types: Element-wise subtraction
- For matrix types: Element-wise subtraction
- Sets flags based on result (zero, sign, overflow, etc.)
- If SATURATE flag is set on the type, saturates instead of wrapping

### MUL (0x42)

Multiplies two values.

**Encoding:**
```
// Three-operand form (dest = src1 * src2)
[0x42][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest *= src)
[0x42][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- For integral types: Standard binary multiplication with overflow according to type
- For floating-point types: IEEE-754 multiplication
- For vector types: Element-wise multiplication
- For matrix types with scalar: Scalar multiplication
- For matrix types with matrix: Matrix multiplication
- Sets flags based on result (zero, sign, overflow, etc.)
- If SATURATE flag is set on the type, saturates instead of wrapping

### DIV (0x43)

Divides one value by another.

**Encoding:**
```
// Three-operand form (dest = src1 / src2)
[0x43][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest /= src)
[0x43][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- For integral types: Integer division (truncated toward zero)
- For floating-point types: IEEE-754 division
- For vector types: Element-wise division
- For matrix types with scalar: Scalar division
- Division by zero raises an exception or sets flags based on implementation
- Sets flags based on result (zero, sign, etc.)

### MOD (0x44)

Calculates the remainder of division.

**Encoding:**
```
// Three-operand form (dest = src1 % src2)
[0x44][0x03][dest: Operand][src1: Operand][src2: Operand]

// Two-operand form (dest %= src)
[0x44][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- For integral types: Remainder after division
- For floating-point types: Floating-point remainder (as in fmod)
- For vector types: Element-wise modulo
- Division by zero raises an exception or sets flags based on implementation
- Sets flags based on result (zero, sign, etc.)

### INC (0x45)

Increments a value by 1.

**Encoding:**
```
[0x45][0x01][dest: Operand]
```

**Behavior:**
- Adds 1 to the operand in-place
- For integral types: Standard binary addition with overflow according to type
- For floating-point types: Adds 1.0
- For vector types: Increments each element by 1
- Sets flags based on result (zero, sign, overflow, etc.)
- If SATURATE flag is set on the type, saturates instead of wrapping

### DEC (0x46)

Decrements a value by 1.

**Encoding:**
```
[0x46][0x01][dest: Operand]
```

**Behavior:**
- Subtracts 1 from the operand in-place
- For integral types: Standard binary subtraction with underflow according to type
- For floating-point types: Subtracts 1.0
- For vector types: Decrements each element by 1
- Sets flags based on result (zero, sign, overflow, etc.)
- If SATURATE flag is set on the type, saturates instead of wrapping

### NEG (0x47)

Negates a value.

**Encoding:**
```
// Two-operand form (dest = -src)
[0x47][0x02][dest: Operand][src: Operand]

// One-operand form (dest = -dest)
[0x47][0x01][dest: Operand]
```

**Behavior:**
- Reverses the sign of the operand
- For integral types: Two's complement negation
- For floating-point types: IEEE-754 sign negation
- For vector types: Negates each element
- For matrix types: Negates each element
- Sets flags based on result (zero, sign, overflow, etc.)

### ABS (0x48)

Calculates the absolute value.

**Encoding:**
```
// Two-operand form (dest = |src|)
[0x48][0x02][dest: Operand][src: Operand]

// One-operand form (dest = |dest|)
[0x48][0x01][dest: Operand]
```

**Behavior:**
- Returns the absolute value of the operand
- For integral types: Distance from zero
- For floating-point types: IEEE-754 absolute value
- For vector types: Absolute value of each element
- For matrix types: Absolute value of each element
- Sets flags based on result (only zero flag)

### SQRT (0x49)

Calculates the square root.

**Encoding:**
```
// Two-operand form (dest = √src)
[0x49][0x02][dest: Operand][src: Operand]

// One-operand form (dest = √dest)
[0x49][0x01][dest: Operand]
```

**Behavior:**
- Calculates the square root of the operand
- For integral types: Truncated square root
- For floating-point types: IEEE-754 square root
- For vector types: Square root of each element
- For matrix types: Not defined (raises exception)
- Sets flags based on result (zero flag)
- Negative input raises an exception or produces a defined result based on type

### CEIL (0x4A)

Rounds up to the nearest integer.

**Encoding:**
```
// Two-operand form (dest = ⌈src⌉)
[0x4A][0x02][dest: Operand][src: Operand]

// One-operand form (dest = ⌈dest⌉)
[0x4A][0x01][dest: Operand]
```

**Behavior:**
- Rounds the operand up to the nearest integer
- For integral types: No effect
- For floating-point types: IEEE-754 ceiling function
- For vector types: Ceiling of each element
- For matrix types: Ceiling of each element
- Result type may differ from input type depending on implementation

### FLOR (0x4B)

Rounds down to the nearest integer.

**Encoding:**
```
// Two-operand form (dest = ⌊src⌋)
[0x4B][0x02][dest: Operand][src: Operand]

// One-operand form (dest = ⌊dest⌋)
[0x4B][0x01][dest: Operand]
```

**Behavior:**
- Rounds the operand down to the nearest integer
- For integral types: No effect
- For floating-point types: IEEE-754 floor function
- For vector types: Floor of each element
- For matrix types: Floor of each element
- Result type may differ from input type depending on implementation

### ROND (0x4C)

Rounds to the nearest integer.

**Encoding:**
```
// Two-operand form (dest = round(src))
[0x4C][0x02][dest: Operand][src: Operand]

// One-operand form (dest = round(dest))
[0x4C][0x01][dest: Operand]
```

**Behavior:**
- Rounds the operand to the nearest integer
- Ties round to even (banker's rounding)
- For integral types: No effect
- For floating-point types: IEEE-754 round to nearest
- For vector types: Round each element
- For matrix types: Round each element
- Result type may differ from input type depending on implementation

## Type-Specific Behavior

Arithmetic operations adapt their behavior based on operand types:

| Type Category | Behavior |
|---------------|----------|
| Integer | Standard binary arithmetic with wrapping or saturation |
| Float | IEEE-754 arithmetic operations |
| Vector | Element-wise operations |
| Matrix | Element-wise or algebraic operations depending on the instruction |
| Tensor | Element-wise or algebraic operations depending on the instruction |

## Related Components

- [Vector Operations](./vector-operations.md) - Vector processing instructions
- [Bit Manipulation](./bit-manipulation.md) - Bitwise operation instructions
- [Type System](../types/type-system.md) - Complete type system reference