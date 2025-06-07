# TBD Toolchain

> **Note**: This project is in early development. The name "TBD" is a placeholder while we decide on the final branding.

A modern cross-platform toolchain for compilation, optimization, and development targeting multiple processor architectures with a unified approach to instruction set design.

## Overview

TBD Toolchain introduces a novel approach to cross-platform development through a three-layer architecture:

- **Low-Level ISA**: Target-specific instruction sets optimized for different processor families
- **High-Level ISA (TBD++)**: A unified intermediate representation for high-level language constructs
- **Object Format**: A standardized object file format supporting mixed native and intermediate code

## Architecture

### Low-Level ISA

The Low-Level ISA consists of multiple target-specific instruction sets, each optimized for different processor families and their execution models.

#### CPU-Default (Flag-Based Architecture)

Designed for processors that use condition flags for branching and comparisons, including:

- **x86 family**: 16-bit, 32-bit, and 64-bit variants
- **ARM family**: Thumb, 32-bit, and 64-bit variants
- **Other compatible architectures**

**Key Characteristics:**
- Flag-based conditional execution and branching
- Unified support for 64-bit operations (with appropriate performance considerations on legacy hardware)
- Optional features: MMU, interrupts, atomics, vector operations
- Backwards compatibility with 16-bit systems (with performance trade-offs)

**Design Rationale:**
Traditional RISC architectures like RISC-V use immediate comparisons for branching (e.g., `bnez rs1, .L_loop`), while established architectures like x86 and ARM rely heavily on condition flags. This creates a fundamental incompatibility that prevents unified toolchain development. CPU-Default bridges this gap by standardizing around the flag-based model used by the majority of deployed processors.

#### CPU-NoFlags

*Documentation in progress*

Target group for processors that use immediate comparison branching without condition flags.

### High-Level ISA (TBD++)

TBD++ serves as an intermediate representation that abstracts common high-level language constructs, making it easier for compiler developers to target multiple architectures without dealing with low-level architectural differences.

**Features:**
- Standardized instruction set for common programming constructs
- Direct mapping to high-level language features (C/C++ and similar)
- Architecture-agnostic design
- Seamless integration with target-specific Low-Level ISA code

## Object Format

TBD Objects support mixed-mode compilation, containing both TBD++ intermediate code, TDB ISA and native machine code for optimal performance and flexibility.

## Toolchain Components

### Processor

The TBD Processor handles the translation pipeline:

1. **TBD++ Processing**: Parses and optimizes high-level intermediate code
2. **Target Translation**: Converts TBD++ to target-specific TBD
3. **Final translation**: Convert TBD target group tailored instruction sets into machine code 

### Development Tools

#### Compiler Integration
- **For Compiler Developers**: Target TBD++ to leverage automatic cross-platform support
- **Focus on Syntax**: Let the toolchain handle architecture-specific optimizations

#### Assembler Support
- **Direct Low-Level ISA**: Full access to target-specific features
- **Direct Assembly**: Target TBD for native instructions

#### Object Tools
- **objdump**: Specialized disassembly handling for mixed TBD++/native objects
- **Linker**: Support for TBD Objects and native formats (ELF64, PE, MACH-O, ...)

## Getting Started

*Documentation in progress*

## Roadmap

### Current Focus
- Complete CPU-Default ISA specification
- Implement core TBD++ instruction set
- Develop reference processor implementation

### Future Expansion
- **GPU Support**: Async execution models and specialized instruction sets
- **FPGA Integration**: Hardware-specific optimizations
- **Accelerator Processors**: Support for specialized compute units

## Contributing

*Guidelines coming soon*

## License

*To be determined*

---

**Status**: Early Development | **Latest Update**: June 2025