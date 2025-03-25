# Memory Operations (0x10-0x2F)

Memory operations handle data movement, variable management, and memory manipulation. These instructions form the foundation for effective memory management in COIL.

## Core Memory Instructions

#### MOV (0x10)
Move data between locations or load immediate values.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Move immediate value to register
MOV TYPE_RGP=RAX, 42

; Move register to memory
MOV [address], TYPE_RGP=RBX

; Move memory to register with condition
MOV_NZ TYPE_RGP=RCX, [array + 8]
```

#### PUSH (0x11)
Push a value onto the stack.
Can be used with variables to demote its position. When pushing a variable, the processor will prioritize this variable on the stack and free up a register slot if using one.

```
Operands:
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Push register value
PUSH TYPE_RGP=RAX

; Push immediate value
PUSH 42

; Push variable to free register
PUSH counter
```

#### POP (0x12)
Pop a value from the stack.
Can be used with variables to promote its position. When popping a variable, the processor will prioritize this variable in a register.

```
Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Pop into register
POP TYPE_RGP=RBX

; Pop into variable
POP result
```

#### LEA (0x13)
Load Effective Address - computes the address of a memory operand and stores it in the destination.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Memory reference (with TYPE_PTR base type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Load address of array element into register
LEA TYPE_RGP=RSI, [array + TYPE_RGP=RCX*4]

; Get address of structure field
LEA TYPE_RGP=RDI, [object + 16]
```

#### SCOPEE (0x14)
Enter a new variable scope. Creates a lexical boundary for variables and can optimize register/stack usage.

```
Operands:
- None
```

Example:
```
; Begin a new scope for local variables
SCOPEE
    VAR TYPE_INT32, temp, 0
    ; ... operations using temp ...
SCOPEL
```

#### SCOPEL (0x15)
Leave the current variable scope. Releases variables defined in this scope.

```
Operands:
- None
```

#### VAR (0x16)
Declare a variable with optional initialization.

```
Operands:
- Type: TYPE_*
- Name: TYPE_SYM
- InitialValue: (optional) Non TYPE_VOID
```

Example:
```
; Declare an uninitialized integer
VAR TYPE_INT32, counter

; Declare and initialize a floating point value
VAR TYPE_FP32, pi, 3.14159

; Declare using a register value
VAR TYPE_INT64, result, TYPE_RGP=RAX
```

#### MEMCPY (0x17)
Copy a block of memory from source to destination.

```
Operands:
- Destination: TYPE_PTR
- Source: TYPE_PTR
- Size: TYPE_*UNT*
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Copy 100 bytes from source to destination
MEMCPY dest_buffer, source_buffer, 100

; Copy with register-based size
MEMCPY TYPE_RGP=RDI, TYPE_RGP=RSI, TYPE_RGP=RCX
```

#### MEMSET (0x18)
Fill a block of memory with a specified value.

```
Operands:
- Destination: TYPE_PTR
- Value: TYPE_UNT8 or TYPE_INT8
- Size: TYPE_*UNT*
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Set 200 bytes to zero
MEMSET buffer, 0, 200

; Initialize memory conditionally
MEMSET_Z TYPE_RGP=RDI, 0xFF, TYPE_RGP=RCX
```

#### MEMCMP (0x19)
Compare two blocks of memory and set flags based on the result.

```
Operands:
- First: TYPE_PTR
- Second: TYPE_PTR
- Size: TYPE_*UNT*
```

Example:
```
; Compare two memory regions
MEMCMP buffer1, buffer2, 50

; Branch based on comparison result
BR_EQ identical
```

#### XCHG (0x1A)
Exchange the contents of two locations.

```
Operands:
- First: Non TYPE_VOID
- Second: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Swap register contents
XCHG TYPE_RGP=RAX, TYPE_RGP=RBX

; Swap variable and memory
XCHG counter, [address]
```

#### CAS (0x1B)
Compare And Swap - atomic operation for synchronization.

```
Operands:
- Destination: Non TYPE_VOID
- Expected: Non TYPE_VOID
- New: Non TYPE_VOID
- TYPE_PARAM0: MEMORY_CTRL_ATOMIC always set
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Atomic compare and swap
CAS [lock_address], 0, 1

; CAS with register operands
CAS TYPE_RGP=RAX, TYPE_RGP=RBX, TYPE_RGP=RCX
```

## Memory Control Instructions

#### PIN (0x2E)
Pin memory for DMA operations or to prevent paging.

```
Operands:
- Address: TYPE_PTR
- Size: TYPE_*UNT*
```

Example:
```
; Pin 4KB page
PIN buffer_address, 4096
```

#### UNPIN (0x2F)
Unpin previously pinned memory.

```
Operands:
- Address: TYPE_PTR
```

Example:
```
; Unpin previously pinned memory
UNPIN buffer_address
```

## Memory Model Considerations

Memory operations in COIL adhere to the memory model specified in the configuration. This includes:

1. **Alignment Requirements**: By default, memory operations follow the architecture's natural alignment. The `ALIGN` directive and `MEMORY_CTRL_ALIGNED` parameter can be used to specify or enforce alignment.

2. **Atomicity**: For multi-threaded environments, atomic operations have explicit instructions (CAS) or can be specified with the `MEMORY_CTRL_ATOMIC` parameter.

3. **Ordering**: Memory operations follow the ordering rules of the target architecture unless explicitly relaxed or enforced.

4. **Caching Behavior**: Can be influenced with the `MEMORY_CTRL_VOLATILE` parameter to bypass caching.

## Implementation Requirements

A COIL v1 processor must:

1. Implement all memory operations with their specified semantics
2. Honor memory control parameters
3. Follow the architecture's memory model for alignment and ordering
4. Properly handle variable scoping (SCOPEE/SCOPEL)
5. Implement variable promotion/demotion optimization as specified