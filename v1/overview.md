# COIL Instruction Set Architecture Overview

## 1. Introduction

The COIL Instruction Set Architecture (ISA) defines the complete set of operations that can be performed in COIL. The instruction set is organized into logical categories, with clear separation between universal operations and architecture-specific operations.

## 2. Instruction Format

COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

## 3. Opcode Organization

The 8-bit opcode space is organized into functional categories:

```
0x00      - No Operation
0x01-0x0F - Control Flow
0x10-0x2F - Memory Operations
0x30-0x4F - Reserved for Multi-Device Operations (v3)
0x50-0x5F - Bit Manipulation
0x60-0x8F - Arithmetic
0x90-0x9F - Vector/Array Operations
0xA0-0xAF - Type Instructions
0xB0-0xBF - Directive Instructions
0xC0-0xFE - Architecture/Processor-Specific Instructions
0xFF      - COIL Processor-Specific Extensions
```

## 4. Instruction Categories

### 4.1 Control Flow (0x01-0x0F)

Instructions for controlling program execution flow:
- Symbol definition
- Conditional and unconditional branching
- Function calls and returns
- Comparison operations for setting flags

[Detailed documentation](./isa/cf.md)

### 4.2 Memory Operations (0x10-0x2F)

Instructions for memory access and management:
- Data movement
- Stack operations
- Memory allocation and scope management
- Variable declarations
- Memory copying, comparison, and manipulation

[Detailed documentation](./isa/memops.md)

### 4.3 Reserved Instructions (0x30-0x4F)

This range is reserved for multi-device operations in COIL v3.

[Placeholder documentation](./isa/resv.md)

### 4.4 Bit Manipulation (0x50-0x5F)

Instructions for binary operations:
- Logical operations (AND, OR, XOR, NOT)
- Shift and rotate operations
- Bit counting and manipulation

[Detailed documentation](./isa/bit.md)

### 4.5 Arithmetic (0x60-0x8F)

Instructions for mathematical operations:
- Basic arithmetic (add, subtract, multiply, divide)
- Advanced arithmetic (fused multiply-add, etc.)
- Comparison operations
- Conversion operations

[Detailed documentation](./isa/arith.md)

### 4.6 Vector/Array Operations (0x90-0x9F)

Instructions for operations on multiple values:
- Vector dot products
- Array operations
- SIMD-style operations

[Detailed documentation](./isa/vec.md)

### 4.7 Type Instructions (0xA0-0xAF)

Instructions for type manipulation:
- Type queries and information
- Type conversions
- Composite type operations
- Field and element access

[Detailed documentation](./isa/type.md)

### 4.8 Directive Instructions (0xB0-0xBF)

Instructions for controlling the compilation process:
- Architecture and processor selection
- Alignment and section control
- Conditional compilation
- ABI definitions
- Data insertion

[Detailed documentation](./isa/dir.md)

### 4.9 Special Instructions (0x00, 0xC0-0xFF)

Instructions specific to certain processors or architectures:
- No Operation (0x00)
- Processor-specific instructions (0xC0-0xFE)
- COIL processor extensions (0xFF)

[Detailed documentation](./isa/spec.md)

## 5. Instruction Operands

Instruction operands use the COIL type system to specify their type and behavior. Most instructions support a wide range of operand types, with behavior determined by the types provided.

Common operand patterns include:
- Source and destination
- Left and right operands with destination
- Conditional execution flags
- Control parameters

## 6. Conditional Execution

Many COIL instructions support conditional execution based on CPU flags:
- Equal/Not Equal
- Greater/Less Than
- Zero/Non-Zero
- Carry/No Carry
- Overflow/No Overflow
- Sign/No Sign

This allows for compact conditional code without branch instructions.

## 7. Implementation Requirements

A COIL v1 processor must:
1. Implement all instructions in categories 0x00-0x2F and 0x50-0xBF
2. Implement appropriate architecture-specific instructions for supported architectures
3. Reject instructions from the reserved range (0x30-0x4F)
4. Handle conditional execution correctly
5. Follow the operand type rules for each instruction

## 8. Recommended Extensions

While not required, COIL processors are encouraged to implement:
1. Common optimization patterns for instruction sequences
2. Hardware acceleration where available
3. Software fallbacks for instructions not directly supported by hardware
4. Efficient handling of conditional execution

## 9. Instruction Reference

Each instruction is documented with:
- Opcode
- Operand format
- Behavior description
- Type restrictions
- Examples

See the individual category documentation files for detailed instruction references.