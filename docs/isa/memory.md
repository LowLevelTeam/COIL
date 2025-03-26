# Memory Operations (0x10-0x2F)

## Purpose

This document defines the memory operations in COIL, which handle data movement, variable management, and memory manipulation. Memory operations form a fundamental part of COIL's instruction set, providing abstractions over processor-specific memory and register handling.

## Variable System Instructions

### VAR (0x16)
Declare a variable with optional initialization. Variables are the primary abstraction mechanism in COIL, hiding the details of register allocation and memory management.

```
Operands:
- Type: TYPE_*
- Name: TYPE_SYM
- InitialValue: (optional) Non TYPE_VOID
```

Example:
```
; Declare variables
VAR TYPE_INT32, counter, 0           ; With initialization
VAR TYPE_FP64, temperature           ; Without initialization
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"  ; Array initialization
```

Binary encoding:
```
0x16      ; Opcode for VAR
0x02/0x03 ; Two or three operands
[type1]   ; Type identifier
0x9100    ; TYPE_SYM for name
[sym_id]  ; Symbol ID
[type3]   ; Type of initial value (if present)
[value3]  ; Initial value (if present)
```

### SCOPEE (0x14)
Enter a new variable scope. Creates a lexical boundary for variables and enables automatic cleanup.

```
Operands:
- None
```

Example:
```
; Begin a new scope
SCOPEE
    ; Variables declared here are local to this scope
    VAR TYPE_INT32, temp, 0
    
    ; Nested scope
    SCOPEE
        VAR TYPE_INT32, inner_var, 5
    SCOPEL  ; inner_var is deallocated here
    
SCOPEL  ; temp is deallocated here
```

Binary encoding:
```
0x14      ; Opcode for SCOPEE
0x00      ; Zero operands
```

### SCOPEL (0x15)
Leave the current variable scope. Releases all variables defined in this scope.

```
Operands:
- None
```

Example:
```
SCOPEE
    ; Variables and code here
SCOPEL  ; End of scope, all variables declared in this scope are deallocated
```

Binary encoding:
```
0x15      ; Opcode for SCOPEL
0x00      ; Zero operands
```

## Data Movement Instructions

### MOV (0x10)
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
MOV param, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0

; Get return value via ABI
MOV result, TYPE_ABICTL=ABICTL_RET=platform_default
```

Binary encoding:
```
0x10      ; Opcode for MOV
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### PUSH (0x11)
Push a value onto the stack or demote a variable to memory storage.

```
Operands:
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Push immediate value onto stack
PUSH 42

; Push variable (also serves as a hint to demote to memory)
PUSH counter

; Conditional push
PUSH_NZ TYPE_RGP=RAX
```

Binary encoding:
```
0x11      ; Opcode for PUSH
0x01/0x02 ; One or two operands
[type1]   ; Type of source
[value1]  ; Source value
[type2]   ; TYPE_PARAM5 (if conditional)
[value2]  ; Condition code (if conditional)
```

### POP (0x12)
Pop a value from the stack or promote a variable to register storage.

```
Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Pop from stack to variable
POP result

; Pop to register
POP TYPE_RGP=RAX

; Pop with condition
POP_NZ temp
```

Binary encoding:
```
0x12      ; Opcode for POP
0x01/0x02 ; One or two operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; TYPE_PARAM5 (if conditional)
[value2]  ; Condition code (if conditional)
```

### LEA (0x13)
Load Effective Address - computes the address of a memory operand and stores it in the destination.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Memory reference (with TYPE_PTR base type)
- TYPE_PARAM5: branch_condition_t (optional)
```

Example:
```
; Get address of array element
LEA ptr, [array + index*4]

; Get address of structure field
LEA field_ptr, [struct + 16]

; Conditional LEA
LEA_NZ ptr, [array + index*8]
```

Binary encoding:
```
0x13      ; Opcode for LEA
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source (memory reference)
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Memory Block Operations

### MEMCPY (0x17)
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

; Copy with variable size
VAR TYPE_UNT32, copy_size, 256
MEMCPY dest_ptr, src_ptr, copy_size

; Conditional copy
MEMCPY_NZ dest_ptr, src_ptr, size
```

Binary encoding:
```
0x17      ; Opcode for MEMCPY
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; Type of size
[value3]  ; Size value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### MEMSET (0x18)
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

; Set with variables
VAR TYPE_PTR, buffer_ptr, buffer
VAR TYPE_UNT8, fill_value, 0xFF
VAR TYPE_UNT64, buffer_size, 1024
MEMSET buffer_ptr, fill_value, buffer_size

; Conditional set
MEMSET_Z buffer, 0, size
```

Binary encoding:
```
0x18      ; Opcode for MEMSET
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of fill value
[value2]  ; Fill value
[type3]   ; Type of size
[value3]  ; Size value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### MEMCMP (0x19)
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

Binary encoding:
```
0x19      ; Opcode for MEMCMP
0x03      ; Three operands
[type1]   ; Type of first pointer
[value1]  ; First pointer value
[type2]   ; Type of second pointer
[value2]  ; Second pointer value
[type3]   ; Type of size
[value3]  ; Size value
```

## Atomic Memory Operations

### XCHG (0x1A)
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

; Conditional exchange
XCHG_NZ x, y
```

Binary encoding:
```
0x1A      ; Opcode for XCHG
0x02/0x03 ; Two or three operands
[type1]   ; Type of first operand
[value1]  ; First operand value
[type2]   ; Type of second operand
[value2]  ; Second operand value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### CAS (0x1B)
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
VAR TYPE_PTR, lock_ptr, lock_address
VAR TYPE_UNT32, expected_value, 0
VAR TYPE_UNT32, new_value, 1
CAS [lock_ptr], expected_value, new_value

; Check if the swap happened (expected_value is updated if failed)
CMP expected_value, 0
BR_NE swap_failed
```

Binary encoding:
```
0x1B      ; Opcode for CAS
0x03/0x05 ; Three to five operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of expected value
[value2]  ; Expected value
[type3]   ; Type of new value
[value3]  ; New value
[type4]   ; TYPE_PARAM0 (if present)
[value4]  ; MEMORY_CTRL_ATOMIC
[type5]   ; TYPE_PARAM5 (if conditional)
[value5]  ; Condition code (if conditional)
```

## Memory Pinning Instructions

### PIN (0x2E)
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

Binary encoding:
```
0x2E      ; Opcode for PIN
0x02      ; Two operands
[type1]   ; Type of address
[value1]  ; Address value
[type2]   ; Type of size
[value2]  ; Size value
```

### UNPIN (0x2F)
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

Binary encoding:
```
0x2F      ; Opcode for UNPIN
0x01      ; One operand
[type1]   ; Type of address
[value1]  ; Address value
```

## Memory Access Patterns

COIL supports various memory access patterns through memory reference operands:

### Direct Memory Access
```
; Access memory at fixed address
MOV [0x1000], 42
```

### Register-Based Address
```
; Access memory at address in register
MOV [TYPE_RGP=RAX], 42
```

### Base + Offset
```
; Access memory with offset from base address
MOV [buffer + 8], 42
```

### Base + Index * Scale
```
; Access array element: base + index*scale
MOV [array + index*4], 42
```

### Base + Index * Scale + Offset
```
; Complex addressing: base + index*scale + offset
MOV [matrix + row*16 + column*4], 42
```

## Variable System Best Practices

For optimal use of COIL's memory operations:

1. **Use variables instead of direct register access**: Let the COIL processor handle register allocation.
   ```
   ; Good: Use variables
   VAR TYPE_INT32, counter, 0
   ADD counter, counter, 1
   
   ; Avoid: Direct register access
   MOV TYPE_RGP=RAX, 0
   ADD TYPE_RGP=RAX, TYPE_RGP=RAX, 1
   ```

2. **Use appropriate scoping**: Create scopes for variable lifetime management.
   ```
   SCOPEE
       VAR TYPE_INT32, temp, 0
       ; Use temp...
   SCOPEL  ; temp is automatically deallocated
   ```

3. **Let the processor handle memory operations**: Use variables rather than managing stack/memory directly.
   ```
   ; Good: Let the processor handle parameters
   CALL function, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
   
   ; Avoid: Manual parameter passing
   PUSH y
   PUSH x
   CALL function
   ```

4. **Use memory block operations for efficiency**: Use `MEMCPY`, `MEMSET` rather than loop implementations.
   ```
   ; Good: Use block operations
   MEMCPY dest, src, 100
   
   ; Avoid: Manual loops
   VAR TYPE_UNT64, i, 0
   loop_start:
       CMP i, 100
       BR_GE loop_end
       MOV [dest + i], [src + i]
       ADD i, i, 1
       BR loop_start
   loop_end:
   ```

5. **Use atomic operations for thread safety**: When sharing data between threads, use atomic operations.
   ```
   ; Atomic increment
   VAR TYPE_UNT32, expected
   loop_start:
       MOV expected, [counter]
       VAR TYPE_UNT32, new_value
       ADD new_value, expected, 1
       CAS [counter], expected, new_value
       CMP expected, [counter]
       BR_NE loop_start  ; Retry if someone else modified it
   ```

## Processor-Specific Considerations

While memory operations are universal across all processor types, some aspects may vary:

1. **Alignment Requirements**: Different processors have different alignment requirements.
   ```
   ; May need alignment on some processors
   ALIGN 16
   VAR TYPE_V128=TYPE_FP32, vector
   ```

2. **Atomic Operation Support**: Atomic operations may be implemented differently.
   ```
   ; Processor-specific implementations
   CAS [lock], expected, new_value  ; Translates to appropriate atomic instruction
   ```

3. **Memory Ordering**: Memory ordering semantics may vary.
   ```
   ; Explicit memory ordering
   MOV [shared_data], value, TYPE_PARAM0=MEMORY_CTRL_SEQ_CST
   ```

4. **Cache Control**: Some processors support explicit cache control.
   ```
   ; Cache hint (processor-specific)
   MOV [data], value, TYPE_PARAM0=MEMORY_CTRL_NONTEMPORAL
   ```

COIL processors automatically handle these differences, translating the universal memory operations to the appropriate processor-specific mechanisms.