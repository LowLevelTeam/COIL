# Error Classification

## Overview

COIL defines a comprehensive error classification system to standardize error handling across different implementations. This system ensures consistent error reporting, simplifies debugging, and enables robust error handling strategies.

## Error Categories

COIL defines four primary error categories:

| Category Code | Name | Description | Phase |
|---------------|------|-------------|-------|
| 0x01 | Compilation | Errors during CASM assembly | CASM to COIL |
| 0x02 | Linking | Errors during linking process | COIL to COILO |
| 0x03 | Validation | Errors during validation | Pre-execution |
| 0x04 | Runtime | Errors during program execution | Execution |

## Error Code Structure

Each error is identified by a 32-bit error code with the following structure:

```
┌───────────────┬───────────────┬─────────────────────┐
│ Category      │ Subcategory   │ Specific Error      │
│ (8 bits)      │ (8 bits)      │ (16 bits)           │
└───────────────┴───────────────┴─────────────────────┘
```

### Subcategory Codes

#### Compilation Subcategories (0x01xx)

| Code | Subcategory |
|------|-------------|
| 0x00 | Syntax |
| 0x01 | Scope |
| 0x02 | Instruction |
| 0x03 | Variable |
| 0x04 | Type |
| 0x05 | Symbol |
| 0x06 | Section |

#### Linking Subcategories (0x02xx)

| Code | Subcategory |
|------|-------------|
| 0x00 | Symbol Resolution |
| 0x01 | Section Alignment |
| 0x02 | Relocation |
| 0x03 | Format |
| 0x04 | Compatibility |

#### Validation Subcategories (0x03xx)

| Code | Subcategory |
|------|-------------|
| 0x00 | Type Check |
| 0x01 | Memory Safety |
| 0x02 | Instruction Validity |
| 0x03 | ABI Compliance |
| 0x04 | Resource Usage |

#### Runtime Subcategories (0x04xx)

| Code | Subcategory |
|------|-------------|
| 0x00 | Arithmetic |
| 0x01 | Memory |
| 0x02 | Function Call |
| 0x03 | Control Flow |
| 0x04 | Type |
| 0x05 | Resource |
| 0x06 | External |

### Examples of Specific Error Codes

#### Compilation - Syntax Errors (0x0100xx)

| Code | Error |
|------|-------|
| 0x0001 | Invalid token |
| 0x0002 | Unexpected end of file |
| 0x0003 | Missing operand |
| 0x0004 | Extra operand |
| 0x0005 | Invalid label |

#### Runtime - Arithmetic Errors (0x0400xx)

| Code | Error |
|------|-------|
| 0x0001 | Division by zero |
| 0x0002 | Integer overflow |
| 0x0003 | Integer underflow |
| 0x0004 | Floating-point overflow |
| 0x0005 | Floating-point underflow |

## Error Information Structure

When an error occurs, a standardized error information structure is created:

```
struct ErrorInfo {
    uint32_t error_code;      // Full 32-bit error code
    uint32_t location;        // File offset or memory address
    uint32_t file_id;         // Source file identifier (if applicable)
    uint32_t line;            // Source line number (if applicable)
    uint32_t column;          // Source column number (if applicable)
    uint16_t symbol_index;    // Related symbol index (if applicable)
    uint16_t section_index;   // Related section index (if applicable)
    uint16_t message_length;  // Length of error message
    char     message[];       // Variable-length error message
}
```

## Error Severity Levels

COIL defines three error severity levels:

| Level | Description | Action |
|-------|-------------|--------|
| Error | Prevents successful completion | Halts processing |
| Warning | Potential issue, but not fatal | Continues with caution |
| Note | Informational message | No impact on processing |

## Standard Error Output Format

COIL tools use a standard error output format:

```
[severity]:[category]:[file]:[line]:[column]: [error_code] - [message]
```

Example:
```
error:compilation:main.casm:42:10: 0x01000001 - Invalid token: expected identifier, found ')'
```

## Device-Specific Error Codes

Different device types may define specialized error codes:

| Code Range | Device |
|------------|--------|
| 0x04F0xxxx | CPU-specific runtime errors |
| 0x04F1xxxx | GPU-specific runtime errors |
| 0x04F2xxxx | TPU-specific runtime errors |

## Related Components

- [Error Detection](/coil-docs/core/error-detection.md) - Error detection mechanisms
- [Handling Mechanisms](/coil-docs/core/handling-mechanisms.md) - Error handling approaches
- [Device Error Handling](/coil-docs/core/device-error-handling.md) - Device-specific error handling