# Extended Instruction Set Architecture

## Overview

The Extended Instruction Set Architecture (ISA-E) provides platform-specific instructions that are optimized for particular processing units, architectures, and modes. While not universally portable, these instructions enable high-performance code when targeting specific hardware platforms.

## Instruction Categories

Extended instructions are organized into three hierarchical layers:

1. **Processing Unit Operations** (0xA0-0xBF)
   - Work across all instances of a specific processing unit type
   - Example: All CPUs, regardless of architecture

2. **Architecture-Specific Operations** (0xC0-0xDF)
   - Work for specific architectures within a processing unit type
   - Example: x86-specific or ARM-specific CPU instructions

3. **Mode-Specific Operations** (0xE0-0xEF)
   - Work only for specific modes within an architecture
   - Example: x86-64 specific instructions that don't work in x86-32

## Compatibility Model

Extended instructions use a nested compatibility model:

```
Processing Unit (e.g., CPU)
  └── Architecture (e.g., x86)
       └── Mode (e.g., 64-bit)
```

Code using extended instructions should check for feature availability using conditional compilation:

```
IF PU_TYPE EQ CPU
    // CPU-specific code
    IF ARCH_TYPE EQ X86
        // x86-specific code
        IF MODE_TYPE EQ MODE_64
            // 64-bit mode specific code
        ENDIF
    ENDIF
ENDIF
```

## Processing Unit Categories

### CPU Operations (0xA0-0xBF)

Instructions that work across all central processing units:

```
INT = 0xA0      // Software interrupt
SYSCALL = 0xA1  // System call
SYSRET = 0xA2   // Return from system call
PUSHA = 0xA3    // Push all registers
POPA = 0xA4     // Pop all registers
PUSHFD = 0xA5   // Push flags register
POPFD = 0xA6    // Pop flags register
```

### GPU Operations (0xA0-0xBF)

Instructions that work across all graphics processing units:

```
BARRIER = 0xA0  // Synchronization barrier
WGSIZE = 0xA1   // Get work group size
WGID = 0xA2     // Get work group ID
ATOMIC = 0xA3   // Atomic operations
```

## Architecture-Specific Operations

Each processor unit type has architecture-specific operations:

### x86-Specific CPU Operations (0xC0-0xDF)

Instructions specific to the x86 architecture:

```
CPUID = 0xC0    // CPU identification
IN = 0xC1       // Input from port
OUT = 0xC2      // Output to port
LIDT = 0xC3     // Load interrupt descriptor table
```

### ARM-Specific CPU Operations (0xC0-0xDF)

Instructions specific to the ARM architecture:

```
SVC = 0xC0      // Supervisor call
DMB = 0xC1      // Data memory barrier
DSB = 0xC2      // Data synchronization barrier
ISB = 0xC3      // Instruction synchronization barrier
```

## Mode-Specific Operations

Some operations are only available in specific architecture modes:

### x86-64 Mode Operations (0xE0-0xEF)

```
SWAPGS = 0xE0   // Swap GS base register
RDFSBASE = 0xE1 // Read FS base
RDGSBASE = 0xE2 // Read GS base
```

## Usage Recommendations

1. **Portability**: Prefer Universal ISA instructions for maximum portability
2. **Performance**: Use Extended ISA instructions when targeting specific platforms
3. **Conditional Code**: Always wrap extended instructions in appropriate conditional blocks
4. **Fallbacks**: Provide alternative implementation using Universal ISA when possible

## Related Components

- [CPU Operations](./cpu-operations.md) - CPU-specific instructions
- [x86 Operations](./x86-operations.md) - x86-specific instructions
- [ARM Operations](./arm-operations.md) - ARM-specific instructions
- [Universal ISA](../isa-u/overview.md) - Platform-independent instructions