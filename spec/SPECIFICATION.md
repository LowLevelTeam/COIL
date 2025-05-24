# COIL Specifications

COIL specifications help with develoment to control what is implemented and what is left to be implemented.

TODO: Each section should be expanded with more specifics.

## COIL Specifications

#### COIL.1
Support for Integers and Basic Instructions

#### COIL.2
Support for floating points and advanced arithmetic instructions

#### COIL.3
Support for vectors and vector specific instructions.

#### COIL.4
Processing Unit specific instructions

#### COIL.5
Basic Directives

#### COIL.6
Advanced Directives

#### COIL.7
Structures / Compound Types

#### COIL.8
Complex Types

## Output/Native Specifications

### CBC-CPU
COIL Byte Code Central Processing Unit Specification

A COIL Byte Code processor can then specify its current capability in development using this specification.

In practice a CBC-CPU Processor could specify for each architecture a level of support for compiling using the instruction set.
i.e. CBC-CPU.x86.1, CBC-CPU.x86_32.2, CBC-CPU.x86_64.4, CBC-CPU.armthumb.1 CBC-CPU.arm32.1 CBC-CPU.arm64.1 CBC-CPU.riscv32.0 CBC-CPU.riscv64.0 CBC-CPU.riscv128.0
An easy way for contributors to know what needs work to become functional.

#### CBC-CPU.1
Support for Integers and Basic Instructions

#### CBC-CPU.2
Support for more sophisticated compound instructions.

#### CBC-CPU.3
Support for floating points and advanced arithmetic instructions

#### CBC-CPU.4
Support for vectors and vector specific instructions.

#### CBC-CPU.5
Support for processing unit and architecture specific instructions.

### CBC-GPU
COIL Byte Code Graphics Processing Unit Specification

**Working on CBC-CPU first.**

## Linker Specifications

### FMT.1
Support for COIL Objects, Binary Format (No Format)

### FMT.2
Support for major formats (ELF64, MACH-O, PE)

### FMT.3
Support for previously major formats and backwards compatible formats (ELF32, MACH-O, PE+)

### FMT.4
Support for all other formats (MZ, NE, COFF, etc...)


