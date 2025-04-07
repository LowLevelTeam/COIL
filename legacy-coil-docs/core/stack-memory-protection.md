# Stack Frame Organization and Memory Protection

## Overview

This document provides detailed specifications for stack frame organization and memory protection mechanisms in COIL. These specifications ensure consistent implementation across different platforms and provide clear guidelines for memory management.

## Stack Frame Organization

### Stack Frame Structure

A stack frame is the memory area allocated during a function call that contains local variables, saved registers, and control information. The general structure is:

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

### Detailed Stack Frame Components

1. **Return Address**
   - Platform-specific size (typically 4 or 8 bytes)
   - Always stored at a fixed offset from frame start
   - Aligned to platform's address size boundary

2. **Saved Registers**
   - Only callee-saved registers as defined by the ABI
   - Stored in a platform-specific order
   - Each register aligned to its natural boundary

3. **Local Variables**
   - Organized to minimize padding while maintaining alignment
   - Each variable aligned to its natural boundary
   - Lifetimes managed through SCOPE/SCOPL instructions

4. **Temporary Space**
   - Used for expression evaluation and short-lived values
   - No specific alignment guarantees
   - Implementation can reuse this space aggressively

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

### ABI-Specific Examples

#### SystemV x86-64 Stack Frame

For the SystemV ABI on x86-64:

```
Offset from RSP │ Content             │ Size
────────────────┼─────────────────────┼──────
+0              │ Red Zone (128 bytes)│ 128 bytes
+128            │ Local Variables     │ Variable
+X              │ Saved Registers     │ Variable
+Y              │ Return Address      │ 8 bytes
+Y+8            │ Parameters 7+       │ Variable
```

- 16-byte stack alignment at call sites
- First 6 integer/pointer parameters in registers (RDI, RSI, RDX, RCX, R8, R9)
- First 8 floating-point parameters in XMM registers
- Red zone available for leaf functions

#### ARM AAPCS Stack Frame

For the ARM AAPCS:

```
Offset from SP  │ Content             │ Size
────────────────┼─────────────────────┼──────
+0              │ Local Variables     │ Variable
+X              │ Saved Registers     │ Variable
+Y              │ Return Address (LR) │ 4 bytes
+Y+4            │ Parameters 5+       │ Variable
```

- 8-byte stack alignment at call sites
- First 4 parameters in registers (R0-R3)
- No red zone

## Memory Protection Mechanisms

### Protection Model

COIL's default memory protection model ensures that:

1. Memory regions have distinct permissions (read, write, execute)
2. Accesses outside allocated regions are detected and prevented
3. Stack and buffer overflow protection is provided
4. Hardware Memory Management Unit (MMU) support is leveraged when available

### Permission Flags

Memory protections are represented as flags in section headers:

| Flag    | Value  | Description              |
|---------|--------|--------------------------|
| READ    | 0x01   | Memory can be read       |
| WRITE   | 0x02   | Memory can be written    |
| EXEC    | 0x04   | Memory can execute       |
| INIT    | 0x08   | Memory is initialized    |
| UNINIT  | 0x10   | Memory is uninitialized  |
| ALLOC   | 0x20   | Memory should be allocated at runtime |

### Protection Implementation

Implementations must provide memory protection through one of these mechanisms:

1. **Hardware MMU-Based Protection**
   - Use page tables to map virtual addresses to physical addresses
   - Set protection bits in page table entries
   - Trap page faults for access violations
   - Map protection flags to hardware-specific page protection:
     ```
     COIL         Hardware (x86)       Hardware (ARM)
     ─────────────────────────────────────────────────
     READ         PAGE_READONLY        PROT_READ
     READ+WRITE   PAGE_READWRITE       PROT_READ|PROT_WRITE
     EXEC         PAGE_EXECUTE         PROT_EXEC
     READ+EXEC    PAGE_EXECUTE_READ    PROT_READ|PROT_EXEC
     ```

2. **Software-Based Protection**
   - For platforms without MMU support
   - Implement access checking before each memory operation
   - Maintain metadata about memory regions and their permissions
   - Throw exceptions for invalid accesses

### Protection Granularity

Protection is applied at different granularity levels:

1. **Page-Level** (typically 4KB)
   - Hardware-enforced through MMU
   - All addresses within a page share the same protection

2. **Object-Level**
   - Software-enforced for finer-grained protection
   - Applied to individual variables or allocations

### Protection Exception Handling

When a protection violation is detected:

1. **Exception Information**
   - Violation type (read, write, execute)
   - Address that caused the violation
   - Access size and alignment
   - Current instruction pointer

2. **Exception Types**
   - `MEMORY_PROTECTION_VIOLATION`: Access with insufficient permissions
   - `MEMORY_ACCESS_VIOLATION`: Access to unmapped/unallocated memory
   - `STACK_OVERFLOW`: Stack expansion exceeds limits
   - `NULL_POINTER_EXCEPTION`: Access through null (0) pointer

3. **Recovery Options**
   - Terminate execution (default)
   - Transfer to exception handler if configured
   - Generate error report with memory state

### Implementation Requirements

1. **Protection Verification**
   - Verify protection settings during section loading
   - Apply appropriate hardware or software mechanisms

2. **Permission Transitions**
   - Support changing permissions on existing memory regions
   - Validate permission change requests
   - Apply changes using appropriate platform mechanisms

3. **Stack Protection**
   - Implement stack guards or canaries
   - Verify stack bounds on function entry/exit
   - Detect and handle stack overflow conditions

4. **Reporting**
   - Provide detailed information for protection violations
   - Include memory maps in error reports where possible