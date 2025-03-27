# COIL Type System

## Purpose

This document defines the foundational type system for COIL, establishing how data is represented and how type information influences instruction behavior. The type system is central to COIL's type-determined instruction philosophy.

## Key Concepts

- **Type-Determined Instructions**: How types define instruction behavior
- **Type Encoding**: How types are represented in binary format
- **Type Categories**: The major classifications of types
- **Type Compatibility**: Rules governing type conversions and operations
- **Type Extensions**: Modifiers that affect type behavior

## Type-Determined Instruction Philosophy

COIL's most distinctive characteristic is its type-determined instruction approach. Rather than having separate opcodes for each variant of an operation, COIL instructions derive their behavior from the types of their operands.

This approach provides several benefits:
- Compact instruction set with fewer opcodes
- Consistent behavior across different data types
- Extensibility without opcode explosion
- Simplified instruction decoding

For example, a single `ADD` instruction works across integers, floating-point values, and vectors, with behavior determined by operand types.

## Type Encoding

Types in COIL are encoded in a 16-bit format:
- First 8 bits: Main Type - Defines the primary type category
- Second 8 bits: Type Extensions - Provides qualifiers and additional type information

Some types require additional data beyond the 16-bit descriptor:
- Composite types include structure definitions or array element types
- Register types include register identifiers
- Special types may include processor-specific data

## Type Data Syntax

In COIL-ASM, type data can be specified in two ways:

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

## Type Categories

The type system organizes types into several major categories:

### Basic Types

#### Integer Types
Signed and unsigned integers of various widths (8, 16, 32, 64, 128 bits).

```
TYPE_INT8, TYPE_INT16, TYPE_INT32, TYPE_INT64, TYPE_INT128  ; Signed
TYPE_UNT8, TYPE_UNT16, TYPE_UNT32, TYPE_UNT64, TYPE_UNT128  ; Unsigned
```

#### Floating-Point Types
Various floating-point formats from 8-bit to 128-bit precision.

```
TYPE_FP8e5m2, TYPE_FP8e4m3  ; 8-bit formats
TYPE_FP16b, TYPE_FP16       ; 16-bit formats
TYPE_FP32t, TYPE_FP32       ; 32-bit formats
TYPE_FP64                   ; 64-bit double precision
TYPE_FP80, TYPE_FP128       ; Extended precision formats
```

#### Boolean Type
Single-bit boolean type for logical operations.

```
TYPE_BIT  ; 1-bit boolean, packed into bytes
```

### Vector Types
Fixed-size vectors for SIMD operations.

```
TYPE_V128  ; 128-bit vector
TYPE_V256  ; 256-bit vector
TYPE_V512  ; 512-bit vector
```

Vector types require element type specification:
```
TYPE_V128=TYPE_FP32  ; 128-bit vector of 32-bit floats (4 elements)
TYPE_V256=TYPE_INT16  ; 256-bit vector of 16-bit integers (16 elements)
```

### Platform-Dependent Types
Types that automatically adjust based on the target platform.

```
TYPE_INT  ; Default integer size for current platform
TYPE_UNT  ; Default unsigned size for current platform
TYPE_FP   ; Default float size for current platform
TYPE_PTR  ; Default pointer size for current platform
```

### Composite Types
Types composed of other types.

```
TYPE_STRUCT  ; Structure type
TYPE_PACK    ; Packed structure (no padding)
TYPE_UNION   ; Union type
TYPE_ARRAY   ; Array type
```

### Special Types
Types for symbolic and reference representation.

```
TYPE_VAR  ; Variable reference
TYPE_SYM  ; Symbol reference
```

### Register Types
Direct register references.

```
TYPE_RGP  ; General purpose register
TYPE_RFP  ; Floating point register
TYPE_RV   ; Vector register
```

### Parameter Types
Special types for instruction parameters.

```
TYPE_PARAM0 through TYPE_PARAM5  ; Parameter types
TYPE_ABICTL                      ; ABI control parameters
```

## Type Extensions

Type extensions modify the basic type behavior through bit flags:

```
TYPEEXT_CONST    = 0x01  ; Constant value (read-only)
TYPEEXT_VOLATILE = 0x02  ; Volatile access (not optimizable)
TYPEEXT_VOID     = 0x10  ; No value
TYPEEXT_IMM      = 0x20  ; Immediate value
TYPEEXT_VAR      = 0x40  ; Variable ID
TYPEEXT_SYM      = 0x80  ; Symbol ID
```

Example usage:
```
TYPE_INT32+CONST  ; Constant 32-bit integer (0x0301)
TYPE_PTR+VOLATILE ; Volatile pointer (0xA602)
TYPE_FP32+IMM     ; Immediate 32-bit float (0x2520)
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
2. **CAST Instruction**: Reinterprets the bit pattern without changing the underlying data.

### Conversion Examples

```
; Integer to float conversion (preserves value)
CONVERT float_value, int_value   ; float_value = 42.0f (from int 42)

; Float to integer conversion (with truncation)
CONVERT int_value, float_value   ; int_value = 3 (from 3.75f)

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
Binary: 0xD3 0x10 (where 0x10 is TYPE_UNT8)
```

## Related Documentation

- [Type Reference](../reference/type-ref.md) - Comprehensive type system reference
- [Type Operations](../instruction-set/type.md) - Instructions for working with types
- [Binary Format](../binary-format.md) - How types are encoded in binary format
- [CASM Syntax](../assembly/syntax.md) - How to use types in CASM