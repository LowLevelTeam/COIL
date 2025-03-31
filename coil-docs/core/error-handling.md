# COIL Error Handling

## Overview

This document defines the standardized error handling mechanisms in COIL. A consistent error model is essential for reliable, portable programs.

## Error Categories

COIL defines several categories of errors:

### 1. Compilation Errors
Errors detected during CASM assembly or COIL binary generation.

### 2. Linking Errors
Errors detected during the linking phase when combining multiple COIL objects.

### 3. Validation Errors
Errors detected during validation of a COIL binary before execution.

### 4. Runtime Errors
Errors that occur during program execution.

## Error Reporting

### Error Codes

Each error is identified by a 32-bit error code:

```
[Category (8 bits)] [Subcategory (8 bits)] [Specific Error (16 bits)]
```

Example error codes:
- `0x01000001`: Compilation - Syntax - Invalid token
- `0x02000001`: Linking - Symbol resolution - Undefined symbol
- `0x03000001`: Validation - Type - Type mismatch
- `0x04000001`: Runtime - Arithmetic - Division by zero

### Error Information Structure

```
struct ErrorInfo {
    uint32_t error_code;      // Error code
    uint32_t location;        // Location (file offset or address)
    uint16_t symbol_index;    // Related symbol (if applicable)
    uint16_t message_length;  // Length of error message
    char     message[];       // Error message text
}
```

## Runtime Error Handling

### Error Detection Points

COIL processors must detect these runtime errors:
1. Division by zero
2. Invalid memory access
3. Type incompatibility
4. Stack overflow/underflow
5. Invalid instruction

### Error Handling Mechanisms

1. **Trap and Signal**: Default behavior - generate a processor trap
2. **Error Handler Function**: Customizable handler specified at program start
3. **Status Return**: For functions that can fail, standard status return patterns

### Exception Model

COIL v1 uses a simple exception model:

```
TRY dest_on_error
    ; Protected code
    ; If error occurs, control transfers to dest_on_error
CATCH
    ; Error handling code
ENDTRY
```

## Implementation Requirements

A compliant COIL processor must:

1. Detect all specified error conditions
2. Provide clear error information
3. Support the standard exception model
4. Never silently ignore errors
5. Ensure deterministic error behavior

## Common Error Patterns

### Function Status Return

For functions that can fail, a common pattern is:

```
; Function that returns status code and result
SYM function_with_status, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Function body...
    
    ; Return status code and result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, status_code, result
    SCOPEL

; Calling code
CALL function_with_status, TYPE_ABICTL=ABICTL_PARAM=platform_default, param1, param2
MOV status, TYPE_ABICTL=ABICTL_RET=platform_default, 0
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default, 1

; Check status
CMP status, 0
BR_NE handle_error
```

### Protected Operations

For operations that might fail:

```
; Protect against division by zero
CMP divisor, 0
BR_EQ handle_division_by_zero

DIV result, dividend, divisor
```

## Version-Specific Behavior

- **COIL v1**: Basic error detection and handling
- **COIL v2**: Enhanced error reporting, standard library error functions
- **COIL v3**: Comprehensive cross-device error management

## Error Categories Reference

### Compilation Errors (0x01xxxxxx)

| Code | Description |
|------|-------------|
| 0x01000001 | Syntax error |
| 0x01000002 | Undefined symbol |
| 0x01000003 | Type error |
| 0x01000004 | Invalid directive |
| 0x01010001 | Invalid scope nesting |
| 0x01010002 | Variable redefinition |
| 0x01020001 | Invalid operand |
| 0x01020002 | Wrong operand count |

### Linking Errors (0x02xxxxxx)

| Code | Description |
|------|-------------|
| 0x02000001 | Undefined external symbol |
| 0x02000002 | Symbol redefinition |
| 0x02000003 | Incompatible symbol type |
| 0x02010001 | Section alignment error |
| 0x02010002 | Invalid relocation |

### Validation Errors (0x03xxxxxx)

| Code | Description |
|------|-------------|
| 0x03000001 | Type mismatch |
| 0x03000002 | Invalid instruction |
| 0x03000003 | Invalid processor type |
| 0x03010001 | Invalid memory access |
| 0x03010002 | Alignment error |

### Runtime Errors (0x04xxxxxx)

| Code | Description |
|------|-------------|
| 0x04000001 | Division by zero |
| 0x04000002 | Arithmetic overflow |
| 0x04000003 | Invalid memory access |
| 0x04010001 | Stack overflow |
| 0x04010002 | Stack underflow |
| 0x04020001 | Invalid function call |
| 0x04020002 | Invalid return |