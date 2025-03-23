# COIL Specification Overview

## Introduction

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing units and architectures. COIL serves as an intermediate representation that bridges the gap between high-level programming languages and hardware-specific machine code.

## Core Design Philosophy

COIL is built on several foundational principles:

### Type-Determined Instruction Philosophy

Instructions in COIL derive their behavior from the types of their operands, rather than having numerous specialized opcodes. This allows for:

- Compact binary representation
- Consistent behavior across data types
- Extensibility without opcode explosion
- Simplified instruction decoding

### Architecture Independence

COIL maintains a clean separation between:

- Universal operations (available on all platforms)
- Processing unit specific operations
- Architecture specific operations
- Implementation specific extensions

This separation ensures maximum portability while still allowing direct access to hardware-specific features when needed.

### Performance-First Approach

COIL is designed to compile efficiently to native code without sacrificing performance:

- No abstraction penalties for universal operations
- Direct mapping to hardware capabilities where possible
- Optimization hints through type extensions
- Explicit control over performance-critical aspects

## Instruction Format

COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Value (variable length based on type)]
```

This format allows for:
- Efficient encoding of common operations
- Extensibility through type system
- Consistent decoding across implementations

## Implementation Approaches

COIL can be implemented through various methods:

1. **Just-In-Time (JIT) Compilation**: Converting COIL to native code at runtime
2. **Ahead-Of-Time (AOT) Compilation**: Preprocessing COIL to target architectures
3. **Interpretation**: Direct execution of COIL instructions
4. **Hardware Implementation**: Direct execution in compatible processing units

## Specification Organization

The COIL specification is organized into several key components:

1. **Type System**: Defines the data types and their representations
2. **Register System**: Describes the register model and mapping to hardware
3. **Instruction Set Architecture (ISA)**: Documents all supported instructions
   - Control Flow
   - Arithmetic Operations
   - Memory Operations
   - Vector Operations
   - Type Manipulation
   - Directives
   - Special Instructions
4. **Implementation Guidelines**: Provides guidance for implementers
5. **ABI Specifications**: Defines calling conventions for interoperability

## Version Control

The COIL specification uses semantic versioning:

- Major version changes (v1.0 → v2.0) indicate breaking changes
- Minor version changes (v1.0 → v1.1) indicate additions without breaking compatibility
- Patch version changes (v1.0.0 → v1.0.1) indicate clarifications or bug fixes

Each major version maintains its own directory in the specification repository.