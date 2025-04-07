# COIL Instruction Set Architecture

## Overview

The COIL Instruction Set Architecture (ISA) defines the core operations available across all implementations. Instructions are categorized based on functionality and opcode ranges, with behavior determined by operand types.

## Instruction Categories

| Opcode Range | Category | Purpose | Extension |
|--------------|----------|---------|-----------|
| 0x00-0x1F | Control Flow | Program execution control | Core |
| 0x20-0x3F | Memory Operations | Memory management and access | Core |
| 0x40-0x5F | Arithmetic Operations | Mathematical computation | Core |
| 0x60-0x7F | Bit Manipulation | Bitwise operations | Core |
| 0x80-0x8F | Vector/Array Operations | Vector and array processing | Core |
| 0x90-0x9F | Special Operations | Specialized instructions | Core |
| 0xE0-0xEF | Type Operations | Direct functionality for types | Core |

## Control Flow Instructions (0x00-0x1F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x00   | NOP      | No operation | 0 | Core |
| 0x01   | BR       | Branch to address | 1-2 | Core |
| 0x02   | CALL     | Call function | 1-2 | Core |
| 0x03   | RET      | Return from function | 0-1 | Core |
| 0x04   | CMP      | Compare values | 2 | Core |
| 0x05   | TEST     | Test values and set flags | 2 | Core |

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

## Bit Manipulation Operations (0x60-0x7F)

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

## Special Operations (0x90-0x9F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x90   | RNG      | Random number generation | 2-3 | Core |
| 0x91   | HASH     | Hash function | 3 | Core |
| 0x92   | ENCRYPT  | Encryption operation | 3-4 | Core |
| 0x93   | DECRYPT  | Decryption operation | 3-4 | Core |
| 0x94   | CRYPT    | Cryptographic operation | 5 | Core |
| 0x95   | SIGN     | Digital signature | 3 | Core |
| 0x96   | VERIFY   | Signature verification | 4 | Core |
| 0x97   | COMPRESS | Data compression | 3-4 | Core |
| 0x98   | EXPAND   | Data expansion | 3-4 | Core |
| 0x99   | CHECKSUM | Checksum calculation | 3 | Core |

## Specific Operations (0xA0-0xDF)
These 64 instructions are split between processing unit instructions, architecture specific instructions and mode specific instructions.
The split is processing unit dependent where some PUs may have more instructions global across all devices others may have more instructions that only work on certain architectures.

For CPUs this generally goes like 
PU:
  Interrupts

Arch:
  Identification

Mode:
  Interrupt Table

To account for the CPU structure.
More information can be found in the pu/ section

## Type Operations (0xE0-0xEF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xE0   | TYPE     | Define a Type | 2 | Core |
| 0xE1   | SIZEOF   | Get size of Type | 1 | Core |
| 0xE2   | ALIGNOF  | Get natural alignment of type | 1 | Core |
| 0xE3   | DTYPE    | Delete type definition | 1 | Core |

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

#### RET (0x03)
- Returns from a function to the caller
- Restores the previous call frame according to the current ABI
- Returns control to the saved return address

#### CMP (0x04)
- Compares two values and sets flags based on the result
- Does not modify either operand
- Comparison behavior depends on operand types

#### TEST (0x05)
- Tests bit patterns and sets flags based on the result
- Performs a bitwise AND between operands
- Sets flags based on the result
- Does not modify either operand

### Memory Operations

#### MOV (0x20)
- Copies data from source to destination
- Type conversion is performed if necessary and possible
- Destination must be a writable location

#### PUSH (0x21)
- Pushes a value onto the stack
- Decrements the stack pointer by the size of the value
- Copies the value to the location pointed to by the stack pointer

#### POP (0x22)
- Pops a value from the stack
- Copies the value at the current stack pointer to the destination
- Increments the stack pointer by the size of the value

#### LEA (0x23)
- Loads the effective address of a memory operand
- Calculates the address without accessing memory
- Stores the resulting address in the destination

#### SCOPE (0x24)
- Creates a new variable scope context
- Variables declared within this scope are destroyed at matching SCOPL
- Scopes can be nested

#### SCOPL (0x25)
- Destroys all variables declared in the current scope
- Restores the previous scope context
- Must match a previous SCOPE instruction

#### VAR (0x26)
- Allocates storage for a variable with the specified type
- Assigns an initial value if provided
- Variable is valid until the end of the current scope

### Arithmetic Operations

#### ADD (0x40)
- Adds two values
- For integral types: Standard binary addition
- For floating-point types: IEEE-754 addition
- For vector types: Element-wise addition
- For matrix types: Element-wise addition

#### MUL (0x42)
- Multiplies two values
- For integral/float types: Standard multiplication
- For vector types: Element-wise multiplication
- For matrix types with scalar: Scalar multiplication
- For matrix types with matrix: Matrix multiplication

#### DIV (0x43)
- Divides one value by another
- For integral types: Integer division (truncated toward zero)
- For floating-point types: IEEE-754 division
- Division by zero raises an exception or sets flags

## Type-Determined Behavior

Instructions adapt their behavior based on operand types:

| Type Category | Behavior |
|---------------|----------|
| Integer | Standard binary arithmetic with wrapping or saturation |
| Float | IEEE-754 arithmetic operations |
| Vector | Element-wise operations |
| Matrix | Element-wise or algebraic operations |

For detailed behavior matrices, see [Type-Determined Behavior](/core/type-determined-behavior.md).# COIL Instruction Set Architecture (ISA)

The COIL ISA can potentially be complex to work around for developers. Working out which features are available by the COIL implementation can be complex for such a wide reaching structure. We try to make this easy by defining the extensions within this one file.

For certain instructions you may see a (ext ...) where ... is some identifier like ABI. This mean the instruction may not be supported by all COIL implementations or in otherwords is not a core COIL feature.

We try to make this clear from one file to give the greatest help to developers searching for instructions rather then having an instruction set spread out across multiple files and being hard to work around here is the documentation for almost all COIL instructions.

There are other files to explain more about ISA but this is unofrtunately needed as describing all the instructions supported by individual processing units from this one file would make it almost impossible to maintain and work on. Aside from the processing unit and architecture specific instruction sets which can be found individually under pu/processing_unit/architecture.md every other instruction is listed in a common format.

Split into three categories, compiler (to be worked out by teh COIL processor leaving no runtime impact), core (default runtime instructions), extended (instructions specific to certain purposes like type operations, crypto, multi dimensional etc...)




