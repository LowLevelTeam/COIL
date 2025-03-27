# Numeric Types in COIL

## Integer Types

COIL provides fixed-width integer types in both signed and unsigned variants:

### Signed Integers

| Type ID | Name | Size (Bytes) | Range |
|---------|------|--------------|-------|
| 0x01    | TYPE_INT8   | 1 | -128 to 127 |
| 0x02    | TYPE_INT16  | 2 | -32,768 to 32,767 |
| 0x03    | TYPE_INT32  | 4 | -2,147,483,648 to 2,147,483,647 |
| 0x04    | TYPE_INT64  | 8 | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| 0x05    | TYPE_INT128 | 16 | -170,141,183,460,469,231,731,687,303,715,884,105,728 to 170,141,183,460,469,231,731,687,303,715,884,105,727 |

### Unsigned Integers

| Type ID | Name | Size (Bytes) | Range |
|---------|------|--------------|-------|
| 0x10    | TYPE_UNT8   | 1 | 0 to 255 |
| 0x11    | TYPE_UNT16  | 2 | 0 to 65,535 |
| 0x13    | TYPE_UNT32  | 4 | 0 to 4,294,967,295 |
| 0x14    | TYPE_UNT64  | 8 | 0 to 18,446,744,073,709,551,615 |
| 0x15    | TYPE_UNT128 | 16 | 0 to 340,282,366,920,938,463,463,374,607,431,768,211,455 |

### Integer Operations

Integer operations follow these rules:

- **Signed Integer Arithmetic**: Two's complement operations
- **Unsigned Integer Arithmetic**: Modular arithmetic
- **Overflow**: Wrap-around for unsigned integers, implementation-defined for signed
- **Division by Zero**: Error condition

## Floating-Point Types

COIL supports various IEEE 754 floating-point formats:

| Type ID | Name | Size (Bytes) | Precision |
|---------|------|--------------|-----------|
| 0x23    | TYPE_FP16    | 2 | Half precision (1 sign, 5 exponent, 10 mantissa) |
| 0x25    | TYPE_FP32    | 4 | Single precision (1 sign, 8 exponent, 23 mantissa) |
| 0x26    | TYPE_FP64    | 8 | Double precision (1 sign, 11 exponent, 52 mantissa) |
| 0x28    | TYPE_FP128   | 16 | Quadruple precision (1 sign, 15 exponent, 112 mantissa) |

### Floating-Point Operations

Floating-point operations follow IEEE 754 rules:

- **Rounding Mode**: Round to nearest even by default
- **Special Values**: Support for NaN, Infinity
- **Exceptions**: Invalid operation, division by zero, overflow, underflow, inexact

## Platform-Dependent Numeric Types

COIL includes types that automatically adjust to the target platform:

| Type ID | Name | Size (Platform-dependent) |
|---------|------|----------------------------|
| 0xA0    | TYPE_INT  | Default integer (typically 4 or 8 bytes) |
| 0xA1    | TYPE_UNT  | Default unsigned integer |
| 0xA2    | TYPE_FP   | Default floating-point (typically 4 or 8 bytes) |
| 0xA3    | TYPE_LINT | Largest integer type |
| 0xA4    | TYPE_LUNT | Largest unsigned integer type |
| 0xA5    | TYPE_LFP  | Largest floating-point type |

These types provide portability while adapting to the platform's natural sizes.

## Complex Numeric Types

COIL supports complex number types:

| Type ID | Name | Size (Varies) |
|---------|------|---------------|
| 0xB0    | TYPE_CINT | Complex integer (pair of integers) |
| 0xB1    | TYPE_CUNT | Complex unsigned integer |
| 0xB2    | TYPE_CFP  | Complex floating-point |

## Type Extensions for Numeric Types

Numeric types can use these extensions:

- **0x01 (CONST)**: Read-only value
- **0x20 (IMM)**: Immediate (literal) value

Example encoding:
```
0x0301 - TYPE_INT32+CONST  ; Constant 32-bit integer
0x2520 - TYPE_FP32+IMM     ; Immediate 32-bit float
```

## Numeric Type Compatibility and Conversion

### Compatible Types
- Same type (e.g., INT32 to INT32)
- Widening conversions (INT8 to INT32)
- Same signedness when widening (UNT8 to UNT16)
- Platform type to its current fixed-width equivalent (INT to INT32)

### Conversion Rules
- **Integer to Wider Integer**: Sign or zero extension
- **Integer to Floating-Point**: Value-preserving conversion
- **Floating-Point to Integer**: Truncation toward zero
- **Floating-Point to Wider Floating-Point**: Precision expansion