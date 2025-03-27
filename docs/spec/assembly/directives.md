# CASM Directives Reference

## Purpose

This document provides a comprehensive reference for all directives in CASM (COIL Assembly). Unlike instructions that generate runtime code, directives control the assembly process itself, providing metadata and configuration to the assembler.

## Key Concepts

- **Directives vs. Instructions**: Directives control assembly rather than generating runtime code
- **Processing Phases**: When and how directives are processed during assembly
- **Directive Categories**: Functional groupings of related directives
- **Directive Options**: Valid parameters and flags for each directive

## Directive Categories

Directives are grouped into these functional categories:

- **Target Specification**: Define processor type, architecture, and mode
- **Section Management**: Define and control code and data sections
- **Symbol Management**: Control symbol visibility and external references
- **ABI Definition**: Define application binary interfaces
- **Conditional Assembly**: Control which code is assembled based on conditions
- **Metadata**: Provide information about the program

## Target Specification Directives

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

#### Mode Codes
For x86 (0x01):
- `0x01`: 16-bit mode (Real mode)
- `0x02`: 32-bit mode (Protected mode)
- `0x03`: 64-bit mode (Long mode)

For ARM (0x02):
- `0x01`: 32-bit mode (AArch32)
- `0x02`: 64-bit mode (AArch64)

#### Example
```
ARCH 0x01, 0x03  ; Select x86 architecture in 64-bit mode
```

## Section Management Directives

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

## Related Documentation

- [CASM Syntax](syntax.md) - Fundamental syntax rules for CASM
- [CASM Instructions](instructions.md) - Detailed description of CASM instructions
- [Binary Format](../binary-format.md) - How CASM translates to binary COIL
- [ABI System](../systems/abi-system.md) - Detailed description of the ABI system