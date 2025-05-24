# COIL

## Overview

COIL (Computer Oriented Intermediate Language) is a modern toolchain for compilation, optimization, and cross-platform development.

The five stages of a coil toolcain.

```
Source Language (.c,.cpp,etc...) -> CCC (COIL C Compiler) -> COIL -> COIL Processor -> CBC -> COIL Specifier -> CBC-Specific -> COIL-Specific Processor -> Native -> Linker -> (executable,shared library,static library,object)
```

## Encoding Specifics

### Object Format

### COIL
COIL is made as a type based high level intermediate language.

Similar to a higher level LLVM IR.

### CBC
CBC is a fast optimizable intermediate language

Similar to a slightly higher level and more expansive WASM

### CBC-*
CBC-* or CBC-Specific is not made for JIT but rather an easy way to write machine code with a similar frontend regardless of target.
Built for each CPU and with extras for each Architecture it would be similar to CBC-CPU-MAJOR CBC-CPU-RISCV. CBC-CPU-MAJOR would be for x86 and ARM targets as they are really similar but because architectures like RISCV and other CPU architectures have intrinsics like RISCV doesn't have typical flags etc..

## Toolchain Specifics

The Toolchain is built to supported all three types of execution like Interpreter, JIT and AOT

AOT
```
Source Language (.c,.cpp,etc...) -> CCC (COIL C Compiler) -> COIL -> COIL Processor -> CBC -> COIL Specifier -> CBC-Specific -> COIL-Specific Processor -> Native -> Linker -> (executable,shared library,static library,object)
```

JIT
```
CBC -> CBC Specifier -> CBC-Specific -> COIL-Specific Processor -> Native
```

Interpreter
```
Source Language (.py,.java,etc...) -> CPI (COIL Python Interpreter) -> CBC -> CBC Specifier -> CBC-Specific -> COIL-Specific Processor -> Native -> Linker -> Execute
```

Complex Hetregenous Computing
```
Host Code -> AOT -> Execute -> JIT (Device CBC) -> Device Execution
Device Code -> COIL Compiler -> COIL -> COIL Processor -> CBC
```


