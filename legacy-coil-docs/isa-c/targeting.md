# Conditional Compilation Directives

## Overview

Target directives enable COIL programs to utilize the extended instruction set based on compile-time conditions. These directives are essential for creating portable code that can adapt to different processing units, architectures, and execution environments while also being needed for situations like CPU bootloaders.


## Directive Set

| Opcode | Mnemonic | Description        |
|--------|----------|--------------------|
| 0xFC   | TARGET   | Begin target block |
| 0xFD   | ETARGET  | End target block   |

## Detailed Specifications

### TARGET (0xFC)

Begins a target compilation block.

**Encoding:**
```
// Specific Processing Unit
[0xFC][0x01][op1: PARAM0]

// Specific Processing Unit and Architecture
[0xFC][0x02][op1: PARAM0][op2: PARAM1]

// Specific Processing Unit, Architecture and Mode
[0xFC][0x02][op1: PARAM0][op2: PARAM1][op3: PARAM2]
```

**Behavior:**
- Evaluates the condition at compile time
- If a previous target directive set processing unit the next one inside the block can select architecture and so on with mode
- If only architecture is given and no discernable processing unit is available expect an error
- If only mode is given and no discernable architecture or processing unit is available expect an error

### ETARGET (0xFD)

Provides an alternative condition when the previous IF or ELIF was false.

**Encoding:**
```
[0xFD]
```

**Behavior:**
- End the current TARGET scope
- If no TARGET is given expect an error

## Parameter Types

| Parameter Type | Mnemonic | Description |
|----------------|----------|-------------|
| 0xFE (PARAM0)  | PU_TYPE       | Processing unit type (CPU, GPU, etc.) |
| 0xFD (PARAM1)  | ARCH_TYPE     | Architecture type (X86, ARM, etc.) |
| 0xFC (PARAM2)  | MODE_TYPE     | Architecture mode (16, 32, 64, etc.) |

## Related Components

- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [Configuration Format](../core/config-format.md) - Target configuration specification