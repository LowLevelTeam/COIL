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
MOV #1, 42               ; Store immediate value to variable #1
MOV #2, #1               ; Copy variable #1 to variable #2
MOV [#3], #4             ; Store variable #4 to memory pointed by #3
MOV #5, [#3]             ; Load from memory to variable #5
MOV TYPE_RGP=RAX, #1     ; Store variable #1 to register
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
PUSH #1                    ; Push variable #1 to stack
POP #2                     ; Pop from stack to variable #2
PUSH #3                    ; Suggest storing variable #3 in memory
POP #3                     ; Suggest loading variable #3 into register
```

## Load Effective Address (LEA)

Calculates an address without accessing memory.

### Syntax
```
LEA destination, source[, TYPE_PARAM5=condition]
```

### Example
```
LEA #1, [#2 + #3*4]        ; Calculate address without accessing memory
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
  VAR #1, TYPE_INT32, 0       ; variable #1 is created here
  ; Use variable #1...
SCOPEL                        ; variable #1 is destroyed here
```

## Variable Declaration (VAR)

Declares a variable with an optional initial value.

### Syntax
```
VAR #id, type[, initial_value]
```

### Examples
```
VAR #1, TYPE_INT32, 0         ; Integer with initialization
VAR #2, TYPE_PTR              ; Pointer without initialization
VAR #3, TYPE_FP32, 3.14159    ; Floating-point with initialization
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
MEMCPY #1, #2, #3             ; Copy memory block of size #3 from #2 to #1
```

### MEMSET

Fills a block of memory with a value.

#### Syntax
```
MEMSET destination, value, size[, TYPE_PARAM5=condition]
```

#### Example
```
MEMSET #1, 0, #2              ; Zero out memory block
```

### MEMCMP

Compares two memory blocks.

#### Syntax
```
MEMCMP result, first, second, size
```

#### Example
```
MEMCMP #1, #2, #3, #4         ; Compare blocks, store result in #1
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
XCHG #1, [#2]                 ; Atomically exchange values
```

### CAS

Compare and swap - atomic operation for synchronization.

#### Syntax
```
CAS destination, expected, new[, TYPE_PARAM0=memory_control][, TYPE_PARAM5=condition]
```

#### Example
```
CAS [#1], #2, #3, TYPE_PARAM0=MEMORY_CTRL_ATOMIC
```

## Memory Addressing Modes

COIL supports several memory addressing modes:

### Direct Addressing
```
MOV #1, [#2]                  ; Access memory at address in variable #2
```

### Register Indirect
```
MOV #1, [TYPE_RGP=RAX]        ; Access memory at address in RAX
```

### Base + Offset
```
MOV #1, [#2 + 8]              ; Access memory at base + offset
```

### Base + Scaled Index
```
MOV #1, [#2 + #3*4]           ; Access memory at base + index*scale
```

### Full Addressing Mode
```
MOV #1, [#2 + #3*4 + 8]       ; Combined addressing
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
MOV #1, TYPE_RGP=RAX
```

Binary:
```
0x10         ; Opcode for MOV
0x02         ; Two operands
0x9000       ; TYPE_VAR
0x01         ; Variable ID 1
0x9200       ; TYPE_RGP
0x00         ; Register ID for RAX
```

### VAR Instruction with Initialization

CASM:
```
VAR #1, TYPE_INT32, 0
```

Binary:
```
0x16         ; Opcode for VAR
0x03         ; Three operands
0x0300       ; TYPE_INT32
0x01         ; Variable ID 1
0x0320       ; TYPE_INT32+IMM
0x00000000   ; Value 0
```

### Memory Access

CASM:
```
MOV #1, [#2 + 4]
```

Binary:
```
0x10         ; Opcode for MOV
0x02         ; Two operands
0x9000       ; TYPE_VAR
0x01         ; Variable ID 1
0xM?P?       ; Memory access type (implementation-defined)
[...]        ; Address calculation data
```