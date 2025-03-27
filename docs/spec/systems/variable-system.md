# COIL Variable System

## Purpose

This document defines the COIL variable system, which provides an abstraction over processor registers and memory, allowing for architecture-independent code with automatic management of storage allocation, lifetime, and optimization.

## Key Concepts

- **Variables vs. Registers**: How variables abstract over hardware details
- **Scoping**: How variable lifetimes are managed
- **Allocation Strategy**: How variables are assigned to storage
- **Optimization**: How variables can be optimized for performance
- **Type Integration**: How variables interact with the type system

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

Examples:
```
VAR TYPE_INT32, counter, 0       ; Integer with initialization
VAR TYPE_PTR, data_pointer       ; Pointer without initialization
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"  ; Array initialization
```

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

The specific algorithms used for register allocation depend on the implementation, but common approaches include:
- Graph coloring allocation
- Linear scan allocation
- Usage frequency analysis
- Liveness analysis

## Advanced Variable Features

### Composite Variables

Variables can have composite types:

```
; Structure variable
VAR TYPE_STRUCT=point, position, 0, 0

; Access structure fields
GET x_coord, position, x
SET position, y, 20

; Array variable
VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)

; Access array elements
INDEX element, numbers, 2  ; Get element at index 2
UPDT numbers, 1, 42       ; Update element at index 1
```

### Global Variables

Variables can be declared at global scope (outside any function):

```
; Global variable (outside any scope)
VAR TYPE_INT32, global_counter, 0

SYM main
    SCOPEE
    ; Access global variable
    ADD global_counter, global_counter, 1
    SCOPEL
```

Global variables have program lifetime and are typically stored in memory rather than registers.

## Integration with Other Systems

### ABI Integration

The variable system integrates with the ABI system for function parameters and return values:

```
; Function using variables with ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters as variables
    VAR TYPE_INT32, a, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Compute and return result
    VAR TYPE_INT32, result
    ADD result, a, b
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

### Type System Integration

Variables are strongly typed and interact closely with the type system:

```
; Variables with different types
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_value

; Type conversion using CONVERT
CONVERT float_value, int_value  ; float_value = 42.0
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

- [Memory Operations](../instruction-set/memory.md) - Instructions for variable operations
- [Type System](type-system.md) - Type system that variables are based on
- [ABI System](abi-system.md) - How variables interact with the ABI
- [Function Examples](../../examples/functions.md) - Examples of variable usage in functions