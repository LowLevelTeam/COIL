# Command Interfaces

## Overview

The COIL toolchain provides standardized command-line interfaces for each component. These interfaces follow consistent patterns to ensure usability and interoperability.

## Common Conventions

All COIL toolchain commands follow these conventions:

- Options begin with a single dash (`-`) for short options or double dash (`--`) for long options
- Input files are specified without option flags
- Output files are specified with `-o` or `--output`
- Multiple input files can be specified for linkers and some processors
- Help is available via `-h` or `--help`
- Version information is available via `-v` or `--version`

## CASM Assembler Interface

Assembles CASM source code into COIL object files:

```
casm [options] input.casm -o output.coil
```

Standard options:
- `-I <path>` - Include directory for header files
- `-D <symbol>[=value]` - Define symbol
- `-O <level>` - Optimization level (0-3)
- `-g` - Include debug information
- `-W <warning>` - Enable specific warning
- `-t <target>` - Target triple (device-arch-mode)

Example usage:
```
casm -I./include -O2 -g -t cpu-x86-64 main.casm -o main.coil
```

## COIL Processor Interface

Processes COIL object files into architecture-specific output objects:

```
coilp [options] input.coil -o output.coilo
```

Standard options:
- `-O <level>` - Optimization level (0-3)
- `-g` - Include debug information
- `-t <target>` - Target triple (device-arch-mode)
- `-f <feature>` - Enable specific feature
- `-S` - Output assembly instead of binary
- `-j` - Generate JIT-friendly output

Example usage:
```
coilp -O2 -g -t cpu-x86-64 -f avx2 main.coil -o main.coilo
```

## CBC Compiler Interface

Compiles COIL objects to CBC bytecode:

```
coil2cbc [options] input.coil -o output.cbc
```

Standard options:
- `-O <level>` - Optimization level (0-3)
- `-g` - Include debug information
- `-c` - Optimize for constant pool efficiency
- `-r` - Enable register allocation hints
- `-t <target>` - Target environment

Example usage:
```
coil2cbc -O2 -g -c main.coil -o main.cbc
```

## CBC Interpreter Interface

Interprets CBC bytecode:

```
cbcrun [options] program.cbc [program arguments]
```

Standard options:
- `-v` - Verbose output
- `-t <trace>` - Enable tracing
- `-m <memory>` - Memory limit
- `-j` - Enable JIT compilation

Example usage:
```
cbcrun -v -m 1G program.cbc arg1 arg2
```

## COIL Validator Interface

Validates COIL binary formats:

```
coilvalidate [options] input_file
```

Standard options:
- `-l <level>` - Validation level (basic, thorough, strict)
- `-e` - Stop on first error
- `-s` - Show statistics
- `-f <format>` - Specify input format

Example usage:
```
coilvalidate -l thorough -s main.coil
```

## COIL Disassembler Interface

Disassembles COIL binary files to CASM source:

```
coildis [options] input_file -o output.casm
```

Standard options:
- `-a` - Show addresses
- `-r` - Show raw bytes
- `-s` - Include symbol information
- `-c` - Include comments
- `-f <format>` - Specify input format

Example usage:
```
coildis -a -s -c main.coil -o main.disasm.casm
```

## Integration Examples

### Basic Compilation

```bash
# Compile CASM to COIL object
casm source.casm -o source.coil

# Process COIL object to target-specific output
coilp source.coil -o source.coilo

# Link with OS-specific linker
ld source.coilo -o executable
```

### Multi-Target Compilation

```bash
# Compile for multiple targets
coilp source.coil -t cpu-x86-64,gpu-cuda-sm75 -o source.coilo
```

### Debug Build

```bash
# Compile with debugging information
casm -g source.casm -o source.coil
coilp -g source.coil -o source.coilo
```

### CBC Generation and Execution

```bash
# Generate CBC bytecode
coil2cbc source.coil -o source.cbc

# Execute CBC bytecode
cbcrun source.cbc arg1 arg2
```

## Related Components

- [Toolchain Components](/coil-docs/implementation/toolchain-components.md) - Description of toolchain components
- [File Formats](/coil-docs/implementation/file-formats.md) - File format specifications
- [Compilation Profiles](/coil-docs/implementation/compilation-profiles.md) - Standard compilation settings