# COIL ABI System

## Overview

The Application Binary Interface (ABI) system provides a processor-independent mechanism for parameter passing and return value handling while maintaining native performance. It applies to all forms of context switches, including function calls, system calls, and interrupts.

## What Is an ABI?

An ABI defines conventions for:
- Parameter passing (which registers or stack locations)
- Return value handling (which registers or memory)
- Register preservation responsibilities
- Stack alignment requirements 
- Calling convention details

COIL's ABI system automates these details, eliminating manual implementation of platform-specific conventions.

## ABI Definition

An ABI is defined using the `ABI` directive:

```
ABI abi_name
  PARAMS register_list    ; Parameter passing registers
  RETS register_list      ; Return value registers
  CALLER register_list    ; Caller-saved registers
  CALLEE register_list    ; Callee-saved registers
  SALLIGN alignment_value ; Stack alignment
  RZONE size_value        ; Red zone size
EXIT
```

### Example ABI Definitions

Function call ABI for x86-64 Linux:
```
ABI linux_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

System call ABI for Linux:
```
ABI linux_syscall
  PARAMS RAX, RDI, RSI, RDX, R10, R8, R9
  RETS RAX
  CALLER RAX, RCX, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

## Using ABIs

### Function Call with ABI

```
; Function call using linux_x86_64 ABI
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, param1, param2

; Get return value
MOV result, TYPE_ABICTL=ABICTL_RET=linux_x86_64
```

### System Call with ABI

```
; System call using linux_syscall ABI
SYSCALL TYPE_ABICTL=ABICTL_PARAM=linux_syscall, syscall_number, arg1, arg2, arg3

; Get system call result
MOV result, TYPE_ABICTL=ABICTL_RET=linux_syscall
```

### Receiving Parameters in Functions

```
; Get parameters with explicit ABI
MOV #1, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0  ; First parameter
MOV #2, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1  ; Second parameter
```

### Returning Values from Functions

```
; Return values with explicit ABI
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result1, result2
```

## Common ABI Definitions

### Linux x86-64 Function Call ABI

```
ABI linux_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

### Linux System Call ABI

The same ABI works for both ARM64 and x86-64 when using system call numbers:

```
ABI linux_syscall
  PARAMS RAX, RDI, RSI, RDX, R10, R8, R9  ; For x86-64
  RETS RAX
  CALLER RAX, RCX, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

## Practical Example: Linux Hello World

```
; Hello World for Linux 64-bit
PROC 0x01                       ; CPU processor (no architecture needed)

; Define Linux syscall ABI
ABI linux_syscall
  PARAMS RAX, RDI, RSI, RDX, R10, R8, R9
  RETS RAX
  CALLER RAX, RCX, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT

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

SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0  ; String with newline and null terminator
```

This code works on any Linux 64-bit system regardless of architecture (ARM64 or x86-64) since the system call numbers remain the same across architectures, while the ABI handles the architecture-specific register assignments.

## Related Components

- [Toolchain Components](/coil-docs/implementation/toolchain-components.md) - ABI role in the toolchain
- [Instruction Reference](/coil-docs/reference/instruction-reference.md) - ABI-related instructions