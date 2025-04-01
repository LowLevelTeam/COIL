# COIL Instruction Reference

## Core Instruction Categories

COIL instructions are organized into functional categories:

| Opcode Range | Category | Description |
|--------------|----------|-------------|
| 0x00         | NOP      | No operation |
| 0x01-0x0F    | Control Flow | Program flow, branches, calls |
| 0x10-0x2F    | Memory Operations | Data movement, variables, memory |
| 0x50-0x5F    | Bit Manipulation | Logical operations, bit-level |
| 0x60-0x8F    | Arithmetic | Mathematical operations |
| 0x90-0x9F    | Vector/Array | Vector and array operations |
| 0xA0-0xAF    | Type Operations | Type management |
| 0xB0-0xBF    | Directives | Assembly control |
| 0xC0-0xFE    | Platform-Specific | Architecture-specific operations |
| 0xFF         | Extensions | Implementation-specific extensions |

## Control Flow Instructions

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x01   | SYM      | symbol_name [, attributes] | Define a symbol |
| 0x02   | BR       | target [, condition] | Branch to target |
| 0x03   | CALL     | target [, parameters] | Call function |
| 0x04   | RET      | [return_values] | Return from function |
| 0x05   | CMP      | left, right | Compare values |
| 0x09   | SWITCH   | value, cases... | Multi-way branch |

### Condition Codes

Used with conditional instructions (e.g., BR_EQ):
- `EQ`: Equal
- `NE`: Not equal
- `GE`: Greater than or equal
- `LT`: Less than
- `GT`: Greater than
- `LE`: Less than or equal
- `Z`: Zero flag set
- `NZ`: Zero flag not set

### Branch Examples

```
BR next_section      ; Unconditional branch
BR_EQ equal_case     ; Branch if equal
BR_LT less_branch    ; Branch if less than
```

### Function Call Examples

```
CALL function        ; Call with no parameters
CALL add, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
                     ; Call with parameters
RET                  ; Return with no value
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
                     ; Return with value
```

## Memory Operations

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x10   | MOV      | dest, src | Move data |
| 0x11   | PUSH     | value | Push to stack |
| 0x12   | POP      | dest | Pop from stack |
| 0x13   | LEA      | dest, addr | Load effective address |
| 0x14   | SCOPEE   | | Enter variable scope |
| 0x15   | SCOPEL   | | Leave variable scope |
| 0x16   | VAR      | id, type [, init_val] | Declare variable |
| 0x17   | MEMCPY   | dest, src, size | Copy memory block |
| 0x18   | MEMSET   | dest, value, size | Fill memory block |
| 0x19   | MEMCMP   | result, ptr1, ptr2, size | Compare memory blocks |
| 0x1A   | XCHG     | a, b | Exchange values |
| 0x1B   | CAS      | dest, expected, new | Compare and swap |

### Memory Addressing Modes

```
MOV #1, [address]              ; Direct addressing
MOV #1, [#2]                   ; Indirect addressing
MOV #1, [base_ptr + 8]         ; Base + offset
MOV #1, [array + index*4]      ; Base + scaled index
MOV #1, [base + index*4 + 8]   ; Full addressing mode
```

### Variable Declaration Examples

```
VAR #1, TYPE_INT32, 0          ; Integer with initialization
VAR #2, TYPE_PTR               ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159     ; Floating-point with initialization
```

## Arithmetic Instructions

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x60   | ADD      | dest, src1, src2 | Addition |
| 0x61   | SUB      | dest, src1, src2 | Subtraction |
| 0x62   | MUL      | dest, src1, src2 | Multiplication |
| 0x63   | DIV      | dest, src1, src2 | Division |
| 0x64   | MOD      | dest, src1, src2 | Modulo |
| 0x65   | INC      | dest | Increment |
| 0x66   | DEC      | dest | Decrement |
| 0x67   | NEG      | dest, src | Negate |
| 0x68   | ABS      | dest, src | Absolute value |
| 0x69   | SQRT     | dest, src | Square root |
| 0x6B   | CEIL     | dest, src | Ceiling |
| 0x6C   | FLOOR    | dest, src | Floor |
| 0x6D   | ROUND    | dest, src | Round |

### Arithmetic Examples

```
ADD #1, #2, #3        ; #1 = #2 + #3
SUB #1, #2, #3        ; #1 = #2 - #3
MUL #1, #2, #3        ; #1 = #2 * #3
DIV #1, #2, #3        ; #1 = #2 / #3
INC #1                ; #1 = #1 + 1
DEC #1                ; #1 = #1 - 1
```

## Bit Manipulation Instructions

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x50   | AND      | dest, src1, src2 | Bitwise AND |
| 0x51   | OR       | dest, src1, src2 | Bitwise OR |
| 0x52   | XOR      | dest, src1, src2 | Bitwise XOR |
| 0x53   | NOT      | dest, src | Bitwise NOT |
| 0x54   | SHL      | dest, src, count | Shift left |
| 0x55   | SHR      | dest, src, count | Shift right logical |
| 0x56   | SAR      | dest, src, count | Shift right arithmetic |

### Bit Manipulation Examples

```
AND #1, #2, #3        ; #1 = #2 & #3
OR #1, #2, #3         ; #1 = #2 | #3
XOR #1, #2, #3        ; #1 = #2 ^ #3
NOT #1, #2            ; #1 = ~#2
SHL #1, #2, 3         ; #1 = #2 << 3
SHR #1, #2, 3         ; #1 = #2 >> 3 (logical)
```

## Vector/Array Operations

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x90   | VLOAD    | dest, src | Load vector |
| 0x91   | VSTORE   | dest, src | Store vector |
| 0x96   | VEXTRACT | dest, src, index | Extract vector element |
| 0x97   | VINSERT  | dest, src, value, index | Insert vector element |
| 0x9C   | VDOT     | dest, src1, src2 | Vector dot product |
| 0xA8   | INDEX    | dest, array, index | Access array element |
| 0xA9   | UPDT     | array, index, value | Update array element |

### Vector/Array Examples

```
VLOAD #1, [array_ptr]           ; Load vector from memory
VEXTRACT #2, #1, 3              ; Extract element 3 from vector
VDOT #3, #1, #4                 ; Calculate dot product
INDEX #5, array, #6             ; Get array element
UPDT array, #7, #8              ; Update array element
```

## Type Operations

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0xA0   | TYPEOF   | dest, src | Get type information |
| 0xA1   | SIZEOF   | dest, type | Get size in bytes |
| 0xA3   | CONVERT  | dest, src | Convert between types |
| 0xA4   | CAST     | dest, src | Reinterpret bit pattern |
| 0xA6   | GET      | dest, struct, field | Get structure field |
| 0xA7   | SET      | struct, field, value | Set structure field |

### Type Operation Examples

```
CONVERT #1, #2                  ; Convert type of #2 and store in #1
CAST #1, #2                     ; Reinterpret #2 bits and store in #1
GET #1, struct_ptr, field_name  ; Get struct field value
SET struct_ptr, field_name, #2  ; Set struct field value
```

## Common Control Flow Patterns

### If-Then-Else
```
; if (#1 == #2) then X else Y
CMP #1, #2
BR_NE else_branch
  ; X code here
BR end_if
else_branch:
  ; Y code here
end_if:
```

### For Loop
```
; for (#1 = 0; #1 < #2; #1++)
MOV #1, 0
loop_start:
CMP #1, #2
BR_GE loop_end
  ; Loop body here
INC #1
BR loop_start
loop_end:
```

### While Loop
```
; while (#1 != 0)
while_start:
CMP #1, 0
BR_EQ while_end
  ; Loop body here
BR while_start
while_end:
```

### Switch Statement
```
; switch (#1) { case 1: ... case 2: ... default: ... }
SWITCH #1, 2
  1, case_one
  2, case_two
  default_case

case_one:
  ; Case 1 code
BR end_switch

case_two:
  ; Case 2 code
BR end_switch

default_case:
  ; Default case code

end_switch:
```

## Related Components

- [Type System](/coil-docs/types/type-system.md) - Type system reference
- [Binary Format](/coil-docs/core/binary-format.md) - Binary instruction encoding
- [CASM Basics](/casm-docs/basics/casm-basics.md) - CASM instruction syntax