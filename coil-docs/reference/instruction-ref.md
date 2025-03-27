# COIL Instruction Reference

## Purpose

This document provides a concise reference of all instructions in the COIL instruction set. It serves as a quick lookup guide for instruction opcodes, operands, and behaviors.

## Key Concepts

- **Instruction Format**: Universal binary structure of instructions
- **Opcode Organization**: How instruction opcodes are arranged by category
- **Operand Patterns**: Standard patterns for instruction operands
- **Conditional Execution**: How instructions can be conditionally executed

## Instruction Format

All COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

## Opcode Space Organization

The COIL opcode space is organized by functional categories:

| Opcode Range | Category | Description |
|--------------|----------|-------------|
| 0x00         | NOP      | No Operation |
| 0x01-0x0F    | Control Flow | Program flow, branches, calls |
| 0x10-0x2F    | Memory Operations | Data movement, variables, memory access |
| 0x30-0x4F    | Reserved | Reserved for multi-device operations (v3) |
| 0x50-0x5F    | Bit Manipulation | Logical operations, shifts, bit-level operations |
| 0x60-0x8F    | Arithmetic | Mathematical operations |
| 0x90-0x9F    | Vector/Array | Operations on multiple data elements |
| 0xA0-0xAF    | Type Operations | Type manipulation, structures, arrays |
| 0xB0-0xBF    | Directives | Processor selection, assembly control |
| 0xC0-0xFE    | Processor-Specific | Architecture/processor-specific instructions |
| 0xFF         | Extensions | Implementation-specific extensions |

## Control Flow Instructions (0x01-0x0F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x01   | SYM      | Symbol[, TYPE_PARAM0] | Define a symbol |
| 0x02   | BR       | Target[, TYPE_PARAM0][, TYPE_PARAM5] | Branch to target |
| 0x03   | CALL     | Target[, TYPE_PARAM0][, TYPE_PARAM5][, params...] | Call function |
| 0x04   | RET      | [TYPE_PARAM0][, TYPE_PARAM5][, return values...] | Return from function |
| 0x05   | CMP      | Left, Right | Compare values and set flags |
| 0x06   | TEST     | Left, Right | Bitwise AND and set flags |
| 0x07   | JMP      | Target[, TYPE_PARAM5] | Jump to target (alias for BR) |
| 0x08   | LOOP     | Counter, Target | Decrement counter and branch if not zero |
| 0x09   | SWITCH   | Value, CaseCount, Cases..., Default | Multi-way branch |

## Memory Operations (0x10-0x2F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x10   | MOV      | Destination, Source[, TYPE_PARAM5] | Move data |
| 0x11   | PUSH     | Source[, TYPE_PARAM5] | Push value to stack |
| 0x12   | POP      | Destination[, TYPE_PARAM5] | Pop value from stack |
| 0x13   | LEA      | Destination, Source[, TYPE_PARAM5] | Load effective address |
| 0x14   | SCOPEE   | None | Enter a variable scope |
| 0x15   | SCOPEL   | None | Leave a variable scope |
| 0x16   | VAR      | Type, Name[, InitialValue] | Declare a variable |
| 0x17   | MEMCPY   | Destination, Source, Size[, TYPE_PARAM5] | Copy memory block |
| 0x18   | MEMSET   | Destination, Value, Size[, TYPE_PARAM5] | Fill memory block |
| 0x19   | MEMCMP   | First, Second, Size | Compare memory blocks |
| 0x1A   | XCHG     | First, Second[, TYPE_PARAM5] | Exchange values |
| 0x1B   | CAS      | Destination, Expected, New[, TYPE_PARAM0][, TYPE_PARAM5] | Compare and swap |
| 0x2E   | PIN      | Address, Size | Pin memory |
| 0x2F   | UNPIN    | Address | Unpin memory |

## Bit Manipulation Instructions (0x50-0x5F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x50   | AND      | Destination, Left, Right[, TYPE_PARAM5] | Bitwise AND |
| 0x51   | OR       | Destination, Left, Right[, TYPE_PARAM5] | Bitwise OR |
| 0x52   | XOR      | Destination, Left, Right[, TYPE_PARAM5] | Bitwise XOR |
| 0x53   | NOT      | Destination, Source[, TYPE_PARAM5] | Bitwise NOT |
| 0x54   | SHL      | Destination, Value, Shift[, TYPE_PARAM5] | Shift left |
| 0x55   | SHR      | Destination, Value, Shift[, TYPE_PARAM5] | Shift right logical |
| 0x56   | SAR      | Destination, Value, Shift[, TYPE_PARAM5] | Shift right arithmetic |
| 0x57   | ROL      | Destination, Value, Rotate[, TYPE_PARAM5] | Rotate left |
| 0x58   | ROR      | Destination, Value, Rotate[, TYPE_PARAM5] | Rotate right |
| 0x59   | POPCNT   | Destination, Source[, TYPE_PARAM5] | Count set bits |
| 0x5A   | BSWAP    | Destination, Source[, TYPE_PARAM5] | Byte swap |
| 0x5B   | CLZ      | Destination, Source[, TYPE_PARAM5] | Count leading zeros |
| 0x5C   | CTZ      | Destination, Source[, TYPE_PARAM5] | Count trailing zeros |

## Arithmetic Instructions (0x60-0x8F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x60   | ADD      | Destination, Left, Right[, TYPE_PARAM5] | Addition |
| 0x61   | SUB      | Destination, Left, Right[, TYPE_PARAM5] | Subtraction |
| 0x62   | MUL      | Destination, Left, Right[, TYPE_PARAM5] | Multiplication |
| 0x63   | DIV      | Destination, Left, Right[, TYPE_PARAM5] | Division |
| 0x64   | MOD      | Destination, Left, Right[, TYPE_PARAM5] | Modulo |
| 0x65   | INC      | Destination[, TYPE_PARAM5] | Increment |
| 0x66   | DEC      | Destination[, TYPE_PARAM5] | Decrement |
| 0x67   | NEG      | Destination, Source[, TYPE_PARAM5] | Negate |
| 0x68   | ABS      | Destination, Source[, TYPE_PARAM5] | Absolute value |
| 0x69   | SQRT     | Destination, Source[, TYPE_PARAM5] | Square root |
| 0x6A   | FMA      | Destination, A, B, C[, TYPE_PARAM5] | Fused multiply-add |
| 0x6B   | CEIL     | Destination, Source[, TYPE_PARAM5] | Ceiling |
| 0x6C   | FLOOR    | Destination, Source[, TYPE_PARAM5] | Floor |
| 0x6D   | ROUND    | Destination, Source[, TYPE_PARAM5] | Round |
| 0x6E   | TRUNC    | Destination, Source[, TYPE_PARAM5] | Truncate |
| 0x7B   | MIN      | Destination, Left, Right[, TYPE_PARAM5] | Minimum |
| 0x7C   | MAX      | Destination, Left, Right[, TYPE_PARAM5] | Maximum |

## Vector/Array Operations (0x90-0x9F)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x90   | VLOAD    | Destination, Source[, TYPE_PARAM0][, TYPE_PARAM5] | Load vector |
| 0x91   | VSTORE   | Destination, Source[, TYPE_PARAM0][, TYPE_PARAM5] | Store vector |
| 0x96   | VEXTRACT | Destination, Source, Index[, TYPE_PARAM5] | Extract element |
| 0x97   | VINSERT  | Destination, Source, Value, Index[, TYPE_PARAM5] | Insert element |
| 0x98   | VSHUFFLE | Destination, Source, Mask[, TYPE_PARAM5] | Shuffle elements |
| 0x99   | VCMP     | Destination, Left, Right, CompareType[, TYPE_PARAM5] | Compare vectors |
| 0x9A   | VBLEND   | Destination, First, Second, Mask[, TYPE_PARAM5] | Blend vectors |
| 0x9B   | VMASKMOV | Destination, Source, Mask[, TYPE_PARAM5] | Masked move |
| 0x9C   | VDOT     | Destination, Left, Right[, TYPE_PARAM5] | Dot product |
| 0x9D   | VCROSS   | Destination, Left, Right[, TYPE_PARAM5] | Cross product |
| 0x9E   | VHSUM    | Destination, Source[, TYPE_PARAM5] | Horizontal sum |

## Type Operations (0xA0-0xAF)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0xA0   | TYPEOF   | Destination, Source[, TYPE_PARAM5] | Get type |
| 0xA1   | SIZEOF   | Destination, Source[, TYPE_PARAM5] | Get size |
| 0xA2   | ALIGNOF  | Destination, Source[, TYPE_PARAM5] | Get alignment |
| 0xA3   | CONVERT  | Destination, Source[, TYPE_PARAM5] | Convert type |
| 0xA4   | CAST     | Destination, Source[, TYPE_PARAM5] | Reinterpret type |
| 0xA5   | STRUCT   | Destination, Fields... | Define structure |
| 0xA6   | GET      | Destination, Source, Field[, TYPE_PARAM5] | Get structure field |
| 0xA7   | SET      | Destination, Field, Value[, TYPE_PARAM5] | Set structure field |
| 0xA8   | INDEX    | Destination, Source, Index[, TYPE_PARAM5] | Access array element |
| 0xA9   | UPDT     | Destination, Index, Value[, TYPE_PARAM5] | Update array element |
| 0xAA   | TYPEDEF  | Destination, Source | Create type alias |
| 0xAB   | ARRAY    | Destination, ElementType, Size | Define array type |

## Directive Instructions (0xB0-0xBF)

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0xB0   | ARCH     | Architecture[, Mode] | Set target architecture |
| 0xB1   | PROC     | Processor | Set processor type |
| 0xB2   | MODE     | Mode | Set processor mode |
| 0xB3   | ALIGN    | Alignment | Align to boundary |
| 0xB4   | SECTION  | Name, Attributes | Define section |
| 0xB5   | DATA     | Type, Value | Insert data |
| 0xB6   | IF       | Condition | Conditional assembly start |
| 0xB7   | ELIF     | Condition | Conditional assembly alternative |
| 0xB8   | ELSE     | None | Conditional assembly default |
| 0xB9   | ENDIF    | None | Conditional assembly end |
| 0xBA   | ABI      | Name | ABI definition start |
| 0xBB   | EXTERN   | Symbol[, Type] | Declare external symbol |
| 0xBC   | GLOBAL   | Symbol | Declare global symbol |
| 0xBD   | INCLUDE  | Filename | Include file |
| 0xBE   | VERSION  | Major, Minor, Patch | Specify COIL version |

## ABI-specific Instructions

These instructions are used within ABI blocks:

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x00   | EXIT     | None | End ABI definition |
| 0x01   | PARAMS   | Registers... | Define parameter registers |
| 0x02   | RETS     | Registers... | Define return registers |
| 0x03   | CALLER   | Registers... | Define caller-saved registers |
| 0x04   | CALLEE   | Registers... | Define callee-saved registers |
| 0x05   | SALLIGN  | Alignment | Define stack alignment |
| 0x06   | RZONE    | Size | Define red zone size |

## Conditional Execution

Most instructions support conditional execution using the TYPE_PARAM5 parameter with a branch_condition_t value:

| Condition | Mnemonic Suffix | Description |
|-----------|----------------|-------------|
| 0x00      | _EQ            | Equal |
| 0x01      | _NE            | Not equal |
| 0x02      | _GE            | Greater than or equal |
| 0x03      | _LT            | Less than |
| 0x04      | _GT            | Greater than |
| 0x05      | _LE            | Less than or equal |
| 0x06      | _Z             | Zero flag set |
| 0x07      | _NZ            | Zero flag not set |
| 0x08      | _C             | Carry flag set |
| 0x09      | _NC            | Carry flag not set |
| 0x0A      | _O             | Overflow flag set |
| 0x0B      | _NO            | Overflow flag not set |
| 0x0C      | _S             | Sign flag set |
| 0x0D      | _NS            | Sign flag not set |

## Related Documentation

For more detailed information on instruction operation, see the category-specific documentation:
- [Control Flow Instructions](../spec/instruction-set/control-flow.md)
- [Memory Operations](../spec/instruction-set/memory.md)
- [Bit Manipulation Instructions](../spec/instruction-set/bit-manipulation.md)
- [Arithmetic Instructions](../spec/instruction-set/arithmetic.md)
- [Vector/Array Operations](../spec/instruction-set/vector.md)
- [Type Operations](../spec/instruction-set/type.md)
- [Directive Instructions](../spec/instruction-set/directive.md)
- [Binary Format](../spec/binary-format.md)