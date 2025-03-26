# COIL Assembly Syntax

## Purpose

This document defines the formal syntax rules for COIL Assembly (COIL-ASM), the human-readable text representation of COIL binary instructions. These syntax rules allow developers to write and read COIL programs in a consistent manner.

## General Syntax Rules

### Line Structure

Each line in COIL-ASM contains at most one of the following:
- A single instruction or directive
- A label definition
- A comment
- A blank line

### Comments

Comments start with a semicolon (`;`) and continue to the end of the line:

```
; This is a comment
MOV counter, 0  ; This is an inline comment
```

### Labels

Labels identify locations in the code and end with a colon (`:`):

```
loop_start:     ; Define a label
    ADD counter, counter, 1
    CMP counter, 10
    BR_LT loop_start  ; Reference the label
```

Labels must:
- Begin with a letter or underscore
- Contain only letters, numbers, and underscores
- Be unique within their scope
- End with a colon when defined

### Instructions

Instructions follow this format:

```
OPCODE [operand1[, operand2[, ...]]]
```

Where:
- `OPCODE` is an instruction mnemonic (e.g., `MOV`, `ADD`)
- Operands are separated by commas
- Operands can be variables, constants, types, or memory references

Conditional instructions use an underscore and condition suffix:

```
BR_EQ label     ; Branch if equal
MOV_NZ a, b     ; Move if not zero
```

### Operands

#### Immediate Values

Numeric literals:
```
42              ; Decimal integer
0xFF            ; Hexadecimal integer
0b1010          ; Binary integer
3.14159         ; Floating point
```

String literals:
```
"Hello, World"  ; String literal
```

#### Variable References

Variables are referenced by name:
```
counter         ; Variable reference
```

#### Register References

Direct register references use the TYPE_RGP syntax:
```
TYPE_RGP=RAX    ; Direct reference to RAX register
```

#### Memory References

Memory references use square brackets:
```
[address]                      ; Direct memory reference
[TYPE_RGP=RAX]                 ; Memory at address in RAX
[base + index*scale]           ; Base + Index*Scale addressing
[base + offset]                ; Base + Offset addressing
[base + index*scale + offset]  ; Full addressing mode
```

#### Type References

Type references use the TYPE_ prefix:
```
TYPE_INT32      ; 32-bit signed integer type
TYPE_V128=TYPE_FP32  ; 128-bit vector of 32-bit floats
```

### Directives

Directives control the assembler and begin with an uppercase letter:

```
PROC 0x01       ; Set processor type to CPU
ARCH 0x01, 0x03 ; Set architecture to x86-64
SECTION .text, 0x01 | 0x04  ; Define text section
```

### Variable Declaration

Variables are declared using the VAR instruction:

```
VAR type, name[, initial_value]
```

Examples:
```
VAR TYPE_INT32, counter, 0      ; Integer with initialization
VAR TYPE_PTR, data_pointer      ; Pointer without initialization
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"  ; Array initialization
```

### Scope Blocks

Scope blocks are defined with SCOPEE and SCOPEL:

```
SCOPEE
    ; Variables and code in this scope
    VAR TYPE_INT32, local_var, 0
    
    ; Nested scope
    SCOPEE
        VAR TYPE_INT32, inner_var, 0
    SCOPEL  ; inner_var destroyed here
    
SCOPEL  ; local_var destroyed here
```

### ABI Blocks

ABI definitions are enclosed in ABI and EXIT directives:

```
ABI custom_abi
    PARAMS RDI, RSI, RDX
    RETS RAX
    CALLER RAX, RCX, RDX
    CALLEE RBX, RSP, RBP
    SALLIGN 16
    RZONE 0
EXIT
```

### Conditional Assembly

Conditional assembly uses IF, ELIF, ELSE, and ENDIF:

```
IF ARCH == 0x01
    ; x86-specific code
ELIF ARCH == 0x02
    ; ARM-specific code
ELSE
    ; Generic code
ENDIF
```

## Whitespace and Formatting

- Whitespace is not significant except as a delimiter
- Indentation is recommended for readability but not required
- Line continuation is not supported; each instruction must be on a single line

## Case Sensitivity

- Instruction opcodes, directives, and register names are case-sensitive (conventionally uppercase)
- Variable and label names are case-sensitive
- Type names are case-sensitive (conventionally uppercase for the TYPE_ prefix)

## Reserved Words

The following are reserved words and cannot be used as variable or label names:
- All instruction opcodes (MOV, ADD, etc.)
- All directive names (PROC, ARCH, etc.)
- All type names (TYPE_INT32, etc.)
- All register names (RAX, RBX, etc.)

## Including Files

Files can be included using the INCLUDE directive:

```
INCLUDE "header.coilh"
```

## Error Signaling

The assembler will generate errors for:
- Unknown opcodes
- Invalid operand types
- Undefined symbols
- Syntax errors
- Type mismatches
- Invalid register usage

## Example of Well-Formed COIL-ASM

```
; Example of well-formed COIL-ASM
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .data, 0x02 | 0x04 | 0x08
SYM message
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 0

SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_PTR, msg_ptr, message
    VAR TYPE_INT, result, 0
    
    ; Compare something
    CMP TYPE_RGP=RAX, 0
    BR_EQ zero_case
    
    ; Non-zero case
    MOV result, 1
    BR end_func
    
zero_case:
    MOV result, 0
    
end_func:
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```