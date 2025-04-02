# COIL Type System

## Overview
The COIL type system is the foundation of COIL's "type-determined instruction" philosophy. Each instruction adapts its behavior based on the types of its operands, allowing for a compact instruction set that works consistently across data types.

## Type Encoding
Types in COIL use a 16-bit encoding format:
```
┌────────────┬────────────┐
│  Main Type │ Extensions │
│  (8 bits)  │  (8 bits)  │
└────────────┴────────────┘
```
Some complex types (like composite types) require additional data beyond this 16-bit descriptor.

## Type Categories

### Integer Types
| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x01    | TYPE_INT8   | 1 | 8-bit signed integer (-128 to 127) |
| 0x02    | TYPE_INT16  | 2 | 16-bit signed integer (-32,768 to 32,767) |
| 0x03    | TYPE_INT32  | 4 | 32-bit signed integer (-2,147,483,648 to 2,147,483,647) |
| 0x04    | TYPE_INT64  | 8 | 64-bit signed integer |
| 0x10    | TYPE_UNT8   | 1 | 8-bit unsigned integer (0 to 255) |
| 0x11    | TYPE_UNT16  | 2 | 16-bit unsigned integer (0 to 65,535) |
| 0x13    | TYPE_UNT32  | 4 | 32-bit unsigned integer (0 to 4,294,967,295) |
| 0x14    | TYPE_UNT64  | 8 | 64-bit unsigned integer |

### Floating-Point Types
| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x23    | TYPE_FP16    | 2 | Half precision (1 sign, 5 exponent, 10 mantissa) |
| 0x25    | TYPE_FP32    | 4 | Single precision (1 sign, 8 exponent, 23 mantissa) |
| 0x26    | TYPE_FP64    | 8 | Double precision (1 sign, 11 exponent, 52 mantissa) |
| 0x28    | TYPE_FP128   | 16 | Quadruple precision (1 sign, 15 exponent, 112 mantissa) |

### Vector Types
| Type ID | Name | Size (Bytes) | Description |
|---------|------|--------------|-------------|
| 0x30    | TYPE_V128 | 16 | 128-bit vector |
| 0x31    | TYPE_V256 | 32 | 256-bit vector |
| 0x32    | TYPE_V512 | 64 | 512-bit vector |

Vector types require an element type parameter:
```
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats (4 elements)
```

### Platform-Dependent Types
| Type ID | Name | Description |
|---------|------|-------------|
| 0xA0    | TYPE_INT  | Default integer (platform-dependent) |
| 0xA1    | TYPE_UNT  | Default unsigned integer |
| 0xA2    | TYPE_FP   | Default floating-point |
| 0xA6    | TYPE_PTR  | Pointer type (platform-dependent) |

### Reference Types
| Type ID | Name | Description |
|---------|------|-------------|
| 0x90    | TYPE_VAR | Reference to a variable |
| 0x91    | TYPE_SYM | Reference to a symbol |
| 0x92    | TYPE_RGP | General purpose register |
| 0x93    | TYPE_RFP | Floating point register |
| 0x94    | TYPE_RV  | Vector register |

### Composite Types
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

### Special Types
| Type ID | Name | Description |
|---------|------|-------------|
| 0xFA    | TYPE_PARAM4 | check function for params real type |
| 0xFB    | TYPE_PARAM3 | check function for params real type |
| 0xFC    | TYPE_PARAM2 | check function for params real type |
| 0xFD    | TYPE_PARAM1 | check function for params real type |
| 0xFE    | TYPE_PARAM0 | check function for params real type |

### Special Types
| Type ID | Name | Description |
|---------|------|-------------|
| 0x40    | TYPE_BIT | Bit (boolean) type, 1/8 byte |
| 0xFF    | TYPE_VOID | Void type (no value) |

## Type Extensions

| Bit | Hex Value | Name | Description |
|-----|-----------|------|-------------|
| 0   | 0x01 | CONST    | Constant (read-only) |
| 1   | 0x02 | VOLATILE | Volatile (not optimizable) |
| 5   | 0x20 | IMM      | Immediate value |
| 6   | 0x40 | VAR      | Variable ID |
| 7   | 0x80 | SYM      | Symbol ID |

Examples:
```
0x0301 - TYPE_INT32+CONST  ; Constant 32-bit integer
0x2520 - TYPE_FP32+IMM     ; Immediate 32-bit float
```

## Type Compatibility Rules

### Compatible Types
Types are compatible when:
- They match exactly
- Platform type matches its fixed-width equivalent (e.g., TYPE_INT to TYPE_INT32 on 32-bit platform)
- Both integers with same signedness and destination has equal/greater width
- Both floating-point and destination has equal/greater precision

### Incompatible Types
Types are incompatible when:
- Different signedness without explicit conversion
- Destination has fewer bits without explicit conversion
- Different type categories without explicit conversion

## Type Conversion

Two conversion mechanisms are provided:

1. **CONVERT Instruction**: Value-preserving conversion
   ```
   CONVERT float_result, int_value    ; Convert integer to float
   ```

2. **CAST Instruction**: Reinterprets bit pattern without changing bits
   ```
   CAST int_bits, float_value         ; View float bits as integer
   ```

## Binary Encoding Examples

```
; TYPE_INT32 (32-bit integer)
0x0300

; TYPE_FP64 (64-bit float)
0x2600

; TYPE_INT32+CONST (constant 32-bit integer)
0x0301

; TYPE_PTR=TYPE_INT32 (pointer to 32-bit integer)
0xA600 0x0300

; TYPE_V128=TYPE_FP32 (vector of 32-bit floats)
0x3000 0x2500
```

## Related Components

- [Instruction Reference](/coil-docs/reference/instruction-reference.md) - Instructions that use types
- [Memory and Variables](/coil-docs/systems/memory-and-variables.md) - Variable type system usage
- [CASM Basics](/casm-docs/basics/casm-basics.md) - Type syntax in CASM