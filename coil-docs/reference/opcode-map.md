# COIL Opcode Map

## Opcode Space Organization

The COIL opcode space (8 bits, values 0x00-0xFF) is organized into functional categories:

| Opcode Range | Category | Description |
|--------------|----------|-------------|
| 0x00         | NOP      | No Operation |
| 0x01-0x0F    | Control Flow | Program flow, branches, calls |
| 0x10-0x2F    | Memory Operations | Data movement, variables, memory access |
| 0x30-0x4F    | Reserved | Reserved for future use |
| 0x50-0x5F    | Bit Manipulation | Logical operations, shifts, bit-level operations |
| 0x60-0x8F    | Arithmetic | Mathematical operations |
| 0x90-0x9F    | Vector/Array | Operations on multiple data elements |
| 0xA0-0xAF    | Type Operations | Type manipulation, structures, arrays |
| 0xB0-0xBF    | Directives | Processor selection, assembly control |
| 0xC0-0xFE    | Processor-Specific | Architecture/processor-specific instructions |
| 0xFF         | Extensions | Implementation-specific extensions |

## Universal vs. Platform-Specific

- **Universal Instructions** (0x00-0xBF): Available on all platforms
- **Platform-Specific Instructions** (0xC0-0xFE): Specific to certain processor types
- **Extension Instructions** (0xFF): Implementation-specific

## Core Opcodes

### Control Flow (0x01-0x0F)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x01   | SYM      | Define a symbol |
| 0x02   | BR       | Branch to target |
| 0x03   | CALL     | Call function |
| 0x04   | RET      | Return from function |
| 0x05   | CMP      | Compare values and set flags |
| 0x06   | TEST     | Bitwise AND and set flags |
| 0x07   | JMP      | Jump (alias for BR) |
| 0x08   | LOOP     | Decrement counter and branch if not zero |
| 0x09   | SWITCH   | Multi-way branch |

### Memory Operations (0x10-0x2F)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x10   | MOV      | Move data |
| 0x11   | PUSH     | Push value to stack |
| 0x12   | POP      | Pop value from stack |
| 0x13   | LEA      | Load effective address |
| 0x14   | SCOPEE   | Enter a variable scope |
| 0x15   | SCOPEL   | Leave a variable scope |
| 0x16   | VAR      | Declare a variable |
| 0x17   | MEMCPY   | Copy memory block |
| 0x18   | MEMSET   | Fill memory block |
| 0x19   | MEMCMP   | Compare memory blocks |
| 0x1A   | XCHG     | Exchange values |
| 0x1B   | CAS      | Compare and swap |

### Bit Manipulation (0x50-0x5F)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x50   | AND      | Bitwise AND |
| 0x51   | OR       | Bitwise OR |
| 0x52   | XOR      | Bitwise XOR |
| 0x53   | NOT      | Bitwise NOT |
| 0x54   | SHL      | Shift left |
| 0x55   | SHR      | Shift right logical |
| 0x56   | SAR      | Shift right arithmetic |
| 0x57   | ROL      | Rotate left |
| 0x58   | ROR      | Rotate right |
| 0x59   | POPCNT   | Count set bits |
| 0x5A   | BSWAP    | Byte swap |
| 0x5B   | CLZ      | Count leading zeros |
| 0x5C   | CTZ      | Count trailing zeros |

### Arithmetic (0x60-0x8F)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x60   | ADD      | Addition |
| 0x61   | SUB      | Subtraction |
| 0x62   | MUL      | Multiplication |
| 0x63   | DIV      | Division |
| 0x64   | MOD      | Modulo |
| 0x65   | INC      | Increment |
| 0x66   | DEC      | Decrement |
| 0x67   | NEG      | Negate |
| 0x68   | ABS      | Absolute value |
| 0x69   | SQRT     | Square root |
| 0x6A   | FMA      | Fused multiply-add |
| 0x6B   | CEIL     | Ceiling |
| 0x6C   | FLOOR    | Floor |
| 0x6D   | ROUND    | Round |
| 0x6E   | TRUNC    | Truncate |
| 0x7B   | MIN      | Minimum |
| 0x7C   | MAX      | Maximum |

### Vector Operations (0x90-0x9F)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x90   | VLOAD    | Load vector |
| 0x91   | VSTORE   | Store vector |
| 0x96   | VEXTRACT | Extract element |
| 0x97   | VINSERT  | Insert element |
| 0x98   | VSHUFFLE | Shuffle elements |
| 0x99   | VCMP     | Compare vectors |
| 0x9C   | VDOT     | Dot product |
| 0x9D   | VCROSS   | Cross product |
| 0x9E   | VHSUM    | Horizontal sum |

### Type Operations (0xA0-0xAF)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xA0   | TYPEOF   | Get type information |
| 0xA1   | SIZEOF   | Get size in bytes |
| 0xA2   | ALIGNOF  | Get alignment requirement |
| 0xA3   | CONVERT  | Convert between types |
| 0xA4   | CAST     | Reinterpret bit pattern |
| 0xA5   | STRUCT   | Define structure type |
| 0xA6   | GET      | Get structure field |
| 0xA7   | SET      | Set structure field |
| 0xA8   | INDEX    | Access array element |
| 0xA9   | UPDT     | Update array element |
| 0xAA   | TYPEDEF  | Create type alias |
| 0xAB   | ARRAY    | Define array type |

### Directive Instructions (0xB0-0xBF)

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xB0   | ARCH     | Set target architecture |
| 0xB1   | PROC     | Set processor type |
| 0xB2   | MODE     | Set processor mode |
| 0xB3   | ALIGN    | Align to boundary |
| 0xB4   | SECTION  | Define section |
| 0xB5   | DATA     | Insert data |
| 0xB6   | IF       | Conditional assembly start |
| 0xB7   | ELIF     | Conditional assembly alternative |
| 0xB8   | ELSE     | Conditional assembly default |
| 0xB9   | ENDIF    | Conditional assembly end |
| 0xBA   | ABI      | ABI definition start |
| 0xBB   | EXTERN   | Declare external symbol |
| 0xBC   | GLOBAL   | Declare global symbol |
| 0xBD   | INCLUDE  | Include file |
| 0xBE   | VERSION  | Specify COIL version |