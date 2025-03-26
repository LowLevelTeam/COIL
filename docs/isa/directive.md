# Directive Instructions (0xB0-0xBF)

## Purpose

This document defines the directive instructions in COIL, which control the assembly process rather than generating runtime code. Directives provide information to the COIL processor about how to handle the code, which processor to target, and other aspects of the compilation environment.

## Processor and Architecture Selection

### PROC (0xB1)
Set the target processor type.

```
Operands:
- Processor: TYPE_UNT8 (processor code)
```

Processor codes:
```
0x01 - CPU
0x02 - GPU (reserved for v3)
0x03 - TPU (reserved for v3)
0x04 - DSP (reserved for v3)
0x05 - FPGA (reserved for v3)
0x06-0xFE - Reserved for future processor types
0xFF - Custom processor
```

Example:
```
PROC 0x01  ; Select CPU processor type
```

Binary encoding:
```
0xB1      ; Opcode for PROC
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x01      ; Value 0x01 (CPU)
```

### ARCH (0xB0)
Set the target architecture and optionally its mode. This directive must be preceded by the PROC directive as architecture is specific to a processor type.

```
Operands:
- Architecture: TYPE_UNT8 (architecture code)
- Mode: TYPE_UNT8 (mode code, optional)
```

CPU architecture codes:
```
0x01 - x86
0x02 - ARM
0x03 - RISC-V
0x04 - MIPS
0x05 - PowerPC
0x06-0xFE - Reserved for future architectures
0xFF - Custom architecture
```

Mode codes for x86:
```
0x01 - 16-bit mode (Real mode)
0x02 - 32-bit mode (Protected mode)
0x03 - 64-bit mode (Long mode)
```

Mode codes for ARM:
```
0x01 - 32-bit mode (AArch32)
0x02 - 64-bit mode (AArch64)
```

Mode codes for RISC-V:
```
0x01 - 32-bit mode (RV32)
0x02 - 64-bit mode (RV64)
0x03 - 128-bit mode (RV128)
```

Example:
```
PROC 0x01           ; Select CPU
ARCH 0x01, 0x03     ; Select x86 architecture in 64-bit mode
```

Binary encoding:
```
0xB0      ; Opcode for ARCH
0x01/0x02 ; One or two operands
0x1020    ; TYPE_UNT8+IMM
0x01      ; Value 0x01 (x86)
0x1020    ; TYPE_UNT8+IMM (if mode specified)
0x03      ; Value 0x03 (64-bit mode)
```

## Memory and Data Organization

### ALIGN (0xB3)
Align the next instruction or data to a specified boundary.

```
Operands:
- Alignment: Non TYPE_VOID
```

Example:
```
ALIGN 16            ; Align to 16-byte boundary
```

Binary encoding:
```
0xB3      ; Opcode for ALIGN
0x01      ; One operand
[type1]   ; Type of alignment value
[value1]  ; Alignment value
```

### SECTION (0xB4)
Define a new section in the output.

```
Operands:
- Name: TYPE_SYM
- Attributes: Non TYPE_VOID
```

Section attributes are bit flags:
```
0x01 - Executable
0x02 - Writable
0x04 - Readable
0x08 - Initialized data
0x10 - Uninitialized data
```

Example:
```
SECTION .text, 0x01 | 0x04  ; Executable and readable section
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized data
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized data
```

Binary encoding:
```
0xB4      ; Opcode for SECTION
0x02      ; Two operands
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID for section name
[type2]   ; Type of attributes
[value2]  ; Attributes value
```

### DATA (0xB5)
Insert data into the program.

```
Operands:
- Type: TYPE_*
- Value: Non TYPE_VOID
```

Example:
```
; Define integer data
DATA TYPE_INT32, 42

; Define string data
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World", 0

; Define array
DATA TYPE_ARRAY=TYPE_INT32, (1, 2, 3, 4, 5)

; Define uninitialized buffer
DATA TYPE_ARRAY=TYPE_UNT8[100]  ; 100 bytes of uninitialized data
```

Binary encoding:
```
0xB5      ; Opcode for DATA
0x02      ; Two operands
[type1]   ; Type of data
[type2]   ; Type of value
[value2]  ; Value
```

## Conditional Assembly

### IF (0xB6)
Begin a conditional assembly block. This allows for processor-specific code without complicating the runtime model.

```
Operands:
- Condition: Non TYPE_VOID
```

Common condition patterns:
```
IF ARCH == 0x01         ; If architecture is x86
IF ARCH == 0x01 && MODE == 0x03  ; If architecture is x86 in 64-bit mode
IF PROC == 0x01         ; If processor is CPU
```

Example:
```
IF ARCH == 0x01
    ; x86-specific code
    MOV TYPE_RGP=RAX, 42
ENDIF
```

Binary encoding:
```
0xB6      ; Opcode for IF
0x01      ; One operand
[type1]   ; Type of condition
[value1]  ; Condition value
```

Note: Conditional assembly directives are processed during assembly and do not appear in the final binary output.

### ELIF (0xB7)
Alternative condition for conditional assembly.

```
Operands:
- Condition: Non TYPE_VOID
```

Example:
```
IF ARCH == 0x01
    ; x86-specific code
ELIF ARCH == 0x02
    ; ARM-specific code
ENDIF
```

Binary encoding:
```
0xB7      ; Opcode for ELIF
0x01      ; One operand
[type1]   ; Type of condition
[value1]  ; Condition value
```

### ELSE (0xB8)
Default case for conditional assembly.

```
Operands:
- None
```

Example:
```
IF ARCH == 0x01
    ; x86-specific code
ELSE
    ; Generic code for other architectures
ENDIF
```

Binary encoding:
```
0xB8      ; Opcode for ELSE
0x00      ; Zero operands
```

### ENDIF (0xB9)
End conditional assembly block.

```
Operands:
- None
```

Example:
```
IF PROC == 0x01
    ; CPU-specific code
ENDIF
```

Binary encoding:
```
0xB9      ; Opcode for ENDIF
0x00      ; Zero operands
```

## ABI Management

### ABI (0xBA)
Define an Application Binary Interface (ABI). Once this directive is encountered, the instruction set temporarily changes to the ABI-specific instruction set until EXIT is encountered.

```
Operands:
- Name: TYPE_SYM
```

ABI-specific instructions:
```
0x00 EXIT    - End ABI definition block
0x01 PARAMS  - Parameter registers
0x02 RETS    - Return registers
0x03 CALLER  - Caller-saved registers
0x04 CALLEE  - Callee-saved registers
0x05 SALLIGN - Stack alignment
0x06 RZONE   - Red zone size
```

Example:
```
ABI standard_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

Binary encoding:
```
0xBA      ; Opcode for ABI
0x01      ; One operand
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID for ABI name

; ABI-specific instructions follow, until EXIT
0x01      ; PARAMS
...

0x00      ; EXIT
```

## Specialized Directives

### MODE (0xB2)
Set the processor mode without changing the architecture. This is an alternative to specifying the mode in the ARCH directive.

```
Operands:
- Mode: TYPE_UNT8 (mode code)
```

Example:
```
PROC 0x01           ; Select CPU
ARCH 0x01           ; Select x86 architecture
MODE 0x03           ; Set 64-bit mode
```

Binary encoding:
```
0xB2      ; Opcode for MODE
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x03      ; Value 0x03 (64-bit mode)
```

### EXTERN (0xBB)
Declare an external symbol reference.

```
Operands:
- Symbol: TYPE_SYM
- Type: TYPE_* (optional)
```

Example:
```
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

Binary encoding:
```
0xBB      ; Opcode for EXTERN
0x01/0x02 ; One or two operands
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
[type2]   ; Type information (if provided)
```

### GLOBAL (0xBC)
Declare a symbol as globally visible.

```
Operands:
- Symbol: TYPE_SYM
```

Example:
```
GLOBAL main
```

Binary encoding:
```
0xBC      ; Opcode for GLOBAL
0x01      ; One operand
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
```

### INCLUDE (0xBD)
Include another COIL file.

```
Operands:
- Filename: TYPE_ARRAY=TYPE_UNT8
```

Example:
```
INCLUDE "header.coilh"
```

Binary encoding:
```
0xBD      ; Opcode for INCLUDE
0x01      ; One operand
0xD310    ; TYPE_ARRAY=TYPE_UNT8
[string]  ; Filename string
```

Note: The INCLUDE directive is processed during assembly and does not appear in the final binary output.

### VERSION (0xBE)
Specify the minimum COIL version required.

```
Operands:
- Major: TYPE_UNT8
- Minor: TYPE_UNT8
- Patch: TYPE_UNT8
```

Example:
```
VERSION 1, 0, 0  ; Requires COIL v1.0.0 or later
```

Binary encoding:
```
0xBE      ; Opcode for VERSION
0x03      ; Three operands
0x1020    ; TYPE_UNT8+IMM
0x01      ; Major version
0x1020    ; TYPE_UNT8+IMM
0x00      ; Minor version
0x1020    ; TYPE_UNT8+IMM
0x00      ; Patch version
```

## Usage Patterns

### Organization of Sections

Typical section organization in a COIL program:

```
PROC 0x01                     ; Select CPU
ARCH 0x01, 0x03               ; Select x86-64

SECTION .text, 0x01 | 0x04    ; Executable code
    ; Code goes here...

SECTION .data, 0x02 | 0x04 | 0x08  ; Initialized data
    ; Initialized data goes here...

SECTION .bss, 0x02 | 0x04 | 0x10   ; Uninitialized data
    ; Uninitialized data goes here...
```

### Processor-Specific Optimizations

Using conditional assembly for processor-specific optimizations:

```
; Function with processor-specific optimizations
SYM optimized_function
    IF ARCH == 0x01  ; x86
        ; x86-specific implementation
        IF MODE == 0x03  ; 64-bit
            ; x86-64 specific code
        ELSE
            ; x86-32 specific code
        ENDIF
    ELIF ARCH == 0x02  ; ARM
        ; ARM-specific implementation
    ELSE
        ; Generic implementation
    ENDIF
```

### Custom ABI Definition

Defining a custom ABI:

```
; Define a custom ABI for a specialized calling convention
ABI my_custom_abi
    PARAMS R0, R1, R2          ; Parameters in R0, R1, R2
    RETS R0                    ; Return value in R0
    CALLER R0, R1, R2, R3      ; Caller-saved registers
    CALLEE R4, R5, R6, R7, SP  ; Callee-saved registers
    SALLIGN 8                  ; 8-byte stack alignment
    RZONE 0                    ; No red zone
EXIT

; Use the custom ABI
SYM specialized_function, TYPE_ABICTL=ABICTL_STANDARD=my_custom_abi
    ; Function implementation
```

### Data Initialization

Different ways to initialize data:

```
SECTION .data, 0x02 | 0x04 | 0x08
    ; Integer data
    SYM int_value
    DATA TYPE_INT32, 42
    
    ; String data
    SYM hello_string
    DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 0
    
    ; Array data
    SYM int_array
    DATA TYPE_ARRAY=TYPE_INT32, (1, 2, 3, 4, 5)
    
    ; Structure data
    SYM point
    DATA TYPE_STRUCT=point_type, 10, 20  ; x=10, y=20

SECTION .bss, 0x02 | 0x04 | 0x10
    ; Uninitialized buffer
    SYM buffer
    ALIGN 16                       ; Align to 16-byte boundary
    DATA TYPE_ARRAY=TYPE_UNT8[1024]  ; 1KB buffer
```

## Best Practices

1. **Start with processor selection**: Begin each file with PROC and ARCH directives to establish the target context.

2. **Organize code into sections**: Use appropriate sections for code, data, and BSS.

3. **Align data appropriately**: Use ALIGN directive to ensure proper alignment for data types, especially for SIMD operations.

4. **Use conditional assembly for portability**: Wrap processor-specific code in IF/ENDIF blocks to maintain portability.

5. **Define ABIs clearly**: Document custom ABIs thoroughly and follow standard ABIs when possible.

6. **Group related directives**: Keep related directives together for better readability.

7. **Use namespaces with sections**: When defining symbols, consider using name prefixes or separate sections to avoid conflicts.

8. **Document version requirements**: Use the VERSION directive to specify the minimum required COIL version.

## Directive Processing

Directives are processed by the COIL processor during different phases:

1. **Preprocessing Phase**:
   - INCLUDE directive
   - VERSION directive
   - Conditional assembly directives (IF, ELIF, ELSE, ENDIF)

2. **Assembly Phase**:
   - PROC, ARCH, MODE directives
   - SECTION, ALIGN directives
   - ABI definition
   - DATA directive

3. **Linking Phase**:
   - EXTERN directive
   - GLOBAL directive

Some directives influence the processing of subsequent code, while others affect the final output format. The COIL processor ensures all directives are applied appropriately to produce the correct result.