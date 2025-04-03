# Memory Operations

## Overview

Memory operations manage the allocation, access, and manipulation of data in memory. These instructions provide a consistent interface for working with variables, memory regions, and program scopes across all architectures.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xE0   | TYPE     | Define a Type |
| 0xE1   | SIZEOF   | Get sizeof Type |
| 0xE2   | ALLIGNOF | Get natural allignment of type |
| 0xE3   | DTYPE    | Delete type definition |

## Detailed Specifications

### TYPE (0xE0)

Defines a type to be usable through a typeid

**Encoding:**
```
[0xE0][typeid: uint64_t][type opcode: uint8_t][type extension: optional uint8_t][type data: optional ...]
```

**Behavior:**
- Think of this as typedef, instead of using anonymous structures define it with TYPE and then utilize as specified in the type documentation.

### SIZEOF (0xE1)

Get sizeof type

**Encoding:**
```
[0xE1][type opcode: uint8_t][type extension: optional uint8_t][type data: optional ...]
```

**Behavior:**
- This opcode can be called as a type in an Operand expression where it can be thought as an immediate value of the types size

### ALLIGNOF (0xE2)

Get allignment of type

**Encoding:**
```
[0xE2][type opcode: uint8_t][type extension: optional uint8_t][type data: optional ...]
```

**Behavior:**
- This opcode can be called as a type in an Operand expression where it can be thought as an immediate value of the types allignment

### DTYPE (0xE3)

Delete a type by id removing any reference that this type id 

**Encoding:**
```
[0xE3][typeid: uint64_t]
```

**Behavior:**
- Delete a type by id removing any reference that this type id 

## Related Components

- [Type System](../types/type-system.md) - Complete type system reference