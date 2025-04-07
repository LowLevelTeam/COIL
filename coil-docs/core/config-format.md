# COIL Configuration Format

## Overview

The COIL Configuration Format defines how capabilities, features, and constraints are specified for COIL implementations. This format enables precise targeting of hardware capabilities while maintaining the platform independence of COIL code.

## Configuration Purpose

The configuration serves several critical functions:

1. **Capability Declaration**: Defines which COIL capabilities are supported
2. **Feature Detection**: Specifies available hardware features
3. **Constraint Definition**: Establishes limits and requirements
4. **Optimization Guidance**: Provides hints for code generation

## Format Structure

COIL configurations use a hierarchical structure:

```
[Configuration]
  ├── [Processing Unit]
  │     ├── Identifier
  │     ├── Version
  │     └── [Capabilities]
  ├── [Architecture]
  │     ├── Identifier
  │     ├── Mode
  │     └── [Features]
  └── [Constraints]
        ├── Memory
        ├── Execution
        └── Alignment
```

### Binary Format

The binary configuration format has this structure:

```c
struct CoilConfig {
    uint32_t magic;           // "COIL" (0x4C494F43)
    uint32_t version;         // Configuration format version
    uint32_t capability_mask; // COIL capability bitfield
    uint32_t feature_mask;    // Native feature bitfield
    uint32_t pu_type;         // Processing unit type
    uint32_t arch_type;       // Architecture type
    uint32_t mode_type;       // Architecture mode
    uint64_t memory_limit;    // Maximum memory (0 = unlimited)
    uint32_t alignment_req;   // Alignment requirements
    uint32_t extension_count; // Number of extension entries
    Extension extensions[];   // Array of extension entries
};

struct Extension {
    uint32_t extension_id;    // Extension identifier
    uint32_t version;         // Extension version
    uint32_t capability_mask; // Extension-specific capabilities
};
```

## COIL Capabilities

COIL capabilities are abstract, platform-independent features that can be queried at compile time:

| Capability ID | Name | Description |
|---------------|------|-------------|
| 0x00000001 | ATOMIC_OPS | Atomic memory operations |
| 0x00000002 | VECTOR_OPS | Vector operations (independent of width) |
| 0x00000004 | MATRIX_OPS | Matrix operations |
| 0x00000008 | CRYPTO_OPS | Cryptographic operations |
| 0x00000010 | FLOAT_OPS | Floating-point operations |
| 0x00000020 | EXTENDED_INT | Extended-precision integer operations |
| 0x00000040 | EXCEPTION_HANDLING | Exception handling support |
| 0x00000080 | DYNAMIC_LINKING | Dynamic linking support |
| 0x00000100 | MEMORY_PROTECTION | Memory protection features |
| 0x00000200 | THREADING | Multi-threading support |

## Native Features

Native features represent hardware-specific capabilities that may be emulated:

| Feature ID | Name | Description |
|------------|------|-------------|
| 0x00000001 | SIMD_128 | 128-bit SIMD operations |
| 0x00000002 | SIMD_256 | 256-bit SIMD operations |
| 0x00000004 | SIMD_512 | 512-bit SIMD operations |
| 0x00000008 | FP16_SUPPORT | Native half-precision floating point |
| 0x00000010 | FP64_SUPPORT | Native double-precision floating point |
| 0x00000020 | INT64_SUPPORT | Native 64-bit integer operations |
| 0x00000040 | HARDWARE_DIV | Hardware division |
| 0x00000080 | HARDWARE_SQRT | Hardware square root |
| 0x00000100 | HARDWARE_TRIG | Hardware trigonometric functions |
| 0x00000200 | UNALIGNED_ACCESS | Efficient unaligned memory access |

## Configuration Generation

Configurations can be generated in several ways:

1. **Auto-Detection**: Tools scan hardware and generate appropriate configuration
2. **Manual Definition**: Developers specify exact capabilities for a target
3. **Preset Configurations**: Pre-defined configurations for common platforms

## Configuration Usage

### Compile-Time Usage

During compilation, the configuration guides:

1. **Instruction Selection**: Choose appropriate instructions based on capabilities
2. **Code Generation**: Generate optimal code for the target
3. **Error Detection**: Identify when code requires unavailable capabilities

### Example Configuration Query

In CASM, capabilities can be queried with directives:

```
!if capability(ATOMIC_OPS)
  ; Use atomic operations
!else
  ; Use software-based atomics implementation
!endif
```

In binary form, capabilities are checked with the PPCOND instruction:

```
[0xF0][0x02][COND_CAPABILITY][ATOMIC_OPS]
```

## Implementation Requirements

COIL implementations must:

1. **Configuration Validation**: Verify configuration consistency and validity
2. **Capability Checking**: Support compile-time checking of capabilities
3. **Feature Emulation**: Provide fallback implementations for unavailable features
4. **Configuration Generation**: Include tools to generate platform-specific configurations