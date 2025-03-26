# COIL ABI System

## Purpose

This document defines the Application Binary Interface (ABI) system for COIL. The ABI system provides a processor-independent mechanism for function calls, parameter passing, and return value handling, eliminating the need for manual register management while maintaining native performance.

## Key Concepts

### What Is an ABI?

An ABI (Application Binary Interface) defines the conventions for:
- How parameters are passed to functions
- How return values are handled
- Which registers must be preserved by callers vs. callees
- Stack alignment requirements
- Other calling convention details

Traditional assembly languages require programmers to manually implement these conventions. COIL's ABI system automates this process while maintaining native performance.

### ABI Abstraction

The COIL ABI system abstracts over the specific register and stack conventions of different processors and architectures. This allows COIL code to be portable across different platforms while maintaining efficient function calls.

## ABI Definition

### Syntax

An ABI definition in COIL-ASM follows this format:

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

### Binary Encoding

When the `ABI` directive is used, the instruction set temporarily changes to use a special set of ABI definition opcodes until the `EXIT` opcode is encountered:

```
0x00 EXIT     - End ABI definition block
0x01 PARAMS   - Define register mapping for parameter values
0x02 RETS     - Define register mapping for return values
0x03 CALLER   - Define caller-saved registers
0x04 CALLEE   - Define callee-saved registers
0x05 SALLIGN  - Define stack alignment
0x06 RZONE    - Define the red zone size
```

## Using ABIs

### ABI Control Type

COIL uses the `TYPE_ABICTL` (0xF8) type for ABI operations, with the following control values:

```
ABICTL_STANDARD = 0x00  ; Use specified ABI convention
ABICTL_PARAM    = 0x01  ; Passing parameters via ABI
ABICTL_RET      = 0x02  ; Returning values via ABI
ABICTL_VARIADIC = 0x03  ; Variadic function support
ABICTL_CUSTOM   = 0x04  ; Custom ABI handling
```

### Function Declaration with ABI

To declare a function using a specific ABI:

```
SYM function_name, TYPE_ABICTL=ABICTL_STANDARD=abi_name
```

Example:
```
; Declare a function using the linux_x86_64 ABI
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

### Function Calling with ABI

To call a function with parameters using ABI:

```
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...
```

Example:
```
; Call calculate_sum with two parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value1, value2
```

### Receiving Parameters

Within a function, parameters are accessed using the same ABI control:

```
; Get the first parameter
MOV param1, TYPE_ABICTL=ABICTL_PARAM=abi_name, 0

; Get the second parameter
MOV param2, TYPE_ABICTL=ABICTL_PARAM=abi_name, 1
```

Example:
```
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get parameters
  VAR TYPE_INT32, a
  VAR TYPE_INT32, b
  MOV a, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  MOV b, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
  
  ; ... function body ...
  SCOPEL
```

### Returning Values

To return values from a function:

```
RET TYPE_ABICTL=ABICTL_RET=abi_name, result1, result2, ...
```

Example:
```
; Calculate result
VAR TYPE_INT32, result
ADD result, a, b

; Return result via ABI
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
```

### Receiving Return Values

After calling a function, return values are accessed:

```
; Get the return value
MOV result, TYPE_ABICTL=ABICTL_RET=abi_name
```

For multiple return values:
```
; Get the first return value
MOV result1, TYPE_ABICTL=ABICTL_RET=abi_name, 0

; Get the second return value
MOV result2, TYPE_ABICTL=ABICTL_RET=abi_name, 1
```

## Standard ABIs

COIL provides predefined ABIs for common calling conventions:

### System V AMD64 ABI (Linux/BSD/macOS)
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

### Microsoft x64 ABI (Windows)
```
ABI windows_x64
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, RDI, RSI, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

### ARM AAPCS64
```
ABI arm_aapcs64
  PARAMS X0, X1, X2, X3, X4, X5, X6, X7
  RETS X0
  CALLER X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15
  CALLEE X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30
  SALLIGN 16
  RZONE 0
EXIT
```

### RISC-V
```
ABI riscv_lp64d
  PARAMS a0, a1, a2, a3, a4, a5, a6, a7
  RETS a0, a1
  CALLER ra, t0, t1, t2, a0, a1, a2, a3, a4, a5, a6, a7, t3, t4, t5, t6
  CALLEE sp, s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11
  SALLIGN 16
  RZONE 0
EXIT
```

## Cross-Processor Compatibility

For maximum portability, COIL provides a special platform-independent ABI:

```
ABI platform_default  ; Maps to the default ABI of the current target
  ; No explicit register listing - determined by the processor
EXIT
```

Code using `platform_default` will be compatible with any processor, as the COIL processor will map to the appropriate ABI for the current target.

## Complete Function Example

```
; Define a portable function with platform_default ABI
SYM add_integers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
  SCOPEE
  ; Get parameters
  VAR TYPE_INT, a
  VAR TYPE_INT, b
  MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
  MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
  
  ; Add the values
  VAR TYPE_INT, result
  ADD result, a, b
  
  ; Return the result
  RET TYPE_ABICTL=ABICTL_RET=platform_default, result
  SCOPEL

; Calling the function
SYM main, TYPE_ABICTL=ABICTL_STANDARD=platform_default
  SCOPEE
  VAR TYPE_INT, x, 10
  VAR TYPE_INT, y, 20
  
  ; Call function with parameters
  CALL add_integers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
  
  ; Get return value
  VAR TYPE_INT, sum
  MOV sum, TYPE_ABICTL=ABICTL_RET=platform_default
  
  ; ... use the result ...
  SCOPEL
```

## ABI Interoperability

The ABI system allows interoperability with external code:

### Foreign Function Interface

```
; Define C function prototype for printf
SYM printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Call the C function
VAR TYPE_PTR, format_string, "Value: %d\n"
VAR TYPE_INT32, value, 42
CALL printf, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, format_string, value
```

### Multiple ABIs in One Program

COIL supports using multiple ABIs within the same program:

```
; Call a Windows API function
CALL CreateFileW, TYPE_ABICTL=ABICTL_PARAM=windows_x64, filename, access, share, security, creation, flags, template

; Call a Linux function
CALL open, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, path, flags, mode
```

This allows COIL code to interface with different systems and libraries seamlessly.

## Advanced ABI Features

### Variadic Functions

COIL supports variadic functions through the ABI system:

```
; Define a variadic function
SYM var_func, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get count of arguments (fixed parameter)
  VAR TYPE_INT32, count
  MOV count, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  
  ; Access variadic arguments
  VAR TYPE_INT32, i, 0
  
  SYM next_arg
  CMP i, count
  BR_GE done
  
  ; Get next argument
  VAR TYPE_INT32, arg
  MOV arg, TYPE_ABICTL=ABICTL_VARIADIC=linux_x86_64, i
  
  ; Process argument...
  
  ADD i, i, 1
  BR next_arg
  
  SYM done
  SCOPEL
```

### Multiple Return Values

For ABIs that support multiple return values:

```
; Function returning quotient and remainder
SYM divmod, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get parameters
  VAR TYPE_INT32, dividend
  VAR TYPE_INT32, divisor
  MOV dividend, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  MOV divisor, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
  
  ; Calculate results
  VAR TYPE_INT32, quotient
  VAR TYPE_INT32, remainder
  DIV quotient, dividend, divisor
  MOD remainder, dividend, divisor
  
  ; Return both values
  RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, quotient, remainder
  SCOPEL

; Calling the function and getting both return values
CALL divmod, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value, 10
VAR TYPE_INT32, q
VAR TYPE_INT32, r
MOV q, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 0  ; First return value
MOV r, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 1  ; Second return value
```