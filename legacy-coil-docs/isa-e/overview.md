# Extended Instruction Set Architecture

## Overview

The Extended Instruction Set Architecture (ISA-E) provides platform-specific instructions that are optimized for particular processing units, architectures, and modes. While not universally portable, these instructions enable high-performance code when targeting specific hardware platforms.

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