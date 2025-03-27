# CASM Assembly Syntax

## Purpose

This document defines the fundamental syntax rules for CASM (COIL Assembly), which is the human-readable text representation of COIL instructions. It serves as the authoritative guide for writing valid CASM code.

## Lexical Elements

### Line Structure

Each line in CASM contains at most one of the following:
- A single instruction or directive
- A label definition
- A comment
- A blank line

Lines follow this general pattern:
```
[label:] [instruction|directive] [operands...] [; comment]
```

### Comments

Comments start with a semicolon (`;`) and continue to the end of the line:

```
; This is a full-line comment
MOV counter, 0  ; This is an inline comment
```

Comments are ignored by the assembler and are purely for documentation purposes.

### Whitespace

- Whitespace (spaces, tabs) is used to separate tokens but is otherwise insignificant
- Multiple whitespace characters are treated the same as a single whitespace
- Indentation is recommended for readability but has no semantic meaning
- At least one whitespace character is required between an instruction and its operands

### Identifiers

Identifiers are used for labels, variable names, and symbols:

- Must begin with a letter (a-z, A-Z) or underscore (_)
- May contain letters, digits (0-9), and underscores
- Are case-sensitive (`label` and `Label` are distinct)
- Cannot be the same as any reserved keyword (instruction opcodes, directives, etc.)

```
valid_label    ; Valid identifier
_start         ; Valid identifier
123invalid     ; Invalid (starts with a digit)
$special       ; Invalid (contains special character)
```

### Literals

#### Integer Literals

```
42              ; Decimal integer
0xFF            ; Hexadecimal integer (prefix 0x)
0b1010          ; Binary integer (prefix 0b)
0o777           ; Octal integer (prefix 0o)
```

#### Floating-Point Literals

```
3.14159         ; Decimal floating point
1.5e3           ; Scientific notation (1500.0)
.5              ; Leading decimal point (0.5)
```

#### String Literals

```
"Hello, World"  ; Double-quoted string
'A'             ; Single character
"\n\t"          ; With escape sequences
```

#### Character Escape Sequences

- `\n` - Newline
- `\r` - Carriage return
- `\t` - Tab
- `\0` - Null character
- `\\` - Backslash
- `\"` - Double quote
- `\'` - Single quote
- `\xHH` - Hexadecimal value (HH is a two-digit hex number)

## Structural Elements

### Labels

Labels identify locations in the code and end with a colon (`:`):

```
loop_start:     ; Define a label
    ADD counter, counter, 1
    CMP counter, 10
    BR_LT loop_start  ; Reference the label
```

Labels must:
- Begin with a letter or underscore
- Contain only letters, numbers, and underscores
- Be unique within their scope
- End with a colon when defined (but not when referenced)

### Sections

Code is organized into sections using the `SECTION` directive:

```
SECTION .text, 0x01 | 0x04   ; Executable and readable section
SECTION .data, 0x02 | 0x04   ; Writable and readable section
```

## Instructions

Instructions follow this format:

```
OPCODE [operand1[, operand2[, ...]]]
```

Where:
- `OPCODE` is an instruction mnemonic (e.g., `MOV`, `ADD`)
- Operands are separated by commas
- Operands can be variables, constants, types, or memory references

### Basic Instruction Format

```
MOV dest, src           ; Move src to dest
ADD result, a, b        ; Add a and b, store in result
```

### Conditional Instructions

Conditional instructions use an underscore and condition suffix:

```
BR_EQ label     ; Branch if equal
MOV_NZ a, b     ; Move if not zero
```

## Operands

### Immediate Values

Numeric literals used directly as operands:
```
MOV counter, 42         ; 42 is an immediate value
ADD total, total, 0xFF  ; 0xFF (255) is an immediate value
```

### Variable References

Variables are referenced by name:
```
MOV counter, value      ; counter and value are variable references
```

### Register References

Direct register references use the `TYPE_RGP=` syntax:
```
MOV TYPE_RGP=RAX, 42    ; Direct reference to RAX register
```

### Memory References

Memory references use square brackets:
```
MOV [address], 42                     ; Direct memory reference
MOV [TYPE_RGP=RAX], 42                ; Memory at address in RAX
MOV [base + offset], value            ; Base + Offset addressing
MOV [base + index*scale], value       ; Base + Index*Scale addressing
MOV [base + index*scale + offset], 42 ; Full addressing mode
```

### Type References

Type references use the `TYPE_` prefix:
```
VAR TYPE_INT32, counter  ; 32-bit signed integer type
```

## Type Specifications

Type specifications follow two patterns:

### Simple Type

```
TYPE_INT32      ; 32-bit signed integer
TYPE_FP64       ; 64-bit floating point
```

### Parameterized Type

#### Single Parameter Syntax
```
TYPE_RGP=RAX    ; Register type with RAX as the register
TYPE_ARRAY=TYPE_UNT8  ; Array of unsigned 8-bit integers
```

#### Double Parameter Syntax
```
TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64  ; ABI control with specific ABI
TYPE_PTR=TYPE_INT32=0x1000  ; Pointer to INT32 at address 0x1000
```

## Variable Declaration

Variables are declared using the `VAR` instruction:

```
VAR type, name[, initial_value]
```

Examples:
```
VAR TYPE_INT32, counter, 0       ; Integer with initialization
VAR TYPE_PTR, data_pointer       ; Pointer without initialization
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"  ; Array initialization
```

## Scope Blocks

Scope blocks are defined with `SCOPEE` and `SCOPEL`:

```
SCOPEE
    ; Variables and code in this scope
    VAR TYPE_INT32, local_var, 0
    
    ; Nested scope
    SCOPEE
        VAR TYPE_INT32, inner_var, 0
    SCOPEL  ; inner_var destroyed here
    
SCOPEL  ; local_var destroyed here
```

## Directives

Directives control the assembler and begin with an uppercase letter:

```
PROC 0x01       ; Set processor type to CPU
ARCH 0x01, 0x03 ; Set architecture to x86-64
SECTION .text, 0x01 | 0x04  ; Define text section
```

Common directives include:
- `PROC` - Set processor type
- `ARCH` - Set architecture
- `SECTION` - Define a section
- `DATA` - Insert data
- `ALIGN` - Align to boundary
- `INCLUDE` - Include another file

## ABI Blocks

ABI definitions are enclosed in `ABI` and `EXIT` directives:

```
ABI custom_abi
    PARAMS RDI, RSI, RDX
    RETS RAX
    CALLER RAX, RCX, RDX
    CALLEE RBX, RSP, RBP
    SALLIGN 16
    RZONE 0
EXIT
```

## Conditional Assembly

Conditional assembly uses `IF`, `ELIF`, `ELSE`, and `ENDIF`:

```
IF ARCH == 0x01
    ; x86-specific code
ELIF ARCH == 0x02
    ; ARM-specific code
ELSE
    ; Generic code
ENDIF
```

## Complete Example

Here's a complete example showing many syntax elements:

```
; Example CASM program
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .data, 0x02 | 0x04 | 0x08
SYM message
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 0

SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_PTR, msg_ptr, message
    VAR TYPE_INT, result, 0
    
    ; Compare something
    CMP TYPE_RGP=RAX, 0
    BR_EQ zero_case
    
    ; Non-zero case
    MOV result, 1
    BR end_func
    
zero_case:
    MOV result, 0
    
end_func:
    RET TYPE_ABICTL=ABICTL_RET=platform_default, result
    SCOPEL
```

## Related Documentation

- [CASM Instructions](casm-instructions.md) - Detailed description of all CASM instructions
- [CASM Directives](casm-directives.md) - Comprehensive coverage of assembly directives
- [Binary Encoding](binary-encoding.md) - How CASM translates to binary COIL
- [Type System](type-system.md) - Details on the type system used in CASM