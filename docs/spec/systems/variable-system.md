# COIL Variable System

## Purpose

This document defines the COIL variable system, which provides an abstraction over processor registers and memory. This system enables architecture-independent code with automatic management of storage allocation, variable lifetime, and optimization.

## Variables vs. Registers

Traditional assembly languages require programmers to manage registers manually:
- Decide which registers to use for each value
- Track register usage across function calls
- Save and restore registers around function calls
- Manage register spilling when running out of registers

COIL's variable system eliminates these complexities:
- Variables are declared with types
- The COIL processor decides where to store each variable
- Variables are automatically allocated to registers or memory
- The processor handles register saving and restoration
- Register spilling happens automatically when needed

## Variable Declaration and Usage

### Basic Declaration

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
VAR TYPE_INT32, counter, 0       ; Integer with initialization
VAR TYPE_PTR, data_pointer       ; Pointer without initialization
VAR TYPE_FP32, pi, 3.14159       ; Floating-point with initialization
```

### Using Variables

Once declared, variables can be used directly in instructions:

```
; Use variables in arithmetic
ADD counter, counter, 1
MUL result, a, b

; Use variables in comparisons
CMP counter, limit
BR_LT loop_start

; Use variables with memory operations
MOV [data_pointer], value
```

The COIL processor automatically:
1. Allocates appropriate storage for each variable (register or memory)
2. Generates the correct code to access the variables
3. Handles any necessary register spilling or reloading
4. Optimizes access patterns based on usage

## Scope Management

### Defining Scopes

Scopes define variable lifetimes using the `SCOPEE` and `SCOPEL` instructions:

```
SCOPEE
    ; Variables declared here exist within this scope
    VAR TYPE_INT32, local_var, 0
    
    ; Variables can be used here
    ADD local_var, local_var, 1
    
    ; Nested scopes are supported
    SCOPEE
        VAR TYPE_INT32, inner_var, 42
        ; Both local_var and inner_var are accessible here
    SCOPEL  ; inner_var is destroyed here
    
    ; Only local_var is accessible here
SCOPEL  ; local_var is destroyed here
```

### Scope Benefits

The scope system provides several benefits:

1. **Automatic Resource Management**:
   - Variables are automatically allocated when declared
   - Variables are automatically released at scope end
   - No manual resource tracking needed

2. **Memory Safety**:
   - Prevents accessing variables outside their lifetime
   - Helps prevent resource leaks
   - Makes code more maintainable

3. **Optimization Opportunity**:
   - Allows the processor to reuse registers and memory
   - Provides clear lifetime information for optimization
   - Enables more efficient code generation

## Variable Storage Optimization

### Promotion and Demotion

COIL provides hints for variable optimization through the `PUSH` and `POP` instructions:

```
; Demote a variable (hint to store in memory)
PUSH counter           ; Suggest storing counter in memory

; Promote a variable (hint to store in register)
POP counter            ; Suggest loading counter into a register
```

These operations have dual purposes:
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

## Integration with Other Systems

### Type System Integration

Variables are strongly typed and integrated with COIL's type system:

```
; Variables with different types
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_value

; Type conversion using CONVERT
CONVERT float_value, int_value  ; float_value = 42.0
```

### ABI System Integration

The variable system integrates with the ABI system for function parameters and return values:

```
; Function using variables with ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters as variables
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Compute result in a variable
    VAR TYPE_INT32, result
    ADD result, a, b
    
    ; Return result variable via ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

### Memory System Integration

Variables can be used with memory operations seamlessly:

```
; Array operations using variables
VAR TYPE_PTR, array_ptr
VAR TYPE_INT32, index, 0
VAR TYPE_INT32, value

; Load from array
MOV value, [array_ptr + index*4]

; Store to array
MOV [array_ptr + index*4], 42
```

## Advanced Variable Features

### Composite Variables

Variables can have composite types:

```
; Structure variable
VAR TYPE_STRUCT=point, position, (10, 20)

; Access structure fields
GET x_coord, position, x
SET position, y, 30

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

## Implementation Strategy

The COIL processor implements the variable system through:

1. **Symbol Table**: Tracks all variable declarations
2. **Scope Tracking**: Maintains the current scope nesting
3. **Usage Analysis**: Analyzes variable access patterns
4. **Register Allocation**: Determines optimal storage for each variable
5. **Lifetime Management**: Handles variable creation and destruction

## Related Documentation

- [Memory Operations](../instruction-set/memory.md) - Instructions for variable operations
- [Type System](type-system.md) - Type system that variables are based on
- [ABI System](abi-system.md) - How variables interact with the ABI
- [Function Examples](../../examples/functions.md) - Examples of variable usage in functions