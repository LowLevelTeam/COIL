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

## Linear Algebra Conventions

All vector and matrix operations in COIL follow these conventions:

1. **Matrix Storage**: Row-major order (rows are stored as contiguous elements)
2. **Vector Orientation**: Column vectors by default for linear algebra operations
3. **Indexing**: Zero-based indexing for all elements
4. **Matrix Multiplication**: Follows standard mathematical definition (A × B requires A.cols = B.rows)
5. **Dimensions**: Explicitly checked before operations

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
- Out-of-range indices raise an exception
- Sets flags based on the extracted value

**Mathematical Definition:**
For a vector v and index i:
dest = v[i]

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
- Out-of-range indices raise an exception
- Sets no flags

**Mathematical Definition:**
For a vector v, index i, and value val:
v[i] = val

### DOT (0x82)

Calculates the dot product of vectors.

**Encoding:**
```
// Vector dot product
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

**Mathematical Definition:**
For vectors a and b of length n:
dot(a, b) = ∑(i=0 to n-1) a[i] × b[i]

**Error Handling:**
- If vector dimensions don't match, an exception is raised
- For empty vectors (length 0), the result is defined as 0

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
- For non-3D vectors, raises an exception

**Mathematical Definition:**
For 3D vectors a and b:
cross(a, b) = [
  a[1] × b[2] - a[2] × b[1],
  a[2] × b[0] - a[0] × b[2],
  a[0] × b[1] - a[1] × b[0]
]

**Error Handling:**
- If either vector doesn't have exactly 3 elements, an exception is raised

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
- For zero vectors, sets D flag and produces a vector of zeros
- Element types must support division
- For integer vectors, result may be truncated
- For floating-point vectors, result is exact
- Sets flags based on the operation success

**Mathematical Definition:**
For a vector v with length |v|:
norm(v) = v / |v| if |v| > 0, otherwise [0, 0, ..., 0]

**Error Handling:**
- If the vector has length 0, D flag is set and a zero vector is produced

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

**Mathematical Definition:**
For a vector v of length n:
|v| = √(∑(i=0 to n-1) v[i]²)

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
- Out-of-range indices in mask raise an exception
- Sets no flags

**Mathematical Definition:**
For a vector v, mask m, and destination d:
For each i, d[i] = v[m[i]]

**Error Handling:**
- If any index in the mask is out of range, an exception is raised

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
- Sets no flags
- Out-of-range extraction raises an exception

**Mathematical Definition:**
For a vector v, start index s, and length l:
extract(v, s, l) = [v[s], v[s+1], ..., v[s+l-1]]

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
- Sets no flags
- Out-of-range insertion raises an exception

**Mathematical Definition:**
For vectors d and s, start index t, and length l:
For i from 0 to l-1: d[t+i] = s[i]

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
- Sets no flags

**Mathematical Definition:**
For a matrix A with elements a_ij, the transpose A^T has elements:
a^T_ij = a_ji

**Error Handling:**
- If destination dimensions don't match required dimensions, an exception is raised
- For in-place transposition of non-square matrices, an exception is raised

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
- For singular matrices (determinant = 0), sets D flag and raises an exception
- For floating-point matrices, may have precision limitations
- Sets flags based on the operation success
- Sets Z flag if matrix is singular (determinant = 0)

**Mathematical Definition:**
For a matrix A with inverse A^(-1):
A × A^(-1) = A^(-1) × A = I (identity matrix)

**Implementation Notes:**
- For 2×2 matrices: Direct formula using determinant
- For 3×3 matrices: Adjugate matrix divided by determinant
- For larger matrices: Gaussian elimination, LU decomposition, or other numerical methods

**Error Handling:**
- If the matrix is singular (determinant = 0), D flag is set and an exception is raised

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

**Mathematical Definition:**
For a 2×2 matrix A:
det(A) = a_00 × a_11 - a_01 × a_10

For a 3×3 matrix A:
det(A) = a_00(a_11a_22 - a_12a_21) - a_01(a_10a_22 - a_12a_20) + a_02(a_10a_21 - a_11a_20)

For larger matrices:
- Use cofactor expansion, LU decomposition, or other methods

**Implementation Notes:**
- 2×2 and 3×3 matrices: Direct formula calculation
- 4×4 matrices: Cofactor expansion or decomposition methods
- Larger matrices: LU decomposition for efficiency

**Error Handling:**
- If the matrix is not square, an exception is raised

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
- Sets no flags
- Out-of-range indices raise an exception

**Mathematical Definition:**
For a matrix A, row index r, and vector v:
- Get: v = [A_r0, A_r1, ..., A_r(cols-1)]
- Set: For each i: A_ri = v_i

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
- Sets no flags
- Out-of-range indices raise an exception

**Mathematical Definition:**
For a matrix A, column index c, and vector v:
- Get: v = [A_0c, A_1c, ..., A_(rows-1)c]
- Set: For each i: A_ic = v_i

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
- Sets no flags
- Out-of-range diagonal indices raise an exception

**Mathematical Definition:**
For an m×n matrix A and diagonal index d:
- Main diagonal (d=0): Elements A_ii for all valid i
- Above main diagonal (d>0): Elements A_i(i+d) for all valid i
- Below main diagonal (d<0): Elements A_(i-d)i for all valid i

**Diagonal Length Calculation:**
- Main diagonal (d=0): min(rows, cols)
- Above main diagonal (d>0): min(rows, cols-d)
- Below main diagonal (d<0): min(rows+d, cols)

## Matrix Multiplication

The MUL instruction performs matrix multiplication when both operands are matrices:

**Behavior:**
- For matrix A (m×n) and matrix B (n×p), result is matrix C (m×p)
- Element types must be compatible for multiplication and addition
- Dimensions must be compatible (A.cols = B.rows)
- Row-major storage is used for all matrices
- Result is calculated using the standard matrix multiplication formula
- Incompatible dimensions raise an exception

**Mathematical Definition:**
For matrices A (m×n) and B (n×p), the result C (m×p) has elements:
C_ij = ∑(k=0 to n-1) A_ik × B_kj

**Binary Encoding Example:**
```
// MUL C, A, B where C is 2×3, A is 2×4, B is 4×3
[0x42][0x03]
  [0x34][0x40][0x10][0x00][0x0002][0x0003][var_id_C]  // C: 2×3 matrix of FP32
  [0x34][0x40][0x10][0x00][0x0002][0x0004][var_id_A]  // A: 2×4 matrix of FP32
  [0x34][0x40][0x10][0x00][0x0004][0x0003][var_id_B]  // B: 4×3 matrix of FP32
```

## Matrix-Vector Multiplication

The MUL instruction performs matrix-vector multiplication when operands are a matrix and a vector:

**Behavior:**
- For matrix A (m×n) and vector v (n), result is vector r (m)
- Vector is treated as a column vector
- Element types must be compatible for multiplication and addition
- Dimensions must be compatible (A.cols = v.length)
- Incompatible dimensions raise an exception

**Mathematical Definition:**
For matrix A (m×n) and vector v (n), the result r (m) has elements:
r_i = ∑(j=0 to n-1) A_ij × v_j

**Binary Encoding Example:**
```
// MUL r, A, v where r is length 3, A is 3×4, v is length 4
[0x42][0x03]
  [0x33][0x40][0x10][0x00][0x0003][var_id_r]  // r: vector of 3 FP32 elements
  [0x34][0x40][0x10][0x00][0x0003][0x0004][var_id_A]  // A: 3×4 matrix of FP32
  [0x33][0x40][0x10][0x00][0x0004][var_id_v]  // v: vector of 4 FP32 elements
```

## Non-Conforming Dimensions

When vector or matrix operations encounter non-conforming dimensions:

1. **Compile-time Detection**: If dimensions are known at compile time and don't conform, compilation fails with an error
2. **Runtime Detection**: If dimensions are only known at runtime and don't conform:
   - An exception is raised
   - The D flag may be set to indicate the dimension error
   - No partial results are produced

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Type System](../types/type-system.md) - Complete type system reference
- [Flag Effects](../core/flag-effects.md) - Detailed flag behavior documentation