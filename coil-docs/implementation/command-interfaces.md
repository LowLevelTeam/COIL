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

## Environment Variables

The toolchain recognizes these standard environment variables:

| Variable | Purpose |
|----------|---------|
| `COIL_INCLUDE_PATH` | CASM include directories (colon-separated) |
| `COIL_TARGET` | Default target triple |
| `COIL_DEBUG_LEVEL` | Default debug level |
| `COIL_OPTIMIZATION` | Default optimization level |
| `COIL_PROCESSOR_TYPE` | Default processor type |
| `COIL_FEATURES` | Default enabled features (comma-separated) |
| `COIL_OPTIONS` | Default additional options |

Example usage:
```bash
export COIL_INCLUDE_PATH=/usr/local/coil/include:/home/user/coil/include
export COIL_TARGET=cpu-x86-64
export COIL_OPTIMIZATION=2
casm main.casm -o main.coil  # Uses environment variables for defaults
```

## Configuration Files

Configuration files can be used to store common options:

```
# .coilconfig
include_path = /usr/local/coil/include:/home/user/coil/include
target = cpu-x86-64
optimization = 2
debug_level = full
features = avx2,fma
```

Configuration files are loaded from:
1. Current directory (`.coilconfig`)
2. User's home directory (`~/.coilconfig`)
3. System-wide configuration (`/etc/coil/config`)

## Return Codes

All COIL tools use standardized return codes:

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | General error |
| 2 | Command-line error |
| 3 | Input file error |
| 4 | Output file error |
| 5 | Processing error |
| 6 | Resource error |

## Batch Processing

For batch processing, tools support:

```
casm -b batch_file
```

Where batch_file contains lines like:
```
input1.casm -o output1.coil -O2
input2.casm -o output2.coil -g
```

## Integration Examples

### Basic Compilation

```bash
# Compile CASM to COIL object
casm source.casm -o source.coil

# Process COIL object to target-specific output
coilp source.coil -o source.coilo
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

## Extending Commands

Tools can be extended with plugins:

```bash
# Load extension
casm --load-extension=my_extension.so source.casm -o source.coil
```

Extensions can add:
- New command-line options
- Custom processing steps
- File format extensions
- Custom optimizations