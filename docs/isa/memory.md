# Memory Operations (0x10-0x2F)

## Purpose

This document defines the memory operations in COIL, which handle data movement, variable management, memory allocation, and direct memory access.

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x10   | MOV      | `MOV dest, src[, TYPE_PARAM5]` | Move data |
| 0x11   | PUSH     | `PUSH src[, TYPE_PARAM5]` | Push value to stack |
| 0x12   | POP      | `POP dest[, TYPE_PARAM5]` | Pop value from stack |
| 0x13   | LEA      | `LEA dest, src[, TYPE_PARAM5]` | Load effective address |
| 0x14   | SCOPEE   | `SCOPEE` | Enter variable scope |
| 0x15   | SCOPEL   | `SCOPEL` | Leave variable scope |
| 0x16   | VAR      | `VAR type, name[, initial_value]` | Declare variable |
| 0x17   | MEMCPY   | `MEMCPY dest, src, size[, TYPE_PARAM5]` | Copy memory block |
| 0x18   | MEMSET   | `MEMSET dest, value, size[, TYPE_PARAM5]` | Fill memory block |
| 0x19   | MEMCMP   | `MEMCMP first, second, size` | Compare memory blocks |
| 0x1A   | XCHG     | `XCHG first, second[, TYPE_PARAM5]` | Exchange values |
| 0x1B   | CAS      | `CAS dest, expected, new[, TYPE_PARAM0][, TYPE_PARAM5]` | Compare and swap |
| 0x2E   | PIN      | `PIN address, size` | Pin memory |
| 0x2F   | UNPIN    | `UNPIN address` | Unpin memory |

## Detailed Descriptions

### MOV (0x10)
Move data between locations.

#### Assembly Syntax
```
MOV destination, source[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x10                  ; Opcode for MOV
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (register to variable)
MOV counter, TYPE_RGP=RAX

; Binary
0x10      ; MOV
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
0x9200    ; TYPE_RGP
0x00      ; Register ID for RAX

; Assembly (memory access)
MOV [address], 42

; Binary
0x10      ; MOV
0x02      ; Two operands
0xA620    ; TYPE_PTR+IMM
[addr]    ; Memory address
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42
```

### PUSH (0x11)
Push a value onto the stack or demote a variable to memory.

#### Assembly Syntax
```
PUSH source[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x11                  ; Opcode for PUSH
0x01/0x02             ; One or two operands
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
PUSH counter

; Binary
0x11      ; PUSH
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "counter"
```

### POP (0x12)
Pop a value from the stack or promote a variable to register.

#### Assembly Syntax
```
POP destination[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x12                  ; Opcode for POP
0x01/0x02             ; One or two operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
POP result

; Binary
0x12      ; POP
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "result"
```

### VAR (0x16)
Declare a variable with optional initialization.

#### Assembly Syntax
```
VAR type, name[, initial_value]
```

#### Binary Encoding
```
0x16                  ; Opcode for VAR
0x02/0x03             ; Two or three operands
[type]                ; Variable type
[name_type]           ; TYPE_SYM for name
[name_id]             ; Symbol ID for name
[init_type]           ; Type of initial value (if present)
[init_value]          ; Initial value (if present)
```

#### Example
```
; Assembly (with initialization)
VAR TYPE_INT32, counter, 0

; Binary
0x16      ; VAR
0x03      ; Three operands
0x0300    ; TYPE_INT32
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "counter"
0x1320    ; TYPE_UNT32+IMM
0x00000000 ; Initial value 0

; Assembly (without initialization)
VAR TYPE_PTR, data_pointer

; Binary
0x16      ; VAR
0x02      ; Two operands
0xA600    ; TYPE_PTR
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "data_pointer"
```

### SCOPEE (0x14) and SCOPEL (0x15)
Enter and leave variable scopes, controlling variable lifetimes.

#### Assembly Syntax
```
SCOPEE
; ...variable declarations and code...
SCOPEL
```

#### Binary Encoding
SCOPEE:
```
0x14                  ; Opcode for SCOPEE
0x00                  ; No operands
```

SCOPEL:
```
0x15                  ; Opcode for SCOPEL
0x00                  ; No operands
```

#### Example
```
; Assembly
SCOPEE
  VAR TYPE_INT32, temp, 0
  ADD temp, temp, 5
SCOPEL

; Binary
0x14      ; SCOPEE
0x00      ; No operands
0x16      ; VAR
0x03      ; Three operands
0x0300    ; TYPE_INT32
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "temp"
0x1320    ; TYPE_UNT32+IMM
0x00000000 ; Initial value 0
0x60      ; ADD (other instructions...)
...
0x15      ; SCOPEL
0x00      ; No operands
```

### LEA (0x13)
Load the effective address of a memory operand.

#### Assembly Syntax
```
LEA destination, source[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x13                  ; Opcode for LEA
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source (memory reference)
[src_value]           ; Source address information
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
LEA ptr, [array + index*4]

; Binary
0x13      ; LEA
0x02      ; Two operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "ptr"
0xA600    ; TYPE_PTR
[complex] ; Complex address calculation
```

### MEMCPY (0x17)
Copy a block of memory.

#### Assembly Syntax
```
MEMCPY destination, source, size[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x17                  ; Opcode for MEMCPY
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[size_type]           ; Type of size
[size_value]          ; Size value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
MEMCPY dest_buffer, source_buffer, 100

; Binary
0x17      ; MEMCPY
0x03      ; Three operands
0x9000    ; TYPE_VAR (for destination)
[id]      ; Variable ID for "dest_buffer"
0x9000    ; TYPE_VAR (for source)
[id]      ; Variable ID for "source_buffer"
0x1320    ; TYPE_UNT32+IMM
0x64000000 ; Size 100
```

### MEMSET (0x18)
Fill a block of memory with a value.

#### Assembly Syntax
```
MEMSET destination, value, size[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x18                  ; Opcode for MEMSET
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[fill_type]           ; Type of fill value
[fill_value]          ; Fill value
[size_type]           ; Type of size
[size_value]          ; Size value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
MEMSET buffer, 0, 1024

; Binary
0x18      ; MEMSET
0x03      ; Three operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "buffer"
0x1020    ; TYPE_UNT8+IMM
0x00      ; Fill value 0
0x1420    ; TYPE_UNT64+IMM
[size]    ; Size 1024
```

### CAS (0x1B)
Compare and swap - atomic operation for synchronization.

#### Assembly Syntax
```
CAS destination, expected, new[, TYPE_PARAM0=MEMORY_CTRL_ATOMIC][, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x1B                  ; Opcode for CAS
0x03-0x05             ; Three to five operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[expected_type]       ; Type of expected value
[expected_value]      ; Expected value
[new_type]            ; Type of new value
[new_value]           ; New value
[TYPE_PARAM0]         ; Memory control (if present)
[atomic_value]        ; MEMORY_CTRL_ATOMIC value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
CAS [lock_ptr], expected_value, new_value, TYPE_PARAM0=MEMORY_CTRL_ATOMIC

; Binary
0x1B      ; CAS
0x04      ; Four operands
0xA600    ; TYPE_PTR
[addr]    ; Address from lock_ptr
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "expected_value"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "new_value"
0xFE00    ; TYPE_PARAM0
0x01      ; MEMORY_CTRL_ATOMIC
```

## Memory Access Patterns

### Direct Memory Access
```
; Access memory at fixed address
MOV [0x1000], 42

; Binary
0x10      ; MOV
0x02      ; Two operands
0xA620    ; TYPE_PTR+IMM
0x00100000 ; Address 0x1000
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42
```

### Register-Based Access
```
; Access memory at address in register
MOV [TYPE_RGP=RAX], 42

; Binary
0x10      ; MOV
0x02      ; Two operands
0xA600    ; TYPE_PTR
0x9200    ; TYPE_RGP
0x00      ; Register ID for RAX
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42
```

### Indexed Access
```
; Access array element: base + index*scale
MOV [array + index*4], 42

; Binary
0x10      ; MOV
0x02      ; Two operands
0xA600    ; TYPE_PTR
[complex] ; Complex address calculation
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42
```

## Variable System Example

```
; Complete variable system example
SCOPEE
  VAR TYPE_INT32, counter, 0
  VAR TYPE_INT32, max_count, 10
  
  loop_start:
    CMP counter, max_count
    BR_GE loop_end
    
    ; Body of loop
    INC counter
    BR loop_start
    
  loop_end:
SCOPEL  ; All variables destroyed here
```

## Best Practices

1. **Use variables instead of direct register access**:
   ```
   ; Preferred: Use variables
   VAR TYPE_INT32, counter, 0
   ADD counter, counter, 1
   
   ; Avoid: Direct register access
   MOV TYPE_RGP=RAX, 0
   ADD TYPE_RGP=RAX, TYPE_RGP=RAX, 1
   ```

2. **Use appropriate scoping**:
   ```
   SCOPEE
     VAR TYPE_INT32, temp, 0
     ; Use temp...
   SCOPEL  ; temp is automatically deallocated
   ```

3. **Use memory block operations for efficiency**:
   ```
   ; Efficient: Use block operations
   MEMCPY dest, src, 100
   ```

4. **Use the ABI system for function parameters**:
   ```
   ; Preferred: Let the processor handle parameters
   CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
   ```