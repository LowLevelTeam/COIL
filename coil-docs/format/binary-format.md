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
- When the primary opcode is 0xFF, the extended opcode byte is **mandatory** and specifies the vendor-specific operation
- Extended opcodes are **never** used in the current specification for any opcodes other than 0xFF
- Reserved for future use to expand instruction space when needed

### Operand Count

The operand count is a single byte that specifies the number of operands that follow:
- **Always** present, even for instructions that never accept operands (in which case it will be 0)
- Maximum number of operands is 255 (0xFF) for any single instruction
- Each instruction defines its own maximum operand count which may be less than 255

### Special Cases

- **Zero-operand instructions**: Always include the operand count byte set to 0
- **Fixed-operand instructions**: Still include the operand count byte for consistency
- **Directive operations**: Follow the same format, with operand count followed by operands

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

The type extension byte is **always** present for all operands.

### Type Data

Additional data required to fully specify the type:
- For complex types: size information
- For composite types: structure definition
- For vector types: element type and count

Type data size and format depends on the specific type, as defined in the type system documentation.

### Data

The actual value or reference:
- For immediate values: literal data of the specified type with size determined by the type
- For variables: variable ID (uint64_t)
- For symbols: symbol index (uint64_t)

## Type Size and Alignment

Types have defined sizes and alignment requirements:

| Type          | Size (bytes)              | Alignment (bytes)         |
|---------------|---------------------------|---------------------------|
| INT8/UNT8     | 1                         | 1                         |
| INT16/UNT16   | 2                         | 2                         |
| INT32/UNT32   | 4                         | 4                         |
| INT64/UNT64   | 8                         | 8                         |
| FP32          | 4                         | 4                         |
| FP64          | 8                         | 8                         |
| V128          | 16                        | 16                        |
| V256          | 32                        | 32                        |
| V512          | 64                        | 64                        |
| Complex Types | Varies (as specified)     | Largest component         |
| Composite     | Sum of components + padding| Largest component         |

## Complete Instruction Examples

### NOP (No Operation)

```
[0x00]             // NOP opcode
[0x00]             // 0 operands
```

### Integer Addition Example (Three-operand form)

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

### Integer Addition Example (Two-operand form)

Incrementing a 32-bit integer (a += b):

```
// ADD dest, src
[0x40]             // ADD opcode
[0x02]             // 2 operands
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_a]  // Variable ID for 'a' (uint64_t)
  [0x04][0x40]     // INT32 type, VARIABLE flag
  [variable_id_b]  // Variable ID for 'b' (uint64_t)
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

### Conditional Branch Example

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

### Structure Type Definition Example

```
// VAR struct_var, STRUCT {...}
[0x26]             // VAR opcode
[0x02]             // 2 operands
  [0xF1][0x00]     // VARID type
  [variable_id]    // Variable ID (uint64_t)
  [0xA0][0x00]     // STRUCT type
  [0x0003]         // 3 members
    [0x0001]       // Member ID
    [0x04][0x00]   // INT32 type
    [0x0002]       // Member ID
    [0x11][0x00]   // FP64 type
    [0x0003]       // Member ID
    [0x20][0x00]   // V128 type
    [0x10][0x00]   // Element type: FP32
```

## Endianness Handling

All multi-byte values in COIL binary format follow these endianness rules:

1. **File-level Endianness**: Determined by the configuration specified in the file header
2. **Default Endianness**: Little-endian unless explicitly specified otherwise
3. **Endianness Marker**: Object files include an endianness marker in the header
4. **Multi-byte Fields**: Follow the file's specified endianness for all:
   - Immediate values
   - Offsets and addresses
   - Size and count fields
   - Type data

For cross-platform compatibility, endianness conversion must be applied when:
- Reading COIL files with different endianness than the host
- Generating COIL files for a target with different endianness

## Instruction Alignment

COIL instructions follow these alignment rules:

1. **Basic Alignment**: Instructions are 1-byte aligned by default
2. **Architecture-Specific Alignment**: Target platforms may specify stricter alignment:
   - x86: 1-byte alignment
   - ARM: 4-byte alignment may be required for some modes
   - RISC-V: May require 2-byte or 4-byte alignment

The COIL processor is responsible for:
- Respecting the target architecture's alignment requirements
- Inserting padding bytes when necessary to maintain alignment
- Documenting alignment requirements in the configuration

## Nesting Limits

COIL enforces these limits on nested structures:

1. **Type Nesting**: Maximum 8 levels of nested types (structs within structs)
2. **Vector Nesting**: Maximum 3 levels (vectors of vectors of vectors)
3. **Matrix Dimensions**: Maximum 4 dimensions for tensors
4. **Struct Members**: Maximum 65,535 members in a structure
5. **Vector Elements**: Maximum 65,535 elements in a vector

## Related Components

- [Object Format](./object-format.md) - COIL object file specification
- [Type System](../types/type-system.md) - Complete type system reference