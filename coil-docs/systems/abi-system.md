# COIL ABI System

## Introduction

The Application Binary Interface (ABI) system provides a processor-independent mechanism for function calls, parameter passing, and return value handling while maintaining native performance.

## What Is an ABI?

An ABI defines conventions for:
- Parameter passing (which registers or stack locations)
- Return value handling (which registers or memory)
- Register preservation responsibilities
- Stack alignment requirements 
- Calling convention details

COIL's ABI system automates these details, eliminating manual implementation of platform-specific calling conventions.

## ABI Definition

An ABI is defined using the `ABI` directive:

```
ABI abi_name
  PARAMS register_list    ; Parameter passing registers
  RETS register_list      ; Return value registers
  CALLER register_list    ; Caller-saved registers
  CALLEE register_list    ; Callee-saved registers
  SALLIGN alignment_value ; Stack alignment
  RZONE size_value        ; Red zone size
EXIT
```

### Example ABI Definition

```
ABI x86_64_sysv
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

### Platform-Independent ABI

COIL provides a special platform-independent ABI:

```
ABI platform_default  ; Maps to the default ABI of the current target
```

Code using `platform_default` will be compatible with any processor.

## Using ABIs

### Function Declaration with ABI

To declare a function using a specific ABI:

```
SYM function_name, TYPE_ABICTL=ABICTL_STANDARD=abi_name
```

Example:
```
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
```

### Calling Functions with Parameters

```
; Explicit ABI specification
CALL function, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2

; Automatic ABI detection (inferred from function)
CALL function, param1, param2
```

Example:
```
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2
```

### Receiving Parameters in Functions

```
; Get parameters with explicit ABI
MOV #1, TYPE_ABICTL=ABICTL_PARAM=abi_name, 0  ; First parameter
MOV #2, TYPE_ABICTL=ABICTL_PARAM=abi_name, 1  ; Second parameter
```

### Returning Values from Functions

```
; Return values with explicit ABI
RET TYPE_ABICTL=ABICTL_RET=abi_name, result1, result2

; Return with automatic ABI detection
RET_AUTO result
```

Example:
```
RET TYPE_ABICTL=ABICTL_RET=platform_default, #1
```

### Receiving Return Values

```
; Get return value with explicit ABI
CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, params
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default

; Get multiple return values
MOV result1, TYPE_ABICTL=ABICTL_RET=platform_default, 0
MOV result2, TYPE_ABICTL=ABICTL_RET=platform_default, 1
```

## Standard ABI Definitions

### x86-64 System V ABI

Used on Linux, macOS, and other Unix-like systems on x86-64:

```
ABI x86_64_sysv
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

### x86-64 Windows ABI

Used on Windows x86-64:

```
ABI x86_64_windows
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, RDI, RSI, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
  SHADOW 32
EXIT
```

### ARM64 ABI

Used on ARM64 platforms:

```
ABI arm64
  PARAMS X0, X1, X2, X3, X4, X5, X6, X7
  RETS X0, X1
  CALLER X0-X15
  CALLEE X19-X28, X29, X30
  SALLIGN 16
  RZONE 0
EXIT
```

## Parameter Passing Rules

### Integer Parameters

1. Assigned to registers in declaration order
2. When registers are exhausted, parameters are placed on stack
3. Parameters larger than register size may use multiple registers or stack

### Floating-Point Parameters

1. Use architecture-specific floating-point registers where available
2. Follow platform conventions for mixed integer/floating-point parameters

### Compound Parameters

1. Small structures may be passed in registers according to their fields
2. Larger structures are passed by reference or on stack

## Complete Function Example

```
; Define a function that adds two numbers
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

; Call the function
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

## ABI Implementation Details

The ABI system automatically handles:

1. **Parameter Passing**: 
   - Maps parameters to registers or stack
   - Follows ABI-specific parameter order
   - Handles overflow parameters to stack

2. **Return Value Handling**:
   - Maps return values to registers or memory
   - Handles multiple return values
   - Ensures consistent access for callers

3. **Register Preservation**:
   - Saves/restores caller-saved registers
   - Ensures callee-saved registers are preserved
   - Manages the stack frame according to ABI