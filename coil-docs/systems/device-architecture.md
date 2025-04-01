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
| 0x06 | NPU | Neural Processing Unit |
| 0x07 | VPU | Vision Processing Unit |
| 0x08-0xFE | Reserved | For future device types |
| 0xFF | Custom | Vendor-specific device |

Device types define the primary processing paradigm and general capabilities.

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
| 0x0106 | SPARC | SPARC architecture |
| 0x0107-0x01FE | Reserved | For future CPU architectures |
| 0x01FF | Custom | Vendor-specific CPU architecture |

### GPU Architectures (0x02xx)

| Architecture Code | Name | Description |
|-------------------|------|-------------|
| 0x0201 | NVIDIA CUDA | NVIDIA CUDA architecture |
| 0x0202 | AMD GCN | AMD GCN architecture |
| 0x0203 | Intel Xe | Intel Xe architecture |
| 0x0204 | ARM Mali | ARM Mali architecture |
| 0x0205 | Imagination PowerVR | PowerVR architecture |
| 0x0206-0x02FE | Reserved | For future GPU architectures |
| 0x02FF | Custom | Vendor-specific GPU architecture |

### TPU Architectures (0x03xx)

| Architecture Code | Name | Description |
|-------------------|------|-------------|
| 0x0301 | Google TPU | Google Tensor Processing Unit |
| 0x0302 | NVIDIA Tensor Cores | NVIDIA Tensor Core architecture |
| 0x0303-0x03FE | Reserved | For future TPU architectures |
| 0x03FF | Custom | Vendor-specific TPU architecture |

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
| 0x010204 | ThumbEE | Thumb EE instruction set |

### NVIDIA CUDA Modes (0x0201xx)

| Mode Code | Name | Description |
|-----------|------|-------------|
| 0x020101 | SM 5.0 | Compute capability 5.0 |
| 0x020102 | SM 6.0 | Compute capability 6.0 |
| 0x020103 | SM 7.0 | Compute capability 7.0 |
| 0x020104 | SM 7.5 | Compute capability 7.5 |
| 0x020105 | SM 8.0 | Compute capability 8.0 |

## Target Specification

A complete target specification consists of three components:

1. **Device Type**: Primary processing unit category
2. **Architecture**: Specific hardware architecture
3. **Mode**: Execution mode or capability level

For example, a complete target specification might be:
- `0x010103`: CPU (0x01), x86 architecture (0x01), 64-bit mode (0x03)
- `0x020104`: GPU (0x02), NVIDIA CUDA (0x01), SM 7.5 (0x04)

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

These capabilities are defined in device configuration files and can be queried at runtime.

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

## Target Inheritance

The device architecture system uses inheritance to share common capabilities:

```
Base Features
├── CPU Features
│   ├── x86 Features
│   │   ├── x86-32 Features
│   │   └── x86-64 Features
│   └── ARM Features
│       ├── ARM32 Features
│       └── ARM64 Features
├── GPU Features
│   ├── NVIDIA Features
│   └── AMD Features
└── ...
```

This hierarchy allows code to target specific capabilities while maintaining compatibility across related devices.