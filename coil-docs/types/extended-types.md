# Extended Types

## Overview

COIL's Extended Types expand the basic type system to include more complex mathematical constructs such as matrices and tensors. These types enable advanced mathematical operations while maintaining the type-determined behavior pattern that is central to COIL.

## Matrix Types (0x34-0x36)

Matrices are two-dimensional arrays of elements with specialized algebraic properties.

| Opcode | Name      | Description                 | Type Data Format |
|--------|-----------|-----------------------------|--------------------|
| 0x34   | MAT       | General matrix              | [element type: Type][rows: uint16_t][cols: uint16_t] |
| 0x35   | SQMAT     | Square matrix               | [element type: Type][size: uint16_t] |
| 0x36   | DIAGMAT   | Diagonal matrix             | [element type: Type][size: uint16_t] |

### MAT (0x34)

General matrix of any dimension.

**Type Data Format:**
```
[element type: Type][rows: uint16_t][cols: uint16_t]
```

**Behavior:**
- Represents a matrix with specified rows and columns
- Element type determines the type of each matrix element
- Memory layout is row-major by default
- Supports standard matrix operations

### SQMAT (0x35)

Square matrix (equal rows and columns).

**Type Data Format:**
```
[element type: Type][size: uint16_t]
```

**Behavior:**
- Represents a square matrix (rows = columns)
- Optimized representation for square matrices
- Supports special square matrix operations (determinant, inverse, etc.)

### DIAGMAT (0x36)

Diagonal matrix (only diagonal elements can be non-zero).

**Type Data Format:**
```
[element type: Type][size: uint16_t]
```

**Behavior:**
- Represents a diagonal matrix
- Only stores diagonal elements (size elements total)
- All off-diagonal elements are implicitly zero
- Optimized for computation involving diagonal matrices


## Tensor Types (0x37-0x38)

Tensors are multi-dimensional arrays with specialized algebraic properties.

| Opcode | Name      | Description              | Type Data Format |
|--------|-----------|--------------------------|------------------|
| 0x37   | TENSOR    | General tensor           | [element type: Type][rank: uint8_t][dim1: uint16_t]...[dimN: uint16_t] |
| 0x38   | SYMTENSOR | Symmetric tensor         | [element type: Type][rank: uint8_t][dim: uint16_t] |

### TENSOR (0x37)

General tensor of any rank and dimensions.

**Type Data Format:**
```
[element type: Type][rank: uint8_t][dim1: uint16_t]...[dimN: uint16_t]
```

**Behavior:**
- Represents a tensor with arbitrary rank and dimensions
- Element type determines the type of each tensor element
- Memory layout is row-major (C-style) by default
- Supports tensor operations (contraction, product, etc.)

### SYMTENSOR (0x38)

Symmetric tensor with equal dimensions.

**Type Data Format:**
```
[element type: Type][rank: uint8_t][dim: uint16_t]
```

**Behavior:**
- Represents a symmetric tensor (all dimensions equal)
- Optimized storage for symmetric tensors
- Ensures symmetry is preserved in operations

## Special Type Extensions for Advanced Types

Matrix and tensor types support additional type extensions:

| Bit      | Name       | Description                             |
|----------|-----------|-----------------------------------------|
| (1 << 4) | TRANSPOSE  | Matrix is stored in transposed form     |
| (1 << 5) | SPARSE     | Sparse representation (compressed)      |

## Memory Layout

The memory layout for extended types follows these conventions:

### Matrix Layout

Matrices are stored in row-major order by default:
```
| A11 A12 A13 |
| A21 A22 A23 | -> [A11, A12, A13, A21, A22, A23]
```

With the TRANSPOSE extension, they are stored in column-major order:
```
| A11 A12 A13 |
| A21 A22 A23 | -> [A11, A21, A12, A22, A13, A23]
```

### Tensor Layout

Tensors use generalized row-major (C-style) ordering:
```
T[i,j,k] is located at base_address + (i * dim_j * dim_k + j * dim_k + k) * element_size
```

## Type Conversion Rules

Conversion between extended types follows these rules:

1. **Vector to Matrix**: A vector can be converted to a single-row or single-column matrix
2. **Matrix to Vector**: A single-row or single-column matrix can be converted to a vector
3. **Matrix to Matrix**: Matrices can be converted if dimensions are compatible
4. **Tensor to Matrix**: A 2D tensor can be converted to a matrix
5. **Matrix to Tensor**: A matrix can be converted to a 2D tensor

## Related Components

- [Type System](./type-system.md) - Complete type system reference
- [Vector Operations](../isa-u/vector-operations.md) - Vector processing instructions
- [Matrix Operations](../isa-u/matrix-operations.md) - Matrix-specific operations