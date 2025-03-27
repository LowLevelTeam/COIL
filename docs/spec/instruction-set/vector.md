# Vector/Array Operations (0x90-0x9F)

## Purpose

This document defines the vector and array operations in COIL, which enable efficient parallel processing of multiple data elements. These instructions provide SIMD (Single Instruction Multiple Data) capabilities that are automatically mapped to the target architecture's vector processing features when available.

## Key Concepts

- **Vector Types**: Special types that contain multiple elements of the same base type
- **SIMD Operations**: Operations that process multiple data elements in parallel
- **Element Access**: How to extract or insert individual elements
- **Vector Manipulation**: How to reorganize vector elements
- **Vector Math**: How to perform mathematical operations on vectors

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x90   | VLOAD    | `VLOAD dest, src[, TYPE_PARAM0][, TYPE_PARAM5]` | Load vector |
| 0x91   | VSTORE   | `VSTORE dest, src[, TYPE_PARAM0][, TYPE_PARAM5]` | Store vector |
| 0x96   | VEXTRACT | `VEXTRACT dest, src, index[, TYPE_PARAM5]` | Extract element |
| 0x97   | VINSERT  | `VINSERT dest, src, value, index[, TYPE_PARAM5]` | Insert element |
| 0x98   | VSHUFFLE | `VSHUFFLE dest, src, mask[, TYPE_PARAM5]` | Shuffle elements |
| 0x99   | VCMP     | `VCMP dest, left, right, cmp_type[, TYPE_PARAM5]` | Compare vectors |
| 0x9A   | VBLEND   | `VBLEND dest, first, second, mask[, TYPE_PARAM5]` | Blend vectors |
| 0x9B   | VMASKMOV | `VMASKMOV dest, src, mask[, TYPE_PARAM5]` | Masked move |
| 0x9C   | VDOT     | `VDOT dest, left, right[, TYPE_PARAM5]` | Dot product |
| 0x9D   | VCROSS   | `VCROSS dest, left, right[, TYPE_PARAM5]` | Cross product |
| 0x9E   | VHSUM    | `VHSUM dest, src[, TYPE_PARAM5]` | Horizontal sum |

## Vector Types

COIL provides three primary vector types:

| Type | Size (Bytes) | Description |
|------|--------------|-------------|
| TYPE_V128 | 16 | 128-bit vector |
| TYPE_V256 | 32 | 256-bit vector |
| TYPE_V512 | 64 | 512-bit vector |

Vector types are parameterized by their element type:
- `TYPE_V128=TYPE_INT32`: Vector of 4x 32-bit integers
- `TYPE_V256=TYPE_FP32`: Vector of 8x 32-bit floats
- `TYPE_V512=TYPE_INT16`: Vector of 32x 16-bit integers

## Detailed Descriptions

### Data Movement Instructions

#### VLOAD (0x90)
Load data from memory into a vector.

##### Syntax
```
VLOAD dest, src[, TYPE_PARAM0=vector_control][, TYPE_PARAM5=condition]
```

##### Vector Control Parameters
- `TYPE_PARAM0=VECTOR_CTRL_ALIGNED`: Aligned memory access
- `TYPE_PARAM0=VECTOR_CTRL_UNALIGNED`: Unaligned memory access
- `TYPE_PARAM0=VECTOR_CTRL_BROADCAST`: Broadcast scalar to all elements

##### Example
```
; Load vector from aligned memory
VLOAD TYPE_V128=TYPE_FP32, vector, [array_ptr]

; Broadcast scalar to all vector elements
VLOAD TYPE_V256=TYPE_INT32, vector, [scalar_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST
```

#### VSTORE (0x91)
Store vector data to memory.

##### Syntax
```
VSTORE dest, src[, TYPE_PARAM0=vector_control][, TYPE_PARAM5=condition]
```

##### Example
```
; Store vector to memory
VSTORE [array_ptr], vector
```

### Element Access Instructions

#### VEXTRACT (0x96)
Extract a scalar element from a vector.

##### Syntax
```
VEXTRACT dest, src, index[, TYPE_PARAM5=condition]
```

##### Example
```
; Extract the second element (index 1)
VEXTRACT scalar_value, vector, 1
```

#### VINSERT (0x97)
Insert a scalar value into a vector at a specified position.

##### Syntax
```
VINSERT dest, src, value, index[, TYPE_PARAM5=condition]
```

##### Example
```
; Insert value at position 2
VINSERT modified_vector, original_vector, new_value, 2
```

### Vector Operation Instructions

#### VCMP (0x99)
Compare vectors element-wise and produce a mask.

##### Syntax
```
VCMP dest, left, right, cmp_type[, TYPE_PARAM5=condition]
```

##### Comparison Types
- `0`: Equal
- `1`: Not Equal
- `2`: Less Than
- `3`: Less Than or Equal
- `4`: Greater Than
- `5`: Greater Than or Equal

##### Example
```
; Compare for greater than
VCMP mask, vector1, vector2, 4  ; Elements where vector1 > vector2
```

#### VBLEND (0x9A)
Combine elements from two vectors based on a mask.

##### Syntax
```
VBLEND dest, first, second, mask[, TYPE_PARAM5=condition]
```

##### Example
```
; Select elements from first or second based on mask
VBLEND result, vector1, vector2, selection_mask
```

#### VDOT (0x9C)
Compute the dot product of two vectors.

##### Syntax
```
VDOT dest, left, right[, TYPE_PARAM5=condition]
```

##### Example
```
; Compute dot product
VDOT scalar_result, vector1, vector2
```

#### VHSUM (0x9E)
Compute the horizontal sum of all elements in a vector.

##### Syntax
```
VHSUM dest, src[, TYPE_PARAM5=condition]
```

##### Example
```
; Sum all elements
VHSUM total, vector
```

## Standard Arithmetic with Vectors

Vector operations work with standard arithmetic instructions, which automatically apply element-wise when used with vector types:

```
; Element-wise addition
ADD vector_sum, vector1, vector2

; Element-wise multiplication
MUL vector_product, vector1, vector2

; Scalar-vector multiplication
MUL scaled_vector, vector, scalar_value
```

## Common Vector Operation Patterns

### Vector Initialization

```
; Initialize vector from aligned memory
VAR TYPE_V128=TYPE_FP32, vector
VLOAD vector, [array_ptr]

; Initialize vector with broadcast value
VAR TYPE_V256=TYPE_INT32, all_ones
VLOAD all_ones, [one_value_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST

; Initialize vector with literal values
VAR TYPE_V128=TYPE_INT32, vector, (1, 2, 3, 4)
```

### Basic Vector Math

```
; Vector addition
VAR TYPE_V128=TYPE_FP32, result
ADD result, vector1, vector2

; Vector scaling (multiply by scalar)
VAR TYPE_V128=TYPE_FP32, scaled
MUL scaled, vector, 2.0

; Distance calculation between 3D points
VAR TYPE_V128=TYPE_FP32, point1, (x1, y1, z1, 0)
VAR TYPE_V128=TYPE_FP32, point2, (x2, y2, z2, 0)
VAR TYPE_V128=TYPE_FP32, diff
SUB diff, point2, point1
VAR TYPE_FP32, distance_squared
VDOT distance_squared, diff, diff
VAR TYPE_FP32, distance
SQRT distance, distance_squared
```

### Conditional Vector Processing

```
; Select values based on condition (vector > threshold)
VAR TYPE_V128=TYPE_FP32, vector, (1.0, 2.0, 3.0, 4.0)
VAR TYPE_V128=TYPE_FP32, threshold, (2.5, 2.5, 2.5, 2.5)
VAR TYPE_V128=TYPE_FP32, zeros, (0.0, 0.0, 0.0, 0.0)

; Compare vectors
VAR TYPE_V128=TYPE_BIT, mask
VCMP mask, vector, threshold, 4  ; vector > threshold

; Blend based on comparison
VBLEND result, vector, zeros, mask
```

## Hardware Implementation

While vector operations are universal across processor types, their implementation varies based on available hardware:

1. **On SIMD-capable CPUs**:
   - Instructions map to native SIMD/vector instructions (SSE, AVX, NEON, etc.)
   - TYPE_V128 maps to 128-bit registers (XMM on x86)
   - TYPE_V256 maps to 256-bit registers (YMM on x86)
   - TYPE_V512 maps to 512-bit registers (ZMM on x86)

2. **On processors without native SIMD**:
   - Operations are automatically decomposed into scalar operations
   - Each element is processed individually by the COIL processor

This abstraction allows COIL code to benefit from SIMD acceleration when available without requiring architecture-specific code.

## Related Documentation

- [Type System](../spec/systems/type-system.md) - Type system including vector types
- [Arithmetic Instructions](arithmetic.md) - Standard arithmetic operations that work with vectors
- [Type Reference](../reference/type-ref.md) - Complete reference of all types including vectors
- [Memory Operations](memory.md) - Memory operations including vector loading/storing