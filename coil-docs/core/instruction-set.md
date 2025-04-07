# COIL Instruction Set Architecture

## Overview

The COIL Instruction Set Architecture (ISA) defines the core operations available across all implementations. Instructions are categorized based on functionality and opcode ranges, with behavior determined by operand types. The ISA is designed for direct device interaction without operating system dependencies.

## Instruction Categories

| Opcode Range | Category | Purpose | Extension |
|--------------|----------|---------|-----------|
| 0x00-0x1F | Control Flow | Program execution control | Core |
| 0x20-0x3F | Memory Operations | Memory management and access | Core |
| 0x40-0x5F | Arithmetic Operations | Mathematical computation | Core |
| 0x60-0x6F | Bit Manipulation | Bitwise operations | Core |
| 0x70-0x7F | Atomic Operations | Synchronization primitives | Core |
| 0x80-0x8F | Vector/Array Operations | Vector and array processing | Core |
| 0x90-0x9F | Device Operations | Direct device interaction | Core |
| 0xA0-0xDF | Extended Operations | Platform-specific instructions | Processor |
| 0xE0-0xEF | Type Operations | Direct functionality for types | Core |
| 0xF0-0xFE | Compiler Operations | Compilation control | Preprocessor |
| 0xFF | Vendor Operations | Vendor-specific extensions | Vendor |

## Control Flow Instructions (0x00-0x1F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x00   | NOP      | No operation | 0 | Core |
| 0x01   | BR       | Branch to address | 1-2 | Core |
| 0x02   | CALL     | Call function | 1-2 | Core |
| 0x03   | RET      | Return from function | 0-1 | Core |
| 0x04   | CMP      | Compare values | 2 | Core |
| 0x05   | TEST     | Test values and set flags | 2 | Core |
| 0x06   | JMP      | Unconditional jump | 1 | Core |
| 0x07   | SWITCH   | Switch-case branch | 2 | Core |
| 0x08   | TRAP     | Trap to exception handler | 1 | Core |
| 0x09   | HALT     | Halt execution | 0 | Core |

## Memory Operations (0x20-0x3F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x20   | MOV      | Move data between locations | 2 | Core |
| 0x21   | PUSH     | Push value onto stack | 1 | Core |
| 0x22   | POP      | Pop value from stack | 1 | Core |
| 0x23   | LEA      | Load effective address | 2 | Core |
| 0x24   | SCOPE    | Begin a variable scope | 0 | Core |
| 0x25   | SCOPL    | End a variable scope | 0 | Core |
| 0x26   | VAR      | Declare a variable | 2-3 | Core |
| 0x27   | XCHG     | Exchange values | 2 | Core |
| 0x28   | CAS      | Compare and swap (atomic) | 3 | Core |
| 0x29   | MEMCPY   | Copy memory block | 3 | Core |
| 0x2A   | MEMSET   | Set memory block | 3 | Core |
| 0x2B   | MEMZERO  | Zero memory block | 2 | Core |
| 0x2C   | ALLOC    | Allocate memory | 2 | Core |
| 0x2D   | FREE     | Free allocated memory | 1 | Core |
| 0x2E   | LOADALIGN| Load with alignment | 3 | Core |
| 0x2F   | STOREALIGN| Store with alignment | 3 | Core |

## Arithmetic Operations (0x40-0x5F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x40   | ADD      | Addition | 2-3 | Core |
| 0x41   | SUB      | Subtraction | 2-3 | Core |
| 0x42   | MUL      | Multiplication | 2-3 | Core |
| 0x43   | DIV      | Division | 2-3 | Core |
| 0x44   | MOD      | Modulo/Remainder | 2-3 | Core |
| 0x45   | INC      | Increment | 1 | Core |
| 0x46   | DEC      | Decrement | 1 | Core |
| 0x47   | NEG      | Negate value | 1-2 | Core |
| 0x48   | ABS      | Absolute value | 1-2 | Core |
| 0x49   | SQRT     | Square root | 1-2 | Core |
| 0x4A   | CEIL     | Ceiling function | 1-2 | Core |
| 0x4B   | FLOR     | Floor function | 1-2 | Core |
| 0x4C   | ROND     | Round to nearest | 1-2 | Core |
| 0x4D   | MIN      | Minimum of two values | 3 | Core |
| 0x4E   | MAX      | Maximum of two values | 3 | Core |
| 0x4F   | CLAMP    | Clamp value to range | 4 | Core |
| 0x50   | FMA      | Fused multiply-add | 4 | Core |
| 0x51   | ADDC     | Add with carry | 3-4 | Core |
| 0x52   | SUBB     | Subtract with borrow | 3-4 | Core |
| 0x53   | MULH     | Multiply high (upper bits) | 3 | Core |
| 0x54   | DIVMOD   | Division with remainder | 4 | Core |

## Bit Manipulation Operations (0x60-0x6F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x60   | AND      | Bitwise AND | 2-3 | Core |
| 0x61   | OR       | Bitwise OR | 2-3 | Core |
| 0x62   | XOR      | Bitwise XOR | 2-3 | Core |
| 0x63   | NOT      | Bitwise NOT | 1-2 | Core |
| 0x64   | SHL      | Shift left logical | 2-3 | Core |
| 0x65   | SHR      | Shift right logical | 2-3 | Core |
| 0x66   | SAL      | Shift arithmetic left | 2-3 | Core |
| 0x67   | SAR      | Shift arithmetic right | 2-3 | Core |
| 0x68   | ROL      | Rotate left | 2-3 | Core |
| 0x69   | ROR      | Rotate right | 2-3 | Core |
| 0x6A   | BSWAP    | Byte swap (endianness conversion) | 1-2 | Core |
| 0x6B   | BITSET   | Set bit at position | 2-3 | Core |
| 0x6C   | BITCLR   | Clear bit at position | 2-3 | Core |
| 0x6D   | BITTGL   | Toggle bit at position | 2-3 | Core |
| 0x6E   | BITCHK   | Check bit at position | 2-3 | Core |
| 0x6F   | POPCNT   | Count set bits | 2 | Core |

## Atomic Operations (0x70-0x7F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x70   | ATOMLD   | Atomic load | 3 | Core |
| 0x71   | ATOMST   | Atomic store | 3 | Core |
| 0x72   | ATOMXCHG | Atomic exchange | 3 | Core |
| 0x73   | ATOMCMPXCHG | Atomic compare and exchange | 4-5 | Core |
| 0x74   | ATOMADD  | Atomic add | 3-4 | Core |
| 0x75   | ATOMSUB  | Atomic subtract | 3-4 | Core |
| 0x76   | ATOMAND  | Atomic AND | 3-4 | Core |
| 0x77   | ATOMOR   | Atomic OR | 3-4 | Core |
| 0x78   | ATOMXOR  | Atomic XOR | 3-4 | Core |
| 0x79   | ATOMMAX  | Atomic maximum | 3-4 | Core |
| 0x7A   | ATOMMIN  | Atomic minimum | 3-4 | Core |
| 0x7B   | ATOMFENCE| Memory fence | 1 | Core |
| 0x7C   | ATOMINCR | Atomic increment | 2-3 | Core |
| 0x7D   | ATOMDECR | Atomic decrement | 2-3 | Core |
| 0x7E   | ATOMWAIT | Wait on atomic value | 3-4 | Core |
| 0x7F   | ATOMNOTIFY | Notify waiters on atomic value | 1-2 | Core |

## Vector/Array Operations (0x80-0x8F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x80   | GETE     | Get element | 3 | Core |
| 0x81   | SETE     | Set element | 3 | Core |
| 0x82   | DOT      | Vector dot product | 3 | MultiDim |
| 0x83   | CROSS    | Vector cross product | 3 | MultiDim |
| 0x84   | NORM     | Vector normalization | 1-2 | MultiDim |
| 0x85   | LEN      | Vector length/magnitude | 2 | MultiDim |
| 0x86   | SHUF     | Vector shuffle elements | 3 | MultiDim |
| 0x87   | EXTRACT  | Extract subvector | 4 | MultiDim |
| 0x88   | INSERT   | Insert subvector | 4 | MultiDim |
| 0x89   | TRANS    | Matrix transpose | 1-2 | MultiDim |
| 0x8A   | INV      | Matrix inverse | 1-2 | MultiDim |
| 0x8B   | DET      | Matrix determinant | 2 | MultiDim |
| 0x8C   | ROW      | Get/set row | 3 | MultiDim |
| 0x8D   | COL      | Get/set column | 3 | MultiDim |
| 0x8E   | DIAG     | Get/set diagonal | 3 | MultiDim |
| 0x8F   | VECOP    | Vector operation | 3-4 | MultiDim |

## Device Operations (0x90-0x9F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x90   | DEVMAP   | Map device memory region | 3-4 | Core |
| 0x91   | DEVUNMAP | Unmap device memory region | 1 | Core |
| 0x92   | INPORT   | Input from I/O port | 2 | Core |
| 0x93   | OUTPORT  | Output to I/O port | 2 | Core |
| 0x94   | DEVCFG   | Configure device | 2-3 | Core |
| 0x95   | DEVSTAT  | Get device status | 2 | Core |
| 0x96   | DEVRESET | Reset device | 1-2 | Core |
| 0x97   | DEVCTL   | Device control operation | 2-3 | Core |
| 0x98   | DEVCPUID | Get device identification | 1-2 | Core |
| 0x99   | DEVINFO  | Get device information | 2-3 | Core |
| 0x9A   | DEVINT   | Configure device interrupt | 2-3 | Core |
| 0x9B   | DEVWAIT  | Wait for device event | 1-2 | Core |
| 0x9C   | INTACK   | Acknowledge interrupt | 1 | Core |
| 0x9D   | INTDIS   | Disable interrupts | 0-1 | Core |
| 0x9E   | INTENA   | Enable interrupts | 0-1 | Core |
| 0x9F   | CACHEOP  | Cache operation | 2-3 | Core |

## Type Operations (0xE0-0xEF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xE0   | TYPE     | Define a Type | 2 | Core |
| 0xE1   | SIZEOF   | Get size of Type | 1 | Core |
| 0xE2   | ALIGNOF  | Get natural alignment of type | 1 | Core |
| 0xE3   | DTYPE    | Delete type definition | 1 | Core |
| 0xE4   | CAST     | Type cast operation | 2-3 | Core |
| 0xE5   | MEMGET   | Get structure member | 3 | Composite |
| 0xE6   | MEMSET   | Set structure member | 3 | Composite |
| 0xE7   | MEMADDR  | Get member address | 3 | Composite |
| 0xE8   | STRDEF   | Define structure type | 2+ | Composite |
| 0xE9   | UNIDEF   | Define union type | 2+ | Composite |
| 0xEA   | TYPEAL   | Define type alias | 2 | Composite |

## Flag Conditions

Flag conditions determine whether conditional operations execute:

| Value | Mnemonic | Description | Extension |
|-------|----------|-------------|-----------|
| 0x00  | EQ       | Equal (Zero) | Core |
| 0x01  | NEQ      | Not Equal (Not Zero) | Core |
| 0x02  | GT       | Greater Than | Core |
| 0x03  | GTE      | Greater Than or Equal | Core |
| 0x04  | LT       | Less Than | Core |
| 0x05  | LTE      | Less Than or Equal | Core |
| 0x06  | O        | Overflow | Core |
| 0x07  | NO       | No Overflow | Core |
| 0x08  | S        | Sign | Core |
| 0x09  | NS       | Not Sign | Core |
| 0x0A  | Z        | Zero | Core |
| 0x0B  | NZ       | Not Zero | Core |
| 0x0C  | C        | Carry | Core |
| 0x0D  | NC       | No Carry | Core |
| 0x0E  | D        | Divide by Zero | Core |
| 0x0F  | ND       | No Divide by Zero | Core |
| 0x10  | N        | NaN Result | Core |
| 0x11  | NN       | Not NaN Result | Core |
| 0x12  | P        | Even Parity | Core |
| 0x13  | NP       | Odd Parity | Core |

## Detailed Instruction Behaviors

### Control Flow Instructions

#### NOP (0x00)
- Performs no operation
- Execution continues to the next instruction
- No effect on program state

#### BR (0x01)
- Branches to the specified address
- Unconditional form: `BR address`
- Conditional form: `BR condition, address`
- Address can be an immediate value, variable, or symbol reference

#### CALL (0x02)
- Calls a function, saving the return address
- Sets up a new call frame according to the current ABI
- Saves the return address to the call stack
- Transfers control to the specified address

### Atomic Operations

#### ATOMLD (0x70)
- Atomically loads a value with specified memory ordering
- Format: `ATOMLD dest, source, order`
- Ordering options: RELAXED, ACQUIRE, SEQ_CST
- No flags affected

#### ATOMCMPXCHG (0x73)
- Atomic compare-and-exchange operation
- Format: `ATOMCMPXCHG dest, ptr, expected, desired, order`
- Sets dest to 1 if exchange occurred, 0 otherwise
- Sets Z flag based on exchange success

### Device Operations

#### DEVMAP (0x90)
- Maps a device memory region into accessible address space
- Format: `DEVMAP dest_ptr, device_addr, size, flags`
- Allows direct interaction with memory-mapped devices
- Sets flags based on mapping success

#### INPORT (0x92)
- Reads a value from an I/O port
- Format: `INPORT dest, port`
- Used for direct device I/O on architectures with separate I/O space
- No flags affected

## Type-Determined Behavior

Instructions adapt their behavior based on operand types:

| Type Category | Behavior |
|---------------|----------|
| Integer | Standard binary arithmetic with wrapping or saturation |
| Float | IEEE-754 arithmetic operations |
| Vector | Element-wise operations |
| Matrix | Element-wise or algebraic operations |

## Implementation Requirements

1. **Core Instructions**: All implementations must support the core instruction set (0x00-0x9F, 0xE0-0xEF)
2. **Device Access**: Implementations must provide direct device access at appropriate privilege levels
3. **Atomic Operations**: Implementations must either provide hardware atomic support or software emulation
4. **Type System**: All core types must be supported
5. **Memory Model**: Implementations must adhere to the memory model specification

## OS Independence

The core instruction set is designed to function without operating system dependencies:

1. No syscall instructions in the core set
2. No OS-dependent memory management
3. No reliance on OS-provided synchronization
4. Direct hardware access for device interaction

For operations that require OS services, the standard library provides appropriate abstractions without compromising the core specification.