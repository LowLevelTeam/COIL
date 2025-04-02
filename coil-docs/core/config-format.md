# COIL Configuration Format

## Overview

The COIL Configuration Format defines a standardized way to specify compilation targets, optimization settings, and platform-specific features for COIL programs. This allows COIL code to be efficiently targeted and optimized for specific environments.

## File Format

COIL configuration files use a structured text format with the `.coilcfg` extension:

```
[Section]
Key1 = Value1
Key2 = Value2

[Another Section]
Key3 = Value3
```

## Core Sections

A complete COIL configuration must define the following core sections:

### [Target]

Defines the processing unit, architecture, and mode:

```
[Target]
PU = CPU           # Processing Unit type
Architecture = x86  # Target architecture
Mode = 64           # Bit mode
Features = SSE4.2,AVX2,FMA  # Optional feature flags
```

Valid Processing Units:
- `CPU` - Central Processing Unit
- `GPU` - Graphics Processing Unit
- `NPU` - Neural Processing Unit
- `DSP` - Digital Signal Processor

### [Optimization]

Controls optimization settings:

```
[Optimization]
Level = 2              # 0-3, where 3 is maximum optimization
SizeOptimization = false
SpeedOptimization = true
VectorizationLevel = 1  # 0-2, where 2 is aggressive vectorization
InliningLevel = 1       # 0-2, where 2 is aggressive inlining
```

### [Memory]

Defines memory model and constraints:

```
[Memory]
Model = Protected      # Memory protection model
Alignment = 16         # Default memory alignment in bytes
StackGrowth = Down     # Stack growth direction
Endianness = Little    # Byte order
```

Valid Memory Models:
- `Protected` - Memory access validation
- `Flat` - No protection mechanisms
- `Segmented` - Segment-based memory

## Optional Sections

### [ABI]

Defines the Application Binary Interface to use:

```
[ABI]
Name = SystemV         # ABI name or identifier
ParameterRegisters = rdi,rsi,rdx,rcx,r8,r9
ReturnRegisters = rax,rdx
StackAlignment = 16
RedZoneSize = 128
```

### [Extensions]

Enables specialized instruction set extensions:

```
[Extensions]
SIMD = AVX512         # SIMD extension to use
Crypto = AES,SHA      # Cryptographic extensions
AtomicOperations = true
```

### [Preprocessor]

Defines preprocessor constants and settings:

```
[Preprocessor]
Define = DEBUG=1,PLATFORM="x64"
Include = /usr/include,/opt/coil/include
```

### [Linker]

Controls linker behavior:

```
[Linker]
DefaultLibraryPath = /usr/lib/coil
Libraries = libc,libm
EntryPoint = main
OutputFormat = ELF
```

## Examples

### x86-64 Linux Configuration

```
[Target]
PU = CPU
Architecture = x86
Mode = 64
Features = SSE4.2,AVX2,FMA

[Memory]
Model = Protected
Alignment = 16
StackGrowth = Down
Endianness = Little

[ABI]
Name = SystemV
ParameterRegisters = rdi,rsi,rdx,rcx,r8,r9
ReturnRegisters = rax,rdx
StackAlignment = 16
RedZoneSize = 128

[Optimization]
Level = 2
SizeOptimization = false
SpeedOptimization = true
```

### Embedded ARM Configuration

```
[Target]
PU = CPU
Architecture = ARM
Mode = 32
Features = NEON,VFPv4

[Memory]
Model = Protected
Alignment = 8
StackGrowth = Down
Endianness = Little

[ABI]
Name = AAPCS
ParameterRegisters = r0,r1,r2,r3
ReturnRegisters = r0,r1
StackAlignment = 8
RedZoneSize = 0

[Optimization]
Level = 2
SizeOptimization = true
SpeedOptimization = false
```

## Programmatic Use

COIL Processors and tools use configuration files as follows:

1. **Compile-Time**: Passed as an option to the COIL processor
2. **Link-Time**: Used to validate compatibility between modules
3. **Runtime**: Used for JIT and interpretation settings

## Extensibility

Vendors can add custom sections with a vendor prefix:

```
[VENDOR_CustomSection]
CustomOption1 = Value1
CustomOption2 = Value2
```

## Schema Validation

Configuration files can be validated against the official schema:

```bash
coil-config-validate config.coilcfg
```

## Related Components

- [Binary Format](./binary-format.md) - COIL binary encoding specification
- [Extended ISA](../isa-e/index.md) - Architecture-specific instructions