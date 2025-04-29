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

COIL follows a multi-stage compilation process:

```
Source Code (.c, etc.) → COIL C Compiler (ccc) → COIL Files (.coil) → COIL Object Processor (COP) → COIL Objects (.coilo) → COIL Latent Linker (cll) → Executable
```

Alternatively, COIL can generate native object files:

```
COIL Files (.coil) → COIL Object Processor (COP) → Native Objects (.o) → OS Linker → Executable
```

The second approach (using CLL) provides greater control over the compilation process and better support for cross-platform development and optimization.

## Components

### Libraries

- **libcoilt**: Core toolchain utilities that replace standard C library functions with safe, optimized alternatives.
- **libcoil**: Library for reading, writing, and manipulating COIL files.
- **libnof**: Library for working with native object file formats across different platforms.
- **libcop**: API for the COIL Object Processor functionality.
- **libcll**: API for the COIL Latent Linker functionality.

### Tools

- **ccc (COIL C Compiler)**: Front-end compiler that translates C code to COIL representation.
- **cop (COIL Object Processor)**: Processes COIL files into either native object files or COIL objects with machine code (.coilo uses the same format).
- **cll (COIL Latent Linker)**: Links COIL object files and native binaries into executable formats.

## File Formats

- **.coil**: The COIL intermediate representation file format
- **.coilo**: COIL object file format that contains compiled COIL code ready for linking

## Getting Started

### Prerequisites

- CMake 3.15+
- C11 compatible compiler
- Git

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/coil-project.git
cd coil-project

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run tests
make test

# Install
make install
```

### Example Usage

Basic compilation of a C file to executable:

```bash
# Compile C to COIL
ccc -o program.coil program.c

# Process COIL to COIL object
cop -o program.coilo program.coil

# Link to executable
cll -o program program.coilo
```

Direct compilation to native object:

```bash
# Compile C to COIL
ccc -o program.coil program.c

# Process COIL to native object
cop --native -o program.o program.coil

# Link with native linker
gcc -o program program.o
```

## Development Status

The COIL project is currently in active development:

- ✅ libcoilt-dev: Basic utilities and memory management implemented
- ✅ libcoil-dev: COIL file format reading and writing support
- 🔄 libnof-dev: Native object format support under development
- 🔄 COP: Initial implementation in progress
- 🔄 CLL: Design phase

```
                  [libcoilt-dev]
      [libcoil-dev]         [libnof-dev]
[CCC,etc...]   ->    [COP]       ->      [CLL]
```

## License

This project is licensed under [LICENSE](LICENSE) - see the file for details.

## Documentation

See the [docs/](docs/) directory for detailed documentation including:
- Architecture documentation
- API references
- File format specifications
- Developer guides
- Example code