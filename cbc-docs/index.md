# COIL Byte Code (CBC)

## Overview

COIL Byte Code (CBC) is a compact, portable binary representation designed for efficient interpretation and just-in-time compilation of COIL programs. It serves as an intermediate format between COIL source code and machine-specific compiled code for fast JIT.

CBC has several sub formats for each processing unit. CBC serves as a way to create selected device code for a compilation target and have it be compiled at runtime (/load time) where it can be compiled for a device selected at runtime similar to the purpose of PTX for graphics card but instead of just being for nvidia GPUs this serves as a standard for all possible devices. CBC is pre optimized by the COIL processor who creates this CBC out of standard COIL code and optimizes it for the new instruction set.

CBC is its own file format with a specific purpose. A COIL JIT Compiler can be called upon from the standard library which will utilize the generated .cbc file. The COIL Processor will create both a native object binary with the code for the host system and a .cbc file for the device code.

## Purpose

CBC addresses several key needs in the COIL ecosystem:

1. **Portable Execution**: Run COIL programs on platforms without a full COIL compiler
2. **Efficient Interpretation**: Optimize for fast execution in interpreted environments
3. **JIT Compilation**: Provide a suitable representation for dynamic compilation
4. **Reduced Size**: Minimize the binary size for embedded applications
5. **Platform Independence**: Ensure consistent execution across diverse environments

## Relationship to COIL

CBC is derived from COIL but differs in several key ways:

| Aspect       | COIL                          | CBC                           |
|--------------|-------------------------------|-------------------------------|
| Purpose      | Intermediate language         | Execution format              |
| Optimization | Platform-specific             | Interpreter-focused           |
| Encoding     | Type-rich, explicit           | Compact, implicit             |
| Structure    | Object-oriented               | Stack-oriented                |
| Scope        | Complete program              | Execution units               |
