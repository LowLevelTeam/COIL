# COIL ABI System

## Purpose

This document defines the Application Binary Interface (ABI) system for COIL, providing a processor-independent mechanism for function calls, parameter passing, and return value handling. The ABI system eliminates the need for manual register management while maintaining native performance.

## Key Concepts

- **ABI Definition**: How ABIs are specified in COIL
- **Parameter Passing**: How function arguments are handled
- **Return Values**: How function results are returned
- **Register Management**: How registers are preserved across calls
- **Platform Independence**: How ABIs enable cross-platform support

## What Is an ABI?

An ABI (Application Binary Interface) defines the conventions for:
- How parameters are passed to functions
- How return values are handled
- Which registers must be preserved by callers vs. callees
- Stack alignment requirements
- Other calling convention details

Traditional assembly languages require programmers to manually implement these conventions. COIL's ABI system automates this process while maintaining native performance.

## ABI Definition

### Definition Syntax

An ABI definition in CASM follows this format:

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

The ABI definition has special binary encoding:

```
0xBA                  ; Opcode for ABI
0x01                  ; One operand
0x9100                ; TYPE_SYM for ABI name
[sym_id]              ; Symbol ID for ABI name

; ABI-specific instructions follow until EXIT
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

### Parameter Passing

To call a function with parameters using an ABI:

```
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...
```

Example:
```
; Call calculate_sum with two parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value1, value2
```

### Parameter Reception

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

### Return Value Handling

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

### Return Value Reception

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

## Platform Independence

For maximum portability, COIL provides a special platform-independent ABI:

```
ABI platform_default  ; Maps to the default ABI of the current target
  ; No explicit register listing - determined by the processor
EXIT
```

Code using `platform_default` will be compatible with any processor, as the COIL processor will map to the appropriate ABI for the current target.

## Standard ABIs

COIL includes definitions for standard platform ABIs:

### x86-64 System V ABI (Linux/macOS/BSD)

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

### x64 Microsoft ABI (Windows)

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

## Variadic Function Support

COIL supports variadic functions through the ABI system:

```
; Access variadic arguments
VAR TYPE_INT32, arg
MOV arg, TYPE_ABICTL=ABICTL_VARIADIC=abi_name, index
```

## Register Management

The ABI system automatically handles:

1. **Parameter Registers**: Maps function parameters to appropriate registers or stack locations
2. **Return Registers**: Maps function return values to appropriate registers or memory
3. **Register Preservation**: Automatically saves and restores caller-saved or callee-saved registers
4. **Stack Maintenance**: Manages stack space and alignment according to ABI requirements

## Implementation Details

When a COIL processor encounters ABI operations, it:

1. **For function calls**:
   - Places parameters in the appropriate registers or stack locations
   - Saves caller-saved registers as needed
   - Performs the call
   - Restores caller-saved registers
   - Makes return values available through the ABI control

2. **For function entries**:
   - Makes parameters available through the ABI control
   - Saves callee-saved registers as needed

3. **For function returns**:
   - Places return values in the appropriate registers or memory locations
   - Restores callee-saved registers
   - Returns to the caller

## Related Documentation

- [Function Examples](../../examples/functions.md) - Examples of ABI usage in functions
- [CASM Directives](../assembly/directives.md) - Documentation on the ABI directive
- [Control Flow Instructions](../instruction-set/control-flow.md) - Instructions for function calls and returns
- [Variable System](variable-system.md) - How variables integrate with the ABI system