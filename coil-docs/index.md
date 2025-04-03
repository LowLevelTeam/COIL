# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Core Design Philosophy

COIL is built on these foundational concepts:

### Type-Determined Instructions

Instructions derive their behavior from operand types, allowing a single opcode to handle different data types:

```
ADD result, a, b  // Works for integers, floats, vectors, matrices based on operand types
```

This approach provides:
- A compact instruction set
- Consistent behavior patterns across types
- Natural extensibility as new types are added

### Architecture Independence

COIL maintains a clean separation between:

- **Universal Operations** (0x00-0x9F): Work identically across all platforms
- **Extended Opeations** (0xA0-0xDF): Works on specific targets according to the rules defined
- **Type Operations** (0xE0-0xEF): Tailored to specific architecture modes
- **Directive Operations** (0xF0-0xFE): Direct communication with the COIL Processor
- **Extension Operations** (0xFF): Vendor-specific features

### Instruction Sets

COIL is organized into three main instruction sets:

1. **ISA Universal (ISA-U)**: Core instructions that work on all platforms
   - Control flow, memory operations, arithmetic, bit manipulation
   - Type-determined behavior adapts to operand types
   - Maximum portability across all environments

2. **ISA Extended (ISA-E)**: Platform-specific instructions
   - Organized by processing unit, architecture, and mode
   - Enables hardware-specific optimizations
   - Conditional compilation for portability

3. **ISA Compiler (ISA-C)**: Directives for the COIL processor
   - Configuration and compilation control
   - ABI definitions and linking directives
   - Conditional compilation constructs

## Type System

COIL's type system defines how data is represented and manipulated:

- **Fixed-Width Types**: Standard integer and floating-point types
- **Vector Types**: SIMD-friendly multi-element types
- **Matrix Types**: Two-dimensional data structures with algebraic properties
- **Tensor Types**: Multi-dimensional data structures
- **Complex Types**: Custom-width numeric types
- **Composite Types**: Structured data aggregates

Types determine instruction behavior, memory layout, and optimization opportunities.

## Binary Format

COIL uses a compact binary format for representing programs:

- **Instructions**: [opcode][operand count][operands...]
- **Operands**: [type opcode][type extension][type data][value]
- **Object Format**: Header, sections, symbols, and code

The binary format is designed for efficient parsing, verification, and execution.

## Configuration System

COIL programs can be configured for specific targets:

- **Target Specification**: Processing unit, architecture, and mode
- **Feature Detection**: Available hardware capabilities
- **Optimization Settings**: Performance vs. size tradeoffs
- **ABI Configuration**: Calling conventions and memory model

Configuration files use a standardized format for consistent results across tools.

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## Related Components

- [Binary Format](./core/binary-format.md) - COIL binary encoding specification
- [Configuration Format](./core/config-format.md) - Target configuration specification
- [Type System](./types/type-system.md) - Complete type system reference
- [Universal ISA](./isa-u/overview.md) - Universal instruction set
- [Extended ISA](./isa-e/overview.md) - Extended instruction set
- [Compiler ISA](./isa-c/index.md) - Compiler directives