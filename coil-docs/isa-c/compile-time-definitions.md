# Compile-time Definition Directives

## Overview

Compile-time definition directives enable the creation and management of symbolic constants, configuration values, and conditional flags. These directives support parameterization, configuration, and conditional compilation of COIL programs.

## Directive Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xFA   | DEF      | Define a compile-time value |
| 0xFB   | UDEF     | Undefine a compile-time value |

## Detailed Specifications

### DEF (0xFA)

Defines a compile-time value.

**Encoding:**
```
// Definition without value (flag)
[0xFA][0x01][definition_id: DEFID]

// Definition with value
[0xFA][0x02][definition_id: DEFID][value: Operand]
```

**Behavior:**
- Creates a named compile-time constant or flag
- Definition ID is a unique identifier for the definition
- Optional value associates data with the definition
- If no value is provided, the definition acts as a flag (exists/does not exist)
- Redefinition with the same ID replaces the previous value
- Definitions can be tested in conditional directives

### UDEF (0xFB)

Undefines a previously defined compile-time value.

**Encoding:**
```
[0xFB][0x01][definition_id: DEFID]
```

**Behavior:**
- Removes a compile-time definition
- Definition ID identifies which definition to remove
- After undefinition, the identifier no longer exists
- Undefining a non-existent definition has no effect (not an error)
- Used to remove definitions that are no longer needed or to control scope

## Definition Scoping

Definitions follow these scoping rules:

1. **Global Scope**: Definitions at the top level are visible throughout the compilation unit
2. **File Scope**: Definitions in included files are visible only within that file unless explicitly exported
3. **Conditional Scope**: Definitions inside conditional blocks exist only if the condition is true
4. **Explicit Undefinition**: Definitions exist until explicitly undefined or until the end of their scope

```
// Global definition
DEF GLOBAL_CONSTANT, 42

// Conditional definition
IF DEBUG
    DEF DEBUG_LEVEL, 3
    // DEBUG_LEVEL only exists within this block if DEBUG is defined
EIF

// Scoped definition and undefinition
DEF TEMP_FLAG
// ... code using TEMP_FLAG ...
UDEF TEMP_FLAG
// TEMP_FLAG no longer exists here
```

## Definition Types

Definitions can have different types of values:

| Value Type | Example | Usage |
|------------|---------|-------|
| Flag (no value) | `DEF DEBUG` | Existence testing with DEF |
| Integer | `DEF MAX_COUNT, 100` | Numeric comparisons and calculations |
| Float | `DEF TOLERANCE, 0.001` | Floating-point comparisons and calculations |
| String | `DEF VERSION, "1.0.0"` | String comparisons and substitutions |

## Predefined Definitions

COIL implementations typically provide predefined definitions:

| Definition | Description | Example Value |
|------------|-------------|---------------|
| `COIL_VERSION` | COIL specification version | 0x010000 (v1.0.0) |
| `PU_TYPE` | Current processing unit type | CPU |
| `ARCH_TYPE` | Current architecture | X86 |
| `MODE_TYPE` | Current mode | MODE_64 |
| `ENDIAN` | Endianness | LITTLE |
| `WORD_SIZE` | Native word size in bits | 64 |

## Definition Usage Patterns

Compile-time definitions enable several important patterns:

1. **Feature Flags**: Control which features are compiled
2. **Configuration Constants**: Set system parameters
3. **Version Control**: Handle different specification versions
4. **Platform Adaptation**: Adjust code for different platforms

## Related Components

- [Conditional Compilation](./conditional-compilation.md) - IF, ELIF, ELSE, EIF directives
- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [Configuration Format](../core/config-format.md) - Target configuration specification