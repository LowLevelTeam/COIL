# COIL Type System

## Overview

The COIL Type System defines how data types are represented, manipulated, and stored within the COIL architecture. The type system is fundamental to COIL's type-determined instruction behavior pattern, where the same opcode can produce different operations based on operand types.

## Type Representation

Every type in COIL is represented by:

```
[type opcode: uint8_t] [type extension: uint8_t] [type data: optional ...] [data: optional ...]
```

The type extension byte is always present, even if all flags are 0.

## Type Categories

### Fixed-Width Integral Types (0x00-0x0F)

| Opcode | Name   | Description         | Size (bytes) | Extension |
|--------|--------|---------------------|--------------|-----------|
| 0x00   | INT8   | 8-bit signed int    | 1            | Core      |
| 0x01   | UNT8   | 8-bit unsigned int  | 1            | Core      |
| 0x02   | INT16  | 16-bit signed int   | 2            | Core      |
| 0x03   | UNT16  | 16-bit unsigned int | 2            | Core      |
| 0x04   | INT32  | 32-bit signed int   | 4            | Core      |
| 0x05   | UNT32  | 32-bit unsigned int | 4            | Core      |
| 0x06   | INT64  | 64-bit signed int   | 8            | Core      |
| 0x07   | UNT64  | 64-bit unsigned int | 8            | Core      |

### Fixed-Width Floating-Point Types (0x10-0x1F)

| Opcode | Name   | Description        | Size (bytes) | Extension |
|--------|--------|--------------------|--------------|-----------|
| 0x10   | FP32   | 32-bit float       | 4            | Core      |
| 0x11   | FP64   | 64-bit double      | 8            | Core      |

### Fixed-Width Vector Types (0x20-0x2F)

| Opcode | Name   | Description        | Size (bytes) | Extension |
|--------|--------|--------------------|--------------|-----------|
| 0x20   | V128   | 128-bit vector     | 16           | Core      |
| 0x21   | V256   | 256-bit vector     | 32           | Core      |
| 0x22   | V512   | 512-bit vector     | 64           | Core      |

### Complex Types (0x30-0x3F)

| Opcode | Name     | Description            | Extension    |
|--------|----------|------------------------|--------------|
| 0x30   | CINT     | Custom-width signed int   | Core      |
| 0x31   | CUNT     | Custom-width unsigned int | Core      |
| 0x32   | CFP      | Custom-width float        | Core      |
| 0x33   | CVEC     | Custom vector             | MultiDim   |
| 0x34   | MAT      | General matrix            | MultiDim   |
| 0x35   | SQMAT    | Square matrix             | MultiDim   |
| 0x36   | DIAGMAT  | Diagonal matrix           | MultiDim   |
| 0x37   | TENSOR   | General tensor            | MultiDim   |
| 0x38   | SYMTENSOR| Symmetric tensor          | MultiDim   |

### Composite Types (0xA0-0xAF)

| Opcode | Name   | Description              | Extension     |
|--------|--------|--------------------------|---------------|
| 0xA0   | STRUCT | Fields at fixed offsets  | Composite     |
| 0xA1   | PACK   | Packed structure         | Composite     |
| 0xA2   | UNION  | Overlapping fields       | Composite     |
| 0xA3   | ALIAS  | Type alias               | Composite     |

### Optimized Types (0xF0-0xF9)

| Opcode | Name    | Description             | Extension     |
|--------|---------|-------------------------|---------------|
| 0xF0   | BIT     | Single bit (boolean)    | Core          |
| 0xF1   | VARID   | Variable identifier     | Core          |
| 0xF2   | SYMBOL  | Symbol reference        | Core          |
| 0xF3   | DEFID   | Definition Identifier   | Preprocessor  |
| 0xF4   | STRING  | String reference        | Core          |
| 0xF9   | REG     | Hardware register       | Processor     |

### Special Types (0xFA-0xFF)

| Opcode | Name     | Description               | Extension     |
|--------|----------|---------------------------|---------------|
| 0xE1   | SIZEOF   | Reference to sizeof       | Core          |
| 0xE2   | ALIGNOF  | Reference to alignof      | Core          |
| 0xFA   | PARAM4   | Parameter type 4          | Core          |
| 0xFB   | PARAM3   | Parameter type 3          | Core          |
| 0xFC   | PARAM2   | Parameter type 2          | Core          |
| 0xFD   | PARAM1   | Parameter type 1          | Core          |
| 0xFE   | PARAM0   | Parameter type 0          | Core          |
| 0xFF   | VOID     | No type/value             | Core          |

## Type Extensions

Type extensions are bitmasks that modify type behavior:

| Bit     | Name       | Description                                |
|---------|------------|--------------------------------------------|
| (1 << 0)| CONST      | Value cannot be modified                   |
| (1 << 1)| VOLATILE   | Value may change unexpectedly              |
| (1 << 2)| ATOMIC     | Operations must be atomic                  |
| (1 << 3)| SATURATE   | Cap at largest value instead of overflow   |
| (1 << 4)| IMMEDIATE  | Value is embedded in instruction           |
| (1 << 5)| DEFINITION | Value is a compile-time definition         |
| (1 << 6)| VARIABLE   | Value is a variable reference              |
| (1 << 7)| SYMBOL     | Value is a symbol reference                |

## Type-Specific Data Formats

### Vector Types
Type data includes element type and count:

```
[vector_type][ext][element_type][count: uint16_t]
```

Example: Vector of 4 INT32 elements
```
[0x33][0x00][0x04][0x00][0x0004]
```

### Matrix Types
Type data includes element type, rows, and columns:

```
[matrix_type][ext][element_type][rows: uint16_t][cols: uint16_t]
```

Example: 3×3 matrix of FP32 elements
```
[0x34][0x00][0x10][0x00][0x0003][0x0003]
```

### Composite Types
Type data includes member count and member definitions:

```
[struct_type][ext][member_count: uint16_t][[member_id: uint16_t][type]...]
```

Example: Struct with INT32 and FP32 fields
```
[0xA0][0x00][0x0002][0x0001][0x04][0x00][0x0002][0x10][0x00]
```

## Memory Layout

### Alignment Requirements

Types have natural alignment requirements based on their size:

| Type Size (bytes) | Natural Alignment (bytes) |
|-------------------|---------------------------|
| 1                 | 1                         |
| 2                 | 2                         |
| 4                 | 4                         |
| 8                 | 8                         |
| 16+               | 16                        |

For composite types, the alignment is determined by the largest alignment required by any member.

### Matrix and Tensor Layout

- Matrices use row-major storage by default
- Tensors use generalized row-major storage (rightmost index varies fastest)
- See [Matrix-Tensor Layout](/extensions/multidim/matrix-tensor-layout.md) for details

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

## Binary Examples

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
[0x34][0x00][0x10][0x00][0x0003][0x0003]  // MAT, no flags, element type FP32, 3 rows, 3 columns
```# COIL Type System (TS)

The COIL TS is a needed complexity in the development ecosystem allowing for the instruction set to be kept relatively simple. instead of addl, addw, adds all being seperate the COIL TS allows just one add instruction and have the real instruction or inention be worked out by the processor. This kind of language is really good for not only writing simple code but also keeping files small, however in certain cases where a fast runtime compilation of the language is needed you should instead check out COIL Byte Code (CBC). A more instruction complex set with limited extensibility and possibly bloated in sizse but allows for extermely fast runtime compilation. 

COIL Byte Code can be made from COIL by a COIL Processor where normal COIL instructions may be extended out into multiple COIL Byte Code instructions.

While reading you may discover certain types under an extension classification, this is to help COIL processors implement part of this incredibly complex type system, major extensions would be composite types, multi dimensional types and complex types all of which require complex implementation in code generation but also in the COIL processor TS implementation.


