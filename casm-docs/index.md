# COIL Assembly Language (CASM)

## Overview

CASM (COIL Assembly) is a human-readable text representation of COIL code. It provides a syntax for writing COIL programs directly, serving as both a low-level programming language and an assembly language for COIL.

## Purpose

CASM addresses several important needs:

1. **Human-Writable COIL**: Allows direct programming in COIL through a readable syntax
2. **Learning Tool**: Provides a way to understand COIL concepts hands-on
3. **Implementation Reference**: Serves as a reference for COIL processor implementations
4. **Debugging Aid**: Facilitates inspection and debugging of COIL programs

## Basic Syntax

CASM uses a familiar assembly-like syntax with instructions, operands, and directives.

### Instructions

Instructions follow this pattern:
```
MNEMONIC [operand1[, operand2[, ...]]]
```

Examples:
```
MOV eax, 42        ; Move immediate value to register
ADD count, 1       ; Add 1 to variable
CALL my_function   ; Call a function
```

### Variables

Variables are declared with the VAR instruction:
```
VAR identifier, type[, initial_value]
```

Examples:
```
VAR count, INT32, 0      ; 32-bit integer initialized to 0
VAR name, STRING, "COIL" ; String variable
VAR vector, V128         ; 128-bit vector
```

### Labels and Symbols

Labels define code locations:
```
label_name:
    ; Instructions
```

Symbol definitions create named functions:
```
SYM function_name
    ; Function body
```

### Directives

Directives control the assembler:
```
.SECTION code       ; Define a code section
.GLOBAL main        ; Export symbol globally
.INCLUDE "file.h"   ; Include another file
```

## Scoping

CASM uses explicit scope declarations:
```
SCOPE              ; Begin a scope block
    VAR x, INT32   ; Local variable
    ; More code
SCOPL              ; End scope block
```

## Control Flow

CASM provides traditional control flow structures:

### Branches
```
    CMP eax, 0
    BR.EQ zero_case    ; Branch if equal
    ; Non-zero case
zero_case:
    ; Zero case
```

### Function Calls
```
    ; Call function
    CALL my_function, arg1, arg2
    
    ; Define function
    SYM my_function
        ; Function body
        RET
```

### Conditional Execution
```
    TEST flags, FLAG_ENABLED
    CALL.NZ enabled_function    ; Call if flag is set
```

## Type Declarations

Types can be declared explicitly:
```
VAR int_var, INT32            ; 32-bit integer
VAR float_var, FP64           ; 64-bit float
VAR vec_var, V128             ; 128-bit vector
VAR custom_int, CINT, 24      ; 24-bit custom integer
```

## Memory Access

CASM provides various memory addressing modes:
```
MOV eax, [ptr]               ; Indirect addressing
MOV ebx, [base + offset]     ; Base plus offset
MOV ecx, array[index]        ; Array indexing
```

## Preprocessor

CASM includes preprocessor directives:
```
#define BUFFER_SIZE 1024     ; Define constant
#ifdef DEBUG                 ; Conditional compilation
    ; Debug code
#endif
```

## Example Program

```
; Hello World in CASM
.SECTION code
.GLOBAL main

SYM main
    SCOPE
        ; Print "Hello, World!"
        VAR message, STRING, "Hello, World!"
        CALL print_string, message
        
        ; Return 0
        MOV eax, 0
        RET
    SCOPL
    
SYM print_string
    SCOPE
        ; Function implementation
        ; ...
        RET
    SCOPL
```

## Related Components

- [CASM Basics](./basics/casm-basics.md) - Detailed syntax and semantics
- [CASM Directives](./features/directives.md) - Assembler directives reference
- [CASM Scoping](./features/scopes.md) - Variable scoping rules
- [COIL Binary Format](../coil-docs/core/binary-format.md) - Output format