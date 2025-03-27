# CASM Directives Reference

## Purpose

This document provides a comprehensive reference for all directives in CASM (COIL Assembly), focusing on their syntax, behavior, and usage. Directives control the assembly process rather than generating runtime code.

## Directive Overview

Directives in CASM:
- Begin with an uppercase letter
- Are processed during assembly, not at runtime
- Control how the assembler processes code
- Configure the target environment
- Manage data and sections
- Provide conditional assembly

## Core Directives

### PROC (0xB1)
Set the target processor type.

#### Syntax
```
PROC processor_code
```

#### Processor Codes
- `0x01`: CPU
- `0x02`: GPU (reserved for v3)
- `0x03`: TPU (reserved for v3)
- `0x04`: DSP (reserved for v3)
- `0x05`: FPGA (reserved for v3)
- `0x06-0xFE`: Reserved for future processor types
- `0xFF`: Custom processor

#### Example
```
PROC 0x01  ; Select CPU processor type
```

#### Binary Encoding
```
0xB1      ; PROC
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x01      ; CPU (0x01)
```

### ARCH (0xB0)
Set the target architecture and optionally its mode.

#### Syntax
```
ARCH architecture_code[, mode_code]
```

#### Architecture Codes
- `0x01`: x86
- `0x02`: ARM
- `0x03`: RISC-V
- `0x04`: MIPS
- `0x05`: PowerPC
- `0x06-0xFE`: Reserved for future architectures
- `0xFF`: Custom architecture

#### Mode Codes
For x86 (0x01):
- `0x01`: 16-bit mode (Real mode)
- `0x02`: 32-bit mode (Protected mode)
- `0x03`: 64-bit mode (Long mode)

For ARM (0x02):
- `0x01`: 32-bit mode (AArch32)
- `0x02`: 64-bit mode (AArch64)

For RISC-V (0x03):
- `0x01`: 32-bit mode (RV32)
- `0x02`: 64-bit mode (RV64)
- `0x03`: 128-bit mode (RV128)

#### Example
```
ARCH 0x01, 0x03  ; Select x86 architecture in 64-bit mode
```

#### Binary Encoding
```
0xB0      ; ARCH
0x02      ; Two operands
0x1020    ; TYPE_UNT8+IMM
0x01      ; x86 (0x01)
0x1020    ; TYPE_UNT8+IMM
0x03      ; 64-bit mode (0x03)
```

### SECTION (0xB4)
Define a new section in the output.

#### Syntax
```
SECTION name, attributes
```

#### Section Attributes
Section attributes are bit flags (combined with bitwise OR `|`):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data

#### Example
```
SECTION .text, 0x01 | 0x04  ; Executable and readable section
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized data
```

#### Binary Encoding
```
0xB4      ; SECTION
0x02      ; Two operands
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for ".text"
0x1320    ; TYPE_UNT32+IMM
0x05000000 ; Value 0x05 (0x01 | 0x04)
```

### DATA (0xB5)
Insert data into the program.

#### Syntax
```
DATA type, value
```

#### Example
```
; Define integer
DATA TYPE_INT32, 42

; Define string
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World", 0
```

#### Binary Encoding
```
0xB5      ; DATA
0x02      ; Two operands
0x0300    ; TYPE_INT32
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42
```

### ALIGN (0xB3)
Align the current position to a boundary.

#### Syntax
```
ALIGN alignment
```

#### Example
```
ALIGN 16  ; Align to 16-byte boundary
```

#### Binary Encoding
```
0xB3      ; ALIGN
0x01      ; One operand
0x1320    ; TYPE_UNT32+IMM
0x10000000 ; Value 16
```

## ABI Directives

### ABI (0xBA)
Begin an ABI (Application Binary Interface) definition.

#### Syntax
```
ABI name
  ; ABI-specific directives...
EXIT
```

#### Example
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

#### Binary Encoding
```
0xBA      ; ABI
0x01      ; One operand
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for ABI name

; ABI-specific instructions follow
; ...

0x00      ; EXIT (ends ABI definition)
```

### ABI-Specific Directives

These directives are only valid within an ABI definition block:

#### PARAMS (0x01)
Define register list for parameter passing.

```
PARAMS register1, register2, ...
```

#### RETS (0x02)
Define register list for return values.

```
RETS register1, register2, ...
```

#### CALLER (0x03)
Define caller-saved registers.

```
CALLER register1, register2, ...
```

#### CALLEE (0x04)
Define callee-saved registers.

```
CALLEE register1, register2, ...
```

#### SALLIGN (0x05)
Define stack alignment requirement.

```
SALLIGN alignment_value
```

#### RZONE (0x06)
Define red zone size.

```
RZONE size_value
```

## Symbol Management Directives

### EXTERN (0xBB)
Declare an external symbol reference.

#### Syntax
```
EXTERN symbol[, type]
```

#### Example
```
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

#### Binary Encoding
```
0xBB      ; EXTERN
0x02      ; Two operands
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "printf"
0xF800    ; TYPE_ABICTL
0x00      ; ABICTL_STANDARD
[abi_id]  ; ABI ID for "linux_x86_64"
```

### GLOBAL (0xBC)
Declare a symbol as globally visible.

#### Syntax
```
GLOBAL symbol
```

#### Example
```
GLOBAL main
```

#### Binary Encoding
```
0xBC      ; GLOBAL
0x01      ; One operand
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "main"
```

## Conditional Assembly Directives

### IF/ELIF/ELSE/ENDIF (0xB6-0xB9)
Control conditional assembly during assembly process.

#### Syntax
```
IF condition
  ; Code for when condition is true
ELIF alternative_condition
  ; Code for when alternative condition is true
ELSE
  ; Code for when no conditions are true
ENDIF
```

#### Example
```
IF ARCH == 0x01
  ; x86-specific code
ELIF ARCH == 0x02
  ; ARM-specific code
ELSE
  ; Generic code
ENDIF
```

#### Processing
These directives are processed during assembly and do not appear in the final binary output.

## Miscellaneous Directives

### INCLUDE (0xBD)
Include another file.

#### Syntax
```
INCLUDE "filename"
```

#### Example
```
INCLUDE "definitions.coilh"
```

#### Processing
The included file is processed as if its contents were inserted at the point of the INCLUDE directive.

### VERSION (0xBE)
Specify COIL version requirements.

#### Syntax
```
VERSION major, minor, patch
```

#### Example
```
VERSION 1, 0, 0  ; Requires COIL v1.0.0
```

#### Binary Encoding
```
0xBE      ; VERSION
0x03      ; Three operands
0x1020    ; TYPE_UNT8+IMM
0x01      ; Major version 1
0x1020    ; TYPE_UNT8+IMM
0x00      ; Minor version 0
0x1020    ; TYPE_UNT8+IMM
0x00      ; Patch version 0
```

## Directive Processing Phases

Directives are processed by the COIL assembler in specific phases:

1. **Preprocessing Phase**:
   - `INCLUDE` directive
   - `VERSION` directive
   - Conditional assembly directives (`IF`, `ELIF`, `ELSE`, `ENDIF`)

2. **Assembly Phase**:
   - `PROC`, `ARCH`, `MODE` directives
   - `SECTION`, `ALIGN` directives
   - `ABI` definition
   - `DATA` directive

3. **Linking Phase**:
   - `EXTERN` directive
   - `GLOBAL` directive

## Common Directive Patterns

### Target Specification Pattern
```
; Set the target processor and architecture
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64
```

### Standard Sections Pattern
```
; Define standard sections
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized
```

### Architecture-Specific Code Pattern
```
; Include processor-specific optimizations
IF ARCH == 0x01  ; x86
  ; x86-specific implementation
ELIF ARCH == 0x02  ; ARM
  ; ARM-specific implementation
ELSE
  ; Generic implementation
ENDIF
```

### ABI Definition Pattern
```
; Define a platform-specific ABI
ABI windows_x64
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, RDI, RSI, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

### Data Definition Pattern
```
; Define constant data
SECTION .data, 0x02 | 0x04 | 0x08
SYM hello_msg
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

; Define uninitialized data
SECTION .bss, 0x02 | 0x04 | 0x10
SYM buffer
ALIGN 16
DATA TYPE_ARRAY=TYPE_UNT8[1024]  ; 1KB buffer
```

## Best Practices

### Naming Conventions
- Use `.text` for code sections
- Use `.data` for initialized data
- Use `.bss` for uninitialized data
- Use `.rodata` for read-only data

### Section Organization
- Group related data and code in logical sections
- Use section attributes appropriate for content (e.g., don't make data executable)
- Consider alignment requirements for data

### Conditional Assembly
- Use conditional assembly to maintain a single codebase across architectures
- Provide fallbacks for architecture-specific features
- Test all code paths in conditional blocks

## Related Documentation

- [CASM Syntax](casm-syntax.md) - Fundamental syntax rules for CASM
- [CASM Instructions](casm-instructions.md) - Detailed description of CASM instructions
- [Binary Encoding](binary-encoding.md) - How CASM translates to binary COIL
- [ABI System](../spec/abi-system.md) - Detailed description of the ABI system