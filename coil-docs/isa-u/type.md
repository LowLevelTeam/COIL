# Type Operations

## Overview

Type operations provide instructions for defining, querying, and manipulating types at compile-time and runtime. These operations enable dynamic type handling, reflection capabilities, and efficient memory management in COIL programs.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xE0   | TYPE     | Define a Type |
| 0xE1   | SIZEOF   | Get size of Type |
| 0xE2   | ALIGNOF  | Get natural alignment of type |
| 0xE3   | DTYPE    | Delete type definition |

## Detailed Specifications

### TYPE (0xE0)

Defines a type to be usable through a type ID.

**Encoding:**
```
[0xE0][0x02][typeid: uint64_t][type: Type]
```

**Behavior:**
- Creates a named type definition referenced by the specified type ID
- Equivalent to a `typedef` in C-like languages
- Type ID must be unique within the current compilation unit
- Type can be any valid COIL type definition
- Named types can be used anywhere a type is expected
- Allows reuse of complex type definitions without repetition

**Example:**
```
// Define a 3×3 matrix of FP32 as type ID 1
TYPE 1, MAT FP32 3 3

// Define a structure as type ID 2
TYPE 2, STRUCT 3 [0x0001 INT32] [0x0002 FP64] [0x0003 V128 FP32]
```

### SIZEOF (0xE1)

Gets the size of a type in bytes.

**Encoding:**
```
[0xE1][0x01][type: Type]
```

**Behavior:**
- Returns the size in bytes required to store a value of the specified type
- Result can be used as an immediate value in expressions
- Takes into account alignment requirements specified by the target architecture
- For variable-length types, returns the minimum fixed size excluding variable portions
- For zero-sized types, returns 0

**Example:**
```
// Get size of INT32
MOV size, SIZEOF INT32  // size = 4

// Get size of a structure
MOV struct_size, SIZEOF STRUCT 2 [0x0001 INT32] [0x0002 FP64]  // struct_size = 16 (with padding)
```

### ALIGNOF (0xE2)

Gets the natural alignment requirement of a type in bytes.

**Encoding:**
```
[0xE2][0x01][type: Type]
```

**Behavior:**
- Returns the natural alignment requirement in bytes for the specified type
- Result can be used as an immediate value in expressions
- Alignment is determined by the largest alignment requirement of any component
- Takes into account the target architecture's alignment rules
- Always returns a power of 2 value (1, 2, 4, 8, 16, etc.)

**Example:**
```
// Get alignment of FP64
MOV align, ALIGNOF FP64  // align = 8

// Get alignment of a structure
MOV struct_align, ALIGNOF STRUCT 2 [0x0001 INT8] [0x0002 FP64]  // struct_align = 8
```

### DTYPE (0xE3)

Deletes a type definition by ID.

**Encoding:**
```
[0xE3][0x01][typeid: uint64_t]
```

**Behavior:**
- Removes the type definition associated with the specified type ID
- Any subsequent references to this type ID are invalid
- Does not affect variables or values that already use this type
- Primarily used to free compilation resources or avoid naming conflicts
- Can only delete types defined within the current compilation unit or scope

**Example:**
```
// Define a type
TYPE 1, MAT FP32 3 3

// Use the type
VAR matrix_var, 1

// Delete the type definition
DTYPE 1
```

## Type System Integration

Type operations integrate with the COIL type system in the following ways:

1. **Compile-Time Evaluation**: Type operations are typically evaluated at compile time
2. **Runtime Type Information**: Type information can be accessed at runtime
3. **Dynamic Type Creation**: Types can be defined dynamically in some execution environments
4. **Reflection Support**: Type operations enable basic reflection capabilities

## Memory Model Impact

Type operations affect the memory model in these ways:

1. **Size Determination**: SIZEOF determines storage allocation size
2. **Alignment Control**: ALIGNOF controls memory alignment requirements
3. **Type Safety**: Named types enhance type checking and safety
4. **Optimization**: Type information enables compiler optimizations

## Related Components

- [Type System](../types/type-system.md) - Complete type system reference
- [Extended Types](../types/extended-types.md) - Matrix and tensor type specifications
- [Memory Operations](./memory-operations.md) - Memory management instructions