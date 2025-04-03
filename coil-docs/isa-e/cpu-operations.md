# CPU Operations

**WARNING - It is important to note all operations below can be overidden, 0xA0 is only an interrupt on CPU targets, on GPU targets it would be something completly different. Macros can help to ensure COIL compltes the correct functionality or ensure that if you are building a library that the current target is a CPU. If you already know your target device to be a CPU then of course you can use this as expected.**

**WARNING - Operations in the extended instruction set differ for each processing unit**

## Overview

CPU Operations are extended instructions (0xA0-0xBF) that work across all Central Processing Units regardless of their specific architecture or mode. These instructions provide access to hardware capabilities common to all CPUs, such as interrupt handling, system calls, and register management.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xA0   | INT      | Software interrupt |
| 0xA1   | SYSCALL  | System call |
| 0xA2   | SYSRET   | Return from system call |
| 0xA3   | PUSHA    | Push all registers |
| 0xA4   | POPA     | Pop all registers |
| 0xA5   | PUSHFD   | Push flags register |
| 0xA6   | POPFD    | Pop flags register |
| 0xA7   | CPUID    | CPU identification |
| 0xA8   | RDTSC    | Read time-stamp counter |
| 0xA9   | HLT      | Halt processor |

## Architecture Instruction Set

### x86

| Opcode | Mnemonic | Description                |
|--------|----------|----------------------------|
| 0xB0   | IN       | Input to processor IO port |
| 0xB1   | OUT      | Input to processor IO port |

## Detailed Specifications

### INT (0xA0)

Generates a software interrupt.

**Encoding:**
```
[0xA0][0x01][interrupt_number: Operand]
```

**Behavior:**
- Triggers a software interrupt with the specified number
- Executes the corresponding interrupt service routine
- Specific behavior depends on the target platform's interrupt model
- Interrupt number must be an integral type

### SYSCALL (0xA1)

Executes a system call.

**Encoding:**
```
[0xA1][0x01][syscall_number: Operand]
```

**Behavior:**
- Transfers control to the operating system to perform a system service
- System call number identifies the requested service
- Implementation adapts to the target platform (SYSENTER on x86-32, SYSCALL on x86-64, SVC on ARM)
- Return value typically placed in a predefined register

### SYSRET (0xA2)

Returns from a system call.

**Encoding:**
```
[0xA2][0x00]
[0xA2][0x01][status: Operand]
```

**Behavior:**
- Returns control from operating system to user program
- Optional status operand provides return value
- Implementation adapts to the target platform (SYSEXIT on x86-32, SYSRET on x86-64)

### PUSHA (0xA3)

Pushes all general-purpose registers onto the stack.

**Encoding:**
```
[0xA3][0x00]
```

**Behavior:**
- Saves all general-purpose registers to the stack
- Order and specific registers depend on the target architecture
- Stack pointer is adjusted accordingly
- Useful for function prologues and context saving

### POPA (0xA4)

Pops all general-purpose registers from the stack.

**Encoding:**
```
[0xA4][0x00]
```

**Behavior:**
- Restores all general-purpose registers from the stack
- Order must match corresponding PUSHA instruction
- Stack pointer is adjusted accordingly
- Useful for function epilogues and context restoration

### PUSHFD (0xA5)

Pushes the flags register onto the stack.

**Encoding:**
```
[0xA5][0x00]
```

**Behavior:**
- Saves the processor flags to the stack
- Stack pointer is adjusted accordingly
- Size depends on the target architecture's flags register

### POPFD (0xA6)

Pops the flags register from the stack.

**Encoding:**
```
[0xA6][0x00]
```

**Behavior:**
- Restores the processor flags from the stack
- Stack pointer is adjusted accordingly
- Useful for preserving and restoring execution state


### CPUID (0xA7)

Queries processor identification and feature information.

**Encoding:**
```
[0xA7][0x01][function: Operand]
[0xA7][0x02][function: Operand][subfunction: Operand]
```

**Behavior:**
- Queries processor information based on the function number
- Results are typically placed in predefined registers
- Optional subfunction provides additional query parameters
- Implementation adapts to target architecture's CPUID mechanism

### RDTSC (0xA8)

Reads the processor's time-stamp counter.

**Encoding:**
```
[0xA8][0x01][dest: Operand]
```

**Behavior:**
- Reads the processor's time-stamp counter into the destination
- Destination must be large enough to hold the counter value
- Useful for high-precision timing and performance measurements
- Implementation adapts to target architecture's timing mechanism

### HLT (0xA9)

Halts the processor until an interrupt occurs.

**Encoding:**
```
[0xA9][0x00]
```

**Behavior:**
- Places the processor in a low-power state
- Execution resumes when an external interrupt is received
- Typically a privileged instruction
- Implementation adapts to target architecture's halt mechanism

## Register Conventions

Register conventions for each architecture is specified in dedicated page for the architecture.
  [x86](./cpu/x86-operations.md)
  [ARM](./cpu/arm-operations.md)

## Privilege Levels

Many CPU operations require specific privilege levels:

| Operation | Typical Privilege Requirement |
|-----------|------------------------------|
| INT       | User or Supervisor mode      |
| SYSCALL   | User mode (transitions to Supervisor) |
| SYSRET    | Supervisor mode only         |
| HLT       | Supervisor mode only         |
| CPUID     | User mode                    |
| RDTSC     | User or Supervisor (configuration dependent) |

## Related Components

- [x86 Operations](./x86-operations.md) - x86-specific instructions
- [ARM Operations](./arm-operations.md) - ARM-specific instructions
- [Universal ISA](../isa-u/overview.md) - Platform-independent instructions