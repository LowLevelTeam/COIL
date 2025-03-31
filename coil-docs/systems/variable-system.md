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
- Variables are declared with types and numeric IDs
- The COIL processor decides optimal storage
- Variables are automatically allocated to registers or memory
- Register preservation is handled automatically
- Register spilling happens transparently

## Variable Declaration

Variables are declared using the `VAR` instruction with numeric IDs:

```
VAR #id, type [, initial_value]
```

Examples:
```
VAR #1, TYPE_INT32, 0       ; Integer with initialization
VAR #2, TYPE_PTR             ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159   ; Floating-point with initialization
```

## Variable Usage

Once declared, variables can be used directly in instructions by their numeric ID:

```
; Arithmetic with variables
ADD #1, #1, 1
MUL #4, #2, #3

; Comparisons with variables
CMP #1, #5
BR_LT loop_start

; Memory access with variables
MOV [#2], #6
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
    VAR #1, TYPE_INT32, 0
    
    ; Variables can be used here
    ADD #1, #1, 1
    
    ; Nested scopes are supported
    SCOPEE
        VAR #1, TYPE_INT32, 42  ; Inner scope #1 is distinct from outer scope #1
        ; Both outer and inner variables are accessible by their scope-relative IDs
    SCOPEL  ; inner scope variables are destroyed here
    
    ; Only outer scope variables are accessible here
SCOPEL  ; outer scope variables are destroyed here
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

## Variable ID System

Variable IDs are numeric and scope-relative:

1. **Scope-Local Identifiers**:
   - Each scope maintains its own ID namespace
   - IDs are reused between different scopes
   - Numeric format eliminates string lookup overhead

2. **ID Assignment**:
   - IDs are assigned sequentially within a scope
   - First variable in a scope gets ID #1
   - IDs are purely numeric with no string association

3. **Access Rules**:
   - Inner scopes can access outer scope variables
   - Outer scopes cannot access inner scope variables
   - Variables with the same ID in different scopes are distinct

## Storage Optimization

### Promotion and Demotion

COIL provides hints for variable optimization:

```
; Demote a variable to memory
PUSH #1           ; Suggest storing variable #1 in memory

; Promote a variable to register
POP #1            ; Suggest loading variable #1 into a register
```

### Register Hints

For performance-critical code:

```
; Suggest specific register for a variable
VAR #1, TYPE_INT32, 0, TYPE_RGP=RAX
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

## Binary Encoding

In the binary format, variables are represented by their numeric IDs:

```
0x9000 [var_id]  ; TYPE_VAR + variable identifier as a numeric index
```

The var_id is scope-relative, making it efficient to encode and process.