# Basic COIL Examples

## Purpose

This document provides fundamental examples of COIL code to demonstrate core language features and simple programs, focusing on basic concepts and syntax.

## Hello World Example

```
; Hello World in COIL-ASM
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
    SYSCALL linux_x86_64 (syscall_write, stdout_fd, message, message_len) ()
    
    ; Exit with code 0
    VAR TYPE_UNT64, syscall_exit, 60
    VAR TYPE_UNT64, exit_code, 0
    SYSCALL linux_x86_64 (syscall_exit, exit_code) ()
    SCOPEL
```

## Variable Declaration and Arithmetic

```
; Basic variable declaration and arithmetic operations
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
    
    ; Increment
    INC a                     ; a += 1
    
    ; Decrement
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
    VAR TYPE_PTR, message1, "Value is greater than or equal to threshold"
    ; ... handle this case
    BR end_condition
    
    SYM less_than_branch
    ; Code for value < threshold
    VAR TYPE_PTR, message2, "Value is less than threshold"
    ; ... handle this case
    
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
    
    ; Loop body
    ; ... operations here
    
    INC i
    BR for_loop_start
    SYM for_loop_end
    
    ; While loop equivalent
    VAR TYPE_INT32, counter, 5
    
    SYM while_loop_start
    CMP counter, 0
    BR_LE while_loop_end
    
    ; Loop body
    ; ... operations here
    
    DEC counter
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
    
    ; Load using indexed addressing
    VAR TYPE_UNT8, fifth_byte
    MOV fifth_byte, [data_ptr + 4]
    
    ; Memory block operations
    VAR TYPE_PTR, dest_ptr, buffer
    ADD dest_ptr, dest_ptr, 20   ; Offset by 20 bytes
    VAR TYPE_UNT64, block_size, 10
    
    ; Copy memory
    MEMCPY dest_ptr, data_ptr, block_size
    
    ; Fill memory
    VAR TYPE_UNT8, fill_value, 'A'
    MEMSET dest_ptr, fill_value, block_size
    
    ; Compare memory
    VAR TYPE_INT32, comparison_result
    MEMCMP comparison_result, data_ptr, dest_ptr, 5
    
    SCOPEL
    RET
```

## Bit Manipulation

```
; Bit manipulation examples
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM bit_example
    SCOPEE
    VAR TYPE_UNT32, flags, 0x0F   ; 0000 1111
    VAR TYPE_UNT32, mask, 0xF0    ; 1111 0000
    VAR TYPE_UNT32, result
    
    ; Bitwise AND
    AND result, flags, mask       ; 0000 0000
    
    ; Bitwise OR
    OR result, flags, mask        ; 1111 1111
    
    ; Bitwise XOR
    XOR result, flags, mask       ; 1111 1111
    
    ; Bitwise NOT
    NOT result, flags             ; 1111 0000
    
    ; Shifts
    VAR TYPE_UNT32, shift_amount, 4
    
    ; Shift left
    SHL result, flags, shift_amount  ; 0000 1111 -> 1111 0000
    
    ; Shift right (logical)
    SHR result, flags, shift_amount  ; 0000 1111 -> 0000 0000
    
    ; Test bit in a value
    VAR TYPE_UNT32, bit_mask, 0x08   ; 0000 1000 (bit 3)
    TEST flags, bit_mask              ; Sets flags based on result
    
    ; Bit manipulation pattern: Set bit 5
    OR flags, flags, 0x20             ; Set bit 5
    
    ; Bit manipulation pattern: Clear bit 4
    AND flags, flags, 0xFFFFFFEF      ; Clear bit 4 (~0x10)
    
    ; Bit manipulation pattern: Toggle bit 6
    XOR flags, flags, 0x40            ; Toggle bit 6
    
    SCOPEL
    RET
```

## Simple Function with Parameters

```
; Simple function example
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
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Declare local variables
    VAR TYPE_INT32, x, 5
    VAR TYPE_INT32, y, 10
    VAR TYPE_INT32, result
    
    ; Call function
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get return value
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Return with success code
    VAR TYPE_INT32, status, 0
    RET TYPE_ABICTL=ABICTL_RET=platform_default, status
    SCOPEL
```

## Basic Data Structures

```
; Basic data structure examples
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM data_structure_example
    SCOPEE
    ; Define a structure type
    STRUCT TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y
    
    ; Create an instance of the structure
    VAR TYPE_STRUCT=point, position, 10, 20
    
    ; Access structure fields
    VAR TYPE_INT32, x_value
    GET x_value, position, x
    
    ; Modify structure fields
    SET position, y, 30
    
    ; Array example
    VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
    
    ; Access array element
    VAR TYPE_INT32, third_element
    INDEX third_element, numbers, 2   ; Zero-based indexing
    
    ; Update array element
    UPDT numbers, 1, 42
    
    SCOPEL
    RET
```

## Switch Statement

```
; Switch statement example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM switch_example
    SCOPEE
    VAR TYPE_INT32, option, 2
    
    ; Switch on the option value
    SWITCH option, 3
      1, case_one
      2, case_two
      3, case_three
      default_case
    
    SYM case_one
    ; Handle option 1
    VAR TYPE_PTR, msg1, "Option 1 selected"
    BR switch_end
    
    SYM case_two
    ; Handle option 2
    VAR TYPE_PTR, msg2, "Option 2 selected"
    BR switch_end
    
    SYM case_three
    ; Handle option 3
    VAR TYPE_PTR, msg3, "Option 3 selected"
    BR switch_end
    
    SYM default_case
    ; Handle any other option
    VAR TYPE_PTR, msg_default, "Unknown option selected"
    
    SYM switch_end
    SCOPEL
    RET
```

## Type Conversion

```
; Type conversion examples
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM type_conversion_example
    SCOPEE
    ; Integer to float conversion
    VAR TYPE_INT32, int_value, 42
    VAR TYPE_FP32, float_result
    
    CONVERT float_result, int_value
    
    ; Float to integer conversion (truncates toward zero)
    VAR TYPE_FP32, pi, 3.14159
    VAR TYPE_INT32, int_pi
    
    CONVERT int_pi, pi
    
    ; Bit pattern reinterpretation (doesn't change value representation)
    VAR TYPE_FP32, float_bits, 1.0
    VAR TYPE_UNT32, bit_pattern
    
    CAST bit_pattern, float_bits
    
    ; Size extension
    VAR TYPE_INT32, small_int, 100
    VAR TYPE_INT64, big_int
    
    CONVERT big_int, small_int
    
    ; Type information
    VAR TYPE_UNT64, size_of_int
    SIZEOF size_of_int, TYPE_INT32
    
    SCOPEL
    RET
```