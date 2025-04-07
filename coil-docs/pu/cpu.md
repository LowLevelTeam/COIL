# CPU Processing Unit

## Overview

The CPU Processing Unit defines COIL instructions specific to traditional Central Processing Units. These instructions leverage CPU-specific features while maintaining compatibility with the core COIL specification.

## Instruction Categories

CPU-specific instructions use opcodes in the 0xA0-0xDF range, divided into three categories:

1. **Processing Unit Instructions (0xA0-0xBF)**: Instructions available on all CPU implementations
2. **Architecture-Specific Instructions (0xC0-0xCF)**: Instructions specific to certain CPU architectures
3. **Mode-Specific Instructions (0xD0-0xDF)**: Instructions available only in specific CPU modes

## Processing Unit Instructions (0xA0-0xBF)

### Interrupt and Exception Management (0xA0-0xA7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xA0   | INT      | Software interrupt | 1 | CPU |
| 0xA1   | INTD     | Software interrupt with data | 2 | CPU |
| 0xA2   | IRET     | Return from interrupt | 0 | CPU |
| 0xA3   | CLI      | Clear interrupt flag | 0 | CPU |
| 0xA4   | STI      | Set interrupt flag | 0 | CPU |
| 0xA5   | EXCPT    | Generate exception | 1 | CPU |
| 0xA6   | EHTRY    | Try block entry | 1 | CPU |
| 0xA7   | EHCAT    | Catch block entry | 2 | CPU |

### CPU Control (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xA8   | HLT      | Halt processor | 0 | CPU |
| 0xA9   | WAIT     | Wait for interrupt | 0 | CPU |
| 0xAA   | YIELD    | Yield execution to another thread | 0 | CPU |
| 0xAB   | FENCE    | Memory fence | 0-1 | CPU |
| 0xAC   | CPUID    | Get CPU identification | 1-2 | CPU |
| 0xAD   | SYSINFO  | Get system information | 1-2 | CPU |
| 0xAE   | RDTSC    | Read time-stamp counter | 1 | CPU |
| 0xAF   | NOP2     | Multi-byte NOP | 0-1 | CPU |

### Thread Management (0xB0-0xB7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xB0   | THCRT    | Create thread | 2-3 | CPU |
| 0xB1   | THJOIN   | Join thread | 1-2 | CPU |
| 0xB2   | THDET    | Detach thread | 1 | CPU |
| 0xB3   | THSUSP   | Suspend thread | 1 | CPU |
| 0xB4   | THRES    | Resume thread | 1 | CPU |
| 0xB5   | THLOCK   | Lock mutex | 1 | CPU |
| 0xB6   | THULCK   | Unlock mutex | 1 | CPU |
| 0xB7   | THID     | Get current thread ID | 1 | CPU |

### Atomic Operations (0xB8-0xBF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xB8   | ATOMLD   | Atomic load | 2 | CPU |
| 0xB9   | ATOMST   | Atomic store | 2 | CPU |
| 0xBA   | ATOMXCH  | Atomic exchange | 3 | CPU |
| 0xBB   | ATOMCAS  | Atomic compare-and-swap | 4 | CPU |
| 0xBC   | ATOMADD  | Atomic add | 3 | CPU |
| 0xBD   | ATOMSUB  | Atomic subtract | 3 | CPU |
| 0xBE   | ATOMAND  | Atomic AND | 3 | CPU |
| 0xBF   | ATOMOR   | Atomic OR | 3 | CPU |

## Architecture-Specific Instructions (0xC0-0xCF)

These instructions are available only on specific CPU architectures as indicated.

### x86/x64 Architecture (0xC0-0xC3)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC0   | X86SPEC  | x86 special instruction | 1-3 | CPU.x86 |
| 0xC1   | RDMSR    | Read model-specific register | 2 | CPU.x86 |
| 0xC2   | WRMSR    | Write model-specific register | 2 | CPU.x86 |
| 0xC3   | CPUID2   | Extended CPU identification | 2-3 | CPU.x86 |

### ARM Architecture (0xC4-0xC7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC4   | ARMSPEC  | ARM special instruction | 1-3 | CPU.ARM |
| 0xC5   | BARRIER  | Data memory barrier | 1 | CPU.ARM |
| 0xC6   | WFIEVENT | Wait for event | 0 | CPU.ARM |
| 0xC7   | SEVEVENT | Send event | 0 | CPU.ARM |

### RISC-V Architecture (0xC8-0xCB)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC8   | RVSPEC   | RISC-V special instruction | 1-3 | CPU.RISC-V |
| 0xC9   | FENCE.I  | Instruction fence | 0 | CPU.RISC-V |
| 0xCA   | FENCE.TSO| Total store ordering fence | 0 | CPU.RISC-V |
| 0xCB   | LR       | Load-reserved | 2 | CPU.RISC-V |

### Reserved/Other Architectures (0xCC-0xCF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xCC   | MIPSSPEC | MIPS special instruction | 1-3 | CPU.MIPS |
| 0xCD   | PWRSPEC  | PowerPC special instruction | 1-3 | CPU.PowerPC |
| 0xCE   | ARCHRSVD1| Reserved for future architectures | 1-3 | Reserved |
| 0xCF   | ARCHRSVD2| Reserved for future architectures | 1-3 | Reserved |

## Mode-Specific Instructions (0xD0-0xDF)

These instructions are available only in specific CPU modes.

### Privileged Mode Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD0   | SYSPG    | Change system page table | 2 | CPU.Privileged |
| 0xD1   | SYSINT   | Configure interrupt controller | 2-3 | CPU.Privileged |
| 0xD2   | IOCFG    | Configure I/O | 2-3 | CPU.Privileged |
| 0xD3   | PRCFG    | Configure processor | 2-3 | CPU.Privileged |
| 0xD4   | INITTBL  | Initialize interrupt table | 2 | CPU.Privileged |
| 0xD5   | INITVM   | Initialize virtual memory | 2 | CPU.Privileged |
| 0xD6   | IOTRAP   | Set I/O trap | 2-3 | CPU.Privileged |
| 0xD7   | PRVMODE  | Change privilege mode | 1 | CPU.Privileged |

### Virtual Machine Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD8   | VMENTRY  | Enter virtual machine | 1-2 | CPU.VM |
| 0xD9   | VMEXIT   | Exit virtual machine | 0-1 | CPU.VM |
| 0xDA   | VMCALL   | Hypercall | 1-3 | CPU.VM |
| 0xDB   | VMCFG    | Configure VM | 2-3 | CPU.VM |
| 0xDC   | VMIOMMU  | Configure I/O MMU | 2-3 | CPU.VM |
| 0xDD   | VMINTR   | Virtual interrupt | 1-2 | CPU.VM |
| 0xDE   | VMRSVD1  | Reserved for future VM instructions | 1-3 | Reserved |
| 0xDF   | VMRSVD2  | Reserved for future VM instructions | 1-3 | Reserved |

## Detailed Instruction Behaviors

### Interrupt Instructions

#### INT (0xA0)
- Generates a software interrupt with the specified interrupt number
- Pushes flags, return address, and other context to stack
- Transfers control to the appropriate interrupt handler
- Flag effects: None (preserved for handler)

#### IRET (0xA2)
- Returns from an interrupt handler
- Restores flags and other context from stack
- Pops return address and transfers control back
- Flag effects: All flags restored from stack

### Atomic Operations

#### ATOMCAS (0xBB)
- Atomically compares and swaps memory values
- Compares the value at address with the expected value
- If equal, stores the new value at address
- Returns the original value
- Flag effects: Z set if exchange occurred

## Implementation Requirements

CPU implementations must adhere to these guidelines:

1. **Instruction Support**:
   - All Processing Unit Instructions (0xA0-0xBF) must be implemented
   - Architecture-Specific Instructions (0xC0-0xCF) are implemented only for supported architectures
   - Mode-Specific Instructions (0xD0-0xDF) are implemented only for supported modes

2. **Interrupt Behavior**:
   - Software interrupts must follow the platform's interrupt vector table
   - Hardware interrupts must be properly prioritized and handled

3. **Atomic Operations**:
   - Must provide strong atomicity guarantees
   - Must be compatible with the memory model

4. **Thread Management**:
   - Must map to the platform's native threading model
   - Must preserve semantics across implementations# CPU Instruction Set Architecture (0xA0-0xDF)

Describe the CPU COIL Instruction Set.


