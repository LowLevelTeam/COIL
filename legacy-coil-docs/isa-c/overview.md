# Compiler Instruction Set Architecture

## Overview

The Compiler Instruction Set Architecture (ISA-C) provides directives that control the compilation process of COIL programs. These directives enable conditional compilation, code organization, ABI definition, and compile-time configuration, allowing COIL code to adapt to different platforms while maintaining a consistent binary interface.

## Purpose

ISA-C directives serve several critical functions in the COIL ecosystem:

1. **Conditional Compilation**: Enable platform-specific code paths based on target characteristics
2. **Code Organization**: Control how code is organized into sections and modules
3. **Binary Interface**: Define Application Binary Interfaces (ABIs) for function calls
4. **Configuration**: Set compile-time constants and behaviors
5. **Platform Adaptation**: Provide mechanisms for adapting to different execution environments

## Instruction Categories

Compiler directives use opcodes in the range 0xF0-0xFE:

| Opcode Range | Category | Purpose |
|--------------|----------|---------|
| 0xF0-0xF3    | Conditional Compilation | Control which code gets compiled |
| 0xF4-0xF7    | Code Organization | Control code layout and inclusion |
| 0xF8-0xF9    | Binary Interface | Define ABIs and calling conventions |
| 0xFA-0xFB    | Compile-time Definitions | Define and manage compile-time constants |
| 0xFC-0xFE    | Reserved | Reserved for future extensions |

## Directive Processing

Compiler directives are processed during the compilation phase:

1. **Parse Phase**: Directives are recognized and interpreted
2. **Conditional Phase**: Code sections are included or excluded based on conditions
3. **Organization Phase**: Code is organized into sections
4. **Symbol Resolution**: Symbols and definitions are resolved
5. **Binary Generation**: Final binary is generated with directives applied

## Execution Context

Unlike regular instructions, compiler directives do not execute at runtime:

- They are processed by the COIL compiler during compilation
- They affect which code is included in the final binary
- They control how code is organized and linked
- They have no direct runtime impact after compilation

## Related Components

- [Conditional Compilation](./conditional-compilation.md) - IF, ELIF, ELSE, EIF directives
- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [ABI Definition](./abi-definition.md) - ABI and related directives
- [Compile-time Definitions](./compile-time-definitions.md) - DEF, UDEF directives
- [Universal ISA](../isa-u/overview.md) - Runtime instruction set
- [Configuration Format](../core/config-format.md) - Target configuration specification