# Memory Operations

## Overview

Memory operations manage the allocation, access, and manipulation of data in memory. These instructions provide a consistent interface for working with variables, memory regions, and program scopes across all architectures.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x20   | MOV      | Move data between locations |
| 0x21   | PUSH     | Push value onto stack |
| 0x22   | POP      | Pop value from stack |
| 0x23   | LEA      | Load effective address |
| 0x24   | SCOPE    | Begin a variable scope |
| 0x25   | SCOPL    | End a variable scope |
| 0x26   | VAR      | Declare a variable |
| 0x27   | XCHG     | Exchange values |
| 0x28   | CAS      | Compare and swap (atomic) |

## Detailed Specifications

### MOV (0x20)

Moves data between memory locations, registers, or immediate values.

**Encoding:**
```
[0x20][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Copies the value from src to dest
- Type conversion is performed if necessary and possible
- Dest must be a writable location (not an immediate or constant)
- Source can be any valid operand


### PUSH (0x21)

Pushes a value onto the stack.

**Encoding:**
```
[0x21][0x01][value: Operand]
```

**Behavior:**
- Decrements the stack pointer by the size of the value
- Copies the value to the location pointed to by the stack pointer
- Respects the current ABI's stack direction and alignment requirements


### POP (0x22)

Pops a value from the stack.

**Encoding:**
```
[0x22][0x01][dest: Operand]
```

**Behavior:**
- Copies the value at the current stack pointer to the destination
- Increments the stack pointer by the size of the value
- Destination must be a writable location
- Respects the current ABI's stack direction and alignment requirements


### LEA (0x23)

Loads the effective address of a memory operand.

**Encoding:**
```
[0x23][0x02][dest: Operand][src: Operand]
```

**Behavior:**
- Calculates the effective address of src without accessing memory
- Stores the resulting address in dest
- Destination must be a writable location that can store an address

### SCOPE (0x24)

Begins a new variable scope.

**Encoding:**
```
[0x24][0x00]
```

**Behavior:**
- Creates a new variable scope context
- Variables declared within this scope are destroyed at matching SCOPL
- Scopes can be nested


### SCOPL (0x25)

Ends the current variable scope.

**Encoding:**
```
[0x25][0x00]
```

**Behavior:**
- Destroys all variables declared in the current scope
- Restores the previous scope context
- Must match a previous SCOPE instruction

### VAR (0x26)

Declares a variable.

**Encoding:**
```
[0x26][0x02][id: VARID][type: Type]
[0x26][0x03][id: VARID][type: Type][initial_value: Operand]
```

**Behavior:**
- Allocates storage for a variable with the specified type
- Assigns an initial value if provided
- Variable is valid until the end of the current scope
- Returns a variable ID that can be used with the VARIABLE extension

### XCHG (0x27)

Exchanges values between two locations.

**Encoding:**
```
[0x27][0x02][op1: Operand][op2: Operand]
```

**Behavior:**
- Atomically swaps the values between op1 and op2
- Both operands must be writable locations
- Both operands must have compatible types

### CAS (0x28)

Compare and swap operation (atomic).

**Encoding:**
```
[0x28][0x03][dest: Operand][compare: Operand][exchange: Operand]
```

**Behavior:**
- Atomically compares dest with compare value
- If equal, replaces dest with exchange value
- Sets status flags based on the comparison
- Returns the original value of dest

## Memory Addressing Modes

COIL supports various memory addressing modes:

| Mode | Syntax | Description |
|------|--------|-------------|
| Direct | var | Access variable directly |
| Indirect | [ptr] | Access memory at address in ptr |
| Based | [base + offset] | Address with fixed offset |
| Indexed | [base + index * scale] | Address with scaled index |
| Combined | [base + index * scale + offset] | Complete addressing |

## Related Components

- [Control Flow](./control-flow.md) - Execution flow instructions
- [Type System](../types/type-system.md) - Complete type system reference