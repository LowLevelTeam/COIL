# COIL Type System

## Overview

The COIL Type System defines how data types are represented, manipulated, and stored within the COIL architecture. The type system is fundamental to COIL's type-determined instruction behavior pattern, where the same opcode can produce different operations based on operand types.

## Core Design Principles

1. **Explicit Typing**: All data has an explicit type
2. **Type-Determined Behavior**: Instructions adapt based on operand types
3. **Unified Representation**: Consistent binary encoding for all types
4. **Extensibility**: Support for both built-in and custom types
5. **Portability**: Types have consistent semantics across platforms

## Type Representation

Every type in COIL is represented by:

```
[type opcode: uint8_t] [type extension: uint8_t] [type data: optional ...] [data: optional ...]
```

The type extension byte is always present, even if all flags are 0.

## Type Categories

### Fixed-Width Integral Types (0x00-0x0F)

Standard integer types with fixed bit widths:

| Opcode | Name   | Description         | Size (bytes) | Binary Representation        |
|--------|--------|---------------------|--------------|------------------------------|
| 0x00   | INT8   | 8-bit signed int    | 1            | [0x00][ext]                  |
| 0x01   | UNT8   | 8-bit unsigned int  | 1            | [0x01][ext]                  |
| 0x02   | INT16  | 16-bit signed int   | 2            | [0x02][ext]                  |
| 0x03   | UNT16  | 16-bit unsigned int | 2            | [0x03][ext]                  |
| 0x04   | INT32  | 32-bit signed int   | 4            | [0x04][ext]                  |
| 0x05   | UNT32  | 32-bit unsigned int | 4            | [0x05][ext]                  |
| 0x06   | INT64  | 64-bit signed int   | 8            | [0x06][ext]                  |
| 0x07   | UNT64  | 64-bit unsigned int | 8            | [0x07][ext]                  |

Fixed-width integral types have no additional type data.

### Fixed-Width Floating-Point Types (0x10-0x1F)

Standard floating-point types:

| Opcode | Name   | Description          | Size (bytes) | Binary Representation        |
|--------|--------|----------------------|--------------|------------------------------|
| 0x10   | FP32   | 32-bit float         | 4            | [0x10][ext]                  |
| 0x11   | FP64   | 64-bit double        | 8            | [0x11][ext]                  |

Fixed-width floating-point types have no additional type data.

### Fixed-Width Vector Types (0x20-0x2F)

SIMD-friendly vector types:

| Opcode | Name   | Description          | Size (bytes) | Binary Representation                   |
|--------|--------|----------------------|--------------|-----------------------------------------|
| 0x20   | V128   | 128-bit vector       | 16           | [0x20][ext][element_type]              |
| 0x21   | V256   | 256-bit vector       | 32           | [0x21][ext][element_type]              |
| 0x22   | V512   | 512-bit vector       | 64           | [0x22][ext][element_type]              |

Vector types require an element type specification, which is a complete type specification for the element type. For example, a V128 of 32-bit integers would be represented as:

```
[0x20][ext][0x04][0x00]
```

### Complex Types (0x30-0x3F)

User-defined bit-width types and advanced data structures:

| Opcode | Name      | Description                 | Type Data Format                    | Binary Representation Example                                |
|--------|-----------|-----------------------------|------------------------------------|--------------------------------------------------------------|
| 0x30   | CINT      | Custom-width signed int     | [bit size: uint16_t]               | [0x30][ext][0x0020] (32-bit custom int)                      |
| 0x31   | CUNT      | Custom-width unsigned int   | [bit size: uint16_t]               | [0x31][ext][0x0040] (64-bit custom uint)                     |
| 0x32   | CFP       | Custom-width float          | [mantissa: uint16_t][exp: uint16_t] | [0x32][ext][0x0017][0x0008] (24-bit float: 23-bit mantissa, 8-bit exponent) |
| 0x33   | CVEC      | Custom vector               | [element type: Type][count: uint16_t] | [0x33][ext][0x04][0x00][0x0004] (Vector of 4 INT32s)        |
| 0x34   | MAT       | General matrix              | [element type: Type][rows: uint16_t][cols: uint16_t] | [0x34][ext][0x10][0x00][0x0003][0x0003] (3×3 matrix of FP32) |
| 0x35   | SQMAT     | Square matrix               | [element type: Type][size: uint16_t] | [0x35][ext][0x10][0x00][0x0004] (4×4 matrix of FP32)        |
| 0x36   | DIAGMAT   | Diagonal matrix             | [element type: Type][size: uint16_t] | [0x36][ext][0x11][0x00][0x0003] (3×3 diagonal matrix of FP64) |
| 0x37   | TENSOR    | General tensor              | [element type: Type][rank: uint8_t][dim1: uint16_t]...[dimN: uint16_t] | [0x37][ext][0x04][0x00][0x03][0x0002][0x0003][0x0004] (2×3×4 tensor of INT32) |
| 0x38   | SYMTENSOR | Symmetric tensor            | [element type: Type][rank: uint8_t][dim: uint16_t] | [0x38][ext][0x10][0x00][0x03][0x0003] (3×3×3 symmetric tensor of FP32) |

### Composite Types (0xA0-0xAF)

Compound types that group other types:

| Opcode | Name   | Description                                | Type Data Format                                     | Binary Representation Example                     |
|--------|--------|--------------------------------------------|-----------------------------------------------------|---------------------------------------------------|
| 0xA0   | STRUCT | Fields at fixed offsets                    | [field count: uint16_t][[member_id: uint16_t][type definition],...] | [0xA0][ext][0x0002][0x0001][0x04][0x00][0x0002][0x10][0x00] (Struct with INT32 and FP32) |
| 0xA1   | PACK   | Packed structure (no padding)              | [field count: uint16_t][[member_id: uint16_t][type definition],...] | [0xA1][ext][0x0002][0x0001][0x02][0x00][0x0002][0x01][0x00] (Packed struct with INT16 and UNT8) |
| 0xA2   | UNION  | Overlapping fields                         | [field count: uint16_t][[member_id: uint16_t][type definition],...] | [0xA2][ext][0x0002][0x0001][0x06][0x00][0x0002][0x20][0x00][0x10][0x00] (Union of INT64 and V128 of FP32) |
| 0xA3   | ALIAS  | Type alias                                 | [typeid: uint64_t]                                 | [0xA3][ext][0x0000000000000001] (Alias to type ID 1) |

The field_count must be between 1 and 65535. Each field has a member_id (1-65535) and a complete type definition.

### Optimized Types (0xF0-0xF9)

Special-purpose optimized types:

| Opcode | Name    | Description                  | Data Format         | Binary Representation Example    |
|--------|---------|------------------------------|---------------------|----------------------------------|
| 0xF0   | BIT     | Single bit (boolean)         | N/A                 | [0xF0][ext]                      |
| 0xF1   | VARID   | Variable identifier          | [var id: uint64_t]  | [0xF1][ext][0x0000000000000001] (Variable 1) |
| 0xF2   | SYMBOL  | Symbol reference             | [symbol id: uint64_t] | [0xF2][ext][0x0000000000000002] (Symbol 2) |
| 0xF3   | DEFID   | Definition Identifier        | [definition id: uint64_t] | [0xF3][ext][0x0000000000000003] (Definition 3) |
| 0xF4   | STRING  | String reference             | [string id: uint64_t] | [0xF4][ext][0x0000000000000004] (String 4) |
| 0xF9   | REG     | Hardware register reference  | [register: uint16_t] | [0xF9][ext][0x0001] (Register 1) |

### Special Types (0xFA-0xFF)

Parameter and control types:

| Opcode | Name     | Description                       | Binary Representation |
|--------|----------|-----------------------------------|------------------------|
| 0xE1   | SIZEOF   | Reference to sizeof instruction   | [0xE1][ext]           |
| 0xE2   | ALIGNOF  | Reference to alignof instruction  | [0xE2][ext]           |
| 0xFA   | PARAM4   | Parameter type 4                  | [0xFA][ext][param_value] |
| 0xFB   | PARAM3   | Parameter type 3                  | [0xFB][ext][param_value] |
| 0xFC   | PARAM2   | Parameter type 2                  | [0xFC][ext][param_value] |
| 0xFD   | PARAM1   | Parameter type 1                  | [0xFD][ext][param_value] |
| 0xFE   | PARAM0   | Parameter type 0                  | [0xFE][ext][param_value] |
| 0xFF   | VOID     | No type/value                     | [0xFF][ext]           |

## Type Extensions

Type extensions are bitmasks that modify type behavior:

| Bit     | Name       | Description                                  |
|---------|------------|----------------------------------------------|
| (1 << 0)| CONST      | Value cannot be modified                     |
| (1 << 1)| VOLATILE   | Value may change unexpectedly                |
| (1 << 2)| ATOMIC     | Operations must be atomic                    |
| (1 << 3)| SATURATE   | Cap at largest value instead of overflow     |
| (1 << 4)| IMMEDIATE  | Value is embedded in instruction             |
| (1 << 5)| DEFINITION | Value is a compile time definition           |
| (1 << 6)| VARIABLE   | Value is a variable reference                |
| (1 << 7)| SYMBOL     | Value is a symbol reference                  |

## Type Values

The value portion depends on the type extension flags:

1. If IMMEDIATE is set: The literal value follows, with size determined by the type
   - Example (INT32 with value 42): `[0x04][0x10][0x0000002A]`

2. If VARIABLE is set: A uint64_t variable ID follows
   - Example (INT32 variable with ID 1): `[0x04][0x40][0x0000000000000001]`

3. If SYMBOL is set: A uint64_t symbol ID follows
   - Example (Symbol reference with ID 5): `[0xF2][0x80][0x0000000000000005]`

4. If DEFINITION is set: A uint64_t definition ID follows
   - Example (Definition with ID 3): `[0xF3][0x20][0x0000000000000003]`

5. If no value flags are set: No value data is present
   - Example (INT32 type only): `[0x04][0x00]`

## Matrix Element Types and Layout

Matrices store element types in row-major order by default. The element type is specified once as part of the matrix type definition:

```
[0x34][ext][element_type][rows][cols]
```

For example, a 3×3 matrix of 32-bit floating-point values would be:

```
[0x34][0x00][0x10][0x00][0x0003][0x0003]
```

When storing matrix data as an immediate value, the elements are laid out in row-major order:

```
[row0col0][row0col1][row0col2][row1col0]...
```

## Tensor Dimensions

Tensors specify dimensions as a sequence of uint16_t values after the rank:

```
[0x37][ext][element_type][rank: uint8_t][dim1][dim2]...[dimN]
```

The maximum supported rank is 8 (8-dimensional tensor). Each dimension can range from 1 to 65,535.

## Nesting Limits

Type nesting follows these limits:

1. **Maximum Nesting Depth**: 8 levels for composite types
2. **Maximum Vector Nesting**: 3 levels (vector of vector of vector)
3. **Maximum Tensor Rank**: 8 dimensions

## Type Conversion Rules

1. **Widening Conversions**: Smaller types to larger types preserve value
   - INT8 → INT16 → INT32 → INT64
   - FP32 → FP64

2. **Narrowing Conversions**: May lose precision or range
   - INT64 → INT32 → INT16 → INT8 (truncation)
   - FP64 → FP32 (precision loss)

3. **Domain Conversions**: Between different domains follow IEEE rules
   - INT32 → FP32 (exact for values within range)
   - FP32 → INT32 (truncation toward zero)

4. **Vector Conversions**: Require matching element counts
   - V128(INT32) → V128(FP32) (element-wise conversion)

5. **Matrix Conversions**: Follow mathematical rules for compatible dimensions
   - MAT(3×3, INT32) → MAT(3×3, FP32) (element-wise conversion)

## Memory Alignment

Types have natural alignment requirements based on their size:

| Type Size (bytes) | Natural Alignment (bytes) |
|-------------------|---------------------------|
| 1                 | 1                         |
| 2                 | 2                         |
| 4                 | 4                         |
| 8                 | 8                         |
| 16                | 16                        |
| 32+               | 16 (platform-specific)    |

For composite types, the alignment is determined by the largest alignment required by any member.

## Complete Binary Examples

### 32-bit Signed Integer

```
[0x04][0x00]  // INT32 with no flags
```

### 32-bit Immediate Integer Value (42)

```
[0x04][0x10][0x0000002A]  // INT32, IMMEDIATE flag, value 42
```

### Variable Reference

```
[0x04][0x40][0x0000000000000001]  // INT32, VARIABLE flag, variable ID 1
```

### Vector of Four 32-bit Floats

```
[0x33][0x00][0x10][0x00][0x0004]  // CVEC, no flags, element type FP32, 4 elements
```

### 3×3 Matrix of 32-bit Floats

```
[0x34][0x00][0x10][0x00][0x0003][0x0003]  // MAT, no flags, element type FP32, 3 rows, 3 columns
```

### Structure with Two Fields

```
[0xA0][0x00][0x0002]           // STRUCT, no flags, 2 fields
  [0x0001][0x04][0x00]         // Member ID 1: INT32
  [0x0002][0x10][0x00]         // Member ID 2: FP32
```

## Related Components

- [Binary Format](../format/binary-format.md) - COIL binary encoding
- [Type-Determined Behavior](./type-determined-behavior.md) - How types affect instruction behavior