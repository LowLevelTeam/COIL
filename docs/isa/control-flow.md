# Control Flow Instructions (0x01-0x0F)

## Purpose

This document defines the control flow instructions in COIL, which enable conditional execution, branching, function calls, and program flow management. These instructions provide the foundation for creating structured programs with functions, conditionals, and loops.

## Instruction Details

### SYM (0x01)
Define a symbol. The symbol is resolved by the linker or COIL processor into a program address.

Symbols can be defined with various attributes including ABI specifications, which the COIL processor will use for function calls and returns.

```
Operands:
- Symbol: TYPE_SYM
- TYPE_PARAM0: symbol_parameter0_t (optional)
```

Example:
```
; Define a global symbol 'main'
SYM main, TYPE_PARAM0=GLOB

; Define a function with ABI
SYM calculate, TYPE_ABICTL=ABICTL_STANDARD=platform_default
```

Binary encoding:
```
0x01      ; Opcode for SYM
0x01      ; One operand (or 0x02 if TYPE_PARAM0 included)
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
```

### BR (0x02)
Branch to a program address. This is the fundamental control flow instruction in COIL.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Unconditional branch
BR loop_start

; Conditional branch (branch if equal)
BR_EQ equal_handler

; Branch with control parameter
BR function_ptr, TYPE_PARAM0=BRANCH_CTRL_FAR
```

Binary encoding:
```
0x02      ; Opcode for BR
0x01      ; One operand (or more if parameters included)
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
```

With condition:
```
0x02      ; Opcode for BR
0x02      ; Two operands
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
0xF000    ; TYPE_PARAM5
0x0000    ; BRANCH_COND_EQ
```

### CALL (0x03)
Enter a procedure with parameters based on an ABI specification.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
- Additional operands for parameters when using TYPE_ABICTL=ABICTL_PARAM
```

Example:
```
; Call a function
CALL calculate_sum

; Conditional call (call if zero flag set)
CALL_Z handle_zero

; Call with ABI parameter passing
CALL printf, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, format, value
```

Binary encoding:
```
0x03      ; Opcode for CALL
0x01      ; One operand (or more for parameters/conditions)
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
```

With ABI parameters:
```
0x03      ; Opcode for CALL
0x03+     ; Three or more operands (target, ABI, parameters)
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
0xF800    ; TYPE_ABICTL
0x0100    ; ABICTL_PARAM
[abi_id]  ; ABI ID
0x9000    ; TYPE_VAR for parameter
[var_id]  ; Variable ID
; ... additional parameters ...
```

### RET (0x04)
Return from a procedure. When used with an ABI, it handles returning values according to the ABI specification.

```
Operands:
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
- Return values when using TYPE_ABICTL=ABICTL_RET
```

Example:
```
; Simple return
RET

; Conditional return (return if not equal)
RET_NE

; Return with ABI return value handling
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
```

Binary encoding:
```
0x04      ; Opcode for RET
0x00      ; Zero operands
```

With return value:
```
0x04      ; Opcode for RET
0x02+     ; Two or more operands (ABI, return values)
0xF800    ; TYPE_ABICTL
0x0200    ; ABICTL_RET
[abi_id]  ; ABI ID
0x9000    ; TYPE_VAR for return value
[var_id]  ; Variable ID
; ... additional return values ...
```

### CMP (0x05)
Compare values and set flags without changing registers. This is the primary instruction for setting up conditional branches.

```
Operands:
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
```

Example:
```
; Compare register to immediate value
CMP TYPE_RGP=RAX, 0

; Compare two variables
CMP count, limit

; Compare memory values
CMP [address1], [address2]
```

Binary encoding:
```
0x05      ; Opcode for CMP
0x02      ; Two operands
[type1]   ; Type of left operand
[value1]  ; Value of left operand
[type2]   ; Type of right operand
[value2]  ; Value of right operand
```

### TEST (0x06)
Perform a bitwise AND and set flags without changing registers. Useful for testing individual bits or bit masks.

```
Operands:
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
```

Example:
```
; Test if the lowest bit is set
TEST TYPE_RGP=RAX, 0x01

; Test if any bits in a mask are set
TEST flags, FLAG_ACTIVE | FLAG_READY
```

Binary encoding:
```
0x06      ; Opcode for TEST
0x02      ; Two operands
[type1]   ; Type of left operand
[value1]  ; Value of left operand
[type2]   ; Type of right operand
[value2]  ; Value of right operand
```

### JMP (0x07)
Alias for BR with improved semantics for jumps within the same context.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Unconditional jump
JMP loop_start

; Conditional jump
JMP_GT greater_handler
```

Binary encoding:
```
0x07      ; Opcode for JMP
0x01      ; One operand (or more with condition)
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID
```

### LOOP (0x08)
Decrement counter and branch if not zero. Optimized for loop constructs.

```
Operands:
- Counter: TYPE_RGP | TYPE_VAR (counter to decrement)
- Target: TYPE_PTR | TYPE_SYM (branch target)
```

Example:
```
; Loop using ECX as counter
LOOP TYPE_RGP=ECX, loop_start

; Loop using a variable
LOOP counter, loop_start
```

Binary encoding:
```
0x08      ; Opcode for LOOP
0x02      ; Two operands
[type1]   ; Type of counter
[value1]  ; Value of counter
0x9100    ; TYPE_SYM for target
[sym_id]  ; Symbol ID
```

### SWITCH (0x09)
Multi-way branch based on a value. Efficiently implements switch/case statements.

```
Operands:
- Value: Non TYPE_VOID
- CaseCount: TYPE_UNT8 (number of cases)
- Case1Value: Non TYPE_VOID
- Case1Target: TYPE_PTR | TYPE_SYM
- ...
- DefaultTarget: TYPE_PTR | TYPE_SYM (optional)
```

Example:
```
; Switch on a value
SWITCH TYPE_RGP=RAX, 3
  10, case_ten
  20, case_twenty
  30, case_thirty
  default_case
```

Binary encoding:
```
0x09      ; Opcode for SWITCH
0x3+      ; Variable number of operands
[type1]   ; Type of value
[value1]  ; Value to switch on
0x1020    ; TYPE_UNT8+IMM for case count
0x03      ; 3 cases
; ... case values and targets ...
0x9100    ; TYPE_SYM for default target
[sym_id]  ; Symbol ID
```

## Conditional Execution

Most control flow instructions support conditional execution based on CPU flags. The condition is specified using the TYPE_PARAM5 parameter with a branch_condition_t value:

```
BRANCH_COND_EQ = 0x00  // Equal
BRANCH_COND_NE = 0x01  // Not equal
BRANCH_COND_GE = 0x02  // Greater than or equal
BRANCH_COND_LT = 0x03  // Less than
BRANCH_COND_GT = 0x04  // Greater than
BRANCH_COND_LE = 0x05  // Less than or equal
BRANCH_COND_Z  = 0x06  // Zero flag set
BRANCH_COND_NZ = 0x07  // Zero flag not set
BRANCH_COND_C  = 0x08  // Carry flag set
BRANCH_COND_NC = 0x09  // Carry flag not set
BRANCH_COND_O  = 0x0A  // Overflow flag set
BRANCH_COND_NO = 0x0B  // Overflow flag not set
BRANCH_COND_S  = 0x0C  // Sign flag set
BRANCH_COND_NS = 0x0D  // Sign flag not set
```

As a convenience, these conditions can be appended to the instruction name with an underscore:

```
BR_EQ  ; Branch if equal
CALL_Z ; Call if zero
RET_GE ; Return if greater or equal
```

## Branch Control Types

The TYPE_PARAM0 parameter can specify how branches are handled:

```
BRANCH_CTRL_FAR       = 0x00  // Far jump/call (different code segment)
BRANCH_CTRL_INL       = 0x01  // Inline the called function
BRANCH_CTRL_ABI       = 0x02  // Use ABI conventions
BRANCH_CTRL_ABI_PARAM = 0x03  // Following operands are parameters
BRANCH_CTRL_ABI_RET   = 0x04  // Following operands are return destinations
```

## Symbol Parameters

When defining symbols with SYM, the TYPE_PARAM0 parameter can specify symbol attributes:

```
TMP  = 0x00  // Symbol is used only in current context
FILE = 0x01  // Symbol is used within the current file
GLOB = 0x02  // Symbol is globally visible
```

## Common Control Flow Patterns

### Conditional Statement (if-then-else)

```
; if (a == b) then X else Y
CMP a, b
BR_NE else_branch
; X code here
BR end_if
SYM else_branch
; Y code here
SYM end_if
```

### Loop Constructs

For loop:
```
; for (i = 0; i < n; i++)
MOV i, 0
SYM loop_start
CMP i, n
BR_GE loop_end
; Loop body here
ADD i, i, 1
BR loop_start
SYM loop_end
```

While loop:
```
; while (condition)
SYM while_start
CMP condition, 0
BR_EQ while_end
; Loop body here
BR while_start
SYM while_end
```

Do-while loop:
```
; do { ... } while (condition)
SYM do_start
; Loop body here
CMP condition, 0
BR_NE do_start
```

### Function Call with ABI Parameters

```
; Call a function with two parameters
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y

; Get the return value
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

### Switch Statement

```
; switch (value)
SWITCH value, 4
  1, case_one
  2, case_two
  5, case_five
  10, case_ten
  default_case
```

## Processor Adaptations

While control flow instructions are universal across all processor types, their implementation may vary:

1. COIL processors automatically adapt the instructions to the most efficient form for the target processor
2. Processors with predicated execution may optimize conditional branches
3. Processors with branch prediction features will apply appropriate hints
4. The ABI system adapts function calls to the target processor's conventions

This allows control flow code to be portable while still achieving optimal performance on each processor type.