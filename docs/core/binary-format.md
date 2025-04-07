# COIL Binary Format

## Overview

This document specifies the binary encoding format for COIL instructions, operands, and program structure. The format is designed for efficient parsing and execution while maintaining platform independence.

## Instruction Encoding

All COIL instructions follow this basic structure:

```
[opcode: uint8_t][operand_count: uint8_t][[operand: Operand], ...operand_count]
```

When the opcode is 0xFF, an extended opcode byte follows to specify vendor-specific operations.

### Opcode Ranges

| Opcode Range | Category |
|--------------|----------|
| 0x00-0x9F    | Universal operations |
| 0xA0-0xDF    | Extended operations |
| 0xE0-0xEF    | Type operations |
| 0xF0-0xFE    | Compiler operations |
| 0xFF         | Vendor-specific |

### Operand Count

- Single byte specifying number of operands that follow
- Maximum number of operands is 255 (0xFF)
- Always present, even for instructions with no operands (where it will be 0)

## Operand Encoding

Operands are encoded as:

```
[type_opcode: uint8_t][type_extension: uint8_t][type_data: optional][data: optional]
```

### Type Opcode Ranges

| Opcode Range | Category |
|--------------|----------|
| 0x00-0x0F    | Fixed-width integral |
| 0x10-0x1F    | Fixed-width floating-point |
| 0x20-0x2F    | Fixed-width vector |
| 0x30-0x3F    | Complex types |
| 0xA0-0xAF    | Composite types |
| 0xF0-0xFF    | Special types |

### Type Extension

The type extension is a bitmap with these flags:

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

## Instruction Examples

### NOP (No Operation)

```
[0x00]             // NOP opcode
[0x00]             // 0 operands
```

### Integer Addition (Three-operand form)

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

```
// BR EQ, target_label
[0x01]             // BR opcode
[0x02]             // 2 operands
  [0xFE][0x00]     // PARAM0 type, FLAG_COND_EQ value
  [0xF2][0x80]     // SYMBOL type, SYMBOL flag
  [symbol_id]      // Symbol ID for target label (uint64_t)
```

## Endianness and Alignment

### Endianness

- **Default Endianness**: Little-endian unless explicitly specified otherwise
- **Endianness Marker**: Object files include an endianness marker in the header

### Instruction Alignment

- **Basic Alignment**: Instructions are 1-byte aligned by default
- **Architecture-Specific Alignment**: Target platforms may specify stricter alignment
  - x86: 1-byte alignment
  - ARM: 4-byte alignment may be required for some modes
  - RISC-V: May require 2-byte or 4-byte alignment

## Nesting Limits

COIL enforces these limits on nested structures:

- **Type Nesting**: Maximum 8 levels of nested types 
- **Vector Nesting**: Maximum 3 levels (vectors of vectors of vectors)
- **Matrix Dimensions**: Maximum 4 dimensions for tensors
- **Struct Members**: Maximum 65,535 members in a structure
- **Vector Elements**: Maximum 65,535 elements in a vector