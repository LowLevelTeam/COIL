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
[type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]
```

## Type Categories

### Fixed-Width Integral Types (0x00-0x0F)

Standard integer types with fixed bit widths:

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

Standard floating-point types:

| Opcode | Name   | Description          | Size (bytes) |
|--------|--------|----------------------|--------------|
| 0x10   | FP32   | 32-bit float         | 4            |
| 0x11   | FP64   | 64-bit double        | 8            |

### Fixed-Width Vector Types (0x20-0x2F)

SIMD-friendly vector types:

| Opcode | Name   | Description          | Size (bytes) |
|--------|--------|----------------------|--------------|
| 0x20   | V128   | 128-bit vector       | 16           |
| 0x21   | V256   | 256-bit vector       | 32           |
| 0x22   | V512   | 512-bit vector       | 64           |

Vector types include an additional element type specification.

### Complex Types (0x30-0x3F)

User-defined bit-width types and advanced data structures:

| Opcode | Name      | Description             | Type Data Format |
|--------|-----------|-------------------------|------------------|
| 0x30   | CINT      | Custom-width signed int | [bit size: uint16_t] |
| 0x31   | CUNT      | Custom-width unsigned int | [bit size: uint16_t] |
| 0x32   | CFP       | Custom-width float | [mantissa: uint16_t][exp: uint16_t] |
| 0x33   | CVEC      | Custom vector | [element type: Type][count: uint16_t] |
| 0x34   | MAT       | General matrix | [element type: Type][rows: uint16_t][cols: uint16_t] |
| 0x35   | SQMAT     | Square matrix | [element type: Type][size: uint16_t] |
| 0x36   | DIAGMAT   | Diagonal matrix | [element type: Type][size: uint16_t] |
| 0x37   | TENSOR    | General tensor | [element type: Type][rank: uint8_t][dim1: uint16_t]...[dimN: uint16_t] |
| 0x38   | SYMTENSOR | Symmetric tensor | [element type: Type][rank: uint8_t][dim: uint16_t] |

### Composite Types (0xA0-0xAF)

Compound types that group other types:

| Opcode | Name   | Description                             | Type Data Format         |
|--------|--------|-----------------------------------------|--------------------------|
| 0xA0   | STRUCT | Fields at fixed offsets                 | [field count][fields...] |
| 0xA1   | PACK   | Packed structure (no padding)           | [field count][fields...] |
| 0xA2   | UNION  | Overlapping fields                      | [field count][fields...] |
| 0xA3   | ALIAS  | Utilizes a type ID to get the real type | [typeid: uint64_T]       |

Composite type data includes member count and member definitions:

```
[0xA.][member_count: uint16_t ?= 0] 
  [type_id: uint64_T]  // If member_count is 0, reference existing type
  : 
  [[member_id: uint16_t][type definition],...member_count]
```

### Optimized Types (0xF0-0xF9)

Special-purpose optimized types:

| Opcode | Name    | Description                  | Data Format |
|--------|---------|------------------------------|-------------|
| 0xF0   | BIT     | Single bit (boolean)         | N/A |
| 0xF1   | VARID   | Variable identifier          | [var id: uint64_t] |
| 0xF2   | SYMBOL  | Symbol reference             | [symbol id: uint64_t] |
| 0xF3   | DEFID   | Definition Identifier        | [definition id: uint64_t] |
| 0xF4   | STRING  | String reference             | [string id: uint64_t] |
| 0xF9   | REG     | Hardware register reference  | [register: uint16_t] |

### Special Types (0xFA-0xFF)

Parameter and control types:

| Opcode | Name     | Description                       |
|--------|----------|-----------------------------------|
| 0xE1   | SIZEOF   | Reference to sizeof instruction   |
| 0xE2   | ALLIGNOF | Reference to allignof instruction |
| 0xFA   | PARAM4   | Parameter type 4                  |
| 0xFB   | PARAM3   | Parameter type 3                  |
| 0xFC   | PARAM2   | Parameter type 2                  |
| 0xFD   | PARAM1   | Parameter type 1                  |
| 0xFE   | PARAM0   | Parameter type 0                  |
| 0xFF   | VOID     | No type/value                     |

## Type Extensions

Type extensions are bitmasks that modify type behavior:

| Bit     | Name       | Description                                  |
|---------|------------|----------------------------------------------|
| (1 << 0)| CONST      | Value cannot be modified                     |
| (1 << 1)| VOLATILE   | Value may change unexpectedly                |
| (1 << 2)| ATOMIC     | Operations must be atomic                    |
| (1 << 3)| SIZE       | Type must be stored as specified size        |
| (1 << 4)| IMMEDIATE  | Value is embedded in instruction             |
| (1 << 5)| DEFINITION | Value is a compile time definition           |
| (1 << 6)| VARIABLE   | Value is a variable reference                |
| (1 << 7)| SYMBOL     | Value is a symbol reference                  |

## Type Values

The value portion depends on the type extension flags:

1. If IMMEDIATE is set: The literal value follows of the appropriate size
2. If VARIABLE is set: A uint64_t variable ID follows
3. If SYMBOL is set: A uint64_t symbol ID follows
4. If DEFINITION is set: A uint64_t definition ID follows
5. If no value flags are set: No value data is present

## Type Conversion Rules

1. **Widening Conversions**: Smaller types to larger types preserve value
2. **Narrowing Conversions**: May lose precision or range
3. **Domain Conversions**: Between different domains (int to float) follow IEEE rules
4. **Vector Conversions**: Between vectors require matching element counts
5. **Matrix Conversions**: Follow mathematical rules for compatible dimensions

## Memory Alignment

Types have natural alignment requirements:
- 8-bit types: 1-byte alignment
- 16-bit types: 2-byte alignment
- 32-bit types: 4-byte alignment
- 64-bit types: 8-byte alignment
- 128-bit and larger: Platform-specific, typically 16-byte alignment

## Examples

### 32-bit Integer Variable

```
[0x04]           // INT32 type
[0x40]           // VARIABLE flag set
[variable_id]    // 64-bit variable identifier
```

### 128-bit Vector of Float32

```
[0x20]           // V128 type
[0x00]           // No flags set
[0x10][0x00]     // Element type: FP32, no flags
```

### Custom Structure Type

```
[0xA0]           // STRUCT type
[0x03]           // 3 members
[0x0001][0x04][0x00]  // Member 1: INT32 field
[0x0002][0x11][0x00]  // Member 2: FP64 field
[0x0003][0x20][0x00][0x10][0x00]  // Member 3: V128 of FP32
```

## Related Components

- [Binary Format](../core/binary-format.md) - COIL binary encoding
- [Instruction Encoding](../instruction-encoding.md) - Instruction format details