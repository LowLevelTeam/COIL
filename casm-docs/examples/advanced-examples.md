# Advanced CASM Examples

## Control Flow

This example demonstrates different control flow constructs in CASM.

```
; Control flow example
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
SYM control_flow_example
    SCOPEE
    VAR #1, TYPE_INT32, 42    ; value
    VAR #2, TYPE_INT32, 50    ; threshold
    VAR #3, TYPE_INT32        ; result
    
    ; Basic if-then-else
    CMP #1, #2
    BR_GE greater_equal_branch
    
    ; "Then" case (value < threshold)
    MOV #3, 0
    BR end_if
    
    SYM greater_equal_branch
    ; "Else" case (value >= threshold)
    MOV #3, 1
    
    SYM end_if
    
    ; For loop example
    VAR #4, TYPE_INT32, 0     ; i = 0
    VAR #5, TYPE_INT32, 0     ; sum = 0
    VAR #6, TYPE_INT32, 5     ; limit = 5
    
    SYM loop_start
    CMP #4, #6                ; Check if i < limit
    BR_GE loop_end
    
    ADD #5, #5, #4            ; sum += i
    INC #4                    ; i++
    BR loop_start
    
    SYM loop_end
    ; sum now contains 0+1+2+3+4 = 10
    
    ; While loop with early exit (break)
    VAR #7, TYPE_INT32, 100   ; num = 100
    VAR #8, TYPE_INT32, 0     ; count = 0
    
    SYM while_start
    CMP #7, 0                 ; Check if num > 0
    BR_LE while_end
    
    INC #8                    ; count++
    DIV #7, #7, 2             ; num /= 2
    
    CMP #8, 5                 ; Example early exit
    BR_GE while_end           ; Break if count >= 5
    
    BR while_start
    
    SYM while_end
    
    ; Switch statement example
    VAR #9, TYPE_INT32, 2     ; option
    VAR #10, TYPE_INT32       ; switch_result
    
    SWITCH #9, 3              ; Switch with 3 cases
      1, case_one
      2, case_two
      3, case_three
      default_case
    
    SYM case_one
    MOV #10, 100
    BR end_switch
    
    SYM case_two
    MOV #10, 200
    BR end_switch
    
    SYM case_three
    MOV #10, 300
    BR end_switch
    
    SYM default_case
    MOV #10, 0
    
    SYM end_switch
    ; #10 now contains 200 (case_two)
    
    SCOPEL
    RET
```

### Key Concepts Demonstrated
- If-then-else branching
- For loops
- While loops with early exit
- Switch statements
- Conditional branching

## Functions and Parameters

This example demonstrates function definition, parameter passing, and calling.

```
; Function examples
PROC 0x01                     ; CPU

SECTION .text, 0x01 | 0x04
; Function to add two numbers
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR #1, TYPE_INT32        ; a
    VAR #2, TYPE_INT32        ; b
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Calculate sum
    VAR #3, TYPE_INT32
    ADD #3, #1, #2
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    SCOPEL

; Function with multiple return values
SYM divmod, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR #1, TYPE_INT32        ; dividend
    VAR #2, TYPE_INT32        ; divisor
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Calculate quotient and remainder
    VAR #3, TYPE_INT32        ; quotient
    VAR #4, TYPE_INT32        ; remainder
    DIV #3, #1, #2
    MOD #4, #1, #2
    
    ; Return both values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3, #4
    SCOPEL

; Recursive function (factorial)
SYM factorial, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameter
    VAR #1, TYPE_UNT32        ; n
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Base case: factorial(0) or factorial(1) = 1
    CMP #1, 1
    BR_GT recursive_case
    
    ; Base case
    VAR #2, TYPE_UNT32, 1     ; result = 1
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #2
    
    SYM recursive_case
    ; Compute factorial(n-1)
    VAR #3, TYPE_UNT32        ; n_minus_1
    SUB #3, #1, 1
    
    ; Recursive call
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, #3
    
    ; Get result of recursive call
    VAR #4, TYPE_UNT32
    MOV #4, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Compute n * factorial(n-1)
    VAR #5, TYPE_UNT32
    MUL #5, #1, #4
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #5
    SCOPEL

; Main function that calls the others
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_INT32, 10    ; x = 10
    VAR #2, TYPE_INT32, 20    ; y = 20
    VAR #3, TYPE_INT32        ; sum
    
    ; Call add_numbers
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2
    MOV #3, TYPE_ABICTL=ABICTL_RET=platform_default
    ; #3 now contains 30
    
    ; Call divmod
    VAR #4, TYPE_INT32, 17    ; a = 17
    VAR #5, TYPE_INT32, 5     ; b = 5
    CALL divmod, TYPE_ABICTL=ABICTL_PARAM=platform_default, #4, #5
    
    ; Get both return values
    VAR #6, TYPE_INT32        ; quotient
    VAR #7, TYPE_INT32        ; remainder
    MOV #6, TYPE_ABICTL=ABICTL_RET=platform_default, 0
    MOV #7, TYPE_ABICTL=ABICTL_RET=platform_default, 1
    ; #6 = 3, #7 = 2 (17 = 3*5 + 2)
    
    ; Call factorial
    VAR #8, TYPE_UNT32, 5     ; n = 5
    CALL factorial, TYPE_ABICTL=ABICTL_PARAM=platform_default, #8
    
    ; Get factorial result
    VAR #9, TYPE_UNT32        ; result
    MOV #9, TYPE_ABICTL=ABICTL_RET=platform_default
    ; #9 now contains 120 (5!)
    
    SCOPEL
    RET
```

### Key Concepts Demonstrated
- Function definition
- Parameter passing
- Return values
- Multiple return values
- Recursive functions
- Function calls
- ABI usage