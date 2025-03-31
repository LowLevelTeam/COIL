# COIL Standard Library Overview

## Purpose

The COIL Standard Library provides a collection of standardized functions with consistent interfaces across all platforms. It serves as a foundation for portable COIL development, eliminating the need to reinvent common operations for each target platform.

## Design Philosophy

The Standard Library is designed with these principles:

1. **Portability**: Functions work identically across all supported platforms
2. **Efficiency**: Implementations optimize for the target architecture
3. **Minimalism**: Focus on essential functionality without bloat
4. **Consistency**: Uniform naming, parameter ordering, and error handling

## Implementation Notes

The Standard Library is not part of the core COIL specification. Rather, it is a standardized API that COIL implementations should provide:

- It consists of functions with standardized ABIs
- It provides a uniform interface to OS services
- It abstracts platform-specific details
- It does not require any language extensions or special features

The Standard Library itself is implemented in COIL, making it self-hosting and platform-independent.

## Library Categories

The Standard Library encompasses several functional categories:

1. **Core Services**: Memory management, error handling, process control
2. **I/O Operations**: File handling, stream I/O, console operations
3. **String Processing**: String manipulation, formatting, parsing
4. **Data Structures**: Common containers and algorithms
5. **System Interface**: OS-specific functionality with consistent API
6. **Mathematical Functions**: Numeric operations and calculations

## ABI Standardization

To ensure interoperability, all Standard Library functions follow consistent ABI conventions:

- Parameter passing follows platform-specific optimal conventions
- Return values use consistent formats
- Error reporting follows a uniform pattern
- Resource management responsibilities are clearly defined

## Usage Example

```
; Example of using Standard Library functions
EXTERN stdio_print, TYPE_ABICTL=ABICTL_STANDARD=platform_default

SECTION .data, 0x02 | 0x04 | 0x08
SYM message
DATA TYPE_ARRAY=TYPE_UNT8, "Hello from Standard Library", 0

SECTION .text, 0x01 | 0x04
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_PTR, message
    
    ; Call Standard Library function to print message
    CALL stdio_print, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1
    
    SCOPEL
    RET
```

## Standard Library vs. User Code

The Standard Library is distinct from user code in several ways:

1. Standardized interface that remains stable
2. Available on all COIL implementations 
3. Optimized for each platform
4. Compatible with all COIL versions from v2 onward

Implementers may provide platform-specific optimizations under the hood while maintaining the standardized interface.