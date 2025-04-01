# CBC Instruction Set

## Overview

CBC instructions are grouped by type to allow for efficient dispatch. Each instruction includes its type code as part of the opcode, allowing for fast execution without complex type determination.

## Control Flow Instructions

| Opcode | Type | Mnemonic | Description |
|--------|------|----------|-------------|
| 0x01 | 0 | nop | No operation |
| 0x02 | 0 | br | Unconditional branch |
| 0x03 | 1 | br.eq | Branch if equal (integer) |
| 0x04 | 1 | br.ne | Branch if not equal (integer) |
| 0x05 | 1 | br.lt | Branch if less than (integer) |
| 0x06 | 1 | br.le | Branch if less than or equal (integer) |
| 0x07 | 1 | br.gt | Branch if greater than (integer) |
| 0x08 | 1 | br.ge | Branch if greater than or equal (integer) |
| 0x09 | 2 | br.eq.f | Branch if equal (float) |
| 0x0A | 2 | br.ne.f | Branch if not equal (float) |
| 0x0B | 2 | br.lt.f | Branch if less than (float) |
| 0x0C | 2 | br.le.f | Branch if less than or equal (float) |
| 0x0D | 2 | br.gt.f | Branch if greater than (float) |
| 0x0E | 2 | br.ge.f | Branch if greater than or equal (float) |
| 0x0F | 0 | call | Call function |
| 0x10 | 0 | ret | Return from function |
| 0x11 | 0 | end | End of function |

## Memory Instructions

| Opcode | Type | Mnemonic | Description |
|--------|------|----------|-------------|
| 0x20 | 1 | load.i8 | Load 8-bit integer |
| 0x21 | 1 | load.i16 | Load 16-bit integer |
| 0x22 | 1 | load.i32 | Load 32-bit integer |
| 0x23 | 1 | load.i64 | Load 64-bit integer |
| 0x24 | 2 | load.f32 | Load 32-bit float |
| 0x25 | 2 | load.f64 | Load 64-bit float |
| 0x26 | 1 | store.i8 | Store 8-bit integer |
| 0x27 | 1 | store.i16 | Store 16-bit integer |
| 0x28 | 1 | store.i32 | Store 32-bit integer |
| 0x29 | 1 | store.i64 | Store 64-bit integer |
| 0x2A | 2 | store.f32 | Store 32-bit float |
| 0x2B | 2 | store.f64 | Store 64-bit float |
| 0x2C | 0 | scopee | Enter scope |
| 0x2D | 0 | scopel | Leave scope |
| 0x2E | 1 | var.i32 | Declare 32-bit integer variable |
| 0x2F | 1 | var.i64 | Declare 64-bit integer variable |
| 0x30 | 2 | var.f32 | Declare 32-bit float variable |
| 0x31 | 2 | var.f64 | Declare 64-bit float variable |

## Arithmetic Instructions (Integer)

| Opcode | Type | Mnemonic | Description |
|--------|------|----------|-------------|
| 0x40 | 1 | add.i32 | Add 32-bit integers |
| 0x41 | 1 | add.i64 | Add 64-bit integers |
| 0x42 | 1 | sub.i32 | Subtract 32-bit integers |
| 0x43 | 1 | sub.i64 | Subtract 64-bit integers |
| 0x44 | 1 | mul.i32 | Multiply 32-bit integers |
| 0x45 | 1 | mul.i64 | Multiply 64-bit integers |
| 0x46 | 1 | div.i32 | Divide 32-bit integers |
| 0x47 | 1 | div.i64 | Divide 64-bit integers |
| 0x48 | 1 | rem.i32 | Remainder of 32-bit integers |
| 0x49 | 1 | rem.i64 | Remainder of 64-bit integers |
| 0x4A | 1 | and.i32 | Bitwise AND of 32-bit integers |
| 0x4B | 1 | and.i64 | Bitwise AND of 64-bit integers |
| 0x4C | 1 | or.i32 | Bitwise OR of 32-bit integers |
| 0x4D | 1 | or.i64 | Bitwise OR of 64-bit integers |
| 0x4E | 1 | xor.i32 | Bitwise XOR of 32-bit integers |
| 0x4F | 1 | xor.i64 | Bitwise XOR of 64-bit integers |

## Arithmetic Instructions (Floating-Point)

| Opcode | Type | Mnemonic | Description |
|--------|------|----------|-------------|
| 0x60 | 2 | add.f32 | Add 32-bit floats |
| 0x61 | 2 | add.f64 | Add 64-bit floats |
| 0x62 | 2 | sub.f32 | Subtract 32-bit floats |
| 0x63 | 2 | sub.f64 | Subtract 64-bit floats |
| 0x64 | 2 | mul.f32 | Multiply 32-bit floats |
| 0x65 | 2 | mul.f64 | Multiply 64-bit floats |
| 0x66 | 2 | div.f32 | Divide 32-bit floats |
| 0x67 | 2 | div.f64 | Divide 64-bit floats |
| 0x68 | 2 | sqrt.f32 | Square root of 32-bit float |
| 0x69 | 2 | sqrt.f64 | Square root of 64-bit float |

## Conversion Instructions

| Opcode | Type | Mnemonic | Description |
|--------|------|----------|-------------|
| 0x80 | 0 | i32.to.i64 | Convert 32-bit integer to 64-bit integer |
| 0x81 | 0 | i64.to.i32 | Convert 64-bit integer to 32-bit integer |
| 0x82 | 0 | i32.to.f32 | Convert 32-bit integer to 32-bit float |
| 0x83 | 0 | i64.to.f64 | Convert 64-bit integer to 64-bit float |
| 0x84 | 0 | f32.to.i32 | Convert 32-bit float to 32-bit integer |
| 0x85 | 0 | f64.to.i64 | Convert 64-bit float to 64-bit integer |
| 0x86 | 0 | f32.to.f64 | Convert 32-bit float to 64-bit float |
| 0x87 | 0 | f64.to.f32 | Convert 64-bit float to 32-bit float |

## Example: Complete Factorial Function

```
; Function: factorial(n)
; Input: n in variable #1
; Output: result in variable #2

; Compare n to 1
03 06 01 01 01 05     ; br.eq #1, #5(const 1), return_one
05 05 01 05 12        ; br.lt #1, #5(const 1), end_factorial

; Recursive case: n * factorial(n-1)
2E 01 03              ; var.i32 #3 (n-1)
42 07 03 01 05        ; sub.i32 #3, #1, #5 (const 1)

; Call factorial recursively
0F 03 03              ; call factorial, #3

; Multiply n by factorial(n-1)
2E 01 02              ; var.i32 #2 (result)
44 07 02 01 02        ; mul.i32 #2, #1, #2
10 01 02              ; ret #2

; Base case: return 1
return_one:
2E 01 02 00000001     ; var.i32 #2, 1
10 01 02              ; ret #2

end_factorial:
```

## Related Components

- [CBC Format](/cbc-docs/cbc-format.md) - Binary format specification
- [CBC Execution](/cbc-docs/cbc-execution.md) - Execution environment details
- [CBC Overview](/cbc-docs/cbc-overview.md) - General CBC introduction