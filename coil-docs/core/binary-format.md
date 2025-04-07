# COIL Binary Format

## Overview

This document specifies the binary encoding format for COIL instructions, operands, and program structure. The format is designed for efficient parsing, verification, and execution while maintaining platform independence.

## Instruction Encoding

All COIL instructions follow this basic structure:

```
[opcode: uint8_t] [extended opcode: optional uint8_t] [operand_count: uint8_t] [[operand: Operand], ...operand_count]
```

### Opcode Ranges

| Opcode Range | Category                | Extension     |
|--------------|-------------------------|---------------|
| 0x00-0x9F    | Universal operations    | Core          |
| 0xA0-0xDF    | Extended operations     | Processor     |
| 0xE0-0xEF    | Type operations         | Core          |
| 0xF0-0xFE    | Compiler operations     | Preprocessor  |
| 0xFF         | Vendor-specific         | Vendor        |

### Operand Count

The operand count is a single byte that specifies the number of operands that follow:
- **Always** present, even for instructions that never accept operands (where it will be 0)
- Maximum number of operands is 255 (0xFF) for any single instruction
- Each instruction defines its own maximum operand count which may be less than 255

### Extended Opcode

Some instructions require an extended opcode:
- When the primary opcode is 0xFF, the extended opcode byte is **mandatory** and specifies the vendor-specific operation
- Extended opcodes are **never** used in the current specification for any opcodes other than 0xFF
- Reserved for future use to expand instruction space when needed

## Operand Encoding

Operands are encoded as:

```
[type opcode: uint8_t] [type extension: uint8_t] [type data: optional ...] [data: optional ...]
```

### Type Opcode Ranges

| Opcode Range | Category                | Extension     |
|--------------|-------------------------|---------------|
| 0x00-0x0F    | Fixed-width integral    | Core          |
| 0x10-0x1F    | Fixed-width floating-point | Core       |
| 0x20-0x2F    | Fixed-width vector      | Core          |
| 0x30-0x3F    | Complex types           | Core/MultiDim |
| 0xA0-0xAF    | Composite types         | Composite     |
| 0xF0-0xF9    | Optimized types         | Core          |
| 0xFA-0xFF    | Special types           | Core          |

### Type Extension

The type extension is a bitmap that modifies the type:
```
CONST     = (1 << 0)  // Ensure no modifications
VOLATILE  = (1 << 1)  // Do not optimize expressions with this value
ATOMIC    = (1 << 2)  // Use atomic variant if possible
SATURATE  = (1 << 3)  // Cap at largest value instead of overflow
IMMEDIATE = (1 << 4)  // Value is embedded in instruction
DEFINITION= (1 << 5)  // Value is a compile-time definition
VARIABLE  = (1 << 6)  // Refers to a variable id
SYMBOL    = (1 << 7)  // Refers to a named memory address
```

The type extension byte is **always** present for all operands.

## Instruction Examples

### NOP (No Operation)

```
[0x00]             // NOP opcode
[0x00]             // 0 operands
```

### Integer Addition (Three-operand form)

Adding two 32-bit integers (a = b + c):

```
// ADD dest, src1, src2
[0x40]             // ADD opcode
[0x03]             // 3 operands
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_a]  // Variable ID for 'a' (uint64_t)
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_b]  // Variable ID for 'b' (uint64_t)
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_c]  // Variable ID for 'c' (uint64_t)
```

### Integer Addition with Immediate Value

Adding an immediate value to a variable (a += 5):

```
// ADD dest, immediate
[0x40]             // ADD opcode
[0x02]             // 2 operands
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_a]  // Variable ID for 'a' (uint64_t)
  [0x04][0x10]     // INT32 type, IMMEDIATE flag
  [0x00000005]     // Immediate value 5 (little-endian)
```

### Conditional Branch

Conditional branch if two values are equal:

```
// BR EQ, target_label
[0x01]             // BR opcode
[0x02]             // 2 operands
  [0xFE][0x00]     // PARAM0 type, FLAG_COND_EQ value
  [0xF2][0x80]     // SYMBOL type, SYMBOL flag
  [symbol_id]      // Symbol ID for target label (uint64_t)
```

### Vector Operation

Setting a vector element:

```
// SETE vector, index, value
[0x81]             // SETE opcode
[0x03]             // 3 operands
  [0x20][0x40]     // V128 type, VARIABLE flag
  [vector_id]      // Variable ID for vector (uint64_t)
  [0x04][0x10]     // INT32 type, IMMEDIATE flag
  [0x00000002]     // Immediate value 2 (element index)
  [0x04][0x10]     // INT32 type, IMMEDIATE flag
  [0x0000000A]     // Immediate value 10
```

## Endianness and Alignment

### Endianness

All multi-byte values in COIL binary format follow these endianness rules:

1. **Default Endianness**: Little-endian unless explicitly specified otherwise
2. **Endianness Marker**: Object files include an endianness marker in the header
3. **Multi-byte Fields**: Follow the file's specified endianness for all fields

### Instruction Alignment

COIL instructions follow these alignment rules:

1. **Basic Alignment**: Instructions are 1-byte aligned by default
2. **Architecture-Specific Alignment**: Target platforms may specify stricter alignment:
   - x86: 1-byte alignment
   - ARM: 4-byte alignment may be required for some modes
   - RISC-V: May require 2-byte or 4-byte alignment

## Object Format

COIL object files have the following structure:

```
[File Header]
[Section Table]
[Symbol Table]
[String Table]
[Relocation Table]
[Sections...]
```

### File Header

```c
struct CoilObjectHeader {
    char     magic[4];         // "COIL"
    uint32_t version;          // Version in format 0xMMmmpp
    uint32_t flags;            // File flags
    uint32_t target_pu;        // Target processing unit
    uint32_t target_arch;      // Target architecture
    uint32_t target_mode;      // Target mode
    uint64_t entry_point;      // Entry point (or 0 if not executable)
    uint32_t section_count;    // Number of sections
    uint32_t symbol_count;     // Number of symbols
    uint32_t reloc_count;      // Number of relocations
    uint64_t section_offset;   // Offset to section table
    uint64_t symbol_offset;    // Offset to symbol table
    uint64_t string_offset;    // Offset to string table
    uint64_t reloc_offset;     // Offset to relocation table
    uint8_t  endianness;       // 0 = little-endian, 1 = big-endian
    uint8_t  padding[7];       // Reserved for future use
}
```

## Nesting Limits

COIL enforces these limits on nested structures:

1. **Type Nesting**: Maximum 8 levels of nested types 
2. **Vector Nesting**: Maximum 3 levels (vectors of vectors of vectors)
3. **Matrix Dimensions**: Maximum 4 dimensions for tensors
4. **Struct Members**: Maximum 65,535 members in a structure
5. **Vector Elements**: Maximum 65,535 elements in a vector

## Implementation Requirements

1. **Byte Order Handling**: Correctly handle endianness conversion
2. **Alignment**: Respect architecture-specific alignment requirements
3. **Validation**: Verify instruction format, operand counts, and opcode validity
4. **Error Detection**: Report clear errors for malformed instructions
