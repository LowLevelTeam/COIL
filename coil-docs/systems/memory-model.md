# COIL Memory Model

## Purpose

This document defines the memory model for COIL, establishing the rules for memory organization, access patterns, memory ordering, and memory management.

## Memory Organization

### Address Space

COIL defines a linear address space with these characteristics:
- Byte-addressable memory
- Platform-dependent size (typically matching pointer width)
- Zero-based addressing

### Memory Sections

COIL programs organize memory into sections:

| Section Type | Purpose | Common Attributes |
|--------------|---------|-------------------|
| Code Section | Executable instructions | Executable, Readable |
| Data Section | Initialized program data | Readable, Writable |
| BSS Section | Zero-initialized data | Readable, Writable, Uninitialized |
| Stack Section | Function calls and local variables | Readable, Writable |
| Heap Section | Dynamically allocated memory | Readable, Writable |

Section attributes are defined as bit flags:
- `0x01`: Executable - Instructions can be fetched
- `0x02`: Writable - Data can be written
- `0x04`: Readable - Data can be read
- `0x08`: Initialized - Contains initialized data
- `0x10`: Uninitialized - Contains zero-filled data

Example section definition:
```
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
```

## Scope-Based Memory Management

COIL provides automatic memory management through scopes:

### Scope Creation and Destruction

```
SCOPEE       ; Enter a new scope
  ; Variables and code in this scope
SCOPEL       ; Leave the scope (and destroy all variables declared within it)
```

### Memory Management Process

1. **Resource Allocation**:
   - Variables are allocated when declared in a scope
   - Scope tracks all resources created within it

2. **Automatic Cleanup**:
   - At scope exit, all variables in that scope are released
   - Register and memory resources are reclaimed
   - No manual deallocation required

3. **Nested Scopes**:
   - Scopes can be nested to create hierarchies
   - Inner scopes can access resources from outer scopes
   - When an inner scope exits, only its own resources are released

## Memory Access Rules

### Alignment Requirements

COIL defines alignment requirements for memory access:

1. **Natural Alignment**: By default, accesses use natural alignment:
   - 1-byte types: 1-byte alignment
   - 2-byte types: 2-byte alignment
   - 4-byte types: 4-byte alignment
   - 8-byte types: 8-byte alignment
   - 16-byte types: 16-byte alignment
   - Vector types: Size-dependent alignment

2. **Explicit Alignment Control**:
   - The `ALIGN` directive forces alignment of subsequent data

### Memory Access Modes

COIL supports different memory access patterns:

1. **Direct Access**:
   ```
   ; Access memory at fixed address
   MOV [0x1000], 42
   
   ; Access memory through pointer
   MOV [ptr], value
   ```

2. **Indexed Access**:
   ```
   ; Base + offset
   MOV [base_ptr + 8], value
   
   ; Base + scaled index
   MOV [array_ptr + index*4], value
   ```

3. **Block Operations**:
   ```
   ; Copy memory blocks
   MEMCPY dest_ptr, src_ptr, size
   
   ; Fill memory
   MEMSET buffer, 0, size
   ```

## Memory Ordering Model

### Sequential Consistency

By default, COIL follows a sequentially consistent memory model:
- Operations appear to execute in program order
- All processors observe the same global ordering of operations
- Visibility of changes is immediate to all processors

### Atomic Operations

COIL provides atomic operations for thread synchronization:

1. **Compare and Swap**:
   ```
   ; Atomic compare and swap
   CAS [lock_address], expected, new, TYPE_PARAM0=MEMORY_CTRL_ATOMIC
   ```

2. **Exchange**:
   ```
   ; Atomic exchange
   XCHG [shared_var], new_value, TYPE_PARAM0=MEMORY_CTRL_ATOMIC
   ```

## Memory Safety

COIL provides mechanisms for memory safety:

### Bounds Checking

1. **Static Bounds Checking**:
   - Compile-time checking where possible
   - Constant index bounds can be validated early

2. **Dynamic Bounds Checking**:
   - Optional runtime bounds checking
   - Configuration control for balancing safety and performance

### Type Safety

Memory access should respect type safety:

1. **Type Compatibility**:
   - Memory access should use compatible types
   - Type punning requires explicit CAST operations

2. **Pointer Safety**:
   - Pointer arithmetic is scaled by the pointed-to type size