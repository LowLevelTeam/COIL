# COIL ABI System

## Purpose

The Application Binary Interface (ABI) system provides a processor-independent mechanism for function calls, parameter passing, and return value handling.

## What Is an ABI?

An ABI defines the conventions for:
- How parameters are passed to functions (which registers or stack locations)
- How return values are handled (which registers or memory locations)
- Which registers must be preserved by callers vs. callees
- Stack alignment requirements and calling convention details

COIL's ABI system automates these details, eliminating the need to manually implement calling conventions while maintaining native performance.

## ABI Definition

### Defining an ABI

An ABI is defined using the `ABI` directive:

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

### Platform-Independent ABI

For maximum portability, COIL provides a special platform-independent ABI:

```
ABI platform_default  ; Maps to the default ABI of the current target
```

Code using `platform_default` will be compatible with any processor.

## Using ABIs

### ABI Control Type

ABI operations use the `TYPE_ABICTL` (0xF8) type with these control values:

```
ABICTL_STANDARD = 0x00  ; Use specified ABI convention
ABICTL_PARAM    = 0x01  ; Passing parameters via ABI
ABICTL_RET      = 0x02  ; Returning values via ABI
```

### Function Declaration with ABI

To declare a function using a specific ABI:

```
SYM function_name, TYPE_ABICTL=ABICTL_STANDARD=abi_name
```

Example:
```
; Declare a function using the platform default ABI
SYM calculate_sum, TYPE_ABICTL=ABICTL_STANDARD=platform_default
```

## Function Example

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

## Parameter Passing

### Passing Parameters

To call a function with parameters:

```
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...
```

Example:
```
; Call calculate_sum with two parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
```

### Receiving Parameters

Within a function, parameters are accessed:

```
; Get the first parameter
MOV param1, TYPE_ABICTL=ABICTL_PARAM=abi_name, 0

; Get the second parameter
MOV param2, TYPE_ABICTL=ABICTL_PARAM=abi_name, 1
```

## Return Value Handling

### Returning Values

To return values from a function:

```
RET TYPE_ABICTL=ABICTL_RET=abi_name, result1, result2, ...
```

Example:
```
; Return a single value
RET TYPE_ABICTL=ABICTL_RET=platform_default, result

; Return multiple values
RET TYPE_ABICTL=ABICTL_RET=platform_default, quotient, remainder
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

## Implementation Details

The ABI system automatically handles:

1. **Parameter Passing**: 
   - Maps parameters to registers or stack locations
   - Follows ABI conventions for parameter order
   - Handles overflow parameters to stack

2. **Return Value Handling**:
   - Maps return values to registers or memory
   - Handles multiple return values
   - Ensures consistent access for the caller

3. **Register Preservation**:
   - Saves and restores caller-saved registers
   - Ensures callee-saved registers are preserved
   - Manages the stack frame according to ABI