# COIL Memory Model Specification

This document defines the memory model for COIL v1, establishing the rules for memory access, ordering, sharing, and management. A well-defined memory model is essential for consistent behavior across different implementations and architectures.

## 1. Memory Organization

### 1.1 Address Space

COIL defines a linear address space with the following characteristics:
- Byte-addressable memory
- Platform-dependent size (typically matching the pointer width)
- Zero-based addressing
- No reserved regions in the core model

Architecture-specific implementations may have:
- Reserved address ranges
- Segmented memory access methods
- Memory-mapped resources

### 1.2 Memory Sections

COIL programs organize memory into sections, each with specific attributes:
- **Code Sections**: Contain executable instructions
- **Data Sections**: Contain program data
- **Initialized Data**: Preset values loaded from the COIL object
- **Uninitialized Data**: Zero-initialized by default
- **Stack Section**: Manages function calls and local variables
- **Heap Section**: Dynamically allocated memory (if supported)

Section attributes control:
- **Executable**: Instructions can be fetched from this section
- **Readable**: Data can be read from this section
- **Writable**: Data can be written to this section
- **Shared**: Section can be shared between multiple contexts

## 2. Memory Access Rules

### 2.1 Alignment Requirements

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
   - May be emulated on architectures that don't support it natively
   - May incur performance penalties

### 2.2 Endianness

COIL supports both little-endian and big-endian memory access:

1. **Default Endianness**:
   - Inherited from the target architecture
   - Specified in the configuration

2. **Explicit Endianness Control**:
   - For binary I/O operations
   - For cross-architecture compatibility
   - Using `BSWAP` and related instructions

### 2.3 Memory Access Sizes

Memory operations work with specific sizes:

1. **Typed Access**:
   - Access size determined by the operand type
   - Ranges from 1 byte to architecture limits

2. **Block Operations**:
   - `MEMCPY`, `MEMSET`, `MEMCMP` for block operations
   - Size specified explicitly

3. **Partial Register Access**:
   - Access to parts of registers follows architecture conventions
   - May have specific alignment and size requirements

## 3. Memory Ordering Model

### 3.1 Sequential Consistency

By default, COIL follows a sequentially consistent memory model:
- Operations appear to execute in program order
- All processors observe the same global ordering of operations
- Visibility of changes is immediate to all processors

### 3.2 Relaxed Memory Ordering

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

### 3.3 Atomic Operations

COIL provides atomic operations for thread synchronization:

1. **Atomic Instructions**:
   - `CAS` (Compare and Swap)
   - `XCHG` (Exchange)
   - Atomic arithmetic operations

2. **Atomicity Guarantees**:
   - All-or-nothing execution
   - No partial updates visible
   - Ordering based on specified memory ordering

## 4. Variable and Register Management

### 4.1 Variable System

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

### 4.2 Register Allocation

COIL processors perform register allocation:

1. **Allocation Strategy**:
   - Based on variable usage patterns
   - Respecting architecture-specific constraints
   - Honoring ABI register requirements

2. **Register Spilling**:
   - Automatic when register pressure is high
   - Explicit with the `PUSH` instruction
   - May save/restore around function calls

3. **Register Preservation**:
   - Based on ABI caller-saved/callee-saved conventions
   - May be optimized away when safe

## 5. Memory Safety

### 5.1 Bounds Checking

COIL defines rules for bounds checking:

1. **Static Bounds Checking**:
   - Performed at compile-time where possible
   - Enforced for constant indices

2. **Dynamic Bounds Checking**:
   - Optional runtime checking
   - Configuration-controlled
   - Implementation-defined handling of violations

### 5.2 Type Safety

Memory access respects type safety rules:

1. **Type Compatibility**:
   - Access must use compatible types
   - Type punning requires explicit `CAST` instructions

2. **Pointer Arithmetic**:
   - Properly scaled by pointed-to type size
   - Bounds-checked when enabled

### 5.3 Memory Isolation

COIL supports memory isolation mechanisms:

1. **Section Protection**:
   - Based on section attributes
   - May map to hardware protection mechanisms

2. **Stack Protection**:
   - Stack guards may be implemented
   - Stack probing for large allocations

## 6. Memory Management

### 6.1 Scope-Based Memory

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

### 6.2 Stack Management

Stack operations follow these rules:

1. **Stack Growth**:
   - Direction determined by architecture
   - Automatic for function calls
   - Explicit with `PUSH`/`POP`

2. **Stack Alignment**:
   - Follows ABI requirements
   - May be architecture-specific

3. **Stack Limits**:
   - Implementation-defined maximum size
   - Optional overflow checking

### 6.3 Dynamic Memory (Optional)

COIL v1 optionally supports dynamic memory:

1. **Allocation**:
   - Implementation-defined mechanism
   - Not part of core instruction set

2. **Deallocation**:
   - Explicit release required
   - No automatic garbage collection

## 7. Architecture-Specific Memory Features

### 7.1 x86-64 Memory Model

Specific features for x86-64:
- Complex addressing modes (base+index*scale+displacement)
- Segment registers for memory access
- Memory prefetch instructions
- Non-temporal memory hints

### 7.2 ARM Memory Model

Specific features for ARM:
- Exclusive load/store for atomic operations
- Pre/post-indexed addressing modes
- Memory barrier instructions
- Unaligned access control

### 7.3 RISC-V Memory Model

Specific features for RISC-V:
- Simple load/store architecture
- Hardware page-based protection
- Atomic memory operations
- Memory model control via fence instructions

## 8. Implementation Requirements

A COIL v1 processor must:

1. Support the complete memory model as defined
2. Handle all memory access types and sizes
3. Correctly implement scope-based variable management
4. Support alignment requirements
5. Implement memory ordering as specified
6. Provide atomic operations
7. Map COIL memory operations to architecture-specific instructions

### 8.1 Minimal Requirements

At minimum, implementations must:
1. Support byte addressable memory
2. Implement naturally aligned access
3. Support all core memory operations
4. Provide scope-based variable management
5. Implement stack operations

### 8.2 Optional Features

Implementations may optionally provide:
1. Dynamic memory management
2. Enhanced bounds checking
3. Memory protection mechanisms
4. Advanced optimization for memory access patterns

## 9. Memory Model Examples

### 9.1 Basic Variable Usage

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

### 9.2 Memory Ordering Example

```
; Sequential consistency for shared data
MOV [shared_data], TYPE_RGP=RAX, TYPE_PARAM0=MEMORY_CTRL_SEQ_CST

; Relaxed ordering for thread-local data
MOV [local_data], TYPE_RGP=RBX, TYPE_PARAM0=MEMORY_CTRL_RELAXED
```

### 9.3 Atomic Operation Example

```
; Atomic compare-and-swap for lock acquisition
MOV TYPE_RGP=RAX, 0          ; Expected: unlocked (0)
MOV TYPE_RGP=RDX, 1          ; New: locked (1)
CAS [lock_address], TYPE_RGP=RAX, TYPE_RGP=RDX

; Check if lock was acquired
CMP TYPE_RGP=RAX, 0
BR_NE lock_failed
```

## 10. Advanced Memory Concepts

### 10.1 Cache Control (Architecture-Specific)

Implementations may provide cache control:
1. Cache prefetch hints
2. Cache bypass options
3. Cache line flush operations

### 10.2 Memory-Mapped I/O

COIL supports memory-mapped I/O:
1. Through normal memory operations
2. With appropriate MEMORY_CTRL_VOLATILE flag
3. Using architecture-specific addressing

### 10.3 Shared Memory

For multi-threaded applications:
1. Atomic operations ensure proper synchronization
2. Memory ordering controls visibility
3. Architecture-specific optimizations may apply