# COIL Type System

## Purpose

This document defines the foundational type system for COIL, establishing how data is represented and how type information influences instruction behavior. The type system is central to COIL's type-determined instruction philosophy.

## Type-Determined Instruction Philosophy

COIL's most distinctive characteristic is its type-determined instruction approach. Rather than having separate opcodes for each variant of an operation, COIL instructions derive their behavior from the types of their operands.

For example:
- `ADD result, a, b` performs integer addition if a and b are integers
- The same instruction performs floating-point addition if a and b are floating-point values
- The same instruction performs vector addition if a and b are vectors

This approach provides several benefits:
- Compact instruction set with fewer opcodes
- Consistent behavior across different data types
- Extensibility without opcode explosion
- Simplified instruction decoding

## Type Encoding

Types in COIL are encoded in a 16-bit format:

- **First 8 bits**: Main Type - Defines the primary type category
- **Second 8 bits**: Type Extensions - Provides qualifiers and additional type information

```
┌────────────┬────────────┐
│  Main Type │ Extensions │
│  (8 bits)  │  (8 bits)  │
└────────────┴────────────┘
```

Some types require additional data beyond the 16-bit descriptor:
- Composite types include structure definitions or array element types
- Register types include register identifiers
- Special types may include processor-specific data

## Type Categories

### Basic Numeric Types

#### Integer Types
Signed and unsigned integers of various widths:

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

#### Floating-Point Types
Various floating-point formats:

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x23    | TYPE_FP16    | 2 | IEEE 754 half precision |
| 0x25    | TYPE_FP32    | 4 | IEEE 754 single precision |
| 0x26    | TYPE_FP64    | 8 | IEEE 754 double precision |
| 0x28    | TYPE_FP128   | 16 | IEEE 754 quadruple precision |

### Vector Types

Vector types support SIMD operations and are universal across all processor types (not processor-specific):

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x30    | TYPE_V128 | 16 | 128-bit vector |
| 0x31    | TYPE_V256 | 32 | 256-bit vector |
| 0x32    | TYPE_V512 | 64 | 512-bit vector |

Vector types require element type specification:
```
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats (4 elements)
TYPE_V256=TYPE_INT16   ; 256-bit vector of 16-bit integers (16 elements)
```

The COIL processor automatically maps vector operations to native SIMD instructions when available or implements them using scalar operations when not available.

### Platform-Dependent Types

Types that automatically adjust based on the target platform:

| Type ID | Name | Size (Platform-dependent) | Description |
|---------|------|----------------------------|-------------|
| 0xA0    | TYPE_INT  | Varies | Default integer for current platform |
| 0xA1    | TYPE_UNT  | Varies | Default unsigned for current platform |
| 0xA2    | TYPE_FP   | Varies | Default float for current platform |
| 0xA6    | TYPE_PTR  | Varies | Default pointer size for current platform |

### Composite Types

Types composed of other types:

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0xD0    | TYPE_STRUCT | Varies | Structure type |
| 0xD1    | TYPE_PACK   | Varies | Packed structure (no padding) |
| 0xD2    | TYPE_UNION  | Varies | Union type |
| 0xD3    | TYPE_ARRAY  | Varies | Array type |

### Special Types

Types for special operations and references:

| Type ID | Name | Size (Varies) | Description |
|---------|------|---------------|-------------|
| 0x90    | TYPE_VAR | Varies | Variable reference |
| 0x91    | TYPE_SYM | Varies | Symbol reference |
| 0x92    | TYPE_RGP | Varies | General purpose register |
| 0x93    | TYPE_RFP | Varies | Floating point register |
| 0x94    | TYPE_RV  | Varies | Vector register |
| 0xFF    | TYPE_VOID | 0 | Void type (no value) |

### Parameter Types

Special types for instruction parameters:

| Type ID | Name | Description |
|---------|------|-------------|
| 0xFE    | TYPE_PARAM0 | General parameter (symbols, etc.) |
| 0xF8    | TYPE_ABICTL | ABI control parameter |
| 0xF0    | TYPE_PARAM5 | Conditions and flags |

## Type Extensions

Type extensions modify the basic type behavior through bit flags:

| Bit Position | Hex Value | Name | Description |
|--------------|-----------|------|-------------|
| Bit 0 | 0x01 | TYPEEXT_CONST    | Constant value (read-only) |
| Bit 1 | 0x02 | TYPEEXT_VOLATILE | Volatile access (not optimizable) |
| Bit 4 | 0x10 | TYPEEXT_VOID     | No Value |
| Bit 5 | 0x20 | TYPEEXT_IMM      | Immediate value |
| Bit 6 | 0x40 | TYPEEXT_VAR      | Variable ID |
| Bit 7 | 0x80 | TYPEEXT_SYM      | Symbol ID |

Example usage:
```
TYPE_INT32+CONST    ; Constant 32-bit integer (0x0301)
TYPE_PTR+VOLATILE   ; Volatile pointer (0xA602)
TYPE_FP32+IMM       ; Immediate 32-bit float (0x2520)
```

## Type Data Specification

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

## Type Compatibility Rules

Types are compatible under the following conditions:

1. The main types match exactly.
2. One type is a platform-dependent type and the other is its currently selected fixed-width equivalent.
3. Both types are integer types with the same signedness and the destination type has equal or greater width.
4. Both types are floating-point types and the destination type has equal or greater precision.
5. Both types are vector types with compatible element types and the destination has sufficient size.

## Type Conversion

COIL provides two primary mechanisms for type conversion:

1. **CONVERT Instruction**: Performs a value-preserving conversion that may involve rounding, truncation, or other operations.
   ```
   ; Integer to float conversion (preserves value)
   CONVERT float_value, int_value   ; float_value = 42.0f (from int 42)
   ```

2. **CAST Instruction**: Reinterprets the bit pattern without changing the underlying data.
   ```
   ; Reinterpret bit pattern
   CAST bit_pattern, float_value    ; View float bits as an integer
   ```

## Binary Encoding Examples

Type encoding in binary:

```
; TYPE_INT32
Binary: 0x03 0x00

; TYPE_INT32+CONST (constant)
Binary: 0x03 0x01

; TYPE_RGP=RAX (x86-64 general purpose register)
Binary: 0x92 0x00 0x00 (where 0x00 is the register ID for RAX)

; TYPE_ARRAY=TYPE_UNT8 (array of bytes)
Binary: 0xD3 0x00 0x10 0x00
```

## Related Documentation

- [Type Reference](../../reference/type-ref.md) - Comprehensive type system reference
- [Type Operations](../../instruction-set/type.md) - Instructions for working with types
- [Binary Format](../binary-format.md) - How types are encoded in binary format
- [CASM Syntax](../assembly/syntax.md) - How to use types in CASM