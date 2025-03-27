# COIL Type System Overview

## Purpose

The type system is the foundation of COIL's type-determined instruction philosophy. It defines how data is represented and how instruction behavior is influenced by operand types.

## Type-Determined Instructions

In COIL, instructions derive their behavior from the types of their operands:

- `ADD result, a, b` with integer operands performs integer addition
- The same instruction with floating-point operands performs floating-point addition
- The same instruction with vector operands performs element-wise vector addition

This approach provides consistency, extensibility, and a compact instruction set.

## Type Encoding

Types in COIL are encoded in a 16-bit format:

```
┌────────────┬────────────┐
│  Main Type │ Extensions │
│  (8 bits)  │  (8 bits)  │
└────────────┴────────────┘
```

- **First 8 bits**: Main Type - Primary type category
- **Second 8 bits**: Type Extensions - Qualifiers and modifiers

Some types require additional data beyond this 16-bit descriptor.

## Type Categories

COIL organizes types into several categories:

### Basic Types
- Integer types (signed and unsigned, various widths)
- Floating-point types (various precisions)
- Boolean type

### Composite Types
- Structures
- Arrays
- Vectors

### Special Types
- Variable references
- Symbol references
- Register references
- Parameter types

### Platform-Dependent Types
- Types that automatically adjust to the target platform

## Type Extensions

Type extensions modify the behavior of the base type:

- `CONST`: Read-only value
- `VOLATILE`: Value that may change unexpectedly
- `IMM`: Immediate value (literal)
- `VAR`: Variable identifier
- `SYM`: Symbol identifier

## Type Compatibility

Types are compatible under specific conditions:

1. Types match exactly
2. One type is platform-dependent, the other is its fixed-width equivalent
3. Both are integers with same signedness and destination has equal/greater width
4. Both are floating-point and destination has equal/greater precision
5. Both are vectors with compatible elements and sufficient size

## Type Conversion

COIL provides two primary conversion mechanisms:

### CONVERT Instruction
- Value-preserving conversion
- May involve rounding or truncation
- Follows standard conversion rules

```
CONVERT float_value, int_value   ; Convert integer to float
```

### CAST Instruction
- Reinterprets the bit pattern
- No change to underlying data
- Useful for low-level operations

```
CAST bit_pattern, float_value    ; View float bits as an integer
```

## Type System Impact

The type system affects many aspects of COIL:

- **Instruction Behavior**: Determines operation semantics
- **Memory Allocation**: Determines size and alignment
- **Register Selection**: Influences register assignment
- **ABI Implementation**: Determines parameter passing conventions
- **Optimization**: Enables type-based optimizations