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
; Assembly
SYM main, TYPE_PARAM0=GLOB

; Binary
0x01      ; SYM
0x02      ; Two operands
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "main"
0xFE00    ; TYPE_PARAM0
0x02      ; GLOB
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

#### Parameters
- Branch control options:
  - `TYPE_PARAM0=BRANCH_CTRL_FAR (0x00)`: Far branch
  - `TYPE_PARAM0=BRANCH_CTRL_INL (0x01)`: Inline
  - `TYPE_PARAM0=BRANCH_CTRL_ABI (0x02)`: Use ABI

- Condition codes:
  - `TYPE_PARAM5=BRANCH_COND_EQ (0x00)`: Equal
  - `TYPE_PARAM5=BRANCH_COND_NE (0x01)`: Not equal
  - `TYPE_PARAM5=BRANCH_COND_GE (0x02)`: Greater or equal
  - `TYPE_PARAM5=BRANCH_COND_LT (0x03)`: Less than
  - `TYPE_PARAM5=BRANCH_COND_GT (0x04)`: Greater than
  - `TYPE_PARAM5=BRANCH_COND_LE (0x05)`: Less than or equal
  - `TYPE_PARAM5=BRANCH_COND_Z  (0x06)`: Zero
  - `TYPE_PARAM5=BRANCH_COND_NZ (0x07)`: Not zero

#### Example
```
; Assembly (unconditional)
BR loop_start

; Binary
0x02      ; BR
0x01      ; One operand
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "loop_start"

; Assembly (conditional)
BR_EQ equal_branch

; Binary
0x02      ; BR
0x02      ; Two operands
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "equal_branch"
0xF000    ; TYPE_PARAM5
0x00      ; BRANCH_COND_EQ
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

#### Parameters
- ABI control options:
  - `TYPE_ABICTL=ABICTL_PARAM=abi_name`: Use specified ABI for parameters

#### Example
```
; Assembly
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y

; Binary
0x03      ; CALL
0x04      ; Four operands (target + ABI + 2 params)
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "calculate_sum"
0xF800    ; TYPE_ABICTL
0x01      ; ABICTL_PARAM
[abi_id]  ; ABI ID
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "x"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "y"
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

#### Parameters
- ABI control options:
  - `TYPE_ABICTL=ABICTL_RET=abi_name`: Use specified ABI for return values

#### Example
```
; Assembly (simple return)
RET

; Binary
0x04      ; RET
0x00      ; No operands

; Assembly (return with value)
RET TYPE_ABICTL=ABICTL_RET=platform_default, result

; Binary
0x04      ; RET
0x03      ; Three operands
0xF800    ; TYPE_ABICTL
0x02      ; ABICTL_RET
[abi_id]  ; ABI ID
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
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
; Assembly
CMP counter, limit

; Binary
0x05      ; CMP
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "limit"
```

### TEST (0x06)
Perform bitwise AND and set flags without storing the result.

#### Assembly Syntax
```
TEST left, right
```

#### Binary Encoding
```
0x06                  ; Opcode for TEST
0x02                  ; Two operands
[left_type]           ; Type of left operand
[left_value]          ; Left operand value
[right_type]          ; Type of right operand
[right_value]         ; Right operand value
```

#### Example
```
; Assembly
TEST flags, 0x01

; Binary
0x06      ; TEST
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "flags"
0x1320    ; TYPE_UNT32+IMM
0x01000000 ; Value 0x01
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
; Assembly
SWITCH value, 2
  10, case_ten
  20, case_twenty
  default_case

; Binary
0x09      ; SWITCH
0x07      ; 7 operands (value + count + 2*2 case pairs + default)
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "value"
0x1320    ; TYPE_UNT32+IMM
0x02000000 ; 2 cases
0x1320    ; TYPE_UNT32+IMM
0x0A000000 ; Case value 10
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "case_ten"
0x1320    ; TYPE_UNT32+IMM
0x14000000 ; Case value 20
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "case_twenty"
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "default_case"
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