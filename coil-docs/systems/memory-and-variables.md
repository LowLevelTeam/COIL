# COIL Memory and Variable System

## Memory Model

### Address Space

COIL defines a linear address space with these characteristics:
- Byte-addressable memory
- Platform-dependent size (typically matching pointer width)
- Zero-based addressing

### Memory Sections

| Section Type | Purpose | Common Attributes |
|--------------|---------|-------------------|
| Code | Instructions | Executable, Readable |
| Data | Initialized program data | Readable, Writable |
| BSS | Zero-initialized data | Readable, Writable, Uninitialized |
| Stack | Function calls and locals | Readable, Writable |
| Heap | Dynamic allocations | Readable, Writable |

Section attributes (bit flags):
- `0x01`: Executable - Instructions can be fetched
- `0x02`: Writable - Data can be written
- `0x04`: Readable - Data can be read
- `0x08`: Initialized - Contains initialized data
- `0x10`: Uninitialized - Contains zero-filled data

Example:
```
SECTION .text, 0x01 | 0x04         ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
```

### Memory Access Rules

COIL enforces alignment requirements:
- 1-byte types: 1-byte alignment
- 2-byte types: 2-byte alignment
- 4-byte types: 4-byte alignment
- 8-byte types: 8-byte alignment
- 16-byte types: 16-byte alignment

### Memory Access Patterns

```
; Direct memory access
MOV [address], 42                  ; Store to memory
MOV value, [address]               ; Load from memory

; Pointer-based access
MOV [ptr], value                   ; Store through pointer 
MOV value, [ptr]                   ; Load through pointer

; Complex addressing
MOV [base_ptr + 8], value          ; Base + offset
MOV [array + index*4], value       ; Base + scaled index
```

### Block Operations

```
; Copy memory
MEMCPY dest_ptr, src_ptr, size     ; Copy memory block

; Fill memory
MEMSET buffer, 0, size             ; Zero out memory block  

; Compare memory
MEMCMP result, buf1, buf2, size    ; Compare memory blocks
```

### Atomic Operations

```
; Atomic exchange  
XCHG old_value, [lock_var]         ; Atomic exchange

; Compare and swap
CAS [lock_var], expected, new_value ; Atomic compare-and-swap
```

## Variable System

COIL's variable system abstracts over registers and memory:
- Variables are declared with types and numeric IDs
- The COIL processor decides optimal storage (register or memory)
- Variable lifetimes are managed through scopes
- No need for manual register management

### Variable Declaration

```
VAR #id, type [, initial_value]
```

Examples:
```
VAR #1, TYPE_INT32, 0              ; Integer with initialization
VAR #2, TYPE_PTR                    ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159         ; Floating-point with initialization
```

### Variable Usage

Once declared, variables are used by their numeric ID:

```
; Arithmetic with variables
ADD #1, #1, 1                       ; #1 = #1 + 1
MUL #4, #2, #3                      ; #4 = #2 * #3

; Comparisons with variables
CMP #1, #5                          ; Compare #1 and #5
BR_LT loop_start                    ; Branch if #1 < #5

; Memory access with variables
MOV [#2], #6                        ; Store #6 to address in #2
MOV #7, [#2]                        ; Load from address in #2 to #7
```

### Scope Management

Scopes define variable lifetimes:

```
SCOPEE                              ; Enter a new scope
    VAR #1, TYPE_INT32, 0           ; Variable #1 is created here
    
    ; Variable #1 is valid here
    ADD #1, #1, 1
    
    SCOPEE                          ; Nested scope
        VAR #2, TYPE_INT32, 20      ; Variable #2 is created here
        ; Both #1 and #2 are valid here
    SCOPEL                          ; #2 is destroyed here
    
    ; Only #1 is valid here
SCOPEL                              ; #1 is destroyed here
```

Benefits of scope-based management:
1. Automatic resource allocation when declared
2. Automatic resource release at scope exit
3. Register/memory reuse optimization
4. Clear variable lifetime rules

### Variable ID System

Variable IDs are numeric and scope-relative:
- Each scope has its own ID namespace starting with #1
- IDs are reused between different scopes
- Inner scopes can access outer scope variables
- Outer scopes cannot access inner scope variables

### Storage Optimization Hints

COIL provides hints for variable storage optimization:

```
; Suggest storing variable in memory
PUSH #1                            ; Demote #1 to memory

; Suggest loading variable into register
POP #1                             ; Promote #1 to register

; Suggest specific register
VAR #1, TYPE_INT32, 0, TYPE_RGP=RAX ; Prefer RAX for #1
```

## Implementation Details

The variable and memory systems are implemented by:

1. **Register Allocator**:
   - Analyzes variable usage patterns
   - Assigns registers by frequency of use
   - Implements register spilling when needed

2. **Scope Manager**:
   - Tracks variable declarations and scopes
   - Manages variable lifetime
   - Handles resource reclamation

3. **Memory Manager**:
   - Allocates memory regions
   - Enforces alignment requirements
   - Implements memory protection

4. **Optimizer**:
   - Identifies optimization opportunities
   - Performs copy propagation
   - Reduces memory-to-register transfers