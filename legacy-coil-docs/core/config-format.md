# COIL Configuration Format

## Overview

The COIL Configuration Format defines a standardized way to specify compilation targets, optimization settings, and platform-specific features for COIL programs. This allows COIL code to be efficiently targeted and optimized for specific environments.

## File Format

COIL configuration files use a structured text format with the `.coilcfg` extension. The format is similar to INI-style configuration:

```
[Section]
Key1 = Value1
Key2 = Value2

[Another Section]
Key3 = Value3
```

### Formal Schema

The configuration format follows this formal schema:

1. **Comments**: Lines beginning with ';' or '#' are comments
2. **Sections**: Enclosed in square brackets `[SectionName]`
3. **Key-Value Pairs**: `Key = Value` format
4. **Data Types**:
   - **String**: Unquoted or quoted text
   - **Integer**: Base-10 by default, prefixed with 0x for hexadecimal
   - **Boolean**: true/false or yes/no or 1/0
   - **List**: Comma-separated values
5. **Whitespace**: Ignored except within quoted strings
6. **Includes**: Special directive `@include "file.coilcfg"` to include other config files

### Example Schema Definition

```
file = *line
line = comment / section / key-value / include / empty
comment = ["#" / ";"] *any-char
section = "[" section-name "]"
key-value = key "=" value
include = "@include" string
empty = *whitespace
section-name = 1*valid-name-char
key = 1*valid-name-char
value = string / integer / boolean / list
string = quoted-string / unquoted-string
quoted-string = DQUOTE *string-char DQUOTE
unquoted-string = 1*unquoted-char
integer = ["0x"] 1*DIGIT
boolean = "true" / "false" / "yes" / "no" / "1" / "0"
list = value *("," value)
```

## Core Sections

A complete COIL configuration must define these core sections:

### [Target]

Defines the processing unit, architecture, and mode:

```
[Target]
PU = CPU           # Processing Unit type (required)
Architecture = x86  # Target architecture (required)
Mode = 64           # Bit mode (required)
Features = SSE4.2,AVX2,FMA  # Optional feature flags
```

Valid Processing Units:
- `CPU` - Central Processing Unit
- `GPU` - Graphics Processing Unit
- `NPU` - Neural Processing Unit
- `DSP` - Digital Signal Processor

Valid Requirements:
- All fields in [Target] section are required
- PU must be one of the defined processing units
- Architecture must be valid for the selected PU
- Mode must be valid for the selected Architecture
- Features must be valid for the selected Architecture

### [Optimization]

Controls optimization settings:

```
[Optimization]
Level = 2              # 0-3, where 3 is maximum optimization (required)
SizeOptimization = false  # Optimize for size over speed (optional)
SpeedOptimization = true  # Optimize for speed over size (optional)
VectorizationLevel = 1  # 0-2, where 2 is aggressive vectorization (optional)
InliningLevel = 1       # 0-2, where 2 is aggressive inlining (optional)
```

Valid Requirements:
- Level must be between 0 and 3
- If both SizeOptimization and SpeedOptimization are true, the implementation decides the trade-off
- VectorizationLevel must be between 0 and 2
- InliningLevel must be between 0 and 2

### [Memory]

Defines memory model and constraints:

```
[Memory]
Model = Protected      # Memory protection model (required)
Alignment = 16         # Default memory alignment in bytes (required)
StackGrowth = Down     # Stack growth direction (required)
Endianness = Little    # Byte order (required)
```

Valid Memory Models:
- `Protected` - Memory access validation
- `Flat` - No protection mechanisms
- `Segmented` - Segment-based memory

Valid Requirements:
- Model must be one of the defined memory models
- Alignment must be a power of 2 (1, 2, 4, 8, 16, etc.)
- StackGrowth must be "Up" or "Down"
- Endianness must be "Little" or "Big"

## Optional Sections

These sections are optional but provide important additional configuration:

### [ABI]

Defines the Application Binary Interface to use:

```
[ABI]
Name = SystemV         # ABI name or identifier (required if section present)
ParameterRegisters = rdi,rsi,rdx,rcx,r8,r9  # Registers for parameters (optional)
ReturnRegisters = rax,rdx  # Registers for return values (optional)
StackAlignment = 16    # Stack alignment requirements (optional)
RedZoneSize = 128      # Red zone size in bytes (optional)
```

Valid Requirements:
- Name must be a defined ABI name or a custom ABI identifier
- Register names must be valid for the target architecture
- StackAlignment must be a power of 2
- RedZoneSize must be a non-negative integer

### [Extensions]

Enables specialized instruction set extensions:

```
[Extensions]
SIMD = AVX512         # SIMD extension to use (optional)
Crypto = AES,SHA      # Cryptographic extensions (optional)
AtomicOperations = true  # Enable atomic operations (optional)
```

Valid Requirements:
- All extensions must be valid for the target architecture
- Extensions may be validated at compile time for compatibility

### [Preprocessor]

Defines preprocessor constants and settings:

```
[Preprocessor]
Define = DEBUG=1,PLATFORM="x64"  # Define preprocessor variables (optional)
Include = /usr/include,/opt/coil/include  # Include paths (optional)
```

Valid Requirements:
- Define format must be NAME or NAME=VALUE
- Include paths must be valid file system paths

### [Linker]

Controls linker behavior:

```
[Linker]
DefaultLibraryPath = /usr/lib/coil  # Default library search path (optional)
Libraries = libc,libm  # Libraries to link (optional)
EntryPoint = main  # Entry point symbol (optional)
OutputFormat = ELF  # Output file format (optional)
```

Valid Requirements:
- DefaultLibraryPath must be a valid directory path
- Libraries must be valid library names
- EntryPoint must be a valid symbol name
- OutputFormat must be a supported format (ELF, PE, Mach-O, Raw)

## Schema Validation

COIL implementations must validate configuration files against this schema:

### Required Validation Rules

1. **Section Validation**:
   - Required sections must be present
   - Section names must be valid
   - Unknown sections should be prefixed with vendor identifier

2. **Key Validation**:
   - Required keys must be present in their respective sections
   - Keys must be valid identifiers
   - Unknown keys should trigger warnings

3. **Value Validation**:
   - Values must be of the correct type
   - Numeric values must be within valid ranges
   - Enumerations must be from the allowed set
   - Lists must contain valid elements

4. **Cross-Field Validation**:
   - Architecture must be valid for the selected PU
   - Features must be valid for the selected Architecture
   - Extensions must be valid for the selected Architecture

### Validation Tool

Configuration files can be validated using the COIL configuration validator:

```bash
coil-config-validate config.coilcfg
```

The validator returns:
- Exit code 0 if validation succeeds
- Exit code 1 if validation fails
- Detailed error messages for validation failures
- Warnings for potential issues

### Validation Schema Format

The formal validation schema is available in JSON Schema format:

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "required": ["Target", "Optimization", "Memory"],
  "properties": {
    "Target": {
      "type": "object",
      "required": ["PU", "Architecture", "Mode"],
      "properties": {
        "PU": {"enum": ["CPU", "GPU", "NPU", "DSP"]},
        "Architecture": {"type": "string"},
        "Mode": {"type": "integer", "minimum": 8, "maximum": 128},
        "Features": {
          "type": "array",
          "items": {"type": "string"}
        }
      }
    },
    "Optimization": {
      "type": "object",
      "required": ["Level"],
      "properties": {
        "Level": {"type": "integer", "minimum": 0, "maximum": 3},
        "SizeOptimization": {"type": "boolean"},
        "SpeedOptimization": {"type": "boolean"},
        "VectorizationLevel": {"type": "integer", "minimum": 0, "maximum": 2},
        "InliningLevel": {"type": "integer", "minimum": 0, "maximum": 2}
      }
    },
    "Memory": {
      "type": "object",
      "required": ["Model", "Alignment", "StackGrowth", "Endianness"],
      "properties": {
        "Model": {"enum": ["Protected", "Flat", "Segmented"]},
        "Alignment": {"type": "integer", "minimum": 1},
        "StackGrowth": {"enum": ["Up", "Down"]},
        "Endianness": {"enum": ["Little", "Big"]}
      }
    },
    // Additional section schemas...
  }
}
```

## Complete Examples

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

[Extensions]
SIMD = AVX2
AtomicOperations = true

[Preprocessor]
Define = LINUX=1,X86_64=1
Include = /usr/include,/usr/local/include

[Linker]
DefaultLibraryPath = /usr/lib
Libraries = libc,libm
EntryPoint = main
OutputFormat = ELF
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

[Extensions]
SIMD = NEON
AtomicOperations = true

[Preprocessor]
Define = EMBEDDED=1,ARM32=1
Include = /arm-none-eabi/include

[Linker]
Libraries = libc_s
EntryPoint = Reset_Handler
OutputFormat = Raw
```

### CUDA-Like GPU Configuration

```
[Target]
PU = GPU
Architecture = NVIDIA
Mode = 64
Features = SM_70

[Memory]
Model = Protected
Alignment = 16
StackGrowth = Down
Endianness = Little

[Optimization]
Level = 3
SizeOptimization = false
SpeedOptimization = true
VectorizationLevel = 2

[Extensions]
AtomicOperations = true
SharedMemory = true

[NVIDIA_SpecialFeatures]
SharedMemorySize = 49152
MaxRegistersPerThread = 255
WarpSize = 32
```

## Platform-Specific Extensions

Vendors can add custom sections with a vendor prefix:

```
[VENDOR_CustomSection]
CustomOption1 = Value1
CustomOption2 = Value2
```

### Vendor Extension Rules

1. **Naming Convention**:
   - Vendor section names must be prefixed with vendor name and underscore
   - Vendor should be registered or use a unique identifier
   - Example: `[NVIDIA_CUDA]`, `[ARM_VFP]`

2. **Value Handling**:
   - Vendor-specific values can use the same data types as core configuration
   - Implementations should preserve unknown vendor sections
   - Implementations may warn about unknown vendor sections

3. **Validation**:
   - Vendor-specific validation schema can be provided
   - Core validator should skip validation of vendor sections
   - Vendor-provided validator can handle vendor-specific sections

### Vendor Extension Example: NVIDIA GPU

```
[NVIDIA_CUDA]
ComputeCapability = 7.0
SharedMemoryBankSize = 4
WarpScheduling = greedy
```

### Vendor Extension Example: ARM NEON

```
[ARM_NEON]
FP16 = true
Dot = true
FMA = true
```

## Programmatic Use

COIL Processors and tools use configuration files as follows:

1. **Compile-Time**: Passed as an option to the COIL processor
   ```
   coil-compile -c config.coilcfg source.coil
   ```

2. **Link-Time**: Used to validate compatibility between modules
   ```
   coil-link -c config.coilcfg main.o utils.o -o program
   ```

3. **Runtime**: Used for JIT and interpretation settings
   ```
   coil-run -c config.coilcfg program
   ```

## Configuration Inheritance

Configurations can inherit from base configurations:

```
@include "base.coilcfg"

[Target]
Features = SSE4.2,AVX2  # Overrides Features from base
```

Inheritance rules:
1. Included file is processed first
2. Local settings override included settings
3. Multiple includes are processed in order
4. Circular includes are detected and prevented

## Default Configuration

If no configuration is specified, the COIL processor uses a default configuration based on the host platform:

```
[Target]
PU = CPU
Architecture = (auto-detected)
Mode = (auto-detected)
Features = (auto-detected)

[Memory]
Model = Protected
Alignment = (architecture-specific)
StackGrowth = Down
Endianness = (auto-detected)

[Optimization]
Level = 1
SizeOptimization = false
SpeedOptimization = true
```

## Related Components

- [Binary Format](../format/binary-format.md) - COIL binary encoding specification
- [Extended ISA](../isa-e/index.md) - Architecture-specific instructions
- [Memory Model](./memory-model.md) - Memory model specification
- [ABI Definition](../isa-c/abi-definition.md) - ABI specification