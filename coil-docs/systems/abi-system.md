# COIL ABI System

## Purpose

The Application Binary Interface (ABI) system provides a processor-independent mechanism for function calls, parameter passing, and return value handling while minimizing the need for explicit ABI management.

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

## ABI Context Detection

### Automatic ABI Selection

COIL provides automatic ABI selection based on symbols:

1. **Symbol-Based ABI Detection**: When a symbol with an associated ABI is used, that ABI is automatically applied
2. **Contextual ABI Inference**: The compiler detects the execution context and applies the appropriate ABI

Example:
```
; Define function with explicit ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Call function with automatic ABI detection
CALL add_numbers, a, b  ; ABI is automatically inferred from the symbol
```

### ABI Type Propagation

ABI types propagate through context:
- Function calls inherit the ABI of the target function
- Return statements inherit the ABI of their containing function
- Symbols can specify their required ABI which is then applied to operations using that symbol

## Using ABIs

### ABI Control Type

ABI operations use the `TYPE_ABICTL` (0xF8) type with these control values:

```
ABICTL_STANDARD = 0x00  ; Use specified ABI convention
ABICTL_PARAM    = 0x01  ; Passing parameters via ABI
ABICTL_RET      = 0x02  ; Returning values via ABI
ABICTL_SWITCH   = 0x03  ; Context switching via ABI
ABICTL_AUTO     = 0x04  ; Automatic ABI detection
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

## Context Switching

COIL provides ABI-aware context switching:

```
; Save current execution context according to ABI rules
SAVE_CONTEXT ctx_var, TYPE_ABICTL=ABICTL_SWITCH=current_abi

; Restore context according to ABI rules
RESTORE_CONTEXT ctx_var, TYPE_ABICTL=ABICTL_SWITCH=current_abi
```

This ensures that register preservation follows the specified ABI rules.

## Function Example

```
; Define a function with a specific ABI
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters (explicit ABI specification)
    VAR #1, TYPE_INT32
    VAR #2, TYPE_INT32
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0  ; First parameter
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1  ; Second parameter
    
    ; Alternative syntax with automatic ABI detection
    VAR #3, TYPE_INT32
    VAR #4, TYPE_INT32
    GET_PARAM #3, 0  ; First parameter with automatic ABI detection
    GET_PARAM #4, 1  ; Second parameter with automatic ABI detection
    
    ; Calculate result
    VAR #5, TYPE_INT32
    ADD #5, #1, #2
    
    ; Return result (explicit ABI specification)
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #5
    
    ; Alternative return with automatic ABI detection
    RET_AUTO #5  ; Return with automatic ABI detection
    SCOPEL
```

## Parameter Passing

### Passing Parameters

To call a function with parameters:

```
; Explicit ABI specification
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...

; Automatic ABI detection
CALL function_name, param1, param2, ...  ; ABI inferred from function_name
```

Example:
```
; Call calculate_sum with two parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2

; Equivalent call with automatic ABI detection
CALL calculate_sum, #1, #2  ; ABI is automatically inferred
```

### Receiving Parameters

Within a function, parameters are accessed:

```
; Explicit ABI specification
MOV #1, TYPE_ABICTL=ABICTL_PARAM=abi_name, 0

; Automatic ABI detection
GET_PARAM #1, 0  ; Get first parameter using the function's ABI
```

## Return Value Handling

### Returning Values

To return values from a function:

```
; Explicit ABI specification
RET TYPE_ABICTL=ABICTL_RET=abi_name, result1, result2, ...

; Automatic ABI detection
RET_AUTO result1, result2, ...  ; Uses the function's defined ABI
```

Example:
```
; Return a single value with explicit ABI
RET TYPE_ABICTL=ABICTL_RET=platform_default, #1

; Return a single value with automatic ABI detection
RET_AUTO #1  ; Uses the function's defined ABI
```

### Receiving Return Values

After calling a function, return values are accessed:

```
; Explicit ABI specification
MOV #1, TYPE_ABICTL=ABICTL_RET=abi_name

; Get multiple return values with explicit ABI
MOV #1, TYPE_ABICTL=ABICTL_RET=abi_name, 0
MOV #2, TYPE_ABICTL=ABICTL_RET=abi_name, 1

; Automatic ABI detection
GET_RESULT #1  ; Get first return value using the callee's ABI
GET_RESULT #2, 1  ; Get second return value using the callee's ABI
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