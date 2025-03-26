# Processor Types

## Purpose

This document defines the processor types supported by COIL, their characteristics, and how COIL's design enables cross-processor compatibility. While COIL v1 focuses primarily on CPUs, the specification establishes a foundation for the multi-processor support that will be expanded in future versions.

## Processor Type System

COIL uses an 8-bit value to identify processor types in the `PROC` directive:

```
PROC processor_type
```

Where `processor_type` is one of the following values:

```
0x01 - CPU (Central Processing Unit)
0x02 - GPU (Graphics Processing Unit) - Reserved for v3
0x03 - TPU (Tensor Processing Unit) - Reserved for v3
0x04 - DSP (Digital Signal Processor) - Reserved for v3
0x05 - FPGA (Field Programmable Gate Array) - Reserved for v3
0x06 - QPU (Quantum Processing Unit) - Reserved for v3
0x07 - NPU (Neural Processing Unit) - Reserved for v3
0x08-0xFE - Reserved for future processor types
0xFF - Custom processor
```

In COIL v1, only CPU (0x01) is fully supported. Other processor types are reserved for COIL v3.

## CPU Processor Type (0x01)

The CPU processor type represents traditional Central Processing Units with general-purpose computation capabilities.

### CPU Architectures

CPUs are further divided into architectures using the `ARCH` directive:

```
ARCH architecture, mode
```

Where `architecture` is one of:

```
0x01 - x86
0x02 - ARM
0x03 - RISC-V
0x04 - MIPS
0x05 - PowerPC
0x06-0xFE - Reserved for future architectures
0xFF - Custom architecture
```

### Architecture Modes

Each architecture supports various modes:

#### x86 (0x01)
```
0x01 - 16-bit mode (Real mode)
0x02 - 32-bit mode (Protected mode)
0x03 - 64-bit mode (Long mode)
```

#### ARM (0x02)
```
0x01 - 32-bit mode (AArch32)
0x02 - 64-bit mode (AArch64)
```

#### RISC-V (0x03)
```
0x01 - 32-bit mode (RV32)
0x02 - 64-bit mode (RV64)
0x03 - 128-bit mode (RV128)
```

### CPU Register Sets

Each CPU architecture has its own register set, which COIL abstracts through the variable system and register types. When direct register access is needed, the register is specified using the appropriate identifier:

```
MOV TYPE_RGP=RAX, 42  ; x86-64 register
MOV TYPE_RGP=X0, 42    ; ARM64 register
```

The COIL processor automatically handles the mapping to the appropriate register set based on the selected architecture.

## Future Processor Types (v3)

While not fully implemented in v1, COIL's design establishes the framework for supporting multiple processor types in v3:

### GPU (0x02)

Graphics Processing Units are specialized for highly parallel operations, particularly in graphics and certain types of computation.

Characteristics:
- Massive parallelism
- SIMD/SIMT execution models
- Memory hierarchy with specialized caches
- Vector processing capabilities

### TPU (0x03)

Tensor Processing Units are specialized for machine learning operations, particularly matrix multiplications and convolutions.

Characteristics:
- Optimized for tensor operations
- Systolic array architecture
- High throughput for specific operations
- Specialized memory layout

### DSP (0x04)

Digital Signal Processors are specialized for processing digital signals in real-time applications.

Characteristics:
- Optimized for signal processing algorithms
- Fixed-point arithmetic
- Specialized instruction set
- Low power consumption

### FPGA (0x05)

Field Programmable Gate Arrays provide reconfigurable hardware acceleration.

Characteristics:
- Reprogrammable logic
- Custom data paths
- Specialized acceleration for specific algorithms
- Flexible I/O capabilities

## Cross-Processor Programming Model

COIL is designed to enable code that works across different processor types:

1. **Universal Instructions**: Core instructions (0x00-0x2F, 0x50-0xBF) work consistently across all processor types
2. **Variable System**: Abstraction over processor-specific register sets
3. **ABI System**: Abstraction over calling conventions
4. **Platform-Dependent Types**: Adaptation to word size and capabilities of each processor

### Writing Processor-Independent Code

To write code that works across different processor types:

1. Use universal instructions
2. Use the variable system rather than direct register access
3. Use the ABI system for function calls
4. Use platform-dependent types (TYPE_INT, TYPE_UNT, TYPE_PTR)
5. Use conditional compilation for processor-specific optimizations

Example:
```
; Processor-independent function
PROC 0x01  ; CPU in v1 (will support any processor in v3)

SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Platform-independent addition
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return via platform-independent ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

### Processor-Specific Optimizations

For performance-critical code, processor-specific optimizations can be applied using conditional compilation:

```
PROC 0x01  ; CPU

; Function with processor-specific optimizations
SYM optimized_function, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Common setup code
    
    ; Processor-specific optimizations
    IF PROC == 0x01  ; CPU
        ; CPU-specific code
        IF ARCH == 0x01  ; x86
            ; x86-specific optimizations
        ELIF ARCH == 0x02  ; ARM
            ; ARM-specific optimizations
        ELSE
            ; Generic CPU implementation
        ENDIF
    ELIF PROC == 0x02  ; GPU (v3)
        ; GPU-specific implementation
    ELSE
        ; Generic implementation
    ENDIF
    
    SCOPEL
```

## Processor Selection

The processor type is specified at the beginning of a COIL program using the `PROC` directive:

```
PROC 0x01  ; Select CPU processor type
```

This directive affects:
1. Which instructions are available
2. How platform-dependent types are resolved
3. Which register sets are accessible
4. Default ABIs and calling conventions

## Multi-Processor Programming (v3)

In COIL v3, programs will be able to utilize multiple processor types within a single program:

```
; CPU section
PROC 0x01
; CPU code...

; GPU section (v3)
PROC 0x02
; GPU code...

; Cross-device operations (v3)
DEVSEL 0x01  ; Select CPU
; Load data...
DEVSEL 0x02  ; Select GPU
; Process data...
```

The opcode range 0x30-0x4F is reserved in v1 for these multi-device operations in v3.

## Default Processor

If no `PROC` directive is specified, COIL defaults to CPU (0x01) processor type.

These parameters are processor-specific and documented in separate processor-specific documentation.

# Processor Abstraction Mechanism

## Purpose

This document explains how COIL abstracts over different processor types, enabling portable code while maintaining access to processor-specific features when needed. The processor abstraction mechanism is a foundational element of COIL's multi-processor design.

## Core Abstraction Principles

COIL provides several levels of abstraction:

1. **Universal Instruction Set**: Core operations that work identically across all processor types
2. **Variable System**: Abstraction over processor-specific register sets
3. **ABI System**: Abstraction over calling conventions
4. **Platform-Dependent Types**: Types that adjust to the target platform
5. **Processor Hierarchy**: Classification of processors for feature targeting

## Processor Selection

The processor type is specified using the `PROC` directive:

```
PROC processor_type  ; e.g., PROC 0x01 for CPU
```

This directive establishes the processor context for subsequent code, affecting:
- Available instructions
- Register set interpretation
- Default types for platform-dependent types
- Memory model assumptions

## Processor Hierarchy

COIL organizes processors in a hierarchy:

1. **Processor Type**: The broad category (CPU, GPU, etc.)
2. **Architecture**: The instruction set architecture within a processor type
3. **Mode**: Specific operating mode of an architecture

This hierarchy allows code to be written at different levels of specificity:
- For maximum portability: Universal operations only
- For processor-type portability: Processor-specific but architecture-independent
- For architecture portability: Architecture-specific but mode-independent
- For maximum performance: Architecture and mode specific

## Abstraction Mechanisms

### Universal Instruction Set (0x00-0x2F, 0x50-0xBF)

These opcodes represent operations available on all processor types:

```
ADD a, b, c         ; Available on all processors
SUB a, b, c         ; Available on all processors
MOV a, b            ; Available on all processors
```

The COIL processor automatically implements these operations efficiently for each target.

### Variable System

Variables abstract over processor-specific register sets:

```
; Variable declarations (processor-independent)
VAR TYPE_INT, counter, 0
VAR TYPE_INT, limit, 100

; Variable usage (processor-independent)
ADD counter, counter, 1
CMP counter, limit
```

The COIL processor decides where to store each variable (register or memory) based on the target processor's capabilities and the variable usage patterns.

### ABI System

The ABI system abstracts over processor-specific calling conventions:

```
; Function declaration with processor-independent ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters (processor-independent)
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Add numbers (processor-independent)
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return result (processor-independent)
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Function call (processor-independent)
CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
```

The `platform_default` ABI automatically maps to the appropriate ABI for the target processor. Specific ABIs can also be selected when needed:

```
; CPU-specific ABIs
CALL function, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, x, y
CALL function, TYPE_ABICTL=ABICTL_PARAM=windows_x64, x, y
```

### Platform-Dependent Types

Platform-dependent types adjust their size based on the target processor:

```
; Types that adapt to the platform
VAR TYPE_INT, a     ; 32-bit on 32-bit platforms, 64-bit on 64-bit platforms
VAR TYPE_PTR, p     ; Pointer sized appropriately for the platform
```

These types provide a balance between portability and efficiency, automatically using the natural word size for the target processor.

### Direct Register Access

When needed, direct register access is still available:

```
; Direct register access (processor-specific)
MOV TYPE_RGP=RAX, 42    ; x86-64 specific
```

This allows maximum performance but sacrifices portability. The register system uses processor-specific register identifiers based on the current processor context.

## Conditional Compilation

For processor-specific optimizations, conditional compilation can be used:

```
; Processor-specific optimizations
IF PROC == 0x01  ; CPU
    ; CPU-specific code
    IF ARCH == 0x01  ; x86
        ; x86-specific optimizations
        IF MODE == 0x03  ; 64-bit mode
            ; 64-bit mode specific optimizations
        ELSE
            ; 32-bit mode specific optimizations
        ENDIF
    ELIF ARCH == 0x02  ; ARM
        ; ARM-specific optimizations
    ELSE
        ; Generic CPU implementation
    ENDIF
ELIF PROC == 0x02  ; GPU (v3)
    ; GPU-specific implementation
ELSE
    ; Generic implementation
ENDIF
```

This allows different code paths for different processors while maintaining a single source file.

## Processor-Specific Instructions (0xC0-0xFE)

For processor-specific features, dedicated opcodes are available:

```
; Processor-specific instructions (only valid in appropriate processor context)
CPUID              ; x86 specific
RDRND              ; x86 specific
```

These instructions are only available when the appropriate processor type is selected.

## Example: Portable Function

A fully portable function using all abstraction mechanisms:

```
; Processor-independent function
PROC 0x01  ; CPU in v1, would be processor-agnostic in v3

SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters via platform-appropriate ABI
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Add values using universal instruction
    VAR TYPE_INT, sum
    ADD sum, a, b
    
    ; Return via platform-appropriate ABI
    RET TYPE_ABICTL=ABICTL_RET=platform_default, sum
    SCOPEL
```

This function will work on any processor type supported by COIL.

## Example: Optimized Function

A function with processor-specific optimizations:

```
; Function with processor-specific optimizations
PROC 0x01  ; CPU

SYM optimized_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Common parameter retrieval
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Processor-specific optimizations
    VAR TYPE_INT, sum
    
    IF ARCH == 0x01  ; x86
        IF MODE == 0x03  ; 64-bit
            ; Use x86-64 specific optimization
            PUSH TYPE_RGP=RAX
            PUSH TYPE_RGP=RBX
            MOV TYPE_RGP=RAX, a
            MOV TYPE_RGP=RBX, b
            ADD TYPE_RGP=RAX, TYPE_RGP=RAX, TYPE_RGP=RBX
            MOV sum, TYPE_RGP=RAX
            POP TYPE_RGP=RBX
            POP TYPE_RGP=RAX
        ELSE
            ; 32-bit x86 path
            ADD sum, a, b
        ENDIF
    ELIF ARCH == 0x02  ; ARM
        ; ARM-specific optimization
        ADD sum, a, b
    ELSE
        ; Generic implementation
        ADD sum, a, b
    ENDIF
    
    RET TYPE_ABICTL=ABICTL_RET=platform_default, sum
    SCOPEL
```

This function provides different optimized implementations for different processors while still working on any supported processor.

## Multi-Processor Programming in v3

In COIL v3, the processor abstraction mechanism will be extended to support multiple processors in a single program:

```
; CPU section (v3)
PROC 0x01
; CPU code...

; GPU section (v3)
PROC 0x02
; GPU code...

; Cross-device operations (v3)
DEVSEL 0x01  ; Select CPU for execution
; CPU operations...
DEVSEL 0x02  ; Select GPU for execution
; GPU operations...
```

The reserved opcode range 0x30-0x4F is designated for these multi-device operations in v3.

## Best Practices

1. **Use universal instructions when possible**: Maximizes portability and future-proofing.

2. **Use the variable system instead of direct register access**: Let the COIL processor handle register allocation for better portability.

3. **Use platform-dependent types for natural word sizes**: Balances portability and performance.

4. **Use the ABI system for function calls**: Abstracts away calling convention differences.

5. **Use conditional compilation for processor-specific optimizations**: Allows targeted optimizations while maintaining portability.

6. **Use processor-specific instructions only when necessary**: Performance-critical sections may need direct access to specialized features.

7. **Test on multiple target processors**: Ensures portability of code that claims to be processor-independent.

## Implementation Notes

COIL processors are responsible for:

1. Translating universal instructions to processor-specific operations
2. Allocating variables to registers or memory based on the target processor
3. Implementing the ABI system for the target processor
4. Mapping platform-dependent types to the appropriate sizes
5. Handling processor-specific instructions when the appropriate processor is selected

This abstraction layer enables COIL's write-once-run-anywhere capability without sacrificing performance.