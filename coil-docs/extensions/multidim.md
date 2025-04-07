# Multi-Dimensional Types Extension

## Overview

The Multi-Dimensional Types Extension extends COIL's type system and instruction set to efficiently handle vectors, matrices, and higher-dimensional tensors. This extension enables scientific computing, graphics, machine learning, and other domains that require advanced mathematical operations.

## Extension Information

- **Extension ID**: 0x01
- **Extension Name**: MultiDim
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Type Definitions

### Vector Types (0x33)

Vector types represent ordered collections of elements of the same type:

```
[0x33][type_extension][element_type][count: uint16_t]
```

Where:
- `element_type`: The type of each vector element
- `count`: Number of elements in the vector

Example: Vector of 4 FP32 elements
```
[0x33][0x00][0x10][0x00][0x0004]
```

### Matrix Types (0x34-0x36)

Matrices represent two-dimensional arrays of elements:

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

Where:
- `element_type`: The type of each matrix element
- `rows`, `cols`: Dimensions of the matrix
- `dim`: Dimension of square or diagonal matrix

Example: 3×3 matrix of FP32 elements
```
[0x34][0x00][0x10][0x00][0x0003][0x0003]
```

### Tensor Types (0x37-0x38)

Tensors represent multi-dimensional arrays:

#### General Tensor (0x37)
```
[0x37][type_extension][element_type][rank: uint8_t][dim1: uint16_t][dim2: uint16_t]...
```

#### Symmetric Tensor (0x38)
```
[0x38][type_extension][element_type][rank: uint8_t][dim: uint16_t]
```

Where:
- `element_type`: The type of each tensor element
- `rank`: Number of dimensions
- `dimN`: Size of each dimension
- `dim`: Size of each dimension for symmetric tensor

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

### Tensor Layout

Tensors use generalized row-major ordering:
- Rightmost indices vary fastest
- Linear index calculation follows standard row-major formula

## Instruction Set Extensions

### Vector Operations (0x82-0x85)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x82   | DOT      | Vector dot product | 3 | MultiDim |
| 0x83   | CROSS    | Vector cross product | 3 | MultiDim |
| 0x84   | NORM     | Vector normalization | 1-2 | MultiDim |
| 0x85   | LEN      | Vector length/magnitude | 2 | MultiDim |

### Vector Element Operations (0x86-0x88)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x86   | SHUF     | Vector shuffle elements | 3 | MultiDim |
| 0x87   | EXTRACT  | Extract subvector | 4 | MultiDim |
| 0x88   | INSERT   | Insert subvector | 4 | MultiDim |

### Matrix Operations (0x89-0x8D)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x89   | TRANS    | Matrix transpose | 1-2 | MultiDim |
| 0x8A   | INV      | Matrix inverse | 1-2 | MultiDim |
| 0x8B   | DET      | Matrix determinant | 2 | MultiDim |
| 0x8C   | ROW      | Get/set row | 3 | MultiDim |
| 0x8D   | COL      | Get/set column | 3 | MultiDim |
| 0x8E   | DIAG     | Get/set diagonal | 3 | MultiDim |

## Detailed Instruction Behaviors

### DOT (0x82)

The dot product instruction computes the scalar product of two vectors:

```
DOT result, vec1, vec2
```

- `result`: Destination for scalar result
- `vec1`, `vec2`: Source vectors of the same length

Behavior:
1. Verifies vectors have the same length
2. Computes sum of element-wise products
3. Stores result in destination
4. Sets flags based on result

Flag effects:
- Z: Set if result is zero
- S: Set if result is negative
- V: Set if computation overflows
- N: Set if result is NaN

### CROSS (0x83)

The cross product instruction computes the vector product of two 3D vectors:

```
CROSS result, vec1, vec2
```

- `result`: Destination vector
- `vec1`, `vec2`: Source 3D vectors

Behavior:
1. Verifies vectors are 3-dimensional
2. Computes the vector cross product
3. Stores result in destination vector
4. Sets flags based on result

Flag effects:
- Z: Set if result is zero vector
- Other flags unchanged

### NORM (0x84)

The normalize instruction creates a unit vector in the same direction:

```
NORM result, vec
```
or
```
NORM vec
```

- `result`: Optional destination vector
- `vec`: Source vector (or source and destination)

Behavior:
1. Computes vector length
2. Divides each element by the length
3. Stores normalized vector in destination
4. Sets flags based on result

Flag effects:
- Z: Set if input is zero vector
- D: Set if normalization by zero attempted
- N: Set if result contains NaN

### Matrix Operations

Matrix operations follow similar patterns:

#### TRANS (0x89)

```
TRANS result, mat
```
or
```
TRANS mat
```

Transposes a matrix by exchanging rows and columns.

#### INV (0x8A)

```
INV result, mat
```
or
```
INV mat
```

Computes the inverse of a square matrix, setting the D flag if matrix is singular.

## Type Promotion Rules

When operating on multi-dimensional types, the following promotion rules apply:

1. **Element Type Promotion**:
   - Follows the same rules as scalar operations
   - The result has the higher precision element type

2. **Dimension Compatibility**:
   - Operations require compatible dimensions
   - Matrix multiplication requires inner dimensions to match
   - Vector operations require same length unless otherwise specified

## Implementation Requirements

Implementations of the Multi-Dimensional Types Extension must:

1. **Type Support**:
   - Support all vector, matrix, and tensor types
   - Handle all type extensions on these types

2. **Instruction Support**:
   - Implement all vector and matrix operations
   - Handle edge cases (zero vectors, singular matrices)

3. **Performance**:
   - Optimize for the target architecture
   - Leverage SIMD/vector hardware when available

4. **Precision**:
   - Document precision guarantees
   - Handle numerical stability appropriately

5. **Graceful Degradation**:
   - Provide fallback implementation when hardware acceleration unavailable
   - Maintain semantics even in non-optimized implementations
