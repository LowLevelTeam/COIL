# COIL Memory Model

## Overview

The COIL Memory Model defines how programs interact with memory, providing a consistent abstraction across different hardware platforms while allowing for platform-specific optimizations.

## Memory Organization

COIL organizes memory into logical regions:

```
+----------------+ High Address
| Stack          |
|                |
+----------------+
|     ↓          |
|                |
|     ↑          |
|                |
+----------------+
| Heap           |
|                |
+----------------+
| Global Data    |
| - Initialized  |
| - Uninitialized|
+----------------+
| Program Code   |
|                |
+----------------+ Low Address
```

### Memory Regions

1. **Program Code**: Contains executable instructions
   - Read-only and executable
   - Fixed size determined at compile-time
   - Typically mapped to lower addresses

2. **Global Data**: Contains program-wide variables
   - Subdivided into initialized and uninitialized (BSS) segments
   - Fixed size determined at compile-time
   - Read-write permissions

3. **Heap**: Dynamic memory allocation region
   - Grows upward from lower to higher addresses
   - Variable size, expands as needed
   - Managed through allocation and deallocation operations

4. **Stack**: Function call stack and local variables
   - Grows downward from higher to lower addresses by default (configurable)
   - Automatic allocation and deallocation with function scope
   - Limited size, typically fixed at program start

## Memory Models

COIL supports multiple memory protection models:

### Protected Memory

Default model with access protection:

- Memory regions have distinct permissions (read, write, execute)
- Accesses outside allocated regions raise exceptions
- Stack and buffer overflow protection
- Hardware Memory Management Unit (MMU) support

### Flat Memory

Simple model without protection:

- Single address space with no permissions
- No overflow detection
- Suitable for embedded systems without MMU
- More efficient but less safe

### Segmented Memory

Model with explicit segments:

- Memory divided into logical segments (code, data, stack)
- Explicit segment selection for memory operations
- Compatible with segmented architectures
- Supports legacy systems

## Addressing Modes

COIL supports various memory addressing modes:

Variable could be replaced with register for specific use cases.

1. **Direct**: Access a specific memory address
   - Example: `[0x1000]`

2. **Variable Indirect**: Use address in a variable
   - Example: `[reg]`

3. **Based**: variable plus offset
   - Example: `[reg + 8]`

4. **Indexed**: Base variable plus index variable (optionally scaled)
   - Example: `[base + index * scale]`

5. **Based Indexed**: Base variable plus index variable plus offset
   - Example: `[base + index * scale + offset]`

## Memory Alignment

Memory alignment requirements are specified in the configuration:

- Default alignment based on target architecture
- Structures align to their largest member by default
- Explicit alignment control available
- Performance implications for misaligned access

## Endianness

Memory byte order is specified in the configuration:

- Little Endian: Least significant byte at lowest address
- Big Endian: Most significant byte at lowest address
- Mixed Endian: Platform-specific ordering

## Threading Model

For multi-threaded environments:

- Each thread has its own stack
- Heap is shared between threads
- Global data is shared unless specified as thread-local
- Synchronization primitives available for shared memory

## Memory Consistency

COIL defines memory ordering guarantees:

- Sequential consistency for synchronized operations
- Relaxed ordering for performance-critical operations
- Memory barriers for explicit ordering control
- Atomic operations for thread-safe modifications

## Variable Scoping

Variables have explicit scope defined by SCOPE/SCOPL instructions:

- Global scope: Accessible throughout the program
- Function scope: Limited to the function
- Block scope: Limited to the defined block
- Variables are automatically destroyed when leaving scope

## Related Components

- [Object Format](../format/object-format.md) - Object file organization
- [Memory Operations](../isa-u/memory-operations.md) - Memory management instructions
- [Configuration Format](./config-format.md) - Memory model configuration