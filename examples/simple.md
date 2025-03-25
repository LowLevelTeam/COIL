# Simple COIL Examples

This document provides several basic examples of COIL code to demonstrate core language features.

## Example 1: Hello World Function

This example shows how to implement a simple function that prints "Hello, World!" to the console.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define data section
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, Readable, Initialized data

; Define our data
SYM hello_str
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0  ; String with newline and null terminator

; Define our main function with standard ABI
SYM main, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Get string length
    MOV TYPE_RGP=RDI, hello_str
    CALL strlen
    MOV TYPE_RGP=RDX, TYPE_RGP=RAX  ; Length to write

    ; Write to stdout
    MOV TYPE_RGP=RAX, 1             ; syscall: write
    MOV TYPE_RGP=RDI, 1             ; file descriptor: stdout
    MOV TYPE_RGP=RSI, hello_str     ; buffer
    SYSCALL

    ; Return 0
    MOV TYPE_RGP=RAX, 0
    RET

; String length function (returns length in RAX)
SYM strlen, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Save RDI (input string pointer)
    MOV TYPE_RGP=RSI, TYPE_RGP=RDI
    
    ; Find null terminator
    MOV TYPE_RGP=RAX, 0             ; Counter
    
SYM strlen_loop
    CMP TYPE_UNT8=[TYPE_RGP=RSI], 0
    BR_EQ strlen_done
    INC TYPE_RGP=RSI
    INC TYPE_RGP=RAX
    BR strlen_loop
    
SYM strlen_done
    RET
```

## Example 2: 64-bit Integer Addition

This example shows a simple function that adds two 64-bit integers.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define our add function with standard ABI
SYM add_integers, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Parameters:
    ;   RDI = first integer
    ;   RSI = second integer
    ; Return:
    ;   RAX = result

    ; Add the integers
    MOV TYPE_RGP=RAX, TYPE_RGP=RDI
    ADD TYPE_RGP=RAX, TYPE_RGP=RSI
    
    ; Return result in RAX (already there)
    RET

; Example main function that uses our add function
SYM main, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Call add_integers(123, 456)
    MOV TYPE_RGP=RDI, 123
    MOV TYPE_RGP=RSI, 456
    CALL add_integers
    
    ; Return result
    RET
```

## Example 3: Variable Declarations and Scope

This example demonstrates COIL's variable system and scope management.

```
; Set processor to CPU
PROC 0x01

; Set architecture independent
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define our function
SYM calculate, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Enter a new scope
    SCOPEE
    
    ; Declare variables
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    
    ; Calculate x * y + 5
    MOV TYPE_RGP=EAX, x
    MUL TYPE_RGP=EAX, y
    ADD TYPE_RGP=EAX, 5
    
    ; Store result in a new variable
    VAR TYPE_INT32, result, TYPE_RGP=EAX
    
    ; Copy result to return register
    MOV TYPE_RGP=EAX, result
    
    ; Leave the scope (cleans up variables)
    SCOPEL
    
    ; Return the result (in EAX)
    RET
```

## Example 4: Cross-Architecture Code

This example demonstrates how to write COIL code that can adapt to different architectures.

```
; Set processor to CPU
PROC 0x01

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Conditional compilation for different architectures
SYM add_platform_integers, TYPE_PARAM0=BRANCH_CTRL_ABI

IF ARCH == 0x01 && MODE == 0x03  ; x86-64
    ; x86-64 implementation
    ; Parameters in RDI, RSI, return in RAX
    MOV TYPE_RGP=RAX, TYPE_RGP=RDI
    ADD TYPE_RGP=RAX, TYPE_RGP=RSI
    RET
ELIF ARCH == 0x02 && MODE == 0x02  ; ARM64
    ; ARM64 implementation
    ; Parameters in X0, X1, return in X0
    ADD TYPE_RGP=X0, TYPE_RGP=X0, TYPE_RGP=X1
    RET
ELIF ARCH == 0x03 && MODE == 0x02  ; RISC-V 64
    ; RISC-V 64 implementation
    ; Parameters in a0, a1, return in a0
    ADD TYPE_RGP=a0, TYPE_RGP=a0, TYPE_RGP=a1
    RET
ELSE
    ; Generic implementation using platform-independent types
    ; This assumes variables are defined by the ABI
    VAR TYPE_INT, result
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    
    MOV result, a
    ADD result, b
    
    ; Return based on ABI
    RET
ENDIF
```

## Example 5: Memory Operations

This example demonstrates various memory operations in COIL.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define data section
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, Readable, Initialized data

; Define BSS section
SECTION .bss, 0x02 | 0x04 | 0x10  ; Writable, Readable, Uninitialized data

; Define some initialized data
SYM source_buffer
DATA TYPE_ARRAY=TYPE_UNT8, "This is some source data", 0

; Define some uninitialized data
SYM dest_buffer
ALIGN 8
DATA TYPE_ARRAY=TYPE_UNT8[100]  ; 100 bytes of uninitialized data

; Function to demonstrate memory operations
SYM memory_demo, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; Enter scope
    SCOPEE
    
    ; Get the length of the source string
    MOV TYPE_RGP=RDI, source_buffer
    CALL strlen
    MOV TYPE_RGP=RCX, TYPE_RGP=RAX  ; Store length
    
    ; Copy memory
    MOV TYPE_RGP=RDI, dest_buffer   ; Destination
    MOV TYPE_RGP=RSI, source_buffer ; Source
    MOV TYPE_RGP=RDX, TYPE_RGP=RCX  ; Length
    CALL memcpy
    
    ; Compare memory
    MOV TYPE_RGP=RDI, dest_buffer
    MOV TYPE_RGP=RSI, source_buffer
    MOV TYPE_RGP=RDX, TYPE_RGP=RCX
    CALL memcmp
    
    ; Zero out the destination
    MOV TYPE_RGP=RDI, dest_buffer
    MOV TYPE_RGP=RSI, 0
    MOV TYPE_RGP=RDX, 100
    CALL memset
    
    ; Leave scope
    SCOPEL
    
    RET

; Simple memory copy function
SYM memcpy, TYPE_PARAM0=BRANCH_CTRL_ABI
    ; RDI = destination, RSI = source, RDX = length
    MOV TYPE_RGP=RCX, TYPE_RGP=RDX
    
    SYM memcpy_loop
    CMP TYPE_RGP=RCX, 0
    BR_EQ memcpy_done
    
    MOV TYPE_UNT8=AL, [TYPE_RGP=RSI]
    MOV [TYPE_RGP=RDI], TYPE_UNT8=AL
    
    INC TYPE_RGP=RSI
    INC TYPE_RGP=RDI
    DEC TYPE_RGP=RCX
    
    BR memcpy_loop
    
    SYM memcpy_done
    RET
```

These examples demonstrate various features of COIL, including functions, variables, memory operations, and cross-architecture compatibility. They showcase COIL's ability to work at a low level while maintaining portability across different architectures.