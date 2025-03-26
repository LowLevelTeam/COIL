# COIL Toolchain

## Purpose

This document describes the standard toolchain for working with COIL, including the tools, their functionality, command-line interfaces, and how they interact within the development process.

## Toolchain Overview

The COIL toolchain consists of the following primary tools:

1. **coilasm**: COIL assembler (COIL-ASM to COIL binary)
2. **coildis**: COIL disassembler (COIL binary to COIL-ASM)
3. **coilobj**: COIL object file utility
4. **coillink**: COIL linker
5. **coilproc**: COIL processor (execution environment)
6. **coildbg**: COIL debugger

## Tool Descriptions

### coilasm - COIL Assembler

The COIL assembler converts COIL-ASM text to COIL binary format.

#### Usage

```
coilasm [options] input.casm [-o output.coil]
```

#### Options

- `-o, --output <file>`: Specify output file (default: input base name with .coil extension)
- `-I, --include <dir>`: Add directory to include search path
- `-v, --verbose`: Enable verbose output
- `-W, --warnings`: Enable additional warnings
- `-O<level>`: Optimization level (0-3)
- `-g`: Include debug information
- `-f, --feature <feature>`: Enable specific features
- `-t, --target <target>`: Specify target architecture
- `-h, --help`: Show help message

#### Example

```
coilasm -O2 -g -I./include program.casm -o program.coil
```

### coildis - COIL Disassembler

The COIL disassembler converts COIL binary to COIL-ASM text.

#### Usage

```
coildis [options] input.coil [-o output.casm]
```

#### Options

- `-o, --output <file>`: Specify output file (default: standard output)
- `-v, --verbose`: Enable verbose output
- `-c, --comments`: Include additional comments in output
- `-r, --raw`: Disassemble raw binary (no object format)
- `-s, --symbols`: Include symbol information
- `-a, --all`: Disassemble all sections
- `-h, --help`: Show help message

#### Example

```
coildis -c -s program.coil -o program.casm
```

### coilobj - COIL Object Utility

The COIL object utility inspects and manipulates COIL object files.

#### Usage

```
coilobj [command] [options] input.coil
```

#### Commands

- `info`: Display general information about the object file
- `symbols`: List symbols in the object file
- `sections`: List sections in the object file
- `dump`: Dump contents of a section
- `extract`: Extract a section to a file
- `replace`: Replace a section from a file
- `strip`: Remove specified information from the object
- `add`: Add information to the object

#### Options

- `-o, --output <file>`: Specify output file for modifications
- `-s, --section <section>`: Specify section for commands that operate on sections
- `-f, --format <format>`: Specify output format (text, binary, hex)
- `-v, --verbose`: Enable verbose output
- `-h, --help`: Show help message

#### Example

```
coilobj symbols program.coil
coilobj dump -s .text program.coil
coilobj strip -s .debug program.coil -o program.stripped.coil
```

### coillink - COIL Linker

The COIL linker combines multiple COIL object files into an executable or larger object file.

#### Usage

```
coillink [options] input1.coil input2.coil ... -o output.coil
```

#### Options

- `-o, --output <file>`: Specify output file
- `-l, --library <lib>`: Link with library
- `-L, --library-path <dir>`: Add directory to library search path
- `-s, --script <script>`: Use linker script
- `-v, --verbose`: Enable verbose output
- `-M, --map <file>`: Generate map file
- `-S, --strip`: Strip symbols from output
- `-e, --entry <symbol>`: Specify entry point
- `-h, --help`: Show help message

#### Example

```
coillink main.coil lib1.coil lib2.coil -o program.coil
```

### coilproc - COIL Processor/Compiler

The COIL processor is the final compilation step that transforms COIL binary objects (.coil) into target-specific executable code (.coilo). It performs the architecture-specific translation and optimization.

#### Usage

```
coilproc [options] input.coil -o output.coilo
```

#### Options

- `-o, --output <file>`: Specify output file
- `-v, --verbose`: Enable verbose output
- `-O<level>`: Optimization level (0-3)
- `-t, --target <arch>`: Specify target architecture (overrides file)
- `-i, --inline`: Enable aggressive inlining
- `-d, --debug`: Include debug information
- `-p, --peephole`: Enable peephole optimizations
- `-r, --reg-alloc <alg>`: Register allocation algorithm
- `-h, --help`: Show help message

#### Example

```
coilproc -O2 -t x86_64 input.coil -o output.coilo
```

### coildbg - COIL Debugger

The COIL debugger provides debugging capabilities for COIL programs.

#### Usage

```
coildbg [options] program.coil [args...]
```

#### Options

- `-c, --command <file>`: Execute debugger commands from file
- `-q, --quiet`: Quiet mode
- `-d, --directory <dir>`: Add source directory
- `-b, --breakpoint <loc>`: Set breakpoint at location
- `-h, --help`: Show help message

#### Example

```
coildbg -b main program.coil
```

## Toolchain Integration

The tools are designed to work together in a complete workflow:

1. Write COIL-ASM code in `.casm` files
2. Assemble with `coilasm` to produce `.coil` object files
3. Compile to target architecture with `coilproc` to produce `.coilo` files
4. Link object files with `coillink` to create the final executable
5. Debug with `coildbg` when needed

## File Extensions

The COIL toolchain uses the following file extensions:

- `.casm`: COIL assembly source files
- `.coil`: COIL binary object files
- `.coilo`: COIL output object files (with native code)
- `.coilh`: COIL header files
- `.coila`: COIL archive (library) files
- `.coild`: COIL debug information files
- `.coils`: COIL linker script files

## Standard Libraries

The toolchain includes standard libraries:

1. **libcoil-std**: Core functionality
2. **libcoil-io**: Input/output operations
3. **libcoil-math**: Mathematical functions
4. **libcoil-mem**: Memory management

## Development Environment Integration

The COIL toolchain is designed to integrate with common development environments:

1. **Command Line Interface**: All tools provide CLI access
2. **Build Systems**: Support for make, CMake, and other build systems
3. **IDE Integration**: API for IDE integration
4. **CI/CD Systems**: Support for continuous integration workflows

## Extension Mechanism

The toolchain provides extension points for:

1. **Custom Backends**: Supporting additional target architectures
2. **Tool Plugins**: Adding functionality to tools
3. **Custom Optimizations**: Implementing specialized optimizations
4. **Debug Format Extensions**: Extending debug capabilities

## Toolchain Configuration

Configuration for the toolchain is stored in:

1. **Global Config**: System-wide configuration
2. **User Config**: User-specific settings
3. **Project Config**: Project-specific configuration
4. **Environment Variables**: Runtime configuration via environment

## Best Practices

For effective use of the COIL toolchain:

1. **Modular Organization**: Use multiple source files for larger projects
2. **Build Automation**: Automate build processes with scripts or build systems
3. **Version Control**: Track source changes using version control systems
4. **Optimization Strategy**: Use appropriate optimization levels for development and release
5. **Debug Information**: Include debug information during development

## Error Management

The toolchain provides consistent error handling:

1. **Error Codes**: Standard error codes across all tools
2. **Error Messages**: Clear, informative error messages
3. **Warning Levels**: Configurable warning levels
4. **Error Formatting**: Machine-readable error output option for IDE integration