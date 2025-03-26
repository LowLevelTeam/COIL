# Function and ABI Examples

This document provides a comprehensive set of examples demonstrating how to define, implement, and call functions in COIL using the ABI system. These examples showcase the power and flexibility of COIL's approach to function abstraction across different processor types.

## Basic Function Definition and Usage

### Simple Function with Platform-Default ABI

```
; Function that adds two integers
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Add numbers
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return result via ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Function to call add_numbers
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Declare variables
    VAR TYPE_INT, x, 10
    VAR TYPE_INT, y, 20
    VAR TYPE_INT, sum
    
    ; Call function with parameters
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get return value
    MOV sum, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Return 0 to indicate success
    VAR TYPE_INT, status, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, status
    SCOPEL
```

### Function with Multiple Return Values

```
; Function to perform division with remainder
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM divmod, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT, dividend
    VAR TYPE_INT, divisor
    MOV dividend, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV divisor, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Compute quotient and remainder
    VAR TYPE_INT, quotient
    VAR TYPE_INT, remainder
    DIV quotient, dividend, divisor
    MOD remainder, dividend, divisor
    
    ; Return both values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, quotient, remainder
    SCOPEL

; Function using multiple return values
SYM use_divmod, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, num, 42
    VAR TYPE_INT, div, 5
    VAR TYPE_INT, quot, rem
    
    ; Call function
    CALL divmod, TYPE_ABICTL=ABICTL_PARAM=platform_default, num, div
    
    ; Get both return values
    MOV quot, TYPE_ABICTL=ABICTL_RET=platform_default, 0  ; First return value
    MOV rem, TYPE_ABICTL=ABICTL_RET=platform_default, 1   ; Second return value
    
    ; Use both values
    ; ...
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    SCOPEL
```

## Custom ABI Definitions and Usage

### Defining a Custom ABI

```
; Define a custom ABI for a specific calling convention
PROC 0x01                     ; CPU

; Define a custom ABI
ABI my_custom_abi
    PARAMS R0, R1, R2, R3     ; First 4 parameters in these registers
    RETS R0                   ; Return value in R0
    CALLER R0, R1, R2, R3     ; Caller-saved registers
    CALLEE R4, R5, R6, R7     ; Callee-saved registers
    SALLIGN 8                 ; 8-byte stack alignment
    RZONE 0                   ; No red zone
EXIT

; Function using custom ABI
SYM custom_function, TYPE_ABICTL=ABICTL_STANDARD=my_custom_abi
    SCOPEE
    ; Get parameters (using custom ABI)
    VAR TYPE_INT, a
    MOV a, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, 0
    
    ; Double the value
    VAR TYPE_INT, result
    MUL result, a, 2
    
    ; Return via custom ABI
    RET TYPE_ABICTL=ABICTL_RET=my_custom_abi, result
    SCOPEL

; Calling custom ABI function
SYM use_custom, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, value, 42
    VAR TYPE_INT, result
    
    ; Call using custom ABI
    CALL custom_function, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, value
    
    ; Get return value from custom ABI
    MOV result, TYPE_ABICTL=ABICTL_RET=my_custom_abi
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    SCOPEL
```

### Standard CPU Architecture ABIs

```
; Function using x86-64 System V ABI (Linux/macOS/BSD)
PROC 0x01                     ; CPU

SYM linux_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    VAR TYPE_INT64, param1
    VAR TYPE_INT64, param2
    MOV param1, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0  ; Will be in RDI
    MOV param2, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1  ; Will be in RSI
    
    ; Function body...
    
    VAR TYPE_INT64, result
    ADD result, param1, param2
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result        ; Will be in RAX
    SCOPEL

; Function using x64 Microsoft ABI (Windows)
SYM windows_function, TYPE_ABICTL=ABICTL_STANDARD=windows_x64
    SCOPEE
    VAR TYPE_INT64, param1
    VAR TYPE_INT64, param2
    MOV param1, TYPE_ABICTL=ABICTL_PARAM=windows_x64, 0    ; Will be in RCX
    MOV param2, TYPE_ABICTL=ABICTL_PARAM=windows_x64, 1    ; Will be in RDX
    
    ; Function body...
    
    VAR TYPE_INT64, result
    ADD result, param1, param2
    RET TYPE_ABICTL=ABICTL_RET=windows_x64, result         ; Will be in RAX
    SCOPEL
```

## Recursive Functions

### Factorial Function

```
; Recursive factorial function
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM factorial, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get input parameter
    VAR TYPE_UNT64, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base case check
    CMP n, 1
    BR_LE factorial_base_case
    
    ; Recursive case
    VAR TYPE_UNT64, n_minus_1
    SUB n_minus_1, n, 1
    
    ; Recursive call
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_1
    
    ; Get result and multiply by n
    VAR TYPE_UNT64, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    MUL result, result, n
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
    
SYM factorial_base_case
    ; Base case (n <= 1)
    VAR TYPE_UNT64, result, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

### Fibonacci Function

```
; Recursive Fibonacci function
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM fibonacci, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get input parameter
    VAR TYPE_UNT32, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base cases
    CMP n, 0
    BR_EQ fib_zero
    CMP n, 1
    BR_EQ fib_one
    
    ; Recursive case: fib(n) = fib(n-1) + fib(n-2)
    VAR TYPE_UNT32, n_minus_1
    SUB n_minus_1, n, 1
    
    VAR TYPE_UNT32, n_minus_2
    SUB n_minus_2, n, 2
    
    ; Call fib(n-1)
    CALL fibonacci, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_1
    VAR TYPE_UNT32, fib_n_minus_1
    MOV fib_n_minus_1, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Call fib(n-2)
    CALL fibonacci, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_2
    VAR TYPE_UNT32, fib_n_minus_2
    MOV fib_n_minus_2, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Return fib(n-1) + fib(n-2)
    VAR TYPE_UNT32, result
    ADD result, fib_n_minus_1, fib_n_minus_2
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
    
SYM fib_zero
    ; Base case: fib(0) = 0
    VAR TYPE_UNT32, result, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
    
SYM fib_one
    ; Base case: fib(1) = 1
    VAR TYPE_UNT32, result, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Function Pointers

### Using Function Pointers

```
; Working with function pointers
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Several simple functions with the same signature
SYM add_function, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, a, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    VAR TYPE_INT, result
    ADD result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

SYM subtract_function, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, a, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    VAR TYPE_INT, result
    SUB result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

SYM multiply_function, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, a, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    VAR TYPE_INT, result
    MUL result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Function that takes a function pointer and calls it
SYM apply_operation, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, operation_ptr
    VAR TYPE_INT, operand1
    VAR TYPE_INT, operand2
    MOV operation_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV operand1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    MOV operand2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 2
    
    ; Call the function through the pointer
    CALL operation_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, operand1, operand2
    
    ; Forward the return value
    VAR TYPE_INT, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Example using function pointers
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, x, 10
    VAR TYPE_INT, y, 5
    VAR TYPE_INT, result
    
    ; Perform addition
    CALL apply_operation, TYPE_ABICTL=ABICTL_PARAM=platform_default, add_function, x, y
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Perform multiplication
    CALL apply_operation, TYPE_ABICTL=ABICTL_PARAM=platform_default, multiply_function, x, y
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    SCOPEL
```

## Variadic Functions

### Simple Variadic Sum Function

```
; Variadic function to sum a variable number of values
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM sum_values, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter count
    VAR TYPE_INT, count
    MOV count, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Initialize sum
    VAR TYPE_INT, sum, 0
    
    ; Iterate through parameters
    VAR TYPE_INT, i, 0
    
    SYM sum_loop
    CMP i, count
    BR_GE sum_done
    
    ; Get next parameter
    VAR TYPE_INT, param_index, value
    ADD param_index, i, 1  ; Skip the count parameter (index 0)
    MOV value, TYPE_ABICTL=ABICTL_VARIADIC=platform_default, param_index
    
    ; Add to sum
    ADD sum, sum, value
    
    ; Next parameter
    INC i
    BR sum_loop
    
    SYM sum_done
    ; Return the sum
    RET TYPE_ABICTL=ABICTL_RET=platform_default, sum
    SCOPEL

; Using the variadic function
SYM use_variadic, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, count, 5
    VAR TYPE_INT, result
    
    ; Call with 5 values
    CALL sum_values, TYPE_ABICTL=ABICTL_PARAM=platform_default, count, 10, 20, 30, 40, 50
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    SCOPEL
```

## Cross-Processor Programming

### Architecture-Independent Function

```
; Function that works across processor types
PROC 0x01  ; CPU in v1 (would be processor-agnostic in v3)

SECTION .text, 0x01 | 0x04
SYM calculate_average, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters (works on any processor)
    VAR TYPE_PTR, array_ptr
    VAR TYPE_INT, count
    MOV array_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV count, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Initialize sum
    VAR TYPE_INT, sum, 0
    
    ; Sum all elements
    VAR TYPE_INT, i, 0
    
    SYM avg_loop
    CMP i, count
    BR_GE avg_done
    
    ; Get array element
    VAR TYPE_INT, value
    MOV value, [array_ptr + i*4]  ; Assumes 4-byte integers
    
    ; Add to sum
    ADD sum, sum, value
    
    ; Next element
    INC i
    BR avg_loop
    
    SYM avg_done
    ; Calculate average
    VAR TYPE_FP32, float_sum, float_count, average
    CONVERT float_sum, sum
    CONVERT float_count, count
    DIV average, float_sum, float_count
    
    ; Return the average
    RET TYPE_ABICTL=ABICTL_RET=platform_default, average
    SCOPEL
```

### Processor-Optimized Function with Common Interface

```
; Function with processor-specific optimizations but common interface
PROC 0x01  ; CPU

SECTION .text, 0x01 | 0x04
SYM optimized_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, array_ptr
    VAR TYPE_INT, count
    MOV array_ptr, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV count, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Different implementation based on processor
    IF ARCH == 0x01  ; x86
        IF MODE == 0x03  ; 64-bit mode
            ; x86-64 SIMD-optimized implementation
            VAR TYPE_INT, result, 0
            
            ; Use SIMD operations for chunks of 4 elements
            VAR TYPE_INT, chunks, remainder, i
            DIV chunks, count, 4
            MOD remainder, count, 4
            
            ; Process chunks
            VAR TYPE_V128=TYPE_INT32, sum_vec, (0, 0, 0, 0)
            VAR TYPE_INT, chunk_idx, 0
            
            SYM chunk_loop
            CMP chunk_idx, chunks
            BR_GE chunk_done
            
            ; Load 4 elements at once
            VAR TYPE_V128=TYPE_INT32, data_vec
            VLOAD data_vec, [array_ptr + chunk_idx*16]  ; 4 ints = 16 bytes
            
            ; Add to running sum
            VADD sum_vec, sum_vec, data_vec
            
            ; Next chunk
            INC chunk_idx
            BR chunk_loop
            
            SYM chunk_done
            ; Horizontal sum of vector
            VAR TYPE_INT, chunk_sum
            VHSUM chunk_sum, sum_vec
            ADD result, result, chunk_sum
            
            ; Handle remainder elements individually
            VAR TYPE_INT, rem_idx, 0
            
            SYM rem_loop
            CMP rem_idx, remainder
            BR_GE rem_done
            
            ; Get element and add to result
            VAR TYPE_INT, offset, value
            MUL offset, chunks, 4
            ADD offset, offset, rem_idx
            MOV value, [array_ptr + offset*4]
            ADD result, result, value
            
            ; Next remainder element
            INC rem_idx
            BR rem_loop
            
            SYM rem_done
        ELSE
            ; x86-32 optimized implementation
            ; ...
        ENDIF
    ELIF ARCH == 0x02  ; ARM
        ; ARM-optimized implementation
        ; ...
    ELSE
        ; Generic implementation
        VAR TYPE_INT, result, 0
        VAR TYPE_INT, i, 0
        
        SYM generic_loop
        CMP i, count
        BR_GE generic_done
        
        ; Get array element
        VAR TYPE_INT, value
        MOV value, [array_ptr + i*4]
        
        ; Add to result
        ADD result, result, value
        
        ; Next element
        INC i
        BR generic_loop
        
        SYM generic_done
    ENDIF
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Foreign Function Interface

### Calling External C Functions

```
; Interface with C library functions
PROC 0x01                     ; CPU

SECTION .data, 0x02 | 0x04 | 0x08
SYM format_string
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, %s! The answer is %d.\n", 0

SECTION .text, 0x01 | 0x04
; External C function declaration
SYM printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Function using printf
SYM print_message, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_PTR, name
    VAR TYPE_INT, value
    MOV name, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV value, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Call C printf function
    CALL printf, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, format_string, name, value
    
    ; Get printf return value (number of characters printed)
    VAR TYPE_INT, chars_printed
    MOV chars_printed, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Return the character count
    RET TYPE_ABICTL=ABICTL_RET=platform_default, chars_printed
    SCOPEL

; Example usage
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_ARRAY=TYPE_UNT8, name, "World", 0
    VAR TYPE_INT, answer, 42
    
    CALL print_message, TYPE_ABICTL=ABICTL_PARAM=platform_default, name, answer
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    SCOPEL
```

### Creating COIL Functions Callable from C

```
; Function to be called from C
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Make function globally visible
GLOBAL calculate_factorial

; Function with C-compatible ABI
SYM calculate_factorial, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get parameter (using C ABI)
    VAR TYPE_INT, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    
    ; Initialize result
    VAR TYPE_INT, result, 1
    
    ; Calculate factorial
    VAR TYPE_INT, i, 1
    
    SYM fact_loop
    CMP i, n
    BR_GT fact_done
    
    MUL result, result, i
    INC i
    BR fact_loop
    
    SYM fact_done
    ; Return result using C ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    SCOPEL
```

## Advanced ABI Usage

### Combining Multiple ABIs in One Program

```
; Using multiple ABIs in one program
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Function using Linux ABI
SYM linux_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get parameter
    VAR TYPE_INT, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    
    ; Double the value
    MUL value, value, 2
    
    ; Return via Linux ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, value
    SCOPEL

; Function using Windows ABI
SYM windows_function, TYPE_ABICTL=ABICTL_STANDARD=windows_x64
    SCOPEE
    ; Get parameter
    VAR TYPE_INT, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=windows_x64, 0
    
    ; Triple the value
    MUL value, value, 3
    
    ; Return via Windows ABI
    RET TYPE_ABICTL=ABICTL_RET=windows_x64, value
    SCOPEL

; Main function using multiple ABIs
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR TYPE_INT, value, 10
    VAR TYPE_INT, linux_result, windows_result
    
    ; Call Linux ABI function
    CALL linux_function, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value
    MOV linux_result, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Call Windows ABI function
    CALL windows_function, TYPE_ABICTL=ABICTL_PARAM=windows_x64, value
    MOV windows_result, TYPE_ABICTL=ABICTL_RET=windows_x64
    
    ; Combine results
    VAR TYPE_INT, combined
    ADD combined, linux_result, windows_result
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, combined
    SCOPEL
```

## Conclusion

The examples in this document demonstrate the power and flexibility of COIL's function and ABI system, which provides:

1. **Processor Independence**: Functions can be written once and run on any supported processor
2. **ABI Abstraction**: No need to manually manage register allocation or parameter passing
3. **Type Safety**: Automatic type checking for function parameters and return values
4. **Interoperability**: Functions can interface with external code using appropriate ABIs
5. **Optimization**: Processor-specific optimizations can be applied while maintaining a common interface

By leveraging these capabilities, COIL enables development of portable, maintainable, and efficient code at the lowest level of programming.