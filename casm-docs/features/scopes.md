# CASM Scopes

## Purpose

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
    
    VAR TYPE_INT32, counter, 0   ; counter is created here
    ; counter is valid here
    
    VAR TYPE_FP32, value, 3.14   ; value is created here
    ; both counter and value are valid here
    
SCOPEL    ; counter and value are destroyed here
```

### Nested Scopes

Scopes can be nested, creating a hierarchy:

```
SCOPEE                           ; Outer scope
    VAR TYPE_INT32, outer_var, 10
    
    SCOPEE                       ; Inner scope
        VAR TYPE_INT32, inner_var, 20
        ; Both outer_var and inner_var are accessible here
    SCOPEL                       ; inner_var is destroyed here
    
    ; Only outer_var is accessible here
SCOPEL                           ; outer_var is destroyed here
```

### Scope Visibility Rules

1. Inner scopes can access variables from outer scopes
2. Outer scopes cannot access variables from inner scopes
3. Variable names in inner scopes can shadow outer variables

```
SCOPEE
    VAR TYPE_INT32, x, 10        ; Outer x
    
    SCOPEE
        VAR TYPE_INT32, x, 20    ; Inner x (shadows outer x)
        ; Here x refers to the inner x (value 20)
    SCOPEL
    
    ; Here x refers to the outer x (value 10)
SCOPEL
```

## Function Scopes

Functions typically have their own scope:

```
SYM add_numbers
    SCOPEE
    ; Function variables
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    VAR TYPE_INT32, result
    ADD result, a, b
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

When the function returns, all local variables are automatically cleaned up.

## Scope Implementation

### Resource Management

Scopes manage several types of resources:

1. **Register Allocation**: Variables may be assigned to registers
2. **Memory Allocation**: Variables may require memory (stack or heap)
3. **Compiler Resources**: Symbol tables, type information, etc.

When a scope ends, the COIL processor:
1. Frees memory used by local variables
2. Releases registers for reuse
3. Removes variable entries from the symbol table

### Scope Optimization

The COIL processor may optimize scope handling:

1. **Register Allocation**: Efficiently reuse registers across scopes
2. **Dead Variable Elimination**: Free resources when variables are no longer used
3. **Scope Merging**: Combine adjacent scopes with no variable conflicts

## Advanced Scope Techniques

### Early Exit

When you exit a function before the end of a scope, the scope is still properly cleaned up:

```
SYM check_value
    SCOPEE
    VAR TYPE_INT32, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    CMP value, 0
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
    VAR TYPE_PTR, resource_handle
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