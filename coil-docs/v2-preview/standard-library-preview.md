# COIL Standard Library Preview (v2)

## Overview

This document previews the COIL Standard Library planned for version 2. While not part of the current v1 specification, this provides a roadmap for implementers.

## Standard Library Principles

The COIL Standard Library adheres to these core principles:

1. **Portability**: Functions behave identically across all platforms
2. **Efficiency**: Platform-specific optimizations under a consistent API
3. **Minimalism**: Focus on essential functionality without bloat
4. **Safety**: Robust error handling and validation
5. **Consistency**: Uniform naming, parameter ordering, and error handling

## Library Modules

### Memory Management (memory)

Functions for dynamic memory allocation and management:

```
; Allocate memory
EXTERN memory_alloc, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL memory_alloc, TYPE_ABICTL=ABICTL_PARAM=platform_default, size
MOV ptr, TYPE_ABICTL=ABICTL_RET=platform_default

; Free memory
EXTERN memory_free, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL memory_free, TYPE_ABICTL=ABICTL_PARAM=platform_default, ptr
```

Key functions:
- `memory_alloc`: Allocate memory block
- `memory_realloc`: Resize memory block
- `memory_free`: Free memory block
- `memory_copy`: Copy memory blocks
- `memory_set`: Fill memory block
- `memory_compare`: Compare memory blocks

### Standard I/O (stdio)

Functions for console and standard stream operations:

```
; Print string to stdout
EXTERN stdio_print, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL stdio_print, TYPE_ABICTL=ABICTL_PARAM=platform_default, string_ptr

; Read line from stdin
EXTERN stdio_readline, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL stdio_readline, TYPE_ABICTL=ABICTL_PARAM=platform_default, buffer_ptr, buffer_size
MOV chars_read, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `stdio_print`: Print string to stdout
- `stdio_println`: Print string with newline
- `stdio_read`: Read from stdin
- `stdio_readline`: Read line from stdin
- `stdio_printf`: Formatted output
- `stdio_scanf`: Formatted input

### File Operations (fileio)

Functions for file system access:

```
; Open file
EXTERN fileio_open, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL fileio_open, TYPE_ABICTL=ABICTL_PARAM=platform_default, filename_ptr, mode_ptr
MOV file_handle, TYPE_ABICTL=ABICTL_RET=platform_default

; Read from file
EXTERN fileio_read, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL fileio_read, TYPE_ABICTL=ABICTL_PARAM=platform_default, file_handle, buffer_ptr, bytes_to_read
MOV bytes_read, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `fileio_open`: Open file
- `fileio_close`: Close file
- `fileio_read`: Read from file
- `fileio_write`: Write to file
- `fileio_seek`: Set file position
- `fileio_tell`: Get file position
- `fileio_delete`: Delete file
- `fileio_rename`: Rename file

### String Processing (string)

Functions for string manipulation:

```
; Get string length
EXTERN string_length, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL string_length, TYPE_ABICTL=ABICTL_PARAM=platform_default, string_ptr
MOV length, TYPE_ABICTL=ABICTL_RET=platform_default

; Compare strings
EXTERN string_compare, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL string_compare, TYPE_ABICTL=ABICTL_PARAM=platform_default, str1_ptr, str2_ptr
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `string_length`: Get string length
- `string_copy`: Copy string
- `string_concat`: Concatenate strings
- `string_compare`: Compare strings
- `string_find`: Find substring
- `string_replace`: Replace substring
- `string_split`: Split string by delimiter
- `string_trim`: Trim whitespace

### Mathematical Functions (math)

Advanced mathematical operations:

```
; Calculate sine
EXTERN math_sin, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL math_sin, TYPE_ABICTL=ABICTL_PARAM=platform_default, angle
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default

; Calculate power
EXTERN math_pow, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL math_pow, TYPE_ABICTL=ABICTL_PARAM=platform_default, base, exponent
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `math_sin`, `math_cos`, `math_tan`: Trigonometric functions
- `math_asin`, `math_acos`, `math_atan`: Inverse trigonometric functions
- `math_exp`, `math_log`, `math_log10`: Exponential and logarithmic functions
- `math_sqrt`, `math_pow`: Power functions
- `math_floor`, `math_ceil`, `math_round`: Rounding functions
- `math_abs`, `math_min`, `math_max`: Comparison functions

### Data Structures (container)

Common data structure implementations:

```
; Create dynamic array
EXTERN container_array_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL container_array_create, TYPE_ABICTL=ABICTL_PARAM=platform_default, element_size
MOV array_handle, TYPE_ABICTL=ABICTL_RET=platform_default

; Add element to array
EXTERN container_array_add, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL container_array_add, TYPE_ABICTL=ABICTL_PARAM=platform_default, array_handle, element_ptr
```

Key data structures:
- Dynamic arrays
- Linked lists
- Hash tables
- Trees
- Priority queues
- Graphs

### System Interface (system)

Functions for operating system interaction:

```
; Get environment variable
EXTERN system_getenv, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL system_getenv, TYPE_ABICTL=ABICTL_PARAM=platform_default, name_ptr
MOV value_ptr, TYPE_ABICTL=ABICTL_RET=platform_default

; Execute process
EXTERN system_exec, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL system_exec, TYPE_ABICTL=ABICTL_PARAM=platform_default, command_ptr
MOV status, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- `system_getenv`: Get environment variable
- `system_setenv`: Set environment variable
- `system_time`: Get system time
- `system_clock`: Get high-precision timer
- `system_exec`: Execute process
- `system_exit`: Exit program
- `system_info`: Get system information

### Thread Management (thread)

Functions for multi-threading:

```
; Create thread
EXTERN thread_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL thread_create, TYPE_ABICTL=ABICTL_PARAM=platform_default, function_ptr, arg_ptr
MOV thread_handle, TYPE_ABICTL=ABICTL_RET=platform_default

; Join thread
EXTERN thread_join, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL thread_join, TYPE_ABICTL=ABICTL_PARAM=platform_default, thread_handle
```

Key functions:
- `thread_create`: Create thread
- `thread_join`: Wait for thread completion
- `thread_detach`: Detach thread
- `thread_exit`: Exit current thread
- `thread_yield`: Yield to other threads
- `thread_sleep`: Sleep current thread

### Synchronization (sync)

Synchronization primitives:

```
; Create mutex
EXTERN sync_mutex_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL sync_mutex_create, TYPE_ABICTL=ABICTL_PARAM=platform_default
MOV mutex_handle, TYPE_ABICTL=ABICTL_RET=platform_default

; Lock mutex
EXTERN sync_mutex_lock, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL sync_mutex_lock, TYPE_ABICTL=ABICTL_PARAM=platform_default, mutex_handle
```

Key primitives:
- Mutexes
- Semaphores
- Condition variables
- Read-write locks
- Barriers
- Atomic operations

### Networking (net)

Networking functions:

```
; Create socket
EXTERN net_socket_create, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL net_socket_create, TYPE_ABICTL=ABICTL_PARAM=platform_default, domain, type, protocol
MOV socket_handle, TYPE_ABICTL=ABICTL_RET=platform_default

; Connect socket
EXTERN net_socket_connect, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL net_socket_connect, TYPE_ABICTL=ABICTL_PARAM=platform_default, socket_handle, address_ptr, port
MOV status, TYPE_ABICTL=ABICTL_RET=platform_default
```

Key functions:
- Socket creation and management
- Connection handling
- Data transmission
- Name resolution
- Protocol support

## Error Handling

The Standard Library uses a consistent error handling approach:

1. **Return Codes**: Functions return status codes
2. **Extended Error Information**: Additional error details available
3. **Error Categories**: Organized by module and type

```
; Standard pattern for error handling
CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, args
MOV status, TYPE_ABICTL=ABICTL_RET=platform_default, 0
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default, 1

; Check status
CMP status, 0
BR_NE handle_error

; Get extended error information if needed
EXTERN system_get_error, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL system_get_error, TYPE_ABICTL=ABICTL_PARAM=platform_default
MOV error_code, TYPE_ABICTL=ABICTL_RET=platform_default, 0
MOV error_msg, TYPE_ABICTL=ABICTL_RET=platform_default, 1
```

## Implementation Requirements

A compliant Standard Library implementation must:

1. Provide all specified functions with defined behavior
2. Support all required platforms
3. Use consistent error handling
4. Maintain backward compatibility
5. Be thread-safe where specified

## Version 3 Extensions

COIL v3 will extend the Standard Library with:

1. **Device Management**: Functions for device discovery and selection
2. **Cross-Device Memory**: Unified memory management across devices
3. **Heterogeneous Computing**: Task scheduling across multiple devices
4. **Advanced Parallelism**: Enhanced parallel processing

## Example Usage

```
; Complete example using Standard Library
PROC 0x01
ARCH 0x01, 0x03

SECTION .data, 0x02 | 0x04 | 0x08
SYM filename
DATA TYPE_ARRAY=TYPE_UNT8, "data.txt", 0

SYM format
DATA TYPE_ARRAY=TYPE_UNT8, "Read %d bytes", 0

SECTION .text, 0x01 | 0x04
EXTERN fileio_open, TYPE_ABICTL=ABICTL_STANDARD=platform_default
EXTERN fileio_read, TYPE_ABICTL=ABICTL_STANDARD=platform_default
EXTERN fileio_close, TYPE_ABICTL=ABICTL_STANDARD=platform_default
EXTERN stdio_printf, TYPE_ABICTL=ABICTL_STANDARD=platform_default
EXTERN memory_alloc, TYPE_ABICTL=ABICTL_STANDARD=platform_default
EXTERN memory_free, TYPE_ABICTL=ABICTL_STANDARD=platform_default

SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_PTR            ; File handle
    VAR #2, TYPE_PTR            ; Buffer
    VAR #3, TYPE_INT32          ; Bytes read
    VAR #4, TYPE_INT32, 1024    ; Buffer size
    
    ; Allocate buffer
    CALL memory_alloc, TYPE_ABICTL=ABICTL_PARAM=platform_default, #4
    MOV #2, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Open file
    CALL fileio_open, TYPE_ABICTL=ABICTL_PARAM=platform_default, filename, "r"
    MOV #1, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Read from file
    CALL fileio_read, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2, #4
    MOV #3, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; Close file
    CALL fileio_close, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1
    
    ; Print result
    CALL stdio_printf, TYPE_ABICTL=ABICTL_PARAM=platform_default, format, #3
    
    ; Free buffer
    CALL memory_free, TYPE_ABICTL=ABICTL_PARAM=platform_default, #2
    
    SCOPEL
    RET
```