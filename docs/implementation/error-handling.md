# COIL Error Handling

## Purpose

This document defines the error handling mechanisms and requirements for COIL implementations. It establishes how errors are detected, reported, and managed within COIL processors and tools to ensure robust and secure operation.

## Key Concepts

- **Error Categories**: Major classifications of errors
- **Error Severity Levels**: How errors are prioritized
- **Error Reporting Interface**: How errors are communicated
- **Error Recovery**: How implementations handle errors
- **Error Codes**: Standardized error identifiers

## Error Categories

COIL errors are organized into categories, each with a distinct range of error codes:

1. **Format Errors (1xxx)**: Errors in COIL binary format
2. **Instruction Errors (2xxx)**: Errors in instruction parsing or execution
3. **Type System Errors (3xxx)**: Type-related errors
4. **Memory Errors (4xxx)**: Memory access or allocation errors
5. **Symbol Errors (5xxx)**: Symbol resolution or definition errors
6. **Architecture Errors (6xxx)**: Architecture-specific errors
7. **ABI Errors (7xxx)**: ABI-related errors
8. **Implementation Errors (8xxx)**: Limitations of specific implementations

## Error Severity Levels

COIL defines three severity levels for errors:

1. **Fatal Errors**: Prevent continued processing, require immediate termination
2. **Non-Fatal Errors**: Allow continued processing with limitations
3. **Warnings**: Potential issues that don't prevent execution

## Error Reporting Interface

COIL processors must implement an error reporting interface that provides:

1. **Error Code**: Numeric identifier for the error
2. **Severity Level**: Fatal, non-fatal, or warning
3. **Message**: Human-readable description of the error
4. **Location**: Where the error occurred (file, offset, line number)
5. **Context**: Additional information about the error context

## Error Handling Phases

### Parsing Phase

During parsing of COIL files, errors may include:
- Invalid magic number or version
- Corrupted headers
- Invalid section structures
- Malformed instructions

### Validation Phase

During validation, errors may include:
- Type compatibility issues
- Invalid operand combinations
- Undefined symbols
- Unsupported features

### Execution Phase

During execution, errors may include:
- Memory access violations
- Division by zero
- Stack overflow/underflow
- Unimplemented instructions

## Recovery Strategies

### Parser Recovery

When a parsing error occurs:
1. Skip to the next instruction boundary if possible
2. Continue parsing to find additional errors
3. Abandon parsing only if header is invalid or corrupted

### Validator Recovery

When a validation error occurs:
1. Mark the affected instructions as invalid
2. Continue validating other instructions
3. Generate a complete validation report

### Execution Recovery

When an execution error occurs:
1. Handle exceptions according to the defined behavior
2. Continue execution if the error is non-fatal
3. Terminate gracefully if the error is fatal

## Standard Error Codes

### Format Error Codes (1xxx)
```
1001 - Invalid magic number
1002 - Unsupported version
1003 - Corrupted header
1004 - Invalid section table
1005 - Invalid symbol table
1006 - Invalid relocation table
1007 - Section overlap
1008 - Invalid alignment
1009 - Missing required section
```

### Instruction Error Codes (2xxx)
```
2001 - Invalid opcode
2002 - Invalid operand count
2003 - Invalid operand type
2004 - Type mismatch
2005 - Missing operand
2006 - Extra operand
2007 - Invalid instruction format
2008 - Reserved instruction used
2009 - Unsupported instruction
```

### Type System Error Codes (3xxx)
```
3001 - Invalid type encoding
3002 - Incompatible types
3003 - Invalid type conversion
3004 - Type size mismatch
3005 - Invalid type extension
3006 - Type not supported on target
3007 - Invalid structure definition
3008 - Invalid array definition
```

### Memory Error Codes (4xxx)
```
4001 - Invalid memory address
4002 - Misaligned memory access
4003 - Protected memory access
4004 - Stack overflow
4005 - Stack underflow
4006 - Invalid memory operation size
4007 - Memory allocation failure
4008 - Invalid scope operation
```

### Symbol Error Codes (5xxx)
```
5001 - Undefined symbol
5002 - Duplicate symbol definition
5003 - Symbol scope violation
5004 - Invalid symbol type
5005 - Mismatched symbol usage
5006 - Unresolved external symbol
5007 - Symbol visibility error
```

## Error Message Format

Error messages should follow a consistent format:

```
ERROR [E1001]: Invalid magic number
  in file: example.coil
  at offset: 0x00000000
  expected: "COIL"
  found: "CILO"
```

A well-formed error message includes:
1. Error code and brief description
2. Location information (file, offset, line number if available)
3. Contextual information (expected vs. actual values)
4. Any relevant hints for resolution

## Error Collection

COIL processors should:
1. Collect and report multiple errors where possible
2. Avoid cascading error reports from a single root cause
3. Prioritize fundamental errors over derived issues
4. Group related errors for clarity

## Error Handling Requirements

### For COIL Processors

A compliant COIL processor must:

1. **Validate Thoroughly**:
   - Validate COIL binary format before execution
   - Check all instruction operands for type compatibility
   - Verify memory access constraints

2. **Report Clearly**:
   - Provide detailed, actionable error messages
   - Include location information where possible
   - Distinguish between errors and warnings

3. **Handle Gracefully**:
   - Terminate gracefully on fatal errors
   - Continue processing after non-fatal errors where possible
   - Avoid resource leaks during error handling

4. **Preserve Security**:
   - Prevent security vulnerabilities during error handling
   - Avoid revealing sensitive implementation details
   - Maintain memory safety despite errors

### For COIL Tools

COIL development tools should:

1. **Provide Diagnostics**:
   - Offer detailed error diagnostics
   - Highlight errors in source code
   - Suggest potential fixes where possible

2. **Support Verification**:
   - Include static validation tools
   - Support verification before execution
   - Offer verbose mode for detailed checking

3. **Enable Debugging**:
   - Preserve debug information in error messages
   - Support step-by-step debugging
   - Allow inspection of state at error points

## Error Handling APIs

COIL implementations should provide error handling APIs that allow:

1. **Error Callbacks**: Registering callbacks for error events
2. **Error Queries**: Checking for specific error conditions
3. **Error Clearing**: Clearing error states
4. **Error Configuration**: Configuring error handling behavior

## Example Error Handling Code

### Error Detection and Reporting

```c
// Example error handling in a COIL processor implementation
coil_status_t execute_instruction(coil_processor_t* proc, coil_instruction_t* instr) {
    // Validate instruction
    if (!is_valid_instruction(instr)) {
        report_error(proc, ERROR_INVALID_INSTRUCTION, 
                    "Invalid instruction opcode: %02X", instr->opcode);
        return COIL_ERROR;
    }
    
    // Check operand types
    if (!are_compatible_types(instr->operands[0].type, instr->operands[1].type)) {
        report_error(proc, ERROR_TYPE_MISMATCH,
                    "Type mismatch: %04X and %04X", 
                    instr->operands[0].type, instr->operands[1].type);
        return COIL_ERROR;
    }
    
    // Execute instruction
    switch (instr->opcode) {
        // Instruction handling...
        
        default:
            report_error(proc, ERROR_UNIMPLEMENTED_INSTRUCTION,
                        "Unimplemented instruction: %02X", instr->opcode);
            return COIL_ERROR;
    }
    
    return COIL_SUCCESS;
}
```

### Error Recovery

```c
// Example error recovery in a COIL assembler
coil_status_t assemble_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        report_error(NULL, ERROR_FILE_NOT_FOUND,
                    "Could not open file: %s", filename);
        return COIL_ERROR;
    }
    
    // Parse file line by line
    char line[256];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        try {
            parse_and_assemble_line(line, line_number);
        } catch (coil_parsing_error& e) {
            // Report error but continue parsing
            report_error(NULL, e.error_code,
                        "Parsing error at line %d: %s", 
                        line_number, e.message);
            
            // Increment error count but continue
            error_count++;
        }
    }
    
    fclose(file);
    
    // Return success only if no errors
    return (error_count == 0) ? COIL_SUCCESS : COIL_ERROR;
}
```

## Security Considerations

### Preventing Information Leakage

Error messages should:
- Not reveal memory contents except as directly relevant to the error
- Not expose internal implementation details unnecessarily
- Avoid stack traces in production environments
- Limit information in security-sensitive contexts

### Avoiding Exploitable Errors

COIL processors should:
- Validate all input before processing
- Implement bounds checking for all memory operations
- Apply the principle of least privilege for all operations
- Handle resource exhaustion gracefully

### Secure Default Actions

When encountering security-relevant errors, COIL processors should:
- Fail securely (deny by default)
- Terminate processing rather than continue in an insecure state
- Log security-relevant errors appropriately
- Not expose sensitive information in error messages

## Error Testing

COIL implementations should include tests for error conditions:

1. **Error Injection**: Deliberately introduce errors to test handling
2. **Boundary Testing**: Test at the boundaries of valid operation
3. **Stress Testing**: Test under high load to find error handling weaknesses
4. **Security Testing**: Test for security implications of error conditions

## Related Documentation

- [Requirements](requirements.md) - Implementation requirements including error handling
- [Debug Format](debug-format.md) - Debug information for error analysis
- [Error Codes](../reference/error-codes.md) - Complete error code reference
- [Object Format](object-format.md) - Format considerations for error handling