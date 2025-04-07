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

## Parameter Types

Conditional directives can use special parameter types for target-specific conditions:

| Parameter Type | Mnemonic | Description |
|----------------|----------|-------------|
| 0xFE (PARAM0)  | flags_p | Comparison condition (EQ, NEQ, GT, etc.) |
| 0xFD (PARAM1)  | pu_p       | Processing unit type (CPU, GPU, etc.) |
| 0xFC (PARAM2)  | arch_p     | Architecture type (X86, ARM, etc.) |
| 0xFB (PARAM3)  | [mode_p](./../types/parameters.md)     | Architecture mode (16, 32, 64, etc.) |


## Relationship with TARGET Directives

While conditional directives (IF/ELIF/ELSE/EIF) can be used for all types of compile-time conditions, the TARGET/ETARGET directives (described in [targeting.md](./targeting.md)) provide a more specialized approach for selecting processing units, architectures, and modes:


```
// Using conditional directives
IF PU_TYPE EQ CPU
    TARGET CPU
    IF ARCH_TYPE EQ X86
        TARGET x86
        ...
    EIF
EIF

// Equivalent using TARGET directives
TARGET CPU
    TARGET X86
        // x86-specific code
    ETARGET
ETARGET
```

The key differences are:

1. **Extended Instruction Access**: TARGET directives enable access to the extended instruction set of the specified target
2. **Nesting Behavior**: TARGET directives have specific nesting rules for PU, architecture, and mode
3. **Error Handling**: TARGET directives provide more specific error messages for target compatibility issues

Use conditional directives for general compile-time decisions, and TARGET directives specifically for platform-specific code sections that use extended instructions.

## Related Components

- [Targeting](./targeting.md) - TARGET and ETARGET directives
- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [Compile-time Definitions](./compile-time-definitions.md) - DEF, UDEF directives
- [Configuration Format](../core/config-format.md) - Target configuration specification