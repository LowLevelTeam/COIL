# COIL ABI Detailed Specification

## Overview

This document provides a comprehensive specification for the COIL Application Binary Interface (ABI) system. The ABI system is a core component that automates function calling conventions while maintaining native performance.

## Core ABI Concepts

The COIL ABI system abstracts the platform-specific details of function calls:

1. **Parameter Passing**: How arguments are transmitted to functions
2. **Return Value Handling**: How results are returned from functions
3. **Register Preservation**: Which registers must be saved by caller vs. callee
4. **Stack Management**: How stack frames are created and managed
5. **Alignment Requirements**: Memory alignment for the stack and parameters

## ABI Definition Format

An ABI is defined using the `ABI` directive with these components:

```
ABI abi_name
  PARAMS register_list    ; Registers for parameter passing
  RETS register_list      ; Registers for return values
  CALLER register_list    ; Caller-saved registers
  CALLEE register_list    ; Callee-saved registers
  SALLIGN alignment_value ; Stack alignment in bytes
  RZONE size_value        ; Red zone size in bytes
EXIT
```

### Required Components

Each ABI definition must include:

1. **PARAMS**: Parameter passing registers (1+)
2. **RETS**: Return value registers (1+)
3. **CALLER**: Caller-saved registers
4. **CALLEE**: Callee-saved registers
5. **SALLIGN**: Stack alignment requirement

### Optional Components

Optional ABI components include:

1. **RZONE**: Red zone size (0 if none)
2. **SHADOW**: Shadow space size (0 if none)
3. **ARGSTYLE**: Argument evaluation order (0 = left-to-right, 1 = right-to-left)

## Standard ABI Definitions

### x86-64 System V ABI

```
ABI x86_64_sysv
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
  SHADOW 0
  ARGSTYLE 0
EXIT
```

### x86-64 Windows ABI

```
ABI x86_64_windows
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, RDI, RSI, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
  SHADOW 32
  ARGSTYLE 0
EXIT
```

### ARM64 ABI

```
ABI arm64
  PARAMS X0, X1, X2, X3, X4, X5, X6, X7
  RETS X0, X1
  CALLER X0, X1, X2, X3, X4, X5, X6, X7, X9, X10, X11, X12, X13, X14, X15
  CALLEE X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30
  SALLIGN 16
  RZONE 0
  SHADOW 0
  ARGSTYLE 0
EXIT
```

### RISC-V ABI

```
ABI riscv
  PARAMS A0, A1, A2, A3, A4, A5, A6, A7
  RETS A0, A1
  CALLER A0, A1, A2, A3, A4, A5, A6, A7, T0, T1, T2, T3, T4, T5, T6
  CALLEE S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, SP, RA
  SALLIGN 16
  RZONE 0
  SHADOW 0
  ARGSTYLE 0
EXIT
```

## Platform-Independent ABI

The special `platform_default` ABI maps to the appropriate ABI for the current target:

```
ABI platform_default
  ; Maps to:
  ; - x86_64_sysv on Linux/macOS x86-64
  ; - x86_64_windows on Windows x86-64
  ; - arm64 on ARM64 platforms
  ; - riscv on RISC-V platforms
EXIT
```

## Register Mapping

Each architecture has its own register names which must be used in ABI definitions:

### x86-64 Register Names

| Register | Purpose |
|----------|---------|
| RAX, RBX, RCX, etc. | 64-bit general purpose |
| EAX, EBX, ECX, etc. | 32-bit general purpose |
| AX, BX, CX, etc. | 16-bit general purpose |
| AL, BL, CL, etc. | 8-bit general purpose |
| XMM0-XMM15 | 128-bit floating point/vector |
| YMM0-YMM15 | 256-bit floating point/vector |
| ZMM0-ZMM31 | 512-bit floating point/vector |

### ARM64 Register Names

| Register | Purpose |
|----------|---------|
| X0-X30 | 64-bit general purpose |
| W0-W30 | 32-bit general purpose |
| SP | Stack pointer |
| V0-V31 | 128-bit floating point/vector |

### RISC-V Register Names

| Register | Purpose |
|----------|---------|
| X0-X31 | Integer registers |
| A0-A7 | Function arguments/results |
| T0-T6 | Temporaries |
| S0-S11 | Saved registers |
| SP | Stack pointer |
| RA | Return address |
| F0-F31 | Floating point |

## Parameter Passing Rules

### Integer Parameters

1. Parameters are assigned to registers in declaration order
2. When registers are exhausted, parameters are placed on stack
3. Parameters larger than register size may use multiple registers or stack

### Floating-Point Parameters

1. Use architecture-specific floating-point registers where available
2. Follow platform conventions for mixed integer/floating-point parameters

### Compound Parameters

1. Small structures may be passed in registers according to their fields
2. Larger structures are passed by reference (pointer) or on stack

## Return Value Handling

### Single Return Values

1. Integer values returned in first integer return register
2. Floating-point values returned in first floating-point return register

### Multiple Return Values

1. Return values assigned to registers in sequence
2. When return registers are exhausted, values are returned via stack or memory

## ABI Control Types

ABI operations use the `TYPE_ABICTL` (0xF8) type with these control values:

```
ABICTL_STANDARD = 0x00  ; Use specified ABI convention
ABICTL_PARAM    = 0x01  ; Passing parameters via ABI
ABICTL_RET      = 0x02  ; Returning values via ABI
```

## Function Declaration with ABI

To declare a function using a specific ABI:

```
SYM function_name, TYPE_ABICTL=ABICTL_STANDARD=abi_name
```

## Parameter Passing

### Passing Parameters

To call a function with parameters:

```
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...
```

### Receiving Parameters

Within a function, parameters are accessed:

```
MOV dest, TYPE_ABICTL=ABICTL_PARAM=abi_name, parameter_index
```

## Return Value Handling

### Returning Values

To return values from a function:

```
RET TYPE_ABICTL=ABICTL_RET=abi_name, return_value1, return_value2, ...
```

### Receiving Return Values

After calling a function, return values are accessed:

```
MOV dest, TYPE_ABICTL=ABICTL_RET=abi_name [, return_value_index]
```

## Stack Frame Management

### Stack Frame Creation

When a function is called:

1. The caller saves caller-saved registers as needed
2. The caller aligns the stack according to SALLIGN
3. The caller pushes parameters that don't fit in registers
4. The callee creates a stack frame for local variables
5. The callee saves callee-saved registers as needed

### Stack Frame Destruction

When a function returns:

1. The callee restores callee-saved registers
2. The callee releases its stack frame
3. The callee returns control to caller
4. The caller restores caller-saved registers
5. The caller removes stack parameters if needed

## ABI Implementation Details

The COIL processor automatically handles:

1. **Register Allocation**: Maps variables to ABI-specified registers
2. **Stack Management**: Creates and destroys stack frames
3. **Register Preservation**: Saves and restores appropriate registers
4. **Parameter Passing**: Routes parameters to correct locations
5. **Return Value Handling**: Routes return values to correct locations

## Binary Encoding

ABI information is encoded in the binary format using TYPE_ABICTL:

```
; CALL function with ABI parameters
0x03         ; Opcode for CALL
0x03+N       ; 3 + N operands (N = parameter count)
0x9100 [sym] ; TYPE_SYM + symbol index for function
0xF800       ; TYPE_ABICTL
0x01         ; ABICTL_PARAM
[abi_id]     ; ABI identifier
[param1]     ; Parameter 1
...
[paramN]     ; Parameter N

; Return with ABI return values
0x04         ; Opcode for RET
0x02+N       ; 2 + N operands (N = return value count)
0xF800       ; TYPE_ABICTL
0x02         ; ABICTL_RET
[abi_id]     ; ABI identifier
[ret1]       ; Return value 1
...
[retN]       ; Return value N
```

## ABI Extension Mechanisms

### Custom ABIs

COIL allows custom ABIs to be defined for specific needs:

```
ABI my_custom_abi
  PARAMS R1, R2, R3      ; Custom parameter registers
  RETS R0               ; Custom return register
  CALLER R0, R1, R2, R3  ; Custom caller-saved registers
  CALLEE R4, R5, R6, R7  ; Custom callee-saved registers
  SALLIGN 8             ; Custom stack alignment
EXIT
```

### ABI Variants

Multiple variants of an ABI can be defined for different purposes:

```
ABI fastcall
  ; Fast calling convention with fewer preserved registers
EXIT

ABI safecall
  ; Safe calling convention with more preserved state
EXIT
```

## Interoperability

### Cross-ABI Calls

COIL supports calls between functions using different ABIs:

```
; Function using platform_default ABI
SYM function1, TYPE_ABICTL=ABICTL_STANDARD=platform_default

; Function using custom ABI
SYM function2, TYPE_ABICTL=ABICTL_STANDARD=my_custom_abi

; Call from platform_default to custom ABI
CALL function2, TYPE_ABICTL=ABICTL_PARAM=my_custom_abi, param1, param2
```

### Foreign Function Interface

For calling external functions with specific ABIs:

```
; Declare external C function with x86_64_sysv ABI
EXTERN c_function, TYPE_ABICTL=ABICTL_STANDARD=x86_64_sysv

; Call the external function
CALL c_function, TYPE_ABICTL=ABICTL_PARAM=x86_64_sysv, param1, param2
```

## Complete Function Example

```
; Define a function that adds two numbers using platform-default ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR #1, TYPE_INT32
    VAR #2, TYPE_INT32
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Calculate sum
    VAR #3, TYPE_INT32
    ADD #3, #1, #2
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    SCOPEL

; Call the function with two parameters
SYM main, TYPE_PARAM0=GLOB
    SCOPEE
    VAR #1, TYPE_INT32, 10
    VAR #2, TYPE_INT32, 20
    VAR #3, TYPE_INT32
    
    ; Call function with parameters
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2
    
    ; Get return value
    MOV #3, TYPE_ABICTL=ABICTL_RET=platform_default
    
    ; #3 now contains 30
    SCOPEL
    RET
```