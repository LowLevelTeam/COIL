# COIL Variable System

## Purpose

The variable system provides an abstraction over processor registers and memory, enabling architecture-independent code with automatic storage allocation and lifetime management.

## Variables vs. Registers

Traditional assembly requires manual register management:
- Explicitly decide which registers to use
- Track register usage across function calls
- Save and restore registers around calls
- Manage register spilling when needed

COIL's variable system eliminates these complexities:
- Variables are declared with types
- The COIL processor decides optimal storage
- Variables are automatically allocated to registers or memory
- Register preservation is handled automatically
- Register spilling happens transparently

## Variable Declaration

Variables are declared using the `VAR` instruction:

```
VAR type, name [, initial_value]
```

Examples:
```
VAR TYPE_INT32, counter, 0       ; Integer with initialization
VAR TYPE_PTR, data_pointer       ; Pointer without initialization
VAR TYPE_FP32, pi, 3.14159       ; Floating-point with initialization
```

## Variable Usage

Once declared, variables can be used directly in instructions:

```
; Arithmetic with variables
ADD counter, counter, 1
MUL result, a, b

; Comparisons with variables
CMP counter, limit
BR_LT loop_start

; Memory access with variables
MOV [data_pointer], value
```

The COIL processor automatically:
1. Determines appropriate storage (register or memory)
2. Generates correct code to access the variables
3. Handles any necessary register spilling
4. Optimizes based on usage patterns

## Scope Management

Scopes define variable lifetimes using `SCOPEE` and `SCOPEL` instructions:

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

### Benefits of Scope Management

1. **Automatic Resource Management**:
   - Variables are automatically allocated when declared
   - Variables are automatically released at scope end
   - No manual resource tracking needed

2. **Memory Safety**:
   - Prevents accessing variables outside their lifetime
   - Helps prevent resource leaks
   - Makes code more maintainable

## Storage Optimization

### Promotion and Demotion

COIL provides hints for variable optimization:

```
; Demote a variable to memory
PUSH counter           ; Suggest storing counter in memory

; Promote a variable to register
POP counter            ; Suggest loading counter into a register
```

### Register Hints

For performance-critical code:

```
; Suggest specific register for a variable
VAR TYPE_INT32, counter, 0, TYPE_RGP=RAX
```

These hints should be used sparingly as they reduce portability.

## Implementation Details

The variable system is implemented by:

1. **Register Allocator**:
   - Analyzes variable usage patterns
   - Assigns registers based on frequency of use
   - Implements register spilling when needed
   - Respects ABI requirements

2. **Scope Manager**:
   - Tracks variable declarations and scopes
   - Manages variable lifetime
   - Handles resource reclamation

3. **Optimizer**:
   - Identifies optimization opportunities
   - Performs copy propagation and elimination
   - Reduces memory-to-register transfers