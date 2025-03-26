# Basic COIL Examples

This document provides fundamental examples of COIL code to demonstrate core language features, focused on COIL's variable system and ABI mechanism.

## Hello World Example

```
; Hello World in COIL-ASM using variables and ABI
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=0x02
    SCOPEE
    ; Declare variables for syscall
    VAR TYPE_UNT64, syscall_write, 1
    VAR TYPE_UNT64, stdout_fd, 1
    VAR TYPE_PTR, message, hello_msg
    VAR TYPE_UNT64, message_len, 14
    
    ; Write to stdout using syscall
    SYSCALL linux_x86_64 (syscall_write, stdout_fd, message, message_len) ()
    
    ; Exit with code 0
    VAR TYPE_UNT64, syscall_exit, 60
    VAR TYPE_UNT64, exit_code, 0
    SYSCALL linux_x86_64 (syscall_exit, exit_code) ()
    SCOPEL
```

## Variable Declaration and Usage

```
; Demonstrate variable declaration and usage
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM variable_example
    SCOPEE
    ; Integer variables with initialization
    VAR TYPE_INT32, counter, 0
    VAR TYPE_INT64, big_number, 1000000
    
    ; Floating point variables
    VAR TYPE_FP32, pi, 3.14159
    VAR TYPE_FP64, e, 2.71828
    
    ; Pointer variable with initialization to NULL
    VAR TYPE_PTR, ptr, 0
    
    ; Using variables in operations
    ADD counter, counter, 1
    MUL big_number, big_number, 2
    
    ; Variables with complex types
    VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
    
    ; Nested scope example
    SCOPEE
        VAR TYPE_INT32, temp, 42
        ADD counter, counter, temp
    SCOPEL  ; temp is destroyed here
    
    SCOPEL  ; All other variables destroyed here
    RET
```

## Basic Arithmetic and Control Flow

```
; Demonstrate arithmetic and control flow
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM calculate_factorial, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get the input parameter
    VAR TYPE_UNT64, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Initialize result
    VAR TYPE_UNT64, result, 1
    
    ; Check for base case
    CMP n, 1
    BR_LE factorial_done
    
    ; Set up loop counter
    VAR TYPE_UNT64, i, 2
    
    SYM factorial_loop
    ; Check loop condition
    CMP i, n
    BR_GT factorial_done
    
    ; Multiply result by i
    MUL result, result, i
    
    ; Increment counter
    ADD i, i, 1
    
    ; Loop back
    BR factorial_loop
    
    SYM factorial_done
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    
    SCOPEL
```

## Memory Operations

```
; Demonstrate memory operations
PROC 0x01                     ; CPU

SECTION .data, 0x02 | 0x04 | 0x08
SYM source_data
DATA TYPE_ARRAY=TYPE_UNT8, "Source data for memory operations", 0

SECTION .bss, 0x02 | 0x04 | 0x10
SYM buffer
DATA TYPE_ARRAY=TYPE_UNT8[100]  ; 100 bytes of uninitialized data

SECTION .text, 0x01 | 0x04
SYM memory_example
    SCOPEE
    ; Declare pointer variables
    VAR TYPE_PTR, src_ptr, source_data
    VAR TYPE_PTR, dst_ptr, buffer
    VAR TYPE_UNT64, size, 32
    
    ; Copy memory from source to buffer
    MEMCPY dst_ptr, src_ptr, size
    
    ; Set part of the buffer to zeros
    VAR TYPE_PTR, zero_start
    ADD zero_start, dst_ptr, size
    VAR TYPE_UNT8, fill_value, 0
    VAR TYPE_UNT64, fill_size, 32
    MEMSET zero_start, fill_value, fill_size
    
    ; Compare memory regions
    MEMCMP dst_ptr, src_ptr, size
    
    ; Access byte at specific offset
    VAR TYPE_UNT64, offset, 5
    VAR TYPE_PTR, byte_ptr
    ADD byte_ptr, dst_ptr, offset
    
    VAR TYPE_UNT8, byte_value
    MOV byte_value, [byte_ptr]
    
    SCOPEL
    RET
```

## Function Calls with ABI

```
; Demonstrate function calls with ABI
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Function that adds two numbers
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Add the numbers
    VAR TYPE_INT32, sum
    ADD sum, a, b
    
    ; Return the result via ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, sum
    SCOPEL

; Function that multiplies two numbers
SYM multiply_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Multiply the numbers
    VAR TYPE_INT32, product
    MUL product, a, b
    
    ; Return the result via ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, product
    SCOPEL

; Main function that calls the others
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Declare variables
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    VAR TYPE_INT32, sum
    VAR TYPE_INT32, product
    
    ; Call add_numbers with parameters
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get the return value
    MOV sum, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Call multiply_numbers with parameters
    CALL multiply_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get the return value
    MOV product, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Calculate final result (sum + product)
    VAR TYPE_INT32, result
    ADD result, sum, product
    
    ; Return final result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Bit Manipulation

```
; Demonstrate bit manipulation operations
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM bit_operations
    SCOPEE
    ; Declare variables
    VAR TYPE_UNT32, a, 0x0F0F  ; 0000 1111 0000 1111
    VAR TYPE_UNT32, b, 0xF0F0  ; 1111 0000 1111 0000
    VAR TYPE_UNT32, result
    
    ; Bitwise AND
    AND result, a, b
    ; result = 0x0000 (0000 0000 0000 0000)
    
    ; Bitwise OR
    OR result, a, b
    ; result = 0xFFFF (1111 1111 1111 1111)
    
    ; Bitwise XOR
    XOR result, a, b
    ; result = 0xFFFF (1111 1111 1111 1111)
    
    ; Bitwise NOT
    NOT result, a
    ; result = 0xFFF0F0F0 (1111 1111 1111 0000 1111 0000 1111 0000)
    
    ; Shift operations
    VAR TYPE_UNT32, shift_amount, 4
    
    ; Shift left
    SHL result, a, shift_amount
    ; result = 0x0F0F0 (0000 1111 0000 1111 0000)
    
    ; Logical shift right
    SHR result, a, shift_amount
    ; result = 0x00F0 (0000 0000 1111 0000)
    
    ; Arithmetic shift right (same as logical for unsigned)
    SAR result, a, shift_amount
    ; result = 0x00F0 (0000 0000 1111 0000)
    
    ; Rotate operations
    VAR TYPE_UNT32, rotate_amount, 8
    
    ; Rotate left
    ROL result, a, rotate_amount
    ; result = 0x0F0F00 (0000 1111 0000 1111 0000 0000)
    
    ; Rotate right
    ROR result, a, rotate_amount
    ; result = 0x0F0F00 (0000 1111 0000 1111 0000 0000)
    
    ; Bit counting
    POPCNT result, a
    ; result = 8 (number of set bits in a)
    
    ; Byte swap (endianness conversion)
    BSWAP result, a
    ; result = 0x0F0F0000 (0000 1111 0000 1111 0000 0000 0000 0000)
    
    SCOPEL
    RET
```

## Conditional Execution

```
; Demonstrate conditional execution
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM conditional_example, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter
    VAR TYPE_INT32, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Declare result
    VAR TYPE_INT32, result, 0
    
    ; Compare value with zero
    CMP value, 0
    
    ; Conditional operations based on comparison
    MOV_GT result, 1     ; Move 1 to result if value > 0
    MOV_LT result, -1    ; Move -1 to result if value < 0
    ; result remains 0 if value == 0
    
    ; Example with conditional branch
    CMP value, 10
    BR_GE value_large    ; Branch if value >= 10
    
    ; Code for small values
    MUL result, result, 2
    BR done
    
    SYM value_large
    ; Code for large values
    MUL result, result, 10
    
    SYM done
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    
    SCOPEL
```

## Type Operations

```
; Demonstrate type operations
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM type_operations
    SCOPEE
    ; Declare variables of different types
    VAR TYPE_INT32, int_value, 42
    VAR TYPE_FP32, float_value, 3.14
    VAR TYPE_UNT64, large_value, 1000000
    
    ; Get type information
    VAR TYPE_UNT64, type_id
    TYPEOF type_id, int_value
    
    ; Get size information
    VAR TYPE_UNT64, int_size
    SIZEOF int_size, TYPE_INT32
    
    ; Get alignment information
    VAR TYPE_UNT64, fp_align
    ALIGNOF fp_align, TYPE_FP64
    
    ; Type conversion
    VAR TYPE_FP32, float_result
    CONVERT float_result, int_value
    
    ; Type casting (bit pattern reinterpretation)
    VAR TYPE_UNT32, bits
    CAST bits, float_value
    
    ; Define a structure type
    STRUCT point, TYPE_INT32, x, TYPE_INT32, y
    
    ; Create a variable of structure type
    VAR point, position, 10, 20
    
    ; Access structure field
    VAR TYPE_INT32, x_value
    GET x_value, position, x
    
    ; Define an array
    VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
    
    ; Access array element
    VAR TYPE_INT32, index, 2
    VAR TYPE_INT32, element
    INDEX element, numbers, index
    
    SCOPEL
    RET
```

## Platform Independent Function

```
; Demonstrate processor/architecture independence
PROC 0x01                     ; Set to CPU for v1, any processor in v3

SECTION .text, 0x01 | 0x04
; This function will work on any processor supported by COIL
SYM portable_add, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters (works on any platform)
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Add the values (portable operation)
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return the result (works on any platform)
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; This function demonstrates conditional compilation for platform-specific optimizations
SYM optimized_add, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Result variable
    VAR TYPE_INT, result
    
    ; Platform-specific optimizations
    IF ARCH == 0x01  ; x86
      ; x86-specific fast path
      ADD result, a, b
    ELIF ARCH == 0x02  ; ARM
      ; ARM-specific fast path
      ADD result, a, b
    ELSE
      ; Generic implementation for other platforms
      ADD result, a, b
    ENDIF
    
    ; Return the result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```