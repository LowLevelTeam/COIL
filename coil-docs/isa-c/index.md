# Compiler Instruction Set Architecture

## Overview

The Compiler Instruction Set Architecture (ISA-C) provides directives that control the compilation and configuration of COIL programs. These directives enable conditional compilation, code organization, ABI definitions, and compile-time configuration.

## Core Directives

ISA-C directives use opcodes in the range 0xF0-0xFE and are processed during compilation:

```c
// Conditional Compilation
IF   = 0xF0  // Begin conditional block
ELIF = 0xF1  // Alternative condition
ELSE = 0xF2  // Default case
EIF  = 0xF3  // End conditional block

// Code Organization
INCL = 0xF4  // Include a file
SECT = 0xF5  // Define code section
DATA = 0xF6  // Insert raw data
PADD = 0xF7  // Pad to specified byte boundary

// Binary Interface
ABI  = 0xF9  // Define an ABI

// Compile-time Definitions
DEF  = 0xFA  // Define a compile-time value
UDEF = 0xFB  // Undefine a compile-time value

// Compilation Targets
TARGET = 0xFC // needed to utilize extended opcode. Within a TARGET scope access to the extended opcode of the selected target type is accessible. Any attempt to use the extended opcode range without defining the expected target is an error
ETARGET = 0xFD // end the target specific code block

PRAGMA = 0xFE //
```

## Purpose

ISA-C directives serve several critical functions in the COIL ecosystem:

1. **Platform Adaptation**: Enable code to adapt to different processing units and architectures
2. **Modular Development**: Support code organization and reuse
3. **Binary Interface**: Define consistent calling conventions
4. **Configuration**: Control compilation parameters and optimizations

## Directive Categories

1. **Conditional Compilation** Conditional directives enable selective compilation based on platform features, configuration settings, and compile-time definitions:
2. **Code organization**: Code organization directives control how code is structured, linked, and laid out:
3. **ABI Definition**: ABI directives define how functions interact with each other and the system:
4. **Compile-time Definitions**: Definition directives create and manage compile-time constants and flags:

## Related Components

- [Conditional Compilation](./conditional-compilation.md) - IF, ELIF, ELSE, EIF directives
- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [ABI Definition](./abi-definition.md) - ABI and related directives
- [Compile-time Definitions](./compile-time-definitions.md) - DEF, UDEF directives
- [Overview](./overview.md) - Comprehensive ISA-C overview