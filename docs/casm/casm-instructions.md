# CASM Instructions Reference

## Purpose

This document provides a detailed reference for all instructions in CASM (COIL Assembly), including their syntax patterns, operand forms, and examples of usage. It serves as the companion to the CASM Syntax document, focusing specifically on instruction usage.

## Instruction Categories

CASM instructions are organized into functional categories:

- **Control Flow** (0x01-0x0F): Program flow, branches, calls
- **Memory Operations** (0x10-0x2F): Data movement, variables, memory access
- **Bit Manipulation** (0x50-0x5F): Logical operations, shifts, bit-level operations
- **Arithmetic** (0x60-0x8F): Mathematical operations
- **Vector/Array** (0x90-0x9F): Operations on multiple data elements
- **Type Operations** (0xA0-0xAF): Type manipulation, structures, arrays
- **Directives** (0xB0-0xBF): Processor selection, assembly control
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

### Type Operands

References to types:
```
TYPEOF type_info, value            ; Get type information
CONVERT TYPE_FP32, result, TYPE_INT32, value  ; Convert integer to float
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

### CMP (0x05)
Compare two values and set flags.

```
CMP left, right
```

Example:
```
CMP counter, limit          ; Compare counter with limit
```

### SWITCH (0x09)
Multi-way branch based on a value.

```
SWITCH value, case_count
  case_value1, case_target1
  case_value2, case_target2
  ...
  default_target
```

Example:
```
SWITCH option, 3
  1, case_one
  2, case_two
  3, case_three
  default_case
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

### PUSH/POP (0x11, 0x12)
Push/pop values to/from the stack or demote/promote variables.

```
PUSH source[, TYPE_PARAM5=condition]
POP destination[, TYPE_PARAM5=condition]
```

Examples:
```
PUSH value                    ; Push value to stack
POP result                    ; Pop from stack to result
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

### MEMCPY/MEMSET/MEMCMP (0x17, 0x18, 0x19)
Memory block operations.

```
MEMCPY destination, source, size[, TYPE_PARAM5=condition]
MEMSET destination, value, size[, TYPE_PARAM5=condition]
MEMCMP result, first, second, size
```

Examples:
```
MEMCPY dest_buffer, src_buffer, 100    ; Copy 100 bytes
MEMSET buffer, 0, 1024                 ; Fill 1KB with zeros
MEMCMP result, buffer1, buffer2, 10    ; Compare 10 bytes
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

### MIN/MAX (0x7B, 0x7C)
Minimum and maximum operations.

```
MIN destination, left, right[, TYPE_PARAM5=condition]
MAX destination, left, right[, TYPE_PARAM5=condition]
```

Examples:
```
MIN result, a, b                ; result = (a < b) ? a : b
MAX result, a, b                ; result = (a > b) ? a : b
```

## Type Operations

### TYPEOF/SIZEOF/ALIGNOF (0xA0-0xA2)
Type information operations.

```
TYPEOF destination, source[, TYPE_PARAM5=condition]
SIZEOF destination, source[, TYPE_PARAM5=condition]
ALIGNOF destination, source[, TYPE_PARAM5=condition]
```

Examples:
```
TYPEOF type_id, variable        ; Get variable's type
SIZEOF size, TYPE_INT64         ; Get size of INT64 (8)
ALIGNOF alignment, TYPE_STRUCT=point  ; Get structure alignment
```

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
- `_Z` or `BRANCH_COND_Z`: Zero
- `_NZ` or `BRANCH_COND_NZ`: Not zero

Examples:
```
ADD_EQ result, a, b           ; Add only if equal flag is set
MOV_NZ result, value          ; Move only if not zero flag is set
```

## Binary Mapping Examples

Below are examples showing CASM instructions and their binary encoding:

### MOV Example

CASM:
```
MOV counter, 42
```

Binary:
```
0x10      ; Opcode for MOV
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42 (little-endian)
```

### ADD Example

CASM:
```
ADD result, a, b
```

Binary:
```
0x60      ; Opcode for ADD
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "a"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "b"
```

## Related Documentation

- [CASM Syntax](casm-syntax.md) - Fundamental syntax rules for CASM
- [CASM Directives](casm-directives.md) - Comprehensive coverage of assembly directives
- [Binary Encoding](binary-encoding.md) - How CASM translates to binary COIL
- [Instruction Reference](../reference/instruction-ref.md) - Complete instruction listing with opcodes