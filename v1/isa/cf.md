# Control Flow Instructions (0x01-0x0F)

Control flow operations enable conditional execution and program flow management. These instructions provide the foundation for creating structured programs with functions, conditionals, and loops in COIL.

## 1. Core Instructions

### 1.1 SYM (0x01)
Define a symbol. The symbol is resolved by the linker or COIL processor into a program address.

Symbols can be defined with either no ABI or a specific ABI. When defined with an ABI, parameters and return values can be specified, which the COIL processor will use for function calls and returns.

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
SYM calculate, TYPE_PARAM0=BRANCH_CTRL_ABI
```

### 1.2 BR (0x02)
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

### 1.3 CALL (0x03)
Enter a procedure with parameters based on an ABI specification.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Call a function
CALL calculate_sum

; Conditional call (call if zero flag set)
CALL_Z handle_zero

; Call with ABI parameter passing
CALL printf, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM
```

### 1.4 RET (0x04)
Return from a procedure. When used with an ABI, it handles returning values according to the ABI specification.

```
Operands:
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Simple return
RET

; Conditional return (return if not equal)
RET_NE

; Return with ABI return value handling
RET TYPE_PARAM0=BRANCH_CTRL_ABI_RET
```

### 1.5 CMP (0x05)
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

### 1.6 TEST (0x06)
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

## 2. Extended Control Flow Instructions

### 2.1 JMP (0x07)
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

### 2.2 LOOP (0x08)
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

### 2.3 SWITCH (0x09)
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

## 3. Conditional Execution

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

## 4. Branch Control Types

The TYPE_PARAM0 parameter can specify how branches are handled:

```
BRANCH_CTRL_FAR       = 0x00  // Far jump/call
BRANCH_CTRL_INL       = 0x01  // Inline
BRANCH_CTRL_ABI       = 0x02  // Use ABI conventions
BRANCH_CTRL_ABI_PARAM = 0x03  // Following operands are parameters
BRANCH_CTRL_ABI_RET   = 0x04  // Following operands are return destinations
```

## 5. Implementation Notes

1. COIL processors are expected to optimize branch patterns where possible.
2. Conditional execution should be implemented efficiently, potentially using CPU conditional execution capabilities.
3. Branch prediction hints may be added through additional type extensions.
4. Symbol resolution is primarily the linker's responsibility, but COIL processors may perform local resolution for optimization.
5. ABI-aware instructions should follow the specified ABI conventions for the target architecture.

## 6. Relationship to Other ISA Components

Control flow instructions interact with:
- The type system for operand specifications
- The ABI system for parameter passing and return value handling
- The register system for condition flags and program counter management

## 7. Architecture-Specific Extensions

While the core control flow instructions are architecture-independent, specific architectures may provide additional functionality:

- x86: Additional condition codes (parity, etc.)
- ARM: Predicated execution, direct condition code access
- RISC-V: Compressed branch instructions, branch delay options

These extensions are available through the architecture-specific instruction range (0xC0-0xFE).