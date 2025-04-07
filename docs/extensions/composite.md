# Composite Types Extension

## Overview

The Composite Types Extension enhances COIL's type system with structures, unions, and type aliases. This extension enables organization of related data into cohesive units, facilitating more intuitive and maintainable code.

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

Example: Structure with INT32 at offset 0 and FP32 at offset 4
```
[0xA0][0x00][0x0002][0x0000][0x04][0x00][0x0004][0x10][0x00]
```

### Packed Structure Type (0xA1)

Packed structures eliminate padding:

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

Example: Union with INT32 and FP32 alternatives
```
[0xA2][0x00][0x0002][0x0000][0x04][0x00][0x0001][0x10][0x00]
```

### Type Alias (0xA3)

Type aliases create named references to existing types:

```
[0xA3][type_extension][aliased_type]
```

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

### Union Layout

Unions overlay all members at the same address:

1. **Size**: Size of the largest member
2. **Alignment**: Maximum alignment of any member
3. **Access**: Only one member should be accessed at a time

## Field Access Instructions

### Member Access (0xE5-0xE7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xE5   | MEMGET   | Get structure member | 3 |
| 0xE6   | MEMSET   | Set structure member | 3 |
| 0xE7   | MEMADDR  | Get member address | 3 |

### MEMGET (0xE5)

```
MEMGET destination, structure, member_id
```

- `destination`: Target for the extracted value
- `structure`: Source structure or union
- `member_id`: Member identifier (uint16_t)

### MEMSET (0xE6)

```
MEMSET structure, member_id, value
```

- `structure`: Target structure or union
- `member_id`: Member identifier (uint16_t)
- `value`: Value to store

### MEMADDR (0xE7)

```
MEMADDR destination, structure, member_id
```

- `destination`: Target for the member address
- `structure`: Source structure or union
- `member_id`: Member identifier (uint16_t)

## Type Definition Instructions

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xE8   | STRDEF   | Define structure type | 2+ |
| 0xE9   | UNIDEF   | Define union type | 2+ |
| 0xEA   | TYPEAL   | Define type alias | 2 |

## Type Compatibility Rules

1. **Structure Assignment**:
   - Structures are compatible for assignment if they have the same members in the same order with compatible types
   - Packed structures are not assignment-compatible with regular structures

2. **Union Assignment**:
   - Unions are compatible for assignment if they have the same members with compatible types
   - The member being accessed must be the same on both sides

3. **Type Alias Compatibility**:
   - A type alias is compatible with its underlying type
   - Type aliases are compatible if their underlying types are compatible

## Usage Examples

### Structure Example

```
; Define a point structure
STRDEF point_t, 2, i32, i32

; Create and initialize a point
VAR p, point_t
MEMSET p, 0, 10      ; p.x = 10
MEMSET p, 1, 20      ; p.y = 20

; Access structure members
i32 x
MEMGET x, p, 0       ; x = p.x
```

### Union Example

```
; Define a value union
UNIDEF value_t, 3, i32, f32, i64

; Create a union and set as integer
VAR v, value_t
MEMSET v, 0, 42      ; v.i = 42

; Access as float (reinterprets the bits)
f32 f
MEMGET f, v, 1       ; f = v.f (bit reinterpretation of 42)
```

### Type Alias Example

```
; Define a type alias for a complex structure
TYPEAL point3d_t, STRUCT<i32, i32, i32>

; Use the alias
VAR p3d, point3d_t
```