# COIL Core Concepts

## What is COIL?

The Computer Oriented Intermediate Language (COIL) is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. COIL bridges the gap between high-level programming languages and hardware-specific machine code.

COIL serves as a binary intermediate representation with these key characteristics:
- **Binary Format Primacy**: COIL is fundamentally a binary format, with CASM (COIL Assembly) as its text representation
- **Processor Agnostic**: Not tied to any specific processor type (CPU, GPU, etc.)
- **Performance Oriented**: Designed for efficient execution while maintaining portability

## Key Features

### Type-Determined Instructions

Instructions derive behavior from operand types:
- Same instruction name works across different data types
- No need for type-specific variants (e.g., ADDI, ADDF, ADDV)
- Example: `ADD result, a, b` works for integers, floats, or vectors

Benefits:
- Compact instruction set
- Consistent behavior patterns
- Simple extensibility with new types

### Architecture Independence

Clear separation between universal and platform-specific:
- **Universal operations** (opcodes 0x00-0xBF): Work identically across platforms
- **Platform-specific operations** (opcodes 0xC0-0xFE): Target specific architectures
- **Implementation extensions** (opcode 0xFF): For specialized capabilities

### Variable System

Abstract management of registers and memory:
- Variables declared with types and numeric IDs
- COIL processor determines optimal storage
- Register allocation handled automatically
- Memory management through scope system

### ABI System

Automated function call conventions:
- Standard ways to pass parameters
- Consistent return value handling
- Platform-specific optimizations under a universal API
- Cross-platform compatibility

## COIL Processing Model

A COIL processor:
1. Reads COIL binary instructions
2. Validates instruction and operand types
3. Dispatches to appropriate implementation based on types
4. Executes the operation using native capabilities where available
5. Manages resources according to the variable and ABI systems

## Relationships to Other Components

### CASM Assembly Language

CASM is the text representation of COIL:
- Human-readable syntax for writing COIL programs
- Direct correlation to underlying binary format
- Additional assembly-time conveniences

Relationship:
```
Source code → CASM → COIL binary → Execution
```

### COIL Binary Formats

COIL uses two primary formats:

1. **COIL Object Format (.coil)**:
   - Intermediate format for linkage
   - Contains relocations and symbols
   - Used during compilation

2. **COIL Output Object Format (.coilo)**:
   - Final format after linking
   - Contains resolved symbols and relocations
   - Ready for execution

## Versioning Roadmap

COIL evolves through distinct versions:

1. **Version 1** (Current): 
   - Core instruction set, types, variables, and ABI
   - Single-device (CPU) focus
   - Binary formats and basic tools

2. **Version 2** (Planned):
   - Standard library of common functions
   - System interfaces
   - Optimization framework
   - Full backward compatibility with v1

3. **Version 3** (Future):
   - Multi-device support (GPU, TPU, etc.)
   - Cross-device memory management
   - Heterogeneous computing model
   - JIT compilation

## Implementation Components

A complete COIL ecosystem consists of:
- CASM Assembler: Converts CASM to COIL binary
- COIL Processor: Executes COIL binary code
- COIL Linker: Combines COIL objects into executables
- Development Tools: Debuggers, profilers, etc.