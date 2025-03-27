# CASM Memory Access

## Memory Addressing

CASM provides flexible memory addressing modes to access data in memory.

### Direct Memory Access

The simplest form uses square brackets to denote a memory location:

```
MOV [address], 42         ; Store 42 to memory at 'address'
MOV value, [address]      ; Load from memory at 'address'
```

### Base + Offset Addressing

Access memory at a base address plus an offset:

```
MOV [base_ptr + 8], value     ; Store at base_ptr + 8 bytes
MOV value, [base_ptr + 8]     ; Load from base_ptr + 8 bytes
```

### Base + Scaled Index Addressing

Access array elements with a scaled index:

```
MOV [array + index*4], value  ; Store to array[index] (4-byte elements)
MOV value, [array + index*4]  ; Load from array[index] (4-byte elements)
```

### Complete Addressing Mode

Combine base, index, and offset:

```
MOV [base + index*scale + offset], value
MOV value, [base + index*scale + offset]
```

## Pointer Variables

### Declaring Pointers

Pointers are declared using `TYPE_PTR`:

```
VAR TYPE_PTR, generic_ptr           ; Generic pointer
VAR TYPE_PTR=TYPE_INT32, int_ptr    ; Pointer to 32-bit integer
```

### Pointer Operations

```
MOV int_ptr, array          ; Point to the beginning of array
MOV value, [int_ptr]        ; Dereference pointer (load value)
MOV [int_ptr], 42           ; Store through pointer
ADD int_ptr, int_ptr, 4     ; Advance pointer by 4 bytes
```

### Pointer Arithmetic

Pointer arithmetic is scaled by the pointed-to type size:

```
VAR TYPE_PTR=TYPE_INT32, ptr, array
ADD ptr, ptr, 1             ; Advance by sizeof(INT32) = 4 bytes
MOV value, [ptr]            ; Access the next INT32 element
```

## Memory Block Operations

### MEMCPY

Copy a block of memory:

```
MEMCPY dest_ptr, src_ptr, byte_count
```

Example:
```
VAR TYPE_PTR, src, source_array
VAR TYPE_PTR, dest, destination_array
VAR TYPE_UNT32, size, 100
MEMCPY dest, src, size      ; Copy 100 bytes from src to dest
```

### MEMSET

Fill a block of memory with a value:

```
MEMSET dest_ptr, value, byte_count
```

Example:
```
VAR TYPE_PTR, buffer
VAR TYPE_UNT32, size, 1024
MEMSET buffer, 0, size      ; Zero out 1024 bytes
```

### MEMCMP

Compare two memory blocks:

```
MEMCMP result, ptr1, ptr2, byte_count
```

Example:
```
VAR TYPE_INT32, comparison_result
MEMCMP comparison_result, buffer1, buffer2, 100
CMP comparison_result, 0
BR_EQ blocks_equal
```

## Array Operations

CASM provides special instructions for array element access:

### INDEX

Access an array element by index:

```
INDEX destination, array, index
```

Example:
```
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)
VAR TYPE_INT32, third_element
INDEX third_element, numbers, 2    ; Get element at index 2 (value 30)
```

### UPDT

Update an array element by index:

```
UPDT array, index, value
```

Example:
```
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)
UPDT numbers, 1, 25         ; Set element at index 1 to 25
```

## Atomic Memory Operations

### XCHG

Atomically exchange values:

```
XCHG destination, source
```

Example:
```
VAR TYPE_INT32, old_value
XCHG old_value, [lock_var]  ; Atomically exchange
```

### CAS

Compare and swap operation:

```
CAS destination, expected, new_value
```

Example:
```
VAR TYPE_INT32, expected, 0
VAR TYPE_INT32, new_value, 1
CAS [lock_var], expected, new_value
```

## Memory Safety Considerations

### Alignment

Memory access should respect alignment requirements:

```
; Proper alignment for different types
MOV [int_ptr], 42             ; INT32 requires 4-byte alignment
MOV [double_ptr], 3.14159     ; FP64 requires 8-byte alignment
```

### Bounds Checking

CASM implementations may perform optional bounds checking:

```
; Array bounds checking (if enabled)
INDEX element, array, index    ; Will check if index is valid
```

### Null Pointer Checking

Avoid dereferencing null pointers:

```
CMP ptr, 0
BR_EQ handle_null_ptr
MOV value, [ptr]    ; Safe because we checked ptr is not null
```

## Memory Model Integration

### Stack Access

Stack memory is typically managed automatically via the SCOPEE/SCOPEL system:

```
SCOPEE
  VAR TYPE_INT32, stack_var, 42   ; Allocated on stack
  ; Use stack_var...
SCOPEL  ; stack_var is automatically deallocated
```

### Heap Access

For dynamic memory allocation, COIL v2 will provide standard library functions.

### Scope-Based Safety

Using scopes helps ensure memory safety:

```
SCOPEE
  VAR TYPE_PTR, resource
  ; Allocate and use resource
  ; ...
SCOPEL  ; Resource automatically freed
```

## Common Memory Access Patterns

### Array Iteration

```
VAR TYPE_PTR=TYPE_INT32, ptr, array
VAR TYPE_INT32, i, 0
VAR TYPE_INT32, count, 10

loop_start:
CMP i, count
BR_GE loop_end
  
MOV value, [ptr + i*4]    ; Access array[i]
; Process value...
  
INC i
BR loop_start
loop_end:
```

### Struct Field Access

```
VAR TYPE_PTR=TYPE_STRUCT=point, point_ptr, point_instance
MOV x_value, [point_ptr]             ; Access x field
MOV y_value, [point_ptr + 4]         ; Access y field (assuming x is 4 bytes)

; Or using GET/SET with structs:
GET x_value, [point_ptr], x
SET [point_ptr], y, new_y_value
```