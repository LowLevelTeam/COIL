# CASM Syntax

## Purpose

This document defines the fundamental syntax rules for CASM (COIL Assembly), the human-readable text representation of COIL instructions. It serves as the authoritative guide for writing valid CASM code.

## Key Concepts

- **Line Structure**: How CASM code is organized line by line
- **Tokens**: Basic building blocks including identifiers, literals, and operators
- **Structural Elements**: Labels, sections, and scope blocks
- **Whitespace Rules**: How spacing affects the code

## Line Structure

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

### Whitespace

- Whitespace separates tokens but is otherwise insignificant
- Multiple whitespace characters are treated the same as a single one
- Indentation is recommended for readability but has no semantic meaning
- At least one whitespace character is required between an instruction and its operands

## Identifiers

Identifiers are used for labels, variable names, and symbols:

- Must begin with a letter (a-z, A-Z) or underscore (_)
- May contain letters, digits (0-9), and underscores
- Are case-sensitive (`label` and `Label` are distinct)
- Cannot be the same as any reserved keyword

```
valid_label    ; Valid identifier
_start         ; Valid identifier
123invalid     ; Invalid (starts with a digit)
$special       ; Invalid (contains special character)
```

## Literals

### Integer Literals

```
42              ; Decimal integer
0xFF            ; Hexadecimal integer (prefix 0x)
0b1010          ; Binary integer (prefix 0b)
0o777           ; Octal integer (prefix 0o)
```

### Floating-Point Literals

```
3.14159         ; Decimal floating point
1.5e3           ; Scientific notation (1500.0)
.5              ; Leading decimal point (0.5)
```

### String Literals

```
"Hello, World"  ; Double-quoted string
'A'             ; Single character
"\n\t"          ; With escape sequences
```

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

- [CASM Instructions](instructions.md) - Detailed reference of all CASM instructions
- [CASM Directives](directives.md) - Complete guide to assembly directives
- [Binary Format](../binary-format.md) - How CASM translates to binary COIL
- [Type System](../systems/type-system.md) - Details on the COIL type system