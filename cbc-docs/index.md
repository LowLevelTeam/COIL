# COIL Byte Code (CBC)

## Overview

COIL Byte Code (CBC) is a compact, portable binary representation designed for efficient interpretation and just-in-time compilation of COIL programs. It serves as an intermediate format between COIL source code and machine-specific compiled code.

## Purpose

CBC addresses several key needs in the COIL ecosystem:

1. **Portable Execution**: Run COIL programs on platforms without a full COIL compiler
2. **Efficient Interpretation**: Optimize for fast execution in interpreted environments
3. **JIT Compilation**: Provide a suitable representation for dynamic compilation
4. **Reduced Size**: Minimize the binary size for embedded applications
5. **Platform Independence**: Ensure consistent execution across diverse environments

## Relationship to COIL

CBC is derived from COIL but differs in several key ways:

| Aspect       | COIL                          | CBC                           |
|--------------|-------------------------------|-------------------------------|
| Purpose      | Intermediate language         | Execution format              |
| Optimization | Platform-specific             | Interpreter-focused           |
| Encoding     | Type-rich, explicit           | Compact, implicit             |
| Structure    | Object-oriented               | Stack-oriented                |
| Scope        | Complete program              | Execution units               |

## CBC Format

CBC is structured as a series of instruction streams with supporting metadata:

```
[CBC Header]
[Constant Pool]
[Type Pool]
[Symbol Table]
[Function Table]
[Instruction Streams]
```

### CBC Header

```
struct CBCHeader {
    char     magic[4];      // "CBC\0"
    uint32_t version;       // CBC format version
    uint32_t flags;         // Format flags
    uint32_t pool_count;    // Number of constant pool entries
    uint32_t type_count;    // Number of type pool entries
    uint32_t sym_count;     // Number of symbol table entries
    uint32_t func_count;    // Number of function table entries
    uint64_t pool_offset;   // Constant pool offset
    uint64_t type_offset;   // Type pool offset
    uint64_t sym_offset;    // Symbol table offset
    uint64_t func_offset;   // Function table offset
    uint64_t code_offset;   // Start of instruction streams
}
```

## Instruction Format

CBC instructions use a compact format:

```
[opcode: uint8_t] [operands: variable]
```

Each opcode has a fixed operand count and format, allowing for efficient decoding.

## Execution Model

CBC uses a stack-based execution model:

1. Instructions push and pop values from an evaluation stack
2. Local variables are accessed by index
3. Function calls manipulate a separate call stack
4. Type information is resolved during execution

Example execution flow:
```
PUSH_CONST 42    // Push constant 42 onto stack
LOAD_LOCAL 0     // Push local variable 0 onto stack
ADD_INT         // Pop two values, add, push result
STORE_LOCAL 1    // Pop result into local variable 1
```

## Execution Environments

CBC can run in several environments:

1. **Interpreter**: Direct execution by a virtual machine
2. **JIT Compiler**: Dynamic compilation to native code
3. **Hybrid Execution**: Mix of interpretation and compilation

## Related Components

- [CBC Format](./cbc-format.md) - Detailed binary format specification
- [CBC Instruction Set](./cbc-instruction-set.md) - Complete instruction reference 
- [COIL Format](../coil-docs/core/binary-format.md) - COIL binary format