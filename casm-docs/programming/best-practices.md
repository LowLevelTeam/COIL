# CASM Best Practices

## Coding Style

### Naming Conventions

1. **Variables**: Descriptive, lowercase with underscores
   ```
   VAR #1, TYPE_INT32, 0  ; counter
   VAR #2, TYPE_FP64, 0.0  ; total_sum
   ```

2. **Labels**: Descriptive, lowercase with underscores
   ```
   loop_start:
   function_end:
   error_handler:
   ```

3. **Symbols**: Use camelCase for functions, UPPER_CASE for constants
   ```
   SYM calculateTotal
   SYM MAX_ITERATIONS, TYPE_PARAM0=GLOB
   ```

### Indentation and Formatting

1. **Indentation**: Use consistent spacing (2 or 4 spaces)
   ```
   SCOPEE
     VAR #1, TYPE_INT32, 0
     ADD #1, #1, 1
   SCOPEL
   ```

2. **Alignment**: Align similar elements for readability
   ```
   VAR #1, TYPE_INT32, 0      ; Loop counter
   VAR #2, TYPE_FP64,  3.14   ; PI value
   VAR #3, TYPE_PTR,   buffer ; Buffer pointer
   ```

3. **Spacing**: Add spaces after commas and around operators
   ```
   ADD result, value1, value2  ; Not: ADD result,value1,value2
   ```

### Comments

1. **Header Comments**: Start files with overview comments
   ```
   ; ----------------------------------------------------
   ; File: math_utilities.casm
   ; Description: Common mathematical functions
   ; Author: Team Member
   ; Created: 2025-03-30
   ; ----------------------------------------------------
   ```

2. **Section Comments**: Describe major sections
   ```
   ; ================ VECTOR OPERATIONS ================
   ```

3. **Inline Comments**: Explain non-obvious code
   ```
   XOR #1, #1, #1  ; Zero out the counter (faster than MOV #1, 0)
   ```

4. **TODO Comments**: Mark incomplete sections
   ```
   ; TODO: Optimize this loop for better cache performance
   ```

## Program Structure

### File Organization

1. **Standard Sections**:
   ```
   ; Processor and architecture declaration
   PROC 0x01
   ARCH 0x01, 0x03

   ; Constant definitions
   SECTION .data, 0x02 | 0x04 | 0x08
   
   ; Code section
   SECTION .text, 0x01 | 0x04
   
   ; Uninitialized data
   SECTION .bss, 0x02 | 0x04 | 0x10
   ```

2. **Function Groups**: Group related functions together

3. **Header Files**: Use INCLUDE for common definitions
   ```
   INCLUDE "standard_defs.coilh"
   ```

### Modularization

1. **Single Responsibility**: Each file should have a clear purpose

2. **Separation of Concerns**: Separate logic from data

3. **Library Organization**: Group related functionality in libraries

## Scope Management

### Variable Scoping

1. **Minimize Scope**: Keep variables in the smallest possible scope
   ```
   SCOPEE
     VAR #1, TYPE_INT32, 0  ; Only needed in this scope
     ; Use #1
   SCOPEL  ; #1 is destroyed here
   ```

2. **Resource Management**: Use scopes for automatic cleanup
   ```
   SCOPEE
     VAR #1, TYPE_PTR  ; Resource handle
     ; Acquire and use resource
   SCOPEL  ; Resource automatically released
   ```

3. **Nested Scopes**: Use for logical grouping
   ```
   SCOPEE  ; Function scope
     ; Function setup
     
     SCOPEE  ; Inner processing scope
       ; Processing logic
     SCOPEL
     
     ; Function cleanup
   SCOPEL
   ```

## Performance Optimization

### Register Usage

1. **Minimize Variables**: Reuse variables when possible

2. **Access Patterns**: Place frequently accessed variables first
   ```
   VAR #1, TYPE_INT32, 0  ; Frequently used counter (likely in register)
   VAR #2, TYPE_ARRAY=TYPE_UNT8, buffer  ; Less frequently accessed (likely in memory)
   ```

3. **Register Hints**: Use sparingly for critical code
   ```
   VAR #1, TYPE_INT32, 0, TYPE_RGP=RAX  ; Suggest using RAX for variable #1
   ```

### Memory Access

1. **Locality**: Keep related data together

2. **Alignment**: Respect data alignment for performance
   ```
   ALIGN 16  ; Align data to 16-byte boundary for SIMD operations
   ```

3. **Efficient Access**: Use optimal addressing modes
   ```
   ; Prefer direct addressing when possible
   MOV value, [base_ptr]  ; Instead of calculating address separately
   ```

### Loop Optimization

1. **Loop Invariants**: Move calculations outside loops
   ```
   ; Calculate once before loop
   MUL size, width, height
   
   ; Loop using precalculated size
   SCOPEE
     VAR #1, TYPE_INT32, 0  ; Loop counter
   loop_start:
     CMP #1, size
     BR_GE loop_end
     ; Loop body
     INC #1
     BR loop_start
   loop_end:
   SCOPEL
   ```

2. **Loop Unrolling**: For small, performance-critical loops
   ```
   ; Process 4 elements per iteration
   ADD #1, #1, [array]
   ADD #1, #1, [array + 4]
   ADD #1, #1, [array + 8]
   ADD #1, #1, [array + 12]
   ADD array, array, 16
   ```

3. **Early Exit**: Exit loops as soon as possible
   ```
   ; Find item in array
   SCOPEE
     VAR #1, TYPE_INT32, 0  ; Loop counter
   search_loop:
     CMP #1, length
     BR_GE not_found
     
     INDEX #2, array, #1
     CMP #2, target
     BR_EQ found
     
     INC #1
     BR search_loop
   
   found:
     ; Item found logic
     BR search_end
     
   not_found:
     ; Item not found logic
     
   search_end:
   SCOPEL
   ```

## Error Handling

### Defensive Programming

1. **Input Validation**: Check parameters before use
   ```
   ; Ensure divisor is not zero
   CMP divisor, 0
   BR_EQ handle_division_by_zero
   
   DIV result, dividend, divisor
   ```

2. **Bounds Checking**: Verify array accesses
   ```
   ; Check array bounds
   CMP index, array_length
   BR_GE handle_out_of_bounds
   
   INDEX value, array, index
   ```

3. **Status Codes**: Use consistent error reporting
   ```
   ; Function returns status in #1, result in #2
   CALL process_data, TYPE_ABICTL=ABICTL_PARAM=platform_default, input
   MOV status, TYPE_ABICTL=ABICTL_RET=platform_default, 0
   MOV result, TYPE_ABICTL=ABICTL_RET=platform_default, 1
   
   ; Check status
   CMP status, 0
   BR_NE handle_error
   ```

### Error Recovery

1. **Cleanup**: Ensure resources are freed on error
   ```
   SCOPEE
     VAR #1, TYPE_PTR  ; Resource
     
     ; Try operation
     CMP status, 0
     BR_NE error_cleanup
     
     ; Normal processing
     BR end_process
     
   error_cleanup:
     ; Error-specific cleanup
     
   end_process:
   SCOPEL  ; Automatic cleanup
   ```

2. **Graceful Degradation**: Continue with reduced functionality
   ```
   ; Try preferred method
   CALL optimal_function, TYPE_ABICTL=ABICTL_PARAM=platform_default, data
   MOV status, TYPE_ABICTL=ABICTL_RET=platform_default
   
   ; Fall back to alternative if needed
   CMP status, 0
   BR_EQ processing_succeeded
   
   CALL fallback_function, TYPE_ABICTL=ABICTL_PARAM=platform_default, data
   ```

## Portability

### Platform Independence

1. **Architecture Abstraction**: Use platform-independent types
   ```
   VAR #1, TYPE_INT, 0  ; Instead of TYPE_INT32 (adapts to platform)
   VAR #2, TYPE_PTR, address  ; Instead of fixed-size address
   ```

2. **Conditional Assembly**: Use for platform-specific code
   ```
   IF ARCH == 0x01
     ; x86-specific optimized code
   ELSE
     ; Generic implementation
   ENDIF
   ```

3. **ABI Abstraction**: Use platform_default ABI
   ```
   CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, param1, param2
   ```

### Future Compatibility

1. **Version Checking**: Verify COIL version support
   ```
   VERSION 1, 0, 0  ; Requires COIL v1.0.0 or later
   ```

2. **Feature Isolation**: Isolate version-specific features
   ```
   IF VERSION >= 0x010100  ; COIL v1.1.0 or later
     ; Use v1.1 features
   ELSE
     ; Compatible alternative
   ENDIF
   ```

## Testing and Debugging

### Testability

1. **Modular Design**: Create testable units
   ```
   SYM test_function, TYPE_PARAM0=GLOB  ; Exposed for testing
   ```

2. **Deterministic Behavior**: Avoid dependencies on external state
   ```
   ; Pass all needed data explicitly
   CALL process, TYPE_ABICTL=ABICTL_PARAM=platform_default, input, state
   ```

3. **Error Injection**: Support testing error paths
   ```
   IF DEBUG
     ; Allow artificially triggering error conditions
     CMP test_error_flag, 1
     BR_EQ simulate_error
   ENDIF
   ```

### Debugging Aids

1. **Debug Output**: Add conditional debug code
   ```
   IF DEBUG
     CALL debug_print, TYPE_ABICTL=ABICTL_PARAM=platform_default, message, value
   ENDIF
   ```

2. **State Validation**: Add assertions
   ```
   IF DEBUG
     ; Assert counter is positive
     CMP counter, 0
     BR_GE assert_passed
     CALL assert_failed, TYPE_ABICTL=ABICTL_PARAM=platform_default, assertion_msg
   assert_passed:
   ENDIF
   ```

3. **Jump Tables**: Use for easier debugging
   ```
   ; Jump table for state machine
   BR_EQ state_idle
   BR_EQ state_running
   BR_EQ state_paused
   BR_EQ state_error
   
   state_idle:
   ; ...
   ```