# Getting Started with COIL

## Introduction

This guide will help you set up your environment for working with COIL, understand the basic concepts, and create your first COIL program.

## Prerequisites

To work with COIL, you'll need:

1. A COIL processor implementation
2. CASM assembler (for human-readable code)
3. Basic understanding of assembly programming concepts

## Installation

### Installing the COIL Toolchain

1. Download the latest COIL toolchain from the official repository
2. Extract the archive to your preferred location
3. Add the bin directory to your system PATH

```bash
# Linux/macOS
export PATH=$PATH:/path/to/coil/bin

# Windows (PowerShell)
$env:Path += ";C:\path\to\coil\bin"
```

### Verifying Installation

Verify your installation with:

```bash
coilc --version
casm --version
```

## Your First COIL Program

### Hello World in CASM

Create a file named `hello.casm` with the following content:

```
; Hello World in CASM
.section text
.global main

main:
    SCOPE                       ; Begin function scope
    
    ; Load "Hello, World!" string address
    LEA r0, hello_str
    
    ; Call print function from standard library
    CALL std_print
    
    ; Return 0
    MOV r0, 0
    RET
    
    SCOPL                       ; End function scope

.section data
hello_str:
    .string "Hello, World!\n"
```

### Compiling and Running

1. Assemble the CASM code to COIL:

```bash
casm hello.casm -o hello.coil
```

2. Compile the COIL code to a native executable:

```bash
coilc hello.coil -o hello
```

3. Run the executable:

```bash
./hello
```

## Basic COIL Concepts

### Type-Determined Instructions

COIL's instructions adapt their behavior based on operand types:

```
; Integer addition
i32 a = 1
i32 b = 2
i32 c
ADD c, a, b        ; c = 3 (integer addition)

; Floating-point addition
f32 x = 1.5
f32 y = 2.5
f32 z
ADD z, x, y        ; z = 4.0 (floating-point addition)

; Vector addition
v128 v1 = [1, 2, 3, 4]
v128 v2 = [5, 6, 7, 8]
v128 v3
ADD v3, v1, v2     ; v3 = [6, 8, 10, 12] (vector addition)
```

### Memory Model

COIL provides various ways to access memory:

```
; Direct memory access
MOV r0, [0x1000]         ; Load from address 0x1000

; Variable access
MOV r1, my_var           ; Load from variable

; Indirect access
MOV r2, [r3]             ; Load from address in r3

; Based addressing
MOV r4, [r5 + 8]         ; Load from r5+8
```

### Control Flow

Control flow in COIL:

```
; Conditional branching
CMP r0, r1                ; Compare r0 and r1
BR EQ, equal_label        ; Branch if equal
BR NEQ, not_equal_label   ; Branch if not equal

; Function calls
CALL function_name        ; Call function
RET                       ; Return from function

; Loops
loop_start:
    ; Loop body
    DEC r0                ; Decrement counter
    CMP r0, 0             ; Compare with zero
    BR GT, loop_start     ; Branch if greater than
```

## Next Steps

Now that you've created your first COIL program, consider exploring:

1. [Core Specification](/docs/core/) to understand COIL's fundamentals
2. [CASM Documentation](/docs/tools/casm.md) for more assembly language details
3. [Standard Library](/docs/stdlib/overview.md) to leverage pre-built functionality
4. [Extensions](/docs/extensions/overview.md) for advanced capabilities

## Troubleshooting

### Common Issues

1. **"Command not found" error**: Ensure the COIL toolchain is in your PATH
2. **Assembly errors**: Check syntax against the CASM documentation
3. **Linker errors**: Verify all referenced symbols are defined
4. **Runtime errors**: Use the debugger to step through your code

### Getting Help

For additional assistance:

1. Check the comprehensive documentation
2. Visit the COIL community forum
3. Report bugs in the issue tracker