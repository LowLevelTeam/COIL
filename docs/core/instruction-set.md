# COIL Instruction Set Architecture

## Overview

The COIL Instruction Set Architecture (ISA) defines operations available across all implementations. Instructions are categorized by functionality and opcode ranges, with behavior determined by operand types.

## Instruction Categories

| Opcode Range | Category | Purpose |
|--------------|----------|---------|
| 0x00-0x1F | Control Flow | Program execution control |
| 0x20-0x3F | Memory Operations | Memory management and access |
| 0x40-0x5F | Arithmetic Operations | Mathematical computation |
| 0x60-0x6F | Bit Manipulation | Bitwise operations |
| 0x70-0x7F | Atomic Operations | Synchronization primitives |
| 0x80-0x8F | Vector/Array Operations | Vector and array processing |
| 0x90-0x9F | Device Operations | Direct device interaction |
| 0xA0-0xDF | Extended Operations | Platform-specific instructions |
| 0xE0-0xEF | Type Operations | Direct functionality for types |
| 0xF0-0xFE | Compiler Operations | Compilation control |
| 0xFF | Vendor Operations | Vendor-specific extensions |

## Core Instructions

### Control Flow Instructions (0x00-0x1F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x00   | NOP      | No operation | 0 |
| 0x01   | BR       | Branch to address | 1-2 |
| 0x02   | CALL     | Call function | 1-2 |
| 0x03   | RET      | Return from function | 0-1 |
| 0x04   | CMP      | Compare values | 2 |
| 0x05   | TEST     | Test values and set flags | 2 |
| 0x06   | JMP      | Unconditional jump | 1 |
| 0x08   | TRAP     | Trap to exception handler | 1 |
| 0x09   | HALT     | Halt execution | 0 |

### Memory Operations (0x20-0x3F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x20   | MOV      | Move data between locations | 2 |
| 0x21   | PUSH     | Push value onto stack | 1 |
| 0x22   | POP      | Pop value from stack | 1 |
| 0x23   | LEA      | Load effective address | 2 |
| 0x24   | SCOPE    | Begin a variable scope | 0 |
| 0x25   | SCOPL    | End a variable scope | 0 |
| 0x26   | VAR      | Declare a variable | 2-3 |
| 0x27   | XCHG     | Exchange values | 2 |
| 0x28   | CAS      | Compare and swap (atomic) | 3 |
| 0x29   | MEMCPY   | Copy memory block | 3 |
| 0x2A   | MEMSET   | Set memory block | 3 |
| 0x2C   | ALLOC    | Allocate memory | 2 |
| 0x2D   | FREE     | Free allocated memory | 1 |

### Arithmetic Operations (0x40-0x5F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x40   | ADD      | Addition | 2-3 |
| 0x41   | SUB      | Subtraction | 2-3 |
| 0x42   | MUL      | Multiplication | 2-3 |
| 0x43   | DIV      | Division | 2-3 |
| 0x44   | MOD      | Modulo/Remainder | 2-3 |
| 0x45   | INC      | Increment | 1 |
| 0x46   | DEC      | Decrement | 1 |
| 0x47   | NEG      | Negate value | 1-2 |
| 0x48   | ABS      | Absolute value | 1-2 |
| 0x49   | SQRT     | Square root | 1-2 |
| 0x4D   | MIN      | Minimum of two values | 3 |
| 0x4E   | MAX      | Maximum of two values | 3 |
| 0x4F   | CLAMP    | Clamp value to range | 4 |

### Bit Manipulation Operations (0x60-0x6F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x60   | AND      | Bitwise AND | 2-3 |
| 0x61   | OR       | Bitwise OR | 2-3 |
| 0x62   | XOR      | Bitwise XOR | 2-3 |
| 0x63   | NOT      | Bitwise NOT | 1-2 |
| 0x64   | SHL      | Shift left logical | 2-3 |
| 0x65   | SHR      | Shift right logical | 2-3 |
| 0x66   | SAL      | Shift arithmetic left | 2-3 |
| 0x67   | SAR      | Shift arithmetic right | 2-3 |
| 0x6F   | POPCNT   | Count set bits | 2 |

### Atomic Operations (0x70-0x7F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x70   | ATOMLD   | Atomic load | 3 |
| 0x71   | ATOMST   | Atomic store | 3 |
| 0x72   | ATOMXCHG | Atomic exchange | 3 |
| 0x73   | ATOMCMPXCHG | Atomic compare and exchange | 4-5 |
| 0x74   | ATOMADD  | Atomic add | 3-4 |
| 0x75   | ATOMSUB  | Atomic subtract | 3-4 |
| 0x7B   | ATOMFENCE| Memory fence | 1 |
| 0x7C   | ATOMINCR | Atomic increment | 2-3 |
| 0x7D   | ATOMDECR | Atomic decrement | 2-3 |

### Vector/Array Operations (0x80-0x8F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x80   | GETE     | Get element | 3 | Core |
| 0x81   | SETE     | Set element | 3 | Core |
| 0x82   | DOT      | Vector dot product | 3 | MultiDim |
| 0x83   | CROSS    | Vector cross product | 3 | MultiDim |
| 0x84   | NORM     | Vector normalization | 1-2 | MultiDim |
| 0x85   | LEN      | Vector length/magnitude | 2 | MultiDim |

### Device Operations (0x90-0x9F)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0x90   | DEVMAP   | Map device memory region | 3-4 |
| 0x91   | DEVUNMAP | Unmap device memory region | 1 |
| 0x92   | INPORT   | Input from I/O port | 2 |
| 0x93   | OUTPORT  | Output to I/O port | 2 |
| 0x94   | DEVCFG   | Configure device | 2-3 |
| 0x95   | DEVSTAT  | Get device status | 2 |
| 0x96   | DEVRESET | Reset device | 1-2 |

## Flag Conditions

Flag conditions for conditional operations:

| Value | Mnemonic | Description |
|-------|----------|-------------|
| 0x00  | EQ       | Equal (Zero) |
| 0x01  | NEQ      | Not Equal (Not Zero) |
| 0x02  | GT       | Greater Than |
| 0x03  | GTE      | Greater Than or Equal |
| 0x04  | LT       | Less Than |
| 0x05  | LTE      | Less Than or Equal |
| 0x06  | O        | Overflow |
| 0x07  | NO       | No Overflow |
| 0x0C  | C        | Carry |
| 0x0D  | NC       | No Carry |
| 0x0E  | D        | Divide by Zero |
| 0x10  | N        | NaN Result |

## Instruction Examples

### Integer Addition
```
// ADD dest, src1, src2
[0x40][0x03]           // ADD with 3 operands
  [0x04][0x40]         // INT32, VARIABLE flag
  [variable_id_dest]   // Variable ID (uint64_t)
  [0x04][0x40]         // INT32, VARIABLE flag
  [variable_id_src1]   // Variable ID (uint64_t)
  [0x04][0x40]         // INT32, VARIABLE flag
  [variable_id_src2]   // Variable ID (uint64_t)
```

### Conditional Branch
```
// BR EQ, target_label
[0x01][0x02]          // BR with 2 operands
  [0xFE][0x00]        // PARAM0 type, FLAG_COND_EQ value
  [0xF2][0x80]        // SYMBOL type, SYMBOL flag
  [symbol_id]         // Symbol ID (uint64_t)
```

## Type-Determined Behavior

Instructions adapt their behavior based on operand types:

| Type Category | Behavior |
|---------------|----------|
| Integer | Standard binary arithmetic with wrapping or saturation |
| Float | IEEE-754 arithmetic operations |
| Vector | Element-wise operations |
| Matrix | Element-wise or algebraic operations |