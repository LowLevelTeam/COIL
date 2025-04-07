# Composite Types Extension

## Overview

The Composite Types Extension enhances COIL's type system with complex data structures, including structures, unions, and type aliases. This extension enables organization of related data into cohesive units, facilitating more intuitive and maintainable code.

## Extension Information

- **Extension ID**: 0x02
- **Extension Name**: Composite
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Type Definitions

### Structure Type (0xA0)

Structures represent collections of named fields with fixed offsets:

```
[0xA0][type_extension][member_count: uint16_t][[member_offset: uint16_t][member_type]...]
```

Where:
- `type_extension`: Type modifier flags
- `member_count`: Number of fields in the structure
- `member_offset`: Byte offset from structure start
- `member_type`: Type of the member field

Example: Structure with INT32 at offset 0 and FP32 at offset 4
```
[0xA0][0x00][0x0002][0x0000][0x04][0x00][0x0004][0x10][0x00]
```

### Packed Structure Type (0xA1)

Packed structures are similar to regular structures but eliminate padding:

```
[0xA1][type_extension][member_count: uint16_t][[member_offset: uint16_t][member_type]...]
```

Example: Packed structure with INT8 at offset 0 and INT16 at offset 1
```
[0xA1][0x00][0x0002][0x0000][0x00][0x00][0x0001][0x02][0x00]
```

### Union Type (0xA2)

Unions represent overlapping fields sharing the same memory:

```
[0xA2][type_extension][member_count: uint16_t][[member_id: uint16_t][member_type]...]
```

Where:
- `member_id`: Unique identifier for the union member
- Other fields are as in structure types

Example: Union with INT32 and FP32 alternatives
```
[0xA2][0x00][0x0002][0x0000][0x04][0x00][0x0001][0x10][0x00]
```

### Type Alias (0xA3)

Type aliases create named references to existing types:

```
[0xA3][type_extension][aliased_type]
```

Where:
- `aliased_type`: The underlying type being aliased

Example: Alias for INT32
```
[0xA3][0x00][0x04][0x00]
```

## Memory Layout

### Structure Layout

Structures follow these layout rules:

1. **Alignment**: Each member is aligned according to its natural alignment requirements
2. **Padding**: Automatic padding inserted between members to maintain alignment
3. **Total Size**: Structure size is rounded up to its alignment requirement

Example:
```
struct Example {
    int8_t  a;    // Offset 0, size 1
    // 1 byte padding
    int16_t b;    // Offset 2, size 2
    int32_t c;    // Offset 4, size 4
}; // Total size: 8 bytes
```

### Packed Structure Layout

Packed structures minimize space by eliminating padding:

1. **No Padding**: Members are placed immediately after each other
2. **Alignment**: Structure has alignment of 1 byte
3. **Access**: May require unaligned memory access on some architectures

Example:
```
packed struct Example {
    int8_t  a;    // Offset 0, size 1
    int16_t b;    // Offset 1, size 2 (potentially unaligned)
    int32_t c;    // Offset 3, size 4 (potentially unaligned)
}; // Total size: 7 bytes
```

### Union Layout

Unions overlay all members at the same address:

1. **Size**: Size of the largest member
2. **Alignment**: Maximum alignment of any member
3. **Access**: Only one member should be accessed at a time

Example:
```
union Example {
    int32_t i;    // Offset 0, size 4
    float   f;    // Offset 0, size 4
    int8_t  b[4]; // Offset 0, size 4
}; // Total size: 4 bytes
```

## Field Access Instructions

### Member Access (0xE5-0xE7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xE5   | MEMGET   | Get structure member | 3 | Composite |
| 0xE6   | MEMSET   | Set structure member | 3 | Composite |
| 0xE7   | MEMADDR  | Get member address | 3 | Composite |

## Detailed Instruction Behaviors

### MEMGET (0xE5)

The member get instruction retrieves a value from a structure or union member:

```
MEMGET destination, structure, member_id
```

- `destination`: Target for the extracted value
- `structure`: Source structure or union
- `member_id`: Member identifier (uint16_t)

Behavior:
1. Computes the address of the specified member
2. Loads the value from that address
3. Stores the value in the destination
4. No flags are affected

### MEMSET (0xE6)

The member set instruction stores a value into a structure or union member:

```
MEMSET structure, member_id, value
```

- `structure`: Target structure or union
- `member_id`: Member identifier (uint16_t)
- `value`: Value to store

Behavior:
1. Computes the address of the specified member
2. Stores the value at that address
3. No flags are affected

### MEMADDR (0xE7)

The member address instruction computes the address of a structure or union member:

```
MEMADDR destination, structure, member_id
```

- `destination`: Target for the member address
- `structure`: Source structure or union
- `member_id`: Member identifier (uint16_t)

Behavior:
1. Computes the address of the specified member
2. Stores the address in the destination
3. No flags are affected

## Type Definition Instructions

### Type Operations (0xE8-0xEA)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xE8   | STRDEF   | Define structure type | 2+ | Composite |
| 0xE9   | UNIDEF   | Define union type | 2+ | Composite |
| 0xEA   | TYPEAL   | Define type alias | 2 | Composite |

## Type Compatibility Rules

Composite types follow these compatibility rules:

1. **Structure Assignment**:
   - Structures are compatible for assignment if they have the same members in the same order with compatible types
   - Packed structures are not assignment-compatible with regular structures

2. **Union Assignment**:
   - Unions are compatible for assignment if they have the same members with compatible types
   - The member being accessed must be the same on both sides

3. **Type Alias Compatibility**:
   - A type alias is compatible with its underlying type
   - Type aliases are compatible if their underlying types are compatible

## Implementation Requirements

Implementations of the Composite Types Extension must:

1. **Type Support**:
   - Support all structure, union, and alias types
   - Handle all type extensions on these types

2. **Memory Layout**:
   - Correctly implement structure and union memory layouts
   - Respect alignment requirements
   - Handle packed structures appropriately

3. **Access Instructions**:
   - Implement all member access instructions
   - Handle misaligned access if required by the target architecture

4. **Type Definition**:
   - Support dynamic type definition
   - Validate type definitions for correctness
   - Handle nested composite types

5. **Performance**:
   - Optimize access patterns for the target architecture
   - Minimize overhead for member access
