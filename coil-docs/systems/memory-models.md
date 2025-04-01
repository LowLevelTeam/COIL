# Memory Models

## Overview

COIL supports multiple memory models to accommodate different device architectures. Each model defines how memory is organized, accessed, and synchronized across processing elements.

## Memory Model Types

COIL defines four primary memory models:

| Model Code | Name | Description | Typical Devices |
|------------|------|-------------|----------------|
| 0x01 | Flat | Single address space | CPUs |
| 0x02 | Segmented | Multiple memory segments | Some CPUs, microcontrollers |
| 0x03 | Hierarchical | Hierarchical memory | GPUs |
| 0x04 | Distributed | Distributed memory system | Multi-node systems |

### Flat Memory Model (0x01)

The flat memory model provides a single, continuous address space:

- All memory locations accessible through linear addressing
- No explicit distinctions between memory regions
- Standard for modern CPU architectures

Example:

```
; Load value from address
MOV #1, [0x1000]

; Store value to address
MOV [0x2000], #1

; Pointer-based access
MOV #2, #3          ; Copy address to pointer
MOV #4, [#2]        ; Load through pointer
```

### Segmented Memory Model (0x02)

The segmented model divides memory into distinct segments:

- Memory references include both segment and offset
- Different access rules may apply to different segments
- Common in some microcontrollers and legacy architectures

Example:

```
; Load from data segment
MOV #1, [DATA:0x100]

; Load from code segment
MOV #2, [CODE:0x200]

; Store to stack segment
MOV [STACK:0x50], #3
```

### Hierarchical Memory Model (0x03)

The hierarchical model includes multiple memory levels with different performance characteristics:

- Explicit distinction between memory levels (global, shared, local, etc.)
- Different visibility and scope for each level
- Typical for GPU architectures

Example:

```
; Load from global memory
LOAD.GLOBAL #1, [0x1000]

; Load from shared memory
LOAD.SHARED #2, [0x100]

; Store to local memory
STORE.LOCAL [0x10], #3
```

### Distributed Memory Model (0x04)

The distributed model spans multiple processing nodes:

- Memory distributed across separate address spaces
- Explicit communication between nodes
- Used in multi-node systems and some specialized accelerators

Example:

```
; Local memory access
MOV #1, [0x1000]

; Remote memory access
REMOTE_LOAD #2, NODE_2, 0x2000

; Send data to another node
SEND NODE_3, #3, 8
```

## Unified Memory Abstraction

COIL provides a unified memory abstraction for cross-device access:

```
; Declare unified memory accessible by all devices
UNIFIED_MEM buffer, size

; Access from CPU
PROC 0x01
MOV [buffer], value

; Access from GPU
PROC 0x02
MOV value, [buffer]
```

This abstraction allows seamless memory sharing between devices with different memory models.

## Memory Synchronization

Different memory models require different synchronization mechanisms:

### Flat Memory Synchronization

```
; Memory barrier
BARRIER

; Atomic operation
ATOMIC_ADD [addr], value
```

### Hierarchical Memory Synchronization

```
; Synchronize threads within a block
SYNC_THREADS

; Synchronize memory levels
MEM_BARRIER GLOBAL_TO_SHARED
```

### Distributed Memory Synchronization

```
; Global barrier across nodes
GLOBAL_BARRIER

; Synchronize specific nodes
SYNC_NODES NODE_1, NODE_2, NODE_3
```

## Memory Operations

Common memory operations across all models:

```
; Block operations
MEMCPY dest, src, size     ; Copy memory block
MEMSET buffer, value, size ; Fill memory block  
MEMCMP result, buf1, buf2, size ; Compare memory blocks

; Atomic operations  
XCHG [lock_var], value     ; Atomic exchange
CAS [lock_var], expected, new_value ; Compare and swap
```

## Device-Specific Memory Management

### CPU Memory Management

```
; Stack allocation
ALLOCA #1, size            ; Allocate on stack
```

### GPU Memory Management

```
; Shared memory allocation
SHARED_ALLOC #1, size      ; Allocate in shared memory

; Texture memory binding
BIND_TEXTURE tex, buffer   ; Bind buffer to texture
```

## Cross-Device Memory Transfer

Explicit data transfer between devices with different memory models:

```
; Copy data from CPU to GPU
COPY gpu_buffer, cpu_buffer, size, 0x01, 0x02

; Copy data from GPU to CPU
COPY cpu_buffer, gpu_buffer, size, 0x02, 0x01
```

## Memory Model Query

Programs can query the current memory model:

```
; Get current memory model
QUERY_MEM_MODEL #1         ; Result in #1

; Branch based on memory model
CMP #1, 0x03
BR_EQ hierarchical_path   ; Branch if hierarchical
```

This allows code to adapt to different memory environments.