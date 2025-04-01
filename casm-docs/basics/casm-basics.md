# CASM Basics

## What is CASM?

CASM (COIL Assembly) is the human-readable text representation of COIL (Computer Oriented Intermediate Language). It serves as an assembly language for writing programs that will be translated into COIL's binary format.

## Basic Syntax

Each line in CASM follows this pattern:
```
[label:] [instruction|directive] [operands...] [; comment]
```

### Comments
```
; This is a full-line comment
MOV counter, 0  ; This is an inline comment
```

### Labels and Symbols
```
loop_start:        ; Define a label
    ; Code
    BR loop_start  ; Reference the label

SYM function_name      ; Define a local symbol
SYM main, TYPE_PARAM0=GLOB  ; Define a global symbol
```

## Variables and Scopes

### Variable Declaration
```
VAR #1, TYPE_INT32, 0       ; Integer with initialization
VAR #2, TYPE_PTR            ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159  ; Floating-point with initialization
```

### Variable Usage
```
ADD #1, #1, 1     ; Add 1 to variable #1
MUL #4, #2, #3    ; Multiply variables #2 and #3, store in #4
```

### Scopes
```
SCOPEE
    ; Variables declared here exist within this scope
    VAR #1, TYPE_INT32, 0
    
    ; Nested scopes can be used
    SCOPEE
        VAR #1, TYPE_INT32, 42  ; Inner scope #1 is distinct
    SCOPEL  ; Inner scope ends, its variables are destroyed
    
SCOPEL  ; Outer scope ends, its variables are destroyed
```

## Literals and Constants

### Integer Literals
```
42              ; Decimal integer
0xFF            ; Hexadecimal integer (255)
0b1010          ; Binary integer (10)
0o777           ; Octal integer (511)
```

### Floating-Point Literals
```
3.14159         ; Standard decimal floating point
.5              ; Leading decimal point (0.5)
5.              ; Trailing decimal point (5.0)
1.5e3           ; Scientific notation (1500.0)
```

### String Literals
```
"Hello, World"  ; Standard string
"Line 1\nLine 2"  ; String with newline escape
'A'             ; Character literal
```

### Type Literals
```
TYPE_INT32      ; 32-bit integer type
TYPE_FP64       ; 64-bit floating point type
TYPE_PTR=TYPE_INT32  ; Pointer to 32-bit integer
TYPE_V128=TYPE_FP32  ; 128-bit vector of 32-bit floats
```

## Memory Access

```
MOV [address], 42         ; Store to memory at 'address'
MOV value, [address]      ; Load from memory at 'address'
MOV [base_ptr + 8], value ; Store with offset
MOV [array + index*4], value  ; Store with scaled index
```

## Sections and Directives

### Sections
```
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized
```

### Common Directives
```
PROC 0x01                     ; Set processor type to CPU
ARCH 0x01, 0x03               ; Set architecture to x86-64
ALIGN 16                      ; Align to 16-byte boundary
DATA TYPE_INT32, 42           ; Insert 32-bit integer
```

## Basic Instructions

### Data Movement
```
MOV dest, src                 ; Move data
PUSH value                    ; Push to stack
POP dest                      ; Pop from stack
```

### Arithmetic
```
ADD result, a, b              ; Addition
SUB result, a, b              ; Subtraction
MUL result, a, b              ; Multiplication
DIV result, a, b              ; Division
INC counter                   ; Increment
DEC counter                   ; Decrement
```

### Control Flow
```
BR target                     ; Branch to target
BR_EQ equal_case              ; Branch if equal
BR_LT less_than_case          ; Branch if less than
CMP a, b                      ; Compare values
```

### Function Calls
```
CALL function                 ; Call with no parameters
CALL add, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
                              ; Call with parameters
RET                           ; Return with no value
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
                              ; Return with value
```

## Example Program

```
; Hello World in CASM
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture

; Data section for our message
SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

; Code section
SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB
    SCOPEE
    ; Variables for syscall
    VAR #1, TYPE_UNT64, 1      ; syscall number (write)
    VAR #2, TYPE_UNT64, 1      ; file descriptor (stdout)
    VAR #3, TYPE_PTR, hello_msg  ; message pointer
    VAR #4, TYPE_UNT64, 14     ; message length
    
    ; Write to stdout
    MOV TYPE_RGP=RAX, #1
    MOV TYPE_RGP=RDI, #2
    MOV TYPE_RGP=RSI, #3
    MOV TYPE_RGP=RDX, #4
    SYSCALL
    
    ; Exit program
    VAR #5, TYPE_UNT64, 60     ; syscall number (exit)
    VAR #6, TYPE_UNT64, 0      ; exit code
    MOV TYPE_RGP=RAX, #5
    MOV TYPE_RGP=RDI, #6
    SYSCALL
    SCOPEL
```

## CASM Workflow

The typical workflow with CASM:

1. Write program in CASM (`.casm` file)
2. Use COIL assembler to translate to COIL binary (`.coil` file)
3. Optionally link with other COIL objects
4. Run the COIL binary using a COIL processor

```
source.casm → [COIL assembler] → program.coil → [COIL processor] → program.coilo → [COIL linker] → Execution
                                                                 → Interpreted Execution
```