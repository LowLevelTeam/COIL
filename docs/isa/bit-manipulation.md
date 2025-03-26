# Bit Manipulation Instructions (0x50-0x5F)

## Purpose

This document defines the bit manipulation instructions in COIL, which provide operations for logical operations, shifts, rotations, and other bit-level manipulations.

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

#### Assembly Syntax
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
AND result, value, mask

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
AND flags, 0xF0

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

#### Assembly Syntax
```
OR dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
OR dest, src[, TYPE_PARAM5=condition]  ; Equivalent to OR dest, dest, src
```

#### Binary Encoding
```
0x51                  ; Opcode for OR
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
OR result, flags, new_flags

; Binary
0x51      ; OR
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "flags"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "new_flags"

; Assembly (two-operand form)
OR status, 0x01

; Binary
0x51      ; OR
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "status"
0x1320    ; TYPE_UNT32+IMM
0x01000000 ; Value 0x01
```

### XOR (0x52)
Perform bitwise XOR (exclusive OR) operation.

#### Assembly Syntax
```
XOR dest, left, right[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
XOR dest, src[, TYPE_PARAM5=condition]  ; Equivalent to XOR dest, dest, src
```

#### Binary Encoding
```
0x52                  ; Opcode for XOR
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
XOR result, value1, value2

; Binary
0x52      ; XOR
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value1"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value2"

; Assembly (two-operand form - toggle all bits)
XOR value, 0xFFFFFFFF

; Binary
0x52      ; XOR
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1320    ; TYPE_UNT32+IMM
0xFFFFFFFF ; Value 0xFFFFFFFF
```

### NOT (0x53)
Perform bitwise NOT (complement) operation.

#### Assembly Syntax
```
NOT dest, src[, TYPE_PARAM5=condition]
```

One-operand form (implied destination):
```
NOT value[, TYPE_PARAM5=condition]  ; Equivalent to NOT value, value
```

#### Binary Encoding
```
0x53                  ; Opcode for NOT
0x01/0x02/0x03        ; One, two, or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source (if two-operand form)
[src_value]           ; Source value (if two-operand form)
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (two-operand form)
NOT result, value

; Binary
0x53      ; NOT
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"

; Assembly (one-operand form)
NOT flags

; Binary
0x53      ; NOT
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "flags"
```

### SHL (0x54)
Shift bits left.

#### Assembly Syntax
```
SHL dest, value, shift[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SHL dest, shift[, TYPE_PARAM5=condition]  ; Equivalent to SHL dest, dest, shift
```

#### Binary Encoding
```
0x54                  ; Opcode for SHL
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[value_type]          ; Type of value to shift (if three-operand form)
[value_value]         ; Value to shift (if three-operand form)
[shift_type]          ; Type of shift amount
[shift_value]         ; Shift amount value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (three-operand form)
SHL result, value, 2

; Binary
0x54      ; SHL
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1020    ; TYPE_UNT8+IMM
0x02      ; Shift amount 2

; Assembly (two-operand form)
SHL value, 1

; Binary
0x54      ; SHL
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1020    ; TYPE_UNT8+IMM
0x01      ; Shift amount 1
```

### SHR (0x55)
Shift bits right logical (zeros shifted in from the left).

#### Assembly Syntax
```
SHR dest, value, shift[, TYPE_PARAM5=condition]
```

Two-operand form (implied first operand):
```
SHR dest, shift[, TYPE_PARAM5=condition]  ; Equivalent to SHR dest, dest, shift
```

#### Binary Encoding
```
0x55                  ; Opcode for SHR
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[value_type]          ; Type of value to shift (if three-operand form)
[value_value]         ; Value to shift (if three-operand form)
[shift_type]          ; Type of shift amount
[shift_value]         ; Shift amount value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (three-operand form)
SHR result, value, 3

; Binary
0x55      ; SHR
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1020    ; TYPE_UNT8+IMM
0x03      ; Shift amount 3
```

### POPCNT (0x59)
Count the number of set bits (population count).

#### Assembly Syntax
```
POPCNT dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x59                  ; Opcode for POPCNT
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
POPCNT count, value

; Binary
0x59      ; POPCNT
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "count"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
```

### BSWAP (0x5A)
Swap the byte order (endianness conversion).

#### Assembly Syntax
```
BSWAP dest, src[, TYPE_PARAM5=condition]
```

One-operand form (implied destination):
```
BSWAP value[, TYPE_PARAM5=condition]  ; Equivalent to BSWAP value, value
```

#### Binary Encoding
```
0x5A                  ; Opcode for BSWAP
0x01/0x02/0x03        ; One, two, or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source (if two-operand form)
[src_value]           ; Source value (if two-operand form)
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (two-operand form)
BSWAP result, value

; Binary
0x5A      ; BSWAP
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
```

## Bit Manipulation Patterns

### Bit Testing
```
; Test if bit 3 is set
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, bit_mask, 0x00000008    ; 1 << 3
AND tmp, value, bit_mask
CMP tmp, 0
BR_EQ bit_not_set
```

Binary encoding (simplified, showing key instructions):
```
0x50      ; AND
0x03      ; Three operands
0x9000    ; TYPE_VAR (tmp)
[id]      ; Variable ID
0x9000    ; TYPE_VAR (value)
[id]      ; Variable ID
0x9000    ; TYPE_VAR (bit_mask)
[id]      ; Variable ID

0x05      ; CMP
0x02      ; Two operands
0x9000    ; TYPE_VAR (tmp)
[id]      ; Variable ID
0x1320    ; TYPE_UNT32+IMM
0x00000000 ; Value 0
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