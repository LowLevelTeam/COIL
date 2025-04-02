# Conditional Compilation Directives

## Overview

Conditional compilation directives enable COIL programs to include or exclude code sections based on compile-time conditions. These directives are essential for creating portable code that can adapt to different processing units, architectures, and execution environments.

## Directive Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xF0   | IF       | Begin conditional block |
| 0xF1   | ELIF     | Alternative condition |
| 0xF2   | ELSE     | Default case |
| 0xF3   | EIF      | End conditional block |

## Detailed Specifications

### IF (0xF0)

Begins a conditional compilation block.

**Encoding:**
```
// Simple form (one operand)
[0xF0][0x01][condition: Operand]

// Comparison form (three operands)
[0xF0][0x03][op1: Operand][comparison: FLAG_CONDITION][op2: Operand]

// Two-operand form (implied comparison)
[0xF0][0x02][parameter_type: Operand][value: Operand]
```

**Behavior:**
- Evaluates the condition at compile time
- If true, the following code block is included in compilation
- If false, the code block is excluded until the next ELIF, ELSE, or EIF
- For one-operand form, condition is treated as a boolean
- For three-operand form, a comparison is performed between op1 and op2
- For two-operand form, an implied equality comparison is performed

**Examples:**
```
// Check if DEBUG is defined
IF DEBUG
    // Debug code here
EIF

// Check if CPU architecture is x86
IF PU_TYPE EQ CPU
    IF ARCH_TYPE EQ X86
        // x86-specific code
    EIF
EIF

// Check if floating-point precision is high
IF FP_PRECISION GT 2
    // High-precision code
EIF
```

### ELIF (0xF1)

Provides an alternative condition when the previous IF or ELIF was false.

**Encoding:**
```
// Simple form (one operand)
[0xF1][0x01][condition: Operand]

// Comparison form (three operands)
[0xF1][0x03][op1: Operand][comparison: FLAG_CONDITION][op2: Operand]

// Two-operand form (implied comparison)
[0xF1][0x02][parameter_type: Operand][value: Operand]
```

**Behavior:**
- Only evaluated if all preceding IF and ELIF conditions were false
- If true, the following code block is included in compilation
- If false, the code block is excluded until the next ELIF, ELSE, or EIF
- Encoding and evaluation follow the same rules as IF

**Examples:**
```
// Platform-specific code
IF PU_TYPE EQ CPU
    IF ARCH_TYPE EQ X86
        // x86-specific code
    ELIF ARCH_TYPE EQ ARM
        // ARM-specific code
    ELSE
        // Generic CPU code
    EIF
ELIF PU_TYPE EQ GPU
    // GPU-specific code
EIF
```

### ELSE (0xF2)

Provides a default case when all previous conditions were false.

**Encoding:**
```
[0xF2]
```

**Behavior:**
- Only evaluated if all preceding IF and ELIF conditions were false
- The following code block is unconditionally included in compilation
- Must appear after at least one IF or ELIF directive
- Must appear before the matching EIF directive

**Examples:**
```
// Feature detection
IF SIMD_SUPPORT
    // SIMD implementation
ELSE
    // Scalar fallback implementation
EIF
```

### EIF (0xF3)

Ends a conditional compilation block.

**Encoding:**
```
[0xF3]
```

**Behavior:**
- Marks the end of a conditional compilation block
- Must match a preceding IF directive
- Compilation continues normally after this directive

**Examples:**
```
// Conditional block with multiple cases
IF OPTIMIZATION_LEVEL EQ 0
    // No optimization
ELIF OPTIMIZATION_LEVEL EQ 1
    // Basic optimization
ELSE
    // Maximum optimization
EIF  // End of conditional block
```

## Parameter Types

Conditional directives can use special parameter types for target-specific conditions:

| Parameter Type | Mnemonic | Description |
|----------------|----------|-------------|
| 0xFE (PARAM0)  | FLAG_CONDITION | Comparison condition (EQ, NEQ, GT, etc.) |
| 0xFD (PARAM1)  | PU_TYPE       | Processing unit type (CPU, GPU, etc.) |
| 0xFC (PARAM2)  | ARCH_TYPE     | Architecture type (X86, ARM, etc.) |
| 0xFB (PARAM3)  | MODE_TYPE     | Architecture mode (16, 32, 64, etc.) |

## Flag Conditions

The following conditions can be used with conditional directives:

| Value | Mnemonic | Description |
|-------|----------|-------------|
| 0x00  | EQ       | Equal |
| 0x01  | NEQ      | Not equal |
| 0x02  | GT       | Greater than |
| 0x03  | GTE      | Greater than or equal |
| 0x04  | LT       | Less than |
| 0x05  | LTE      | Less than or equal |
| 0xFF  | DEF      | Defined (exists) |

## Nesting and Scope

Conditional directives can be nested to create complex conditions:

- Each IF must have a matching EIF
- Maximum nesting depth is implementation-defined
- Inner conditionals are only evaluated if the outer condition is true

```
IF PU_TYPE EQ CPU
    // CPU-specific code
    IF ARCH_TYPE EQ X86
        // x86-specific code
        IF MODE_TYPE EQ MODE_64
            // 64-bit mode specific code
        EIF
    EIF
EIF
```

## Related Components

- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [Compile-time Definitions](./compile-time-definitions.md) - DEF, UDEF directives
- [Configuration Format](../core/config-format.md) - Target configuration specification