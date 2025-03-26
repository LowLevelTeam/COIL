# COIL Memory Model

## Purpose

This document defines the memory model for COIL, establishing the rules for memory organization, access, ordering, and management. A well-defined memory model is essential for consistent behavior across different implementations and processor types.

## Memory Organization

### Address Space

COIL defines a linear address space with the following characteristics:
- Byte-addressable memory
- Platform-dependent size (typically matching the pointer width)
- Zero-based addressing
- No reserved regions in the core model

Processing unit-specific implementations may have:
- Reserved address ranges
- Segmented memory access methods
- Memory-mapped resources

### Memory Sections

COIL programs organize memory into sections, each with specific attributes:
- **Code Sections**: Contain executable instructions
- **Data Sections**: Contain initialized program data
- **Uninitialized Data (BSS)**: Zero-initialized data
- **Stack Section**: Manages function calls and local variables
- **Heap Section**: Dynamically allocated memory (if supported)

Section attributes control:
- **Executable**: Instructions can be fetched from this section
- **Readable**: Data can be read from this section
- **Writable**: Data can be written to this section
- **Shared**: Section can be shared between multiple contexts

## Memory Access Rules

### Alignment Requirements

COIL defines alignment requirements for memory access:

1. **Natural Alignment**: By default, accesses should use natural alignment for the data type:
   - 1-byte types: 1-byte alignment
   - 2-byte types: 2-byte alignment
   - 4-byte types: 4-byte alignment
   - 8-byte types: 8-byte alignment
   - 16-byte types: 16-byte alignment
   - Vector types: Size-dependent alignment (typically 16, 32, or 64 bytes)

2. **Alignment Control**:
   - The `ALIGN` directive forces alignment of subsequent data
   - Type extensions can specify alignment requirements
   - Memory operations can specify alignment attributes

3. **Unaligned Access**:
   - Explicitly marked with `MEMORY_CTRL_UNALIGNED`
   - May be emulated on processor types that don't support it natively
   - May incur performance penalties

### Endianness

COIL supports both little-endian and big-endian memory access:

1. **Default Endianness**:
   - Inherited from the target processor
   - Specified in the configuration

2. **Explicit Endianness Control**:
   - For binary I/O operations
   - For cross-processor compatibility
   - Using `BSWAP` and related instructions

### Memory Access Sizes

Memory operations work with specific sizes:

1. **Typed Access**:
   - Access size determined by the operand type
   - Ranges from 1 byte to processor limits

2. **Block Operations**:
   - `MEMCPY`, `MEMSET`, `MEMCMP` for block operations
   - Size specified explicitly

3. **Partial Register Access**:
   - Access to parts of registers follows processor conventions
   - May have specific alignment and size requirements

## Memory Ordering Model

### Sequential Consistency

By default, COIL follows a sequentially consistent memory model:
- Operations appear to execute in program order
- All processors observe the same global ordering of operations
- Visibility of changes is immediate to all processors

### Relaxed Memory Ordering

For performance optimization, COIL supports relaxed memory models:

1. **Memory Barriers**:
   - Full barriers: Enforce ordering of all memory operations
   - Store barriers: Enforce ordering of store operations
   - Load barriers: Enforce ordering of load operations

2. **Ordering Attributes**:
   - `MEMORY_CTRL_ACQUIRE`: Acquire semantics for loads
   - `MEMORY_CTRL_RELEASE`: Release semantics for stores
   - `MEMORY_CTRL_RELAXED`: No ordering guarantees
   - `MEMORY_CTRL_SEQ_CST`: Sequential consistency

### Atomic Operations

COIL provides atomic operations for thread synchronization:

1. **Atomic Instructions**:
   - `CAS` (Compare and Swap)
   - `XCHG` (Exchange)
   - Atomic arithmetic operations

2. **Atomicity Guarantees**:
   - All-or-nothing execution
   - No partial updates visible
   - Ordering based on specified memory ordering

## Variable and Register Management

### Variable System

COIL's variable system provides an abstraction over registers and memory:

1. **Variable Declaration**:
   - Using the `VAR` instruction
   - Specified type, name, and optional initial value
   - Scope determined by surrounding `SCOPEE`/`SCOPEL`

2. **Variable Storage**:
   - COIL processor decides optimal storage location
   - May use registers for frequently accessed variables
   - May spill to stack when register pressure is high

3. **Variable Promotion/Demotion**:
   - `PUSH`/`POP` instructions can suggest storage preferences
   - Implementation-defined optimization strategies

### Register Allocation

COIL processors perform register allocation:

1. **Allocation Strategy**:
   - Based on variable usage patterns
   - Respecting processor-specific constraints
   - Honoring ABI register requirements

2. **Register Spilling**:
   - Automatic when register pressure is high
   - Explicit with the `PUSH` instruction
   - May save/restore around function calls

3. **Register Preservation**:
   - Based on ABI caller-saved/callee-saved conventions
   - May be optimized away when safe

## Memory Safety

### Bounds Checking

COIL defines rules for bounds checking:

1. **Static Bounds Checking**:
   - Performed at compile-time where possible
   - Enforced for constant indices

2. **Dynamic Bounds Checking**:
   - Optional runtime checking
   - Configuration-controlled
   - Implementation-defined handling of violations

### Type Safety

Memory access respects type safety rules:

1. **Type Compatibility**:
   - Access must use compatible types
   - Type punning requires explicit `CAST` instructions

2. **Pointer Arithmetic**:
   - Properly scaled by pointed-to type size
   - Bounds-checked when enabled

### Memory Isolation

COIL supports memory isolation mechanisms:

1. **Section Protection**:
   - Based on section attributes
   - May map to hardware protection mechanisms

2. **Stack Protection**:
   - Stack guards may be implemented
   - Stack probing for large allocations

## Memory Management

### Scope-Based Memory

COIL provides scope-based memory management:

1. **Scope Creation**:
   - `SCOPEE` instruction creates a new variable scope
   - Nested scopes are supported

2. **Scope Destruction**:
   - `SCOPEL` instruction destroys current scope
   - Automatically releases all variables in the scope

3. **Scope Implementation**:
   - May use stack frames
   - May optimize by reusing storage

### Stack Management

Stack operations follow these rules:

1. **Stack Growth**:
   - Direction determined by processor
   - Automatic for function calls
   - Explicit with `PUSH`/`POP`

2. **Stack Alignment**:
   - Follows ABI requirements
   - May be processor-specific

3. **Stack Limits**:
   - Implementation-defined maximum size
   - Optional overflow checking

### Dynamic Memory (Optional)

COIL v1 optionally supports dynamic memory:

1. **Allocation**:
   - Implementation-defined mechanism
   - Not part of core instruction set

2. **Deallocation**:
   - Explicit release required
   - No automatic garbage collection

## Processor-Specific Memory Features

COIL allows for processor-specific memory features through dedicated opcode ranges:

1. **Memory-Mapped I/O**:
   - Through normal memory operations
   - With appropriate `MEMORY_CTRL_VOLATILE` flag
   - Using processor-specific addressing

2. **Shared Memory**:
   - Atomic operations ensure proper synchronization
   - Memory ordering controls visibility
   - Processor-specific optimizations may apply

3. **Specialized Memory Types**:
   - Cache control operations in processor-specific ranges
   - Device-specific memory in v3

## Memory Model Examples

### Basic Variable Usage

```
; Scope-based variable example
SCOPEE
    ; Declare variables
    VAR TYPE_INT32, counter, 0
    VAR TYPE_INT32, max, 10
    
    ; Loop using variables
    SYM loop_start
    CMP counter, max
    BR_GE loop_end
    
    ; Use variable
    INC counter
    
    BR loop_start
    SYM loop_end
SCOPEL  ; Variables released here
```

### Memory Ordering Example

```
; Sequential consistency for shared data
MOV [shared_data], TYPE_RGP=RAX, TYPE_PARAM0=MEMORY_CTRL_SEQ_CST

; Relaxed ordering for thread-local data
MOV [local_data], TYPE_RGP=RBX, TYPE_PARAM0=MEMORY_CTRL_RELAXED
```

### Atomic Operation Example

```
; Atomic compare-and-swap for lock acquisition
MOV TYPE_RGP=RAX, 0          ; Expected: unlocked (0)
MOV TYPE_RGP=RDX, 1          ; New: locked (1)
CAS [lock_address], TYPE_RGP=RAX, TYPE_RGP=RDX

; Check if lock was acquired
CMP TYPE_RGP=RAX, 0
BR_NE lock_failed
```

## Memory Model Versioning

### Version 1
- Focus on CPU memory model
- Single-device memory operations
- Basic atomic operations

### Version 2
- Extended memory model for standard library
- Enhanced safety features
- More comprehensive atomic operations

### Version 3
- Multi-device memory model
- Cross-device memory transfers
- Device-specific memory optimizations
- Unified memory abstractions