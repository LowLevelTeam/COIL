# COIL Variable System

## Purpose

This document defines the COIL variable system, a key feature that abstracts over processor registers and memory, providing architecture-independent variables with automatic management of storage allocation, lifetime, and optimization.

## Key Concepts

### Variables vs. Registers

Traditional assembly languages require programmers to manage registers manually:
- Decide which registers to use for each value
- Track register usage across function calls
- Save and restore registers around function calls
- Manage register spilling when running out of registers

COIL's variable system abstracts away these details:
- Variables are declared with types
- The COIL processor decides where to store each variable
- Variables are automatically allocated to registers or memory
- The processor handles register saving and restoration
- Register spilling happens automatically when needed

### Scope-Based Lifetime

Variables in COIL have explicit lifetimes based on lexical scopes:
- Variables exist from declaration until the end of their scope
- Nested scopes allow for organized variable management
- When a scope ends, all variables in that scope are automatically deallocated
- Resources (registers or memory) are automatically freed when variables go out of scope

### Optimization

The COIL processor can optimize variable storage based on usage patterns:
- Frequently used variables can be kept in registers
- Infrequently used variables can be stored in memory
- Register allocation can be prioritized for performance-critical code
- Variable promotion and demotion allows fine-grained optimization when needed

## Variable Declaration

Variables are declared using the `VAR` instruction:

```
VAR type, name [, initial_value]
```

Where:
- `type` is any valid COIL type
- `name` is a symbolic identifier for the variable
- `initial_value` is an optional initialization value

Examples:
```
VAR TYPE_INT32, counter, 0      ; 32-bit integer initialized to 0
VAR TYPE_FP64, pi, 3.14159      ; 64-bit float initialized to pi
VAR TYPE_PTR, data_pointer      ; Pointer variable, uninitialized
```

## Variable Scoping

Variables exist within lexical scopes defined by the `SCOPEE` and `SCOPEL` instructions:

```
SCOPEE
    ; Variables declared here...
    VAR TYPE_INT32, local_var, 0
    
    ; Variable can be used here...
    ADD local_var, local_var, 1
    
    ; Nested scopes are supported
    SCOPEE
        VAR TYPE_INT32, inner_var, 42
        ; Both local_var and inner_var are accessible here
    SCOPEL  ; inner_var is destroyed here
    
    ; Only local_var is accessible here
SCOPEL  ; local_var is destroyed here
```

Scoping rules:
1. Variables exist from their declaration until the end of their enclosing scope
2. Inner scopes can access variables from outer scopes
3. Variables with the same name in inner scopes shadow outer variables
4. When a scope ends, all variables declared in that scope are deallocated

## Variable Usage

Once declared, variables can be used directly in instructions without specifying registers:

```
; Declare variables
VAR TYPE_INT32, counter, 0
VAR TYPE_INT32, max_value, 100

; Use variables directly
ADD counter, counter, 1
CMP counter, max_value
BR_LT loop_start
```

The COIL processor automatically:
1. Allocates appropriate storage for each variable (register or memory)
2. Generates the correct code to access the variables
3. Handles any necessary register spilling or reloading
4. Optimizes access patterns based on usage

## Variable Reference

To reference a variable in COIL, simply use its name:

```
; Direct variable reference
ADD counter, counter, 1      ; Use variable directly

; Getting the address of a variable
LEA addr_var, counter        ; Load effective address of counter

; Using the address
MOV [addr_var], 42           ; Store value at the address
```

Variables are represented internally using the `TYPE_VAR` type with a unique identifier for each variable.

## Variable Optimization

### Promotion and Demotion

COIL provides hints for variable optimization through the `PUSH` and `POP` instructions:

```
; Demote a variable (hint to store in memory)
PUSH counter           ; Suggest storing counter in memory

; Promote a variable (hint to store in register)
POP counter            ; Suggest loading counter into a register
```

These operations serve dual purposes:
1. As normal stack operations when used with values
2. As optimization hints when used with variables

### Register Hints

For performance-critical code, register hints can be provided:

```
; Suggest specific register for a variable
VAR TYPE_INT32, counter, 0, TYPE_RGP=RAX

; Alternative method using PUSH/POP
PUSH TYPE_RGP=RAX
POP counter
```

While these hints are available, they should be used sparingly as they reduce portability.

## Variable System Implementation

The COIL processor implements the variable system through:

1. **Symbol Table**: Tracks all variable declarations
2. **Scope Tracking**: Maintains the current scope nesting
3. **Usage Analysis**: Analyzes variable access patterns
4. **Register Allocation**: Determines optimal storage for each variable
5. **Lifetime Management**: Handles variable creation and destruction

## Advanced Variable Features

### Composite Variables

Variables can have composite types:

```
; Structure variable
VAR TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y, 0, 0

; Array variable
VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
```

### Global Variables

Variables can be declared at global scope:

```
; Global variable (outside any scope)
VAR TYPE_INT32, global_counter, 0

SYM main
    SCOPEE
    ; Access global variable
    ADD global_counter, global_counter, 1
    SCOPEL
```

## Complete Example

```
; Function using the variable system
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Initialize sum variable
    VAR TYPE_INT, sum, 0
    
    ; Add values to sum
    ADD sum, sum, a
    ADD sum, sum, b
    
    ; Calculate average
    VAR TYPE_INT, count, 2
    VAR TYPE_INT, average
    DIV average, sum, count
    
    ; Return result through ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, average
    SCOPEL  ; All variables automatically destroyed here
```

## Advantages Over Traditional Assembly

### Traditional x86-64 Assembly:
```
; Function using direct register management
calculate_sum:
    ; Save callee-saved registers
    push rbx
    
    ; Get parameters from registers
    ; rdi = a, rsi = b (System V ABI)
    
    ; Calculate sum
    xor rax, rax     ; Clear sum register
    add rax, rdi     ; Add a
    add rax, rsi     ; Add b
    
    ; Calculate average
    mov rbx, 2       ; Count
    xor rdx, rdx     ; Clear upper bits
    div rbx          ; rax = rax / rbx
    
    ; Restore saved register
    pop rbx
    
    ; Result in rax
    ret
```

### COIL Equivalent:
```
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
    SCOPEE
    ; Get parameters
    VAR TYPE_INT64, a
    VAR TYPE_INT64, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
    
    ; Calculate sum
    VAR TYPE_INT64, sum, 0
    ADD sum, sum, a
    ADD sum, sum, b
    
    ; Calculate average
    VAR TYPE_INT64, count, 2
    VAR TYPE_INT64, average
    DIV average, sum, count
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, average
    SCOPEL
```

### Advantages:
1. **Processor Independence**: COIL code works on any processor
2. **Simplified Programming**: No manual register management
3. **Automatic Optimization**: Optimal register allocation
4. **Reduced Errors**: No register usage conflicts
5. **Better Readability**: Clear variable names and scope
6. **Automatic Resource Management**: Variables cleaned up automatically