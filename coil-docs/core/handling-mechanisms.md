# Error Handling Mechanisms

## Overview

COIL provides several mechanisms for handling errors. These mechanisms allow programs to respond to error conditions in a controlled manner, enabling robust error recovery and graceful degradation.

## Error Handling Approaches

COIL supports three primary error handling mechanisms:

### 1. Trap and Signal

The default behavior for unhandled errors is to generate a processor trap:

```
; Division that might cause division by zero
DIV result, dividend, divisor   ; If divisor is zero, generates arithmetic trap
```

Traps terminate program execution and report the error. This approach is suitable for:
- Unrecoverable errors
- Development/debugging environments
- Situations where program integrity cannot be guaranteed

### 2. Structured Exception Handling

COIL provides a structured exception mechanism:

```
TRY error_handler
    ; Protected code
    DIV result, dividend, divisor
CATCH
    ; Error handling code
    MOV result, 0   ; Default value on error
ENDTRY
```

When an exception occurs within the TRY block, control transfers to the CATCH block. This approach is suitable for:
- Recoverable errors
- Operations that frequently encounter error conditions
- Complex recovery logic

### 3. Error Status Return

Functions can return status codes to indicate errors:

```
; Function that returns status and result
SYM divide_safe, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR #1, TYPE_INT32   ; Dividend
    VAR #2, TYPE_INT32   ; Divisor
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Check for division by zero
    VAR #3, TYPE_INT32   ; Status code
    VAR #4, TYPE_INT32   ; Result
    
    CMP #2, 0
    BR_NE perform_division
    
    ; Division by zero case
    MOV #3, 0x04000001   ; Division by zero error code
    MOV #4, 0            ; Default result
    BR end_function
    
    perform_division:
    DIV #4, #1, #2       ; Perform division
    MOV #3, 0            ; Success status
    
    end_function:
    ; Return status and result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3, #4
    SCOPEL
```

This approach is suitable for:
- API functions
- Operations with complex error conditions
- Situations requiring detailed error information

## Standard Error Recovery Patterns

### 1. Default Values

Provide safe default values when errors occur:

```
; Safely get array element with bounds checking
SYM array_get_safe
    SCOPEE
    VAR #1, TYPE_PTR    ; Array pointer
    VAR #2, TYPE_INT32  ; Index
    VAR #3, TYPE_INT32  ; Default value
    VAR #4, TYPE_INT32  ; Array size
    
    ; Check bounds
    CMP #2, 0
    BR_LT out_of_bounds
    CMP #2, #4
    BR_GE out_of_bounds
    
    ; Valid index
    INDEX #5, #1, #2    ; Get array[index]
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #5
    
    out_of_bounds:
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    SCOPEL
```

### 2. Retry Logic

Implement retry logic for transient errors:

```
SYM operation_with_retry
    SCOPEE
    VAR #1, TYPE_INT32, 0  ; Retry count
    VAR #2, TYPE_INT32, 3  ; Max retries
    
    retry_loop:
    ; Attempt operation
    TRY operation_failed
        ; Operation code
        ...
        BR operation_succeeded
    CATCH
        ; Handle failure
        INC #1
        CMP #1, #2
        BR_LT retry_loop
        
        ; Too many retries
        VAR #3, TYPE_INT32, 0x04070001  ; Max retries exceeded
        RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    ENDTRY
    
    operation_succeeded:
    VAR #4, TYPE_INT32, 0  ; Success
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #4
    SCOPEL
```

### 3. Cleanup Pattern

Ensure resources are properly cleaned up even when errors occur:

```
SYM function_with_cleanup
    SCOPEE
    ; Acquire resources
    CALL allocate_resource, TYPE_ABICTL=ABICTL_PARAM=platform_default
    MOV #1, TYPE_ABICTL=ABICTL_RET=platform_default  ; Resource handle
    
    ; Use resources with error handling
    TRY cleanup
        ; Operation that might fail
        ...
    CATCH
        ; Error occurred, still need to clean up
    ENDTRY
    
    cleanup:
    ; Always clean up resources
    CALL free_resource, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1
    
    SCOPEL
    RET
```

## Error Handler Registration

COIL programs can register custom error handlers:

```
; Global error handler function
SYM custom_error_handler, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    VAR #1, TYPE_INT32  ; Error code
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Handle specific errors
    ...
    
    SCOPEL
    RET

; Register error handler
REGISTER_HANDLER custom_error_handler, 0x04000000, 0xFF000000  ; All runtime errors
```

## Error Propagation

Errors can be propagated across function calls:

```
; Propagate errors up the call stack
SYM inner_function
    SCOPEE
    ; Operation that might fail
    DIV #1, #2, #3
    CMP #3, 0
    BR_NE success
    
    ; Error case
    MOV #4, 0x04000001  ; Division by zero
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #4, 0
    
    success:
    RET TYPE_ABICTL=ABICTL_RET=platform_default, 0, #1
    SCOPEL

SYM outer_function
    SCOPEE
    ; Call function and check for errors
    CALL inner_function, TYPE_ABICTL=ABICTL_PARAM=platform_default, ...
    MOV #1, TYPE_ABICTL=ABICTL_RET=platform_default, 0  ; Status
    MOV #2, TYPE_ABICTL=ABICTL_RET=platform_default, 1  ; Result
    
    CMP #1, 0
    BR_EQ success
    
    ; Propagate error
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #1, 0
    
    success:
    ; Continue processing
    ...
    SCOPEL
```

## Error Logging

COIL provides standard error logging functions:

```
; Log error message
EXTERN log_error, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL log_error, TYPE_ABICTL=ABICTL_PARAM=platform_default, error_code, message_ptr

; Get error message for code
EXTERN get_error_message, TYPE_ABICTL=ABICTL_STANDARD=platform_default
CALL get_error_message, TYPE_ABICTL=ABICTL_PARAM=platform_default, error_code
MOV message_ptr, TYPE_ABICTL=ABICTL_RET=platform_default
```

## Error Testing and Simulation

COIL provides mechanisms to simulate errors for testing:

```
; Simulate division by zero for testing
SIMULATE_ERROR 0x04000001
DIV #1, #2, #3  ; This will trigger the simulated error
END_SIMULATE
```

This allows testing error handling code without relying on actual error conditions.