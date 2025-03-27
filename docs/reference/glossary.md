# COIL Glossary

## Purpose

This document defines key terminology used throughout the COIL specification to ensure consistent understanding of technical terms.

## Core Concepts

### COIL
Computer Oriented Intermediate Language - A universal binary instruction format designed for maximum portability while enabling native performance across diverse processing architectures.

### CASM
COIL Assembly - The human-readable text representation of COIL binary code. While CASM provides a syntax for writing COIL programs, the binary format is considered the authoritative specification.

### COIL Processor
Software or hardware that processes COIL instructions. This could be an interpreter, JIT compiler, AOT compiler, or hardware implementation.

### Processing Unit
The physical computational device that executes instructions, such as a CPU, GPU, or TPU. In COIL v1, the focus is primarily on CPUs.

### Type-Determined Instruction
A key concept in COIL where an instruction's behavior is determined by the types of its operands rather than having separate opcodes for different types.

## File Formats

### COIL Object (.coil)
A binary file containing COIL instructions, symbols, and metadata in the COIL object format.

### COIL Output Object (.coilo)
A processed format containing architecture-specific binary code generated from COIL.

### COIL Header (.coilh)
A file containing COIL declarations for inclusion in other files.

### COIL Archive (.coila)
A library file containing multiple COIL objects.

## System Components

### ABI (Application Binary Interface)
A set of conventions for function calls, parameter passing, register usage, and stack management. COIL's ABI system automates these details.

### Directive
An instruction that controls the assembly process rather than generating runtime code.

### Scope
A lexical block that defines the lifetime of variables, created with `SCOPEE` and destroyed with `SCOPEL`.

### Variable
An abstraction over registers and memory that allows architecture-independent code. Variables are automatically allocated to registers or memory by the COIL processor.

### Symbol
A named entity in COIL code that serves as a reference point, such as a function or label.

### Type
A classification that specifies how to interpret the binary representation of a value.

### Operand
A component of an instruction that specifies the data to operate on or the location to store results.

## Memory Concepts

### Section
A discrete portion of a COIL object file with specific attributes, containing code, data, or metadata.

### Relocation
Information in a COIL object that specifies how addresses should be adjusted during linking.

### Symbol Table
A collection of symbol definitions and references in a COIL object.

### Memory Model
The rules governing memory access, ordering, and visibility in COIL programs.

### Stack
A region of memory used for temporary storage, local variables, and function call management.

## Type System Terminology

### Main Type
The primary type category, represented in the first 8 bits of the 16-bit type field.

### Type Extension
Additional qualifiers for a type, represented in the second 8 bits of the 16-bit type field.

### Platform-Dependent Type
A type that automatically adjusts its size based on the target platform (e.g., `TYPE_INT`, `TYPE_PTR`).

### Fixed-Width Type
A type with a specific size regardless of platform (e.g., `TYPE_INT32`, `TYPE_UNT64`).

### Composite Type
A type composed of other types, such as structures and arrays.

## Instruction Terminology

### Opcode
The operation code that identifies which instruction to perform (first byte of an instruction).

### Operand Count
The number of operands that follow an instruction opcode (second byte of an instruction).

### Universal Instruction
Instructions available on all processor types (opcodes 0x00-0xBF).

### Processor-Specific Instruction
Instructions specific to certain processor types (opcodes 0xC0-0xFE).

### Conditional Execution
Executing an instruction only if a specific condition is met, controlled by `TYPE_PARAM5`.

## Common Abbreviations

### ISA (Instruction Set Architecture)
The specification of instructions, registers, and behaviors defined by an architecture.

### SIMD (Single Instruction, Multiple Data)
A parallel computing approach where a single instruction operates on multiple data elements simultaneously. In COIL, represented by vector operations.

### JIT (Just-In-Time Compilation)
A technique where COIL code is compiled to native code during execution.

### AOT (Ahead-Of-Time Compilation)
A technique where COIL code is compiled to native code before execution.

## Toolchain Terms

### Assembler (coilasm)
A tool that converts CASM text to COIL binary format.

### Disassembler (coildis)
A tool that converts COIL binary to CASM text.

### Linker (coillink)
A tool that combines multiple COIL objects into a single executable COIL object.

### Processor (coilproc)
A tool that executes COIL code, either through interpretation or compilation to native code.

### Debugger (coildbg)
A tool for debugging COIL programs, supporting breakpoints, variable inspection, and step-by-step execution.

## Related Documentation

- [Core Overview](../spec/core/overview.md) - General introduction to COIL
- [Binary Format](../spec/binary-format.md) - Details on the binary representation
- [Type System](../spec/systems/type-system.md) - Complete type system details
- [Instruction Reference](instruction-ref.md) - Complete instruction listing