# COIL Binary Format

## Overview

This document specifies the binary encoding format for COIL instructions, operands, and program structure. The format is designed for efficient parsing, verification, and execution while maintaining platform independence.

## Instruction Encoding

All COIL instructions follow this basic structure:

```
[opcode: uint8_t] [extended opcode: optional uint8_t] [operand_count: uint8_t] [[operand: Operand], ...operand_count]
```

### Opcode

The first byte of every instruction is the primary opcode:

- **0x00-0x9F**: Universal operations
- **0xA0-0xDF**: Target operations
- **0xF0-0xFE**: Directive operations
- **0xFF**: Extension operations

### Extended Opcode

Some instructions require an extended opcode:
- When the primary opcode is 0xFF, the extended opcode specifies the vendor-specific operation
- When additional precision is needed within an instruction category

### Operand Count

The operand count is a single byte that specifies the number of operands that follow.

Notable exceptions:
- Instructions that never accept operands (e.g., NOP) may omit the operand count
- Certain directive operations have fixed operand formats

## Operand Encoding

Operands are encoded as:

```
[type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]
```

### Type Opcode

The type opcode specifies the basic type:
- **0x00-0x0F**: Fixed-width integral types
- **0x10-0x1F**: Fixed-width floating-point types
- **0x20-0x2F**: Fixed-width vector types
- **0x30-0x3F**: Complex types
- **0xA0-0xAF**: Composite types
- **0xF0-0xF9**: Optimized types
- **0xFA-0xFF**: Special types

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

### Type Data

Additional data required to fully specify the type:
- For complex types: size information
- For composite types: structure definition
- For vector types: element type and count

### Data

The actual value or reference:
- For immediate values: literal data of the specified type
- For variables: variable ID (uint64_t)
- For symbols: symbol index (uint64_t)

## Complete Instruction Examples

### Integer Addition Example

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

### Branch If Equal Example

Conditional branch if two values are equal:

```
// BR EQ, target_label
[0x01]             // BR opcode
[0x02]             // 2 operands
  [0xFE][0x00]     // PARAM0 type, FLAG_COND_EQ value
  [0xF2][0x80]     // SYMBOL type, SYMBOL flag
  [symbol_id]      // Symbol ID for target label (uint64_t)
```

### Vector Operation Example

Setting a vector element:

```
// VSET vector, index, value
[0x81]             // VSET opcode
[0x03]             // 3 operands
  [0x20][0x40]     // V128 type, VARIABLE flag
  [vector_id]      // Variable ID for vector (uint64_t)
  [0x04][0x10]     // INT32 type, IMMEDIATE flag
  [0x00000002]     // Immediate value 2 (element index)
  [0x04][0x10]     // INT32 type, IMMEDIATE flag
  [0x0000000A]     // Immediate value 10
```

## Endianness Considerations

- All multi-byte values follow the endianness specified in the configuration
- By default, COIL uses the endianness of the target architecture
- For universal portability, files may include an endianness marker

## Alignment Requirements

Instruction and operand alignment follows these rules:

1. Instructions begin at architecture-appropriate boundaries
   - For most platforms: 1-byte aligned
   - For RISC platforms: may require 2 or 4-byte alignment

2. Operands follow immediately after the instruction header
   - No padding between operands by default
   - Explicit alignment may be required by certain architectures

3. Data elements align according to their natural alignment
   - 16-bit values: 2-byte alignment
   - 32-bit values: 4-byte alignment
   - 64-bit values: 8-byte alignment

## Edge Cases and Special Values

1. **Immediate Zero Values**
   - May use optimized encoding for common zero values

2. **Null References**
   - Variable ID 0 represents a null reference
   - Symbol ID 0 represents an undefined symbol

3. **Extended Types**
   - Types exceeding standard width use the Complex type encoding

4. **Overflow and Underflow**
   - Handled according to the SATURATE flag

## Platform-Specific Variations

While the core binary format remains consistent, platforms may have special requirements:

1. **Big Endian vs. Little Endian**
   - Multi-byte values stored according to platform endianness
   - Conversion performed during cross-platform execution

2. **Instruction Alignment**
   - Some platforms require instructions to be aligned
   - Padding bytes may be inserted to meet alignment requirements

3. **Register References**
   - Platform-specific register encoding in the REG type
   - Abstracted through the variable system for portability

4. **Extended Instruction Sets**
   - Vendor-specific extensions use the 0xFF opcode range
   - Must include capability detection for portability

## Binary Format Evolution

The binary format includes version information to support evolution:

1. **Version Identification**
   - Major version changes indicate incompatible format changes
   - Minor version changes add new capabilities while maintaining compatibility

2. **Compatibility Layer**
   - Processors can implement translation for older formats
   - Format converters available for offline conversion

## Related Components

- [Object Format](./object-format.md) - COIL object file specification
- [Type System](../types/type-system.md) - Complete type system reference
- [Instruction Reference](../instruction-reference.md)