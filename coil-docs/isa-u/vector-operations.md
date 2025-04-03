# Vector Operations

## Overview

Vector operations provide specialized instructions for working with vector types, arrays, matrices, and tensors. These operations enable efficient manipulation of multiple data elements, supporting both element-wise operations and higher-level mathematical computations.

Note: Basic arithmetic operations (ADD, SUB, MUL, DIV, etc.) are already supported for vectors through the standard arithmetic instructions with type inference. This section covers only specialized vector operations that would not make sense for scalar values.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x80   | GETE     | Get element |
| 0x81   | SETE     | Set element |
| 0x82   | DOT      | Vector dot product |
| 0x83   | CROSS    | Vector cross product |
| 0x84   | NORM     | Vector normalization |
| 0x85   | LEN      | Vector length/magnitude |
| 0x86   | SHUF     | Vector shuffle elements |
| 0x87   | EXTRACT  | Extract subvector |
| 0x88   | INSERT   | Insert subvector |
| 0x89   | TRANS    | Matrix transpose |
| 0x8A   | INV      | Matrix inverse |
| 0x8B   | DET      | Matrix determinant |
| 0x8C   | ROW      | Get/set row |
| 0x8D   | COL      | Get/set column |
| 0x8E   | DIAG     | Get/set diagonal |

## Detailed Specifications

### GETE (0x80)

Retrieves an element from a vector.

**Encoding:**
```
[0x80][0x03][dest: Operand][vector: Operand][index: Operand]
```

**Behavior:**
- Extracts a single element from the vector at the specified index
- Copies the value to the destination operand
- Destination type must be compatible with the vector element type
- Index must be an integral type
- For fixed-width vectors, index must be in range [0, element_count-1]
- Out-of-range indices raise an exception or have implementation-defined behavior
- Sets flags based on the extracted value

### SETE (0x81)

Sets an element in a vector.

**Encoding:**
```
[0x81][0x03][vector: Operand][index: Operand][value: Operand]
```

**Behavior:**
- Replaces a single element in the vector at the specified index
- Vector must be a writable location
- Index must be an integral type
- For fixed-width vectors, index must be in range [0, element_count-1]
- Value type must be compatible with the vector element type
- Out-of-range indices raise an exception or have implementation-defined behavior
- Sets flags based on the operation success

### DOT (0x82)

Calculates the dot product of vectors.

**Encoding:**
```
// Three-operand form
[0x82][0x03][dest: Operand][vec1: Operand][vec2: Operand]
```

**Behavior:**
- Calculates the dot product (scalar product) of two vectors
- Vectors must have the same number of elements
- Element types must be compatible for multiplication
- Result is a scalar value of appropriate type
- For integer vectors, result is an integer
- For floating-point vectors, result is a floating-point value
- Sets flags based on the result (zero, sign)

### CROSS (0x83)

Calculates the cross product of 3D vectors.

**Encoding:**
```
[0x83][0x03][dest: Operand][vec1: Operand][vec2: Operand]
```

**Behavior:**
- Calculates the cross product of two 3D vectors
- Input vectors must have exactly 3 elements
- Element types must be compatible for multiplication and subtraction
- Result is a 3D vector
- Produces a vector perpendicular to both input vectors
- Sets flags based on the result (zero)
- For non-3D vectors, raises an exception or has implementation-defined behavior

### NORM (0x84)

Normalizes a vector to unit length.

**Encoding:**
```
// Two-operand form
[0x84][0x02][dest: Operand][src: Operand]

// One-operand form (in-place)
[0x84][0x01][vector: Operand]
```

**Behavior:**
- Normalizes the vector to have a magnitude of 1.0
- Divides each element by the vector magnitude
- For zero vectors, behavior is implementation-defined
- Element types must support division
- For integer vectors, result may be truncated
- For floating-point vectors, result is exact
- Sets flags based on the operation success

### LEN (0x85)

Calculates the length (magnitude) of a vector.

**Encoding:**
```
[0x85][0x02][dest: Operand][vector: Operand]
```

**Behavior:**
- Calculates the Euclidean norm (magnitude) of the vector
- Result is a scalar value (floating-point)
- For integer vectors, result is converted to floating-point
- Uses the formula: sqrt(sum(v[i]²))
- Sets flags based on the result (zero, sign)

### SHUF (0x86)

Shuffles elements of a vector according to a mask.

**Encoding:**
```
[0x86][0x03][dest: Operand][vector: Operand][mask: Operand]
```

**Behavior:**
- Rearranges elements from the source vector according to indices in the mask
- Destination must be a vector type
- Mask must be a vector of integral values
- Each element in the mask specifies the source element index
- Out-of-range indices yield implementation-defined behavior
- Sets flags based on the operation success

### EXTRACT (0x87)

Extracts a contiguous subvector.

**Encoding:**
```
[0x87][0x04][dest: Operand][vector: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Extracts a contiguous sequence of elements from the source vector
- Start specifies the first element index (integral value)
- Length specifies the number of elements to extract (integral value)
- Destination must be a vector type that can hold the specified number of elements
- Sets flags based on the operation success
- Out-of-range extraction raises an exception or has implementation-defined behavior

### INSERT (0x88)

Inserts a subvector into another vector.

**Encoding:**
```
[0x88][0x04][dest: Operand][subvec: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Inserts a sequence of elements from subvec into dest starting at the specified index
- Start specifies the index in dest where insertion begins (integral value)
- Length specifies the number of elements to insert (integral value)
- Length must not exceed the length of subvec
- Destination must be a writable vector with sufficient space
- Sets flags based on the operation success
- Out-of-range insertion raises an exception or has implementation-defined behavior

### TRANS (0x89)

Transposes a matrix.

**Encoding:**
```
// Two-operand form
[0x89][0x02][dest: Operand][src: Operand]

// One-operand form (in-place)
[0x89][0x01][matrix: Operand]
```

**Behavior:**
- Transposes a matrix (rows become columns, columns become rows)
- Source must be a matrix type (MAT, SQMAT)
- Destination must be a matrix type with dimensions swapped
- For square matrices, can be performed in-place
- For non-square matrices, dest must have correct dimensions (rows=src.cols, cols=src.rows)
- Sets flags based on the operation success

### INV (0x8A)

Calculates a matrix inverse.

**Encoding:**
```
// Two-operand form
[0x8A][0x02][dest: Operand][src: Operand]

// One-operand form (in-place)
[0x8A][0x01][matrix: Operand]
```

**Behavior:**
- Calculates the inverse of a matrix
- Source must be a square matrix type (SQMAT)
- Destination must be a square matrix of the same dimensions
- For singular matrices, raises an exception or has implementation-defined behavior
- For floating-point matrices, may have precision limitations
- Sets flags based on the operation success
- Sets a condition flag if the matrix is singular

### DET (0x8B)

Calculates a matrix determinant.

**Encoding:**
```
[0x8B][0x02][dest: Operand][matrix: Operand]
```

**Behavior:**
- Calculates the determinant of a square matrix
- Matrix must be a square matrix type (SQMAT)
- Destination must be a scalar type compatible with the matrix element type
- For non-square matrices, raises an exception
- For large matrices, uses efficient algorithms (LU decomposition)
- Sets flags based on the result (zero, sign)

### ROW (0x8C)

Gets or sets a matrix row.

**Encoding:**
```
// Get row
[0x8C][0x03][dest: Operand][matrix: Operand][row_idx: Operand]

// Set row
[0x8C][0x03][matrix: Operand][row_idx: Operand][vector: Operand]
```

**Behavior:**
- Gets or sets an entire row of a matrix
- Matrix must be a matrix type (MAT, SQMAT)
- Row index must be an integral type in range [0, matrix.rows-1]
- For get: Destination must be a vector type that can hold matrix.cols elements
- For set: Vector must be a vector type with matrix.cols elements
- Sets flags based on the operation success
- Out-of-range indices raise an exception

### COL (0x8D)

Gets or sets a matrix column.

**Encoding:**
```
// Get column
[0x8D][0x03][dest: Operand][matrix: Operand][col_idx: Operand]

// Set column
[0x8D][0x03][matrix: Operand][col_idx: Operand][vector: Operand]
```

**Behavior:**
- Gets or sets an entire column of a matrix
- Matrix must be a matrix type (MAT, SQMAT)
- Column index must be an integral type in range [0, matrix.cols-1]
- For get: Destination must be a vector type that can hold matrix.rows elements
- For set: Vector must be a vector type with matrix.rows elements
- Sets flags based on the operation success
- Out-of-range indices raise an exception

### DIAG (0x8E)

Gets or sets a matrix diagonal.

**Encoding:**
```
// Get diagonal
[0x8E][0x03][dest: Operand][matrix: Operand][diag_idx: Operand]

// Set diagonal
[0x8E][0x03][matrix: Operand][diag_idx: Operand][vector: Operand]
```

**Behavior:**
- Gets or sets a diagonal of a matrix
- Matrix must be a matrix type (MAT, SQMAT)
- Diagonal index must be an integral type:
  - 0 = main diagonal
  - positive = above main diagonal
  - negative = below main diagonal
- Length of diagonal depends on matrix dimensions and diagonal index
- Sets flags based on the operation success
- Out-of-range indices raise an exception

## Vector Types

Vector operations work with several types of multi-element data structures:

| Vector Types | Description |
|-------------|-------------|
| Fixed-width | Standard SIMD vectors (V128, V256, V512) |
| Custom-width | User-defined vector (CVEC) |
| Matrix | 2D arrays represented as vectors of vectors |
| Tensor | Multi-dimensional arrays |

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Type System](../types/type-system.md) - Complete type system reference