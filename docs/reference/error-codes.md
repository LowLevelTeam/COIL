# COIL Error Codes Reference

## Purpose

This document provides a reference for all standard error codes in COIL, their meanings, and appropriate handling strategies. A unified error code system enables consistent error reporting across different COIL implementations.

## Error Categories

COIL error codes are organized into categories based on the subsystem where they occur:

| Category | Code Range | Description |
|----------|------------|-------------|
| Format Errors | 1xxx | Errors in COIL binary format |
| Instruction Errors | 2xxx | Errors in instruction parsing or execution |
| Type System Errors | 3xxx | Type-related errors |
| Memory Errors | 4xxx | Memory access or allocation errors |
| Symbol Errors | 5xxx | Symbol resolution or definition errors |
| Architecture Errors | 6xxx | Architecture-specific errors |
| ABI Errors | 7xxx | ABI-related errors |
| Implementation Errors | 8xxx | Limitations of specific implementations |

## Error Severity Levels

Each error has an associated severity level:

1. **Fatal (F)**: Prevents continued processing, requires immediate termination
2. **Non-Fatal (N)**: Allows continued processing with limitations
3. **Warning (W)**: Indicates potential issues but doesn't prevent execution

## Standard Error Codes

### Format Errors (1xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 1001 | F | Invalid magic number |
| 1002 | F | Unsupported version |
| 1003 | F | Corrupted header |
| 1004 | F | Invalid section table |
| 1005 | F | Invalid symbol table |
| 1006 | F | Invalid relocation table |
| 1007 | F | Section overlap |
| 1008 | N | Invalid alignment |
| 1009 | F | Missing required section |

### Instruction Errors (2xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 2001 | F | Invalid opcode |
| 2002 | F | Invalid operand count |
| 2003 | F | Invalid operand type |
| 2004 | F | Type mismatch |
| 2005 | F | Missing operand |
| 2006 | F | Extra operand |
| 2007 | F | Invalid instruction format |
| 2008 | N | Reserved instruction used |
| 2009 | N | Unsupported instruction |

### Type System Errors (3xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 3001 | F | Invalid type encoding |
| 3002 | F | Incompatible types |
| 3003 | N | Invalid type conversion |
| 3004 | N | Type size mismatch |
| 3005 | N | Invalid type extension |
| 3006 | N | Type not supported on target |
| 3007 | F | Invalid structure definition |
| 3008 | F | Invalid array definition |

### Memory Errors (4xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 4001 | F | Invalid memory address |
| 4002 | N | Misaligned memory access |
| 4003 | F | Protected memory access |
| 4004 | F | Stack overflow |
| 4005 | F | Stack underflow |
| 4006 | N | Invalid memory operation size |
| 4007 | F | Memory allocation failure |
| 4008 | F | Invalid scope operation |

### Symbol Errors (5xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 5001 | F | Undefined symbol |
| 5002 | F | Duplicate symbol definition |
| 5003 | N | Symbol scope violation |
| 5004 | F | Invalid symbol type |
| 5005 | N | Mismatched symbol usage |
| 5006 | F | Unresolved external symbol |
| 5007 | N | Symbol visibility error |

### Architecture Errors (6xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 6001 | F | Unsupported processor |
| 6002 | F | Unsupported architecture |
| 6003 | F | Unsupported mode |
| 6004 | F | Invalid register reference |
| 6005 | N | Invalid addressing mode |
| 6006 | N | Feature not available in selected mode |
| 6007 | F | Privileged instruction in unprivileged context |

### ABI Errors (7xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 7001 | F | Incompatible ABI |
| 7002 | F | Invalid parameter passing |
| 7003 | F | Invalid return value handling |
| 7004 | N | Stack alignment violation |
| 7005 | N | Register preservation failure |
| 7006 | F | Invalid calling convention |

### Implementation Errors (8xxx)

| Code | Severity | Description |
|------|----------|-------------|
| 8001 | N | Implementation limit exceeded |
| 8002 | N | Unsupported feature |
| 8003 | F | Internal resource exhaustion |
| 8004 | N | Processing timeout |
| 8005 | N | Implementation-specific limitation |

## Error Message Format

Error messages should follow a consistent format:

```
ERROR [E1001]: Invalid magic number
  in file: example.coil
  at offset: 0x00000000
  expected: "COIL"
  found: "CILO"
```

Warning messages follow a similar format:

```
WARNING [W2050]: Inefficient instruction sequence
  in file: example.coil
  at lines: 45-47
  suggestion: Use LEA or direct calculation
```

## Error Handling Guidelines

### For COIL Processors

A compliant COIL processor should:

1. **Detect errors early** and provide clear diagnostics
2. **Report multiple errors** where possible (avoid stopping at the first error)
3. **Provide location information** (file, line, offset)
4. **Suggest potential fixes** for common errors

### For COIL Toolchain

COIL tools should:

1. **Validate thoroughly** before processing
2. **Provide clear error messages** with actionable information
3. **Group related errors** to avoid overwhelming the user
4. **Enable verbose error reporting** modes for debugging

## Related Documentation

- [Error Handling](../implementation/error-handling.md) - Detailed error handling mechanisms
- [Implementation Requirements](../implementation/requirements.md) - Requirements for error handling
- [Binary Format](../spec/binary-format.md) - Binary format validation