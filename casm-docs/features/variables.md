# CASM Variables

## Purpose

Variables in CASM provide an abstraction over registers and memory, allowing you to write code without manually managing register allocation and data storage.

## Variable Fundamentals

### Declaration

Variables are declared using the `VAR` instruction:

```
VAR type, name [, initial_value]
```

Where:
- `type` is any valid COIL type (like TYPE_INT32, TYPE_FP64)
- `name` is the variable's identifier
- `initial_value` is an optional initialization value

Examples:
```
VAR TYPE_INT32, counter, 0       ; Integer with initialization
VAR TYPE_PTR, data_pointer       ; Pointer without initialization
VAR TYPE_FP32, pi, 3.14159       ; Floating-point with initialization
```

### Usage

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
MOV value, [data_pointer]
```

### Lifecycle

Variables exist from their declaration point until the end of their enclosing scope:

```
SCOPEE
    VAR TYPE_INT32, counter, 0   ; counter is created here
    
    ; counter is valid here
    ADD counter, counter, 1
    
SCOPEL    ; counter is destroyed here

; counter is no longer valid here
```

## Variable Types

Variables can have any valid COIL type:

### Basic Types

```
VAR TYPE_INT32, signed_int, -42      ; Signed 32-bit integer
VAR TYPE_UNT8, byte_value, 0xFF      ; Unsigned 8-bit integer
VAR TYPE_FP64, double_value, 3.14159 ; 64-bit floating point
```

### Pointer Types

```
VAR TYPE_PTR, generic_pointer        ; Generic pointer
VAR TYPE_PTR=TYPE_INT32, int_ptr     ; Pointer to 32-bit integer
```

### Composite Types

```
VAR TYPE_ARRAY=TYPE_INT32, int_array, (1, 2, 3, 4, 5)  ; Array of integers
VAR TYPE_STRUCT=point, position, (10, 20)              ; Structure with x,y fields
```

### Vector Types

```
VAR TYPE_V128=TYPE_FP32, vector, (1.0, 2.0, 3.0, 4.0)  ; Vector of 4 floats
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
VAR TYPE_INT32, counter, 0, TYPE_RGP=RAX  ; Suggest using RAX for counter
```

This is a hint only and the processor may choose a different register if necessary.

### Promotion and Demotion

You can suggest register allocation changes:

```
PUSH counter   ; Suggest storing counter in memory (demote)
POP counter    ; Suggest loading counter into register (promote)
```

### Global Variables

Variables can be declared at global scope (outside any function):

```
; Global variable
VAR TYPE_INT32, global_counter, 0

SYM main
    SCOPEE
    ADD global_counter, global_counter, 1
    SCOPEL
```

Global variables have program lifetime and are typically stored in memory.

## Variable Access Patterns

### Direct Variable Access

```
MOV counter, 42           ; Store 42 in counter
ADD counter, counter, 1   ; Increment counter
MOV result, counter       ; Copy counter to result
```

### Indirect Variable Access (via Pointers)

```
VAR TYPE_PTR=TYPE_INT32, ptr
MOV ptr, &counter         ; Point to counter
MOV [ptr], 42             ; Store 42 in what ptr points to
ADD [ptr], [ptr], 1       ; Increment what ptr points to
```

### Array Element Access

```
VAR TYPE_ARRAY=TYPE_INT32, array, (10, 20, 30, 40, 50)
VAR TYPE_INT32, elem
INDEX elem, array, 2      ; Get element at index 2 (value 30)
UPDT array, 1, 25         ; Update element at index 1 to 25
```

### Structure Field Access

```
VAR TYPE_STRUCT=point, pos, (10, 20)
VAR TYPE_INT32, x_coord
GET x_coord, pos, x       ; Get x field from pos
SET pos, y, 25            ; Set y field in pos to 25
```

## Best Practices

1. **Declare variables close to first use**
2. **Use meaningful variable names**
3. **Initialize variables when declared**
4. **Use the smallest appropriate type**
5. **Keep variable scope as narrow as possible**
6. **Avoid using register hints unless necessary**