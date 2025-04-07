# GPU Operations

**IMPORTANT**: Not a single instruction here is finalized

## Overview

GPU Operations are extended instructions (0xA0-0xBF) that work across all Graphics Processing Units regardless of their specific architecture. These instructions provide access to GPU-specific capabilities such as parallel execution, shared memory, and synchronization primitives.

**IMPORTANT**: These operations are only available when targeting a GPU processing unit. Use conditional compilation to ensure portability.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xA0   | BARRIER  | Synchronization barrier |
| 0xA1   | WGSIZE   | Get work group size |
| 0xA2   | WGID     | Get work group ID |
| 0xA3   | ATOMIC   | Atomic operations |
| 0xA4   | LMEM     | Local memory operations |
| 0xA5   | SMEM     | Shared memory operations |
| 0xA6   | GMEM     | Global memory operations |
| 0xA7   | WGSYNC   | Work group synchronization |
| 0xA8   | SHUFFLE  | Shuffle/exchange data between threads |
| 0xA9   | VOTE     | Thread voting operations |

## Detailed Specifications

### BARRIER (0xA0)

Creates a synchronization barrier for all threads in a work group.

**Encoding:**
```
[0xA0][0x01][scope: Operand]
```

**Behavior:**
- Blocks execution until all threads in the specified scope reach the barrier
- Ensures memory visibility (similar to a memory fence)
- Scope specifies synchronization level (workgroup, subgroup, etc.)
- Implementation adapts to the target GPU's synchronization mechanisms

### WGSIZE (0xA1)

Gets the size of the current work group.

**Encoding:**
```
[0xA1][0x02][dest: Operand][dimension: Operand]
```

**Behavior:**
- Retrieves the work group size for the specified dimension
- Dimension is an integral type (0 = X, 1 = Y, 2 = Z)
- Destination receives the size as an unsigned integer
- Common use case for parallel algorithm computations

### WGID (0xA2)

Gets the ID of the current work group.

**Encoding:**
```
[0xA2][0x02][dest: Operand][dimension: Operand]
```

**Behavior:**
- Retrieves the work group ID for the specified dimension
- Dimension is an integral type (0 = X, 1 = Y, 2 = Z)
- Destination receives the ID as an unsigned integer
- Used for partitioning work across multiple work groups

### ATOMIC (0xA3)

Performs an atomic operation on memory.

**Encoding:**
```
[0xA3][0x03][operation: Operand][dest: Operand][value: Operand]
```

**Behavior:**
- Performs the specified atomic operation on the destination memory location
- Operations include Add, Sub, And, Or, Xor, Min, Max, Exchange, Compare-and-Swap
- Guarantees atomic execution across all threads
- Returns the original value before modification
- Memory location must be properly aligned for the target architecture

### LMEM (0xA4)

Performs operations on local (thread-private) memory.

**Encoding:**
```
[0xA4][0x03][operation: Operand][dest: Operand][size: Operand]
```

**Behavior:**
- Allocates or manipulates local memory specific to the current thread
- Operations include Allocate, Free, Copy
- Size specifies the amount of memory in bytes
- Provides high-speed private storage for thread computation

### SMEM (0xA5)

Performs operations on shared (work group) memory.

**Encoding:**
```
[0xA5][0x03][operation: Operand][dest: Operand][size: Operand]
```

**Behavior:**
- Allocates or manipulates shared memory accessible by all threads in a work group
- Operations include Allocate, Free, Copy
- Size specifies the amount of memory in bytes
- Provides fast inter-thread communication within a work group

### GMEM (0xA6)

Performs operations on global memory.

**Encoding:**
```
[0xA6][0x03][operation: Operand][dest: Operand][size: Operand]
```

**Behavior:**
- Performs specialized operations on global memory
- Operations include prefetch, flush, invalidate cache
- Size specifies the affected memory range
- Allows manual optimization of memory access patterns

### WGSYNC (0xA7)

Synchronizes specific operations within a work group.

**Encoding:**
```
[0xA7][0x02][scope: Operand][flags: Operand]
```

**Behavior:**
- More flexible synchronization than BARRIER
- Scope specifies synchronization level (workgroup, subgroup)
- Flags specify what to synchronize (memory, execution, etc.)
- Allows fine-grained control over thread execution

### SHUFFLE (0xA8)

Exchanges data between threads in a work group.

**Encoding:**
```
[0xA8][0x04][dest: Operand][src: Operand][srcLane: Operand][width: Operand]
```

**Behavior:**
- Copies data from the source lane to the destination variable
- srcLane specifies the source thread ID relative to the current thread
- Width specifies the size of the thread group for wrap-around operations
- Enables efficient data sharing without using shared memory

### VOTE (0xA9)

Performs voting operations across threads in a work group.

**Encoding:**
```
[0xA9][0x03][operation: Operand][dest: Operand][predicate: Operand]
```

**Behavior:**
- Performs a voting operation based on a predicate across threads
- Operations include All, Any, Count
- Destination receives the result of the vote
- Enables thread cooperation and divergence handling

## Memory Model

GPU operations work within a hierarchical memory model:

1. **Private Memory**: Thread-specific, fastest access (LMEM)
2. **Shared Memory**: Work group visible, fast access (SMEM)
3. **Global Memory**: Visible to all work groups, slower access (GMEM)

Memory operations follow the synchronization and visibility rules of the target GPU architecture.

## Conditional Compilation

Always wrap GPU operations in appropriate conditional blocks:

```
IF PU_TYPE EQ GPU
    // GPU-specific code using GPU operations
ELSE
    // Alternative implementation for other processing units
EIF
```

## Related Components

- [Extended ISA Overview](./overview.md) - Extended instruction set summary
- [CPU Operations](./cpu-operations.md) - CPU-specific operations
- [Vector Operations](../isa-u/vector-operations.md) - Universal vector operations