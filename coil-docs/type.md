# COIL Type System

## Overview

The COIL Type System defines how data types are represented and manipulated within the COIL architecture. Types determine instruction behavior and memory layout.

## Type Representation

Every type in COIL is represented by:

```
[type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]
```

data in the cases down below depend on the type extension where it would be an immediate value, a variable identifier or a symbol id.

## Type Categories

### Fixed-Width Integral Types (0x00-0x0F)

Standard integer types with fixed bit widths:

| Opcode | Name   | Description         |
|--------|--------|---------------------|
| 0x00   | INT8   | 8-bit signed int    |
| 0x01   | UNT8   | 8-bit unsigned int  |
| 0x02   | INT16  | 16-bit signed int   |
| 0x03   | UNT16  | 16-bit unsigned int |
| 0x04   | INT32  | 32-bit signed int   |
| 0x05   | UNT32  | 32-bit unsigned int |
| 0x06   | INT64  | 64-bit signed int   |
| 0x07   | UNT64  | 64-bit unsigned int |

### Fixed-Width Floating-Point Types (0x10-0x1F)

Standard floating-point types:

| Opcode | Name   | Description          |
|--------|--------|----------------------|
| 0x10   | FP32   | 32-bit float         |
| 0x11   | FP64   | 64-bit double        |

### Fixed-Width Vector Types (0x20-0x2F)

SIMD-friendly vector types:

| Opcode | Name   | Description          |
|--------|--------|----------------------|
| 0x20   | V128   | 128-bit vector       |
| 0x21   | V256   | 256-bit vector       |
| 0x22   | V512   | 512-bit vector       |

This type is follwed up by the element type.

[element type: Type]

### Complex Types (0x30-0x33)

User-defined bit-width types:

| Opcode | Name   | Description             | Type Data Format             |
|--------|--------|-------------------------|------------------------------|
| 0x30   | CINT   | Custom-width signed int | [bit size: uint16_t]         |
| 0x31   | CUNT   | Custom-width unsigned   | [bit size: uint16_t]         |
| 0x32   | CFP    | Custom-width float      | [mantissa: uint16_t][exp: uint16_t] |
| 0x33   | CVEC   | Custom vector           | [element type: Type][count: uint16_t] |

### Composite Types (0xA0-0xAF)

Compound types that group other types:

| Opcode | Name   | Description                      |
|--------|--------|----------------------------------|
| 0xA0   | STRUCT | Fields at fixed offsets          |
| 0xA1   | PACK   | Packed structure (no padding)    |
| 0xA2   | UNION  | Overlapping fields               |

Type data for composite types includes field count and field definitions.

Right after each definition an inline structure or if this is to be used with a type operation for easy definition then you have to define a member count.
this is a uint16_t and if zero is defined then the next value would instead be the uint64_t type id.

```
[0xA.][member_count: uint16_t ?= 0] 
  [type_id: uint64_T]
  : 
  [[member_id: uint16_t] [type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...], ...member_count]
```

### Optimized Types (0xF0-0xF9)

Special-purpose optimized types:

| Opcode | Name    | Description                  | Data Format          |
|--------|---------|------------------------------|----------------------|
| 0xF0   | BIT     | Single bit (boolean)         | N/A                  |
| 0xF1   | VARID   | Variable identifier          | [var id: uint64_t]   |
| 0xF2   | SYMBOL  | Symbol reference             | [symbol id: uint64_t]|
| 0xF3   | DEFID   | Definition Identifier        | [definition id: uint64_t] |
| 0xF4   | STRING  | String reference             | [string id: uint64_t]|
| 0xF9   | REG     | Hardware register reference  | [register: uint16_t] |


### Special Types (0xFA-0xFF)

Parameter and control types:

| Opcode | Name    | Description                  |
|--------|---------|------------------------------|
| 0xFA   | PARAM4  | Parameter type 4             |
| 0xFB   | PARAM3  | Parameter type 3             |
| 0xFC   | PARAM2  | Parameter type 2             |
| 0xFD   | PARAM1  | Parameter type 1             |
| 0xFE   | PARAM0  | Parameter type 0             |
| 0xFF   | VOID    | No type/value                |

## Type Extensions

Type extensions are bitmasks that modify type behavior:

If neither IMMEDIATE, DEFINITION, VARIABLE or SYMBOL is set then there is no value following the type.
If more then one is set it should be reported as an error

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

The size extension may be slightly confusing but for some types like a float 16 or float 8 the compiler may see it faster to just use a float 32 as it can represent all values needed making it no differnt from the type except in sizing. In some cases however you may want to ensure that when stored in a structure or on the stack it utilizes the exact space specified making it explicit you want to optimize for memory over speed. The value may still be moved of the stack or memory or structure into a register of larger size as the type is still faster but will have storage and loading functionality as if it was the bitsize (this is also needed with something like complex types as an int24 will more often then not be a int32 to the compiler unless otherwise specified or unless there is native int24 functionality).

## Parameter Types

Parameter types modify instruction behavior:

### Flag Condition Parameters

Used for conditional execution:

| Value  | Name           | Description                  |
|--------|----------------|------------------------------|
| 0x00   | FLAG_COND_EQ   | Equal                        |
| 0x01   | FLAG_COND_NEQ  | Not equal                    |
| 0x02   | FLAG_COND_GT   | Greater than                 |
| 0x03   | FLAG_COND_GTE  | Greater than or equal        |
| 0x04   | FLAG_COND_LT   | Less than                    |
| 0x05   | FLAG_COND_LTE  | Less than or equal           |
| 0x06   | FLAG_COND_O    | Overflow                     |
| 0x07   | FLAG_COND_NO   | No Overflow                  |
| 0x08   | FLAG_COND_S    | Sign                         |
| 0x09   | FLAG_COND_NS   | Not sign                     |
| 0x0A   | FLAG_COND_Z    | Equal Zero                   |
| 0x0B   | FLAG_COND_NZ   | Not Equal Zero               |
| 0xFF   | FLAG_COND_DEF  | Is value defined             |


### Processing Unit Parameters

Specify the target processing unit:

| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | PU_CPU   | Central Processing Unit      |
| 0x01   | PU_GPU   | Graphic Processing Unit      | (Planned)

### Architecture Parameters

Specify the target architecture (values depend on PU):

For PU_CPU:
| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | CPU_X86  | x86 architecture             |
| 0x01   | CPU_ARM  | ARM architecture             |

For PU_GPU
| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | GPU_NV   | Nvidia architecture (SASS)   |
| 0x01   | GPU_AMD  | AMD architecture (GCN,RDNA)  |


### Mode Parameters

Specify the architecture mode:

| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | MODE_8   | 8-bit mode                   |
| 0x01   | MODE_16  | 16-bit mode                  |
| 0x02   | MODE_32  | 32-bit mode                  |
| 0x03   | MODE_64  | 64-bit mode                  |
| 0x04   | MODE_128 | 128-bit mode                 |

### sect_flag_p

Specify section capabilities as a bitmap:

| Value  | Name       | Description                |
|--------|------------|----------------------------|
| bit 0  | SECT_EXEC  | Executable                 |
| bit 1  | SECT_READ  | Readable                   |
| bit 2  | SECT_WRITE | Writeable                  |
| bit 3  | SECT_INIT  | Initalized Data            |
| bit 4  | SECT_UNIT  | Uninitalized Data          |

## Examples

### 32-bit Integer Variable

```
[0x04]           // INT32 type
[0x40]           // VARIABLE flag set
[variable_id]    // 64-bit variable identifier
```

### Immediate 64-bit Float

```
[0x11]           // FP64 type
[0x00]           // No flags set
[value]          // 64-bit double value
```

### Conditional Branch Parameter

```
[0xFE]           // PARAM0 type
[0x02]           // FLAG_COND_GT value
```

## Related Components

- [Binary Format](../core/binary-format.md)
- [Universal ISA](../isa-u/index.md)