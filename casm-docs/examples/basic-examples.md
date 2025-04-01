# Basic CASM Examples

## Hello World Example

This example demonstrates a simple "Hello, World!" program in CASM, properly using the ABI system for system calls.

```
; Hello World in CASM
PROC 0x01                       ; CPU processor

; Define Linux syscall ABI
ABI linux_syscall
  PARAMS RAX, RDI, RSI, RDX, R10, R8, R9
  RETS RAX
  CALLER RAX, RCX, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT

; Data section for message
SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0  ; String with newline and null terminator

; Code section
SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB    ; Global entry point
    SCOPEE
    ; System call variables
    VAR #1, TYPE_UNT64, 1       ; syscall number (write)
    VAR #2, TYPE_UNT64, 1       ; file descriptor (stdout)
    VAR #3, TYPE_PTR, hello_msg ; message pointer
    VAR #4, TYPE_UNT64, 14      ; message length
    
    ; Write syscall using linux_syscall ABI
    SYSCALL TYPE_ABICTL=ABICTL_PARAM=linux_syscall, #1, #2, #3, #4
    
    ; Exit syscall using linux_syscall ABI
    VAR #5, TYPE_UNT64, 60      ; syscall number (exit)
    VAR #6, TYPE_UNT64, 0       ; exit code
    SYSCALL TYPE_ABICTL=ABICTL_PARAM=linux_syscall, #5, #6
    SCOPEL
```

### Key Concepts Demonstrated
- Section definitions (code and data)
- Symbol declaration
- Variable usage
- System calls using proper ABI abstraction
- Basic memory operations

## Arithmetic Operations

This example shows basic arithmetic operations in CASM.

```
; Basic arithmetic examples
PROC 0x01                       ; CPU

SECTION .text, 0x01 | 0x04
SYM arithmetic_example
    SCOPEE
    ; Integer arithmetic
    VAR #1, TYPE_INT32, 10      ; a = 10
    VAR #2, TYPE_INT32, 20      ; b = 20
    VAR #3, TYPE_INT32          ; result
    
    ADD #3, #1, #2              ; result = a + b = 30
    SUB #3, #2, #1              ; result = b - a = 10
    MUL #3, #1, #2              ; result = a * b = 200
    DIV #3, #2, #1              ; result = b / a = 2
    MOD #3, #2, #3              ; result = b % result = 0
    
    ; Floating-point
    VAR #4, TYPE_FP32, 3.5      ; x = 3.5
    VAR #5, TYPE_FP32, 2.0      ; y = 2.0
    VAR #6, TYPE_FP32           ; f_result
    
    ADD #6, #4, #5              ; f_result = x + y = 5.5
    DIV #6, #4, #5              ; f_result = x / y = 1.75
    
    ; Type conversion example
    VAR #7, TYPE_INT32          ; int_result
    CONVERT #7, #6              ; Convert float to int (truncates to 1)
    
    ; Vector arithmetic
    VAR #8, TYPE_V128=TYPE_FP32, (1.0, 2.0, 3.0, 4.0)  ; vec1
    VAR #9, TYPE_V128=TYPE_FP32, (5.0, 6.0, 7.0, 8.0)  ; vec2
    VAR #10, TYPE_V128=TYPE_FP32                       ; vec_result
    
    ADD #10, #8, #9             ; Element-wise addition = (6.0, 8.0, 10.0, 12.0)
    MUL #10, #8, #9             ; Element-wise multiplication = (5.0, 12.0, 21.0, 32.0)
    
    ; Dot product
    VAR #11, TYPE_FP32          ; dot_result
    VDOT #11, #8, #9            ; dot_result = 1*5 + 2*6 + 3*7 + 4*8 = 70.0
    
    SCOPEL
    RET
```

### Key Concepts Demonstrated
- Integer arithmetic
- Floating-point operations
- Type conversion
- Vector operations
- Combined operations