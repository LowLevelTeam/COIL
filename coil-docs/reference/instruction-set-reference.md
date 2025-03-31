# COIL Instruction Set Reference

## Instruction Overview

This document provides a comprehensive reference for the COIL instruction set. Each instruction is listed with its opcode, operand format, and operational behavior.

## Instruction Categories

### Control Flow (0x01-0x0F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x01   | SYM      | symbol_name [, attributes] | Define a symbol |
| 0x02   | BR       | target [, condition] | Branch to target |
| 0x03   | CALL     | target [, parameters] | Call function |
| 0x04   | RET      | [return_values] | Return from function |
| 0x05   | CMP      | left, right | Compare values and set flags |
| 0x06   | TEST     | value, mask | Bitwise AND and set flags |
| 0x07   | JMP      | target [, condition] | Jump (alias for BR) |
| 0x08   | LOOP     | counter, target | Decrement counter and branch if not zero |
| 0x09   | SWITCH   | value, case_count, cases... | Multi-way branch |
| 0x0A   | SYSCALL  | [parameters] | System call |
| 0x0B   | INT      | interrupt_number | Trigger interrupt |
| 0x0C   | HALT     | | Stop execution |
| 0x0D   | NOP      | | No operation |
| 0x0E   | BREAK    | | Breakpoint |
| 0x0F   | HCF      | | Halt and catch fire (debug) |

### Memory Operations (0x10-0x2F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x10   | MOV      | dest, src | Move data between locations |
| 0x11   | PUSH     | value | Push value to stack |
| 0x12   | POP      | dest | Pop value from stack |
| 0x13   | LEA      | dest, address | Load effective address |
| 0x14   | SCOPEE   | | Enter variable scope |
| 0x15   | SCOPEL   | | Leave variable scope |
| 0x16   | VAR      | id, type [, initial_value] | Declare variable |
| 0x17   | MEMCPY   | dest, src, size | Copy memory block |
| 0x18   | MEMSET   | dest, value, size | Fill memory block |
| 0x19   | MEMCMP   | result, ptr1, ptr2, size | Compare memory blocks |
| 0x1A   | XCHG     | a, b | Exchange values |
| 0x1B   | CAS      | dest, expected, new | Compare and swap |
| 0x1C   | FENCE    | [type] | Memory barrier |
| 0x1D   | CMPXCHG  | dest, expected, new | Compare and exchange |
| 0x1E   | PREFETCH | address, type | Prefetch memory |
| 0x1F   | FLUSH    | address | Flush cache line |

### Bit Manipulation (0x50-0x5F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x50   | AND      | dest, src1, src2 | Bitwise AND |
| 0x51   | OR       | dest, src1, src2 | Bitwise OR |
| 0x52   | XOR      | dest, src1, src2 | Bitwise XOR |
| 0x53   | NOT      | dest, src | Bitwise NOT |
| 0x54   | SHL      | dest, src, count | Shift left |
| 0x55   | SHR      | dest, src, count | Shift right logical |
| 0x56   | SAR      | dest, src, count | Shift right arithmetic |
| 0x57   | ROL      | dest, src, count | Rotate left |
| 0x58   | ROR      | dest, src, count | Rotate right |
| 0x59   | POPCNT   | dest, src | Count set bits |
| 0x5A   | BSWAP    | dest, src | Byte swap |
| 0x5B   | CLZ      | dest, src | Count leading zeros |
| 0x5C   | CTZ      | dest, src | Count trailing zeros |
| 0x5D   | BITSET   | dest, src, bit | Set specific bit |
| 0x5E   | BITCLR   | dest, src, bit | Clear specific bit |
| 0x5F   | BITTST   | dest, src, bit | Test specific bit |

### Arithmetic (0x60-0x8F)

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
| 0x6A   | FMA      | dest, a, b, c | Fused multiply-add (a*b+c) |
| 0x6B   | CEIL     | dest, src | Ceiling |
| 0x6C   | FLOOR    | dest, src | Floor |
| 0x6D   | ROUND    | dest, src | Round |
| 0x6E   | TRUNC    | dest, src | Truncate |
| 0x6F   | SIN      | dest, src | Sine |
| 0x70   | COS      | dest, src | Cosine |
| 0x71   | TAN      | dest, src | Tangent |
| 0x72   | ASIN     | dest, src | Arc sine |
| 0x73   | ACOS     | dest, src | Arc cosine |
| 0x74   | ATAN     | dest, src | Arc tangent |
| 0x75   | ATAN2    | dest, y, x | Arc tangent (2 parameters) |
| 0x76   | SINH     | dest, src | Hyperbolic sine |
| 0x77   | COSH     | dest, src | Hyperbolic cosine |
| 0x78   | TANH     | dest, src | Hyperbolic tangent |
| 0x79   | EXP      | dest, src | Exponential |
| 0x7A   | LOG      | dest, src | Natural logarithm |
| 0x7B   | MIN      | dest, src1, src2 | Minimum |
| 0x7C   | MAX      | dest, src1, src2 | Maximum |
| 0x7D   | POW      | dest, base, exp | Power |
| 0x7E   | LOG10    | dest, src | Base-10 logarithm |
| 0x7F   | LOG2     | dest, src | Base-2 logarithm |

### Vector/Array Operations (0x90-0x9F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x90   | VLOAD    | dest, src | Load vector |
| 0x91   | VSTORE   | dest, src | Store vector |
| 0x92   | VADD     | dest, src1, src2 | Vector addition |
| 0x93   | VSUB     | dest, src1, src2 | Vector subtraction |
| 0x94   | VMUL     | dest, src1, src2 | Vector multiplication |
| 0x95   | VDIV     | dest, src1, src2 | Vector division |
| 0x96   | VEXTRACT | dest, src, index | Extract vector element |
| 0x97   | VINSERT  | dest, src, value, index | Insert vector element |
| 0x98   | VSHUFFLE | dest, src, mask | Shuffle vector elements |
| 0x99   | VCMP     | dest, src1, src2, type | Compare vectors |
| 0x9A   | VMIN     | dest, src1, src2 | Vector minimum |
| 0x9B   | VMAX     | dest, src1, src2 | Vector maximum |
| 0x9C   | VDOT     | dest, src1, src2 | Vector dot product |
| 0x9D   | VCROSS   | dest, src1, src2 | Vector cross product |
| 0x9E   | VHSUM    | dest, src | Horizontal sum of vector |
| 0x9F   | VHPROD   | dest, src | Horizontal product of vector |

### Type Operations (0xA0-0xAF)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0xA0   | TYPEOF   | dest, src | Get type information |
| 0xA1   | SIZEOF   | dest, type | Get size in bytes |
| 0xA2   | ALIGNOF  | dest, type | Get alignment requirement |
| 0xA3   | CONVERT  | dest, src | Convert between types |
| 0xA4   | CAST     | dest, src | Reinterpret bit pattern |
| 0xA5   | STRUCT   | name, fields... | Define structure type |
| 0xA6   | GET      | dest, struct, field | Get structure field |
| 0xA7   | SET      | struct, field, value | Set structure field |
| 0xA8   | INDEX    | dest, array, index | Access array element |
| 0xA9   | UPDT     | array, index, value | Update array element |
| 0xAA   | TYPEDEF  | name, type | Create type alias |
| 0xAB   | ARRAY    | name, type, size | Define array type |
| 0xAC   | UNION    | name, fields... | Define union type |
| 0xAD   | ENUM     | name, values... | Define enumeration |
| 0xAE   | INSTANCE | dest, type | Create instance of type |
| 0xAF   | ISTYPE   | dest, value, type | Check if value is of specific type |

### Directive Instructions (0xB0-0xBF)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0xB0   | ARCH     | arch_code [, mode_code] | Set target architecture |
| 0xB1   | PROC     | processor_code | Set processor type |
| 0xB2   | MODE     | mode_code | Set processor mode |
| 0xB3   | ALIGN    | alignment | Align to boundary |
| 0xB4   | SECTION  | name, attributes | Define section |
| 0xB5   | DATA     | type, value | Insert data |
| 0xB6   | IF       | condition | Conditional assembly start |
| 0xB7   | ELIF     | condition | Conditional assembly alternative |
| 0xB8   | ELSE     | | Conditional assembly default |
| 0xB9   | ENDIF    | | Conditional assembly end |
| 0xBA   | ABI      | name | ABI definition start |
| 0xBB   | EXTERN   | symbol [, type] | Declare external symbol |
| 0xBC   | GLOBAL   | symbol | Declare global symbol |
| 0xBD   | INCLUDE  | filename | Include file |
| 0xBE   | VERSION  | major, minor, patch | Specify COIL version |
| 0xBF   | EXIT     | | End block definition |

## Condition Codes

Condition codes used with conditional instructions (e.g., BR_EQ):

| Code | Mnemonic | Description |
|------|----------|-------------|
| 0x00 | EQ | Equal |
| 0x01 | NE | Not equal |
| 0x02 | GE | Greater than or equal |
| 0x03 | LT | Less than |
| 0x04 | GT | Greater than |
| 0x05 | LE | Less than or equal |
| 0x06 | Z | Zero flag set |
| 0x07 | NZ | Zero flag not set |
| 0x08 | C | Carry flag set |
| 0x09 | NC | Carry flag not set |
| 0x0A | S | Sign flag set |
| 0x0B | NS | Sign flag not set |
| 0x0C | O | Overflow flag set |
| 0x0D | NO | Overflow flag not set |
| 0x0E | P | Parity flag set |
| 0x0F | NP | Parity flag not set |

## Shorthand Syntax

Conditions can be appended to instruction mnemonics as shorthand:

```
BR_EQ target    ; Branch if equal (instead of BR target, TYPE_PARAM5=BRANCH_COND_EQ)
MOV_NZ dest, src ; Move if not zero (instead of MOV dest, src, TYPE_PARAM5=BRANCH_COND_NZ)
```

## Instruction Encoding Example

```
; ADD #1, #2, #3  (add variables 1 and 2, store in variable 3)
0x60         ; Opcode for ADD
0x03         ; Three operands
0x9000 0x01  ; TYPE_VAR + Variable ID 1
0x9000 0x02  ; TYPE_VAR + Variable ID 2
0x9000 0x03  ; TYPE_VAR + Variable ID 3

; BR_EQ target  (branch to target if equal)
0x02         ; Opcode for BR
0x02         ; Two operands
0x9100 [sym] ; TYPE_SYM + Symbol index for target
0xF000 0x00  ; TYPE_PARAM5 + BRANCH_COND_EQ
```