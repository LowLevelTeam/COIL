# COIL Future Roadmap

## Version Timeline

COIL development follows a structured versioning strategy:

| Version | Focus | Status |
|---------|-------|--------|
| Version 1 | Core Specification | Current |
| Version 2 | Standard Library | Planned |
| Version 3 | Multi-Device Support | Future |

## Version 1: Core Specification (Current)

COIL v1 establishes the foundation:

- Complete CPU-oriented instruction set
- Comprehensive type system
- Binary formats (.coil and .coilo)
- ABI mechanisms for function calling
- Variable and scope systems
- Basic toolchain support

## Version 2: Standard Library

COIL v2 will extend v1 by defining a comprehensive standard library:

### Core Services

Functions for memory management and system interfaces:
```
; Memory allocation 
EXTERN memory_alloc, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL memory_alloc, TYPE_ABICTL=ABICTL_PARAM=platform_default, size
MOV ptr, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `memory_alloc`: Allocate memory block
- `memory_realloc`: Resize memory block
- `memory_free`: Free memory block
- `memory_copy`: Copy memory blocks
- `memory_set`: Fill memory block

### Standard I/O

Functions for console and stream operations:
```
; Print string to stdout
EXTERN stdio_print, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL stdio_print, TYPE_ABICTL=ABICTL_PARAM=platform_default, string_ptr
```

Key functions:
- `stdio_print`: Print string to stdout
- `stdio_readline`: Read line from stdin
- `stdio_printf`: Formatted output
- `stdio_scanf`: Formatted input

### File Operations

Functions for file system access:
```
; Open file
EXTERN fileio_open, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL fileio_open, TYPE_ABICTL=ABICTL_PARAM=platform_default, filename_ptr, mode_ptr
MOV file_handle, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `fileio_open`: Open file
- `fileio_close`: Close file
- `fileio_read`: Read from file
- `fileio_write`: Write to file

### String Processing

Functions for string manipulation:
```
; Get string length
EXTERN string_length, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL string_length, TYPE_ABICTL=ABICTL_PARAM=platform_default, string_ptr
MOV length, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `string_length`: Get string length
- `string_copy`: Copy string
- `string_compare`: Compare strings
- `string_find`: Find substring

### Mathematical Functions

Advanced mathematical operations:
```
; Calculate sine
EXTERN math_sin, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL math_sin, TYPE_ABICTL=ABICTL_PARAM=platform_default, angle
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `math_sin`, `math_cos`, `math_tan`: Trigonometric functions
- `math_exp`, `math_log`: Exponential and logarithmic
- `math_sqrt`, `math_pow`: Power functions

### Data Structures

Common data structure implementations:
```
; Create dynamic array
EXTERN container_array_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL container_array_create, TYPE_ABICTL=ABICTL_PARAM=platform_default, element_size
MOV array_handle, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key data structures:
- Dynamic arrays
- Linked lists
- Hash tables
- Trees

### Thread Management

Functions for multi-threading:
```
; Create thread
EXTERN thread_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL thread_create, TYPE_ABICTL=ABICTL_PARAM=platform_default, function_ptr, arg_ptr
MOV thread_handle, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `thread_create`: Create thread
- `thread_join`: Wait for thread completion
- `thread_exit`: Exit current thread
- `thread_sleep`: Sleep current thread

## Version 3: Multi-Device Support

COIL v3 will enable programs to utilize multiple heterogeneous processing units:

### Device Types

| Device Code | Device Type | Description |
|-------------|-------------|-------------|
| 0x01        | CPU         | Central Processing Unit |
| 0x02        | GPU         | Graphics Processing Unit |
| 0x03        | TPU         | Tensor Processing Unit |
| 0x04        | DSP         | Digital Signal Processor |
| 0x05        | FPGA        | Field Programmable Gate Array |

### Device Selection

COIL v3 will provide directives for targeting specific devices:
```
; Define a CPU-targeted section
PROC 0x01
SECTION .cpu_code, 0x01 | 0x04

; Define a GPU-targeted section
PROC 0x02
SECTION .gpu_code, 0x01 | 0x04
```

### Device-Specific Code

Code can be tailored to different device types:
```
; CPU-specific implementation
PROC 0x01
SYM matrix_multiply_cpu
    SCOPEE
    ; CPU-optimized matrix multiplication
    SCOPEL
    RET

; GPU-specific implementation
PROC 0x02
SYM matrix_multiply_gpu
    SCOPEE
    ; GPU-optimized matrix multiplication
    SCOPEL
    RET
```

### Unified Memory Model

COIL v3 will feature a unified memory model across devices:
```
; Allocate unified memory (accessible by all devices)
EXTERN mem_alloc_unified, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL mem_alloc_unified, TYPE_ABICTL=ABICTL_PARAM=platform_default, size
MOV unified_ptr, TYPE_ABICTL=ABICTL_RET=platform_default
```

Memory types:
- Host Memory: CPU-accessible memory
- Device Memory: Device-local memory
- Unified Memory: Automatically managed across devices

### Task-Based Parallelism

COIL v3 will introduce a task-based programming model:
```
; Define a task
TASK matrix_add_task, TYPE_PARAM0=DEVICE_ANY
    SCOPEE
    ; Task implementation
    SCOPEL
    RET

; Submit task for execution
EXTERN task_submit, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL task_submit, TYPE_ABICTL=ABICTL_PARAM=platform_default, matrix_add_task, args_ptr
MOV task_id, TYPE_ABICTL=ABICTL_RET=platform_default
```

### JIT Compilation

COIL v3 will support Just-In-Time compilation:
```
; JIT compile COIL code
EXTERN jit_compile, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL jit_compile, TYPE_ABICTL=ABICTL_PARAM=platform_default, coil_buffer_ptr, size, device_type
MOV function_ptr, TYPE_ABICTL=ABICTL_RET=platform_default
```

## Version Compatibility

Each new COIL version maintains compatibility:
- COIL v2 will execute all valid COIL v1 programs
- COIL v3 will execute all valid COIL v2 and v1 programs
- Feature isolation ensures clear boundaries
- Migration tools help with version transitions

## Timeline

- **2025 Q1-Q2**: Finalize COIL v1 specification
- **2025 Q3-Q4**: First reference implementation 
- **2026 Q1-Q2**: Draft COIL v2 standard library
- **2026 Q3-Q4**: COIL v2 implementation and testing
- **2027**: COIL v3 design and specification
- **2028**: COIL v3 implementation and ecosystem