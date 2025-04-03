# Type-Determined Instruction Behavior

## Overview

A core principle of COIL is that instructions derive their behavior from operand types. This document provides concrete examples and guidelines for how instructions adapt to different operand types, ensuring consistent and predictable behavior across implementations.

## Core Concept

In COIL, a single instruction opcode can perform different operations depending on the operand types. This approach provides several benefits:

1. **Compact Instruction Set**: Fewer opcodes needed to cover all operations
2. **Intuitive Programming**: Operations follow natural expectations for each type
3. **Consistent Patterns**: Similar operations work similarly across different types
4. **Extensibility**: New types integrate naturally into the existing instruction set

## Behavior Rules

When determining instruction behavior, implementations must follow these rules:

1. **Type Priority**: When operands have different types, the highest-priority type determines behavior
2. **Implicit Conversion**: Lower-priority types are converted to higher-priority types when needed
3. **Size Preservation**: Operations preserve the bit width of operands unless explicitly changed
4. **Domain Consistency**: Operations maintain the mathematical properties of their domains
5. **Explicit Control**: Type extension flags modify default behavior

## Type Priority Hierarchy

From highest to lowest priority:

1. Tensor types
2. Matrix types
3. Vector types
4. Floating-point types
5. Integer types
6. Boolean/bit types

## Type Combination Matrix

The following matrix defines the expected behavior for different type combinations across major instructions. For each cell, the result type and behavior are specified.

### ADD Instruction (0x40)

| Operand 1 | Operand 2 | Result Type | Behavior | Error Conditions |
|-----------|-----------|-------------|----------|------------------|
| INT8      | INT8      | INT8        | Integer addition with potential overflow | None |
| INT16     | INT8      | INT16       | INT8 promoted to INT16, then addition | None |
| INT32     | INT16     | INT32       | INT16 promoted to INT32, then addition | None |
| INT64     | INT32     | INT64       | INT32 promoted to INT64, then addition | None |
| FP32      | INT32     | FP32        | INT32 converted to FP32, then addition | None |
| FP64      | FP32      | FP64        | FP32 promoted to FP64, then addition | None |
| V128(INT32) | INT32   | V128(INT32) | Scalar added to each vector element | None |
| V128(INT32) | V128(INT32) | V128(INT32) | Element-wise addition | Vector dimensions must match |
| V128(FP32) | V128(INT32) | V128(FP32) | INT32 elements converted to FP32, then element-wise addition | Vector dimensions must match |
| MAT(INT32) | INT32   | MAT(INT32)  | Scalar added to each matrix element | None |
| MAT(FP32) | MAT(FP32) | MAT(FP32)  | Element-wise matrix addition | Matrix dimensions must match |
| MAT(INT32) | MAT(FP32) | MAT(FP32)  | INT32 elements converted to FP32, then element-wise addition | Matrix dimensions must match |
| TENSOR    | TENSOR   | TENSOR      | Element-wise tensor addition | Tensor dimensions must match |

### SUB Instruction (0x41)

| Operand 1 | Operand 2 | Result Type | Behavior | Error Conditions |
|-----------|-----------|-------------|----------|------------------|
| INT32     | INT32     | INT32       | Integer subtraction with potential underflow | None |
| FP32      | INT32     | FP32        | INT32 converted to FP32, then subtraction | None |
| V128(INT32) | INT32   | V128(INT32) | Scalar subtracted from each vector element | None |
| V128(INT32) | V128(INT32) | V128(INT32) | Element-wise subtraction | Vector dimensions must match |
| MAT(FP32) | MAT(FP32) | MAT(FP32)  | Element-wise matrix subtraction | Matrix dimensions must match |

### MUL Instruction (0x42)

| Operand 1 | Operand 2 | Result Type | Behavior | Error Conditions |
|-----------|-----------|-------------|----------|------------------|
| INT32     | INT32     | INT32       | Integer multiplication with potential overflow | None |
| FP32      | INT32     | FP32        | INT32 converted to FP32, then multiplication | None |
| V128(INT32) | INT32   | V128(INT32) | Scalar multiplies each vector element | None |
| V128(INT32) | V128(INT32) | V128(INT32) | Element-wise multiplication (not dot product) | Vector dimensions must match |
| MAT(FP32) | MAT(FP32) | MAT(FP32)  | Matrix multiplication | Matrix dimensions must be compatible (M×N * N×P = M×P) |
| MAT(FP32) | V128(FP32) | V128(FP32) | Matrix-vector multiplication | Vector length must match matrix columns |
| MAT(FP32) | FP32      | MAT(FP32)  | Scalar multiplies each matrix element | None |

### DIV Instruction (0x43)

| Operand 1 | Operand 2 | Result Type | Behavior | Error Conditions |
|-----------|-----------|-------------|----------|------------------|
| INT32     | INT32     | INT32       | Integer division with truncation toward zero | Division by zero |
| FP32      | INT32     | FP32        | INT32 converted to FP32, then division | Division by zero |
| V128(INT32) | INT32   | V128(INT32) | Each vector element divided by scalar | Division by zero |
| V128(INT32) | V128(INT32) | V128(INT32) | Element-wise division | Vector dimensions must match, division by zero |
| MAT(FP32) | FP32      | MAT(FP32)  | Each matrix element divided by scalar | Division by zero |
| MAT(FP32) | MAT(FP32) | MAT(FP32)  | Matrix multiplication by inverse of second matrix | Second matrix must be invertible |

### Bitwise Operations (AND, OR, XOR)

| Operand 1 | Operand 2 | Result Type | Behavior | Error Conditions |
|-----------|-----------|-------------|----------|------------------|
| INT32     | INT32     | INT32       | Bitwise operation on integers | None |
| BIT       | BIT       | BIT         | Logical operation on bits | None |
| V128(INT32) | INT32   | V128(INT32) | Scalar bitwise operation with each element | None |
| V128(INT32) | V128(INT32) | V128(INT32) | Element-wise bitwise operation | Vector dimensions must match |
| FP32      | INT32     | Error       | Invalid operation on FP | Type mismatch error |
| FP32      | FP32      | Error       | Invalid operation on FP | Type mismatch error |

### Memory Operations (MOV, PUSH, POP)

| Operand Types | Behavior | Error Conditions |
|---------------|----------|------------------|
| dest:INT32, src:INT32 | Copy 32-bit integer | None |
| dest:FP32, src:INT32 | Convert INT32 to FP32, then copy | Potential precision loss |
| dest:INT16, src:INT32 | Truncate INT32 to INT16, then copy | Potential value loss |
| dest:V128, src:V128 | Copy entire vector | Vector types must be compatible |
| dest:MAT, src:MAT | Copy entire matrix | Matrix dimensions must match |
| dest:STRUCT, src:STRUCT | Copy entire structure | Structure definitions must match |

### Comparison Operations (CMP, TEST)

| Operand 1 | Operand 2 | Flag Effects | Behavior |
|-----------|-----------|--------------|----------|
| INT32     | INT32     | Z,S,C,V      | Compare values and set flags |
| FP32      | FP32      | Z,S,N        | Compare values according to IEEE 754 |
| FP32      | INT32     | Z,S,N        | Convert INT32 to FP32, then compare |
| V128      | V128      | Z only       | Element-wise comparison, Z set if all elements equal |
| MAT       | MAT       | Z only       | Element-wise comparison, Z set if all elements equal |

## Mixed Type Operation Examples

### Example 1: Adding Integer to Floating-Point

```
// ADD result, a, b  where a is FP32 and b is INT32
[0x40][0x03]
  [0x10][0x40][var_id_result]  // result: FP32 variable
  [0x10][0x40][var_id_a]       // a: FP32 variable
  [0x04][0x40][var_id_b]       // b: INT32 variable
```

Behavior:
1. Operand b (INT32) is converted to FP32
2. Floating-point addition is performed
3. FP32 result is stored in the result variable

### Example 2: Matrix Multiplication with Vector

```
// MUL result, mat, vec  where mat is 3×4 MAT(FP32) and vec is V128(FP32) with 4 elements
[0x42][0x03]
  [0x33][0x40][0x10][0x00][0x0003][var_id_result]  // result: vector of 3 FP32 elements
  [0x34][0x40][0x10][0x00][0x0003][0x0004][var_id_mat]  // mat: 3×4 matrix of FP32
  [0x33][0x40][0x10][0x00][0x0004][var_id_vec]      // vec: vector of 4 FP32 elements
```

Behavior:
1. Mathematical matrix-vector multiplication is performed
2. Result is a vector with length matching matrix rows (3)
3. Each element in result is the dot product of a matrix row with the vector

### Example 3: Bitwise Operation with Mixed Integer Types

```
// AND result, a, b  where a is INT32 and b is INT8
[0x60][0x03]
  [0x04][0x40][var_id_result]  // result: INT32 variable
  [0x04][0x40][var_id_a]       // a: INT32 variable
  [0x00][0x40][var_id_b]       // b: INT8 variable
```

Behavior:
1. Operand b (INT8) is sign-extended to INT32
2. Bitwise AND is performed
3. INT32 result is stored in the result variable

## Error Handling for Invalid Type Combinations

When invalid type combinations are encountered, implementations must:

1. **Compilation Time**: If detected at compile time, generate an error and abort compilation
2. **Runtime**: If only detectable at runtime, either:
   - Throw a type mismatch exception
   - Set a special error flag
   - Produce a well-defined error value

### Common Error Conditions

| Operation | Invalid Type Combination | Required Error Handling |
|-----------|--------------------------|-------------------------|
| Arithmetic | Bitwise types in floating-point operation | Type mismatch error |
| Vector | Operations on vectors with mismatched dimensions | Dimension mismatch error |
| Matrix | Matrix multiplication with incompatible dimensions | Dimension mismatch error |
| Division | Division by zero | Division by zero exception or special flag |
| Conversion | Conversion that would lose required precision | Precision loss warning (compile time) |

## Handling Special Values

### NaN Handling

For floating-point operations that produce NaN:

1. The NaN flag (N) must be set
2. The result must be a quiet NaN
3. Propagation follows IEEE-754 rules (NaN input → NaN output)

### Infinity Handling

For floating-point operations with infinity:

1. Operations follow IEEE-754 rules
2. Infinity signs are preserved according to IEEE-754
3. Infinity combined with finite values follows IEEE-754 rules

### Division by Zero

For division operations with zero divisor:

1. Integer division: Sets the divide-by-zero flag (D) and either:
   - Raises an exception, or
   - Returns a well-defined error value (typically 0)

2. Floating-point division: 
   - Produces IEEE-754 infinity with appropriate sign
   - Sets the divide-by-zero flag (D)

### Overflow and Underflow

For operations that exceed representable range:

1. Integer operations:
   - Without SATURATE flag: Wrap around and set overflow flag (V)
   - With SATURATE flag: Clamp to max/min value and set overflow flag (V)

2. Floating-point operations:
   - Follow IEEE-754 overflow/underflow behavior
   - Set appropriate flags (V for overflow, Z for underflow to zero)

## Implementation Requirements

Conforming implementations must:

1. **Respect Type Semantics**: Operations must preserve the mathematical properties of each type
2. **Handle Edge Cases**: Properly manage overflow, underflow, NaN, infinity, etc.
3. **Document Precision**: Clearly specify precision guarantees for floating-point operations
4. **Maintain Consistency**: Ensure consistent behavior across all supported platforms
5. **Check Dimensions**: Validate vector and matrix dimensions for compatibility
6. **Report Errors**: Provide clear error messages for type incompatibilities

## Related Components

- [Type System](./type-system.md) - Complete type system reference
- [Flag Effects](../core/flag-effects.md) - Detailed flag behavior documentation
- [Vector Operations](../isa-u/vector-operations.md) - Vector-specific operations