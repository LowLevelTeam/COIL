# Bit Manipulation Instructions (0x50-0x5F)

## Purpose

This document defines the bit manipulation instructions in COIL, which perform bitwise logical operations, shifts, rotations, and other bit-level manipulations. These instructions provide fundamental capabilities for working with data at the bit level across all processor types.

## Logical Operations

### AND (0x50)
Bitwise AND of two values.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

The AND instruction can be used with two or three operands:
- Three operands: `AND dst, src1, src2` (dst = src1 & src2)
- Two operands: `AND dst, src` (dst = dst & src)

Example:
```
; Three-operand form
VAR TYPE_UNT32, result
AND result, flags, mask

; Two-operand form (implied first operand)
AND status_flags, 0xF0

; AND with condition
AND_NZ result, value, mask
```

Binary encoding:
```
0x50      ; Opcode for AND
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### OR (0x51)
Bitwise OR of two values.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
OR result, flags, new_flags    ; result = flags | new_flags

; Two-operand form
OR status_flags, 0x03          ; status_flags = status_flags | 0x03

; OR with condition
OR_NZ status_flags, FLAG_READY
```

Binary encoding:
```
0x51      ; Opcode for OR
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### XOR (0x52)
Bitwise XOR (exclusive OR) of two values.

```
Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
XOR result, a, b               ; result = a ^ b

; Two-operand form
XOR value, 0xFFFFFFFF          ; Toggle all bits (complement)

; XOR with condition
XOR_NZ checksum, data_byte
```

Binary encoding:
```
0x52      ; Opcode for XOR
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of left operand
[value2]  ; Left operand value
[type3]   ; Type of right operand (if three-operand form)
[value3]  ; Right operand value (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### NOT (0x53)
Bitwise complement (flip all bits).

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Two-operand form
NOT result, value              ; result = ~value

; One-operand form (implied first operand)
NOT flags                      ; flags = ~flags

; NOT with condition
NOT_NZ result, value
```

Binary encoding:
```
0x53      ; Opcode for NOT
0x01/0x02/0x03 ; One, two, or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source (if two-operand form)
[value2]  ; Source value (if two-operand form)
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Shift and Rotate Operations

### SHL (0x54)
Logical left shift (zeros shifted in from the right).

```
Operands:
- Destination: Non TYPE_VOID
- Value: Non TYPE_VOID
- Shift: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
SHL result, value, 2           ; result = value << 2

; Two-operand form (implied first operand)
SHL value, 1                   ; value = value << 1

; SHL with variable shift amount
VAR TYPE_UNT8, shift_amount, 3
SHL result, value, shift_amount

; SHL with condition
SHL_NZ result, value, 4
```

Binary encoding:
```
0x54      ; Opcode for SHL
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of value to shift
[value2]  ; Value to shift
[type3]   ; Type of shift amount (if three-operand form)
[value3]  ; Shift amount (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### SHR (0x55)
Logical right shift (zeros shifted in from the left).

```
Operands:
- Destination: Non TYPE_VOID
- Value: Non TYPE_VOID
- Shift: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
SHR result, value, 3           ; result = value >> 3 (logical)

; Two-operand form
SHR flags, 4                   ; flags = flags >> 4 (logical)

; SHR with variable shift amount
VAR TYPE_UNT8, shift_amount, 2
SHR result, value, shift_amount

; SHR with condition
SHR_NZ result, value, 8
```

Binary encoding:
```
0x55      ; Opcode for SHR
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of value to shift
[value2]  ; Value to shift
[type3]   ; Type of shift amount (if three-operand form)
[value3]  ; Shift amount (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### SAR (0x56)
Arithmetic right shift (sign bit replicated from the left).

```
Operands:
- Destination: Non TYPE_VOID
- Value: Non TYPE_VOID
- Shift: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
SAR result, value, 2           ; result = value >> 2 (arithmetic)

; Two-operand form
SAR signed_value, 1            ; signed_value = signed_value >> 1 (arithmetic)

; SAR with variable shift amount
VAR TYPE_UNT8, shift_amount, 3
SAR result, signed_value, shift_amount

; SAR with condition
SAR_NZ result, value, 1
```

Binary encoding:
```
0x56      ; Opcode for SAR
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of value to shift
[value2]  ; Value to shift
[type3]   ; Type of shift amount (if three-operand form)
[value3]  ; Shift amount (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### ROL (0x57)
Rotate left (bits shifted out from the left are shifted in from the right).

```
Operands:
- Destination: Non TYPE_VOID
- Value: Non TYPE_VOID
- Rotate: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
ROL result, value, 3           ; result = (value << 3) | (value >> (bits - 3))

; Two-operand form
ROL value, 1                   ; value = (value << 1) | (value >> (bits - 1))

; ROL with variable rotation amount
VAR TYPE_UNT8, rotate_amount, 4
ROL result, value, rotate_amount

; ROL with condition
ROL_NZ result, value, 2
```

Binary encoding:
```
0x57      ; Opcode for ROL
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of value to rotate
[value2]  ; Value to rotate
[type3]   ; Type of rotation amount (if three-operand form)
[value3]  ; Rotation amount (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### ROR (0x58)
Rotate right (bits shifted out from the right are shifted in from the left).

```
Operands:
- Destination: Non TYPE_VOID
- Value: Non TYPE_VOID
- Rotate: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Three-operand form
ROR result, value, 2           ; result = (value >> 2) | (value << (bits - 2))

; Two-operand form
ROR value, 1                   ; value = (value >> 1) | (value << (bits - 1))

; ROR with variable rotation amount
VAR TYPE_UNT8, rotate_amount, 3
ROR result, value, rotate_amount

; ROR with condition
ROR_NZ result, value, 4
```

Binary encoding:
```
0x58      ; Opcode for ROR
0x02/0x03/0x04 ; Two, three, or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of value to rotate
[value2]  ; Value to rotate
[type3]   ; Type of rotation amount (if three-operand form)
[value3]  ; Rotation amount (if three-operand form)
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Advanced Bit Operations

### POPCNT (0x59)
Count set bits (population count).

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Count number of set bits
POPCNT result, value           ; result = number of 1 bits in value

; POPCNT with condition
POPCNT_NZ result, value
```

Binary encoding:
```
0x59      ; Opcode for POPCNT
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### BSWAP (0x5A)
Byte swap (endianness conversion).

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Convert between endianness
BSWAP result, value            ; Swap byte order of value

; In-place byte swap
BSWAP value, value

; BSWAP with condition
BSWAP_NZ result, value
```

Binary encoding:
```
0x5A      ; Opcode for BSWAP
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### CLZ (0x5B)
Count leading zeros.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Count leading zeros
CLZ result, value              ; result = number of leading zeros in value

; CLZ with condition
CLZ_NZ result, value
```

Binary encoding:
```
0x5B      ; Opcode for CLZ
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### CTZ (0x5C)
Count trailing zeros.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Count trailing zeros
CTZ result, value              ; result = number of trailing zeros in value

; CTZ with condition
CTZ_NZ result, value
```

Binary encoding:
```
0x5C      ; Opcode for CTZ
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Bit Manipulation Patterns

### Bit Testing and Manipulation

#### Test a Specific Bit
```
; Test if bit 3 is set
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, bit_mask, 0x00000008    ; 1 << 3
AND tmp, value, bit_mask
CMP tmp, 0
BR_EQ bit_not_set                       ; Branch if bit is not set
```

#### Set a Specific Bit
```
; Set bit 4
VAR TYPE_UNT32, value, 0x00000000
VAR TYPE_UNT32, bit_mask, 0x00000010    ; 1 << 4
OR value, value, bit_mask               ; Set the bit
```

#### Clear a Specific Bit
```
; Clear bit 2
VAR TYPE_UNT32, value, 0xFFFFFFFF
VAR TYPE_UNT32, bit_mask, 0x00000004    ; 1 << 2
VAR TYPE_UNT32, inverted_mask
NOT inverted_mask, bit_mask             ; Create mask with only bit 2 cleared
AND value, value, inverted_mask         ; Clear the bit
```

#### Toggle a Specific Bit
```
; Toggle bit 5
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, bit_mask, 0x00000020    ; 1 << 5
XOR value, value, bit_mask              ; Toggle the bit
```

### Common Bit Manipulation Techniques

#### Extract Bit Field
```
; Extract bits 4-7 from a value
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT32, result

; Shift right to position the field at bit 0
SHR result, value, 4                    ; Move the field to the rightmost position

; Mask off unwanted bits
AND result, result, 0x0F                ; Keep only the lowest 4 bits (bits 4-7 of original)
```

#### Insert Bit Field
```
; Insert 4-bit value into bits 8-11
VAR TYPE_UNT32, target, 0x12345678      ; Target value to modify
VAR TYPE_UNT32, field, 0x0A             ; 4-bit value to insert
VAR TYPE_UNT32, result
VAR TYPE_UNT32, mask, 0x00000F00        ; Mask for bits 8-11

; Clear the target bits
NOT tmp_mask, mask
AND result, target, tmp_mask            ; Clear bits 8-11

; Position the field
SHL tmp_field, field, 8                 ; Shift field to bits 8-11

; Combine
OR result, result, tmp_field            ; Insert the field
```

#### Bit Reversal
```
; Reverse the bits in a byte
VAR TYPE_UNT8, value, 0x3E              ; 00111110 in binary
VAR TYPE_UNT8, result, 0

; Manual bit reversal for a byte
VAR TYPE_UNT8, bit_pos, 0
SYM reverse_loop
    CMP bit_pos, 8
    BR_GE reverse_done
    
    ; Extract current bit
    VAR TYPE_UNT8, mask, 1
    SHL bit_mask, mask, bit_pos
    AND current_bit, value, bit_mask
    
    ; If bit is set, set corresponding bit in result
    CMP current_bit, 0
    BR_EQ bit_not_set
    
    VAR TYPE_UNT8, result_bit_pos
    SUB result_bit_pos, 7, bit_pos      ; Reverse position: 7-bit_pos
    
    VAR TYPE_UNT8, result_mask, 1
    SHL result_mask, result_mask, result_bit_pos
    OR result, result, result_mask
    
SYM bit_not_set
    INC bit_pos
    BR reverse_loop
    
SYM reverse_done
```

#### Parity Calculation
```
; Calculate parity of a value (1 if odd number of bits set, 0 if even)
VAR TYPE_UNT32, value, 0x12345678
VAR TYPE_UNT8, parity

; Get population count (number of bits set)
POPCNT bit_count, value

; Check if odd or even
AND parity, bit_count, 1     ; Parity = bit_count & 1
```

### Optimization Techniques

#### Multiply by Powers of 2
```
; Multiply by 8 (2^3)
VAR TYPE_UNT32, value, 10
VAR TYPE_UNT32, result
SHL result, value, 3         ; result = value * 8
```

#### Divide by Powers of 2
```
; Divide by 4 (2^2)
VAR TYPE_UNT32, value, 20
VAR TYPE_UNT32, result
SHR result, value, 2         ; result = value / 4 (unsigned)
SAR result, value, 2         ; result = value / 4 (signed, with rounding toward negative infinity)
```

#### Check if Power of 2
```
; Check if value is a power of 2
VAR TYPE_UNT32, value, 16
VAR TYPE_UNT32, tmp
VAR TYPE_BIT, is_power_of_2

; A power of 2 has exactly one bit set and value > 0
CMP value, 0
BR_EQ not_power_of_2         ; 0 is not a power of 2

; value & (value - 1) is 0 for powers of 2
VAR TYPE_UNT32, value_minus_1
SUB value_minus_1, value, 1
AND tmp, value, value_minus_1
CMP tmp, 0
BR_NE not_power_of_2

; Value is a power of 2
MOV is_power_of_2, 1
BR end_check

SYM not_power_of_2
MOV is_power_of_2, 0

SYM end_check
```

#### Align to Power of 2
```
; Align value up to nearest multiple of 16
VAR TYPE_UNT32, value, 42
VAR TYPE_UNT32, result
VAR TYPE_UNT32, alignment, 16
VAR TYPE_UNT32, mask

; Create alignment mask
SUB mask, alignment, 1       ; mask = 15 (0x0F)
NOT mask, mask               ; mask = ~0x0F (0xFFFFFFF0)

; Add (alignment-1) and mask
ADD result, value, 15        ; result = value + (alignment - 1)
AND result, result, mask     ; result = (value + (alignment - 1)) & ~(alignment - 1)
```

## Processor-Specific Considerations

While bit manipulation instructions are universal across all processor types, implementation details may vary:

1. **Instruction Mapping**:
   - On processors with dedicated bit manipulation instructions, COIL maps directly to those instructions
   - On processors without specific hardware support, COIL implements operations using available instructions

2. **Performance Considerations**:
   - Certain bit operations may be more efficient on specific processor types
   - For maximum performance, consider processor characteristics in performance-critical code

3. **Width Dependence**:
   - Behavior for shifts/rotates with counts larger than operand width is processor-dependent
   - For portable code, use shift/rotate amounts less than the operand width

4. **Endianness Effects**:
   - BSWAP behavior is consistent across all processors (swaps byte order)
   - Bit-level operations like shifts might require adjustment on big-endian processors for byte-oriented results

The COIL processor automatically handles these differences, but understanding them can help write more efficient code.