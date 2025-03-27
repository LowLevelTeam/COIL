# Function Examples in CASM

## Basic Function Definition

The simplest function takes no parameters and returns no values:

```
; Simple function with no parameters or return values
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM simple_function
    SCOPEE
    ; Function body
    VAR TYPE_INT32, local_var, 42
    ; Do something with local_var
    SCOPEL
    RET
```

## Functions with Parameters

This example shows how to define a function that takes parameters:

```
; Function with parameters
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get the parameters
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Calculate the sum
    VAR TYPE_INT32, result
    ADD result, a, b
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Functions with Return Values

This example demonstrates returning a value from a function:

```
; Function with a return value
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM calculate_square, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get the parameter
    VAR TYPE_INT32, x
    MOV x, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Calculate the square
    VAR TYPE_INT32, result
    MUL result, x, x
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Calling Functions

This example shows how to call functions:

```
; Calling functions
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_INT32, a, 5
    VAR TYPE_INT32, b, 7
    VAR TYPE_INT32, result
    
    ; Call add_numbers function
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
    
    ; Get the return value
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Call calculate_square function
    CALL calculate_square, TYPE_ABICTL=ABICTL_PARAM=platform_default, result
    
    ; Get the return value
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    SCOPEL
    RET
```

## Multiple Return Values

COIL allows functions to return multiple values:

```
; Function with multiple return values
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM divmod, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT32, dividend
    VAR TYPE_INT32, divisor
    MOV dividend, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV divisor, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Calculate quotient and remainder
    VAR TYPE_INT32, quotient
    VAR TYPE_INT32, remainder
    DIV quotient, dividend, divisor
    MOD remainder, dividend, divisor
    
    ; Return both values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, quotient, remainder
    SCOPEL

; Calling a function with multiple return values
SYM call_divmod
    SCOPEE
    VAR TYPE_INT32, a, 17
    VAR TYPE_INT32, b, 5
    VAR TYPE_INT32, q, r
    
    ; Call divmod function
    CALL divmod, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
    
    ; Get both return values
    MOV q, TYPE_ABICTL=ABICTL_RET=platform_default, 0  ; First return value
    MOV r, TYPE_ABICTL=ABICTL_RET=platform_default, 1  ; Second return value
    
    ; q now contains 3, r contains 2 (17 = 3*5 + 2)
    SCOPEL
    RET
```

## Recursive Functions

This example demonstrates recursion with a Fibonacci function:

```
; Recursive Fibonacci function
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM fibonacci, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter
    VAR TYPE_INT32, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base cases: fibonacci(0) = 0, fibonacci(1) = 1
    CMP n, 0
    BR_EQ return_zero
    
    CMP n, 1
    BR_EQ return_one
    
    ; Recursive case: fibonacci(n) = fibonacci(n-1) + fibonacci(n-2)
    VAR TYPE_INT32, n_minus_1, n_minus_2
    SUB n_minus_1, n, 1
    SUB n_minus_2, n, 2
    
    ; Calculate fibonacci(n-1)
    CALL fibonacci, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_1
    VAR TYPE_INT32, fib_n_minus_1
    MOV fib_n_minus_1, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Calculate fibonacci(n-2)
    CALL fibonacci, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_2
    VAR TYPE_INT32, fib_n_minus_2
    MOV fib_n_minus_2, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Add the results
    VAR TYPE_INT32, result
    ADD result, fib_n_minus_1, fib_n_minus_2
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    
    SYM return_zero
    VAR TYPE_INT32, zero, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, zero
    
    SYM return_one
    VAR TYPE_INT32, one, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, one
    
    SCOPEL
```

## Iterative Fibonacci (More Efficient)

A more efficient, non-recursive implementation:

```
; Iterative Fibonacci function
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM fibonacci_iterative, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter
    VAR TYPE_INT32, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base cases
    CMP n, 0
    BR_EQ return_zero
    
    CMP n, 1
    BR_EQ return_one
    
    ; Iterative calculation
    VAR TYPE_INT32, prev, 0
    VAR TYPE_INT32, current, 1
    VAR TYPE_INT32, i, 2
    
    SYM loop_start
    CMP i, n
    BR_GT loop_end
    
    ; Calculate next Fibonacci number
    VAR TYPE_INT32, next
    ADD next, prev, current
    
    ; Update prev and current
    MOV prev, current
    MOV current, next
    
    INC i
    BR loop_start
    
    SYM loop_end
    ; current now contains fibonacci(n)
    RET TYPE_ABICTL=ABICTL_RET=platform_default, current
    
    SYM return_zero
    VAR TYPE_INT32, zero, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, zero
    
    SYM return_one
    VAR TYPE_INT32, one, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, one
    
    SCOPEL
```

## Function Pointers

COIL allows using function pointers:

```
; Function pointer example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Define a higher-order function that applies an operation
SYM apply_operation, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, func_ptr
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV func_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; Function pointer
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1         ; First value
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 2         ; Second value
    
    ; Call the function through the pointer
    CALL func_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
    
    ; Forward the return value
    VAR TYPE_INT32, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Define some operation functions
SYM add, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT32, a, b, result
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    ADD result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

SYM multiply, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT32, a, b, result
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    MUL result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Main function to demonstrate function pointers
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    VAR TYPE_INT32, result
    
    ; Use add operation
    CALL apply_operation, TYPE_ABICTL=ABICTL_PARAM=platform_default, add, x, y
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    ; result = 30 (10 + 20)
    
    ; Use multiply operation
    CALL apply_operation, TYPE_ABICTL=ABICTL_PARAM=platform_default, multiply, x, y
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    ; result = 200 (10 * 20)
    
    SCOPEL
    RET
```

## Custom ABIs

This example demonstrates using a custom ABI for a specific function:

```
; Custom ABI example
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

; Define a custom ABI
ABI my_custom_abi
    PARAMS RDI, RSI, RDX      ; Parameter registers
    RETS RAX                  ; Return value register
    CALLER RAX, RCX, RDX, RSI, RDI  ; Caller-saved
    CALLEE RBX, RSP, RBP      ; Callee-saved
    SALLIGN 16                ; Stack alignment
    RZONE 0                   ; No red zone
EXIT

; Function using the custom ABI
SYM custom_function, TYPE_ABICTL=ABICTL_STANDARD=my_custom_abi
    SCOPEE
    ; Get parameters using the custom ABI
    VAR TYPE_INT32, a
    MOV a, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, 0
    
    ; Double the value
    VAR TYPE_INT32, result
    MUL result, a, 2
    
    ; Return result via the custom ABI
    RET TYPE_ABICTL=ABICTL_RET=my_custom_abi, result
    SCOPEL

; Main function using the platform default ABI
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_INT32, value, 21
    VAR TYPE_INT32, result
    
    ; Call the custom ABI function
    CALL custom_function, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, value
    
    ; Get return value
    MOV result, TYPE_ABICTL=ABICTL_RET=my_custom_abi
    ; result = 42 (21 * 2)
    
    SCOPEL
    RET
```