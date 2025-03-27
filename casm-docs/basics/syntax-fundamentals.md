# CASM Syntax Fundamentals

## Line Structure

Each line in CASM follows this general pattern:

```
[label:] [instruction|directive] [operands...] [; comment]
```

Each line contains at most one of:
- A single instruction or directive
- A label definition
- A comment
- A blank line

## Comments

Comments start with a semicolon (`;`) and continue to the end of the line:

```
; This is a full-line comment
MOV counter, 0  ; This is an inline comment
```

## Whitespace

- Whitespace (spaces, tabs) separates tokens
- Multiple whitespace characters are treated as a single one
- At least one whitespace character is required between an instruction and its operands
- Indentation improves readability but has no semantic meaning

```
MOV counter,0      ; Invalid (no space after comma)
MOV counter, 0     ; Valid
MOV   counter,   0 ; Valid (multiple spaces allowed)
    MOV counter, 0 ; Valid (indentation allowed)
```

## Labels

Labels identify locations in the code and end with a colon (`:`):

```
loop_start:        ; Define a label
    ADD counter, counter, 1
    CMP counter, 10
    BR_LT loop_start  ; Reference the label (no colon when referenced)
```

Label requirements:
- Begin with a letter or underscore
- Contain only letters, digits, and underscores
- Be unique within their scope
- End with a colon when defined

## Symbols

Symbols are named entities defined using the `SYM` directive:

```
SYM function_name      ; Define a local symbol
SYM main, TYPE_PARAM0=GLOB  ; Define a global symbol
```

Unlike labels, symbols can have additional attributes and are more versatile.

## Instructions

Instructions have a mnemonic followed by operands:

```
OPCODE [operand1[, operand2[, ...]]]
```

Examples:
```
MOV dest, src           ; Move src to dest
ADD result, a, b        ; Add a and b, store in result
```

Conditional variants use an underscore and condition suffix:
```
BR_EQ label             ; Branch if equal
MOV_NZ a, b             ; Move if not zero
```

## Operands

Operands can be in various forms:

### Immediate Values

Numeric literals:
```
MOV counter, 42         ; Decimal integer
ADD total, total, 0xFF  ; Hexadecimal integer (0x prefix)
MOV pi, 3.14159         ; Floating point
```

### Variables

Named references:
```
MOV counter, value      ; counter and value are variables
```

### Registers

Direct register references:
```
MOV TYPE_RGP=RAX, 42    ; Direct reference to RAX register
```

### Memory References

Using square brackets:
```
MOV [address], 42                  ; Store to memory
MOV value, [address]               ; Load from memory
MOV value, [base_ptr + 8]          ; Base + offset addressing
MOV value, [array + index*4]       ; Base + scaled index
```

### Type References

Using the `TYPE_` prefix:
```
VAR TYPE_INT32, counter  ; 32-bit integer type
```

## Directives

Directives control the assembly process:

```
PROC 0x01                     ; Set processor type to CPU
ARCH 0x01, 0x03               ; Set architecture to x86-64
SECTION .text, 0x01 | 0x04    ; Define a code section
ALIGN 16                      ; Align to 16-byte boundary
```

## Scopes

Scopes define variable lifetimes:

```
SCOPEE                        ; Begin scope
    VAR TYPE_INT32, temp, 0   ; Variable exists in this scope
    ; Operations using temp...
SCOPEL                        ; End scope, temp is destroyed
```

## Complete Example

Here's a complete example showing many syntax elements:

```
; Example CASM program
PROC 0x01                     ; CPU
ARCH 0x01, 0x03               ; x86-64

SECTION .data, 0x02 | 0x04 | 0x08
SYM message
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

SECTION .text, 0x01 | 0x04
SYM _start, TYPE_PARAM0=GLOB
    SCOPEE
    VAR TYPE_PTR, msg_ptr, message
    VAR TYPE_INT32, counter, 0
    
    ; Loop to print message
loop_start:
    CMP counter, 5
    BR_GE loop_end
    
    ; Print message (simplified)
    ; ... output code would go here ...
    
    INC counter
    BR loop_start
    
loop_end:
    RET
    SCOPEL
```

## Common Syntax Errors

1. **Missing whitespace**:
   ```
   MOVcounter,0        ; Invalid - no space after MOV
   MOV counter,0       ; Invalid - no space after comma
   ```

2. **Incorrect label syntax**:
   ```
   loop                ; Invalid - missing colon for definition
   BR loop:            ; Invalid - colon on reference
   ```

3. **Wrong operand order**:
   ```
   MOV 42, counter     ; Probably wrong - immediate to variable
   ```

4. **Invalid identifiers**:
   ```
   VAR TYPE_INT32, 123var  ; Invalid - starts with digit
   ```

5. **Incorrect type references**:
   ```
   VAR INT32, counter      ; Invalid - missing TYPE_ prefix
   ```