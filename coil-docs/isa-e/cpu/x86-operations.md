# x86-Specific CPU Operations

## Overview

x86-specific operations are extended instructions that work specifically on the x86 and x86-64 CPU architectures. These instructions provide access to unique features of the x86 instruction set architecture, including specific registers, addressing modes, and hardware capabilities.

## Architecture Support

These operations use the architecture-specific operation range (0xC0-0xDF) and are only available when:
1. The current processing unit is CPU (`PU_TYPE EQ CPU`)
2. The current architecture is x86 (`ARCH_TYPE EQ X86`)

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xC0   | CPUID    | CPU identification |
| 0xC1   | IN       | Input from port |
| 0xC2   | OUT      | Output to port |
| 0xC3   | CLI      | Clear interrupt flag |
| 0xC4   | STI      | Set interrupt flag |
| 0xC5   | INVD     | Invalidate cache |
| 0xC6   | WBINVD   | Write back and invalidate cache |
| 0xC7   | LGDT     | Load global descriptor table |
| 0xC8   | LIDT     | Load interrupt descriptor table |
| 0xC9   | LLDT     | Load local descriptor table |
| 0xCA   | LTR      | Load task register |
| 0xCB   | INVLPG   | Invalidate TLB entry |
| 0xCC   | RDMSR    | Read model-specific register |
| 0xCD   | WRMSR    | Write model-specific register |
| 0xCE   | RDPMC    | Read performance monitoring counter |
| 0xCF   | RDTSC    | Read time-stamp counter |

## Detailed Specifications

### CPUID (0xC0)

Queries processor identification and feature information.

**Encoding:**
```
// Basic CPUID
[0xC0][0x02][function: Operand][result: Operand]

// Extended CPUID
[0xC0][0x03][function: Operand][subfunction: Operand][result: Operand]
```

**Behavior:**
- Executes the x86 CPUID instruction with the specified function number
- Function must be an integral type
- Result must be a writable location that can store 4 32-bit values (EAX, EBX, ECX, EDX)
- For extended form, subfunction is passed in ECX
- Sets no flags
- Requires user or supervisor privilege depending on function

### IN (0xC1)

Reads from an I/O port.

**Encoding:**
```
[0xC1][0x02][dest: Operand][port: Operand]
```

**Behavior:**
- Reads a value from the specified I/O port
- Port must be an 8-bit or 16-bit integral type
- Destination size determines the transfer size (byte, word, or dword)
- Sets no flags
- Typically requires supervisor privilege

### OUT (0xC2)

Writes to an I/O port.

**Encoding:**
```
[0xC2][0x02][port: Operand][value: Operand]
```

**Behavior:**
- Writes a value to the specified I/O port
- Port must be an 8-bit or 16-bit integral type
- Value size determines the transfer size (byte, word, or dword)
- Sets no flags
- Typically requires supervisor privilege

### CLI (0xC3)

Clears the interrupt flag, disabling maskable interrupts.

**Encoding:**
```
[0xC3][0x00]
```

**Behavior:**
- Clears the IF flag in the EFLAGS register
- No operands required
- Sets no flags
- Requires supervisor privilege

### STI (0xC4)

Sets the interrupt flag, enabling maskable interrupts.

**Encoding:**
```
[0xC4][0x00]
```

**Behavior:**
- Sets the IF flag in the EFLAGS register
- No operands required
- Sets no flags
- Requires supervisor privilege

### INVD (0xC5)

Invalidates internal caches.

**Encoding:**
```
[0xC5][0x00]
```

**Behavior:**
- Invalidates all internal processor caches
- Does not write back modified cache lines
- No operands required
- Sets no flags
- Requires supervisor privilege

### WBINVD (0xC6)

Writes back and invalidates internal caches.

**Encoding:**
```
[0xC6][0x00]
```

**Behavior:**
- Writes back all modified cache lines to memory
- Invalidates all internal processor caches
- No operands required
- Sets no flags
- Requires supervisor privilege

### LGDT (0xC7)

Loads the global descriptor table register.

**Encoding:**
```
[0xC7][0x01][src: Operand]
```

**Behavior:**
- Loads the GDTR from the specified memory location
- Source must be a 6-byte memory operand (16-bit limit, 32/64-bit base)
- Sets no flags
- Requires supervisor privilege

### LIDT (0xC8)

Loads the interrupt descriptor table register.

**Encoding:**
```
[0xC8][0x01][src: Operand]
```

**Behavior:**
- Loads the IDTR from the specified memory location
- Source must be a 6-byte memory operand (16-bit limit, 32/64-bit base)
- Sets no flags
- Requires supervisor privilege

### LLDT (0xC9)

Loads the local descriptor table register.

**Encoding:**
```
[0xC9][0x01][src: Operand]
```

**Behavior:**
- Loads the LDTR from the specified selector
- Source must be a 16-bit selector
- Sets no flags
- Requires supervisor privilege

### LTR (0xCA)

Loads the task register.

**Encoding:**
```
[0xCA][0x01][src: Operand]
```

**Behavior:**
- Loads the task register from the specified selector
- Source must be a 16-bit selector
- Sets no flags
- Requires supervisor privilege

### INVLPG (0xCB)

Invalidates a TLB entry.

**Encoding:**
```
[0xCB][0x01][addr: Operand]
```

**Behavior:**
- Invalidates the TLB entry for the specified address
- Address must be a memory operand
- Sets no flags
- Requires supervisor privilege

### RDMSR (0xCC)

Reads a model-specific register.

**Encoding:**
```
[0xCC][0x02][ecx_value: Operand][result: Operand]
```

**Behavior:**
- Reads the MSR specified by the ECX value
- ECX value must be an integral type
- Result must be a writable location that can store 2 32-bit values (EDX:EAX)
- Sets no flags
- Requires supervisor privilege

### WRMSR (0xCD)

Writes a model-specific register.

**Encoding:**
```
[0xCD][0x02][ecx_value: Operand][edx_eax: Operand]
```

**Behavior:**
- Writes the MSR specified by the ECX value
- ECX value must be an integral type
- EDX:EAX values must be a 64-bit value or two 32-bit values
- Sets no flags
- Requires supervisor privilege

### RDPMC (0xCE)

Reads a performance monitoring counter.

**Encoding:**
```
[0xCE][0x02][ecx_value: Operand][result: Operand]
```

**Behavior:**
- Reads the performance counter specified by the ECX value
- ECX value must be an integral type
- Result must be a writable location that can store 2 32-bit values (EDX:EAX)
- Sets no flags
- Privilege level depends on CR4.PCE

### RDTSC (0xCF)

Reads the processor's time-stamp counter.

**Encoding:**
```
[0xCF][0x01][result: Operand]
```

**Behavior:**
- Reads the time-stamp counter into the destination
- Result must be a writable location that can store 2 32-bit values (EDX:EAX) or a 64-bit value
- Sets no flags
- Privilege level depends on CR4.TSD

## x86 Register Mapping

The following register mappings are used for x86 architecture:

### 16-bit mode

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0000      | AX   | Accumulator |
| 0x0001      | BX   | Base |
| 0x0002      | CX   | Counter |
| 0x0003      | DX   | Data |
| 0x0004      | SI   | Source Index |
| 0x0005      | DI   | Destination Index |
| 0x0006      | BP   | Base Pointer |
| 0x0007      | SP   | Stack Pointer |
| 0x0008      | CS   | Code Segment |
| 0x0009      | DS   | Data Segment |
| 0x000A      | ES   | Extra Segment |
| 0x000B      | SS   | Stack Segment |
| 0x000C      | FS   | F Segment |
| 0x000D      | GS   | G Segment |

### 32-bit mode (additional registers)

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0010      | EAX  | Extended Accumulator |
| 0x0011      | EBX  | Extended Base |
| 0x0012      | ECX  | Extended Counter |
| 0x0013      | EDX  | Extended Data |
| 0x0014      | ESI  | Extended Source Index |
| 0x0015      | EDI  | Extended Destination Index |
| 0x0016      | EBP  | Extended Base Pointer |
| 0x0017      | ESP  | Extended Stack Pointer |
| 0x0018      | CR0  | Control Register 0 |
| 0x0019      | CR2  | Control Register 2 |
| 0x001A      | CR3  | Control Register 3 |
| 0x001B      | CR4  | Control Register 4 |

### 64-bit mode (additional registers)

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0020      | RAX  | 64-bit Accumulator |
| 0x0021      | RBX  | 64-bit Base |
| 0x0022      | RCX  | 64-bit Counter |
| 0x0023      | RDX  | 64-bit Data |
| 0x0024      | RSI  | 64-bit Source Index |
| 0x0025      | RDI  | 64-bit Destination Index |
| 0x0026      | RBP  | 64-bit Base Pointer |
| 0x0027      | RSP  | 64-bit Stack Pointer |
| 0x0028      | R8   | 64-bit Register 8 |
| 0x0029      | R9   | 64-bit Register 9 |
| 0x002A      | R10  | 64-bit Register 10 |
| 0x002B      | R11  | 64-bit Register 11 |
| 0x002C      | R12  | 64-bit Register 12 |
| 0x002D      | R13  | 64-bit Register 13 |
| 0x002E      | R14  | 64-bit Register 14 |
| 0x002F      | R15  | 64-bit Register 15 |
| 0x0030      | CR8  | Control Register 8 |

## Conditional Compilation

Always wrap x86-specific operations in appropriate conditional blocks:

```
IF PU_TYPE EQ CPU
    IF ARCH_TYPE EQ X86
        // x86-specific code using x86 operations
    ENDIF
ENDIF
```

## Related Components

- [CPU Operations](./cpu-operations.md) - General CPU operations
- [Extended ISA Overview](./overview.md) - Extended instruction set summary
- [Universal ISA](../isa-u/overview.md) - Platform-independent instructions
- [Memory Model](../core/memory-model.md) - Memory management and organization