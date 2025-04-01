# COIL Memory and Variable System

## Overview
The COIL memory and variable system provides a unified abstraction over both memory access and register allocation. Programs use variables with numeric IDs, and the COIL processor automatically determines optimal storage (register or memory) based on usage patterns.

## Memory Model

### Address Space
- Byte-addressable linear memory
- Platform-dependent size (typically matching pointer width)
- Zero-based addressing

### Memory Sections

| Section | Purpose | Common Attributes |
|---------|---------|-------------------|
| Code | Program instructions | Executable, Readable |
| Data | Initialized data | Readable, Writable |
| BSS | Zero-initialized data | Readable, Writable, Uninitialized |
| Stack | Function calls, local variables | Readable, Writable |
| Heap | Dynamic allocations | Readable, Writable |

Section attributes (bit flags):
- `0x01`: Executable - Instructions can be fetched
- `0x02`: Writable - Data can be written
- `0x04`: Readable - Data can be read
- `0x08`: Initialized - Contains initial data
- `0x10`: Uninitialized - Zero-filled initially

Example:
```
SECTION .text, 0x01 | 0x04         ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
```

### Memory Access Patterns

```
; Direct access
MOV [address], 42                  ; Store to memory
MOV value, [address]               ; Load from memory

; Pointer-based
MOV [ptr], value                   ; Store through pointer 
MOV value, [ptr]                   ; Load through pointer

; Address calculations
MOV [base_ptr + 8], value          ; Base + offset
MOV [array + index*4], value       ; Base + scaled index
MOV [base + index*4 + 8], value    ; Full addressing mode
```

## Variable System

### Variable Declaration
Variables are declared using the `VAR` instruction with a numeric ID:

```
VAR #id, type [, initial_value]
```

Examples:
```
VAR #1, TYPE_INT32, 0              ; Integer with initialization
VAR #2, TYPE_PTR                   ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159         ; Floating-point with initialization
```

### Variable Usage
Once declared, variables are used by their numeric ID:

```
; Arithmetic
ADD #1, #1, 1                      ; #1 = #1 + 1
MUL #4, #2, #3                     ; #4 = #2 * #3

; Comparisons
CMP #1, #5                         ; Compare #1 and #5
BR_LT loop_start                   ; Branch if #1 < #5

; Memory access
MOV [#2], #6                       ; Store #6 to address in #2
MOV #7, [#2]                       ; Load from address in #2 to #7
```

### Scope-Based Variable Management
Variables exist within scopes that define their lifetimes:

```
SCOPEE                              ; Enter a new scope
    VAR #1, TYPE_INT32, 0           ; Variable #1 is created
    
    ; #1 is valid here
    ADD #1, #1, 1
    
    SCOPEE                          ; Nested scope
        VAR #2, TYPE_INT32, 20      ; Variable #2 is created
        ; Both #1 and #2 are valid here
    SCOPEL                          ; #2 is destroyed here
    
    ; Only #1 is valid here
SCOPEL                              ; #1 is destroyed here
```

Key scope properties:
1. Inner scopes can access variables from outer scopes
2. Outer scopes cannot access variables from inner scopes
3. Variable IDs are scope-relative (the same ID can be reused in different scopes)
4. Resources are automatically released when a scope ends

### Implementation Mechanics

The COIL processor handles:
1. **Register Allocation**: Choosing optimal registers for variables
2. **Spill/Fill**: Moving variables between registers and memory as needed
3. **Lifetime Management**: Tracking when variables can be freed
4. **Register Pressure Management**: Optimizing register usage under constraints

### Memory Operations

```
; Block operations
MEMCPY dest_ptr, src_ptr, size     ; Copy memory block
MEMSET buffer, 0, size             ; Zero out memory block  
MEMCMP result, buf1, buf2, size    ; Compare memory blocks

; Atomic operations  
XCHG old_value, [lock_var]         ; Atomic exchange
CAS [lock_var], expected, new_val  ; Compare and swap
```

### Optimization Hints
COIL provides optional hints for variable storage:

```
; Suggest register/memory preferences
PUSH #1                            ; Demote #1 to memory
POP #1                             ; Promote #1 to register
VAR #1, TYPE_INT32, 0, TYPE_RGP=RAX ; Suggest using RAX for #1
```

These are hints only - the processor may choose differently based on optimizations.

## Alignment Requirements

COIL enforces these alignment requirements:
- 1-byte types: 1-byte alignment
- 2-byte types: 2-byte alignment
- 4-byte types: 4-byte alignment
- 8-byte types: 8-byte alignment
- 16-byte types: 16-byte alignment

## Array and Structure Access

```
; Array element access
INDEX #5, array, #6             ; Get array[#6] into #5
UPDT array, #7, #8              ; Set array[#7] = #8

; Structure field access
GET #1, struct_ptr, field_name  ; Get field value
SET struct_ptr, field_name, #2  ; Set field value
```