# COIL v1 Core Specification

## 1. Introduction

The Computer Oriented Intermediate Language (COIL) version 1 defines a complete Instruction Set Architecture (ISA) capable of targeting any single processing unit, with a primary focus on CPUs. This specification establishes the foundational binary format, instruction set, and type system that enables portable code generation across diverse architectures while maintaining native performance characteristics.

COIL follows a type-determined instruction philosophy where behavior is inferred from operand types, enabling compact representation while maintaining expressive power. This specification defines the complete framework for implementing, extending, and utilizing COIL in software and hardware environments.

### 1.1 Design Philosophy

COIL is built on several foundational principles:

#### 1.1.1 Type-Determined Instruction Philosophy

Instructions in COIL derive their behavior from the types of their operands, rather than having numerous specialized opcodes. This allows for:

- Compact binary representation
- Consistent behavior across data types
- Extensibility without opcode explosion
- Simplified instruction decoding

#### 1.1.2 Architecture Independence

COIL maintains a clean separation between:

- Universal operations (available on all platforms)
- Processing unit specific operations
- Architecture specific operations
- Implementation specific extensions

This separation ensures maximum portability while still allowing direct access to hardware-specific features when needed.

#### 1.1.3 Performance-First Approach

COIL is designed to compile efficiently to native code without sacrificing performance:

- No abstraction penalties for universal operations
- Direct mapping to hardware capabilities where possible
- Optimization hints through type extensions
- Explicit control over performance-critical aspects

### 1.2 Scope of v1

COIL v1 is focused on:
- Support for single processing unit targeting, primarily CPUs
- Complete type system and instruction set
- Foundational binary formats
- Basic Application Binary Interface (ABI) mechanisms

Future versions will extend COIL to support:
- Standard library (v2)
- Multi-device coordination (v3) including GPUs, TPUs, and other accelerators
- Cross-architecture optimizations

## 2. Binary Encoding

### 2.1 Instruction Format

COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

The Type field is composed of:
- Main Type (8 bits) - Defines the primary type category
- Type Extensions (8 bits) - Provides qualifiers and additional type information

### 2.2 Opcode Organization

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

#### 2.2.1 Reserved Instruction Space

The range 0x30-0x4F is intentionally reserved in COIL v1 for future multi-device operations in COIL v3. This space will later include instructions for:

- Device selection and switching
- Cross-device memory transfers
- Synchronization between devices
- Parallel execution coordination
- Device capability queries
- Queue and workgroup management

Attempting to use opcodes in this range in v1 will result in an invalid instruction error.

## 3. Implementation Requirements

To qualify as a COIL v1 processor, an implementation must:

1. Support the full core instruction set
2. Implement the complete type system
3. Handle the object format for both input and output
4. Process the configuration format
5. Support at least one target CPU architecture fully

## 4. Version Compatibility

COIL v1 establishes the foundation for future versions. Implementations should:

1. Use version markers in generated files
2. Reject incompatible version constructs
3. Clearly identify any extensions beyond the core specification

Future versions will maintain backward compatibility with valid v1 code:
- COIL v2 will add a standard library
- COIL v3 will add multi-device support