# COIL Development Roadmap

This document outlines the planned development path for the Computer Oriented Intermediate Language (COIL) across its major versions.

## Version 1: Core Specification (Current)

COIL v1 establishes the foundation with a focus on CPU targeting:

### Goals
- Complete CPU-oriented instruction set
- Comprehensive type system
- Foundational binary formats (COIL object and COIL output object)
- Application Binary Interface (ABI) mechanisms
- CPU register system
- Configuration format
- Debug information format

### Timeline
- **Q1-Q2 2025:** Finalize v1 specification
- **Q3 2025:** First reference implementation of COIL processor
- **Q4 2025:** Initial toolchain support

### Key Deliverables
1. Complete specification documentation
2. Reference implementation
3. Basic tooling (assembler/disassembler)
4. Sample programs

## Version 2: Standard Library

COIL v2 will extend v1 by defining a comprehensive standard library:

### Goals
- Maintain full backward compatibility with v1
- Define core utility functions and data structures
- Establish mathematics library
- Provide I/O operations framework
- Create system interface APIs
- Implement common algorithms

### Timeline
- **Q1-Q2 2026:** Draft v2 standard library specification
- **Q3-Q4 2026:** Implementation and testing

### Key Deliverables
1. Standard library specification
2. Reference implementation
3. Extended toolchain support
4. Test suite

## Version 3: Multi-Device Support

COIL v3 will enable programs to utilize multiple heterogeneous processing units:

### Goals
- Support for GPUs, TPUs, DSPs, FPGAs, and other accelerators
- Device management and selection
- Cross-device memory management
- Synchronization primitives
- Parallel execution model
- JIT compilation support

### Timeline
- **2027:** Design and specification
- **2028:** Implementation and ecosystem development

### Key Deliverables
1. Multi-device extension specification
2. Support for major accelerator types
3. JIT compilation framework
4. Performance optimization guidelines
5. Extended toolchain with multi-device support

## Contributing

We welcome contributions to any phase of the roadmap. Please refer to CONTRIBUTING.md for guidelines on how to participate in COIL's development.