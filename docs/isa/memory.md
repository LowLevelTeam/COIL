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
; Copy value from variable to register
MOV TYPE_RGP=RAX, counter

; Copy immediate value to variable
MOV counter, 42

; Copy memory value to variable
MOV value, [address]
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
; Define an integer variable with initialization
VAR TYPE_INT32, counter, 0

; Define a pointer variable without initialization
VAR TYPE_PTR, data_pointer

; Define a floating-point variable
VAR TYPE_FP32, pi, 3.14159
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
SCOPEE
  ; Variables exist from here...
  VAR TYPE_INT32, temp, 0
  ADD temp, temp, 5
  
  ; Nested scope
  SCOPEE
    VAR TYPE_INT32, nested_var, 10
    ; Both temp and nested_var are accessible here
  SCOPEL  ; nested_var is destroyed here
  
  ; Only temp exists here
SCOPEL  ; temp is destroyed here
```

### PUSH (0x11) and POP (0x12)
Push/pop values to/from the stack or demote/promote variables.

#### Assembly Syntax
```
PUSH source[, TYPE_PARAM5=condition]
POP destination[, TYPE_PARAM5=condition]
```

#### Binary Encoding
PUSH:
```
0x11                  ; Opcode for PUSH
0x01/0x02             ; One or two operands
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

POP:
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
; Stack operations
PUSH value            ; Push value to stack
POP result            ; Pop from stack to result

; Variable optimization hints
PUSH counter          ; Suggest storing counter in memory
POP counter           ; Suggest loading counter into register
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
; Load address of array element
LEA ptr, [array + index*4]

; Calculate address without accessing memory
LEA result, [base + offset]
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
; Copy 100 bytes from source_buffer to dest_buffer
MEMCPY dest_buffer, source_buffer, 100
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
; Atomic compare and swap for lock acquisition
MOV TYPE_RGP=RAX, 0          ; Expected: unlocked (0)
MOV TYPE_RGP=RDX, 1          ; New: locked (1)
CAS [lock_address], TYPE_RGP=RAX, TYPE_RGP=RDX, TYPE_PARAM0=MEMORY_CTRL_ATOMIC
```

## Memory Access Patterns

### Direct Memory Access
```
; Access memory at fixed address
MOV [0x1000], 42

; Access memory at address in register
MOV [TYPE_RGP=RAX], 42

; Access memory with offset
MOV [base_ptr + 8], value
```

### Indexed Access
```
; Array element access: base + index*scale
MOV element, [array + index*4]

; Structure field access
MOV field, [struct_ptr + offset]
```

### Memory Block Operations
```
; Fill a block with a value
MEMSET buffer, 0, 1024       ; Zero out 1KiB of memory

; Copy a block
MEMCPY dest, src, size

; Compare memory blocks
MEMCMP result, block1, block2, size
```

## Variable System Integration

The memory operations work closely with COIL's variable system:

1. **Variable Declaration**:
   - `VAR` creates variables with proper types
   - Variables can be initialized during declaration

2. **Scope Management**:
   - `SCOPEE`/`SCOPEL` manage variable lifetimes
   - When a scope ends, all its variables are released

3. **Variable Access**:
   - Variables are used directly in instructions
   - The COIL processor translates variable references to the appropriate register or memory access

4. **Variable Optimization**:
   - `PUSH`/`POP` provide hints for variable storage optimization
   - Frequently used variables can be promoted to registers
   - Infrequently used variables can be demoted to memory

## Related Documentation

For more information on memory concepts and related features, see:
- [Memory Model](../concepts/memory-model.md) - Memory organization and rules
- [Variable System](../concepts/variable-system.md) - Details on variable management
- [Type Operations](type.md) - Instructions for working with typed memory