# Directive Instructions (0xB0-0xBF)

## Purpose

This document defines the directive instructions in COIL, which control the assembly process rather than generating runtime code. Directives provide information to the COIL processor about how to handle code, target architectures, and other aspects of the compilation environment.

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

### PROC (0xB1)
Set the target processor type.

#### Assembly Syntax
```
PROC processor_code
```

#### Binary Encoding
```
0xB1                  ; Opcode for PROC
0x01                  ; One operand
[type]                ; Type of processor code
[value]               ; Processor code value
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
; Assembly
PROC 0x01  ; Select CPU processor type

; Binary
0xB1      ; PROC
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x01      ; CPU (0x01)
```

### ARCH (0xB0)
Set the target architecture and optionally its mode.

#### Assembly Syntax
```
ARCH architecture_code[, mode_code]
```

#### Binary Encoding
```
0xB0                  ; Opcode for ARCH
0x01/0x02             ; One or two operands
[arch_type]           ; Type of architecture code
[arch_value]          ; Architecture code value
[mode_type]           ; Type of mode code (if specified)
[mode_value]          ; Mode code value (if specified)
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
; Assembly
ARCH 0x01, 0x03  ; Select x86 architecture in 64-bit mode

; Binary
0xB0      ; ARCH
0x02      ; Two operands
0x1020    ; TYPE_UNT8+IMM
0x01      ; x86 (0x01)
0x1020    ; TYPE_UNT8+IMM
0x03      ; 64-bit mode (0x03)
```

### SECTION (0xB4)
Define a new section in the output.

#### Assembly Syntax
```
SECTION name, attributes
```

#### Binary Encoding
```
0xB4                  ; Opcode for SECTION
0x02                  ; Two operands
[name_type]           ; Type of section name
[name_value]          ; Section name value
[attr_type]           ; Type of attributes
[attr_value]          ; Attributes value
```

#### Section Attributes
Section attributes are bit flags:
- `0x01`: Executable
- `0x02`: Writable
- `0x04`: Readable
- `0x08`: Initialized data
- `0x10`: Uninitialized data

#### Example
```
; Assembly
SECTION .text, 0x01 | 0x04  ; Executable and readable section

; Binary
0xB4      ; SECTION
0x02      ; Two operands
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for ".text"
0x1320    ; TYPE_UNT32+IMM
0x05000000 ; Value 0x05 (0x01 | 0x04)
```

### DATA (0xB5)
Insert data into the program.

#### Assembly Syntax
```
DATA type, value
```

#### Binary Encoding
```
0xB5                  ; Opcode for DATA
0x02                  ; Two operands
[data_type]           ; Type of data
[value_type]          ; Type of value
[value]               ; Value
```

#### Example
```
; Assembly - Define integer
DATA TYPE_INT32, 42

; Binary
0xB5      ; DATA
0x02      ; Two operands
0x0300    ; TYPE_INT32
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42

; Assembly - Define string
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World", 0

; Binary
0xB5      ; DATA
0x02      ; Two operands
0xD310    ; TYPE_ARRAY=TYPE_UNT8
0xD320    ; String with null
[string]  ; String data
```

### ABI (0xBA)
Define an Application Binary Interface (ABI).

#### Assembly Syntax
```
ABI abi_name
  PARAMS register_list
  RETS register_list
  CALLER register_list
  CALLEE register_list
  SALLIGN alignment_value
  RZONE size_value
EXIT
```

#### Binary Encoding
ABI instruction:
```
0xBA                  ; Opcode for ABI
0x01                  ; One operand
[name_type]           ; Type of ABI name
[name_value]          ; ABI name value
```

Followed by ABI-specific instructions until EXIT (0x00):
```
0x01                  ; PARAMS
[count]               ; Parameter count
[reg1, reg2, ...]     ; Register list

0x02                  ; RETS
[count]               ; Return register count
[reg1, reg2, ...]     ; Register list

0x03                  ; CALLER
[count]               ; Caller-saved register count
[reg1, reg2, ...]     ; Register list

0x04                  ; CALLEE
[count]               ; Callee-saved register count
[reg1, reg2, ...]     ; Register list

0x05                  ; SALLIGN
[value]               ; Stack alignment value

0x06                  ; RZONE
[value]               ; Red zone size value

0x00                  ; EXIT
```

#### Example
```
; Assembly
ABI linux_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT

; Binary
0xBA      ; ABI
0x01      ; One operand
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "linux_x86_64"

0x01      ; PARAMS
0x06      ; 6 parameters
0x00      ; RDI
0x01      ; RSI
... 

0x02      ; RETS
0x02      ; 2 return registers
0x00      ; RAX
0x02      ; RDX

... 

0x05      ; SALLIGN
0x10      ; 16 byte alignment

0x06      ; RZONE
0x80      ; 128 byte red zone

0x00      ; EXIT
```

### Conditional Assembly Directives (0xB6-0xB9)
Control conditional assembly during assembly process.

#### Assembly Syntax
```
IF condition
  ; Code for when condition is true
ELIF alternative_condition
  ; Code for when alternative condition is true
ELSE
  ; Code for when no conditions are true
ENDIF
```

#### Binary Encoding
These directives are processed during assembly and do not appear in the final binary output.

IF:
```
0xB6                  ; Opcode for IF
0x01                  ; One operand
[cond_type]           ; Type of condition
[cond_value]          ; Condition value
```

ELIF:
```
0xB7                  ; Opcode for ELIF
0x01                  ; One operand
[cond_type]           ; Type of condition
[cond_value]          ; Condition value
```

ELSE:
```
0xB8                  ; Opcode for ELSE
0x00                  ; No operands
```

ENDIF:
```
0xB9                  ; Opcode for ENDIF
0x00                  ; No operands
```

#### Example
```
; Assembly
IF ARCH == 0x01
  ; x86-specific code
ELIF ARCH == 0x02
  ; ARM-specific code
ELSE
  ; Generic code
ENDIF
```

### EXTERN (0xBB)
Declare an external symbol reference.

#### Assembly Syntax
```
EXTERN symbol[, type]
```

#### Binary Encoding
```
0xBB                  ; Opcode for EXTERN
0x01/0x02             ; One or two operands
[sym_type]            ; Type of symbol
[sym_value]           ; Symbol value
[type]                ; Type information (if provided)
```

#### Example
```
; Assembly
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Binary
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

#### Assembly Syntax
```
GLOBAL symbol
```

#### Binary Encoding
```
0xBC                  ; Opcode for GLOBAL
0x01                  ; One operand
[sym_type]            ; Type of symbol
[sym_value]           ; Symbol value
```

#### Example
```
; Assembly
GLOBAL main

; Binary
0xBC      ; GLOBAL
0x01      ; One operand
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "main"
```

## Directive Processing

Directives are processed by the COIL processor during different phases:

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

## Common Usage Patterns

### Target Specification
```
; Set the target processor and architecture
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64
```

### Section Definition
```
; Define standard sections
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized
```

### Architecture-Specific Code
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

### ABI Definition
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

### Data Definition
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