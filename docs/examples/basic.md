# Basic COIL Examples

## Purpose

This document provides fundamental examples of COIL code to demonstrate core language features and simple programs. These examples serve as reference implementations for basic COIL programming patterns.

## Hello World Example

```
; Hello World in COIL
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB
    SCOPEE
    ; Declare variables for syscall
    VAR TYPE_UNT64, syscall_write, 1
    VAR TYPE_UNT64, stdout_fd, 1
    VAR TYPE_PTR, message, hello_msg
    VAR TYPE_UNT64, message_len, 14
    
    ; Write to stdout using syscall
    MOV TYPE_RGP=RAX, syscall_write
    MOV TYPE_RGP=RDI, stdout_fd
    MOV TYPE_RGP=RSI, message
    MOV TYPE_RGP=RDX, message_len
    SYSCALL
    
    ; Exit with code 0
    VAR TYPE_UNT64, syscall_exit, 60
    VAR TYPE_UNT64, exit_code, 0
    MOV TYPE_RGP=RAX, syscall_exit
    MOV TYPE_RGP=RDI, exit_code
    SYSCALL
    SCOPEL
```

## Variable Declaration and Arithmetic

```
; Basic variable declaration and arithmetic
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM arithmetic_example
    SCOPEE
    ; Integer variables with initialization
    VAR TYPE_INT32, a, 10
    VAR TYPE_INT32, b, 20
    VAR TYPE_INT32, result
    
    ; Addition
    ADD result, a, b          ; result = a + b
    
    ; Subtraction
    SUB result, a, b          ; result = a - b
    
    ; Multiplication
    MUL result, a, b          ; result = a * b
    
    ; Division
    DIV result, a, b          ; result = a / b
    
    ; Modulo (remainder)
    MOD result, a, b          ; result = a % b
    
    ; Increment and decrement
    INC a                     ; a += 1
    DEC b                     ; b -= 1
    
    SCOPEL
    RET
```

## Conditional Logic

```
; Conditional branching example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM conditional_example
    SCOPEE
    VAR TYPE_INT32, value, 42
    VAR TYPE_INT32, threshold, 50
    
    ; Compare values
    CMP value, threshold
    
    ; Branch if less than
    BR_LT less_than_branch
    
    ; Code for value >= threshold
    VAR TYPE_INT32, result1, 1
    BR end_condition
    
    SYM less_than_branch
    ; Code for value < threshold
    VAR TYPE_INT32, result2, 0
    
    SYM end_condition
    SCOPEL
    RET
```

## Loops

```
; Loop examples
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM loop_examples
    SCOPEE
    ; Simple counted loop (for loop equivalent)
    VAR TYPE_INT32, i, 0
    VAR TYPE_INT32, limit, 10
    
    SYM for_loop_start
    CMP i, limit
    BR_GE for_loop_end
    
    ; Loop body here
    ADD i, i, 1               ; i++
    BR for_loop_start
    SYM for_loop_end
    
    ; While loop equivalent
    VAR TYPE_INT32, counter, 5
    
    SYM while_loop_start
    CMP counter, 0
    BR_LE while_loop_end
    
    ; Loop body here
    DEC counter               ; counter--
    BR while_loop_start
    SYM while_loop_end
    
    SCOPEL
    RET
```

## Memory Operations

```
; Memory operation examples
PROC 0x01                     ; CPU

SECTION .data, 0x02 | 0x04 | 0x08
SYM buffer
DATA TYPE_ARRAY=TYPE_UNT8, "Initial data", 0

SECTION .text, 0x01 | 0x04
SYM memory_example
    SCOPEE
    ; Pointer variable
    VAR TYPE_PTR, data_ptr, buffer
    
    ; Direct memory access
    VAR TYPE_UNT8, first_byte
    MOV first_byte, [data_ptr]
    
    ; Store to memory
    MOV [data_ptr], 'M'
    
    ; Access using indexed addressing
    VAR TYPE_UNT8, fifth_byte
    MOV fifth_byte, [data_ptr + 4]
    
    ; Memory block operations
    VAR TYPE_PTR, dest_ptr
    ADD dest_ptr, data_ptr, 20   ; Offset by 20 bytes
    VAR TYPE_UNT64, block_size, 10
    
    ; Copy memory
    MEMCPY dest_ptr, data_ptr, block_size
    
    ; Fill memory
    MEMSET dest_ptr, 0, block_size
    
    SCOPEL
    RET
```

## Function Calls and Returns

```
; Function call example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Function to add two numbers
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Calculate sum
    VAR TYPE_INT32, sum
    ADD sum, a, b
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, sum
    SCOPEL

; Main function
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_INT32, x, 5
    VAR TYPE_INT32, y, 10
    VAR TYPE_INT32, result
    
    ; Call function
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get return value
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    SCOPEL
    RET
```

## Related Documentation

- [CASM Syntax](../spec/assembly/syntax.md) - Fundamental syntax rules for CASM
- [Memory Operations](../spec/instruction-set/memory.md) - Details on memory operations
- [Function Examples](functions.md) - More advanced function examples
- [Control Flow Instructions](../spec/instruction-set/control-flow.md) - Details on branching and control flow