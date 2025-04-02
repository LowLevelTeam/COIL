# Control Flow Instructions

## Overview

Control flow instructions manage program execution by enabling conditional logic, context switching, function calls, and comparisons. These operations form the foundation of program flow in COIL.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x00   | NOP      | No operation |
| 0x01   | BR       | Branch to address |
| 0x02   | CALL     | Call function |
| 0x03   | RET      | Return from function |
| 0x04   | CMP      | Compare values |
| 0x05   | TEST     | Test values and set flags |

## Detailed Specifications

### NOP (0x00)

Performs no operation. Execution continues to the next instruction.

**Encoding:**
```
[0x00]
```

**Behavior:**
- No effect on program state
- Used for alignment, timing, or placeholder purposes

### BR (0x01)

Branches to the specified address, optionally based on a flag condition.

**Encoding:**
```
// Unconditional branch
[0x01][0x01][address: integral, symbol]

// Conditional branch
[0x01][0x02][0xFE][FLAG_CONDITION][address: integral, symbol]
```

**Behavior:**
- Unconditional: Program execution continues at specified address
- Conditional: Execution branches only if the specified condition is met
- Address can be an immediate value, variable, or symbol reference

### CALL (0x02)

Calls a function, saving the return address.

**Encoding:**
```
// Unconditional call
[0x02][0x01][address: integral, symbol]

// Conditional call
[0x02][0x02][0xFE][FLAG_CONDITION][address: integral, symbol]
```

**Behavior:**
- Sets up a new call frame according to the current ABI
- Saves the return address to the call stack
- Transfers control to the specified address
- Conditional calls only execute if the specified condition is met

### RET (0x03)

Returns from a function to the caller.

**Encoding:**
```
// Unconditional return
[0x03][0x00]

// Conditional return
[0x03][0x01][0xFE][FLAG_CONDITION]
```

**Behavior:**
- Restores the previous call frame according to the current ABI
- Returns control to the saved return address
- Conditional returns only execute if the specified condition is met

### CMP (0x04)

Compares two values and sets flags based on the result.

**Encoding:**
```
[0x04][0x02][op1: Operand][op2: Operand]
```

**Behavior:**
- Performs a comparison between op1 and op2
- Sets flags based on the comparison result
- Does not modify either operand
- Comparison behavior depends on operand types:
  - Integers: Arithmetic comparison
  - Floating-point: IEEE-754 comparison
  - Vectors: Element-wise comparison
  - Complex types: Implementation-defined comparison

### TEST (0x05)

Tests bit patterns and sets flags based on the result.

**Encoding:**
```
[0x05][0x02][op1: Operand][op2: Operand]
```

**Behavior:**
- Performs a bitwise AND between op1 and op2
- Sets flags based on the result
- Does not modify either operand
- Zero flag set if result is zero
- Sign flag set if high bit of result is set

## Flag Conditions

Flag conditions determine whether conditional operations execute:

| Value | Mnemonic | Description |
|-------|----------|-------------|
| 0x00  | EQ       | Equal (Zero) |
| 0x01  | NEQ      | Not Equal (Not Zero) |
| 0x02  | GT       | Greater Than |
| 0x03  | GTE      | Greater Than or Equal |
| 0x04  | LT       | Less Than |
| 0x05  | LTE      | Less Than or Equal |
| 0xFF  | DEF      | Defined |

## Related Components

- [Memory Operations](./memory-operations.md) - Memory management instructions
- [Type System](../types/type-system.md) - Complete type system reference