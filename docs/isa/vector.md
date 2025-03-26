# Vector Operations (0x90-0x9F)

## Purpose

This document defines the vector/SIMD (Single Instruction, Multiple Data) operations in COIL, which enable efficient parallel processing of multiple data elements. Vector operations are essential for high-performance computing, graphics, signal processing, and many other applications.

## Vector Types

COIL supports vector operations through dedicated vector types:

```
TYPE_V128 = 0x30  // 128-bit vector
TYPE_V256 = 0x31  // 256-bit vector
TYPE_V512 = 0x32  // 512-bit vector
```

Vector types are parameterized by their element type:
- `TYPE_V128=TYPE_INT32` represents a 128-bit vector of 4x 32-bit integers
- `TYPE_V256=TYPE_FP32` represents a 256-bit vector of 8x 32-bit floats
- `TYPE_V512=TYPE_INT16` represents a 512-bit vector of 32x 16-bit integers

Vector operations typically apply the same operation to all elements in parallel.

## Vector Load/Store Operations

### VLOAD (0x90)
Load data from memory into a vector register.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Source: Memory address
- TYPE_PARAM0: vector_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

Vector control parameters:
```
VECTOR_CTRL_ALIGNED    = 0x01  // Aligned memory access
VECTOR_CTRL_UNALIGNED  = 0x02  // Unaligned memory access
VECTOR_CTRL_BROADCAST  = 0x03  // Broadcast scalar to all elements
VECTOR_CTRL_GATHER     = 0x04  // Gather from indexed memory locations
```

Example:
```
; Load aligned vector from memory
VLOAD TYPE_V128=TYPE_FP32, vec1, [array_ptr]

; Load with broadcasting a single value to all elements
VLOAD TYPE_V256=TYPE_INT32, vec2, [scalar_value], TYPE_PARAM0=VECTOR_CTRL_BROADCAST

; Conditional vector load
VLOAD_NZ TYPE_V128=TYPE_INT16, vec3, [array_ptr]
```

Binary encoding:
```
0x90      ; Opcode for VLOAD
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination vector
[value1]  ; Destination value
[type2]   ; Type of memory address
[value2]  ; Source memory address
[type3]   ; TYPE_PARAM0 (if vector control included)
[value3]  ; Vector control parameter (if included)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### VSTORE (0x91)
Store vector data to memory.

```
Operands:
- Destination: Memory address
- Source: TYPE_V* or TYPE_RV
- TYPE_PARAM0: vector_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Store vector to aligned memory address
VSTORE [array_ptr], TYPE_V256=TYPE_FP32, vec1

; Store with scatter to indexed memory locations
VSTORE [base_ptr], vec2, TYPE_PARAM0=VECTOR_CTRL_SCATTER

; Conditional vector store
VSTORE_NZ [array_ptr], vec3
```

Binary encoding:
```
0x91      ; Opcode for VSTORE
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of memory address
[value1]  ; Destination memory address
[type2]   ; Type of source vector
[value2]  ; Source vector value
[type3]   ; TYPE_PARAM0 (if vector control included)
[value3]  ; Vector control parameter (if included)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Vector Data Movement

### VEXTRACT (0x96)
Extract a scalar element from a vector.

```
Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_V* or TYPE_RV
- Index: TYPE_UNT8 or TYPE_UNT16
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Extract the second element (index 1)
VEXTRACT TYPE_INT32, result, TYPE_V128=TYPE_INT32, vec1, 1

; Extract with variable index
VEXTRACT TYPE_FP32, scalar_result, vec2, index
```

Binary encoding:
```
0x96      ; Opcode for VEXTRACT
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination scalar
[value1]  ; Destination value
[type2]   ; Type of source vector
[value2]  ; Source vector value
[type3]   ; Type of index
[value3]  ; Index value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### VINSERT (0x97)
Insert a scalar element into a vector.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Source: TYPE_V* or TYPE_RV
- Value: Non TYPE_VOID (scalar to insert)
- Index: TYPE_UNT8 or TYPE_UNT16
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Insert value 42 at index 2
VINSERT TYPE_V128=TYPE_INT32, result_vec, src_vec, 42, 2

; Insert scalar variable at specified index
VINSERT TYPE_V256=TYPE_FP32, result_vec, src_vec, scalar_value, index
```

Binary encoding:
```
0x97      ; Opcode for VINSERT
0x04/0x05 ; Four or five operands
[type1]   ; Type of destination vector
[value1]  ; Destination value
[type2]   ; Type of source vector
[value2]  ; Source vector value
[type3]   ; Type of scalar to insert
[value3]  ; Scalar value
[type4]   ; Type of index
[value4]  ; Index value
[type5]   ; TYPE_PARAM5 (if conditional)
[value5]  ; Condition code (if conditional)
```

### VSHUFFLE (0x98)
Shuffle elements of a vector according to a mask.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Source: TYPE_V* or TYPE_RV
- Mask: TYPE_V*, TYPE_UNT8, or immediate pattern
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Reverse elements in a 4-element vector
VSHUFFLE TYPE_V128=TYPE_INT32, result_vec, src_vec, (3, 2, 1, 0)

; Interleave even elements from two vectors
VSHUFFLE TYPE_V256=TYPE_FP32, result_vec, vec1, vec2, (0, 2, 4, 6, 1, 3, 5, 7)
```

Binary encoding:
```
0x98      ; Opcode for VSHUFFLE
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination vector
[value1]  ; Destination value
[type2]   ; Type of source vector
[value2]  ; Source vector value
[type3]   ; Type of shuffle mask
[value3]  ; Shuffle mask value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Vector Comparison and Masks

### VCMP (0x99)
Compare vectors and produce a mask.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Left: TYPE_V* or TYPE_RV
- Right: TYPE_V* or TYPE_RV, or scalar
- Compare Type: TYPE_UNT8
  - 0: Equal
  - 1: Not Equal
  - 2: Less Than
  - 3: Less Than or Equal
  - 4: Greater Than
  - 5: Greater Than or Equal
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Compare for equality, produce mask
VCMP TYPE_V128=TYPE_BIT, result_mask, vec1, vec2, 0

; Compare with scalar, greater than test
VCMP TYPE_V256=TYPE_BIT, result_mask, vec1, 0, 4

; Conditional vector compare
VCMP_NZ TYPE_V128=TYPE_BIT, result_mask, vec1, vec2, 0
```

Binary encoding:
```
0x99      ; Opcode for VCMP
0x04/0x05 ; Four or five operands
[type1]   ; Type of destination mask vector
[value1]  ; Destination value
[type2]   ; Type of left operand vector
[value2]  ; Left operand value
[type3]   ; Type of right operand (vector or scalar)
[value3]  ; Right operand value
[type4]   ; Type of compare type
[value4]  ; Compare type value
[type5]   ; TYPE_PARAM5 (if conditional)
[value5]  ; Condition code (if conditional)
```

### VBLEND (0x9A)
Blend elements from two vectors based on a mask.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- First: TYPE_V* or TYPE_RV
- Second: TYPE_V* or TYPE_RV
- Mask: TYPE_V* or TYPE_RV
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Blend based on mask (result[i] = mask[i] ? first[i] : second[i])
VBLEND TYPE_V128=TYPE_INT32, result_vec, vec1, vec2, mask

; Conditional vector blend
VBLEND_NZ TYPE_V256=TYPE_FP32, result_vec, vec1, vec2, mask
```

Binary encoding:
```
0x9A      ; Opcode for VBLEND
0x04/0x05 ; Four or five operands
[type1]   ; Type of destination vector
[value1]  ; Destination value
[type2]   ; Type of first source vector
[value2]  ; First source value
[type3]   ; Type of second source vector
[value3]  ; Second source value
[type4]   ; Type of mask
[value4]  ; Mask value
[type5]   ; TYPE_PARAM5 (if conditional)
[value5]  ; Condition code (if conditional)
```

### VMASKMOV (0x9B)
Masked vector move operation.

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Source: TYPE_V* or TYPE_RV
- Mask: TYPE_V* or TYPE_RV
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Move only elements where mask is set
VMASKMOV TYPE_V256=TYPE_INT32, dest_vec, src_vec, mask

; Conditional masked move
VMASKMOV_NZ TYPE_V128=TYPE_FP32, dest_vec, src_vec, mask
```

Binary encoding:
```
0x9B      ; Opcode for VMASKMOV
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination vector
[value1]  ; Destination value
[type2]   ; Type of source vector
[value2]  ; Source value
[type3]   ; Type of mask
[value3]  ; Mask value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Advanced Vector Operations

### VDOT (0x9C)
Vector dot product.

```
Operands:
- Destination: Non TYPE_VOID
- Left: TYPE_V* or TYPE_RV
- Right: TYPE_V* or TYPE_RV
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Calculate dot product of two vectors
VDOT TYPE_FP32, result, vec1, vec2

; Integer dot product
VDOT TYPE_INT32, dot_result, int_vec1, int_vec2
```

Binary encoding:
```
0x9C      ; Opcode for VDOT
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination scalar
[value1]  ; Destination value
[type2]   ; Type of left vector
[value2]  ; Left vector value
[type3]   ; Type of right vector
[value3]  ; Right vector value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### VCROSS (0x9D)
Vector cross product (for 3D vectors).

```
Operands:
- Destination: TYPE_V* or TYPE_RV
- Left: TYPE_V* or TYPE_RV
- Right: TYPE_V* or TYPE_RV
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Calculate cross product of two 3D vectors
VCROSS TYPE_V128=TYPE_FP32, result_vec, vec1, vec2
```

### VHSUM (0x9E)
Horizontal sum of vector elements.

```
Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_V* or TYPE_RV
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Sum all elements in a vector
VHSUM TYPE_FP32, result, vec

; Integer horizontal sum
VHSUM TYPE_INT32, sum_result, int_vec
```

## Vector Operation Patterns

### Vector Initialization

```
; Initialize vector with all elements set to same value
VAR TYPE_V128=TYPE_INT32, vec1, (5, 5, 5, 5)  ; Explicit initialization

; Alternative using scalar broadcast
VLOAD TYPE_V128=TYPE_INT32, vec2, [scalar_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST
```

### Vector Element-wise Processing

```
; Element-wise operation on array data
VAR TYPE_PTR=TYPE_FP32, src_array_ptr, source_array
VAR TYPE_PTR=TYPE_FP32, dst_array_ptr, dest_array
VAR TYPE_UNT32, length, 1024
VAR TYPE_UNT32, i, 0

SYM process_loop
    CMP i, length
    BR_GE process_done
    
    ; Load vector chunk
    VLOAD TYPE_V256=TYPE_FP32, src_vec, [src_array_ptr + i*4]
    
    ; Process vector (e.g., multiply by 2.0)
    MUL TYPE_V256=TYPE_FP32, result_vec, src_vec, 2.0
    
    ; Store result
    VSTORE [dst_array_ptr + i*4], result_vec
    
    ; Advance to next chunk
    ADD i, i, 8  ; Process 8 elements at a time (256-bit / 32-bit)
    BR process_loop
    
SYM process_done
```

### Vector Reduction

```
; Compute sum of all elements in an array using vector reduction
VAR TYPE_PTR=TYPE_INT32, array_ptr, data_array
VAR TYPE_UNT32, length, 1000
VAR TYPE_UNT32, i, 0
VAR TYPE_INT32, total_sum, 0

; Process in chunks of 8 elements (256-bit vector / 32-bit elements)
SYM reduce_loop
    CMP i, length
    BR_GE reduce_remainder
    
    ; Load vector chunk
    VLOAD TYPE_V256=TYPE_INT32, vec, [array_ptr + i*4]
    
    ; Horizontal sum of this vector
    VAR TYPE_INT32, chunk_sum
    VHSUM TYPE_INT32, chunk_sum, vec
    
    ; Add to total
    ADD total_sum, total_sum, chunk_sum
    
    ; Advance to next chunk
    ADD i, i, 8
    BR reduce_loop
    
; Handle remainder elements
SYM reduce_remainder
    CMP i, length
    BR_GE reduce_done
    
    ; Process one element at a time
    VAR TYPE_INT32, value
    MOV value, [array_ptr + i*4]
    ADD total_sum, total_sum, value
    
    INC i
    BR reduce_remainder
    
SYM reduce_done
```

### Vector Conditional Processing

```
; Conditionally process elements based on comparison
VAR TYPE_V256=TYPE_FP32, vec1, (1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0)
VAR TYPE_V256=TYPE_FP32, vec2, (5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0)
VAR TYPE_V256=TYPE_FP32, result_vec

; Generate mask for elements where vec1 > vec2
VAR TYPE_V256=TYPE_BIT, mask
VCMP TYPE_V256=TYPE_BIT, mask, vec1, vec2, 4  ; Greater than comparison

; Create two potential result vectors
VAR TYPE_V256=TYPE_FP32, result_if_true
VMUL TYPE_V256=TYPE_FP32, result_if_true, vec1, 2.0  ; Double vec1

VAR TYPE_V256=TYPE_FP32, result_if_false
ADD result_if_false, vec1, vec2  ; Add vec1 and vec2

; Blend based on mask
VBLEND TYPE_V256=TYPE_FP32, result_vec, result_if_true, result_if_false, mask
```

### Matrix Multiplication

```
; Simple 4x4 matrix multiplication using vectors
; Matrix A stored in row-major format in four vectors
VAR TYPE_V128=TYPE_FP32, A_row0, (a00, a01, a02, a03)
VAR TYPE_V128=TYPE_FP32, A_row1, (a10, a11, a12, a13)
VAR TYPE_V128=TYPE_FP32, A_row2, (a20, a21, a22, a23)
VAR TYPE_V128=TYPE_FP32, A_row3, (a30, a31, a32, a33)

; Matrix B stored in column-major format in four vectors
VAR TYPE_V128=TYPE_FP32, B_col0, (b00, b10, b20, b30)
VAR TYPE_V128=TYPE_FP32, B_col1, (b01, b11, b21, b31)
VAR TYPE_V128=TYPE_FP32, B_col2, (b02, b12, b22, b32)
VAR TYPE_V128=TYPE_FP32, B_col3, (b03, b13, b23, b33)

; Calculate result matrix C
VAR TYPE_FP32, c00, c01, c02, c03, c10, c11, c12, c13
VAR TYPE_FP32, c20, c21, c22, c23, c30, c31, c32, c33

; Calculate first row of result
VDOT TYPE_FP32, c00, A_row0, B_col0
VDOT TYPE_FP32, c01, A_row0, B_col1
VDOT TYPE_FP32, c02, A_row0, B_col2
VDOT TYPE_FP32, c03, A_row0, B_col3

; Calculate second row of result
VDOT TYPE_FP32, c10, A_row1, B_col0
VDOT TYPE_FP32, c11, A_row1, B_col1
VDOT TYPE_FP32, c12, A_row1, B_col2
VDOT TYPE_FP32, c13, A_row1, B_col3

; Calculate third row of result
VDOT TYPE_FP32, c20, A_row2, B_col0
VDOT TYPE_FP32, c21, A_row2, B_col1
VDOT TYPE_FP32, c22, A_row2, B_col2
VDOT TYPE_FP32, c23, A_row2, B_col3

; Calculate fourth row of result
VDOT TYPE_FP32, c30, A_row3, B_col0
VDOT TYPE_FP32, c31, A_row3, B_col1
VDOT TYPE_FP32, c32, A_row3, B_col2
VDOT TYPE_FP32, c33, A_row3, B_col3
```

## Processor-Specific Considerations

While vector operations are supported across all processor types, their implementation varies based on available hardware:

1. **Vector Width**:
   - Processors with wider SIMD units (e.g., AVX-512) can process more elements in parallel
   - COIL automatically decomposes wider vector operations on hardware with narrower SIMD units

2. **Instruction Mapping**:
   - On processors with equivalent SIMD instructions, vector operations map directly
   - On processors with limited or no SIMD support, operations are decomposed into scalar instructions

3. **Vector Register Mapping**:
   - TYPE_V128 typically maps to 128-bit registers (e.g., XMM on x86, NEON on ARM)
   - TYPE_V256 typically maps to 256-bit registers (e.g., YMM on x86, SVE on ARM)
   - TYPE_V512 typically maps to 512-bit registers (e.g., ZMM on x86)

4. **Alignment Requirements**:
   - Some processors require aligned memory access for optimal vector performance
   - The VECTOR_CTRL_ALIGNED/VECTOR_CTRL_UNALIGNED parameters control alignment behavior

5. **Performance Considerations**:
   - Vector operations are significantly faster than scalar loops on supported hardware
   - The performance gap varies widely depending on the processor capabilities
   - For performance-critical code, consider processor-specific optimizations using conditional assembly

The COIL processor is responsible for mapping vector operations to the most efficient implementation for the target hardware while maintaining consistent behavior across different processors.