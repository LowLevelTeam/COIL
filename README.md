# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Specification Version 1.1.0

This documentation represents version 1.1.0 of the COIL specification, with comprehensive updates addressing implementation challenges encountered in libcoil-dev. Key improvements include:

1. **Type System Clarity**
   - Enhanced matrix and tensor memory layout specification
   - Detailed binary encoding examples for all complex types
   - Explicit nesting limits and alignment rules

2. **Binary Format Precision**
   - Clarified zero-operand instruction handling
   - Explicit operand count encoding rules
   - Comprehensive type encoding examples

3. **Object Format Robustness**
   - Detailed section merging algorithm
   - Explicit symbol resolution rules
   - Precise relocation processing steps

4. **Memory Model Refinements**
   - Explicit stack frame organization
   - Detailed memory protection mechanisms
   - Clear alignment requirements

5. **Instruction Behavior Consistency**
   - Standardized flag effect documentation
   - Complete type combination matrices for operations
   - Platform-specific behavior tables

6. **ABI System Improvements**
   - Parameter passing specifications by type and size
   - Return value handling rules
   - Register usage conventions

## Core Components

The COIL ecosystem consists of three primary components:

### 1. COIL (Computer Oriented Intermediate Language)
The universal binary instruction format that serves as the machine code for COIL-compatible systems.

### 2. CASM (COIL Assembly)
The standard human-readable text representation of COIL code.

### 3. CBC (COIL Byte Code)
A compact, portable binary representation for efficient interpretation and JIT compilation.

## Documentation Structure

The COIL documentation is organized into the following sections:

- **Core Concepts**: Type system, memory model, execution model
- **Instruction Sets**: Universal ISA, Extended ISA, Compiler ISA
- **Binary Formats**: Binary encoding, object file structure
- **Configuration**: Target specification, linking model

## For Implementers

For detailed implementation guidance, refer to the specific sections of this documentation. Each module has been enhanced with clear examples and precise specifications to ensure consistent behavior across implementations.

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.