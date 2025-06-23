# Orion++ Binary Format Specification

## Overview

The Orion++ binary format (.orionpp) is a compact binary representation of Orion++ Intermediate Representation (IR) code. Unlike the human-readable text format, the binary format uses numeric opcodes and variable identifiers for efficient storage and faster parsing.

## File Structure

```
+------------------+
| Header           |
+------------------+
| String Table     |
+------------------+
| Symbol Table     |
+------------------+
| Function Table   |
+------------------+
| Instruction Data |
+------------------+
```

## Header Format

```c
struct OrionPPHeader {
  uint32_t magic;           // 0x4F50504F ("OPPO" - Orion++ Binary)
  uint16_t version;         // Format version (currently 1)
  uint16_t flags;           // Format flags
  uint32_t string_table_offset;
  uint32_t string_table_size;
  uint32_t symbol_table_offset;
  uint32_t symbol_table_size;
  uint32_t function_table_offset;
  uint32_t function_table_size;
  uint32_t instruction_data_offset;
  uint32_t instruction_data_size;
  uint32_t entry_point;     // Entry function ID
  uint32_t reserved[4];     // Reserved for future use
};
```

## String Table

Contains null-terminated strings referenced by offset throughout the file:
- Function names
- Symbol names
- Section names
- External references

Format: Consecutive null-terminated UTF-8 strings with no padding.

## Symbol Table

```c
struct OrionPPSymbol {
  uint32_t name_offset;     // Offset into string table
  uint8_t type;             // Symbol type (function, object, etc.)
  uint8_t visibility;       // local, global, weak
  uint16_t section;         // Section identifier
  uint32_t value;           // Symbol value/address
  uint32_t size;            // Symbol size
};
```

## Function Table

```c
struct OrionPPFunction {
  uint32_t name_offset;     // Offset into string table
  uint16_t param_count;     // Number of parameters
  uint16_t local_var_count; // Number of local variables
  uint32_t instruction_offset; // Offset into instruction data
  uint32_t instruction_count;  // Number of instructions
  uint8_t abi_type;         // ABI type (C, custom, etc.)
  uint8_t return_type;      // Return type
  uint16_t flags;           // Function flags
};
```

## Instruction Format

Each instruction is variable length with the following format:

```
+--------+--------+--------+--------+
| Opcode | Flags  |    Operands     |
+--------+--------+--------+--------+
```

- **Opcode** (8 bits): Instruction opcode
- **Flags** (8 bits): Instruction-specific flags
- **Operands** (variable): Operand data

### Variable References

Variables are referenced by numeric ID:
- Parameter variables: 0-255 (parameter index)
- Local variables: 256-65535 (local variable index + 256)
- Temporary variables: 65536+ (temporary index + 65536)

### Operand Encoding

Operands are encoded using a compact format:

```c
// Operand type in high 2 bits
#define OPERAND_IMMEDIATE   0x00  // 00xxxxxx - immediate value follows
#define OPERAND_VARIABLE    0x40  // 01xxxxxx - variable ID follows
#define OPERAND_LABEL       0x80  // 10xxxxxx - label offset follows
#define OPERAND_STRING      0xC0  // 11xxxxxx - string table offset follows

// Size in low 6 bits for variable-length operands
```

## Instruction Set Opcodes

### Core Instructions
```c
#define OP_NOP          0x00
#define OP_ENTER        0x01
#define OP_LEAVE        0x02
#define OP_RET          0x03
```

### Variable Operations
```c
#define OP_LET          0x10  // let var, value
#define OP_CONST        0x11  // const var, immediate
#define OP_MOV          0x12  // mov dest, src
```

### Arithmetic Operations
```c
#define OP_ADD          0x20  // add dest, src1, src2
#define OP_SUB          0x21  // sub dest, src1, src2
#define OP_MUL          0x22  // mul dest, src1, src2
#define OP_DIV          0x23  // div dest, src1, src2
#define OP_MOD          0x24  // mod dest, src1, src2
#define OP_NEG          0x25  // neg dest, src
```

### Bitwise Operations
```c
#define OP_AND          0x30  // and dest, src1, src2
#define OP_OR           0x31  // or dest, src1, src2
#define OP_XOR          0x32  // xor dest, src1, src2
#define OP_NOT          0x33  // not dest, src
#define OP_SHL          0x34  // shl dest, src, count
#define OP_SHR          0x35  // shr dest, src, count
```

### Control Flow
```c
#define OP_JMP          0x40  // jmp label
#define OP_CALL         0x41  // call function
#define OP_BEQ          0x42  // beq src1, src2, label
#define OP_BNE          0x43  // bne src1, src2, label
#define OP_BLT          0x44  // blt src1, src2, label
#define OP_BLE          0x45  // ble src1, src2, label
#define OP_BGT          0x46  // bgt src1, src2, label
#define OP_BGE          0x47  // bge src1, src2, label
#define OP_BEQZ         0x48  // beqz src, label
#define OP_BNEZ         0x49  // bnez src, label
```

### ABI Operations
```c
#define OP_ABI_CALLEE_ENTER    0x50
#define OP_ABI_CALLEE_LEAVE    0x51
#define OP_ABI_CALLEE_GETARG   0x52  // getarg dest, index
#define OP_ABI_CALLEE_SETRET   0x53  // setret index, src
#define OP_ABI_CALLER_SETUP    0x54
#define OP_ABI_CALLER_CLEANUP  0x55
#define OP_ABI_CALLER_SETARG   0x56  // setarg index, src
#define OP_ABI_CALLER_GETRET   0x57  // getret dest, index
```

## Example Binary Encoding

For the human-readable instruction:
```
isa.add result, a, b
```

Binary encoding (assuming variable IDs: result=256, a=257, b=258):
```
Opcode: 0x20 (OP_ADD)
Flags:  0x00
Dest:   0x41 0x00 0x01 (OPERAND_VARIABLE, var 256)
Src1:   0x41 0x01 0x01 (OPERAND_VARIABLE, var 257)
Src2:   0x41 0x02 0x01 (OPERAND_VARIABLE, var 258)
```

## Advantages of Binary Format

1. **Compact Size**: Significantly smaller than text representation
2. **Fast Parsing**: No tokenization or string parsing required
3. **Numeric Variables**: Eliminates string-based variable lookups
4. **Structured Access**: Direct random access to functions and symbols
5. **Validation**: Built-in integrity checking via offsets and sizes

## Tools

- **orionc++**: Compiles Orion++ human format to binary format and Orion binary format to an underlying orion variation.
- **oriondump**: Disassembles binary format back to human-readable
- **orionlink**: Links binary Orion++ objects into final executables

## Version History

- **Version 1**: Initial binary format specification