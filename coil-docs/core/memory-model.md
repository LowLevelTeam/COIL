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

3. **Stack**: Function call stack and local variables
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

**Implementation Requirements:**
1. **Permission Flags**: Memory protections are represented as flags in section headers:
   - `READ` (0x01): Page is readable
   - `WRITE` (0x02): Page is writable
   - `EXEC` (0x04): Page is executable

2. **Page Granularity**: Protections apply at page granularity (typically 4KB)

3. **Protection Violations**: When detected, must:
   - Generate a specific exception type (MEMORY_PROTECTION_VIOLATION)
   - Include fault address and attempted access type in exception

### Flat Memory

Simple model without protection:

- Single address space with no permissions
- No overflow detection
- Suitable for embedded systems without MMU
- More efficient but less safe

**Implementation Requirements:**
1. **Direct Access**: All memory addresses are directly accessible
2. **No Bounds Checking**: No runtime bounds checking performed
3. **Optimization**: More aggressive optimizations allowed
4. **Platform Restriction**: Limited to platforms without MMU

### Segmented Memory

Model with explicit segments:

- Memory divided into logical segments (code, data, stack)
- Explicit segment selection for memory operations
- Compatible with segmented architectures
- Supports legacy systems

**Implementation Requirements:**
1. **Segment Registers**: Platform-specific segment registers or equivalent
2. **Segment Encoding**: Instructions encode segment identifier
3. **Bounds Checking**: Segment bounds are enforced

## Stack Frame Organization

### Stack Frame Structure

A stack frame is the memory area allocated during a function call:

```
Higher Address │ Previous Frame  │
               ├────────────────┤
               │ Return Address │
               ├────────────────┤
               │ Saved Registers│
               ├────────────────┤
               │ Local Variables│
               ├────────────────┤
Lower Address  │ Temporary Space│
               └────────────────┘
```

### Standard Stack Frame Layout

For a downward-growing stack (default), the exact byte-level layout is:

```
Offset from SP │ Content             │ Alignment │ Size
───────────────┼─────────────────────┼───────────┼────────
+0             │ Temporary Space     │ 8-byte    │ Variable
+X             │ Local Variables     │ Natural*  │ Variable
+Y             │ Callee-Saved Regs   │ Natural*  │ Platform-specific
+Z             │ Frame Pointer (opt) │ Natural*  │ Platform-specific
+Z+N           │ Return Address      │ Natural*  │ Platform-specific
+Z+N+M         │ Parameters (if any) │ Natural*  │ Variable
```

* Natural alignment means each value is aligned according to its type requirements (e.g., 4 bytes for INT32, 8 bytes for FP64).

### Function Prologue/Epilogue

Every function with a stack frame must have:

1. **Prologue** (function entry):
   ```
   - Allocate stack space
   - Save return address
   - Save callee-saved registers
   - Initialize frame pointer (if used)
   - Initialize local variables
   ```

2. **Epilogue** (function exit):
   ```
   - Destroy local variables (run finalizers if needed)
   - Restore callee-saved registers
   - Restore frame pointer (if used)
   - Deallocate stack space
   - Return to caller
   ```

## Variable Scoping

Variables have explicit scope defined by SCOPE/SCOPL instructions:

1. **Global Scope**:
   - Accessible throughout the program
   - Allocated in .data or .bss section
   - Initialized at program start

2. **Function Scope**:
   - Limited to the function
   - Allocated on stack by default
   - Initialized on each function entry
   - Destroyed on function exit

3. **Block Scope**:
   - Limited to the defined block
   - Allocated on stack
   - Exists between SCOPE and SCOPL
   - Variables are automatically destroyed when leaving scope

## Addressing Modes

COIL supports various memory addressing modes:

1. **Direct**: Access memory at a specific address
   - Example: `[0x1000]`
   - Binary encoding: `[mem_type][address]`

2. **Variable**: Direct reference to a variable
   - Example: `var_id`
   - Binary encoding: `[var_type][var_id]`

3. **Variable Indirect**: Use address from a variable
   - Example: `[var_id]`
   - Binary encoding: `[mem_type][var_type][var_id]`

4. **Based**: Variable plus offset
   - Example: `[var_id + 8]`
   - Binary encoding: `[mem_type][var_type][var_id][imm_type][offset]`

5. **Indexed**: Base variable plus index variable (optionally scaled)
   - Example: `[base + index * scale]`
   - Binary encoding: `[mem_type][var_type][base_id][var_type][index_id][imm_type][scale]`

6. **Based Indexed**: Base variable plus index variable plus offset
   - Example: `[base + index * scale + offset]`
   - Binary encoding: `[mem_type][var_type][base_id][var_type][index_id][imm_type][scale][imm_type][offset]`

## Memory Alignment

Memory alignment requirements are specified in the configuration:

1. **Natural Alignment**:
   - By default, data is aligned to its natural boundary:
     - 1-byte values: 1-byte alignment
     - 2-byte values: 2-byte alignment
     - 4-byte values: 4-byte alignment
     - 8-byte values: 8-byte alignment
     - 16-byte values and larger: 16-byte alignment

2. **Structure Alignment**:
   - Structures align to their largest member by default
   - Padding inserted between members as needed
   - Total structure size is rounded up to alignment boundary

3. **Explicit Alignment Control**:
   - Types can request specific alignment requirements
   - Alignment must be power of 2
   - Maximum alignment is platform-specific

## Endianness

Memory byte order is specified in the configuration:

1. **Little Endian**: Least significant byte at lowest address
   - Default for x86, ARM (most modes), RISC-V
   - Example: 0x1234 stored as [0x34, 0x12]

2. **Big Endian**: Most significant byte at lowest address
   - Used in some network protocols, SPARC, older PowerPC
   - Example: 0x1234 stored as [0x12, 0x34]

3. **Endianness Conversion**:
   - Explicit conversion functions provided by standard library
   - Binary format includes endianness marker
   - Network protocols typically use big-endian (network byte order)

## Error Handling

Memory-related errors are handled through these mechanisms:

1. **Null Pointer Access**:
   - Dereferencing null (0) pointer raises exception
   - Exception type: NULL_POINTER_EXCEPTION
   - Cannot be disabled even in flat memory model

2. **Out of Bounds Access**:
   - Accessing memory outside allocated region
   - Exception type: BOUNDS_VIOLATION
   - May only be detected in protected memory model

3. **Protection Violation**:
   - Accessing memory without proper permissions
   - Exception type: PROTECTION_VIOLATION
   - Includes type (read/write/execute)

4. **Stack Overflow**:
   - Exceeding stack size limit
   - Exception type: STACK_OVERFLOW
   - May be handled by OS or runtime

## Implementation Requirements

Conforming implementations must:

1. Support at least the Protected Memory Model
2. Correctly handle all addressing modes
3. Implement variable scoping with SCOPE/SCOPL
4. Respect alignment requirements
5. Properly handle endianness conversion
6. Generate appropriate exceptions for memory errors
