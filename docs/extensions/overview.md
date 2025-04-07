# COIL Extensions Framework

## Overview

The COIL Extensions Framework provides a standardized mechanism for extending COIL functionality beyond the core specification. Extensions enable specialized capabilities without complicating the base language.

## Extension Categories

COIL extensions are organized into the following categories:

### 1. Core Extensions

Extensions that enhance the fundamental capabilities of COIL:

- **Multi-Dimensional Types**: Vectors, matrices, and tensors
- **Composite Types**: Structures, unions, and complex data types
- **ABI**: Application Binary Interface specifications
- **Preprocessor**: Conditional compilation and compile-time features

### 2. Processing Unit Extensions

Extensions specific to particular processing units:

- **CPU Extensions**: Traditional CPU-specific instructions
- **GPU Extensions**: Graphics and parallel processing capabilities
- **NPU Extensions**: Neural network accelerator instructions

### 3. Architecture Extensions

#### 3.1 CPU

Extensions targeting specific CPU hardware architectures:

- **x86/x64**: Intel/AMD CPU architecture extensions
- **ARM**: ARM CPU architecture extensions
- **RISC-V**: RISC-V CPU architecture extensions

#### 3.2 GPU

Extensions targeting specific GPU hardware architectures:

- **PTX**: Instruction Set for Nvidia GPUs 
- **CGN**: Instruction Set for AMD GPUs

### 4. Functional Extensions

Extensions that add domain-specific functionality:

- **Cryptography**: Cryptographic algorithms and operations
- **Graphics**: Advanced graphics processing operations
- **Networking**: Network protocol operations

## Extension Identification

Each extension is uniquely identified by:

1. **Extension ID**: A unique numeric identifier
2. **Extension Name**: A human-readable name (e.g., "MultiDim")
3. **Extension Version**: Major.minor.patch version number

## Integration Mechanism

Extensions are integrated into COIL through several mechanisms:

### Instruction Space Allocation

- **Extended Operations (0xA0-0xDF)**: For processing unit and architecture extensions
- **Compiler Operations (0xF0-0xFE)**: For preprocessor and compile-time extensions
- **Vendor-Specific (0xFF)**: Used with extended opcode for vendor extensions

### Type System Integration

- **Type Extensions**: Specific type opcode ranges reserved for extensions
- **Type Modifiers**: Additional type modifiers defined by extensions

## Compatibility Rules

Extensions follow strict compatibility rules:

1. **Fallback Behavior**: Code using extensions must gracefully degrade if extension not available
2. **Explicit Declaration**: Extensions must be explicitly declared in the object file
3. **Version Checking**: Runtime checking for extension compatibility
4. **Core Independence**: Core COIL functionality must work without extensions

## Standard Extensions

### Multi-Dimensional Types Extension

Provides support for vectors, matrices, and tensors:

- **Extension ID**: 0x01
- **Type Opcodes**: 0x33-0x38 (CVEC, MAT, SQMAT, etc.)
- **Instruction Opcodes**: 0x82-0x8E (DOT, CROSS, NORM, etc.)

### Composite Types Extension

Extends the type system with complex data structures:

- **Extension ID**: 0x02
- **Type Opcodes**: 0xA0-0xA3 (STRUCT, PACK, UNION, ALIAS)

### ABI Extension

Defines standardized Application Binary Interfaces:

- **Extension ID**: 0x03
- **Features**: Calling conventions, register allocation, exception handling

### Preprocessor Extension

Provides compile-time code manipulation:

- **Extension ID**: 0x04
- **Instruction Opcodes**: 0xF0-0xF9

## Extension Registry

The COIL Extension Registry maintains the official list of extensions:

| ID   | Name     | Version | Category | Description |
|------|----------|---------|----------|-------------|
| 0x01 | MultiDim | 1.0.0   | Core     | Multi-dimensional types and operations |
| 0x02 | Composite| 1.0.0   | Core     | Composite data types |
| 0x03 | ABI      | 1.0.0   | Core     | Application Binary Interface definitions |
| 0x04 | Preproc  | 1.0.0   | Core     | Preprocessor functionality |
| 0x10 | CPU.x86  | 1.0.0   | PU       | x86/x64 CPU extensions |
| 0x11 | CPU.ARM  | 1.0.0   | PU       | ARM CPU extensions |
| 0x20 | GPU.Core | 1.0.0   | PU       | Core GPU functionality |

## Implementation Requirements

A compliant COIL implementation must:

1. **Core Support**: Fully implement the core COIL specification
2. **Extension Detection**: Provide a mechanism to detect available extensions
3. **Graceful Degradation**: Handle missing extensions appropriately
4. **Extension Versioning**: Check extension version compatibility
5. **Documentation**: Clearly document supported extensions