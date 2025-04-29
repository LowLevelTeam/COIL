# COIL Project

**C**omputer **O**riented **I**ntermediate **L**anguage (COIL) - A modern toolchain for compilation, optimization, and cross-platform development.

## Overview

COIL is a comprehensive toolchain that provides a platform-independent intermediate representation for code compilation and optimization. Similar to LLVM's approach, COIL defines a binary machine language that serves as a common format between front-end compilers and back-end machine code generators.

The COIL project aims to:
- Provide a well-defined intermediate representation for compiled code
- Enable cross-platform compilation and optimization
- Support the creation of language-specific front-ends
- Facilitate architecture-specific back-ends
- Offer a complete toolchain from source code to executable binaries

## Architecture

COIL follows a streamlined, cross-platform compilation process:

```
Source Code (.c, etc.) → COIL C Compiler (ccc) → COIL IR Files (.coil) → COIL Object Processor (COP) → COIL Native Objects (.coilo) → COIL Latent Linker (cll) → Executable
```

In this architecture:
1. Front-end compilers like CCC translate source code to the COIL intermediate representation
2. COP processes COIL IR into native machine code, but preserves it in the cross-platform COIL Object format (.coilo)
3. CLL handles the final transformation to platform-specific executable formats, managing all native object format complexities

This approach provides several key advantages:
- Maintains a consistent cross-platform representation until the final linking stage
- Concentrates platform-specific complexity in the CLL component
- Simplifies COP implementations by removing the need to output different native formats
- Enables more sophisticated cross-platform optimization opportunities

## Components

### Libraries

- **libcoilt**: Core toolchain utilities that replace standard C library functions with safe, optimized alternatives.
- **libcoil**: Library for reading, writing, and manipulating COIL files and objects.
- **libcop**: API for the COIL Object Processor functionality.
- **libcll**: API for the COIL Latent Linker functionality, handling native object formats internally.

### Tools

- **ccc (COIL C Compiler)**: Front-end compiler that translates C code to COIL intermediate representation.
- **cop (COIL Object Processor)**: Processes COIL IR files into COIL objects containing native machine code (.coilo).
- **cll (COIL Latent Linker)**: Transforms COIL objects into platform-specific executables, handling all native format requirements.

## File Formats

- **.coil**: The COIL intermediate representation file format
- **.coilo**: COIL object file format that contains native machine code while maintaining cross-platform compatibility

## Getting Started

### Prerequisites

- make
- C99 compatible compiler
- Git


### Example Usage

Compilation of a C file to executable:

```bash
# Compile C to COIL IR
ccc -o program.coil program.c

# Process COIL IR to COIL object with native code
cop -o program.coilo program.coil

# Link to executable for the current platform
cll -o program program.coilo

# Or link to executable for a different platform
cll -o program.exe --format=PE --pu=CPU --arch=x86-32 program.coilo
```

## Development Status

The COIL project is currently in active development:

- ✅ libcoilt: Basic utilities and memory management implemented
- ✅ libcoil: COIL file format reading and writing support
- 🔄 COP: Initial implementation in progress
- 🔄 CLL: Design phase with native object format support integrated

```
                  [libcoilt]
                     ↓
                  [libcoil]
                     ↓
[CCC,etc...] → [COP] → [CLL] → Native Executables
```

## Future Considerations

While the current architecture embeds native code in the COIL object format, future versions may explore a dedicated cross-platform native object format that provides additional optimization opportunities across different target platforms.

## License

This project is licensed under [LICENSE](LICENSE) - see the file for details.

## Documentation

See the [docs/](docs/) directory for detailed documentation including:
- Architecture documentation
- API references
- File format specifications
- Developer guides
- Example code