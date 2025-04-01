# COIL Type System

## Overview

The COIL type system is the foundation of COIL's type-determined instruction philosophy. It defines how data is represented and how instruction behavior is influenced by operand types.

## Type Encoding

Types use a 16-bit encoding format:
```
┌────────────┬────────────┐
│  Main Type │ Extensions │
│  (8 bits)  │  (8 bits)  │
└────────────┴────────────┘
```

Some types require additional data beyond this 16-bit descriptor.

## Basic Types

### Integer Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x01    | TYPE_INT8   | 1 | 8-bit signed integer |
| 0x02    | TYPE_INT16  | 2 | 16-bit signed integer |
| 0x03    | TYPE_INT32  | 4 | 32-bit signed integer |
| 0x04    | TYPE_INT64  | 8 | 64-bit signed integer |
| 0x10    | TYPE_UNT8   | 1 | 8-bit unsigned integer |
| 0x11    | TYPE_UNT16  | 2 | 16-bit unsigned integer |
| 0x13    | TYPE_UNT32  | 4 | 32-bit unsigned integer |
| 0x14    | TYPE_UNT64  | 8 | 64-bit unsigned integer |

### Floating-Point Types

| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x23    | TYPE_FP16    | 2 | Half precision |
| 0x25    | TYPE_FP32    | 4 | Single precision |
| 0x26    | TYPE_FP64    | 8 | Double precision |
| 0x28    | TYPE_FP128   | 16 | Quadruple precision |

### Platform-Dependent Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0xA0    | TYPE_INT  | Default integer (platform-dependent) |
| 0xA1    | TYPE_UNT  | Default unsigned integer |
| 0xA2    | TYPE_FP   | Default floating-point |
| 0xA6    | TYPE_PTR  | Pointer type (platform-dependent) |

## Vector Types

Vector types enable SIMD operations and specify element type:

| Type ID | Name | Size | Description |
|---------|------|------|-------------|
| 0x30    | TYPE_V128 | 16 bytes | 128-bit vector |
| 0x31    | TYPE_V256 | 32 bytes | 256-bit vector |
| 0x32    | TYPE_V512 | 64 bytes | 512-bit vector |

Vector operations work consistently across platforms and are mapped to native SIMD instructions when available.

Examples:
```
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats (4 elements)
TYPE_V256=TYPE_INT16   ; 256-bit vector of 16-bit integers (16 elements)
```

## Special Types

### Reference Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x90    | TYPE_VAR | Reference to a variable |
| 0x91    | TYPE_SYM | Reference to a symbol |

### Register Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0x92    | TYPE_RGP | General purpose register |
| 0x93    | TYPE_RFP | Floating point register |
| 0x94    | TYPE_RV  | Vector register |

### Parameter Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0xF8    | TYPE_ABICTL | ABI control parameter |
| 0xF0    | TYPE_PARAM5 | Conditions and flags |
| 0xFE    | TYPE_PARAM0 | General purpose parameter |

## Composite Types

| Type ID | Name | Description |
|---------|------|-------------|
| 0xD0    | TYPE_STRUCT | Structure type |
| 0xD3    | TYPE_ARRAY  | Array type |
| 0xD2    | TYPE_UNION  | Union type |

Examples:
```
TYPE_STRUCT=point     ; Structure named "point"
TYPE_ARRAY=TYPE_UNT8  ; Array of unsigned 8-bit integers
```

## Type Extensions

| Bit | Hex Value | Name | Description |
|-----|-----------|------|-------------|
| 0   | 0x01 | CONST    | Constant (read-only) |
| 1   | 0x02 | VOLATILE | Volatile (not optimizable) |
| 5   | 0x20 | IMM      | Immediate value |

Examples:
```
0x0301 - TYPE_INT32+CONST  ; Constant 32-bit integer
0x2520 - TYPE_FP32+IMM     ; Immediate 32-bit float
```

## Type Compatibility and Conversion

### Compatible Types

Types are compatible when:
- They match exactly
- Platform type matches its fixed-width equivalent
- Both integers with same signedness and destination has equal/greater width
- Both floating-point and destination has equal/greater precision

### Type Conversion

Two primary conversion mechanisms:

1. **CONVERT Instruction**: Value-preserving conversion
   ```
   CONVERT float_result, int_value    ; Convert integer to float
   ```

2. **CAST Instruction**: Reinterprets bit pattern
   ```
   CAST int_bits, float_value         ; View float bits as integer
   ```

## Examples

```
; Basic type declarations
VAR #1, TYPE_INT32, 0       ; 32-bit integer initialized to 0
VAR #2, TYPE_FP64           ; 64-bit float (uninitialized)
VAR #3, TYPE_PTR=TYPE_INT32 ; Pointer to 32-bit integer

; Vector type
VAR #4, TYPE_V128=TYPE_FP32, (1.0, 2.0, 3.0, 4.0)

; Using different types with the same instruction
ADD #1, #1, 1               ; Integer addition
ADD #2, #2, 3.14            ; Floating-point addition
ADD #4, #4, #4              ; Vector addition
```