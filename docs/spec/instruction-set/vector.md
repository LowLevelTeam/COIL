# Vector/Array Operations (0x90-0x9F)

## Purpose

This document defines the vector and array operations in COIL, which enable efficient parallel processing of multiple data elements. These instructions provide SIMD (Single Instruction Multiple Data) capabilities and are universal across all processor types.

## Key Concepts

- **Universal Operations**: Vector operations are available on all processors (opcodes 0x90-0x9F)
- **Automatic Implementation**: Operations map to SIMD instructions when available or scalar operations when not 
- **Vector Types**: Special types that contain multiple elements of the same base type
- **Element Access**: How to extract or insert individual elements
- **Vector Math**: Mathematical operations on vectors

## Vector Types

COIL provides three primary vector types:

| Type | Size (Bytes) | Description | Example Elements |
|------|--------------|-------------|------------------|
| TYPE_V128 | 16 | 128-bit vector | 4 x 32-bit floats |
| TYPE_V256 | 32 | 256-bit vector | 8 x 32-bit floats |
| TYPE_V512 | 64 | 512-bit vector | 16 x 32-bit floats |

Vector types are parameterized by their element type:
```
TYPE_V128=TYPE_INT32    ; Vector of 4 x 32-bit integers
TYPE_V256=TYPE_FP32     ; Vector of 8 x 32-bit floats
TYPE_V512=TYPE_INT16    ; Vector of 32 x 16-bit integers
```

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
VLOAD vector, [array_ptr]

; Broadcast scalar to all vector elements
VLOAD vector, [scalar_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST
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

### Vector Math Operations

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

Basic arithmetic instructions automatically work with vectors in an element-wise manner:

```
; Element-wise addition
ADD vector_sum, vector1, vector2

; Element-wise multiplication
MUL vector_product, vector1, vector2

; Scalar-vector multiplication
MUL scaled_vector, vector, scalar_value
```

## Implementation Approach

Vector operations are **universal** across all processor types (unlike opcodes in the 0xC0-0xFE range which are processor-specific). Their implementation varies based on available hardware:

1. **On SIMD-capable CPUs**:
   - Instructions map to native SIMD/vector instructions (SSE, AVX, NEON, etc.)
   - TYPE_V128 maps to 128-bit registers (XMM on x86)
   - TYPE_V256 maps to 256-bit registers (YMM on x86)
   - TYPE_V512 maps to 512-bit registers (ZMM on x86)

2. **On processors without native SIMD**:
   - Operations are automatically decomposed into scalar operations
   - Each element is processed individually by the COIL processor

This abstraction allows COIL code to benefit from SIMD acceleration when available without requiring architecture-specific code.

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

## Related Documentation

- [Type System](../systems/type-system.md) - Type system including vector types
- [Arithmetic Instructions](arithmetic.md) - Standard arithmetic operations that work with vectors
- [Type Reference](../../reference/type-ref.md) - Complete reference of all types including vectors
- [Memory Operations](memory.md) - Memory operations including vector loading/storing