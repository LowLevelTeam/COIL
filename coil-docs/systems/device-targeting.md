# Device Targeting

## Overview

Device targeting is the process of specifying which processing units, architectures, and modes a COIL program targets. Effective targeting ensures optimal code generation while maintaining portability across compatible devices.

## Device Selection Directives

### Basic Device Selection

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

These directives inform the COIL assembler which device to target for subsequent code.

### Target Triple Format

Targets can also be specified using a compact triple format:

```
TARGET 0x010103         ; CPU (0x01), x86 (0x01), 64-bit (0x03)
```

This format is equivalent to the separate `PROC`, `ARCH`, and `MODE` directives.

## Multi-Target Sections

COIL supports defining different implementations for different targets using conditional sections:

```
SECTION .text.cpu, 0x01 | 0x04, TARGET=0x010103
  ; CPU-specific implementation
ENDSECTION

SECTION .text.gpu, 0x01 | 0x04, TARGET=0x020104
  ; GPU-specific implementation
ENDSECTION
```

When the program runs, the appropriate section is selected based on the execution environment.

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

This allows dynamic selection of optimal implementation paths.

## Device Communication

### Device Selection

```
; Select active device
DEVICE_SELECT 0x02, 0x01, 0x04   ; GPU, NVIDIA CUDA, SM 7.5

; Execute on selected device
...

; Restore default device
DEVICE_SELECT 0x01, 0x01, 0x03   ; CPU, x86, 64-bit
```

### Parallel Execution

```
; Launch kernel on GPU
KERNEL_LAUNCH gpu_function, grid_dims, block_dims, param1, param2

; Continue CPU execution concurrently
...

; Wait for kernel completion
KERNEL_WAIT
```

This model allows concurrent execution across multiple devices.

## Conditional Compilation

COIL supports conditional compilation based on target properties:

```
IF TARGET == 0x010103
  ; x86-64 specific code
ELIF TARGET == 0x010202
  ; ARM64 specific code
ELSE
  ; Generic fallback code
ENDIF
```

This allows different code paths to be included based on the compilation target.

## Target Aliases

Targets can be given human-readable aliases:

```
TARGET_ALIAS CPU_X86_64 = 0x010103
TARGET_ALIAS GPU_CUDA = 0x020104

TARGET CPU_X86_64
; Code targeting x86-64 CPU
```

This improves code readability when targeting specific devices.

## Target Compatibility

COIL defines compatibility relationships between targets:

```
TARGET_COMPAT 0x010103, 0x010102  ; x86-64 code can run on x86-32
TARGET_COMPAT 0x020104, 0x020103  ; CUDA SM 7.5 code can run on SM 7.0
```

These relationships help the COIL processor make intelligent decisions about code execution.

## Example: Matrix Multiplication

```
; CPU implementation
TARGET 0x010103                  ; x86-64

SYM matrix_multiply_cpu
    SCOPEE
    ; Get parameters: A, B, C matrices
    VAR #1, TYPE_PTR             ; Matrix A
    VAR #2, TYPE_PTR             ; Matrix B
    VAR #3, TYPE_PTR             ; Result matrix C
    VAR #4, TYPE_INT32           ; Size N
    
    ; CPU implementation with loops
    ...
    
    SCOPEL
    RET

; GPU implementation
TARGET 0x020104                  ; NVIDIA CUDA SM 7.5

SYM matrix_multiply_gpu
    SCOPEE
    ; Get parameters and grid/block configuration
    VAR #1, TYPE_PTR             ; Matrix A
    VAR #2, TYPE_PTR             ; Matrix B
    VAR #3, TYPE_PTR             ; Result matrix C
    VAR #4, TYPE_INT32           ; Size N
    
    ; GPU implementation with parallelism
    ...
    
    SCOPEL
    RET

; Main function with device selection
TARGET 0x010103                  ; Back to CPU

SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    ; Check for GPU availability
    VAR #10, TYPE_INT32          ; Device capability flag
    QUERY_DEVICE 0x02, 0x01, 0x04, #10
    
    ; Branch based on available device
    CMP #10, 0
    BR_EQ use_cpu_implementation
    
    ; Use GPU implementation
    ...
    BR end_compute
    
use_cpu_implementation:
    ; Use CPU implementation
    ...
    
end_compute:
    ...
    
    SCOPEL
    RET
```

This example demonstrates how to provide different implementations for different targets and select between them at runtime.

## Configuration Files

Target specifications can be stored in external files and referenced:

```
INCLUDE "targets/x86-64.target"
INCLUDE "targets/cuda-sm75.target"

TARGET X86_64      ; Use predefined target
...
```

This promotes code organization and reusability.