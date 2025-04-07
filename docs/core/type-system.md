# COIL Type System

## Overview

The COIL type system defines how data is represented, manipulated, and stored. It is the foundation for COIL's type-determined instruction behavior, where operations adapt based on operand types.

## Type Representation

Every type in COIL is represented by:

```
[type_opcode: uint8_t][type_extension: uint8_t][type_data: optional][data: optional]
```

## Type Categories

### Fixed-Width Integral Types (0x00-0x0F)

| Opcode | Name   | Description         | Size (bytes) |
|--------|--------|---------------------|--------------|
| 0x00   | INT8   | 8-bit signed int    | 1            |
| 0x01   | UNT8   | 8-bit unsigned int  | 1            |
| 0x02   | INT16  | 16-bit signed int   | 2            |
| 0x03   | UNT16  | 16-bit unsigned int | 2            |
| 0x04   | INT32  | 32-bit signed int   | 4            |
| 0x05   | UNT32  | 32-bit unsigned int | 4            |
| 0x06   | INT64  | 64-bit signed int   | 8            |
| 0x07   | UNT64  | 64-bit unsigned int | 8            |

### Fixed-Width Floating-Point Types (0x10-0x1F)

| Opcode | Name   | Description        | Size (bytes) |
|--------|--------|--------------------|--------------|
| 0x10   | FP32   | 32-bit float       | 4            |
| 0x11   | FP64   | 64-bit double      | 8            |

### Fixed-Width Vector Types (0x20-0x2F)

| Opcode | Name   | Description        | Size (bytes) |
|--------|--------|--------------------|--------------|
| 0x20   | V128   | 128-bit vector     | 16           |
| 0x21   | V256   | 256-bit vector     | 32           |
| 0x22   | V512   | 512-bit vector     | 64           |

### Complex Types (0x30-0x3F)

| Opcode | Name     | Description            | Extension    |
|--------|----------|------------------------|--------------|
| 0x33   | CVEC     | Custom vector          | MultiDim     |
| 0x34   | MAT      | General matrix         | MultiDim     |
| 0x35   | SQMAT    | Square matrix          | MultiDim     |
| 0x36   | DIAGMAT  | Diagonal matrix        | MultiDim     |
| 0x37   | TENSOR   | General tensor         | MultiDim     |

### Composite Types (0xA0-0xAF)

| Opcode | Name   | Description              | Extension    |
|--------|--------|--------------------------|--------------|
| 0xA0   | STRUCT | Fields at fixed offsets  | Composite    |
| 0xA1   | PACK   | Packed structure         | Composite    |
| 0xA2   | UNION  | Overlapping fields       | Composite    |
| 0xA3   | ALIAS  | Type alias               | Composite    |

### Special Types (0xF0-0xFF)

| Opcode | Name    | Description             |
|--------|---------|-------------------------|
| 0xF0   | BIT     | Single bit (boolean)    |
| 0xF1   | VARID   | Variable identifier     |
| 0xF2   | SYMBOL  | Symbol reference        |
| 0xF4   | STRING  | String reference        |
| 0xFF   | VOID    | No type/value           |

## Type Extensions

Type extensions are bitmasks that modify type behavior:

| Bit     | Name       | Description                              |
|---------|------------|------------------------------------------|
| (1 << 0)| CONST      | Value cannot be modified                 |
| (1 << 1)| VOLATILE   | Value may change unexpectedly            |
| (1 << 2)| ATOMIC     | Operations must be atomic                |
| (1 << 3)| SATURATE   | Cap at largest value instead of overflow |
| (1 << 4)| IMMEDIATE  | Value is embedded in instruction         |
| (1 << 5)| DEFINITION | Value is a compile-time definition       |
| (1 << 6)| VARIABLE   | Value is a variable reference            |
| (1 << 7)| SYMBOL     | Value is a symbol reference              |

## Memory Layout and Alignment

Types have natural alignment requirements based on their size:

| Type Size (bytes) | Natural Alignment (bytes) |
|-------------------|---------------------------|
| 1                 | 1                         |
| 2                 | 2                         |
| 4                 | 4                         |
| 8                 | 8                         |
| 16+               | 16                        |

For composite types, the alignment is determined by the largest alignment required by any member.

## Type Conversion Rules

- **Widening Conversions**: Smaller types to larger types preserve value
  - INT8 → INT16 → INT32 → INT64
  - FP32 → FP64

- **Narrowing Conversions**: May lose precision or range
  - INT64 → INT32 → INT16 → INT8 (truncation)
  - FP64 → FP32 (precision loss)

- **Domain Conversions**: Between different domains follow IEEE rules
  - INT32 → FP32 (exact for values within range)
  - FP32 → INT32 (truncation toward zero)

## Examples

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

### 3×3 Matrix of 32-bit Floats
```
[0x34][0x00][0x10][0x00][0x0003][0x0003]  // MAT, element type FP32, 3×3
```