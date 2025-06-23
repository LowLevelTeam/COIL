# Orion++ Binary Format Specification

## Overview

The Orion++ binary format (.orionpp) provides efficient storage and loading of intermediate representation files.

## File Layout

```
┌─────────────────────┐ Offset 0
│ Header (32 bytes)   │
├─────────────────────┤
│ String Table        │
├─────────────────────┤  
│ Function Table      │
├─────────────────────┤
│ Instruction Data    │
└─────────────────────┘
```

## Header Structure

```c
struct OrionHeader {
  uint32_t magic;           // 0x4F52494F ("ORIO")
  uint16_t version;         // Format version (2)
  uint16_t flags;           // Reserved (0)
  uint32_t string_offset;   // String table offset
  uint32_t string_size;     // String table size  
  uint32_t function_offset; // Function table offset
  uint32_t function_size;   // Function table size
  uint32_t code_offset;     // Instruction data offset
  uint32_t code_size;       // Instruction data size
  uint32_t entry_point;     // Entry function index
  uint32_t reserved;        // Reserved (0)
} __attribute__((packed));
```

## String Table

Contains null-terminated UTF-8 strings referenced by offset.

```
┌─────────────────────┐ Offset 0
│ \0                  │ (empty string)
├─────────────────────┤ Offset 1  
│ "main\0"            │
├─────────────────────┤ Offset 6
│ "add\0"             │
└─────────────────────┘
```

## Function Table

Array of function entries:

```c
struct OrionFunctionEntry {
  uint32_t name_offset;     // Offset into string table
  uint16_t param_count;     // Number of parameters
  uint16_t flags;           // Function flags
  uint32_t code_offset;     // Offset into instruction data
  uint32_t code_size;       // Size in bytes
  uint32_t first_var_id;    // First variable ID
  uint32_t last_var_id;     // Last variable ID
} __attribute__((packed));
```

### Function Flags

| Bit | Name | Description |
|-----|------|-------------|
| 0 | ABI_C | Uses C calling convention |
| 1 | RETURNS_WORD | Returns word type |
| 2-15 | Reserved | Must be zero |

## Instruction Encoding

### Instruction Format

```
┌─────────┬─────────┬─────────────────────┐
│ Opcode  │ Count   │ Operands            │
│ 8 bits  │ 8 bits  │ Variable length     │  
└─────────┴─────────┴─────────────────────┘
```

### Operand Encoding

Each operand is 5 bytes:

```
┌─────────┬─────────────────────────────────────┐
│ Type    │ Value (32-bit little-endian)        │
│ 8 bits  │ 32 bits                             │
└─────────┴─────────────────────────────────────┘
```

### Operand Types

| Value | Type | Description |
|-------|------|-------------|
| 0x00 | Immediate | Signed integer constant |
| 0x01 | Variable | Variable ID |
| 0x02 | Label | Relative offset in bytes |
| 0x03 | Symbol | String table offset |

### Core Opcodes

```c
// Control
#define OP_NOP      0x00
#define OP_ENTER    0x01
#define OP_LEAVE    0x02  
#define OP_RET      0x03

// Variables
#define OP_CONST    0x10    // dest, immediate
#define OP_LET      0x11    // dest, src

// Arithmetic  
#define OP_ADD      0x20    // dest, a, b
#define OP_SUB      0x21    // dest, a, b
#define OP_MUL      0x22    // dest, a, b
#define OP_DIV      0x23    // dest, a, b
#define OP_NEG      0x24    // dest, src

// Control flow
#define OP_JMP      0x30    // label
#define OP_CALL     0x31    // symbol
#define OP_BEQ      0x32    // a, b, label
#define OP_BNE      0x33    // a, b, label
#define OP_BLT      0x34    // a, b, label
#define OP_BGT      0x35    // a, b, label

// ABI
#define OP_CALLEE_ENTER     0x40
#define OP_CALLEE_LEAVE     0x41
#define OP_CALLEE_GETARG    0x42    // dest, index
#define OP_CALLEE_SETRET    0x43    // index, value
#define OP_CALLER_SETUP     0x44
#define OP_CALLER_CLEANUP   0x45
#define OP_CALLER_SETARG    0x46    // index, value
#define OP_CALLER_GETRET    0x47    // dest, index
```

## Encoding Examples

### Simple Instruction

```
Human: isa.const $256, 42
Binary: 11 02 01 00 01 00 00 00 00 2A 00 00 00
        │  │  │  └─ Variable ID 256
        │  │  └──── Variable operand
        │  └─────── 2 operands
        └────────── CONST opcode
                    └─ Immediate value 42
```

### Function Call

```
Human: isa.call @"add"
Binary: 31 01 03 05 00 00 00
        │  │  │  └─ String offset 5 ("add")
        │  │  └──── Symbol operand
        │  └─────── 1 operand
        └────────── CALL opcode
```

## Loading Process

1. **Memory Map**: Map entire file into memory
2. **Validate Header**: Check magic number and version
3. **Setup Pointers**: Calculate section pointers from offsets
4. **Validate Sections**: Check bounds and alignment
5. **Build Indices**: Create symbol and function lookup tables

## Implementation Notes

### Endianness
All multi-byte values are little-endian.

### Alignment
All structures are naturally aligned and packed.

### Error Handling
- Invalid magic number → ORION_ERROR_FORMAT
- Unsupported version → ORION_ERROR_FORMAT  
- Truncated file → ORION_ERROR_FORMAT
- Invalid offsets → ORION_ERROR_FORMAT

### Memory Requirements
Typical overhead: 16 bytes per function + 8 bytes per string + instruction data.

### Performance
- Loading: O(1) - memory mapping only
- Symbol lookup: O(1) - direct offset access
- Instruction access: O(1) - array indexing

---

*This format provides optimal balance between compactness and loading speed for Orion++ intermediate representation.*