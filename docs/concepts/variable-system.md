# COIL Variable System

## Purpose

This document defines the COIL variable system, a key feature that abstracts over processor registers and memory, providing architecture-independent variables with automatic management of storage allocation, lifetime, and optimization.

## Variables vs. Registers

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

## Variable Declaration and Lifetime

Variables in COIL are declared using the `VAR` instruction:

```
VAR type, name [, initial_value]
```

Where:
- `type` is any valid COIL type
- `name` is a symbolic identifier for the variable
- `initial_value` is an optional initialization value

Variables are scoped with explicit lifetimes based on lexical scopes:
- Variables exist from declaration until the end of their scope
- Nested scopes allow for organized variable management
- When a scope ends, all variables in that scope are automatically deallocated
- Resources (registers or memory) are automatically freed when variables go out of scope

## Scope Management

Scopes are defined by the `SCOPEE` and `SCOPEL` instructions:

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

## Implementation Strategy

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

## Benefits Over Traditional Assembly

The COIL variable system provides several advantages:

1. **Processor Independence**: Code works on any processor
2. **Simplified Programming**: No manual register management
3. **Automatic Optimization**: Optimal register allocation
4. **Reduced Errors**: No register usage conflicts
5. **Better Readability**: Clear variable names and scope
6. **Automatic Resource Management**: Variables cleaned up automatically

## Related Documentation

For more information about interacting with variables, see:
- [Memory Operations](../isa/memory.md) - Instructions for variable operations
- [Scope Instructions](../isa/memory.md#scope-management) - Details on scope management
- [Function Examples](../examples/functions.md) - Examples of variable usage in functions