# COIL Types Reference

## Overview

This document provides a comprehensive reference for the COIL type system, including all type IDs, encoding formats, and compatibility rules.

## Type Encoding

Types in COIL use a 16-bit encoding format:

```
┌────────────┬────────────┐
│  Main Type │ Extensions │
│  (8 bits)  │  (8 bits)  │
└────────────┴────────────┘
```

Some types require additional data beyond this 16-bit descriptor.

## Integer Types

| Type ID | Name | Size (Bytes) | Range |
|---------|------|--------------|-------|
| 0x01    | TYPE_INT8   | 1 | -128 to 127 |
| 0x02    | TYPE_INT16  | 2 | -32,768 to 32,767 |
| 0x03    | TYPE_INT32  | 4 | -2,147,483,648 to 2,147,483,647 |
| 0x04    | TYPE_INT64  | 8 | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| 0x05    | TYPE_INT128 | 16 | -170,141,183,460,469,231,731,687,303,715,884,105,728 to 170,141,183,460,469,231,731,687,303,715,884,105,727 |
| 0x10    | TYPE_UNT8   | 1 | 0 to 255 |
| 0x11    | TYPE_UNT16  | 2 | 0 to 65,535 |
| 0x13    | TYPE_UNT32  | 4 | 0 to 4,294,967,295 |
| 0x14    | TYPE_UNT64  | 8 | 0 to 18,446,744,073,709,551,615 |
| 0x15    | TYPE_UNT128 | 16 | 0 to 340,282,366,920,938,463,463,374,607,431,768,211,455 |

## Floating-Point Types

| Type ID | Name | Size (Bytes) | Precision |
|---------|------|--------------|-----------|
| 0x23    | TYPE_FP16    | 2 | Half precision (1 sign, 5 exponent, 10 mantissa) |
| 0x25    | TYPE_FP32    | 4 | Single precision (1 sign, 8 exponent, 23 mantissa) |
| 0x26    | TYPE_FP64    | 8 | Double precision (1 sign, 11 exponent, 52 mantissa) |
| 0x28    | TYPE_FP128   | 16 | Quadruple precision (1 sign, 15 exponent, 112 mantissa) |

## Vector Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x30    | TYPE_V128 | 16 | 128-bit vector |
| 0x31    | TYPE_V256 | 32 | 256-bit vector |
| 0x32    | TYPE_V512 | 64 | 512-bit vector |

Vector types require an element type parameter:
```
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats (4 elements)
```

## Special Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x40    | TYPE_BIT | Bit (boolean) type, 1/8 byte |
| 0xFF    | TYPE_VOID | Void type (no value) |

## Pointer Type

| Type ID | Name | Size | Description |
|---------|------|------|-------------|
| 0xA6    | TYPE_PTR | Platform-dependent | Pointer type |

Pointer can specify the pointed-to type:
```
TYPE_PTR=TYPE_INT32   ; Pointer to 32-bit integer
```

## Platform-Dependent Types

| Type ID | Name | Size (Platform-dependent) |
|---------|------|----------------------------|
| 0xA0    | TYPE_INT  | Default integer (typically 4 or 8 bytes) |
| 0xA1    | TYPE_UNT  | Default unsigned integer |
| 0xA2    | TYPE_FP   | Default floating-point (typically 4 or 8 bytes) |
| 0xA3    | TYPE_LINT | Largest integer type |
| 0xA4    | TYPE_LUNT | Largest unsigned integer type |
| 0xA5    | TYPE_LFP  | Largest floating-point type |

## Reference Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x90    | TYPE_VAR | Reference to a variable |
| 0x91    | TYPE_SYM | Reference to a symbol |

## Register Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x92    | TYPE_RGP | General purpose register |
| 0x93    | TYPE_RFP | Floating point register |
| 0x94    | TYPE_RV  | Vector register |
| 0x9A    | TYPE_IP  | Instruction Pointer register |
| 0x9B    | TYPE_SP  | Stack Pointer register |
| 0x9C    | TYPE_BP  | Base Pointer register |

## State Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x96    | TYPE_SAR | State - all registers |
| 0x97    | TYPE_SAF | State - all flags |
| 0x98    | TYPE_SES | State - extended processor state |
| 0x99    | TYPE_SS  | State - complete state |

## Composite Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0xD0    | TYPE_STRUCT | Structure type |
| 0xD1    | TYPE_PACK   | Packed structure (no padding) |
| 0xD2    | TYPE_UNION  | Union type |
| 0xD3    | TYPE_ARRAY  | Array type |

Composite types require additional type information:
```
TYPE_STRUCT=point     ; Structure named "point"
TYPE_ARRAY=TYPE_UNT8  ; Array of unsigned 8-bit integers
```

## Complex Numeric Types

| Type ID | Name | Size (Varies) |
|---------|------|---------------|
| 0xB0    | TYPE_CINT | Complex integer (pair of integers) |
| 0xB1    | TYPE_CUNT | Complex unsigned integer |
| 0xB2    | TYPE_CFP  | Complex floating-point |

## Parameter Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0xF8    | TYPE_ABICTL | ABI control parameter |
| 0xF0    | TYPE_PARAM5 | Parameter 5 - Conditions and flags |
| 0xFE    | TYPE_PARAM0 | Parameter 0 - General purpose |
| 0xFD    | TYPE_PARAM1 | Parameter 1 |
| 0xFC    | TYPE_PARAM2 | Parameter 2 |

## Type Extensions

| Bit | Hex Value | Name | Description |
|-----|-----------|------|-------------|
| 0   | 0x01 | CONST    | Constant (read-only) |
| 1   | 0x02 | VOLATILE | Volatile (not optimizable) |
| 5   | 0x20 | IMM      | Immediate value |
| 6   | 0x40 | VAR      | Variable ID |
| 7   | 0x80 | SYM      | Symbol ID |

Example of type extension usage:
```
0x0301 - TYPE_INT32+CONST  ; Constant 32-bit integer
0x2520 - TYPE_FP32+IMM     ; Immediate 32-bit float
```

## Type Compatibility Rules

### Compatible Types

Types are compatible under these conditions:

1. **Exact Match**: Types are identical
2. **Platform to Fixed**: Platform type matches its fixed-width equivalent
   - TYPE_INT to TYPE_INT32 (on 32-bit platform)
   - TYPE_FP to TYPE_FP64 (on 64-bit platform)

3. **Integer Widening**: Destination type is wider with same signedness
   - TYPE_INT8 to TYPE_INT32 (signed widening)
   - TYPE_UNT8 to TYPE_UNT16 (unsigned widening)

4. **Floating Widening**: Destination type has more precision
   - TYPE_FP32 to TYPE_FP64 (float to double)

5. **Vector Element Compatibility**: Vector elements are compatible
   - TYPE_V128=TYPE_FP32 to TYPE_V128=TYPE_FP64 (with potential precision change)

### Incompatible Types

Types are incompatible under these conditions:

1. **Signedness Mismatch**: Different signedness without explicit conversion
   - TYPE_INT32 to TYPE_UNT32 (requires CONVERT)

2. **Narrowing**: Destination has fewer bits without explicit conversion
   - TYPE_INT32 to TYPE_INT16 (requires CONVERT)

3. **Category Mismatch**: Different type categories without explicit conversion
   - TYPE_INT32 to TYPE_FP32 (requires CONVERT)
   - TYPE_PTR to TYPE_INT (requires CAST)

## Type Conversion

### CONVERT Instruction
Used for value-preserving conversions:
```
CONVERT float_value, int_value  ; Convert integer to float
```

### CAST Instruction
Used to reinterpret bit patterns:
```
CAST int_bits, float_value  ; View float bits as integer
```

## Type Literals in CASM

```
TYPE_INT32      ; 32-bit integer type
TYPE_FP64       ; 64-bit floating point type
TYPE_PTR        ; Generic pointer type
TYPE_ARRAY=TYPE_UNT8  ; Array of bytes
TYPE_PTR=TYPE_INT32   ; Pointer to 32-bit integer
TYPE_V128=TYPE_FP32   ; 128-bit vector of 32-bit floats
```

## Binary Encoding Examples

```
; TYPE_INT32 (32-bit integer)
0x0300

; TYPE_FP64 (64-bit float)
0x2600

; TYPE_INT32+CONST (constant 32-bit integer)
0x0301

; TYPE_UNT8+IMM (immediate 8-bit unsigned integer)
0x1020

; TYPE_PTR=TYPE_INT32 (pointer to 32-bit integer)
0xA600 0x0300

; TYPE_V128=TYPE_FP32 (vector of 32-bit floats)
0x3000 0x2500
```