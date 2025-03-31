# CASM Quick Reference

## Basic Syntax

```
[label:] [instruction|directive] [operands...] [; comment]
```

### Comments
```
; This is a full-line comment
MOV counter, 0  ; This is an inline comment
```

### Labels
```
loop_start:     ; Define a label
    ; Code
    BR loop_start  ; Reference the label
```

### Symbols
```
SYM function_name      ; Define a local symbol
SYM main, TYPE_PARAM0=GLOB  ; Define a global symbol
```

## Sections

```
SECTION .text, 0x01 | 0x04    ; Executable and readable
SECTION .data, 0x02 | 0x04 | 0x08  ; Writable, readable, initialized
SECTION .bss, 0x02 | 0x04 | 0x10   ; Writable, readable, uninitialized
```

## Variables

```
VAR #1, TYPE_INT32, 0       ; Integer with initialization
VAR #2, TYPE_PTR            ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159  ; Floating-point with initialization

; Using variables
ADD #1, #1, 1
MUL #4, #2, #3
```

## Scopes

```
SCOPEE
    ; Variables declared here exist within this scope
    VAR #1, TYPE_INT32, 0
    ; Operations using variable #1
SCOPEL  ; variable #1 is destroyed here
```

## Types

```
TYPE_INT8    ; 8-bit signed integer
TYPE_UNT32   ; 32-bit unsigned integer
TYPE_FP64    ; 64-bit floating point
TYPE_PTR     ; Generic pointer
TYPE_V128=TYPE_FP32  ; 128-bit vector of 32-bit floats
```

## Memory Access

```
MOV [address], 42         ; Store to memory at 'address'
MOV value, [address]      ; Load from memory at 'address'
MOV [base_ptr + 8], value ; Store with offset
MOV [array + index*4], value  ; Store with scaled index
```

## Control Flow

### Branches

```
BR target               ; Unconditional branch
BR_EQ equal_case        ; Branch if equal
BR_LT less_than_case    ; Branch if less than
```

### Conditionals

```
; if (#1 > #2) then X else Y
CMP #1, #2
BR_LE else_branch
  ; X code here
BR end_if
else_branch:
  ; Y code here
end_if:
```

### Loops

```
; for (#1 = 0; #1 < #2; #1++)
MOV #1, 0
loop_start:
CMP #1, #2
BR_GE loop_end
  ; Loop body here
INC #1
BR loop_start
loop_end:
```

### Switch

```
SWITCH #1, 3
  1, case_one
  2, case_two
  3, case_three
  default_case
```

## Functions

### Definition

```
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
    SCOPEE
    ; Get parameters
    VAR #1, TYPE_INT32
    VAR #2, TYPE_INT32
    MOV #1, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    MOV #2, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
    
    ; Calculate result
    VAR #3, TYPE_INT32
    ADD #3, #1, #2
    
    ; Return result
    RET TYPE_ABICTL=ABICTL_RET=platform_default, #3
    SCOPEL
```

### Calling

```
; Call function with parameters
CALL add_numbers, TYPE_ABICTL=ABICTL_PARAM=platform_default, #1, #2

; Get return value
MOV #3, TYPE_ABICTL=ABICTL_RET=platform_default
```

## Arithmetic

```
ADD result, a, b          ; Addition
SUB result, a, b          ; Subtraction
MUL result, a, b          ; Multiplication
DIV result, a, b          ; Division
MOD result, a, b          ; Modulo
INC counter               ; Increment
DEC counter               ; Decrement
NEG result, a             ; Negate
ABS result, a             ; Absolute value
```

## Bitwise Operations

```
AND result, a, b          ; Bitwise AND
OR result, a, b           ; Bitwise OR
XOR result, a, b          ; Bitwise XOR
NOT result, a             ; Bitwise NOT
SHL result, a, count      ; Shift left
SHR result, a, count      ; Shift right logical
```

## Vector Operations

```
; Initialize vector
VAR TYPE_V128=TYPE_FP32, vector, (1.0, 2.0, 3.0, 4.0)

; Vector addition
ADD result, vector1, vector2

; Dot product
VDOT result, vector1, vector2
```

## Data Initialization

```
; String data
DATA TYPE_ARRAY=TYPE_UNT8, "Hello, World!", 10, 0

; Integer array
DATA TYPE_ARRAY=TYPE_INT32, (1, 2, 3, 4, 5)

; Floating-point value
DATA TYPE_FP64, 3.14159265358979
```

## Processor & Architecture

```
PROC 0x01                     ; CPU processor
ARCH 0x01, 0x03               ; x86-64 architecture
```

## External Symbols

```
EXTERN printf, TYPE_ABICTL=ABICTL_STANDARD=platform_default

CALL printf, TYPE_ABICTL=ABICTL_PARAM=platform_default, format_str, arg1, arg2
```

## ABI Definition

```
ABI my_custom_abi
  PARAMS RDI, RSI, RDX      ; Parameter registers
  RETS RAX                  ; Return value register
  CALLER RAX, RCX, RDX      ; Caller-saved registers
  CALLEE RBX, RSP, RBP      ; Callee-saved registers
  SALLIGN 16                ; Stack alignment
  RZONE 0                   ; No red zone
EXIT
```

## Conditional Assembly

```
IF ARCH == 0x01
  ; x86-specific code
ELIF ARCH == 0x02
  ; ARM-specific code
ELSE
  ; Generic code
ENDIF
```

## Literals

```
42              ; Decimal integer
0xFF            ; Hexadecimal integer (255)
0b1010          ; Binary integer (10)
3.14159         ; Floating-point number
"Hello"         ; String
'A'             ; Character
```

## Register Access

```
MOV TYPE_RGP=RAX, 42     ; Direct register access
MOV TYPE_RFP=XMM0, #1    ; Access floating-point register
```

## Memory Operations

```
MEMCPY dest_ptr, src_ptr, byte_count    ; Copy memory
MEMSET buffer, 0, size                  ; Fill memory
MEMCMP result, buffer1, buffer2, size   ; Compare memory
```

## Type Conversion

```
CONVERT float_result, int_value    ; Convert integer to float
CAST int_bits, float_value         ; Reinterpret bit pattern
```