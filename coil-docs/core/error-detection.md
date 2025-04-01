# Error Detection

## Overview

Error detection is the process of identifying error conditions throughout the COIL lifecycle. Proper error detection is essential for creating reliable software and providing meaningful diagnostics for debugging.

## Detection Points

COIL processors must implement detection for all defined error conditions at specific points in the toolchain.

### Compilation Phase Detection

The CASM assembler detects errors during translation from CASM to COIL:

1. **Lexical Analysis**:
   - Invalid tokens
   - Unexpected characters
   - Invalid escape sequences

2. **Parsing**:
   - Syntax errors
   - Unexpected structures
   - Mismatched delimiters

3. **Semantic Analysis**:
   - Type mismatches
   - Undefined symbols
   - Invalid operations
   - Scope violations

4. **Code Generation**:
   - Invalid instructions
   - Unsupported features
   - Resource limitations

Example assembler error:
```
error:compilation:matrix.casm:157:23: 0x01030002 - Variable #7 used before declaration
```

### Linking Phase Detection

The COIL linker detects errors during the linking process:

1. **Symbol Resolution**:
   - Undefined symbols
   - Multiply defined symbols
   - Incompatible symbol types

2. **Section Merging**:
   - Incompatible sections
   - Alignment issues
   - Size limitations

3. **Relocation Processing**:
   - Invalid relocations
   - Out-of-range references
   - Mismatched relocation types

4. **Output Generation**:
   - Format errors
   - Size limitations
   - Compatibility issues

Example linker error:
```
error:linking:0x02000001 - Undefined external symbol 'calculate_checksum'
```

### Validation Phase Detection

The COIL validator detects errors during pre-execution validation:

1. **Type System**:
   - Type compatibility
   - Conversion validity
   - Type constraints

2. **Memory Access**:
   - Alignment requirements
   - Permission checks
   - Address range validation

3. **Instruction Set**:
   - Valid opcodes
   - Operand counts
   - Operand types

4. **ABI Compliance**:
   - Parameter passing conventions
   - Return value handling
   - Register usage

Example validation error:
```
error:validation:0x03000003 - Instruction 'VDOT' requires vector type, found 'TYPE_INT32'
```

### Runtime Phase Detection

The COIL processor detects errors during program execution:

1. **Arithmetic Operations**:
   - Division by zero
   - Overflow/underflow
   - Invalid floating-point operations

2. **Memory Operations**:
   - Null pointer dereference
   - Out-of-bounds access
   - Misaligned access

3. **Control Flow**:
   - Invalid branch targets
   - Stack imbalance
   - Infinite loops (in some cases)

4. **Function Calls**:
   - Parameter mismatches
   - Invalid function pointers
   - Stack overflow

Example runtime error:
```
error:runtime:0x04000001 - Division by zero at address 0x00401230
```

## Implementation Requirements

A compliant COIL implementation must:

1. **Detect All Specified Errors**: Identify all error conditions defined in the error classification.

2. **Provide Specific Error Codes**: Use standardized error codes for each detected condition.

3. **Include Context Information**: Provide location and context in error reports.

4. **Follow Severity Guidelines**: Distinguish between errors, warnings, and notes.

5. **Enable Error Recovery**: Where possible, allow recovery from non-fatal errors.

## Required Detection Checks

### Critical Checks

These checks must be implemented by all COIL processors:

1. **Division by zero**: Check divisor before division operations
2. **Null pointer dereference**: Validate pointer before dereferencing
3. **Out-of-bounds access**: Verify array/memory accesses within bounds
4. **Type compatibility**: Ensure operand types match operation requirements
5. **Stack overflow/underflow**: Monitor stack usage against limits

### Additional Checks

These checks should be implemented when possible:

1. **Integer overflow/underflow**: Detect arithmetic operations that exceed type ranges
2. **Uninitialized variables**: Detect use of variables before initialization
3. **Memory leaks**: Track memory allocations and deallocations
4. **Infinite loops**: Detect certain cases of non-terminating loops
5. **Dead code**: Identify unreachable code sections

## Diagnostic Modes

COIL processors support different diagnostic modes:

1. **Production Mode**: Detect only critical runtime errors
2. **Debug Mode**: Detect all errors with detailed reporting
3. **Strict Mode**: Treat warnings as errors
4. **Performance Mode**: Minimal error checking for maximum performance

These modes can be selected through compiler/processor options.

## Error Detection Example

Example of division with error detection:

```
; Division that might cause division by zero
CMP #2, 0                 ; Check divisor
BR_EQ division_by_zero    ; Branch if zero

DIV #3, #1, #2            ; Safe division
BR division_end

division_by_zero:
; Handle error
MOV #4, 0x04000001        ; Set error code (division by zero)
MOV #3, 0                 ; Set default result

division_end:
```

This pattern explicitly checks for the error condition before performing the operation.