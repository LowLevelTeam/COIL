# Control Flow Instructions (0x01-0x0F)

## Purpose

This document defines the control flow instructions in COIL, which manage program execution flow through branches, jumps, calls, and returns.

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x01   | SYM      | `SYM symbol[, TYPE_PARAM0]` | Define a symbol |
| 0x02   | BR       | `BR target[, TYPE_PARAM5]` | Branch to target |
| 0x03   | CALL     | `CALL target[, params...]` | Call function |
| 0x04   | RET      | `RET [return values...]` | Return from function |
| 0x05   | CMP      | `CMP left, right` | Compare values and set flags |
| 0x06   | TEST     | `TEST left, right` | Bitwise AND and set flags |
| 0x07   | JMP      | `JMP target[, TYPE_PARAM5]` | Jump (alias for BR) |
| 0x08   | LOOP     | `LOOP counter, target` | Decrement counter and branch if not zero |
| 0x09   | SWITCH   | `SWITCH value, cases..., default` | Multi-way branch |

## Detailed Descriptions

### SYM (0x01)
Define a symbol for use in the program.

#### Assembly Syntax
```
SYM symbol_name[, TYPE_PARAM0=symbol_parameter]
```

#### Binary Encoding
```
0x01                  ; Opcode for SYM
0x01/0x02             ; One or two operands
0x9100                ; TYPE_SYM
[symbol_id]           ; Symbol ID
[TYPE_PARAM0]         ; Optional symbol parameter
[parameter_value]     ; Parameter value
```

#### Parameters
Symbol parameters control visibility:
- `TYPE_PARAM0=TMP (0x00)`: Symbol is local to current context
- `TYPE_PARAM0=FILE (0x01)`: Symbol is visible within file
- `TYPE_PARAM0=GLOB (0x02)`: Symbol is globally visible

#### Example
```
; Define a global symbol 'main'
SYM main, TYPE_PARAM0=GLOB
```

### BR (0x02)
Branch to a target address.

#### Assembly Syntax
```
BR target[, TYPE_PARAM0=branch_control][, TYPE_PARAM5=condition]
```

Conditional branches can use the shorthand syntax:
```
BR_condition target
```

#### Binary Encoding
```
0x02                  ; Opcode for BR
0x01/0x02/0x03        ; Operand count
0x9100                ; TYPE_SYM (or other target type)
[target_id]           ; Target identifier
[TYPE_PARAM0]         ; Optional branch control
[control_value]       ; Control value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Conditions
The condition codes determine when the branch is taken:
- `BR_EQ`: Equal
- `BR_NE`: Not equal
- `BR_GE`: Greater than or equal
- `BR_LT`: Less than
- `BR_GT`: Greater than
- `BR_LE`: Less than or equal
- `BR_Z`: Zero flag set
- `BR_NZ`: Zero flag not set
- And others (see [instruction reference](../reference/instruction-ref.md))

#### Example
```
; Unconditional branch to loop_start
BR loop_start

; Branch to equal_branch if equal flag is set
BR_EQ equal_branch
```

### CALL (0x03)
Call a function.

#### Assembly Syntax
```
CALL target[, TYPE_ABICTL=ABICTL_PARAM=abi_name, param1, param2, ...]
```

#### Binary Encoding
```
0x03                  ; Opcode for CALL
0x01+                 ; One plus parameter count
0x9100                ; TYPE_SYM for target
[target_id]           ; Symbol ID
[TYPE_ABICTL]         ; ABI control (if using ABI)
[abi_param]           ; ABICTL_PARAM value
[abi_id]              ; ABI identifier
[param_types...]      ; Parameter types
[param_values...]     ; Parameter values
```

#### Example
```
; Call a function with parameters using the platform default ABI
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
```

### RET (0x04)
Return from a function.

#### Assembly Syntax
```
RET [TYPE_ABICTL=ABICTL_RET=abi_name, return_value1, return_value2, ...]
```

#### Binary Encoding
```
0x04                  ; Opcode for RET
0x00+                 ; Zero plus return value count
[TYPE_ABICTL]         ; ABI control (if using ABI)
[abi_ret]             ; ABICTL_RET value
[abi_id]              ; ABI identifier
[return_types...]     ; Return value types
[return_values...]    ; Return values
```

#### Example
```
; Simple return with no values
RET

; Return with a value using the platform default ABI
RET TYPE_ABICTL=ABICTL_RET=platform_default, result
```

### CMP (0x05)
Compare two values and set flags.

#### Assembly Syntax
```
CMP left, right
```

#### Binary Encoding
```
0x05                  ; Opcode for CMP
0x02                  ; Two operands
[left_type]           ; Type of left operand
[left_value]          ; Left operand value
[right_type]          ; Type of right operand
[right_value]         ; Right operand value
```

#### Example
```
; Compare counter with limit
CMP counter, limit
```

### SWITCH (0x09)
Multi-way branch based on a value.

#### Assembly Syntax
```
SWITCH value, case_count
  case_value1, case_target1
  case_value2, case_target2
  ...
  default_target
```

#### Binary Encoding
```
0x09                  ; Opcode for SWITCH
0x03+                 ; 3 + 2*case_count operands
[value_type]          ; Type of switch value
[value_value]         ; Value to switch on
0x1320                ; TYPE_UNT32+IMM for case count
[case_count]          ; Number of cases
[case1_value_type]    ; Type of case 1 value
[case1_value]         ; Case 1 value
[case1_target_type]   ; Type of case 1 target
[case1_target]        ; Case 1 target
... additional cases ...
[default_target_type] ; Type of default target
[default_target]      ; Default target
```

#### Example
```
; Switch on the value variable with 3 cases
SWITCH value, 3
  10, case_ten
  20, case_twenty
  30, case_thirty
  default_case
```

## Common Control Flow Patterns

### Conditional Execution

The most common control flow pattern in COIL uses CMP followed by a conditional branch:

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

### Loops

For loop:
```
; for (i = 0; i < n; i++)
MOV i, 0
loop_start:
CMP i, n
BR_GE loop_end
; Loop body here
ADD i, i, 1
BR loop_start
loop_end:
```

While loop:
```
; while (condition)
while_start:
CMP condition, 0
BR_EQ while_end
; Loop body here
BR while_start
while_end:
```

### Function Calls with ABI

```
; Call function with parameters
CALL calculate, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y

; Get return value
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

## Related Documentation

For more information on control flow concepts and related features, see:
- [ABI System](../concepts/abi-system.md) - ABI system for parameter passing
- [Function Examples](../examples/functions.md) - More examples of function usage
- [Binary Encoding](binary-encoding.md) - Details on binary encoding of instructions