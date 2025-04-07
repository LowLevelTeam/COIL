# Matrix and Tensor Memory Layout

## Overview

This document provides detailed specifications for the memory layout of matrix and tensor types in COIL. These specifications ensure consistent implementation across different platforms and provide clear guidelines for handling multi-dimensional data structures.

## Matrix Memory Layout

Matrices in COIL follow these layout rules:

### 1. Row-Major Order

All matrices are stored in row-major order, where elements from the same row are stored contiguously in memory:

For a matrix M with dimensions (rows × cols):
- Element M[i,j] is at offset: (i × cols + j) × element_size
- Row i starts at offset: (i × cols) × element_size

### 2. Alignment Requirements

- The entire matrix is aligned to the larger of:
  - The natural alignment of the element type
  - Platform-specific SIMD alignment requirements (typically 16 bytes)
- Each row's starting address must be aligned to the element's natural alignment
- No padding is inserted between elements within a row

### 3. Memory Layout Example (3×4 Matrix of FP32)

For a 3×4 matrix of 32-bit floating-point values:

```
Memory Offset | Content   | Description
------------- | --------- | ------------------
0x00          | M[0,0]    | Row 0, Column 0
0x04          | M[0,1]    | Row 0, Column 1
0x08          | M[0,2]    | Row 0, Column 2
0x0C          | M[0,3]    | Row 0, Column 3
0x10          | M[1,0]    | Row 1, Column 0
0x14          | M[1,1]    | Row 1, Column 1
0x18          | M[1,2]    | Row 1, Column 2
0x1C          | M[1,3]    | Row 1, Column 3
0x20          | M[2,0]    | Row 2, Column 0
0x24          | M[2,1]    | Row 2, Column 1
0x28          | M[2,2]    | Row 2, Column 2
0x2C          | M[2,3]    | Row 2, Column 3
```

Total size: 48 bytes (3 rows × 4 columns × 4 bytes per FP32)

## Tensor Memory Layout

Tensors with dimensions higher than 2 follow a generalized row-major ordering:

### 1. Generalized Row-Major Order

- Elements are stored with the rightmost index varying fastest
- For a tensor T with dimensions [d₁, d₂, ..., dₙ]:
  - Element T[i₁, i₂, ..., iₙ] is at offset: (i₁ × s₁ + i₂ × s₂ + ... + iₙ) × element_size
  - Where stride s₁ = d₂ × d₃ × ... × dₙ, s₂ = d₃ × ... × dₙ, ..., sₙ₋₁ = dₙ, sₙ = 1

### 2. Alignment Requirements

- The entire tensor is aligned to the larger of:
  - The natural alignment of the element type
  - Platform-specific SIMD alignment requirements (typically 16 bytes)
- Each "slice" (equivalent to a row in matrices) starts at an address aligned to the element's natural alignment
- No padding is inserted between elements within a slice

### 3. Memory Layout Example (2×3×4 Tensor of INT32)

For a 2×3×4 tensor of 32-bit integers:

```
Memory Offset | Content      | Indices    | Description
------------- | ------------ | ---------- | ------------------
0x00          | T[0,0,0]     | [0,0,0]    | First slice, first row, first column
0x04          | T[0,0,1]     | [0,0,1]    | First slice, first row, second column
0x08          | T[0,0,2]     | [0,0,2]    | First slice, first row, third column
0x0C          | T[0,0,3]     | [0,0,3]    | First slice, first row, fourth column
0x10          | T[0,1,0]     | [0,1,0]    | First slice, second row, first column
...
0x6C          | T[1,2,3]     | [1,2,3]    | Second slice, third row, fourth column
```

Total size: 96 bytes (2 × 3 × 4 × 4 bytes per INT32)

### 4. Stride Calculation Example

For the 2×3×4 tensor above:
- Stride s₁ = 3 × 4 = 12 (elements to skip to reach same position in next slice)
- Stride s₂ = 4 (elements to skip to reach same position in next row)
- Stride s₃ = 1 (elements to skip to reach next column)

Element [i,j,k] is at offset: (i × 12 + j × 4 + k) × 4 bytes

## Binary Encoding Examples

### 1. 3×3 Matrix of FP32

```
Binary: [0x34][0x00][0x10][0x00][0x0003][0x0003]
Breakdown:
- 0x34: MAT type opcode
- 0x00: Type extension (no flags)
- 0x10, 0x00: Element type (FP32)
- 0x0003: Rows (3)
- 0x0003: Columns (3)
```

### 2. 2×3×4 Tensor of INT32

```
Binary: [0x37][0x00][0x04][0x00][0x03][0x0002][0x0003][0x0004]
Breakdown:
- 0x37: TENSOR type opcode
- 0x00: Type extension (no flags)
- 0x04, 0x00: Element type (INT32)
- 0x03: Rank (3 dimensions)
- 0x0002: First dimension (2)
- 0x0003: Second dimension (3)
- 0x0004: Third dimension (4)
```

## Implementation Notes

1. **Dimension Limits**
   - Maximum rank: 8 dimensions
   - Maximum size per dimension: 65,535 (uint16_t)
   - Maximum total elements: Platform-dependent

2. **Allocation Considerations**
   - Implementations should allocate tensor memory as a single contiguous block
   - For large tensors, platform-specific allocation strategies may be necessary

3. **Optimizations**
   - Implementations may internally use different layouts for performance
   - Any such optimization must preserve the logical indexing behavior
   - Conversions between different physical layouts must be transparent to the user