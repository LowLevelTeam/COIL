# CPU Processing Unit

## Overview

The CPU Processing Unit defines COIL instructions specific to traditional Central Processing Units. These instructions provide a consistent interface for CPU-specific functionality while maintaining compatibility with the core COIL specification.

## Architecture Modes

CPU instructions adapt to different architecture modes:

| Mode Value | Name | Description |
|------------|------|-------------|
| 0x01 | Real/16-bit | Original 16-bit mode (x86) |
| 0x02 | Protected/32-bit | 32-bit protected mode (x86), A32 (ARM) |
| 0x03 | Long/64-bit | 64-bit mode (x86-64), A64 (ARM64) |
| 0x81 | v16 | Virtualized 16-bit mode |
| 0x82 | v32 | Virtualized 32-bit mode |
| 0x83 | v64 | Virtualized 64-bit mode |

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
| 0xA1   | IRET     | Return from interrupt | 0 | CPU |
| 0xA2   | CLI      | Clear interrupt flag | 0 | CPU |
| 0xA3   | STI      | Set interrupt flag | 0 | CPU |
| 0xA4   | EXCPT    | Generate exception | 1 | CPU |
| 0xA5   | HLT      | Halt processor | 0 | CPU |
| 0xA6   | WAIT     | Wait for interrupt | 0 | CPU |
| 0xA7   | FENCE    | Memory fence | 0-1 | CPU |

### CPU Control (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xA8   | CPUID    | Get CPU identification | 1-2 | CPU |
| 0xA9   | RDTSC    | Read time-stamp counter | 1 | CPU |
| 0xAA   | NOP2     | Multi-byte NOP | 0-1 | CPU |
| 0xAB   | EHTRY    | Exception handler try block | 1 | CPU |
| 0xAC   | EHCAT    | Exception handler catch block | 2 | CPU |
| 0xAD   | EHFIN    | Exception handler finally block | 1 | CPU |
| 0xAE   | EHEND    | End exception handler | 0 | CPU |
| 0xAF   | CPUMODE  | Query/set CPU mode | 1-2 | CPU |

## Architecture-Specific Instructions (0xC0-0xCF)

### x86/x64 Architecture (0xC0-0xC7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC0   | RDMSR    | Read model-specific register | 2 | CPU.x86 |
| 0xC1   | WRMSR    | Write model-specific register | 2 | CPU.x86 |
| 0xC2   | CPUID2   | Extended CPU identification | 2-3 | CPU.x86 |
| 0xC3   | RDPMC    | Read performance monitoring counter | 2 | CPU.x86 |
| 0xC4   | LGDT     | Load global descriptor table | 1 | CPU.x86 |
| 0xC5   | SGDT     | Store global descriptor table | 1 | CPU.x86 |
| 0xC6   | LIDT     | Load interrupt descriptor table | 1 | CPU.x86 |
| 0xC7   | SIDT     | Store interrupt descriptor table | 1 | CPU.x86 |

### ARM Architecture (0xC8-0xCF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC8   | BARRIER  | Data memory barrier | 1 | CPU.ARM |
| 0xC9   | WFIEVENT | Wait for event | 0 | CPU.ARM |
| 0xCA   | SEVEVENT | Send event | 0 | CPU.ARM |
| 0xCB   | MRS      | Move from system register | 2 | CPU.ARM |
| 0xCC   | MSR      | Move to system register | 2 | CPU.ARM |
| 0xCD   | ISB      | Instruction synchronization barrier | 0-1 | CPU.ARM |
| 0xCE   | DSB      | Data synchronization barrier | 0-1 | CPU.ARM |
| 0xCF   | DMB      | Data memory barrier | 0-1 | CPU.ARM |

## Mode-Specific Instructions (0xD0-0xDF)

### Privileged Mode Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD0   | SYSPG    | Change system page table | 2 | CPU.Privileged |
| 0xD1   | SYSINT   | Configure interrupt controller | 2-3 | CPU.Privileged |
| 0xD2   | INITTBL  | Initialize interrupt table | 2 | CPU.Privileged |
| 0xD3   | INITVM   | Initialize virtual memory | 2 | CPU.Privileged |
| 0xD4   | PRVMODE  | Change privilege mode | 1 | CPU.Privileged |
| 0xD5   | IOCFG    | Configure I/O | 2-3 | CPU.Privileged |
| 0xD6   | IOMAP    | Map I/O space | 3 | CPU.Privileged |
| 0xD7   | IOPORT   | Direct I/O port access | 2-3 | CPU.Privileged |

### Virtual Machine Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD8   | VMENTRY  | Enter virtual machine | 1-2 | CPU.VM |
| 0xD9   | VMEXIT   | Exit virtual machine | 0-1 | CPU.VM |
| 0xDA   | VMCALL   | Hypercall | 1-3 | CPU.VM |
| 0xDB   | VMCFG    | Configure VM | 2-3 | CPU.VM |
| 0xDC   | VMIOMMU  | Configure I/O MMU | 2-3 | CPU.VM |
| 0xDD   | VMINTR   | Virtual interrupt | 1-2 | CPU.VM |
| 0xDE   | VMREAD   | Read VM control structure | 2 | CPU.VM |
| 0xDF   | VMWRITE  | Write VM control structure | 2 | CPU.VM |

## Detailed Instruction Behaviors

### Interrupt Instructions

#### INT (0xA0)
- Generates a software interrupt with the specified interrupt number
- Pushes flags, return address, and other context to stack
- Transfers control to the appropriate interrupt handler
- Flag effects: None (preserved for handler)

#### IRET (0xA1)
- Returns from an interrupt handler
- Restores flags and other context from stack
- Pops return address and transfers control back
- Flag effects: All flags restored from stack

### x86-Specific Instructions

#### RDMSR (0xC0)
- Reads the Model-Specific Register specified by ECX
- Places the result in EDX:EAX (high:low)
- Privileged instruction, requires ring 0
- Only available on x86/x64 architectures

#### WRMSR (0xC1)
- Writes EDX:EAX (high:low) to the Model-Specific Register specified by ECX
- Privileged instruction, requires ring 0
- Only available on x86/x64 architectures

## Directive-Based Targeting

To target specific CPU features in code, use the preprocessor directives:

```
!if defined(PU_CPU)
  !target CPU
  ; CPU-specific code here
!endif
```

This approach allows for:
1. Multiple processing unit targets in a single module
2. Conditional compilation based on target capabilities
3. Clean separation of architecture-specific code

## Implementation Requirements

CPU implementations must adhere to these guidelines:

1. **Instruction Support**:
   - Processing Unit Instructions (0xA0-0xBF) must be implemented by all CPU targets
   - Architecture-Specific Instructions (0xC0-0xCF) are implemented only for the respective architecture
   - Mode-Specific Instructions (0xD0-0xDF) are implemented only for supported modes

2. **Bare Metal Support**:
   - All instructions must work in bare metal environments
   - No OS dependencies in core instructions
   - OS-specific functionality is provided by the standard library, not CPU instructions

3. **Mode Handling**:
   - Implementations must correctly detect and handle the current architecture mode
   - Invalid instructions for the current mode must generate appropriate exceptions
