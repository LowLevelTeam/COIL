# COIL Memory Operations Reference

## Core Memory Instructions

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x10   | MOV      | Move data between locations |
| 0x11   | PUSH     | Push value to stack |
| 0x12   | POP      | Pop value from stack |
| 0x13   | LEA      | Load effective address |
| 0x14   | SCOPEE   | Enter variable scope |
| 0x15   | SCOPEL   | Leave variable scope |
| 0x16   | VAR      | Declare variable |
| 0x17   | MEMCPY   | Copy memory block |
| 0x18   | MEMSET   | Fill memory block |
| 0x19   | MEMCMP   | Compare memory blocks |
| 0x1A   | XCHG     | Exchange values |
| 0x1B   | CAS      | Compare and swap |

## Data Movement (MOV)

Transfers data between locations.

### Syntax
```
MOV destination, source[, TYPE_PARAM5=condition]
```

### Examples
```
MOV counter, 42               ; Store immediate value to variable
MOV result, counter           ; Copy variable to variable
MOV [address], value          ; Store to memory
MOV value, [address]          ; Load from memory
MOV TYPE_RGP=RAX, value       ; Store to register
```

## Stack Operations (PUSH/POP)

Manage the stack or provide variable promotion/demotion hints.

### Syntax
```
PUSH source[, TYPE_PARAM5=condition]
POP destination[, TYPE_PARAM5=condition]
```

### Examples
```
PUSH value                    ; Push value to stack
POP result                    ; Pop from stack to result
PUSH counter                  ; Suggest storing counter in memory
POP counter                   ; Suggest loading counter into register
```

## Load Effective Address (LEA)

Calculates an address without accessing memory.

### Syntax
```
LEA destination, source[, TYPE_PARAM5=condition]
```

### Example
```
LEA ptr, [array + index*4]    ; Calculate address without accessing memory
```

## Scope Management (SCOPEE/SCOPEL)

Define variable lifetimes and resource management boundaries.

### Syntax
```
SCOPEE                        ; Enter a new scope
; Variables and code...
SCOPEL                        ; Leave the scope (and destroy all variables)
```

### Example
```
SCOPEE
  VAR TYPE_INT32, temp, 0     ; temp is created here
  ; Use temp...
SCOPEL                        ; temp is destroyed here
```

## Variable Declaration (VAR)

Declares a variable with an optional initial value.

### Syntax
```
VAR type, name[, initial_value]
```

### Examples
```
VAR TYPE_INT32, counter, 0    ; Integer with initialization
VAR TYPE_PTR, data_pointer    ; Pointer without initialization
VAR TYPE_FP32, pi, 3.14159    ; Floating-point with initialization
```

## Memory Block Operations

### MEMCPY

Copies a block of memory from source to destination.

#### Syntax
```
MEMCPY destination, source, size[, TYPE_PARAM5=condition]
```

#### Example
```
MEMCPY dest_buffer, src_buffer, 100    ; Copy 100 bytes
```

### MEMSET

Fills a block of memory with a value.

#### Syntax
```
MEMSET destination, value, size[, TYPE_PARAM5=condition]
```

#### Example
```
MEMSET buffer, 0, 1024                 ; Zero out 1KB
```

### MEMCMP

Compares two memory blocks.

#### Syntax
```
MEMCMP result, first, second, size
```

#### Example
```
MEMCMP comparison_result, block1, block2, 100
```

## Atomic Operations

### XCHG

Atomically exchanges two values.

#### Syntax
```
XCHG first, second[, TYPE_PARAM5=condition]
```

#### Example
```
XCHG old_value, [lock_var]    ; Atomically exchange values
```

### CAS

Compare and swap - atomic operation for synchronization.

#### Syntax
```
CAS destination, expected, new[, TYPE_PARAM0=memory_control][, TYPE_PARAM5=condition]
```

#### Example
```
CAS [lock_address], 0, 1, TYPE_PARAM0=MEMORY_CTRL_ATOMIC
```

## Memory Addressing Modes

COIL supports several memory addressing modes:

### Direct Addressing
```
MOV value, [address]          ; Access memory at fixed address
```

### Register Indirect
```
MOV value, [TYPE_RGP=RAX]     ; Access memory at address in RAX
```

### Base + Offset
```
MOV value, [base_ptr + 8]     ; Access memory at base + offset
```

### Base + Scaled Index
```
MOV value, [array + index*4]  ; Access memory at base + index*scale
```

### Full Addressing Mode
```
MOV value, [base + index*scale + offset]  ; Combined addressing
```

## Memory Model Integration

The memory operations interact with COIL's memory model:

- **Address Space**: Operations target the linear address space
- **Alignment**: Memory access should respect alignment requirements
- **Ordering**: Memory operations follow the defined ordering model

## Binary Encoding Examples

### MOV Instruction (Register to Variable)

CASM:
```
MOV counter, TYPE_RGP=RAX
```

Binary:
```
0x10         ; Opcode for MOV
0x02         ; Two operands
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "counter"
0x9200       ; TYPE_RGP
0x00         ; Register ID for RAX
```

### VAR Instruction with Initialization

CASM:
```
VAR TYPE_INT32, counter, 0
```

Binary:
```
0x16         ; Opcode for VAR
0x03         ; Three operands
0x0300       ; TYPE_INT32
0x9100       ; TYPE_SYM for variable name
[sym_id]     ; Symbol ID for "counter"
0x0320       ; TYPE_INT32+IMM
0x00000000   ; Value 0
```

### Memory Access

CASM:
```
MOV value, [address + 4]
```

Binary:
```
0x10         ; Opcode for MOV
0x02         ; Two operands
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "value"
0xM?P?       ; Memory access type (implementation-defined)
[...]        ; Address calculation data
```