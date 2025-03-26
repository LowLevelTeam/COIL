# COIL Glossary

## Purpose

This document defines key terminology used throughout the COIL specification to ensure consistent understanding and usage of technical terms.

## Core Concepts

### COIL (Computer Oriented Intermediate Language)
A universal binary instruction format designed for maximum portability across processing architectures while enabling native performance. COIL serves as an intermediate representation between high-level languages and hardware-specific machine code.

### COIL-ASM
The human-readable text representation of COIL. COIL-ASM provides a syntax for writing and debugging COIL code, but the binary format is considered the definitive specification.

### COIL Processor
A software implementation that processes COIL code. This could be a compiler, interpreter, JIT compiler, or hardware implementation. The term "COIL processor" refers to the software that processes COIL, not the hardware processor it targets.

### Processing Unit
A physical computational device that executes instructions. Examples include CPU, GPU, TPU, etc. In COIL v1, the focus is primarily on CPUs.

### COIL Object
A binary file containing COIL instructions, symbols, and metadata, with the file extension `.coil`.

### COIL Output Object
A processed output format containing architecture-specific binary code and linking information, with the file extension `.coilo`.

### Type-Determined Instruction
A key concept in COIL where an instruction's behavior is determined by the types of its operands, rather than having specific opcodes for each variant.

## Processor and Architecture Terminology

### Processor Type
The broad category of processing unit (CPU, GPU, etc.) specified using the `PROC` directive.

### Architecture
The fundamental instruction set and register model of a processing unit, without regard to specific implementation details. Examples include x86, ARM, RISC-V.

### Mode
A specific operating mode of an architecture that significantly changes the available features, register widths, or behavior. Examples include x86-64 (long mode), ARM AArch64, RISC-V RV64.

### Processor
A specific implementation of an architecture by a manufacturer. Examples include Intel Core i7 (x86), Apple M1 (ARM), etc.

### Microarchitecture
The implementation details of a processor, such as pipeline design, cache structure, and branch prediction. Examples include Intel Skylake, ARM Cortex-A76.

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

### Memory Model
The rules governing memory access, ordering, and visibility in COIL programs.

## File Formats

### Section
A discrete portion of a COIL object file containing code, data, or metadata.

### Relocation
An entry in a COIL object that specifies how addresses should be adjusted during linking.

### Symbol Table
A collection of symbol definitions and references in a COIL object.

### Debug Information
Optional metadata in a COIL object that maps between COIL instructions and original source code.

## Common Abbreviations

### ISA (Instruction Set Architecture)
The complete set of instructions, registers, and behaviors defined by an architecture.

### SIMD (Single Instruction, Multiple Data)
A computational approach where a single instruction operates on multiple data points simultaneously.

### JIT (Just-In-Time Compilation)
A technique where COIL code is compiled to native code during execution rather than in advance.

### AOT (Ahead-Of-Time Compilation)
A technique where COIL code is compiled completely to native code before execution.

### BR (Branch)
An instruction that changes the flow of execution.

### CMP (Compare)
An instruction that compares two values and sets flags accordingly.

### MOV (Move)
An instruction that copies data between locations.

### ADD, SUB, MUL, DIV
Basic arithmetic operations (addition, subtraction, multiplication, division).

### SYM (Symbol)
An instruction that defines a named entity in COIL.

### VAR (Variable)
An instruction that declares a variable in COIL.

## Version-Specific Terminology

### Universal Operations
Instructions available on all processor types (opcodes 0x00-0x2F, 0x50-0xBF).

### Processor-Specific Operations
Instructions specific to certain processor types (opcodes 0xC0-0xFE).

### Reserved Instructions
Opcodes 0x30-0x4F, reserved for multi-device operations in COIL v3.

### Multi-Device Operations
Instructions for coordinating multiple processing units (planned for COIL v3).

## Memory and Register Terms

### Register
A processor-specific storage location for data. In COIL, direct register references use the `TYPE_RGP`, `TYPE_RFP`, and similar types.

### Memory Reference
A pointer to data in memory, typically represented with square brackets in COIL-ASM: `[address]`.

### Stack
A region of memory used for temporary storage and function call management.

### Heap
A region of memory used for dynamic allocation (optional in COIL).

### Variable Promotion
Moving a variable from memory to a register for improved performance.

### Variable Demotion
Moving a variable from a register to memory to free register space.

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

## Control Flow Terminology

### Branch
A change in the flow of execution to a different part of the program.

### Conditional Execution
Executing an instruction only if a specific condition is met.

### Function Call
Transferring control to a named function with parameter passing.

### Return
Transferring control back to the caller of a function, possibly with return values.

## Common Terms for Binary Format

### Opcode
The operation code that identifies what instruction to perform.

### Operand Count
The number of operands that follow an instruction opcode.

### Encoding
The binary representation of COIL instructions.

### Object File
A binary file containing COIL code and data in a standardized format.

### Linking
The process of combining multiple COIL objects into a single executable.

## Versioning Terms

### Major Version
A version increment that indicates incompatible changes (e.g., v1 to v2).

### Minor Version
A version increment that indicates backward-compatible additions (e.g., v1.0 to v1.1).

### Patch Version
A version increment that indicates clarifications or bug fixes (e.g., v1.0.0 to v1.0.1).