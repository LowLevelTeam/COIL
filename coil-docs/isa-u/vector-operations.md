# multi scalar type Operations

## Overview

multi scalar type operations provide specialized instructions for working with multi scalar types, arrays, matrices, and tensors. These operations enable efficient manipulation of multiple data elements, supporting both element-wise operations and higher-level mathematical computations.

If you are looking for a multi scalar type basic addition then this is not the place. Vectors, Matricies, Tensors, etc... can all be used with the default basic arithmetic instructions utilizing type inference even allowing for something like mul vec, scalar. Here are all operations that would not make sense to do on a single scalar value.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x80   | GETE     | Get element |
| 0x81   | SETE     | Set element |
| 0x82   | DOT      | multi scalar type dot product |
| 0x83   | CROSS    | multi scalar type cross product |
| 0x84   | NORM     | multi scalar type normalization |
| 0x85   | LEN      | multi scalar type length/magnitude |
| 0x86   | SHUF     | multi scalar type shuffle elements |
| 0x87   | EXTRACT  | Extract submulti scalar type |
| 0x88   | INSERT   | Insert submulti scalar type |
| 0x89   | TRANS    | Matrix transpose |
| 0x8A   | INV      | Matrix inverse |
| 0x8B   | DET      | Matrix determinant |
| 0x8C   | ROW      | Get/set row |
| 0x8D   | COL      | Get/set column |
| 0x8E   | DIAG     | Get/set diagonal |

## Detailed Specifications

### GET (0x80)

Retrieves an element from a multi scalar type.

**Encoding:**
```
[0x80][0x03][dest: Operand][multi scalar type: Operand][index: Operand]
```

**Behavior:**
- Extracts a single element from the multi scalar type at the specified index
- Copies the value to the destination operand
- Destination type must be compatible with the multi scalar type element type
- Index must be an integral type
- Sets flags based on the extracted value

### SET (0x81)

Sets an element in a multi scalar type.

**Encoding:**
```
[0x81][0x03][multi scalar type: Operand][index: Operand][value: Operand]
```

**Behavior:**
- Replaces a single element in the multi scalar type at the specified index
- multi scalar type must be a writable location
- Index must be an integral type
- Value type must be compatible with the multi scalar type element type
- Sets flags based on the operation success

### DOT (0x82)

Calculates the dot product of multi scalar types.

**Encoding:**
```
// Two-operand form (accumulate in dest)
[0x82][0x02][dest: Operand][src: Operand]

// Three-operand form
[0x82][0x03][dest: Operand][src1: Operand][src2: Operand]
```

**Behavior:**
- Calculates the dot product (scalar product) of multi scalar types
- For two-operand form: dest = dot(dest, src)
- For three-operand form: dest = dot(src1, src2)
- multi scalar types must have the same number of elements
- Result is a scalar value
- Sets flags based on the result (zero, sign)

### CROSS (0x83)

Calculates the cross product of 3D multi scalar types.

**Encoding:**
```
[0x83][0x03][dest: Operand][src1: Operand][src2: Operand]
```

**Behavior:**
- Calculates the cross product of two 3D multi scalar types
- Input multi scalar types must have exactly 3 elements
- Result is a 3D multi scalar type
- Sets flags based on the result (zero)

### NORM (0x84)

Normalizes a multi scalar type to unit length.

**Encoding:**
```
// Two-operand form
[0x84][0x02][dest: Operand][src: Operand]

// One-operand form (in-place)
[0x84][0x01][multi scalar type: Operand]
```

**Behavior:**
- Normalizes the multi scalar type to have a magnitude of 1.0
- Divides each element by the multi scalar type magnitude
- For zero multi scalar types, behavior is implementation-defined
- Sets flags based on the operation success

### LEN (0x85)

Calculates the length (magnitude) of a multi scalar type.

**Encoding:**
```
[0x85][0x02][dest: Operand][multi scalar type: Operand]
```

**Behavior:**
- Calculates the Euclidean norm (magnitude) of the multi scalar type
- Result is a scalar value (floating-point)
- Sets flags based on the result (zero, sign)

### SHUF (0x86)

Shuffles elements of a multi scalar type according to a mask.

**Encoding:**
```
[0x86][0x03][dest: Operand][multi scalar type: Operand][mask: Operand]
```

**Behavior:**
- Rearranges elements from the source multi scalar type according to indices in the mask
- Mask must be a multi scalar type of integral values
- Each element in the mask specifies the source element index
- Out-of-range indices yield implementation-defined behavior
- Sets flags based on the operation success

### EXTRACT (0x87)

Extracts a contiguous submulti scalar type.

**Encoding:**
```
[0x87][0x04][dest: Operand][multi scalar type: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Extracts a contiguous sequence of elements from the source multi scalar type
- Start specifies the first element index (integral value)
- Length specifies the number of elements to extract (integral value)
- Destination must be able to hold the specified number of elements
- Sets flags based on the operation success
- Out-of-range extraction raises an exception or has implementation-defined behavior

### INSERT (0x88)

Inserts a submulti scalar type into another multi scalar type.

**Encoding:**
```
[0x88][0x04][dest: Operand][subvec: Operand][start: Operand][length: Operand]
```

**Behavior:**
- Inserts a sequence of elements from subvec into dest starting at the specified index
- Start specifies the index in dest where insertion begins (integral value)
- Length specifies the number of elements to insert (integral value)
- Destination must be a writable multi scalar type with sufficient space
- Sets flags based on the operation success
- Out-of-range insertion raises an exception or has implementation-defined behavior

## multi scalar type Types

multi scalar type operations work with several types of multi-element data structures:

| Multi Scalar Types | Description |
|-------------|-------------|
| Fixed-width | Standard SIMD multi scalar types (V128, V256, V512) |
| Custom-width | User-defined multi scalar type (CVEC) |
| Matrix | 2D arrays represented as multi scalar types of multi scalar types |
| Tensor | Multi-dimensional arrays |

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Mathematical computation instructions
- [Type System](../types/type-system.md) - Complete type system reference
- [Matrix Operations](./matrix-operations.md) - Matrix-specific operations