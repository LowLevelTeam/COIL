# COIL Assembly Language (CASM)

## Overview

CASM (COIL Assembly) is a human-readable text representation of COIL binary code. It provides a syntax for writing and visualizing COIL programs, serving as a textual interface to the binary COIL format.

## Purpose

CASM serves as:

1. **Human-Readable Representation**: Makes COIL binary code readable and writable
2. **Learning Tool**: Provides a way to understand COIL concepts
3. **Debugging Aid**: Facilitates inspection and debugging of COIL programs

Note that compilers targeting COIL can generate the binary format directly without using CASM as an intermediate step.

## Basic Syntax

CASM uses a simple instruction-based syntax:

```
label:  instruction operand1, operand2, operand3  ; comment
```

### Examples

```
; Simple addition
        ADD r0, r1, r2         ; r0 = r1 + r2

; Conditional branch
        CMP r0, r1             ; Compare r0 and r1
        BR EQ, equal_label     ; Branch if equal

; Function call
        CALL function_name     ; Call function
        RET                    ; Return from function
```

## Data Types

CASM supports all COIL data types with a readable syntax:

```
; Integer types
i8 var_i8  = -42               ; 8-bit signed integer
u8 var_u8  = 42                ; 8-bit unsigned integer
i32 var_i32 = 1234567          ; 32-bit signed integer

; Floating-point types
f32 var_f32 = 3.14159          ; 32-bit float
f64 var_f64 = 2.71828          ; 64-bit double

; Vector types
v128 var_v128                  ; 128-bit vector
```

## Directives

Directives control the assembly process and define metadata:

```
.section text                  ; Begin text section
.global main                   ; Export symbol 'main'
.align 16                      ; Align to 16-byte boundary
.byte 0x00, 0x01, 0x02         ; Define byte sequence
.word 0x1234                   ; Define 16-bit word
.dword 0x12345678              ; Define 32-bit doubleword
.string "Hello, World!"        ; Define null-terminated string
```

## Textual Representation of Preprocessor Instructions

CASM provides a textual syntax for COIL's preprocessor instructions:

```
!define DEBUG                  ; Represents PPDEF instruction
!undef NDEBUG                  ; Represents PPUNDEF instruction

!if defined(DEBUG)            ; Represents PPCOND and PPIF instructions
    ; Debug code here
!else                         ; Represents PPELSE instruction
    ; Release code here
!endif                        ; Represents PPENDIF instruction

!include "header.inc"          ; Represents PPINC instruction

!target CPU                    ; Represents PPTARG instruction
!arch X86_64                   ; Represents PPTARG instruction
```

These directives are processed by the CASM assembler and translated to their corresponding COIL binary instructions.

## Functions and Scope

Functions and scopes are explicitly defined:

```
function_name:
        SCOPE                  ; Begin function scope
        ; Function body
        RET                    ; Return
        SCOPL                  ; End function scope

block_example:
        SCOPE                  ; Begin block scope
        ; Block code
        SCOPL                  ; End block scope
```

## Memory Access

CASM provides syntax for various memory access patterns:

```
        MOV r0, [0x1000]       ; Direct memory access
        MOV r1, [r2]           ; Indirect memory access
        MOV r3, [r4 + 8]       ; Based addressing
        MOV r5, [r6 + r7*4]    ; Indexed addressing
```

## Type Flags

Type flags can be specified with clear syntax:

```
        MOV r0, const i32 42   ; Constant integer
        VAR r1, volatile i32   ; Volatile integer variable
        ATOMLD r2, [r3], acq   ; Atomic load with acquire semantics
```

## Processing Unit Targeting

CASM allows targeting specific processing units:

```
!target CPU
        ; CPU-specific code here

!target GPU
        ; GPU-specific code here

!target NPU
        ; NPU-specific code here
```

## Binary Correspondence

CASM has a direct correspondence to the COIL binary format:

```
; CASM
        ADD r0, r1, r2

; Binary equivalent
[0x40][0x03]
  [0x04][0x40][r0_id]
  [0x04][0x40][r1_id]
  [0x04][0x40][r2_id]
```