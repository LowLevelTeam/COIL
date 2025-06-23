# Orion++ Binary Format Specification

This document describes the binary file format (.orionpp) used by the Orion++ intermediate representation.

## Overview

The Orion++ binary format is designed for:
- **Compact Size**: Efficient storage using numeric IDs and compressed structures
- **Fast Loading**: Direct memory mapping and minimal parsing
- **Multiple IR Support**: Can contain multiple intermediate representations
- **Cross-Platform**: Consistent binary layout across architectures

## File Structure

```
+----------------------------+
| Header (64 bytes)          |
+----------------------------+
| String Table               |
+----------------------------+
| Symbol Table               |
+----------------------------+
| Function Table             |
+----------------------------+
| Variable Table             |
+----------------------------+
| Instruction Data           |
+----------------------------+
```

## Header Format

```c
typedef struct {
  uint32_t magic;                   // 0x4F50504F ("OPPO")
  uint16_t version;                 // Format version (currently 2)
  uint16_t flags;                   // Format flags
  uint32_t string_table_offset;     // Offset to string table
  uint32_t string_table_size;       // Size of string table in bytes
  uint32_t symbol_table_offset;     // Offset to symbol table
  uint32_t symbol_table_size;       // Size of symbol table in bytes
  uint32_t function_table_offset;   // Offset to function table
  uint32_t function_table_size;     // Size of function table in bytes
  uint32_t variable_table_offset;   // Offset to variable table
  uint32_t variable_table_size;     // Size of variable table in bytes
  uint32_t instruction_data_offset; // Offset to instruction data
  uint32_t instruction_data_size;   // Size of instruction data in bytes
  uint32_t entry_point;             // Entry function index
  uint32_t next_var_id;             // Next available variable ID
  uint32_t reserved[2];             // Reserved for future use
} OrionPPHeader;
```

### Header Fields

- **magic**: Must be `0x4F50504F` ("OPPO" in ASCII)
- **version**: Format version, currently `2`
- **flags**: Bitfield for format options (currently unused)
- **entry_point**: Index of the entry function (typically `main`)
- **next_var_id**: Next variable ID to use when adding variables

## String Table

Contains null-terminated UTF-8 strings referenced by offset throughout the file.

### Format
- Consecutive null-terminated strings
- No padding between strings
- First byte is always `\0` (null string at offset 0)

### Example
```
Offset  Content
0       \0
1       "main\0"
6       "add\0"
10      "global_var\0"
```

## Symbol Table

```c
typedef struct {
  uint32_t name_offset;     // Offset into string table
  uint8_t type;             // Symbol type
  uint8_t visibility;       // Symbol visibility
  uint16_t section;         // Section identifier
  uint32_t value;           // Symbol value/address
  uint32_t size;            // Symbol size in bytes
} OrionPPSymbol;
```

### Symbol Types
- `0x01`: Function
- `0x02`: Object (data)
- `0x03`: Section

### Symbol Visibility
- `0x00`: Local (file scope)
- `0x01`: Global (external linkage)
- `0x02`: Weak (can be overridden)

## Function Table

```c
typedef struct {
  uint32_t symbol_offset;       // Index into symbol table
  uint16_t param_count;         // Number of parameters
  uint16_t local_count;         // Number of local variables
  uint32_t instruction_offset;  // Offset into instruction data
  uint32_t instruction_count;   // Number of instruction bytes
  uint8_t abi_type;             // ABI type
  uint8_t return_type;          // Return type
  uint16_t flags;               // Function flags
  uint32_t first_var_id;        // First variable ID in function
  uint32_t last_var_id;         // Last variable ID in function
} OrionPPFunction;
```

### ABI Types
- `0x01`: C calling convention
- `0x02`: Custom calling convention

### Return Types
- `0x00`: void
- `0x01`: i32

## Variable Table

```c
typedef struct {
  uint32_t id;              // Unique variable ID
  uint8_t type;             // Variable type
  uint32_t name_offset;     // Offset into string table (debug info)
  uint8_t data_type;        // Data type
  uint32_t scope_id;        // Scope identifier
} OrionPPVariable;
```

### Variable Types
- `0x00`: Temporary variable
- `0x01`: Local variable
- `0x02`: Parameter
- `0x03`: Global variable

### Data Types
- `0x00`: void
- `0x01`: i32

## Instruction Data

Variable-length instruction encoding with the following format:

```
+--------+--------+--------+--------+
| Opcode | Flags  |    Operands     |
+--------+--------+--------+--------+
   8-bit   8-bit     Variable length
```

### Instruction Format

Each instruction consists of:
1. **Opcode** (8 bits): Instruction type
2. **Flags** (8 bits): Instruction-specific flags (currently unused)
3. **Operands** (variable): Zero or more operands

### Operand Encoding

Operands are encoded with a type byte followed by data:

```c
// Operand type in high 2 bits
#define OPERAND_IMMEDIATE   0x00  // 00xxxxxx - immediate value follows
#define OPERAND_VARIABLE    0x40  // 01xxxxxx - variable ID follows  
#define OPERAND_LABEL       0x80  // 10xxxxxx - label offset follows
#define OPERAND_SYMBOL      0xC0  // 11xxxxxx - string table offset follows
```

#### Immediate Operand
```
+--------+--------+--------+--------+--------+
| 0x00   |        32-bit immediate value      |
+--------+--------+--------+--------+--------+
```

#### Variable Operand
```
+--------+--------+--------+--------+--------+
| 0x40   |        32-bit variable ID          |
+--------+--------+--------+--------+--------+
```

#### Label Operand
```
+--------+--------+--------+--------+--------+
| 0x80   |        32-bit offset               |
+--------+--------+--------+--------+--------+
```

#### Symbol Operand
```
+--------+--------+--------+--------+--------+
| 0xC0   |        32-bit string offset       |
+--------+--------+--------+--------+--------+
```

## Instruction Set

### Core Instructions
```c
#define OP_NOP          0x00
#define OP_ENTER        0x01
#define OP_LEAVE        0x02
#define OP_RET          0x03
```

### Variable Operations
```c
#define OP_LET          0x10  // let dest, src
#define OP_CONST        0x11  // const dest, immediate
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
#define OP_CALL         0x41  // call symbol
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
#define OP_ABI_CALLEE_ENTER     0x50
#define OP_ABI_CALLEE_LEAVE     0x51
#define OP_ABI_CALLEE_GETARG    0x52  // getarg dest, index
#define OP_ABI_CALLEE_SETRET    0x53  // setret index, src
#define OP_ABI_CALLER_SETUP     0x54
#define OP_ABI_CALLER_CLEANUP   0x55
#define OP_ABI_CALLER_SETARG    0x56  // setarg index, src
#define OP_ABI_CALLER_GETRET    0x57  // getret dest, index
```

## Example Binary Encoding

### Simple Function

Human format:
```assembly
obj.sym[global,func] @"add":
  abi.callee.enter
  isa.enter
    isa.const $1, 10
    isa.const $2, 20
    isa.add $3, $1, $2
    abi.callee.setret.0 $3
  isa.leave
  abi.callee.leave
  isa.RET
```

Binary encoding:
```
Header:
  magic: 0x4F50504F
  version: 2
  string_table_offset: 64
  string_table_size: 5
  symbol_table_offset: 69
  symbol_table_size: 16
  function_table_offset: 85
  function_table_size: 24
  variable_table_offset: 109
  variable_table_size: 48
  instruction_data_offset: 157
  instruction_data_size: 43

String Table (5 bytes):
  0: \0
  1: "add\0"

Symbol Table (16 bytes):
  Symbol 0:
    name_offset: 1
    type: 0x01 (function)
    visibility: 0x01 (global)
    section: 0
    value: 0
    size: 43

Function Table (24 bytes):
  Function 0:
    symbol_offset: 0
    param_count: 0
    local_count: 3
    instruction_offset: 0
    instruction_count: 43
    abi_type: 0x01 (C)
    return_type: 0x01 (i32)
    flags: 0
    first_var_id: 1
    last_var_id: 3

Variable Table (48 bytes):
  Variable 0: id=1, type=local, data_type=i32
  Variable 1: id=2, type=local, data_type=i32
  Variable 2: id=3, type=local, data_type=i32

Instruction Data (43 bytes):
  0x50, 0x00                          // abi.callee.enter
  0x01, 0x00                          // isa.enter
  0x11, 0x00, 0x40, 0x01,0x00,0x00,0x00, 0x00, 0x0A,0x00,0x00,0x00  // isa.const $1, 10
  0x11, 0x00, 0x40, 0x02,0x00,0x00,0x00, 0x00, 0x14,0x00,0x00,0x00  // isa.const $2, 20
  0x20, 0x00, 0x40, 0x03,0x00,0x00,0x00, 0x40, 0x01,0x00,0x00,0x00, 0x40, 0x02,0x00,0x00,0x00  // isa.add $3, $1, $2
  0x53, 0x00, 0x00, 0x00,0x00,0x00,0x00, 0x40, 0x03,0x00,0x00,0x00  // abi.callee.setret.0 $3
  0x02, 0x00                          // isa.leave
  0x51, 0x00                          // abi.callee.leave
  0x03, 0x00                          // isa.RET
```

## Advantages of Binary Format

1. **Compact Size**: 
   - Variable IDs use 4 bytes vs. variable-length strings
   - String table deduplication
   - No parsing overhead

2. **Fast Loading**:
   - Direct memory mapping possible
   - Fixed-size structures
   - No tokenization required

3. **Efficient Processing**:
   - Random access to functions and symbols
   - Built-in validation via offsets and sizes
   - Structured metadata

## Version History

- **Version 1**: Initial binary format
- **Version 2**: Added variable table, updated variable ID system, symbol improvements

## Tools

- **liborionpp-dev**: Library for reading/writing binary format
- **orionc++**: Compiler that generates binary format
- **oriondump**: Disassembler for binary format inspection
- **orionlink**: Linker that processes binary objects

## Validation

The binary format includes several validation mechanisms:

1. **Magic Number**: Verify file type
2. **Version Check**: Ensure compatibility
3. **Offset Validation**: All offsets must be within file bounds
4. **Size Validation**: Table sizes must be multiples of entry sizes
5. **String Table**: All string offsets must be valid
6. **Variable IDs**: All variable references must exist in variable table

## Portability

The binary format is designed to be portable across:
- **Architectures**: Uses fixed-size integers with consistent endianness
- **Operating Systems**: No OS-specific features
- **Compilers**: Standard C data types with explicit sizes

All multi-byte integers are stored in little-endian format for consistency.