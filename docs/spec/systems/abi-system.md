# COIL ABI System

## Purpose

This document defines the Application Binary Interface (ABI) system for COIL, providing a processor-independent mechanism for function calls, parameter passing, and return value handling. The ABI system eliminates the need for manual register management while maintaining native performance.

## What Is an ABI?

An ABI (Application Binary Interface) defines the conventions for:
- How parameters are passed to functions (which registers or stack locations)
- How return values are handled (which registers or memory locations)
- Which registers must be preserved by callers vs. callees
- Stack alignment requirements and calling convention details

Traditional assembly languages require programmers to manually implement these conventions. COIL's ABI system automates this process while maintaining native performance.

## ABI Definition

### Defining an ABI

An ABI is defined using the `ABI` directive followed by ABI-specific instructions:

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

### Standard ABIs

COIL includes definitions for standard platform ABIs:

#### x86-64 System V ABI (Linux/macOS/BSD)

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

#### x64 Microsoft ABI (Windows)

```
ABI windows_x64
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RBP, RDI, RSI, RSP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

### Platform-Independent ABI

For maximum portability, COIL provides a special platform-independent ABI:

```
ABI platform_default  ; Maps to the default ABI of the current target
  ; No explicit register listing - determined by the processor
EXIT
```

Code using `platform_default` will be compatible with any processor, as the COIL processor will map to the appropriate ABI for the current target.

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

### Complete Function Example

```
; Define a function with a specific ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Calculate result
    VAR TYPE_INT32, result
    ADD result, a, b
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL

; Call the function and use its result
SYM main
    SCOPEE
    VAR TYPE_INT32, x, 10
    VAR TYPE_INT32, y, 20
    VAR TYPE_INT32, sum
    
    ; Call function with parameters
    CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
    
    ; Get return value
    MOV sum, TYPE_ABICTL=ABICTL_RET=platform_default
    SCOPEL
```

## Parameter Handling

### Passing Parameters

To call a function with parameters using an ABI:

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
; Get parameters from the platform default ABI
MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
```

## Return Value Handling

### Returning Values

To return values from a function:

```
RET TYPE_ABICTL=ABICTL_RET=abi_name, result1, result2, ...
```

Example:
```
; Return a single value via ABI
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result

; Return multiple values
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, quotient, remainder
```

### Receiving Return Values

After calling a function, return values are accessed:

```
; Get the single return value
MOV result, TYPE_ABICTL=ABICTL_RET=abi_name

; Get multiple return values
MOV result1, TYPE_ABICTL=ABICTL_RET=abi_name, 0
MOV result2, TYPE_ABICTL=ABICTL_RET=abi_name, 1
```

Example:
```
; Call function that returns multiple values
CALL divmod, TYPE_ABICTL=ABICTL_PARAM=platform_default, num, div

; Get both return values
MOV quotient, TYPE_ABICTL=ABICTL_RET=platform_default, 0
MOV remainder, TYPE_ABICTL=ABICTL_RET=platform_default, 1
```

## Advanced ABI Features

### Variadic Functions

COIL supports variadic functions (functions with variable argument counts):

```
; Access variadic arguments
VAR TYPE_INT32, arg
MOV arg, TYPE_ABICTL=ABICTL_VARIADIC=abi_name, index
```

Example:
```
; Print function with format string and variable arguments
SYM printf, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get format string
    VAR TYPE_PTR, format
    MOV format, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Get variadic arguments
    VAR TYPE_INT32, arg1
    MOV arg1, TYPE_ABICTL=ABICTL_VARIADIC=platform_default, 0
    
    ; ... process arguments ...
    SCOPEL
```

## Implementation Details

The ABI system automatically handles:

1. **Parameter Passing**: 
   - Maps function parameters to appropriate registers or stack locations
   - Follows the specific ABI conventions for parameter order and location
   - Handles overflow parameters to stack when registers are exhausted

2. **Return Value Handling**:
   - Maps function return values to appropriate registers or memory
   - Handles multiple return values according to ABI rules
   - Ensures consistent access to return values for the caller

3. **Register Preservation**:
   - Automatically saves and restores caller-saved registers as needed
   - Ensures callee-saved registers are preserved across function calls
   - Manages the stack frame according to ABI requirements

4. **Stack Management**:
   - Maintains proper stack alignment as required by the ABI
   - Allocates stack space for local variables and spilled registers
   - Cleans up stack resources on function return

## Related Documentation

- [Function Examples](../../examples/functions.md) - Examples of ABI usage in functions
- [CASM Directives](../assembly/directives.md) - Documentation on the ABI directive
- [Control Flow Instructions](../instruction-set/control-flow.md) - Instructions for function calls and returns
- [Variable System](variable-system.md) - How variables integrate with the ABI system