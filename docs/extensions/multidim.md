# Multi-Dimensional Types Extension

## Overview

The Multi-Dimensional Types Extension enhances COIL's type system with vectors, matrices, and higher-dimensional tensors. This enables scientific computing, graphics, machine learning, and other domains requiring advanced mathematical operations.

## Extension Information

- **Extension ID**: 0x01
- **Extension Name**: MultiDim
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Type Definitions

### Vector Types (0x33)

```
[0x33][type_extension][element_type][count: uint16_t]
```

Example: Vector of 4 FP32 elements
```
[0x33][0x00][0x10][0x00][0x0004]
```

### Matrix Types (0x34-0x36)

#### General Matrix (0x34)
```
[0x34][type_extension][element_type][rows: uint16_t][cols: uint16_t]
```

#### Square Matrix (0x35)
```
[0x35][type_extension][element_type][dim: uint16_t]
```

#### Diagonal Matrix (0x36)
```
[0x36][type_extension][element_type][dim: uint16_t]
```

Example: 3×3 matrix of FP32 elements
```
[0x34][0x00][0x10][0x00][0x0003][0x0003]
```

### Tensor Types (0x37-0x38)

#### General Tensor (0x37)
```
[0x37][type_extension][element_type][rank: uint8_t][dim1: uint16_t][dim2: uint16_t]...
```

Example: 3D tensor of shape [2,3,4] with FP32 elements
```
[0x37][0x00][0x10][0x00][0x03][0x0002][0x0003][0x0004]
```

## Memory Layout

### Vector Layout

Vectors use a contiguous memory layout:
```
│ Element 0 │ Element 1 │ ... │ Element N-1 │
```

### Matrix Layout

Matrices use row-major layout by default:
```
│ Row 0, Col 0 │ Row 0, Col 1 │ ... │ Row 0, Col M-1 │
│ Row 1, Col 0 │ Row 1, Col 1 │ ... │ Row 1, Col M-1 │
                           ...
│ Row N-1, Col 0 │ Row N-1, Col 1 │ ... │ Row N-1, Col M-1 │
```

## Instruction Set Extensions

### Vector Operations (0x82-0x85)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x82   | DOT      | Vector dot product | 3 |
| 0x83   | CROSS    | Vector cross product | 3 |
| 0x84   | NORM     | Vector normalization | 1-2 |
| 0x85   | LEN      | Vector length/magnitude | 2 |

### Vector Element Operations (0x86-0x88)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x86   | SHUF     | Vector shuffle elements | 3 |
| 0x87   | EXTRACT  | Extract subvector | 4 |
| 0x88   | INSERT   | Insert subvector | 4 |

### Matrix Operations (0x89-0x8D)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x89   | TRANS    | Matrix transpose | 1-2 |
| 0x8A   | INV      | Matrix inverse | 1-2 |
| 0x8B   | DET      | Matrix determinant | 2 |
| 0x8C   | ROW      | Get/set row | 3 |
| 0x8D   | COL      | Get/set column | 3 |

## Detailed Instruction Behaviors

### DOT (0x82)

```
DOT result, vec1, vec2
```

Computes the scalar product of two vectors.

- `result`: Destination for scalar result
- `vec1`, `vec2`: Source vectors of the same length

Flag effects:
- Z: Set if result is zero
- S: Set if result is negative
- V: Set if computation overflows
- N: Set if result is NaN

### CROSS (0x83)

```
CROSS result, vec1, vec2
```

Computes the vector product of two 3D vectors.

- `result`: Destination vector
- `vec1`, `vec2`: Source 3D vectors

Flag effects:
- Z: Set if result is zero vector
- Other flags unchanged

### NORM (0x84)

```
NORM result, vec
```
or
```
NORM vec
```

Creates a unit vector in the same direction.

Flag effects:
- Z: Set if input is zero vector
- D: Set if normalization by zero attempted
- N: Set if result contains NaN

## Type Promotion Rules

When operating on multi-dimensional types, the following promotion rules apply:

1. **Element Type Promotion**:
   - Follows the same rules as scalar operations
   - The result has the higher precision element type

2. **Dimension Compatibility**:
   - Operations require compatible dimensions
   - Matrix multiplication requires inner dimensions to match
   - Vector operations require same length unless otherwise specified

## Usage Examples

```
; Vector dot product
CVEC<F32, 3> v1 = [1.0, 2.0, 3.0]
CVEC<F32, 3> v2 = [4.0, 5.0, 6.0]
F32 result
DOT result, v1, v2     ; result = 32.0

; Matrix transpose
MAT<F32, 2, 3> m1 = [[1.0, 2.0, 3.0], [4.0, 5.0, 6.0]]
MAT<F32, 3, 2> m2
TRANS m2, m1           ; m2 = [[1.0, 4.0], [2.0, 5.0], [3.0, 6.0]]

; Vector normalization
CVEC<F32, 3> v = [3.0, 0.0, 4.0]
NORM v                 ; v = [0.6, 0.0, 0.8] (normalized)
```