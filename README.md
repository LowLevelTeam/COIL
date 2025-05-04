# COIL - Computer Oriented Intermediate Language

COIL (Computer Oriented Intermediate Language) is a modern toolchain for compilation, optimization, and cross-platform development. It provides a unified approach to generating machine code for diverse hardware targets while maintaining portability.

## Overview

COIL is a comprehensive toolchain that provides a platform-independent intermediate representation for code compilation and optimization. Similar to LLVM's approach, COIL defines a binary machine language that serves as a common format between front-end compilers and back-end machine code generators.

The COIL project's key features include:
- A well-defined intermediate representation for compiled code
- Cross-platform compilation and optimization
- Support for multiple processing units (CPU, GPU, etc.) and architectures
- Embedding of native machine code with metadata for specific targets
- A complete toolchain from source code to executable binaries

## Architecture

COIL follows a streamlined, cross-platform compilation process:

```
Source Code → COIL C Compiler (ccc) → COIL IR Files (.coil) → COIL Object Processor (COP) → COIL CBC Objects (.coilbc) → COIL Object Code Generator (COCG) → COIL Native Objects (.coilo) → COIL Latent Linker (cll) → Executable
```

In this architecture:
1. Front-end compilers like CCC translate source code to the COIL intermediate representation
2. COP processes COIL IR into CBC
3. COCG process CBC into Native Mahine Code
4. CLL handles the final transformation to platform-specific executable formats, managing all native object format complexities

## COIL and CBC: Complementary Internal Representations

COIL utilizes a two-tier representation approach within its processing pipeline:

### COIL IR: Type-Determined Representation
- High-level, type-determined language where operations are determined by operand types
- Supports advanced features like multi-dimensional vectors, composite types, and complex expressions
- Focuses on platform-independent code representation and optimization
- Provides rich type information and semantics for analysis and transformation

### CBC (COIL Byte Code): Instruction-Determined Representation
- An internal lower-level, instruction-determined representation similar to native assembly
- Each CBC instruction contains all necessary information for direct code generation
- Facilitates:
  - Fast JIT compilation and interpretation
  - Simplified native code generation process with less repeated logic

## Native Code Support

COIL introduces a powerful approach to handling native code across multiple platforms:

### Double Value Architecture Specification

COIL uses a dual-level specification for target architectures:
1. **Processing Unit (PU)**: The type of processor (CPU, GPU, TPU, etc.)
2. **Architecture**: The specific architecture within that PU category (x86-64, ARM64, NVIDIA PTX, etc.)

This approach allows COIL to:
- Support a wide range of hardware targets
- Clearly separate platform-specific code
- Enable cross-compilation between different architectures
- Expand to new processing units as they emerge

### Native Code in COIL Objects

COIL object files can contain native machine code alongside the IR:
- Each section can have associated metadata specifying its target PU and architecture
- Multiple native code sections can exist for different targets
- Feature flags allow fine-grained control over architecture-specific optimizations

The `.coilo` extension is used for COIL object files that contain native code, though the underlying format is the same as regular COIL files with additional metadata.

## Components

### Libraries

- **libcoil**: Core library for reading, writing, and manipulating COIL objects.
- **libcop**: API for the COIL Object Processor functionality, translating CBC to native code.
- **libcll**: API for the COIL Latent Linker functionality, handling native object formats internally.

### Tools

- **ccc (COIL C Compiler)**: Front-end compiler that translates C code to COIL intermediate representation.
- **cop (COIL Object Processor)**: Processes COIL IR files into COIL objects containing CBC (.coilbc).
- **cocg (CBC Code Generator)**: Transforms COIL Byte Code into COIL objects containing native machine code (.coilo).
- **cll (COIL Latent Linker)**: Transforms COIL objects into platform-specific executables for various targets.

## File Formats

- **.coil**: COIL object file format that contains COIL IR code
- **.coilbc**: COIL object file format that contains COIL Byte Code
- **.coilo**: COIL object file format that contains native machine code

## Supported Platforms

### Processing Units
- CPU (Central Processing Unit)
- GPU (Graphics Processing Unit)
- TPU (Tensor Processing Unit)
- NPU (Neural Processing Unit)
- DSP (Digital Signal Processor)
- FPGA (Field-Programmable Gate Array)

### CPU Architectures
- x86 (16 bit, 32-bit and 64-bit)
- ARM (thumb, 32-bit and 64-bit)
- RISC-V (32-bit, 64-bit and 128-bit)
- PowerPC (32-bit and 64-bit)
- MIPS (32-bit and 64-bit)
- WebAssembly (32-bit and 64-bit)

### GPU Architectures
- NVIDIA (SASS)
- AMD (GCN, RDNA)
- Intel (Gen9, Xe)

Note: Most GPU code will utilize the COIL Byte Code (CBC) internal representation for runtime JIT, so GPU device code will almost never be stored directly in objects.

## Getting Started

### Prerequisites

- make
- C99 compatible compiler
- Git

### Build

```bash
git clone https://github.com/coil-project/coil.git
cd coil
mkdir build && cd build
cmake ..
make
```
## Development Status

The COIL project is currently in active development:

- ✅ libcoil: COIL file format reading and writing support with native code capabilities
- 🔄 COP: Implementation for transpiling COIL to CBC
- 🔄 COCG: Implementing with multiple target architecture support
- 🔄 CLL: Design phase with native object format support integrated

## License

This project is licensed under [LICENSE](LICENSE) - see the file for details.

