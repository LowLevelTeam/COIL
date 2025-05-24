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

### CBCC
COIL Byte Code Central Processing Unit Specification

A COIL Byte Code processor can then specify its current capability in development using this specification.

In practice a CBC-CPU/CBCC Processor could specify for each architecture a level of support for compiling using the instruction set.
i.e. CBCC.x86.1, CBCC.x86_32.2, CBCC.x86_64.4, CBCC.armthumb.1 CBCC.arm32.1 CBCC.arm64.1 CBCC.riscv32.0 CBCC.riscv64.0 CBCC.riscv128.0
An easy way for contributors to know what needs work to become functional.

#### CBCC.1
Support for Integers and Basic Instructions

#### CBCC.2
Support for more sophisticated compound instructions.

#### CBCC.3
Support for floating points and advanced arithmetic instructions

#### CBCC.4
Support for vectors and vector specific instructions.

#### CBCC.5
Support for processing unit and architecture specific instructions.

### CBCG
COIL Byte Code Graphics Processing Unit Specification

**Working on CBCC first.**

## Linker Specifications

### FMT.1
Support for Binary Format (No Format)

### FMT.2
Support for major formats (ELF64, MACH-O, PE)

### FMT.3
Support for previously major formats and backwards compatible formats (ELF32, MACH-O, PE+)

### FMT.4
Support for all other formats (MZ, NE, COFF, etc...)


