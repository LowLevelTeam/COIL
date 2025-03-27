# Control Flow Examples in CASM

## Conditional Branching

This example demonstrates different ways to implement conditional logic:

```
; Conditional branching example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM conditional_example
    SCOPEE
    VAR TYPE_INT32, value, 42
    VAR TYPE_INT32, threshold, 50
    VAR TYPE_INT32, result
    
    ; Basic if-then-else using compare and branch
    CMP value, threshold
    BR_GE greater_equal_branch
    
    ; "Then" case (value < threshold)
    MOV result, 0
    BR end_if
    
    SYM greater_equal_branch
    ; "Else" case (value >= threshold)
    MOV result, 1
    
    SYM end_if
    ; Code continues here regardless of which branch was taken
    
    SCOPEL
    RET
```

## Multiple Conditions (if-elif-else)

This example shows how to implement multiple conditional branches:

```
; Multiple conditions example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM multiple_conditions
    SCOPEE
    VAR TYPE_INT32, grade, 85
    VAR TYPE_ARRAY=TYPE_UNT8, result
    
    ; Check for A grade (90-100)
    CMP grade, 90
    BR_LT check_b_grade
    
    ; A grade
    MOV result, "A"
    BR end_checks
    
    SYM check_b_grade
    ; Check for B grade (80-89)
    CMP grade, 80
    BR_LT check_c_grade
    
    ; B grade
    MOV result, "B"
    BR end_checks
    
    SYM check_c_grade
    ; Check for C grade (70-79)
    CMP grade, 70
    BR_LT check_d_grade
    
    ; C grade
    MOV result, "C"
    BR end_checks
    
    SYM check_d_grade
    ; Check for D grade (60-69)
    CMP grade, 60
    BR_LT default_grade
    
    ; D grade
    MOV result, "D"
    BR end_checks
    
    SYM default_grade
    ; F grade (below 60)
    MOV result, "F"
    
    SYM end_checks
    ; Continue with rest of code
    
    SCOPEL
    RET
```

## Switch Statement

This example demonstrates the SWITCH instruction for multi-way branching:

```
; Switch statement example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM switch_example
    SCOPEE
    VAR TYPE_INT32, option, 2
    VAR TYPE_INT32, result
    
    ; Switch statement with 3 cases
    SWITCH option, 3
      1, case_one
      2, case_two
      3, case_three
      default_case
    
    SYM case_one
    MOV result, 100
    BR end_switch
    
    SYM case_two
    MOV result, 200
    BR end_switch
    
    SYM case_three
    MOV result, 300
    BR end_switch
    
    SYM default_case
    MOV result, 0
    
    SYM end_switch
    ; Code continues here
    
    SCOPEL
    RET
```

## Basic For Loop

This example shows a basic counter-based loop (equivalent to a for loop):

```
; For loop example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM for_loop
    SCOPEE
    VAR TYPE_INT32, i, 0
    VAR TYPE_INT32, sum, 0
    VAR TYPE_INT32, limit, 10
    
    SYM loop_start
    ; Check loop condition (i < limit)
    CMP i, limit
    BR_GE loop_end
    
    ; Loop body
    ADD sum, sum, i       ; sum += i
    
    ; Increment counter
    INC i                 ; i++
    BR loop_start         ; go back to start
    
    SYM loop_end
    ; Loop has finished, sum now contains 0+1+2+...+9 = 45
    
    SCOPEL
    RET
```

## While Loop

This example demonstrates a while loop that continues until a condition is met:

```
; While loop example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM while_loop
    SCOPEE
    VAR TYPE_INT32, num, 100
    VAR TYPE_INT32, count, 0
    
    SYM while_start
    ; Check condition (num > 0)
    CMP num, 0
    BR_LE while_end
    
    ; Loop body
    INC count             ; count++
    DIV num, num, 2       ; num /= 2
    BR while_start
    
    SYM while_end
    ; Loop has finished, count contains the number of divisions
    ; needed to get num below or equal to 0 (log₂ approximation)
    
    SCOPEL
    RET
```

## Do-While Loop

This example shows a do-while loop (executes at least once):

```
; Do-while loop example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM do_while_loop
    SCOPEE
    VAR TYPE_INT32, num, 5
    VAR TYPE_INT32, factorial, 1
    
    SYM do_start
    ; Loop body (executed at least once)
    MUL factorial, factorial, num    ; factorial *= num
    DEC num                          ; num--
    
    ; Check condition (num > 0)
    CMP num, 0
    BR_GT do_start
    
    ; Loop has finished, factorial contains 5! = 120
    
    SCOPEL
    RET
```

## Early Loop Exit (break)

This example demonstrates how to exit a loop early when a condition is met:

```
; Early loop exit example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM early_exit
    SCOPEE
    VAR TYPE_INT32, i, 0
    VAR TYPE_INT32, found, 0
    VAR TYPE_INT32, target, 42
    VAR TYPE_ARRAY=TYPE_INT32, array, (10, 25, 42, 67, 89)
    VAR TYPE_INT32, length, 5
    
    SYM search_loop
    CMP i, length
    BR_GE search_end
    
    ; Check if current element matches target
    VAR TYPE_INT32, current
    INDEX current, array, i
    
    CMP current, target
    BR_NE continue_search
    
    ; Found the target value
    MOV found, 1
    BR search_end          ; Early exit (break)
    
    SYM continue_search
    INC i
    BR search_loop
    
    SYM search_end
    ; Loop has exited, found indicates whether target was in the array
    
    SCOPEL
    RET
```

## Loop Continue Pattern

This example shows how to skip the rest of a loop iteration (continue):

```
; Loop continue example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM continue_example
    SCOPEE
    VAR TYPE_INT32, i, 0
    VAR TYPE_INT32, sum, 0
    VAR TYPE_INT32, limit, 10
    
    SYM loop_start
    CMP i, limit
    BR_GE loop_end
    
    INC i                  ; i++
    
    ; Skip odd numbers (continue if i is odd)
    MOD temp, i, 2
    CMP temp, 0
    BR_NE loop_start       ; Continue if odd
    
    ; This code only executes for even values
    ADD sum, sum, i        ; sum += i (even numbers only)
    BR loop_start
    
    SYM loop_end
    ; Loop has finished, sum contains 2+4+6+8+10 = 30
    
    SCOPEL
    RET
```

## Nested Loops

This example demonstrates nested loops:

```
; Nested loops example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM nested_loops
    SCOPEE
    VAR TYPE_INT32, i, 0
    VAR TYPE_INT32, j, 0
    VAR TYPE_INT32, sum, 0
    VAR TYPE_INT32, outer_limit, 5
    VAR TYPE_INT32, inner_limit, 3
    
    SYM outer_loop_start
    CMP i, outer_limit
    BR_GE outer_loop_end
    
    ; Reset j for each iteration of the outer loop
    MOV j, 0
    
    SYM inner_loop_start
    CMP j, inner_limit
    BR_GE inner_loop_end
    
    ; Inner loop body
    ADD sum, sum, 1       ; sum += 1
    
    INC j
    BR inner_loop_start
    
    SYM inner_loop_end
    ; Inner loop has finished for this i
    
    INC i
    BR outer_loop_start
    
    SYM outer_loop_end
    ; Both loops have finished, sum contains 5*3 = 15
    
    SCOPEL
    RET
```

## Recursive Factorial

This example shows recursion using the factorial function:

```
; Recursive factorial example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Factorial function: factorial(n) = n * factorial(n-1)
SYM factorial, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter
    VAR TYPE_UNT32, n
    MOV n, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base case: factorial(0) or factorial(1) = 1
    CMP n, 1
    BR_GT recursive_case
    
    ; Base case
    VAR TYPE_UNT32, result, 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    
    SYM recursive_case
    ; Compute factorial(n-1)
    VAR TYPE_UNT32, n_minus_1
    SUB n_minus_1, n, 1
    
    ; Recursive call
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, n_minus_1
    
    ; Get result of recursive call
    VAR TYPE_UNT32, sub_result
    MOV sub_result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Compute n * factorial(n-1)
    VAR TYPE_UNT32, result
    MUL result, n, sub_result
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Main function to call factorial
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_UNT32, number, 5
    
    ; Call factorial function
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, number
    
    ; Get result
    VAR TYPE_UNT32, result
    MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Result now contains 5! = 120
    
    SCOPEL
    RET
```