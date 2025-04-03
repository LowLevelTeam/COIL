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

1. **Type Priority**: When operands have different types, the higher-priority type determines behavior
2. **Implicit Conversion**: Lower-priority types are converted to higher-priority types when needed
3. **Size Preservation**: Operations preserve the bit width of operands
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

## Examples by Instruction Category

### Arithmetic Operations

#### ADD Instruction (0x40)

| Operand Types | Behavior | Example | Result |
|---------------|----------|---------|--------|
| INT32 + INT32 | Integer addition | ADD r0(5), r1(10) | r0 = 15 |
| INT32 + FP32 | Floating-point addition | ADD r0(5), r1(10.5) | r0 = 15.5 |
| INT32 + CONST INT32 | Integer addition with immediate | ADD r0(5), 10 | r0 = 15 |
| V128 + V128 | Element-wise vector addition | ADD v0([1,2,3,4]), v1([5,6,7,8]) | v0 = [6,8,10,12] |
| MAT + MAT | Element-wise matrix addition | ADD m0([[1,2],[3,4]]), m1([[5,6],[7,8]]) | m0 = [[6,8],[10,12]] |
| MAT + INT32 | Scalar addition to all elements | ADD m0([[1,2],[3,4]]), 5 | m0 = [[6,7],[8,9]] |

#### MUL Instruction (0x42)

| Operand Types | Behavior | Example | Result |
|---------------|----------|---------|--------|
| INT32 * INT32 | Integer multiplication | MUL r0(5), r1(10) | r0 = 50 |
| FP32 * FP32 | Floating-point multiplication | MUL r0(5.5), r1(2.0) | r0 = 11.0 |
| V128 * V128 | Element-wise vector multiplication | MUL v0([1,2,3,4]), v1([5,6,7,8]) | v0 = [5,12,21,32] |
| MAT * MAT | Matrix multiplication | MUL m0([[1,2],[3,4]]), m1([[5,6],[7,8]]) | m0 = [[19,22],[43,50]] |
| MAT * INT32 | Scalar multiplication of all elements | MUL m0([[1,2],[3,4]]), 2 | m0 = [[2,4],[6,8]] |

### Bit Manipulation Operations

#### AND Instruction (0x60)

| Operand Types | Behavior | Example | Result |
|---------------|----------|---------|--------|
| INT32 & INT32 | Bitwise AND | AND r0(0x0F), r1(0xF0) | r0 = 0x00 |
| BIT & BIT | Boolean AND | AND b0(1), b1(0) | b0 = 0 |
| V128 & V128 | Element-wise bitwise AND | AND v0([0x0F,0xFF]), v1([0xF0,0x0F]) | v0 = [0x00,0x0F] |
| V128 & INT32 | AND scalar with all elements | AND v0([0x0F,0xFF]), 0x0F | v0 = [0x0F,0x0F] |

### Memory Operations

#### MOV Instruction (0x20)

| Operand Types | Behavior | Example | Result |
|---------------|----------|---------|--------|
| INT32 ← INT32 | Copy integer value | MOV r0, r1(42) | r0 = 42 |
| FP64 ← FP32 | Convert and copy float | MOV d0, f0(3.14) | d0 = 3.14 (as FP64) |
| V128 ← V128 | Copy vector | MOV v0, v1([1,2,3,4]) | v0 = [1,2,3,4] |
| MAT ← MAT | Copy matrix | MOV m0, m1([[1,2],[3,4]]) | m0 = [[1,2],[3,4]] |
| INT32 ← [INT32] | Load from memory | MOV r0, [0x1000] | r0 = *0x1000 |
| [INT32] ← INT32 | Store to memory | MOV [0x1000], r0(42) | *0x1000 = 42 |

### Vector Operations

#### DOT Instruction (0x82)

| Operand Types | Behavior | Example | Result |
|---------------|----------|---------|--------|
| V128 DOT V128 | Vector dot product | DOT r0, v0([1,2,3]), v1([4,5,6]) | r0 = 32 (1*4+2*5+3*6) |
| MAT DOT V128 | Matrix-vector product | DOT v0, m0([[1,2],[3,4]]), v1([5,6]) | v0 = [17,39] |
| V128 DOT MAT | Vector-matrix product | DOT v0, v1([1,2]), m0([[3,4],[5,6]]) | v0 = [13,16] |

## Type Extension Modifiers

Type extension flags modify instruction behavior:

### SATURATE Flag

When set, prevents overflow by clamping to type limits:

```
// Without SATURATE
ADD r0(INT32_MAX), 1  // Result: INT32_MIN (overflow)

// With SATURATE
ADD r0(INT32_MAX<SATURATE>), 1  // Result: INT32_MAX (clamped)
```

### CONST Flag

Prevents modification of values:

```
// Variable can be modified
MOV r0(10), 20  // r0 = 20

// Constant cannot be modified
MOV r0(10<CONST>), 20  // Error: Cannot modify constant
```

### ATOMIC Flag

Ensures operations are atomic even in multi-threaded environments:

```
// Regular addition (not thread-safe)
ADD [shared_counter], 1

// Atomic addition (thread-safe)
ADD [shared_counter<ATOMIC>], 1
```

## Type Conversion Rules

When operands have different types, conversion follows these rules:

1. **Widening Conversions** (smaller to larger type):
   - Preserve exact value
   - Example: INT8 to INT32, FP32 to FP64

2. **Narrowing Conversions** (larger to smaller type):
   - May lose precision or range
   - Example: INT64 to INT32, FP64 to FP32

3. **Domain Conversions** (between different type domains):
   - Follow standard mathematical rules
   - Example: INT32 to FP32, FP32 to INT32 (truncation)

4. **Composite Conversions** (between complex types):
   - Applied element-wise to components
   - Example: V128(INT32) to V128(FP32)

## Implementation Requirements

Conforming implementations must:

1. **Respect Type Semantics**: Operations must preserve the mathematical properties of each type
2. **Handle Edge Cases**: Properly manage overflow, underflow, NaN, infinity, etc.
3. **Document Precision**: Clearly specify precision guarantees for floating-point operations
4. **Maintain Consistency**: Ensure consistent behavior across all supported platforms

## Common Implementation Patterns

### 1. Generic Templates

Implementations often use templated code to handle different types:

```cpp
template <typename T>
void ExecuteAdd(T& dest, const T& src1, const T& src2) {
    dest = src1 + src2;
}

// Specializations for specific types
template <>
void ExecuteAdd<Matrix>(Matrix& dest, const Matrix& src1, const Matrix& src2) {
    // Matrix-specific addition implementation
}
```

### 2. Type Dispatch Tables

Function pointers selected based on operand types:

```cpp
typedef void (*AddFunction)(void* dest, void* src1, void* src2);

AddFunction addDispatchTable[TYPE_COUNT][TYPE_COUNT] = {
    // [dest_type][src_type] = implementation
    [INT32][INT32] = AddInt32Int32,
    [FP32][FP32] = AddFp32Fp32,
    [INT32][FP32] = AddInt32Fp32,
    // ...
};
```

### 3. Layered Implementation

Operations implemented in layers with common machinery:

1. Type checking and validation
2. Type conversion where needed
3. Core operation implementation
4. Result post-processing
5. Flag setting

## Standard Type Behavior Patterns

These patterns should be consistent across all instructions:

### 1. Scalar-Scalar Operations

Basic operations between scalar values:
- Follow standard mathematical rules
- Respect type domain semantics
- Set appropriate flags

### 2. Vector-Vector Operations

Operations between vectors:
- Applied element-wise
- Require matching dimensions
- Optimize for SIMD when possible

### 3. Matrix-Matrix Operations

Operations between matrices:
- Follow linear algebra rules
- Check dimension compatibility
- Optimize for spatial locality

### 4. Scalar-Vector/Matrix Operations

Operations between scalars and vectors/matrices:
- Scalar applied to all elements
- Preserve matrix/vector dimensions
- Optimize for parallel execution

## Case Study: ADD Across Type Categories

Detailed examination of how ADD behaves:

1. **Integer Addition**:
   - Standard binary addition with wrap-around on overflow
   - Sets Zero, Sign, Carry, Overflow flags

2. **Floating-Point Addition**:
   - IEEE-754 compliant addition
   - Handles special values (NaN, Infinity)
   - Sets Zero, Sign, NaN flags

3. **Vector Addition**:
   - Element-wise addition
   - Result has same dimensions as inputs
   - Sets flags based on element-wise results

4. **Matrix Addition**:
   - Element-wise addition of corresponding elements
   - Requires matching dimensions
   - Sets flags based on element-wise results

By following these guidelines, implementations can ensure consistent type-determined behavior across all COIL operations.