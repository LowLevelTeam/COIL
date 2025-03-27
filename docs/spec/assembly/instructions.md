# CASM Instructions Reference

## Purpose

This document provides a detailed reference for all instructions in CASM (COIL Assembly), including their syntax patterns, operand forms, and usage examples. It focuses solely on executable instructions, serving as a companion to the CASM Syntax and CASM Directives documents.

## Key Concepts

- **Instruction Patterns**: Common formats for instruction usage
- **Operand Forms**: Different types of operands that instructions can accept
- **Type-Determined Behavior**: How operand types affect instruction execution
- **Instruction Categories**: Functional groupings of related instructions

## Instruction Categories

CASM instructions are organized into functional categories:

- **Control Flow** (0x01-0x0F): Program flow, branches, calls
- **Memory Operations** (0x10-0x2F): Data movement, variables, memory access
- **Bit Manipulation** (0x50-0x5F): Logical operations, shifts, bit-level operations
- **Arithmetic** (0x60-0x8F): Mathematical operations
- **Vector/Array** (0x90-0x9F): Operations on multiple data elements
- **Type Operations** (0xA0-0xAF): Type manipulation, structures, arrays
- **Processor-Specific** (0xC0-0xFE): Architecture/processor-specific instructions

## Common Instruction Patterns

Most CASM instructions follow one of these patterns:

### Three-Operand Instructions

```
INSTRUCTION destination, source1, source2[, extra_parameters]
```

Examples:
```
ADD result, a, b           ; result = a + b
SUB total, total, value    ; total = total - value
```

### Two-Operand Instructions

```
INSTRUCTION destination, source[, extra_parameters]
```

Examples:
```
MOV counter, 0             ; counter = 0
NOT flags, original_flags  ; flags = ~original_flags
```

### One-Operand Instructions

```
INSTRUCTION operand[, extra_parameters]
```

Examples:
```
INC counter                ; counter++
PUSH value                 ; Push value onto stack
```

### Zero-Operand Instructions

```
INSTRUCTION
```

Examples:
```
NOP                        ; No operation
SCOPEE                     ; Begin scope
SCOPEL                     ; End scope
```

## Operand Forms

Instructions can accept operands in various forms:

### Register Operands

Direct reference to a processor register:
```
MOV TYPE_RGP=RAX, 42       ; Move 42 to RAX register
```

### Immediate Operands

Literal values used directly as operands:
```
MOV counter, 42            ; 42 is an immediate value
ADD total, total, 0xFF     ; 0xFF is an immediate hexadecimal value
```

### Variable Operands

References to named variables:
```
MOV result, counter        ; Move counter value to result
```

### Memory Operands

References to memory locations:
```
MOV value, [address]       ; Load from memory
MOV [address], value       ; Store to memory
```

Complex addressing modes:
```
MOV value, [base + offset]           ; Base + offset
MOV value, [base + index*scale]      ; Base + scaled index
MOV value, [base + index*scale + offset] ; Full addressing mode
```

## Control Flow Instructions

### SYM (0x01)
Define a symbol for use in the program.

```
SYM symbol_name[, TYPE_PARAM0=symbol_parameter]
```

Example:
```
SYM main, TYPE_PARAM0=GLOB   ; Define global symbol 'main'
```

### BR (0x02)
Branch to a target address.

```
BR target[, TYPE_PARAM0=branch_control][, TYPE_PARAM5=condition]
```

Conditional form:
```
BR_condition target   ; e.g., BR_EQ, BR_LT, BR_GT
```

Examples:
```
BR loop_start             ; Unconditional branch
BR_EQ equal_branch        ; Branch if equal
BR_LT less_than_branch    ; Branch if less than
```

### CALL (0x03)
Call a function.

```
CALL target[, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...]
```

Examples:
```
CALL function               ; Call with no parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
                            ; Call with parameters using platform ABI
```

### RET (0x04)
Return from a function.

```
RET [TYPE_ABICTL=ABICTL_RET=abi_name, return_value1, return_value2, ...]
```

Examples:
```
RET                          ; Return with no value
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
                             ; Return with value using platform ABI
```

## Memory Operations

### MOV (0x10)
Move data between locations.

```
MOV destination, source[, TYPE_PARAM5=condition]
```

Examples:
```
MOV counter, 0                ; Set counter to 0
MOV result, value             ; Copy value to result
MOV [address], 42             ; Store 42 to memory
MOV value, [address]          ; Load from memory
MOV TYPE_RGP=RAX, TYPE_RGP=RBX  ; Copy between registers
```

### VAR (0x16)
Declare a variable with optional initialization.

```
VAR type, name[, initial_value]
```

Examples:
```
VAR TYPE_INT32, counter, 0                    ; Integer with initialization
VAR TYPE_PTR, data_pointer                    ; Pointer without initialization
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"     ; Array initialization
VAR TYPE_STRUCT=point, position, (10, 20)     ; Structure initialization
```

### SCOPEE/SCOPEL (0x14, 0x15)
Enter and leave variable scopes.

```
SCOPEE
  ; Variables and code in this scope
SCOPEL
```

Example:
```
SCOPEE
  VAR TYPE_INT32, temp, 0
  ; Use temp...
SCOPEL   ; temp is destroyed here
```

## Arithmetic Instructions

### ADD/SUB/MUL/DIV (0x60-0x63)
Basic arithmetic operations.

```
ADD destination, left, right[, TYPE_PARAM5=condition]
SUB destination, left, right[, TYPE_PARAM5=condition]
MUL destination, left, right[, TYPE_PARAM5=condition]
DIV destination, left, right[, TYPE_PARAM5=condition]
```

Examples:
```
ADD sum, a, b                   ; sum = a + b
SUB difference, a, b            ; difference = a - b
MUL product, a, b               ; product = a * b
DIV quotient, a, b              ; quotient = a / b
```

### INC/DEC (0x65, 0x66)
Increment and decrement operations.

```
INC destination[, TYPE_PARAM5=condition]
DEC destination[, TYPE_PARAM5=condition]
```

Examples:
```
INC counter                     ; counter += 1
DEC remaining                   ; remaining -= 1
```

## Bit Manipulation Instructions

### AND/OR/XOR/NOT (0x50-0x53)
Bitwise logical operations.

```
AND destination, left, right[, TYPE_PARAM5=condition]
OR destination, left, right[, TYPE_PARAM5=condition]
XOR destination, left, right[, TYPE_PARAM5=condition]
NOT destination, source[, TYPE_PARAM5=condition]
```

Examples:
```
AND result, flags, mask         ; result = flags & mask
OR flags, flags, new_bits       ; flags = flags | new_bits
XOR value, value, toggle_mask   ; value = value ^ toggle_mask
NOT inverted, original          ; inverted = ~original
```

### SHL/SHR/SAR (0x54-0x56)
Shift operations.

```
SHL destination, value, shift_amount[, TYPE_PARAM5=condition]
SHR destination, value, shift_amount[, TYPE_PARAM5=condition]
SAR destination, value, shift_amount[, TYPE_PARAM5=condition]
```

Examples:
```
SHL result, value, 2            ; result = value << 2
SHR result, value, 3            ; result = value >> 3 (logical)
SAR result, value, 1            ; result = value >> 1 (arithmetic)
```

## Type Operations

### CONVERT/CAST (0xA3, 0xA4)
Type conversion operations.

```
CONVERT destination, source[, TYPE_PARAM5=condition]
CAST destination, source[, TYPE_PARAM5=condition]
```

Examples:
```
CONVERT float_result, int_value   ; Convert with value preservation
CAST bit_pattern, float_value     ; Reinterpret bit pattern
```

### Structure and Array Operations (0xA5-0xA9)
Composite type operations.

```
STRUCT destination, field_types_and_names...
GET destination, source, field[, TYPE_PARAM5=condition]
SET destination, field, value[, TYPE_PARAM5=condition]
INDEX destination, source, index[, TYPE_PARAM5=condition]
UPDT destination, index, value[, TYPE_PARAM5=condition]
```

Examples:
```
STRUCT TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y
GET x_value, point, x           ; Get x field from point
SET point, y, 20                ; Set y field in point
INDEX element, array, 2         ; Get element at index 2
UPDT array, 1, 42               ; Set element at index 1 to 42
```

## Conditional Execution

Many instructions support conditional execution using the TYPE_PARAM5 parameter:

```
INSTRUCTION_condition operands...
```

Or the explicit form:
```
INSTRUCTION operands..., TYPE_PARAM5=condition
```

Common conditions include:
- `_EQ` or `BRANCH_COND_EQ`: Equal
- `_NE` or `BRANCH_COND_NE`: Not equal
- `_GE` or `BRANCH_COND_GE`: Greater than or equal
- `_LT` or `BRANCH_COND_LT`: Less than
- `_GT` or `BRANCH_COND_GT`: Greater than
- `_LE` or `BRANCH_COND_LE`: Less than or equal

Examples:
```
ADD_EQ result, a, b           ; Add only if equal flag is set
MOV_NZ result, value          ; Move only if not zero flag is set
```

## Common Instruction Patterns

### Counter Loop

```
; Loop with counter
VAR TYPE_INT32, i, 0
VAR TYPE_INT32, limit, 10

loop_start:
  CMP i, limit
  BR_GE loop_end
  
  ; Loop body...
  
  INC i
  BR loop_start
loop_end:
```

### Conditional Execution

```
; if (a == b) then X else Y
CMP a, b
BR_NE else_branch
; X code here
BR end_if
else_branch:
; Y code here
end_if:
```

### Function Call with Return Value

```
; Call function and use return value
CALL calculate, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

## Related Documentation

- [CASM Syntax](syntax.md) - Fundamental syntax rules for CASM
- [CASM Directives](directives.md) - Documentation for assembly directives
- [Binary Format](../binary-format.md) - How CASM translates to binary COIL
- [Instruction Set](../instruction-set/) - Detailed documentation of each instruction category