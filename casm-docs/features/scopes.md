# CASM Scopes

## Overview

Scopes in CASM define variable lifetimes and resource management boundaries. They provide automatic resource allocation and deallocation, simplifying memory management and program structure.

## Scope Basics

### Creating Scopes

Scopes are created using the `SCOPEE` (scope enter) and `SCOPEL` (scope leave) instructions:

```
SCOPEE
    ; Code and variable declarations within the scope
SCOPEL
```

### Scope Effects

A scope:
1. Creates a new variable context
2. Manages lifetime of variables declared within it
3. Automatically releases resources when exited

## Variable Scope Rules

### Declaration and Lifetime

Variables exist from their declaration point until the end of their enclosing scope:

```
SCOPEE
    ; No variables yet
    
    VAR #1, TYPE_INT32, 0   ; #1 is created here
    ; #1 is valid here
    
    VAR #2, TYPE_FP32, 3.14 ; #2 is created here
    ; Both #1 and #2 are valid here
    
SCOPEL    ; #1 and #2 are destroyed here
```

### Nested Scopes

Scopes can be nested, creating a hierarchy:

```
SCOPEE                           ; Outer scope
    VAR #1, TYPE_INT32, 10
    
    SCOPEE                       ; Inner scope
        VAR #2, TYPE_INT32, 20
        ; Both #1 and #2 are accessible here
    SCOPEL                       ; #2 is destroyed here
    
    ; Only #1 is accessible here
SCOPEL                           ; #1 is destroyed here
```

### Scope Visibility Rules

1. Inner scopes can access variables from outer scopes
2. Outer scopes cannot access variables from inner scopes
3. Variable names in inner scopes can shadow outer variables

```
SCOPEE
    VAR #1, TYPE_INT32, 10        ; Outer #1
    
    SCOPEE
        VAR #1, TYPE_INT32, 20    ; Inner #1 (shadows outer #1)
        ; Here #1 refers to the inner #1 (value 20)
    SCOPEL
    
    ; Here #1 refers to the outer #1 (value 10)
SCOPEL
```

## Function Scopes

Functions typically have their own scope:

```
SYM add_numbers
    SCOPEE
    ; Function variables
    VAR #1, TYPE_INT32
    VAR #2, TYPE_INT32
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    VAR #3, TYPE_INT32
    ADD #3, #1, #2
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    SCOPEL
```

When the function returns, all local variables are automatically cleaned up.

## Advanced Scope Techniques

### Early Exit

When you exit a function before the end of a scope, the scope is still properly cleaned up:

```
SYM check_value
    SCOPEE
    VAR #1, TYPE_INT32
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    CMP #1, 0
    BR_GE non_negative
    
    ; Early return for negative values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0
    
    SYM non_negative
    ; Continue processing for non-negative values
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 1
    SCOPEL
```

In this example, regardless of which return path is taken, all variables in the scope are properly cleaned up.

### Scoped Control Structures

Scopes can be used to create structured control flow:

```
; If-then-else structure using scopes
CMP value, threshold
BR_LT else_branch

SCOPEE  ; Then branch
    ; Code for the "then" case
SCOPEL

BR end_if

else_branch:
SCOPEE  ; Else branch
    ; Code for the "else" case
SCOPEL

end_if:
```

### Resource Scope

Scopes can be used specifically for resource management:

```
SCOPEE  ; Resource scope
    VAR #1, TYPE_PTR
    ; Acquire resource
    
    ; Use resource
    
SCOPEL  ; Resource is automatically released
```

## Best Practices

1. **Keep scopes as narrow as possible**
2. **Explicitly close all scopes with SCOPEL**
3. **Use nested scopes to limit variable lifetimes**
4. **Avoid excessive nesting (more than 3-4 levels)**
5. **Ensure all code paths properly exit scopes**

## Related Components

- [Memory and Variables](/coil-docs/systems/memory-and-variables.md) - Variable system
- [ABI System](/coil-docs/systems/abi-system.md) - Function scope usage with ABI
- [CASM Basics](/casm-docs/basics/casm-basics.md) - Basic CASM syntax and constructs