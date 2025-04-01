# Device Configuration Format

## Overview

The Device Configuration Format (DCF) provides a standardized way to define device capabilities, features, and characteristics. This format is essential for the COIL processor to understand how to generate efficient code for each target device.

## Device Configuration Format (DCF)

Device capabilities are defined using the following syntax:

```
DEVICE device_code, architecture_code, mode_code
  CAPS capability_flags            ; Device capabilities
  MEM memory_model                 ; Memory model
  FEAT feature_list                ; Supported features
  ISA isa_version                  ; Instruction set version
  REG register_definition          ; Register set
  ABI abi_definition               ; ABI support
ENDDEV
```

### Example Configuration: x86-64

```
DEVICE 0x01, 0x01, 0x03            ; CPU, x86, 64-bit mode
  CAPS 0x01 | 0x02 | 0x04          ; Integer, Floating-point, Vector
  MEM 0x01                         ; Flat memory model
  FEAT SSE, SSE2, SSE3, AVX        ; Supported features
  ISA 0x01                         ; ISA version 1
  REG
    GP RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8-R15
    FP XMM0-XMM15, YMM0-YMM15
  ENDREG
  ABI
    PARAMS RDI, RSI, RDX, RCX, R8, R9
    RETS RAX, RDX
    CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
    CALLEE RBX, RSP, RBP, R12, R13, R14, R15
    SALLIGN 16
    RZONE 128
  ENDABI
ENDDEV
```

### Example Configuration: NVIDIA CUDA

```
DEVICE 0x02, 0x01, 0x04            ; GPU, NVIDIA CUDA, SM 7.5
  CAPS 0x01 | 0x02 | 0x04 | 0x08   ; Integer, Floating-point, Vector, Tensor
  MEM 0x03                         ; Hierarchical memory model
  FEAT TENSOR_CORES, FP16, INT8    ; Supported features
  ISA 0x01                         ; ISA version 1
  REG
    GP R0-R255
    PRED P0-P7
    SPEC LANEID, WARPID, SMID, GRIDID
  ENDREG
  ABI
    PARAMS R0-R31
    RETS R0-R7
    CALLER R0-R31
    CALLEE NONE
    SALLIGN 32
    RZONE 0
  ENDABI
ENDDEV
```

## Feature Inheritance System

The Feature Inheritance System allows devices to inherit features from parent categories:

### Feature Sets

Features are grouped into feature sets:

```
FEATSET CPU_BASE
  INTEGER_ARITHMETIC
  FLOATING_POINT_BASIC
  MEMORY_ACCESS
  CONTROL_FLOW
ENDFEATSET

FEATSET X86_64
  INHERIT CPU_BASE
  SSE
  SSE2
  AVX
  AVX2
  BMI1
  BMI2
ENDFEATSET

FEATSET NVIDIA_CUDA
  INHERIT GPU_BASE
  TENSOR_CORES
  SHARED_MEMORY
  WARP_SHUFFLE
  ATOMIC_OPERATIONS
ENDFEATSET
```

Feature sets can inherit from parent sets, creating a rich hierarchy of capabilities.

## Configuration Files

Device configurations can be stored in external files using the DCF format:

```
# x86-64.dcf - Configuration for x86-64
DEVICE 0x01, 0x01, 0x03           # CPU, x86, 64-bit
  CAPS 0x01 | 0x02 | 0x04
  MEM 0x01
  FEAT SSE, SSE2, SSE3, AVX
  ...
ENDDEV
```

Configuration files are loaded by the COIL toolchain to determine available targets.

## Feature Emulation

When a target device lacks a specific feature, the COIL processor can emulate it:

```
EMULATE AVX2, 0x01, 0x01, 0x02     ; Emulate AVX2 on x86-32
  VPSHUFB -> SHUFFLE_SEQUENCE      ; Map to sequence of basic instructions
  ...
ENDEMULATE
```

This allows code written for newer devices to run on older hardware with minimal modifications.

## Configuration Querying

Programs can query device configurations at runtime:

```
; Check for feature availability
VAR #1, TYPE_INT32            ; Result flag
QUERY_FEATURE 0x01, 0x01, 0x03, "AVX2", #1

; Branch based on feature availability
CMP #1, 0
BR_EQ use_alternative
```

This enables adaptive code that can use optimal implementations based on available features.

## Instruction Mapping

Instruction mapping tables define how COIL instructions map to device-specific implementations:

```
INSTR_MAP ADD, 0x01, 0x01, 0x03    ; ADD on CPU, x86, 64-bit
  TYPE_INT8   -> ADD_I8            ; 8-bit integer add
  TYPE_INT16  -> ADD_I16           ; 16-bit integer add
  TYPE_INT32  -> ADD_I32           ; 32-bit integer add
  TYPE_INT64  -> ADD_I64           ; 64-bit integer add
  TYPE_FP32   -> ADDSS             ; Single-precision float add
  TYPE_FP64   -> ADDSD             ; Double-precision float add
  TYPE_V128=TYPE_FP32 -> ADDPS     ; Vector of floats add
ENDINSTR_MAP
```

These maps help the COIL processor generate optimal code for each target.

## Related Components

- [Device Architecture](/coil-docs/systems/device-architecture.md) - Device architecture specifications
- [Device Targeting](/coil-docs/systems/device-targeting.md) - Code targeting for specific devices
- [Memory Models](/coil-docs/systems/memory-models.md) - Memory models for different devices