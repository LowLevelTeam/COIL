# Hello World Example in CASM

## Overview

This example demonstrates a simple "Hello, World!" program in CASM, showing the basic structure of a COIL program and how to print text to the console.

## Code Example

```
; Hello World in CASM
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture

; Data section for our message
SECTION .data, 0x02 | 0x04 | 0x08    ; Writable, readable, initialized
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0  ; String with newline and null terminator

; Code section
SECTION .text, 0x01 | 0x04    ; Executable and readable
SYM _start, TYPE_PARAM0=GLOB  ; Global entry point symbol
    SCOPEE
    ; Declare variables for syscall
    VAR #1, TYPE_UNT64, 1      ; Linux syscall number for write
    VAR #2, TYPE_UNT64, 1      ; File descriptor for stdout
    VAR #3, TYPE_PTR, hello_msg  ; Pointer to our message
    VAR #4, TYPE_UNT64, 14     ; Length of our message (including newline)
    
    ; Write to stdout using syscall
    MOV TYPE_RGP=RAX, #1       ; Syscall number in RAX
    MOV TYPE_RGP=RDI, #2       ; First argument: file descriptor
    MOV TYPE_RGP=RSI, #3       ; Second argument: message pointer
    MOV TYPE_RGP=RDX, #4       ; Third argument: message length
    SYSCALL                    ; Invoke syscall
    
    ; Exit program with code 0
    VAR #5, TYPE_UNT64, 60     ; Linux syscall number for exit
    VAR #6, TYPE_UNT64, 0      ; Exit code 0 (success)
    MOV TYPE_RGP=RAX, #5       ; Syscall number in RAX
    MOV TYPE_RGP=RDI, #6       ; First argument: exit code
    SYSCALL                    ; Invoke syscall
    SCOPEL
```

## Code Breakdown

### Program Setup

```
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture
```

These directives specify that we're targeting a CPU processor with the x86-64 architecture. This helps the COIL processor generate appropriate code.

### Data Section

```
SECTION .data, 0x02 | 0x04 | 0x08    ; Writable, readable, initialized
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0
```

This creates a data section containing our message string:
- `SECTION .data` defines a data section with attributes for writable, readable, and initialized data
- `SYM hello_msg` creates a symbol that points to the beginning of our data
- `DATA` directive places our string in memory, with a newline (10) and null terminator (0)

### Code Section

```
SECTION .text, 0x01 | 0x04    ; Executable and readable
SYM _start, TYPE_PARAM0=GLOB  ; Global entry point symbol
```

This creates a code section and defines our entry point:
- `SECTION .text` defines a code section with executable and readable attributes
- `SYM _start, TYPE_PARAM0=GLOB` creates a global symbol (visible to the linker) that marks our entry point

### Variable Declaration

```
SCOPEE
VAR #1, TYPE_UNT64, 1      ; Linux syscall number for write
VAR #2, TYPE_UNT64, 1      ; File descriptor for stdout
VAR #3, TYPE_PTR, hello_msg  ; Pointer to our message
VAR #4, TYPE_UNT64, 14     ; Length of our message
```

We enter a scope and declare variables for our syscall using numeric IDs:
- `#1` holds the Linux syscall number for the write operation
- `#2` is the file descriptor for standard output
- `#3` is a pointer to our string
- `#4` is the length of our string

### Writing to Standard Output

```
MOV TYPE_RGP=RAX, #1       ; Syscall number in RAX
MOV TYPE_RGP=RDI, #2       ; First argument: file descriptor
MOV TYPE_RGP=RSI, #3       ; Second argument: message pointer
MOV TYPE_RGP=RDX, #4       ; Third argument: message length
SYSCALL                    ; Invoke syscall
```

This code prepares for and executes the write syscall:
- We move the syscall number into RAX
- We place the three syscall arguments in RDI, RSI, and RDX respectively
- We invoke the syscall using the SYSCALL instruction

### Program Exit

```
VAR #5, TYPE_UNT64, 60     ; Linux syscall number for exit
VAR #6, TYPE_UNT64, 0      ; Exit code 0 (success)
MOV TYPE_RGP=RAX, #5       ; Syscall number in RAX
MOV TYPE_RGP=RDI, #6       ; First argument: exit code
SYSCALL                    ; Invoke syscall
SCOPEL
```

Finally, we exit the program:
- We declare variables for the exit syscall number and exit code
- We prepare the syscall similarly to the write syscall
- We invoke the syscall to terminate the program
- We close our scope with SCOPEL

## Platform-Independent Version

The above example uses Linux syscalls. Here's a more platform-independent version using the standard COIL ABI system (available in version 2 with standard library):

```
; Hello World in CASM (platform-independent version)
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture

SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

SECTION .text, 0x01 | 0x04
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_PTR, hello_msg

    ; Call platform's print function (using platform_default ABI)
    EXTERN print_string, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    CALL print_string, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1
    
    ; Return 0
    VAR #2, TYPE_INT32, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #2
    SCOPEL
```

This version would use the standard library's print_string function when available.