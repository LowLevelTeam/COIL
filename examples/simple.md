# Simple COIL Examples

This document provides several basic examples of COIL code to demonstrate core language features, emphasizing COIL's variable system and ABI mechanism.

## Example 1: Hello World Function Using Variables and Explicit ABI

This example shows how to implement a simple function that prints "Hello, World!" to the console without direct register manipulation.

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

; Define our main function with linux_x86_64 ABI
SYM main, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    ; Enter scope for variables
    SCOPEE
    
    ; Declare variables
    VAR TYPE_PTR, string_ptr, hello_str
    VAR TYPE_UNT32, string_len
    VAR TYPE_UNT32, file_desc, 1  ; stdout
    
    ; Get string length using the linux_x86_64 ABI
    CALL strlen, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, string_ptr
    MOV string_len, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Write to stdout using syscall
    VAR TYPE_UNT32, syscall_num, 1  ; write syscall
    SYSCALL abi=linux_x86_64 (syscall_num, file_desc, string_ptr, string_len) ()
    
    ; Return 0
    VAR TYPE_INT32, return_code, 0
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, return_code
    
    SCOPEL

; String length function (using variables instead of direct register management)
SYM strlen, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    ; Enter scope
    SCOPEE
    
    ; Get input parameter from linux_x86_64 ABI
    VAR TYPE_PTR, str_ptr
    MOV str_ptr, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    
    ; Initialize counter
    VAR TYPE_UNT32, count, 0
    VAR TYPE_UNT8, current_char
    
SYM strlen_loop
    ; Load character
    MOV current_char, [str_ptr]
    
    ; Check if null terminator
    CMP current_char, 0
    BR_EQ strlen_done
    
    ; Increment pointer and counter
    ADD str_ptr, str_ptr, 1
    ADD count, count, 1
    
    BR strlen_loop
    
SYM strlen_done
    ; Return count using linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, count
    
    SCOPEL
```

## Example 2: 64-bit Integer Addition with Variables and Explicit ABI

This example shows a simple function that adds two 64-bit integers using variables instead of explicit registers.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define our add function with linux_x86_64 ABI
SYM add_integers, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    
    ; Get parameters via linux_x86_64 ABI
    VAR TYPE_INT64, a
    VAR TYPE_INT64, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0  ; First parameter
    MOV b, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1  ; Second parameter
    
    ; Add the integers
    VAR TYPE_INT64, result
    ADD result, a, b
    
    ; Return result via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    
    SCOPEL

; Example main function that uses our add function
SYM main, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    
    ; Declare variables for parameters
    VAR TYPE_INT64, value1, 123
    VAR TYPE_INT64, value2, 456
    
    ; Call add_integers with parameters via linux_x86_64 ABI
    CALL add_integers, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value1, value2
    
    ; Get result via linux_x86_64 ABI
    VAR TYPE_INT64, sum
    MOV sum, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, sum
    
    SCOPEL
```

## Example 3: Variable Declarations and Scope

This example demonstrates COIL's variable system and scope management.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Define our function with linux_x86_64 ABI
SYM calculate, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    ; Enter a new scope
    SCOPEE
    
    ; Declare variables
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    
    ; Calculate x * y + 5
    VAR TYPE_INT32, temp
    MUL temp, x, y
    
    VAR TYPE_INT32, result
    ADD result, temp, 5
    
    ; Return result via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    
    ; Leave the scope (cleans up variables)
    SCOPEL
```

## Example 4: Cross-Architecture Code

This example demonstrates how to write COIL code that can adapt to different architectures using the platform_default ABI.

```
; Set processor to CPU
PROC 0x01

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Platform-independent function using variables and platform_default ABI
SYM add_platform_integers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters (works on any architecture)
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Add integers (platform-independent)
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return result (works on any architecture)
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Example 5: Memory Operations with the Variable System

This example demonstrates memory operations using COIL's variable system.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

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

; Function to demonstrate memory operations with linux_x86_64 ABI
SYM memory_demo, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    
    ; Declare variables for buffer management
    VAR TYPE_PTR, src_ptr, source_buffer
    VAR TYPE_PTR, dst_ptr, dest_buffer
    VAR TYPE_UNT32, buffer_size, 100
    VAR TYPE_UNT32, string_len
    
    ; Get the length of the source string via linux_x86_64 ABI
    CALL strlen, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, src_ptr
    MOV string_len, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Copy memory
    MEMCPY dst_ptr, src_ptr, string_len
    
    ; Compare memory
    MEMCMP dst_ptr, src_ptr, string_len
    
    ; Zero out the destination
    MEMSET dst_ptr, 0, buffer_size
    
    SCOPEL
    
    ; Return via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64
```

## Example 6: Factorial Function with Variables and Explicit ABI

This example demonstrates a recursive function using the variable system and linux_x86_64 ABI.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Factorial function using variables and linux_x86_64 ABI
SYM factorial, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get input parameter via linux_x86_64 ABI
    VAR TYPE_UNT64, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    
    ; Base case check
    CMP n, 1
    BR_LE factorial_base_case
    
    ; Recursive case - prepare parameter for recursive call
    VAR TYPE_UNT64, n_minus_1
    SUB n_minus_1, n, 1
    
    ; Recursive call with automatic parameter passing via linux_x86_64 ABI
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, n_minus_1
    
    ; Get result from recursive call via linux_x86_64 ABI
    VAR TYPE_UNT64, result
    MOV result, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Multiply by n
    MUL result, result, n
    
    ; Return result via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    SCOPEL
    
factorial_base_case:
    ; Return 1 via linux_x86_64 ABI
    VAR TYPE_UNT64, result, 1
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    SCOPEL
```

## Example 7: Multiple ABI Usage

This example demonstrates using different ABIs in the same program.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Function using linux_x86_64 ABI
SYM linux_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get parameter via linux_x86_64 ABI
    VAR TYPE_INT32, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    
    ; Double the value
    MUL value, value, 2
    
    ; Return via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, value
    SCOPEL

; Function using windows_x64 ABI
SYM windows_function, TYPE_ABICTL=ABICTL_STANDARD=windows_x64
    SCOPEE
    ; Get parameter via windows_x64 ABI
    VAR TYPE_INT32, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=windows_x64, 0
    
    ; Triple the value
    MUL value, value, 3
    
    ; Return via windows_x64 ABI
    RET TYPE_ABICTL=ABICTL_RET=windows_x64, value
    SCOPEL

; Main function demonstrating different ABI calls
SYM main, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    VAR TYPE_INT32, value, 10
    VAR TYPE_INT32, linux_result
    VAR TYPE_INT32, windows_result
    
    ; Call linux_function using linux_x86_64 ABI
    CALL linux_function, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value
    MOV linux_result, TYPE_ABICTL=ABICTL_RET=linux_x86_64
    
    ; Call windows_function using windows_x64 ABI
    CALL windows_function, TYPE_ABICTL=ABICTL_PARAM=windows_x64, value
    MOV windows_result, TYPE_ABICTL=ABICTL_RET=windows_x64
    
    ; Process results (linux_result = 20, windows_result = 30)
    VAR TYPE_INT32, total
    ADD total, linux_result, windows_result
    
    ; Return via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, total
    SCOPEL
```

## Example 8: Type Data and Multiple Parameters

This example demonstrates the use of type data syntax and multiple parameters with ABIs.

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86-64
ARCH 0x01, 0x03  ; x86 in 64-bit mode

; Define text section
SECTION .text, 0x01 | 0x04  ; Executable and Readable

; Example of type data syntax
SYM process_data, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Declare variables with type data
    VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"  ; Type with data
    VAR TYPE_RGP=RAX, reg_ptr                 ; Register type with data
    VAR TYPE_PTR=TYPE_INT32, typed_ptr        ; Pointer with type data
    
    ; Get parameters from linux_x86_64 ABI
    VAR TYPE_INT32, param1
    VAR TYPE_INT32, param2
    MOV param1, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    MOV param2, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
    
    ; Process data
    VAR TYPE_INT32, result
    ADD result, param1, param2
    
    ; Return result via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
    SCOPEL

; Function returning multiple values via linux_x86_64 ABI
SYM div_with_remainder, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get parameters via linux_x86_64 ABI
    VAR TYPE_INT32, dividend
    VAR TYPE_INT32, divisor
    MOV dividend, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    MOV divisor, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
    
    ; Calculate quotient and remainder
    VAR TYPE_INT32, quotient
    DIV quotient, dividend, divisor
    
    VAR TYPE_INT32, remainder
    MOD remainder, dividend, divisor
    
    ; Return multiple values via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, quotient, remainder
    SCOPEL

; Calling function that returns multiple values
SYM use_multiple_returns, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    VAR TYPE_INT32, value, 42
    VAR TYPE_INT32, div, 5
    
    ; Call function with linux_x86_64 ABI
    CALL div_with_remainder, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value, div
    
    ; Get multiple return values via linux_x86_64 ABI
    VAR TYPE_INT32, quot
    VAR TYPE_INT32, rem
    MOV quot, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 0  ; First return value
    MOV rem, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 1   ; Second return value
    
    ; Use the results
    VAR TYPE_INT32, check
    MUL check, quot, div
    ADD check, check, rem
    
    ; Return via linux_x86_64 ABI
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, check
    SCOPEL
```

These examples demonstrate COIL's key advantages:
1. Variables abstract away register management
2. The ABI system handles parameter passing automatically with explicit ABI specification
3. Code can be written in an architecture-independent manner using platform_default ABI
4. Scopes provide structured variable lifetime management
5. Different ABIs can be used within the same program
6. Type data syntax provides rich type information