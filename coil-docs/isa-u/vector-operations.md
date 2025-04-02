# Vector Operations

## Overview

Vector operations provide specialized instructions for working with vectors, arrays, matrices, and tensors. These operations enable efficient manipulation of multiple data elements, supporting both element-wise operations and higher-level mathematical computations.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x80   | VGET     | Get vector element |
| 0x81   | VSET     | Set vector element |
| 0x82   | VDOT     | Vector dot product |
| 0x83   | VCROSS   | Vector cross product |
| 0x84   | VNORM    | Vector normalization |
| 0x85   | VLEN     | Vector length/magnitude |
| 0x86   | VSHUF    | Vector shuffle elements |
| 0x87   | VEXTRACT | Extract subvector |
| 0x88   | VINSERT  | Insert subvector |

## Detailed Specifications

### VGET (0x80)

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
- Sets flags based on the extracted value

### VSET (0x81)

Sets an element in a vector.

**Encoding:**
```
[0x81][0x03][vector: Operand][index: Operand][value: Operand]
```

**Behavior:**
- Replaces a single element in the vector at the specified index
- Vector must be a writable location
- Index must be an integral type
- Value type must be compatible with the vector element type
- Sets flags based on the operation success

### VDOT (0x82)

Calculates the dot product of vectors.

**Encoding:**
```
// Two-operand form (accumulate in dest)
[0x82][0x02][dest: Operand][src: Operand]

// Three-operand form
[0x82][0x03][dest: Operand][src1: Operand][src2: Operand]
```

**Behavior:**
- Calculates the dot product (scalar product) of vectors
- For two-operand form: dest = dot(dest, src)
- For three-operand form: dest = dot(src1, src2)
- Vectors must have the same number of elements
- Result is a scalar value
- Sets flags based on the result (zero, sign)

### VCROSS (0x83)

Calculates the cross product of 3D vectors.

**Encoding:**
```
[0x83][0x03][dest: Operand][src1: Operand][src2: Operand]
```

**Behavior:**
- Calculates the cross product of two 3D vectors
- Input vectors must have exactly 3 elements
- Result is a 3D vector
- Sets flags based on the result (zero)

### VNORM (0x84)

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
- Sets flags based on the operation success

### VLEN (0x85)

Calculates the length (magnitude) of a vector.

**Encoding:**
```
[0x85][0x02][dest: Operand][vector: Operand]
```

**Behavior:**
- Calculates the Euclidean norm (magnitude) of the vector
- Result is a scalar value (floating-point)
- Sets flags based on the result (zero, sign)

### VSHUF (0x86)

Shuffles elements of a vector according to a mask.

**Encoding:**
```
[0x86][0x03][dest: Operand][vector: Operand][mask: Operand]
```

**Behavior:**
- Rearranges elements from the source vector according to indices in the mask
- Mask must be a vector of integral values
- Each element in the mask specifies the source element index
- Out-of-range indices yield implementation-defined behavior
- Sets flags based on the operation success

### VEXTRACT (0x87)

Extracts a contiguous subvector.

**Encoding:**
```
[0x87][0x04][dest: Operand][vector: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Extracts a contiguous sequence of elements from the source vector
- Start specifies the first element index (integral value)
- Length specifies the number of elements to extract (integral value)
- Destination must be able to hold the specified number of elements
- Sets flags based on the operation success
- Out-of-range extraction raises an exception or has implementation-defined behavior

### VINSERT (0x88)

Inserts a subvector into another vector.

**Encoding:**
```
[0x88][0x04][dest: Operand][subvec: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Inserts a sequence of elements from subvec into dest starting at the specified index
- Start specifies the index in dest where insertion begins (integral value)
- Length specifies the number of elements to insert (integral value)
- Destination must be a writable vector with sufficient space
- Sets flags based on the operation success
- Out-of-range insertion raises an exception or has implementation-defined behavior

## Vector Types

Vector operations work with several types of multi-element data structures:

| Vector Type | Description |
|-------------|-------------|
| Fixed-width | Standard SIMD vectors (V128, V256, V512) |
| Custom-width | User-defined vector (CVEC) |
| Matrix | 2D arrays represented as vectors of vectors |
| Tensor | Multi-dimensional arrays |

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Type System](../types/type-system.md) - Complete type system reference
- [Matrix Operations](./matrix-operations.md) - Matrix-specific operations