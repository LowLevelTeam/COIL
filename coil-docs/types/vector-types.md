# Vector Types in COIL

## Overview

Vector types in COIL enable SIMD (Single Instruction, Multiple Data) operations, allowing the same operation to be performed on multiple data elements simultaneously. 

Importantly, vector operations are **universal** across all processor types (not processor-specific). The COIL processor maps them to native SIMD instructions when available or implements them using scalar operations when hardware support is missing.

## Vector Type Definitions

COIL provides three primary vector types:

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x30    | TYPE_V128 | 16 | 128-bit vector |
| 0x31    | TYPE_V256 | 32 | 256-bit vector |
| 0x32    | TYPE_V512 | 64 | 512-bit vector |

## Element Types

Vector types are parameterized by their element type, specified with the following syntax:

```
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats (4 elements)
TYPE_V256=TYPE_INT16   ; 256-bit vector of 16-bit integers (16 elements)
TYPE_V512=TYPE_UNT8    ; 512-bit vector of 8-bit unsigned integers (64 elements)
```

## Number of Elements

The number of elements in a vector depends on the vector size and element type:

| Vector Size | Element Type | Number of Elements |
|-------------|--------------|-------------------|
| 128-bit     | 8-bit        | 16 elements       |
| 128-bit     | 16-bit       | 8 elements        |
| 128-bit     | 32-bit       | 4 elements        |
| 128-bit     | 64-bit       | 2 elements        |
| 256-bit     | 32-bit       | 8 elements        |
| 512-bit     | 32-bit       | 16 elements       |

## Vector Operations

COIL supports several types of vector operations:

### Basic Arithmetic

Standard arithmetic operations work element-wise on vectors:

```
ADD result, vector1, vector2   ; Element-wise addition
SUB result, vector1, vector2   ; Element-wise subtraction
MUL result, vector1, vector2   ; Element-wise multiplication
DIV result, vector1, vector2   ; Element-wise division
```

### Vector-Specific Operations

Special vector operations:

```
VLOAD dest, src                ; Load vector from memory
VSTORE dest, src               ; Store vector to memory
VEXTRACT dest, src, index      ; Extract scalar element
VINSERT dest, src, value, idx  ; Insert scalar element
VSHUFFLE dest, src, mask       ; Rearrange elements
VCMP dest, left, right, type   ; Compare elements
VDOT dest, left, right         ; Dot product
VHSUM dest, src                ; Horizontal sum of elements
```

### Scalar-Vector Operations

Operations between scalars and vectors:

```
MUL result, vector, scalar     ; Scalar multiplication (all elements)
ADD result, vector, scalar     ; Add scalar to all elements
```

## Implementation Approach

Vector operations are implemented based on available hardware:

1. **On SIMD-capable CPUs**:
   - Maps to native SIMD instructions (SSE, AVX, NEON, etc.)
   - TYPE_V128 maps to 128-bit registers (XMM on x86)
   - TYPE_V256 maps to 256-bit registers (YMM on x86)
   - TYPE_V512 maps to 512-bit registers (ZMM on x86)

2. **On processors without native SIMD**:
   - Operations are automatically decomposed into scalar operations
   - Each element is processed individually by the COIL processor

This abstraction allows COIL code to benefit from SIMD acceleration when available without requiring architecture-specific code.

## Vector Type Compatibility

Vector types are compatible under these conditions:

1. Same vector size and element type
2. Same vector size with compatible element types 
3. Different vector size but same element type, when the operation supports size conversion

## Examples

### Vector Initialization

```
; Initialize vector with literal values
VAR TYPE_V128=TYPE_INT32, vector, (1, 2, 3, 4)

; Initialize vector from aligned memory
VLOAD vector, [array_ptr]

; Initialize vector with broadcast value (same value in all elements)
VLOAD vector, [scalar_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST
```

### Vector Math

```
; Element-wise vector addition
ADD result, vector1, vector2

; Vector scaling (multiply by scalar)
MUL scaled, vector, 2.0

; Dot product calculation
VDOT result, vector1, vector2
```