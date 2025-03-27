# CASM Literals and Identifiers

## Identifiers

Identifiers are used for labels, variable names, and symbols.

### Rules for Identifiers

- Must begin with a letter (a-z, A-Z) or underscore (_)
- May contain letters, digits (0-9), and underscores
- Are case-sensitive (`counter` and `Counter` are distinct)
- Cannot be the same as any reserved keyword

```
valid_name    ; Valid identifier
_temp         ; Valid identifier
123abc        ; Invalid (starts with a digit)
my-var        ; Invalid (contains hyphen)
```

### Reserved Keywords

These identifiers are reserved and cannot be used as variable or label names:

- All instruction mnemonics (MOV, ADD, etc.)
- All directive names (PROC, ARCH, etc.)
- Special identifiers (TYPE_*, ABICTL_*, etc.)

## Integer Literals

CASM supports several integer formats:

### Decimal Integers

```
42              ; Decimal integer
1234567890      ; Larger decimal integer
```

### Hexadecimal Integers

Prefixed with `0x`:

```
0xFF            ; 255 in hexadecimal
0x1A3           ; 419 in hexadecimal
```

### Binary Integers

Prefixed with `0b`:

```
0b1010          ; 10 in binary
0b1111_0000     ; 240 in binary (underscores for readability)
```

### Octal Integers

Prefixed with `0o`:

```
0o777           ; 511 in octal
0o644           ; 420 in octal
```

### Integer Size

By default, integers are sized according to context. You can also explicitly specify size:

```
0xFFFF_FFFF     ; 32-bit value
0xFFFF_FFFF_FFFF_FFFF  ; 64-bit value
```

## Floating-Point Literals

CASM supports several floating-point formats:

### Decimal Floating-Point

```
3.14159         ; Standard decimal floating point
.5              ; Leading decimal point (0.5)
5.              ; Trailing decimal point (5.0)
```

### Scientific Notation

```
1.5e3           ; 1.5 × 10³ = 1500.0
2.998e8         ; 2.998 × 10⁸ = 299,800,000.0
1e-2            ; 1 × 10⁻² = 0.01
```

### Floating-Point Size

Floating-point literals are sized based on context, typically defaulting to TYPE_FP32 or TYPE_FP64.

## String Literals

### Double-Quoted Strings

```
"Hello, World"  ; Standard string
"Line 1\nLine 2"  ; String with newline escape sequence
```

### Character Literals

Single characters use single quotes:

```
'A'             ; Character 'A'
'\n'            ; Newline character
```

### Escape Sequences

Special characters use escape sequences:

| Sequence | Description |
|----------|-------------|
| \n       | Newline |
| \r       | Carriage return |
| \t       | Tab |
| \\       | Backslash |
| \"       | Double quote |
| \'       | Single quote |
| \xHH     | Hex value (2 digits) |
| \0       | Null character |

Example:
```
"Hello,\tWorld!\n"   ; Hello, [TAB] World! [NEWLINE]
"Path: C:\\Program Files\\"  ; Path: C:\Program Files\
```

## Type Literals

Type literals use the prefix `TYPE_`:

```
TYPE_INT32      ; 32-bit integer type
TYPE_FP64       ; 64-bit floating point type
TYPE_PTR        ; Pointer type
```

Type literals can have parameters:

```
TYPE_ARRAY=TYPE_UNT8   ; Array of unsigned 8-bit integers
TYPE_PTR=TYPE_INT32    ; Pointer to 32-bit integers
TYPE_V128=TYPE_FP32    ; 128-bit vector of 32-bit floats
```

## Register Literals

Register literals reference specific hardware registers:

```
TYPE_RGP=RAX    ; RAX general purpose register
TYPE_RFP=XMM0   ; XMM0 floating point register
```

Register availability depends on the architecture.

## Composite Literals

### Array Initializers

Arrays can be initialized with parenthesized lists:

```
(1, 2, 3, 4, 5)   ; Array of integers
(1.0, 2.0, 3.0)   ; Array of floats
```

### Structure Initializers

Structures can also use parenthesized lists:

```
(10, 20)        ; Initialize a struct with field values
(x=10, y=20)    ; Named field initialization (if supported)
```

## Special Value Literals

### Boolean Values

Boolean values are typically represented as integers:

```
0               ; False
1               ; True
```

### Null Pointer

```
0               ; Null pointer value
```

## Using Literals in CASM

```
; Integer literals
MOV counter, 42
ADD value, value, 0xFF

; Floating-point literals
MOV pi, 3.14159
MUL area, radius, 2.0

; String literals
VAR TYPE_ARRAY=TYPE_UNT8, message, "Hello, World!", 0

; Type literals in declarations
VAR TYPE_INT32, counter, 0
VAR TYPE_PTR=TYPE_FP32, float_array_ptr

; Register literals
MOV TYPE_RGP=RAX, 0

; Composite literals
VAR TYPE_V128=TYPE_FP32, vector, (1.0, 2.0, 3.0, 4.0)
```