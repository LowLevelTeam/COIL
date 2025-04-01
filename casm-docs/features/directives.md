# CASM Directives

## Overview

Directives are special instructions that control the assembly process rather than generating runtime code. They provide information to the assembler about the target architecture, memory organization, symbol handling, and other aspects of the compilation environment.

## Core Directives

### Target Specification

#### PROC (0xB1)
Specify the target processor type.

```
PROC processor_code
```

Common processor codes:
- `0x01`: CPU
- `0x02`: GPU
- `0x03`: TPU

Example:
```
PROC 0x01  ; Set processor type to CPU
```

#### ARCH (0xB0)
Specify the target architecture and mode.

```
ARCH architecture_code[, mode_code]
```

Common architecture codes:
- `0x01`: x86
- `0x02`: ARM
- `0x03`: RISC-V

Common mode codes (for x86):
- `0x01`: 16-bit mode
- `0x02`: 32-bit mode
- `0x03`: 64-bit mode

Example:
```
ARCH 0x01, 0x03  ; Set architecture to x86-64
```

### Section Management

#### SECTION (0xB4)
Define a new section with attributes.

```
SECTION name, attributes
```

Section attributes (combined with bitwise OR `|`):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)

Examples:
```
SECTION .text, 0x01 | 0x04  ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized
```

#### ALIGN (0xB3)
Align the current position to a specific boundary.

```
ALIGN alignment
```

Example:
```
ALIGN 16  ; Align to 16-byte boundary
```

#### DATA (0xB5)
Insert data directly into the current section.

```
DATA type, value
```

Examples:
```
DATA TYPE_INT32, 42            ; Insert 32-bit integer
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World", 0  ; Insert string with null terminator
```

### Symbol Management

#### SYM (0x01)
Define a symbol with optional attributes.

```
SYM symbol_name[, TYPE_PARAM0=symbol_attribute]
```

Common symbol attributes:
- `TYPE_PARAM0=TMP`: Symbol is local to current context
- `TYPE_PARAM0=FILE`: Symbol is visible within the file
- `TYPE_PARAM0=GLOB`: Symbol is globally visible

Examples:
```
SYM loop_start               ; Define local symbol
SYM main, TYPE_PARAM0=GLOB   ; Define global symbol
```

#### EXTERN (0xBB)
Declare an external symbol reference.

```
EXTERN symbol[, type]
```

Example:
```
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

#### GLOBAL (0xBC)
Declare a symbol as globally visible.

```
GLOBAL symbol
```

Example:
```
GLOBAL main
```

### ABI Specification

#### ABI (0xBA)
Begin an Application Binary Interface definition.

```
ABI abi_name
  ; ABI-specific directives
EXIT
```

Example:
```
ABI linux_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

### Conditional Assembly

#### IF/ELIF/ELSE/ENDIF (0xB6-0xB9)
Control conditional assembly based on compile-time conditions.

```
IF condition
  ; Code for when condition is true
ELIF alternative_condition
  ; Code for when alternative condition is true
ELSE
  ; Code for when no conditions are true
ENDIF
```

Example:
```
IF ARCH == 0x01
  ; x86-specific code
ELIF ARCH == 0x02
  ; ARM-specific code
ELSE
  ; Generic code
ENDIF
```

### File Inclusion

#### INCLUDE (0xBD)
Include another file during assembly.

```
INCLUDE "filename"
```

Example:
```
INCLUDE "definitions.coilh"
```

### Version Specification

#### VERSION (0xBE)
Specify COIL version requirements.

```
VERSION major, minor, patch
```

Example:
```
VERSION 1, 0, 0  ; Requires COIL v1.0.0
```

## Common Directive Patterns

### Standard Project Setup

```
; Version and target
VERSION 1, 0, 0
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

; Standard sections
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized

; Entry point
SYM main, TYPE_PARAM0=GLOB
```

### Architecture-Specific Code

```
; Conditional implementation based on architecture
IF ARCH == 0x01  ; x86
  ; x86-specific implementation
ELIF ARCH == 0x02  ; ARM
  ; ARM-specific implementation
ELSE
  ; Generic implementation
ENDIF
```

### Creating Data Tables

```
SECTION .data, 0x02 | 0x04 | 0x08
ALIGN 16
SYM lookup_table
DATA TYPE_ARRAY=TYPE_INT32, (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)

SYM message
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 0
```

## Related Components

- [CASM Basics](/casm-docs/basics/casm-basics.md) - Basic CASM syntax elements
- [Binary Format](/coil-docs/core/binary-format.md) - Binary encoding of directives
- [Device Architecture](/coil-docs/systems/device-architecture.md) - Target architecture details