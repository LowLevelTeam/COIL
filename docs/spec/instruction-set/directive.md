# Directive Instructions (0xB0-0xBF)

## Purpose

This document defines the directive instructions in COIL, which control the assembly process rather than generating runtime code. Directives provide information to the assembler and processor about how to handle code, target architectures, and other aspects of the compilation environment.

## Key Concepts

- **Assembly Control**: How directives affect the assembly process
- **Target Specification**: How to define target processors and architectures
- **Section Management**: How to organize code and data
- **Symbol Handling**: How to manage symbol visibility and references
- **ABI Definition**: How to define calling conventions

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xB0   | ARCH     | `ARCH architecture[, mode]` | Set target architecture |
| 0xB1   | PROC     | `PROC processor` | Set processor type |
| 0xB2   | MODE     | `MODE mode` | Set processor mode |
| 0xB3   | ALIGN    | `ALIGN alignment` | Align to boundary |
| 0xB4   | SECTION  | `SECTION name, attributes` | Define section |
| 0xB5   | DATA     | `DATA type, value` | Insert data |
| 0xB6   | IF       | `IF condition` | Begin conditional assembly |
| 0xB7   | ELIF     | `ELIF condition` | Alternative in conditional assembly |
| 0xB8   | ELSE     | `ELSE` | Default case in conditional assembly |
| 0xB9   | ENDIF    | `ENDIF` | End conditional assembly |
| 0xBA   | ABI      | `ABI name` | Begin ABI definition |
| 0xBB   | EXTERN   | `EXTERN symbol[, type]` | Declare external symbol |
| 0xBC   | GLOBAL   | `GLOBAL symbol` | Declare global symbol |
| 0xBD   | INCLUDE  | `INCLUDE filename` | Include file |
| 0xBE   | VERSION  | `VERSION major, minor, patch` | Specify COIL version |

## Detailed Descriptions

### Target Specification

#### PROC (0xB1)
Specify the target processor type.

##### Syntax
```
PROC processor_code
```

##### Processor Codes
- `0x01`: CPU
- `0x02`: GPU (reserved for v3)
- `0x03`: TPU (reserved for v3)
- `0x04`: DSP (reserved for v3)
- `0x05`: FPGA (reserved for v3)
- `0xFF`: Custom processor

##### Example
```
PROC 0x01  ; Select CPU processor type
```

#### ARCH (0xB0)
Specify the target architecture and optionally its mode.

##### Syntax
```
ARCH architecture_code[, mode_code]
```

##### Architecture Codes
- `0x01`: x86
- `0x02`: ARM
- `0x03`: RISC-V
- `0x04`: MIPS
- `0x05`: PowerPC

##### Example
```
ARCH 0x01, 0x03  ; Select x86 architecture in 64-bit mode
```

### Section Management

#### SECTION (0xB4)
Define a new section with specific attributes.

##### Syntax
```
SECTION name, attributes
```

##### Section Attributes
Section attributes are bit flags (combined with bitwise OR `|`):
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data (BSS)

##### Example
```
SECTION .text, 0x01 | 0x04  ; Executable and readable section
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized data
```

#### ALIGN (0xB3)
Align the current position to a specified boundary.

##### Syntax
```
ALIGN alignment
```

##### Example
```
ALIGN 16  ; Align to 16-byte boundary
```

#### DATA (0xB5)
Insert data directly into the program.

##### Syntax
```
DATA type, value
```

##### Example
```
; Define integer value
DATA TYPE_INT32, 42

; Define string with null terminator
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World", 0
```

### ABI Definition

#### ABI (0xBA)
Begin an Application Binary Interface definition.

##### Syntax
```
ABI name
  ; ABI-specific directives
EXIT
```

##### ABI-Specific Instructions
- `PARAMS`: Register list for parameter passing
- `RETS`: Register list for return values
- `CALLER`: Caller-saved registers
- `CALLEE`: Callee-saved registers
- `SALLIGN`: Stack alignment requirement
- `RZONE`: Red zone size

##### Example
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

### Symbol Management

#### EXTERN (0xBB)
Declare an external symbol reference.

##### Syntax
```
EXTERN symbol[, type]
```

##### Example
```
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

#### GLOBAL (0xBC)
Declare a symbol as globally visible.

##### Syntax
```
GLOBAL symbol
```

##### Example
```
GLOBAL main
```

### Conditional Assembly

#### IF/ELIF/ELSE/ENDIF (0xB6-0xB9)
Control conditional assembly based on compile-time conditions.

##### Syntax
```
IF condition
  ; Code for when condition is true
ELIF alternative_condition
  ; Code for when alternative condition is true
ELSE
  ; Code for when no conditions are true
ENDIF
```

##### Example
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

##### Syntax
```
INCLUDE "filename"
```

##### Example
```
INCLUDE "system_definitions.coilh"
```

### Version Specification

#### VERSION (0xBE)
Specify COIL version requirements.

##### Syntax
```
VERSION major, minor, patch
```

##### Example
```
VERSION 1, 0, 0  ; Requires COIL v1.0.0
```

## Directive Processing Phases

Directives are processed by the COIL assembler in specific phases:

1. **Preprocessing Phase**:
   - `INCLUDE` directive
   - `VERSION` directive
   - Conditional assembly directives

2. **Assembly Phase**:
   - `PROC`, `ARCH`, `MODE` directives
   - `SECTION`, `ALIGN` directives
   - `ABI` definition
   - `DATA` directive

3. **Linking Phase**:
   - `EXTERN` directive
   - `GLOBAL` directive

## Common Usage Patterns

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

## Related Documentation

- [CASM Directives](../spec/assembly/directives.md) - Detailed assembly directives
- [CASM Syntax](../spec/assembly/syntax.md) - General assembly syntax
- [Object Format](../implementation/object-format.md) - Object file format details
- [ABI System](../spec/systems/abi-system.md) - Details on the ABI system