# Device Architecture Specification

## Overview

The COIL Device Architecture Specification defines the standardized categorization of processing units, architectures, and execution modes. This hierarchical classification system enables COIL to target diverse hardware while maintaining consistent abstractions.

## Device Types

COIL supports the following primary device types:

| Device Code | Name | Description |
|-------------|------|-------------|
| 0x01 | CPU | Central Processing Unit |
| 0x02 | GPU | Graphics Processing Unit |
| 0x03 | TPU | Tensor Processing Unit |
| 0x04 | DSP | Digital Signal Processor |
| 0x05 | FPGA | Field-Programmable Gate Array |
| 0x06-0xFE | Reserved | For future device types |
| 0xFF | Custom | Vendor-specific device |

## Architecture Types

Each device type supports multiple architectures. Architecture codes use the device code as a prefix:

### CPU Architectures (0x01xx)

| Architecture Code | Name | Description |
|-------------------|------|-------------|
| 0x0101 | x86 | x86 architecture |
| 0x0102 | ARM | ARM architecture |
| 0x0103 | RISC-V | RISC-V architecture |
| 0x0104 | MIPS | MIPS architecture |
| 0x0105 | PowerPC | PowerPC architecture |
| 0x0106-0x01FE | Reserved | For future CPU architectures |
| 0x01FF | Custom | Vendor-specific CPU architecture |

### GPU Architectures (0x02xx)

| Architecture Code | Name | Description |
|-------------------|------|-------------|
| 0x0201 | NVIDIA CUDA | NVIDIA CUDA architecture |
| 0x0202 | AMD GCN | AMD GCN architecture |
| 0x0203 | Intel Xe | Intel Xe architecture |
| 0x0204-0x02FE | Reserved | For future GPU architectures |
| 0x02FF | Custom | Vendor-specific GPU architecture |

## Mode Types

Each architecture may support multiple execution modes. Mode codes use the architecture code as a prefix:

### x86 Modes (0x0101xx)

| Mode Code | Name | Description |
|-----------|------|-------------|
| 0x010101 | x86-16 | 16-bit mode |
| 0x010102 | x86-32 | 32-bit mode |
| 0x010103 | x86-64 | 64-bit mode |

### ARM Modes (0x0102xx)

| Mode Code | Name | Description |
|-----------|------|-------------|
| 0x010201 | ARM32 | 32-bit mode |
| 0x010202 | ARM64 | 64-bit mode |
| 0x010203 | Thumb | Thumb instruction set |

## Target Specification

A complete target specification consists of three components:

1. **Device Type**: Primary processing unit category
2. **Architecture**: Specific hardware architecture
3. **Mode**: Execution mode or capability level

For example, a complete target specification might be:
- `0x010103`: CPU (0x01), x86 architecture (0x01), 64-bit mode (0x03)
- `0x020104`: GPU (0x02), NVIDIA CUDA (0x01), SM 7.5 (0x04)

## Device Selection

In CASM, devices are targeted using the `PROC`, `ARCH`, and `MODE` directives:

```
PROC 0x01               ; CPU
ARCH 0x01               ; x86
MODE 0x03               ; 64-bit

; CPU-specific code
...

PROC 0x02               ; GPU
ARCH 0x01               ; NVIDIA CUDA
MODE 0x04               ; SM 7.5

; GPU-specific code
...
```

Alternative compact format:

```
TARGET 0x010103         ; CPU (0x01), x86 (0x01), 64-bit (0x03)
```

## Device Capabilities

Each device has core capabilities that determine supported operations:

| Capability Flag | Description |
|-----------------|-------------|
| 0x01 | Integer arithmetic |
| 0x02 | Floating-point arithmetic |
| 0x04 | Vector operations |
| 0x08 | Tensor operations |
| 0x10 | Atomic operations |
| 0x20 | Threading support |
| 0x40 | Memory hierarchy control |
| 0x80 | Specialized acceleration |

## Multi-Target Sections

COIL supports defining different implementations for different targets:

```
SECTION .text.cpu, 0x01 | 0x04, TARGET=0x010103
  ; CPU-specific implementation
ENDSECTION

SECTION .text.gpu, 0x01 | 0x04, TARGET=0x020104
  ; GPU-specific implementation
ENDSECTION
```

## Runtime Target Detection

Programs can detect available targets at runtime:

```
; Check for device availability
VAR #10, TYPE_INT32            ; Result flag
QUERY_DEVICE 0x02, 0x01, 0x04, #10  ; Is CUDA SM 7.5 available?

; Branch based on available device
CMP #10, 0
BR_EQ use_cpu                  ; If not available, use CPU
```

## Implementation Status

COIL Version 1.0 includes:
- Full CPU targeting (0x01)
- Basic GPU targeting framework (0x02)
- Device detection and query capabilities
- Multi-target section support

Future versions will expand device support with full GPU, TPU, and other device implementations.

## Related Components

- [Device Configuration](/coil-docs/systems/device-configuration.md) - Detailed device capability configuration
- [Device Targeting](/coil-docs/systems/device-targeting.md) - Code targeting for specific devices 
- [Memory Models](/coil-docs/systems/memory-models.md) - Device-specific memory models