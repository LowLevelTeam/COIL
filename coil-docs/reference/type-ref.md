# COIL Type Reference

## Purpose

This document provides a comprehensive reference of the type system in COIL, including all type identifiers, type extensions, and type-specific behaviors. It serves as the authoritative listing of all types in the COIL ecosystem.

## Key Concepts

- **Type Encoding**: How types are represented in binary
- **Type Categories**: Major classifications of types
- **Type Extensions**: Modifiers that affect type behavior
- **Type Compatibility**: Rules governing type conversions and operations
- **Type-Specific Data**: Additional data associated with certain types

## Type Encoding

Types in COIL are encoded in a 16-bit format:
- First 8 bits: Main Type (0x00-0xFF)
- Second 8 bits: Type Extensions (flags)

Some types require additional data beyond the 16-bit descriptor.

## Integer Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x01    | TYPE_INT8   | 1 | 8-bit signed integer |
| 0x02    | TYPE_INT16  | 2 | 16-bit signed integer |
| 0x03    | TYPE_INT32  | 4 | 32-bit signed integer |
| 0x04    | TYPE_INT64  | 8 | 64-bit signed integer |
| 0x05    | TYPE_INT128 | 16 | 128-bit signed integer |
| 0x10    | TYPE_UNT8   | 1 | 8-bit unsigned integer |
| 0x11    | TYPE_UNT16  | 2 | 16-bit unsigned integer |
| 0x13    | TYPE_UNT32  | 4 | 32-bit unsigned integer |
| 0x14    | TYPE_UNT64  | 8 | 64-bit unsigned integer |
| 0x15    | TYPE_UNT128 | 16 | 128-bit unsigned integer |

## Floating Point Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x20    | TYPE_FP8e5m2 | 1 | 8-bit float (5-bit exponent, 2-bit mantissa) |
| 0x21    | TYPE_FP8e4m3 | 1 | 8-bit float (4-bit exponent, 3-bit mantissa) |
| 0x22    | TYPE_FP16b   | 2 | bfloat16 format |
| 0x23    | TYPE_FP16    | 2 | IEEE 754 half precision |
| 0x24    | TYPE_FP32t   | 4 | tensor float 32 |
| 0x25    | TYPE_FP32    | 4 | IEEE 754 single precision |
| 0x26    | TYPE_FP64    | 8 | IEEE 754 double precision |
| 0x27    | TYPE_FP80    | 10 | x87 extended precision |
| 0x28    | TYPE_FP128   | 16 | IEEE 754 quadruple precision |

## Vector Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x30    | TYPE_V128 | 16 | 128-bit vector |
| 0x31    | TYPE_V256 | 32 | 256-bit vector |
| 0x32    | TYPE_V512 | 64 | 512-bit vector |

Vector types require element type specification:
```
TYPE_V128=TYPE_FP32  ; 128-bit vector of 32-bit floats (4 elements)
TYPE_V256=TYPE_INT16  ; 256-bit vector of 16-bit integers (16 elements)
```

## Boolean Type

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x40    | TYPE_BIT | 1/8 | 1-bit boolean, used in bit arrays |

## Special Types

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0x90    | TYPE_VAR | Varies | Value is a variable ID |
| 0x91    | TYPE_SYM | Varies | Value is a symbol rather than value at symbol |

## Register Types

| Type ID | Name | Size (Arch-dependent) | Description |
|---------|------|------------------------|-------------|
| 0x92    | TYPE_RGP | Varies | Register general purpose |
| 0x93    | TYPE_RFP | Varies | Register floating point |
| 0x94    | TYPE_RV  | Varies | Register vector |
| 0x95    | TYPE_RS  | Varies | Register segment |
| 0x96    | TYPE_SAR | Varies | State - all registers |
| 0x97    | TYPE_SAF | Varies | State - all flags |
| 0x98    | TYPE_SES | Varies | State - extended processor state |
| 0x99    | TYPE_SS  | Varies | State - complete state |
| 0x9A    | TYPE_IP  | Varies | Register Instruction Pointer |
| 0x9B    | TYPE_SP  | Varies | Register Stack Pointer |
| 0x9C    | TYPE_BP  | Varies | Register Base Pointer |

## Platform Dependent Types

| Type ID | Name | Size (Platform-dependent) | Description |
|---------|------|----------------------------|-------------|
| 0xA0    | TYPE_INT  | Varies | Default integer for current platform |
| 0xA1    | TYPE_UNT  | Varies | Default unsigned for current platform |
| 0xA2    | TYPE_FP   | Varies | Default float for current platform |
| 0xA3    | TYPE_LINT | Varies | Largest integer for current platform |
| 0xA4    | TYPE_LUNT | Varies | Largest unsigned for current platform |
| 0xA5    | TYPE_LFP  | Varies | Largest float point for current platform |
| 0xA6    | TYPE_PTR  | Varies | Default pointer size for current platform |

## Complex Types

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0xB0    | TYPE_CINT | Varies | Complex integer |
| 0xB1    | TYPE_CUNT | Varies | Complex unsigned |
| 0xB2    | TYPE_CFP  | Varies | Complex floating point |

## Composite Types

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0xD0    | TYPE_STRUCT | Varies | Structure type |
| 0xD1    | TYPE_PACK   | Varies | Packed structure (no padding) |
| 0xD2    | TYPE_UNION  | Varies | Union type |
| 0xD3    | TYPE_ARRAY  | Varies | Array type |

Composite types require additional type information:
```
TYPE_STRUCT=point  ; Structure type with name "point"
TYPE_ARRAY=TYPE_UNT8  ; Array of unsigned 8-bit integers
```

## Parameter Types

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0xF0    | TYPE_PARAM5 | Varies | Parameter type 5 |
| 0xF1    | TYPE_PARAM4 | Varies | Parameter type 4 |
| 0xFB    | TYPE_PARAM3 | Varies | Parameter type 3 |
| 0xFC    | TYPE_PARAM2 | Varies | Parameter type 2 |
| 0xFD    | TYPE_PARAM1 | Varies | Parameter type 1 |
| 0xFE    | TYPE_PARAM0 | Varies | Parameter type 0 |
| 0xF8    | TYPE_ABICTL | Varies | ABI control parameter |

## Void Type

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0xFF    | TYPE_VOID | 0 | Void type (no value) |

## Type Extensions (Second 8 bits)

Type extensions provide additional qualifiers for type values:

| Bit Position | Hex Value | Name | Description |
|--------------|-----------|------|-------------|
| Bit 0 | 0x01 | TYPEEXT_CONST    | Constant value (read-only) |
| Bit 1 | 0x02 | TYPEEXT_VOLATILE | Volatile access (not optimizable) |
| Bit 4 | 0x10 | TYPEEXT_VOID     | No Value |
| Bit 5 | 0x20 | TYPEEXT_IMM      | Immediate value |
| Bit 6 | 0x40 | TYPEEXT_VAR      | Variable ID |
| Bit 7 | 0x80 | TYPEEXT_SYM      | Symbol ID |

Examples:
```
0x0301 - TYPE_INT32+CONST  ; Constant 32-bit integer
0x1420 - TYPE_UNT64+IMM    ; Immediate unsigned 64-bit value
0x0302 - TYPE_INT32+VOLATILE ; Volatile 32-bit integer
```

## Parameter Type Values

### TYPE_PARAM0 - Symbol Parameters

| Value | Name | Description |
|-------|------|-------------|
| 0x00  | TMP  | Symbol is used only in this context |
| 0x01  | FILE | Symbol is used around the file |
| 0x02  | GLOB | Symbol is used in other files |

### TYPE_PARAM5 - Branch Conditions

| Value | Name | Description |
|-------|------|-------------|
| 0x00  | BRANCH_COND_EQ | Equal |
| 0x01  | BRANCH_COND_NE | Not equal |
| 0x02  | BRANCH_COND_GE | Greater than or equal |
| 0x03  | BRANCH_COND_LT | Less than |
| 0x04  | BRANCH_COND_GT | Greater than |
| 0x05  | BRANCH_COND_LE | Less than or equal |
| 0x06  | BRANCH_COND_Z  | Zero flag set |
| 0x07  | BRANCH_COND_NZ | Zero flag not set |
| 0x08  | BRANCH_COND_C  | Carry flag set |
| 0x09  | BRANCH_COND_NC | Carry flag not set |
| 0x0A  | BRANCH_COND_O  | Overflow flag set |
| 0x0B  | BRANCH_COND_NO | Overflow flag not set |
| 0x0C  | BRANCH_COND_S  | Sign flag set |
| 0x0D  | BRANCH_COND_NS | Sign flag not set |

### TYPE_ABICTL - ABI Control

| Value | Name | Description |
|-------|------|-------------|
| 0x00  | ABICTL_STANDARD | Use specified ABI convention |
| 0x01  | ABICTL_PARAM    | Passing parameters via ABI |
| 0x02  | ABICTL_RET      | Returning values via ABI |
| 0x03  | ABICTL_VARIADIC | Variadic function support |
| 0x04  | ABICTL_CUSTOM   | Custom ABI handling |

## Type Compatibility Rules

Types are compatible under the following conditions:

1. The main types match exactly.
2. One type is a platform-dependent type and the other is its currently selected fixed-width equivalent.
3. Both types are integer types with the same signedness and the destination type has equal or greater width.
4. Both types are floating-point types and the destination type has equal or greater precision.
5. Both types are vector types with compatible element types and the destination has sufficient size.

## Type Data Syntax

In CASM, type data can be specified in two ways:

1. **Single Equals Syntax**: `TYPE=data` where `data` is the type data
   ```
   TYPE_RGP=RAX        ; Register type with RAX as type data
   TYPE_ARRAY=TYPE_UNT8  ; Array type with UNT8 as element type
   ```

2. **Double Equals Syntax**: `TYPE=typedata=followingdata` for complex type specifications
   ```
   TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64  ; ABI control with standard type and specific ABI
   TYPE_PTR=TYPE_INT32=0x1000             ; Pointer type with INT32 as pointed type and address
   ```

## Binary Encoding Examples

Examples of type encoding in binary:

```
; TYPE_INT32
Binary: 0x03 0x00

; TYPE_INT32+CONST
Binary: 0x03 0x01

; TYPE_RGP=RAX (where RAX has ID 0x00)
Binary: 0x92 0x00 0x00

; TYPE_ARRAY=TYPE_UNT8
Binary: 0xD3 0x00 0x10 0x00

; TYPE_V128=TYPE_FP32 
Binary: 0x30 0x00 0x25 0x00
```

## Type System Versioning

The type system is versioned along with COIL itself:

- **Version 1**: All types listed in this document
- **Version 2**: Adds library-support types
- **Version 3**: Adds multi-device types

## Error Codes for Type System

Common type system error codes:

```
3001 - Invalid type encoding
3002 - Incompatible types
3003 - Invalid type conversion
3004 - Type size mismatch
3005 - Invalid type extension
3006 - Type not supported on target
3007 - Invalid structure definition
3008 - Invalid array definition
```

## Related Documentation

- [Type System](../spec/systems/type-system.md) - Conceptual overview of the type system
- [Type Operations](../spec/instruction-set/type.md) - Instructions for working with types
- [Binary Format](../spec/binary-format.md) - How types are encoded in binary format
- [CASM Syntax](../spec/assembly/syntax.md) - How to use types in CASM