# CASM Variables

## Purpose

Variables in CASM provide an abstraction over registers and memory, allowing you to write code without manually managing register allocation and data storage.

## Variable Fundamentals

### Declaration

Variables are declared using the `VAR` instruction with a numeric ID:

```
VAR #id, type [, initial_value]
```

Where:
- `#id` is the numeric identifier for the variable
- `type` is any valid COIL type (like TYPE_INT32, TYPE_FP64)
- `initial_value` is an optional initialization value

Examples:
```
VAR #1, TYPE_INT32, 0       ; Integer with initialization
VAR #2, TYPE_PTR            ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159  ; Floating-point with initialization
```

### Usage

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
MOV #7, [#2]
```

### Lifecycle

Variables exist from their declaration point until the end of their enclosing scope:

```
SCOPEE
    VAR #1, TYPE_INT32, 0   ; variable #1 is created here
    
    ; variable #1 is valid here
    ADD #1, #1, 1
    
SCOPEL    ; variable #1 is destroyed here

; variable #1 is no longer valid here
```

## Variable Types

Variables can have any valid COIL type:

### Basic Types

```
VAR #1, TYPE_INT32, -42      ; Signed 32-bit integer
VAR #2, TYPE_UNT8, 0xFF      ; Unsigned 8-bit integer
VAR #3, TYPE_FP64, 3.14159   ; 64-bit floating point
```

### Pointer Types

```
VAR #4, TYPE_PTR             ; Generic pointer
VAR #5, TYPE_PTR=TYPE_INT32  ; Pointer to 32-bit integer
```

### Composite Types

```
VAR #6, TYPE_ARRAY=TYPE_INT32, (1, 2, 3, 4, 5)  ; Array of integers
VAR #7, TYPE_STRUCT=point, (10, 20)             ; Structure with x,y fields
```

### Vector Types

```
VAR #8, TYPE_V128=TYPE_FP32, (1.0, 2.0, 3.0, 4.0)  ; Vector of 4 floats
```

## Scope-Based Variable System

### Variable ID Assignment

Variable IDs are scope-relative:
- Each scope has its own variable ID namespace
- IDs are assigned sequentially starting from 1
- The same ID can be reused in different scopes

```
SCOPEE           ; Outer scope
    VAR #1, TYPE_INT32, 10   ; Outer scope variable #1
    
    SCOPEE       ; Inner scope
        VAR #1, TYPE_INT32, 20   ; Inner scope variable #1 (different from outer)
        ; Here #1 refers to the inner variable (value 20)
    SCOPEL
    
    ; Here #1 refers to the outer variable (value 10)
SCOPEL
```

### Nested Scope Access

Inner scopes can access variables from outer scopes by their ID:

```
SCOPEE           ; Outer scope
    VAR #1, TYPE_INT32, 10
    
    SCOPEE       ; Inner scope
        ADD #1, #1, 5        ; Modifies outer scope variable #1
    SCOPEL
    
    ; Here #1 now has value 15
SCOPEL
```

## Implementation Details

Variables are automatically:

1. **Allocated** to registers or memory based on usage patterns
2. **Initialized** when declared with initial values
3. **Tracked** within their scope
4. **Freed** when their scope ends

The COIL processor handles all these details, allowing you to focus on program logic.

## Advanced Variable Features

### Register Hints

For performance-critical code, you can provide register hints:

```
VAR #1, TYPE_INT32, 0, TYPE_RGP=RAX  ; Suggest using RAX for variable #1
```

This is a hint only and the processor may choose a different register if necessary.

### Promotion and Demotion

You can suggest register allocation changes:

```
PUSH #1   ; Suggest storing variable #1 in memory (demote)
POP #1    ; Suggest loading variable #1 into register (promote)
```

## Variable Access Patterns

### Direct Variable Access

```
MOV #1, 42           ; Store 42 in variable #1
ADD #1, #1, 1        ; Increment variable #1
MOV #2, #1           ; Copy variable #1 to variable #2
```

### Indirect Variable Access (via Pointers)

```
VAR #3, TYPE_PTR=TYPE_INT32
LEA #3, #1           ; Point to variable #1
MOV [#3], 42         ; Store 42 in what #3 points to
ADD [#3], [#3], 1    ; Increment what #3 points to
```

### Array Element Access

```
VAR #4, TYPE_ARRAY=TYPE_INT32, (10, 20, 30, 40, 50)
VAR #5, TYPE_INT32
INDEX #5, #4, 2      ; Get element at index 2 (value 30)
UPDT #4, 1, 25       ; Update element at index 1 to 25
```

### Structure Field Access

```
VAR #6, TYPE_STRUCT=point, (10, 20)
VAR #7, TYPE_INT32
GET #7, #6, x        ; Get x field from variable #6
SET #6, y, 25        ; Set y field in variable #6 to 25
```

## Best Practices

1. **Declare variables close to first use**
2. **Keep variable scope as narrow as possible**
3. **Initialize variables when declared**
4. **Use the smallest appropriate type**
5. **Avoid using register hints unless necessary**