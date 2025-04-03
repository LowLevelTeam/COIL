# Conditional Compilation Directives

## Overview

The pragma directive enables COIL programs to give hints at compile time for the COIL Processor to take in account when generating the native binary or modified.

## Directive Set

| Opcode | Mnemonic | Description        |
|--------|----------|--------------------|
| 0xFE   | PRAGMA   | Hint the Processor |

## Detailed Specifications

### PARGMA (0xFC)

Begins a target compilation block.

**Encoding:**
```
[0xFC][hint: HINT]

type: HINT = {
  [ERROR = 0x00][printable_byte_count: uint16_t][bytes: bytes[printable_byte_count]]
    // Stop execution with a user defined compilation error
    // Two arguments as defined above
  // More coming later
}
```

## Related Components

- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
