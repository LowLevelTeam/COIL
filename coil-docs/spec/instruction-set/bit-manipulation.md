# Bit Manipulation Instructions (0x50-0x5F)

## Purpose

This document defines the bit manipulation instructions in COIL, which provide operations for logical operations, shifts, rotations, and other bit-level manipulations. These instructions enable fine-grained control over the bits that make up data values.

## Key Concepts

- **Logical Operations**: Bitwise AND, OR, XOR, and NOT operations
- **Shift Operations**: Moving bits left or right within a value
- **Rotation Operations**: Circular shifts of bits
- **Bit Analysis**: Counting and examining bits
- **Endianness Conversion**: Reordering bytes within a value

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x50   | AND      | `AND dest, left, right[, TYPE_PARAM5]` | Bitwise AND |
| 0x51   | OR       | `OR dest, left, right[, TYPE_PARAM5]` | Bitwise OR |
| 0x52   | XOR      | `XOR dest, left, right[, TYPE_PARAM5]` | Bitwise XOR |
| 0x53   | NOT      | `NOT dest, src[, TYPE_PARAM5]` | Bitwise NOT |
| 0x54   | SHL      | `SHL dest, value, shift[, TYPE_PARAM5]` | Shift left |
| 0x55   | SHR      | `SHR dest, value, shift[, TYPE_PARAM5]` | Shift right logical |
| 0x56   | SAR      | `SAR dest, value, shift[, TYPE_PARAM5]` | Shift right arithmetic |
| 0x57   | ROL      | `ROL dest, value, rotate[, TYPE_PARAM5]` | Rotate left |
| 0x58   | ROR      | `ROR dest, value, rotate[, TYPE_PARAM5]` | Rotate right |
| 0x59   | POPCNT   | `POPCNT dest, src[, TYPE_PARAM5]` | Count set bits |
| 0x5A   | BSWAP    | `BSWAP dest, src[, TYPE_PARAM5]` | Byte swap |
| 0x5B   | CLZ      | `CLZ dest, src[, TYPE_PARAM5]` | Count leading zeros |
| 0x5C   | CTZ      | `CTZ dest, src[, TYPE_PARAM5]` | Count trailing zeros |

## Detailed Descriptions

### AND (0x50)
Perform bitwise AND operation.

#### Syntax
```
AND dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
AND dest, src[, TYPE_PARAM5=condition]  ; Equivalent to AND dest, dest, src
```

#### Binary Encoding
```
0x50                  ; Opcode for AND
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[left_type]           ; Type of left operand (if three-operand form)
[left_value]          ; Left operand value (if three-operand form)
[right_type]          ; Type of right operand
[right_value]         ; Right operand value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (three-operand form)
AND result, value, mask       ; result = value & mask

; Binary
0x50      ; AND
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "mask"

; Assembly (two-operand form)
AND flags, 0xF0               ; flags = flags & 0xF0

; Binary
0x50      ; AND
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "flags"
0x1320    ; TYPE_UNT32+IMM
0xF0000000 ; Value 0xF0
```

### OR (0x51)
Perform bitwise OR operation.

#### Syntax
```
OR dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
OR dest, src[, TYPE_PARAM5=condition]  ; Equivalent to OR dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
OR result, flags, new_flags   ; result = flags | new_flags

; Assembly (two-operand form)
OR status, 0x01               ; status = status | 0x01
```

### XOR (0x52)
Perform bitwise XOR (exclusive OR) operation.

#### Syntax
```
XOR dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
XOR dest, src[, TYPE_PARAM5=condition]  ; Equivalent to XOR dest, dest, src
```

#### Example
```
; Assembly (three-operand form)
XOR result, value1, value2    ; result = value1 ^ value2

; Assembly (two-operand form - toggle all bits)
XOR value, 0xFFFFFFFF         ; value = value ^ 0xFFFFFFFF
```

### NOT (0x53)
Perform bitwise NOT (complement) operation.

#### Syntax
```
NOT dest, src[, TYPE_PARAM5=condition]
```

One-operand form (implied destination):
```
NOT value[, TYPE_PARAM5=condition]  ; Equivalent to NOT value, value
```

#### Example
```
; Assembly (two-operand form)
NOT result, value             ; result = ~value

; Assembly (one-operand form)
NOT flags                     ; flags = ~flags
```

### SHL (0x54)
Shift bits left.

#### Syntax
```
SHL dest, value, shift[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SHL dest, shift[, TYPE_PARAM5=condition]  ; Equivalent to SHL dest, dest, shift
```

#### Example
```
; Assembly (three-operand form)
SHL result, value, 2          ; result = value << 2

; Assembly (two-operand form)
SHL value, 1                  ; value = value << 1
```

### SHR (0x55)
Shift bits right logical (zeros shifted in from the left).

#### Syntax
```
SHR dest, value, shift[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SHR dest, shift[, TYPE_PARAM5=condition]  ; Equivalent to SHR dest, dest, shift
```

#### Example
```
; Assembly (three-operand form)
SHR result, value, 3          ; result = value >> 3 (logical)

; Assembly (two-operand form)
SHR value, shift_amount       ; value = value >> shift_amount (logical)
```

### POPCNT (0x59)
Count the number of set bits (population count).

#### Syntax
```
POPCNT dest, src[, TYPE_PARAM5=condition]
```

#### Example
```
; Assembly
POPCNT count, value           ; count = number of 1 bits in value
```

### BSWAP (0x5A)
Swap the byte order (endianness conversion).

#### Syntax
```
BSWAP dest, src[, TYPE_PARAM5=condition]
```

One-operand form (implied destination):
```
BSWAP value[, TYPE_PARAM5=condition]  ; Equivalent to BSWAP value, value
```

#### Example
```
; Assembly (two-operand form)
BSWAP result, value           ; result = byte-swapped value (endianness conversion)

; Assembly (one-operand form)
BSWAP network_value           ; Convert between network and host byte order
```

## Bit Manipulation Patterns

### Bit Testing
```
; Test if bit 3 is set
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, bit_mask, 0x00000008    ; 1 << 3
VAR TYPE_UNT32, tmp

AND tmp, value, bit_mask
CMP tmp, 0
BR_EQ bit_not_set
```

### Setting, Clearing, and Toggling Bits
```
; Set bit 4
OR value, value, 0x00000010    ; 1 << 4

; Clear bit 2
AND value, value, 0xFFFFFFFB   ; ~(1 << 2)

; Toggle bit 5
XOR value, value, 0x00000020   ; 1 << 5
```

### Extracting Bit Fields
```
; Extract bits 4-7 from a value
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, result

; Shift right to position the field at bit 0
SHR result, value, 4           ; Move field to right

; Mask off unwanted bits
AND result, result, 0x0F       ; Keep only 4 bits
```

### Inserting Bit Fields
```
; Insert 4-bit field (field_value) at position 8
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, field_value, 0x0A       ; Field to insert
VAR TYPE_UNT32, field_mask, 0x00000F00  ; Mask for the field position
VAR TYPE_UNT32, field_pos, 8            ; Position of the field

; Clear the target bits
AND value, value, 0xFFFFF0FF   ; ~field_mask

; Position the field
SHL field_value, field_value, field_pos

; Insert the field
OR value, value, field_value
```

### Parity Calculation
```
; Calculate parity (1 if odd number of bits set, 0 if even)
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT8, parity

; Get population count
POPCNT bit_count, value

; Check if odd or even
AND parity, bit_count, 1       ; Parity = bit_count & 1
```

### Bit Reversal
```
; Reverse the bits in a byte
VAR TYPE_UNT8, value, 0x3E     ; 00111110
VAR TYPE_UNT8, result, 0

; Process each bit
VAR TYPE_UNT8, i, 0
VAR TYPE_UNT8, bit

SYM reverse_loop
CMP i, 8
BR_GE reverse_done

; Extract bit from source
SHR bit, value, i
AND bit, bit, 1

; Place bit in result
SHL result, result, 1
OR result, result, bit

INC i
BR reverse_loop

SYM reverse_done
; result now contains 01111100 (0x7C)
```

## Implementation Considerations

### Hardware Acceleration

Many processors provide hardware instructions for bit operations:
- Bit scanning (BSF, BSR in x86)
- Population count (POPCNT in x86, VCNT in ARM)
- Byte swapping (BSWAP in x86, REV in ARM)

COIL processors should map to these where available.

### Software Fallbacks

For operations without hardware support, efficient algorithms exist:
- Population count using parallel bit counting
- Leading/trailing zero count using binary search
- Bit manipulation using lookup tables

## Related Documentation

- [CASM Instructions](../../assembly/instructions.md) - General guide to CASM instructions
- [Arithmetic Instructions](arithmetic.md) - Numeric operations
- [Type System](../../systems/type-system.md) - Type rules for bit operations
- [Binary Format](../../binary-format.md) - Binary encoding details