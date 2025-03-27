# COIL Control Flow Reference

## Core Control Flow Instructions

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x01   | SYM      | Define a symbol |
| 0x02   | BR       | Branch to target |
| 0x03   | CALL     | Call function |
| 0x04   | RET      | Return from function |
| 0x05   | CMP      | Compare values and set flags |
| 0x06   | TEST     | Bitwise AND and set flags |
| 0x07   | JMP      | Jump (alias for BR) |
| 0x08   | LOOP     | Decrement counter and branch if not zero |
| 0x09   | SWITCH   | Multi-way branch |

## Symbol Definition (SYM)

Defines a symbol (label or function) that can be referenced elsewhere.

### Syntax
```
SYM symbol_name[, TYPE_PARAM0=symbol_attribute]
```

### Common Attributes
- `TYPE_PARAM0=TMP`: Symbol is local to current context
- `TYPE_PARAM0=FILE`: Symbol is visible within the file
- `TYPE_PARAM0=GLOB`: Symbol is globally visible

### Example
```
SYM loop_start               ; Define local symbol
SYM main, TYPE_PARAM0=GLOB   ; Define global symbol
```

## Branch Instructions (BR/JMP)

Unconditionally or conditionally transfers control to a target location.

### Syntax
```
BR target[, TYPE_PARAM5=condition]
JMP target[, TYPE_PARAM5=condition]  ; JMP is an alias for BR
```

### Condition Codes
- `TYPE_PARAM5=BRANCH_COND_EQ (0x00)`: Equal
- `TYPE_PARAM5=BRANCH_COND_NE (0x01)`: Not equal
- `TYPE_PARAM5=BRANCH_COND_GE (0x02)`: Greater than or equal
- `TYPE_PARAM5=BRANCH_COND_LT (0x03)`: Less than
- `TYPE_PARAM5=BRANCH_COND_GT (0x04)`: Greater than
- `TYPE_PARAM5=BRANCH_COND_LE (0x05)`: Less than or equal
- `TYPE_PARAM5=BRANCH_COND_Z  (0x06)`: Zero flag set
- `TYPE_PARAM5=BRANCH_COND_NZ (0x07)`: Zero flag not set

### Shorthand Syntax
```
BR_condition target
```

For example:
```
BR_EQ equal_branch   ; Branch if equal
BR_LT less_branch    ; Branch if less than
```

### Examples
```
BR next_section      ; Unconditional branch
BR_EQ equal_case     ; Branch if equal
```

## Function Call (CALL)

Calls a function, optionally passing parameters.

### Syntax
```
CALL target[, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...]
```

### Examples
```
CALL function                ; Call with no parameters
CALL add, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
                            ; Call with parameters using platform ABI
```

## Function Return (RET)

Returns from a function, optionally with return values.

### Syntax
```
RET [TYPE_ABICTL=ABICTL_RET=abi_name, return_value1, return_value2, ...]
```

### Examples
```
RET                         ; Return with no value
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
                           ; Return with a value using platform ABI
```

## Compare (CMP)

Compares two values and sets flags.

### Syntax
```
CMP left, right
```

### Effect
Sets various processor flags based on the comparison:
- Zero flag: Set if left == right
- Sign flag: Set if left < right (signed comparison)
- Carry flag: Set if left < right (unsigned comparison)
- Overflow flag: Set on signed overflow

### Example
```
CMP counter, limit        ; Compare counter with limit
BR_LT loop_continue       ; Branch if counter < limit
```

## Bitwise Test (TEST)

Performs a bitwise AND and sets flags without storing the result.

### Syntax
```
TEST left, right
```

### Effect
Sets flags based on the result of (left & right):
- Zero flag: Set if (left & right) == 0
- Sign flag: Set if the highest bit of (left & right) is set

### Example
```
TEST flags, 0x01         ; Test if lowest bit is set
BR_NZ bit_is_set         ; Branch if the bit is set
```

## Multi-way Branch (SWITCH)

Performs a multi-way branch based on a value.

### Syntax
```
SWITCH value, case_count
  case_value1, case_target1
  case_value2, case_target2
  ...
  default_target
```

### Example
```
SWITCH option, 3
  1, case_one
  2, case_two
  3, case_three
  default_case
```

## Common Control Flow Patterns

### If-Then-Else
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

### For Loop
```
; for (i = 0; i < limit; i++)
MOV i, 0
loop_start:
CMP i, limit
BR_GE loop_end
  ; Loop body here
INC i
BR loop_start
loop_end:
```

### While Loop
```
; while (condition)
while_start:
CMP condition, 0
BR_EQ while_end
  ; Loop body here
BR while_start
while_end:
```

### Do-While Loop
```
; do { ... } while (condition)
do_start:
  ; Loop body here
CMP condition, 0
BR_NE do_start
```

### Switch Statement
```
; switch (value) { case 1: ... break; case 2: ... break; default: ... }
SWITCH value, 2
  1, case_one
  2, case_two
  default_case

case_one:
  ; Case 1 code
BR end_switch

case_two:
  ; Case 2 code
BR end_switch

default_case:
  ; Default case code

end_switch:
```

## Binary Encoding Examples

### BR Instruction

CASM:
```
BR target
```

Binary:
```
0x02         ; Opcode for BR
0x01         ; One operand
0x9100       ; TYPE_SYM
[sym_id]     ; Symbol ID for "target"
```

### Conditional BR

CASM:
```
BR_EQ target
```

Binary:
```
0x02         ; Opcode for BR
0x02         ; Two operands
0x9100       ; TYPE_SYM
[sym_id]     ; Symbol ID for "target"
0xF000       ; TYPE_PARAM5
0x00         ; BRANCH_COND_EQ
```

### CALL with Parameters

CASM:
```
CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, a, b
```

Binary:
```
0x03         ; Opcode for CALL
0x04         ; Four operands
0x9100       ; TYPE_SYM for target
[sym_id]     ; Symbol ID for "function"
0xF800       ; TYPE_ABICTL
0x01         ; ABICTL_PARAM
[abi_id]     ; ABI ID for "platform_default"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "a"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "b"
```