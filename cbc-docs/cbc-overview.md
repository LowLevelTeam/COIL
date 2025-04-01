# COIL Byte Code (CBC) Overview

## Purpose

COIL Byte Code (CBC) is a lightweight, efficient binary format derived from COIL, specifically designed for runtime execution environments such as interpreters and JIT compilers. CBC addresses the need for fast interpretation and JIT compilation in scenarios where ahead-of-time compilation to native code is impractical or undesirable.

CBC serves as:

1. A runtime execution format for dynamic languages built on COIL
2. An intermediate representation for JIT compilers
3. A portable execution format for constrained environments
4. A lightweight alternative to full COIL for interpreted contexts

## Relationship to COIL

CBC differs from full COIL in several key ways:

| Aspect | Full COIL | CBC |
|--------|-----------|-----|
| Primary Goal | Portable native performance | Fast interpretation/JIT |
| Instruction Model | Type-determined | Type-specific opcodes |
| Complexity | Full feature set | Streamlined for execution |
| Target | Multiple compilation targets | Runtime environment |
| Usage Context | System programming | Dynamic language backends |

CBC retains COIL's variable system while simplifying the instruction encoding for faster decoding and execution.

## Design Principles

CBC is designed with these core principles:

1. **Fast Decoding**: Instruction formats designed for quick parsing and dispatch
2. **Compact Representation**: Efficient encoding minimizes memory footprint
3. **Type-Specific Instructions**: Explicit type encoding for faster dispatch
4. **Variable System Compatibility**: Maintains COIL's variable abstraction
5. **Streamlined Operation Set**: Focuses on common operations needed for interpreters

## Standard Workflows

CBC can be generated and used in several workflows:

1. **Direct Interpretation**: `.casm` → `.coil` → `.cbc` → CBC Interpreter
2. **JIT Compilation**: `.casm` → `.coil` → `.cbc` → JIT Compiler → Native Execution
3. **Embedded Mode**: `.cbc` sections within `.coilo` files for mixed execution models

## Key Components

The CBC system consists of:

1. **CBC Format**: Binary encoding of instructions and data
2. **CBC Compiler**: Converts COIL objects to CBC format
3. **CBC Interpreter**: Executes CBC instructions directly
4. **CBC JIT**: Compiles hot CBC code paths to native code

Together, these components provide a complete execution environment for COIL-based programs requiring interpretation or dynamic compilation.