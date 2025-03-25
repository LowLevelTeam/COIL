# Memory Operations (0x10-0x2F)

Memory operations handle data movement, variable management, and memory manipulation. These instructions form the foundation for effective memory management in COIL.

## Variable System Instructions

#### VAR (0x16)
Declare a variable with optional initialization. Variables are the primary mechanism for data handling in COIL and abstract over registers and memory locations.

```
Operands:
- Type: TYPE_*
- Name: TYPE_SYM
- InitialValue: (optional) Non TYPE_VOID
```

The COIL processor automatically determines optimal storage for variables:
- Frequently accessed variables are typically assigned to registers
- Less frequently used variables may be stored in memory
- When register pressure is high, variables may be spilled to stack
- Register selection considers the variable's type and usage pattern

Example:
```
; Declare variables
VAR TYPE_INT32, counter, 0
VAR TYPE_INT64, result
VAR TYPE_FP32, pi, 3.14159

; Use variables (no register allocation needed)
ADD counter, counter, 1
MUL result, counter, 10
```

#### SCOPEE (0x14)
Enter a new variable scope. Creates a lexical boundary for variables and enables automatic cleanup and optimization.

```
Operands:
- None
```

Scopes provide several benefits:
- Automatic variable lifetime management
- Register optimization opportunities
- Structured control over variable visibility
- Automatic cleanup when exiting the scope

Example:
```
; Begin a new scope
SCOPEE
    ; Variables only accessible within this scope
    VAR TYPE_INT32, temp, 0
    
    ; Operations using temp
    ADD temp, temp, 5
    
    ; Nested scope
    SCOPEE
        VAR TYPE_INT32, inner_temp, 10
        ADD temp, inner_temp
    SCOPEL  ; inner_temp is deallocated here
    
SCOPEL  ; temp is deallocated here
```

#### SCOPEL (0x15)
Leave the current variable scope. Releases variables defined in this scope.

```
Operands:
- None
```

When a scope is exited:
- All variables declared in the scope are deallocated
- Resources (such as registers) used by those variables are freed
- The COIL processor may optimize based on scope boundaries

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
; Move immediate value to variable
MOV counter, 42

; Move variable to memory
MOV [address], value

; Move memory to variable with condition
MOV_NZ temp, [array + 8]

; Move with register (only when absolutely necessary)
MOV TYPE_RGP=RAX, 42

; Get parameter via ABI
MOV param, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0

; Get return value via ABI
MOV result, TYPE_ABICTL=ABICTL_RET=windows_x64
```

#### PUSH (0x11)
Push a value onto the stack.
When used with variables, PUSH can demote a variable, suggesting the processor prioritize this variable on the stack rather than in a register. This is useful for infrequently used variables or to free up register space.

```
Operands:
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Push value
PUSH 42

; Push variable to demote its storage
PUSH counter  ; Hint to the processor to store counter in memory
```

#### POP (0x12)
Pop a value from the stack.
When used with variables, POP can promote a variable, suggesting the processor prioritize this variable in a register. This is useful for frequently used variables.

```
Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Pop into variable
POP result

; Promote variable to register
POP counter  ; Hint to the processor to keep counter in a register
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
; Load address of array element into variable
LEA ptr, [array + index*4]

; Get address of structure field
LEA field_ptr, [object + 16]
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

; Copy with variable-based size
VAR TYPE_UNT32, copy_size, 256
MEMCPY dest_ptr, src_ptr, copy_size
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

; Initialize memory conditionally with variables
VAR TYPE_PTR, buffer_ptr, buffer
VAR TYPE_UNT8, fill_value, 0xFF
VAR TYPE_UNT64, buffer_size, 1024
MEMSET_Z buffer_ptr, fill_value, buffer_size
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
; Swap variable contents
XCHG a, b

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
; Atomic compare and swap with variables
VAR TYPE_PTR, lock_ptr, lock_address
VAR TYPE_UNT32, expected_value, 0
VAR TYPE_UNT32, new_value, 1
CAS [lock_ptr], expected_value, new_value
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
VAR TYPE_PTR, buffer_ptr, buffer_address
VAR TYPE_UNT64, page_size, 4096
PIN buffer_ptr, page_size
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
VAR TYPE_PTR, buffer_ptr, buffer_address
UNPIN buffer_ptr
```

## Variable System Best Practices

For optimal use of COIL's variable system:

1. **Declare variables at the beginning of their scope**: This improves readability and helps the processor optimize variable allocation.

2. **Use appropriate scoping**: Smaller, more focused scopes allow better resource management.

3. **Consider variable promotion/demotion**: Use POP/PUSH hints for variables with specific performance needs.

4. **Use appropriate types**: Accurate type information allows better optimization.

5. **Avoid premature optimization**: Let the COIL processor handle register allocation in most cases.

## ABI Integration with the Variable System

The variable syste