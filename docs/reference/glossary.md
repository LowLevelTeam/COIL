# COIL Glossary

## Purpose

This document defines key terminology used throughout the COIL specification to ensure consistent understanding and usage of technical terms.

## Core Concepts

### COIL
Computer Oriented Intermediate Language - A universal binary instruction format designed for maximum portability across processing architectures while enabling native performance.

### COIL-ASM
The human-readable text representation of COIL. COIL-ASM provides a syntax for writing and debugging COIL code, but the binary format is considered the definitive specification.

### COIL Processor
A software implementation that processes COIL code. This could be a compiler, interpreter, JIT compiler, or hardware implementation. The term "COIL processor" refers to the software that processes COIL, not the hardware processor it targets.

### Processing Unit
A physical computational device that executes instructions. Examples include CPU, GPU, TPU, etc. In COIL v1, the focus is primarily on CPUs.

### Type-Determined Instruction
A key concept in COIL where an instruction's behavior is determined by the types of its operands, rather than having specific opcodes for each variant.

## File Formats

### COIL Object (.coil)
A binary file containing COIL instructions, symbols, and metadata.

### COIL Output Object (.coilo)
A processed output format containing architecture-specific binary code and linking information.

### COIL Header (.coilh)
A file containing COIL declarations for inclusion in other files.

### COIL Archive (.coila)
A library file containing multiple COIL objects.

## Processor and Architecture Terminology

### Processor Type
The broad category of processing unit (CPU, GPU, etc.) specified using the `PROC` directive.

### Architecture
The fundamental instruction set and register model of a processing unit, without regard to specific implementation details. Examples include x86, ARM, RISC-V.

### Mode
A specific operating mode of an architecture that significantly changes the available features, register widths, or behavior. Examples include x86-64 (long mode), ARM AArch64, RISC-V RV64.

## COIL System Components

### ABI (Application Binary Interface)
A set of conventions for function calls, parameter passing, and register usage. COIL's ABI system automates parameter passing and return value handling.

### Directive
An instruction that controls the compilation process rather than generating runtime code.

### Scope
A lexical block that defines the lifetime of variables, created with `SCOPEE` and destroyed with `SCOPEL`.

### Variable
An abstraction over registers and memory that allows for architecture-independent code. Variables are declared with types and automatically allocated by the COIL processor.

### Symbol
A named entity in COIL code, such as a function or global variable.

### Type
A classification that specifies how to interpret the memory representation of a value.

### Operand
A component of an instruction that specifies the data to operate on.

## Memory Concepts

### Section
A discrete portion of a COIL object file containing code, data, or metadata.

### Relocation
An entry in a COIL object that specifies how addresses should be adjusted during linking.

### Symbol Table
A collection of symbol definitions and references in a COIL object.

### Memory Model
The rules governing memory access, ordering, and visibility in COIL programs.

### Stack
A region of memory used for temporary storage and function call management.

### Heap
A region of memory used for dynamic allocation (optional in COIL).

## Type System Terminology

### Main Type
The primary type category, represented in the first 8 bits of the 16-bit type field.

### Type Extension
Additional qualifiers for a type, represented in the second 8 bits of the 16-bit type field.

### Type Data
Additional information beyond the 16-bit type field that specifies aspects like register identifiers or array element types.

### Platform-Dependent Type
A type that automatically adjusts its size based on the target platform (e.g., `TYPE_INT`, `TYPE_PTR`).

### Fixed-Width Type
A type with a specific size regardless of platform (e.g., `TYPE_INT32`, `TYPE_UNT64`).

### Composite Type
A type composed of other types, such as structures and arrays.

## Instruction Terminology

### Opcode
The operation code that identifies what instruction to perform.

### Operand Count
The number of operands that follow an instruction opcode.

### Universal Instruction
Instructions available on all processor types (opcodes 0x00-0xBF).

### Processor-Specific Instruction
Instructions specific to certain processor types (opcodes 0xC0-0xFE).

### Conditional Execution
Executing an instruction only if a specific condition is met.

## Common Abbreviations

### ISA (Instruction Set Architecture)
The complete set of instructions, registers, and behaviors defined by an architecture.

### SIMD (Single Instruction, Multiple Data)
A computational approach where a single instruction operates on multiple data points simultaneously.

### JIT (Just-In-Time Compilation)
A technique where COIL code is compiled to native code during execution rather than in advance.

### AOT (Ahead-Of-Time Compilation)
A technique where COIL code is compiled completely to native code before execution.

### Basic Instruction Abbreviations
- **BR**: Branch
- **CMP**: Compare
- **MOV**: Move
- **ADD/SUB/MUL/DIV**: Basic arithmetic operations
- **SYM**: Symbol
- **VAR**: Variable

## Versioning Terms

### Major Version
A version increment that indicates incompatible changes (e.g., v1 to v2).

### Minor Version
A version increment that indicates backward-compatible additions (e.g., v1.0 to v1.1).

### Patch Version
A version increment that indicates clarifications or bug fixes (e.g., v1.0.0 to v1.0.1).

## Debug and Development Terms

### Debug Information
Optional metadata in a COIL object that maps between COIL instructions and original source code.

### Toolchain
The collection of tools used to develop COIL programs (assembler, disassembler, linker, etc.).

### Assembler (coilasm)
A tool that converts COIL-ASM text to COIL binary format.

### Disassembler (coildis)
A tool that converts COIL binary to COIL-ASM text.

### Linker (coillink)
A tool that combines multiple COIL objects into a single executable.

### Processor (coilproc)
A tool that translates COIL to native code for a specific target.

## Low Level Team

### Low Level Team (LLT)
A group of anonymous developers who contribute to creating a new framework under a single specification, responsible for developing the COIL specification and core implementations.

### COIL Projects
The collection of tools and libraries developed by the Low Level Team to support COIL:
- COIL Assembler - Parse .casm into .coil
- COIL VM - Interpret .coil
- COIL Processor - Parse .coil into .coilo
- COIL Linker - Link .coilo into executables
- COIL Library - COIL standard library