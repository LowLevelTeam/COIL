# COIL Atomic Operations

## Overview

This document defines atomic operations in COIL, their semantics, and implementation requirements. Atomic operations provide synchronization primitives and memory ordering guarantees essential for concurrent programming.

## Atomic Types

Any COIL type can be made atomic using the ATOMIC type extension flag:

```
[type_opcode][ATOMIC_FLAG | other_flags][type_data...]
```

Example: Atomic INT32
```
[0x04][0x04]  // INT32 with ATOMIC flag (0x04)
```

## Memory Order Models

COIL defines memory ordering models for atomic operations:

| Value | Name | Description |
|-------|------|-------------|
| 0x00  | RELAXED | No ordering constraints beyond atomicity |
| 0x01  | ACQUIRE | Operation acts as an acquire fence |
| 0x02  | RELEASE | Operation acts as a release fence |
| 0x03  | ACQ_REL | Operation acts as both acquire and release fence |
| 0x04  | SEQ_CST | Sequential consistency (strongest guarantee) |

## Atomic Instructions

### Atomic Memory Operations (0x70-0x7F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x70   | ATOMLD   | Atomic load | 3 | Core |
| 0x71   | ATOMST   | Atomic store | 3 | Core |
| 0x72   | ATOMXCHG | Atomic exchange | 3 | Core |
| 0x73   | ATOMCMPXCHG | Atomic compare and exchange | 4-5 | Core |
| 0x74   | ATOMADD  | Atomic add | 3-4 | Core |
| 0x75   | ATOMSUB  | Atomic subtract | 3-4 | Core |
| 0x76   | ATOMAND  | Atomic AND | 3-4 | Core |
| 0x77   | ATOMOR   | Atomic OR | 3-4 | Core |
| 0x78   | ATOMXOR  | Atomic XOR | 3-4 | Core |
| 0x79   | ATOMMAX  | Atomic maximum | 3-4 | Core |
| 0x7A   | ATOMMIN  | Atomic minimum | 3-4 | Core |
| 0x7B   | ATOMFENCE| Memory fence | 1 | Core |
| 0x7C   | ATOMINCR | Atomic increment | 2-3 | Core |
| 0x7D   | ATOMDECR | Atomic decrement | 2-3 | Core |

## Detailed Instruction Behaviors

### ATOMLD (0x70)

Atomic load operation:

```
ATOMLD dest, source, order
```

- `dest`: Destination for loaded value
- `source`: Memory location to load from
- `order`: Memory ordering constraint (one of the memory order constants)

Behavior:
1. Atomically loads the value from `source` memory location
2. Stores the loaded value in `dest`
3. Applies the specified memory ordering constraint
4. No flags are affected

### ATOMCMPXCHG (0x73)

Atomic compare and exchange operation:

```
ATOMCMPXCHG dest, ptr, expected, desired, order
```

- `dest`: Result flag (1 if exchange occurred, 0 otherwise)
- `ptr`: Pointer to memory location
- `expected`: Expected value at memory location
- `desired`: Value to store if comparison succeeds
- `order`: Memory ordering constraint

Behavior:
1. Atomically compares the value at `ptr` with `expected`
2. If equal, replaces with `desired` and sets `dest` to 1
3. If not equal, loads current value into `expected` and sets `dest` to 0
4. Applies the specified memory ordering constraint
5. The Z flag is set based on whether the exchange occurred

### ATOMFENCE (0x7B)

Memory fence operation:

```
ATOMFENCE order
```

- `order`: Memory ordering constraint

Behavior:
1. Inserts a memory barrier with the specified ordering
2. Ensures memory operations are properly ordered across threads
3. Does not transfer data, only enforces ordering
4. No flags are affected

## Implementation on Different Platforms

### Hardware with Native Atomic Support

On platforms with native atomic operations:
1. Direct mapping to hardware atomic instructions
2. Memory ordering constraints mapped to hardware memory barriers
3. Full performance of hardware atomics

### Hardware without Native Atomic Support

On platforms without hardware atomics:
1. Software-based atomics implementation using locks
2. Possible performance penalties
3. May require global lock for certain operations

## Fallback Implementation Guidelines

When ATOMIC_OPS capability is not available:
1. Use software mutex implementation for exclusive access
2. Implement Compare-And-Swap (CAS) loops for operations
3. Provide clear performance warnings

## Sample Usage

### Atomic Increment

```
ATOMINCR counter, RELAXED
```

### Lock-Free Counter

```
start:
  ATOMLD old_val, counter, RELAXED
  ADD new_val, old_val, 1
  ATOMCMPXCHG success, counter, old_val, new_val, ACQ_REL
  CMP success, 0
  BR EQ, start
```

## Implementation Requirements

COIL implementations must:
1. **Atomicity**: Ensure operations appear indivisible
2. **Memory Ordering**: Correctly implement all memory order models
3. **Type Support**: Support atomic flag on all appropriate types
4. **Capability Declaration**: Indicate atomic support in capabilities
5. **Fallback**: Provide software implementation when hardware support unavailable