# COIL Function Examples

## Purpose

This document provides examples of function definitions, calls, and the ABI system in COIL. These examples demonstrate how to create and use functions across different processor types and architectures.

## Basic Function Definition

```
; Simple function to add two numbers
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Add numbers
    VAR TYPE_INT32, result
    ADD result, a, b
    
    ; Return result via ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Calling a Function

```
; Calling a function with parameters
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    ; Declare variables
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    VAR TYPE_INT32, sum
    
    ; Call function with parameters
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get return value
    MOV sum, TYPE_ABICTL=ABICTL_RET=platform_default
    
    SCOPEL
    RET
```

## Multiple Return Values

```
; Function with multiple return values
SYM divmod, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT32, dividend
    VAR TYPE_INT32, divisor
    MOV dividend, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV divisor, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Compute quotient and remainder
    VAR TYPE_INT32, quotient
    VAR TYPE_INT32, remainder
    DIV quotient, dividend, divisor
    MOD remainder, dividend, divisor
    
    ; Return both values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, quotient, remainder
    SCOPEL

; Using multiple return values
SYM use_divmod
    SCOPEE
    VAR TYPE_INT32, num, 42
    VAR TYPE_INT32, div, 5
    VAR TYPE_INT32, quot, rem
    
    ; Call function
    CALL divmod, TYPE_ABICTL=ABICTL_PARAM=platform_default, num, div
    
    ; Get both return values
    MOV quot, TYPE_ABICTL=ABICTL_RET=platform_default, 0
    MOV rem, TYPE_ABICTL=ABICTL_RET=platform_default, 1
    
    SCOPEL
    RET
```

## Recursive Functions

```
; Recursive factorial function
SYM factorial, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get input parameter
    VAR TYPE_UNT32, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base case check
    CMP n, 1
    BR_LE factorial_base_case
    
    ; Recursive case
    VAR TYPE_UNT32, n_minus_1
    SUB n_minus_1, n, 1
    
    ; Recursive call
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_1
    
    ; Get result and multiply by n
    VAR TYPE_UNT32, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    MUL result, result, n
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
    
SYM factorial_base_case
    ; Base case (n <= 1)
    VAR TYPE_UNT32, result, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Custom ABI Definition

```
; Define a custom ABI
ABI my_custom_abi
    PARAMS RDI, RSI, RDX, RCX, R8, R9  ; Parameter registers
    RETS RAX, RDX                      ; Return value registers
    CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11  ; Caller-saved
    CALLEE RBX, RSP, RBP, R12, R13, R14, R15  ; Callee-saved
    SALLIGN 16                         ; Stack alignment
    RZONE 128                          ; Red zone size
EXIT

; Function using custom ABI
SYM custom_function, TYPE_ABICTL=ABICTL_STANDARD=my_custom_abi
    SCOPEE
    ; Get parameter (using custom ABI)
    VAR TYPE_INT32, a
    MOV a, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, 0
    
    ; Double the value
    VAR TYPE_INT32, result
    MUL result, a, 2
    
    ; Return result via custom ABI
    RET TYPE_ABICTL=ABICTL_RET=my_custom_abi, result
    SCOPEL
```

## Function Pointers

```
; Function that takes a function pointer
SYM apply_operation, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, func_ptr
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV func_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 2
    
    ; Call the function through the pointer
    CALL func_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
    
    ; Forward the return value
    VAR TYPE_INT32, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Usage example
SYM main
    SCOPEE
    ; Call apply_operation with add_numbers function pointer
    VAR TYPE_INT32, x, 5
    VAR TYPE_INT32, y, 10
    VAR TYPE_INT32, result
    
    CALL apply_operation, TYPE_ABICTL=ABICTL_PARAM=platform_default, add_numbers, x, y
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    SCOPEL
    RET
```

## Platform-Specific Optimizations with Common Interface

```
; Function with processor-specific optimizations
SYM optimized_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, array_ptr
    VAR TYPE_INT32, count
    MOV array_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV count, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Different implementation based on processor
    IF ARCH == 0x01  ; x86
        IF MODE == 0x03  ; 64-bit mode
            ; x86-64 SIMD-optimized implementation
            VAR TYPE_INT32, result, 0
            VAR TYPE_V128=TYPE_INT32, sum_vec, (0, 0, 0, 0)
            
            ; Process vector chunks
            ; ... SIMD implementation here ...
        ELSE
            ; x86-32 implementation
            ; ... 32-bit implementation here ...
        ENDIF
    ELSE
        ; Generic implementation
        VAR TYPE_INT32, result, 0
        VAR TYPE_INT32, i, 0
        
        SYM sum_loop
        CMP i, count
        BR_GE sum_done
        
        VAR TYPE_INT32, value
        MOV value, [array_ptr + i*4]
        ADD result, result, value
        INC i
        BR sum_loop
        
        SYM sum_done
    ENDIF
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Related Documentation

- [ABI System](../spec/systems/abi-system.md) - Details on the ABI system
- [Control Flow](../spec/instruction-set/control-flow.md) - Control flow instructions
- [Basic Examples](basic.md) - Simple COIL examples
- [Type System](../spec/systems/type-system.md) - Type system documentation