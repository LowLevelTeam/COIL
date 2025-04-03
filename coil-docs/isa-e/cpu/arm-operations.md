# ARM-Specific CPU Operations

## Overview

ARM-specific operations are extended instructions that work specifically on ARM CPU architectures. These instructions provide access to unique features of the ARM instruction set architecture, including specific registers, addressing modes, and hardware capabilities.

## Architecture Support

These operations use the architecture-specific operation range (0xC0-0xDF) and are only available when:
1. The current processing unit is CPU (`PU_TYPE EQ CPU`)
2. The current architecture is ARM (`ARCH_TYPE EQ ARM`)

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xC1   | DMB      | Data Memory Barrier |
| 0xC2   | DSB      | Data Synchronization Barrier |
| 0xC3   | ISB      | Instruction Synchronization Barrier |
| 0xC4   | VMSR     | Move to Special Register |
| 0xC5   | VMRS     | Move from Special Register |
| 0xC6   | MRS      | Move from System Register |
| 0xC7   | MSR      | Move to System Register |
| 0xC8   | CPS      | Change Processor State |
| 0xC9   | SETEND   | Set Endianness |
| 0xCA   | PLD      | Preload Data |
| 0xCB   | PLI      | Preload Instructions |
| 0xCC   | WFI      | Wait For Interrupt |
| 0xCD   | WFE      | Wait For Event |
| 0xCE   | SEV      | Send Event |
| 0xCF   | SEVL     | Send Event Local |

## Detailed Specifications

### DMB (0xC1)

Performs a Data Memory Barrier operation.

**Encoding:**
```
[0xC1][0x01][option: Operand]
```

**Behavior:**
- Ensures that all explicit memory accesses before the barrier complete before any explicit memory accesses after the barrier
- Option specifies the barrier domain (SY, ST, ISH, ISHST, NSH, NSHST, OSH, OSHST)
- If option is omitted, defaults to full system barrier (SY)
- Sets no flags
- Available in ARMv7 and later

### DSB (0xC2)

Performs a Data Synchronization Barrier operation.

**Encoding:**
```
[0xC2][0x01][option: Operand]
```

**Behavior:**
- More restrictive than DMB, ensures all instructions following the barrier do not execute until all memory accesses before the barrier are complete
- Option specifies the barrier domain (SY, ST, ISH, ISHST, NSH, NSHST, OSH, OSHST)
- If option is omitted, defaults to full system barrier (SY)
- Sets no flags
- Available in ARMv7 and later

### ISB (0xC3)

Performs an Instruction Synchronization Barrier operation.

**Encoding:**
```
[0xC3][0x01][option: Operand]
```

**Behavior:**
- Flushes the pipeline and ensures all instructions after the barrier are fetched from cache or memory after the barrier has completed
- Option is typically SY (system) or ignored
- Used after changing program flow or instruction memory
- Sets no flags
- Available in ARMv7 and later

### VMSR (0xC4)

Moves a value to a special SIMD/Floating-point register.

**Encoding:**
```
[0xC4][0x02][special_reg: Operand][value: Operand]
```

**Behavior:**
- Moves a value to a special SIMD or floating-point register (FPSCR, FPSID, etc.)
- Special register must be an immediate value or register representing the special register
- Value must be a 32-bit integral type
- Sets no flags
- May require privileged mode depending on the register

### VMRS (0xC5)

Moves a value from a special SIMD/Floating-point register.

**Encoding:**
```
[0xC5][0x02][dest: Operand][special_reg: Operand]
```

**Behavior:**
- Moves a value from a special SIMD or floating-point register (FPSCR, FPSID, etc.)
- Special register must be an immediate value or register representing the special register
- Destination must be a writable location that can store a 32-bit value
- Sets no flags
- May require privileged mode depending on the register

### MRS (0xC6)

Moves a value from a system register.

**Encoding:**
```
[0xC6][0x02][dest: Operand][system_reg: Operand]
```

**Behavior:**
- Reads a system register (CPSR, SPSR, etc.)
- System register must be an immediate value or register representing the system register
- Destination must be a writable location that can store a 32-bit value
- Sets no flags
- May require privileged mode depending on the register

### MSR (0xC7)

Moves a value to a system register.

**Encoding:**
```
[0xC7][0x02][system_reg: Operand][value: Operand]
```

**Behavior:**
- Writes a value to a system register (CPSR, SPSR, etc.)
- System register must be an immediate value or register representing the system register
- Value must be a 32-bit integral type
- Sets no flags
- Usually requires privileged mode

### CPS (0xC8)

Changes processor state.

**Encoding:**
```
[0xC8][0x01][mode: Operand]
```

**Behavior:**
- Changes processor mode, enables/disables interrupts, or both
- Mode specifies the new mode and/or interrupt enable/disable flags
- Requires privileged mode
- Sets no flags
- Immediate effect on processor state

### SETEND (0xC9)

Sets the endianness for data accesses.

**Encoding:**
```
[0xC9][0x01][endian: Operand]
```

**Behavior:**
- Sets the E bit in the CPSR to change the data access endianness
- Endian must be an immediate value (0 for little endian, 1 for big endian)
- Sets no flags
- Available in ARMv6 and later, deprecated in ARMv7 and later

### PLD (0xCA)

Preloads data into cache.

**Encoding:**
```
[0xCA][0x01][addr: Operand]
```

**Behavior:**
- Preloads data from the specified address into cache
- Non-blocking operation (execution continues immediately)
- Treated as a hint (processor may ignore it)
- Sets no flags
- Available in ARMv5TE and later

### PLI (0xCB)

Preloads instructions into cache.

**Encoding:**
```
[0xCB][0x01][addr: Operand]
```

**Behavior:**
- Preloads instructions from the specified address into cache
- Non-blocking operation (execution continues immediately)
- Treated as a hint (processor may ignore it)
- Sets no flags
- Available in ARMv7 and later

### WFI (0xCC)

Waits for an interrupt.

**Encoding:**
```
[0xCC][0x00]
```

**Behavior:**
- Places the processor in a low-power state until an interrupt occurs
- Execution resumes when an interrupt is received
- Implementation-defined behavior (may vary between processors)
- Sets no flags
- Available in ARMv6 and later

### WFE (0xCD)

Waits for an event.

**Encoding:**
```
[0xCD][0x00]
```

**Behavior:**
- Places the processor in a low-power state until an event occurs
- Execution resumes when an event register is set
- Implementation-defined behavior (may vary between processors)
- Sets no flags
- Available in ARMv6 and later

### SEV (0xCE)

Sends an event to all processors in the multiprocessor system.

**Encoding:**
```
[0xCE][0x00]
```

**Behavior:**
- Sets the event register of all processors in the system
- Used to wake processors from WFE state
- Sets no flags
- Available in ARMv6K and later

### SEVL (0xCF)

Sends an event locally to the processor.

**Encoding:**
```
[0xCF][0x00]
```

**Behavior:**
- Sets the local event register
- Does not affect other processors in the system
- Typically used in combination with WFE for spinlocks
- Sets no flags
- Available in ARMv8 and later

## ARM Register Mapping

The following register mappings are used for ARM architecture:

### Core Registers

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0000      | R0   | General-purpose register 0 |
| 0x0001      | R1   | General-purpose register 1 |
| 0x0002      | R2   | General-purpose register 2 |
| 0x0003      | R3   | General-purpose register 3 |
| 0x0004      | R4   | General-purpose register 4 |
| 0x0005      | R5   | General-purpose register 5 |
| 0x0006      | R6   | General-purpose register 6 |
| 0x0007      | R7   | General-purpose register 7 |
| 0x0008      | R8   | General-purpose register 8 |
| 0x0009      | R9   | General-purpose register 9 |
| 0x000A      | R10  | General-purpose register 10 |
| 0x000B      | R11  | General-purpose register 11 |
| 0x000C      | R12  | General-purpose register 12 |
| 0x000D      | SP   | Stack Pointer (R13) |
| 0x000E      | LR   | Link Register (R14) |
| 0x000F      | PC   | Program Counter (R15) |

### Special Registers

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0010      | CPSR | Current Program Status Register |
| 0x0011      | SPSR | Saved Program Status Register |
| 0x0012      | FPSCR| Floating-Point Status and Control Register |

### ARMv8 64-bit Registers (AArch64 mode)

| Register ID | Name | Description |
|-------------|------|-------------|
| 0x0020      | X0   | 64-bit General-purpose register 0 |
| 0x0021      | X1   | 64-bit General-purpose register 1 |
| ...         | ...  | ... |
| 0x002F      | X15  | 64-bit General-purpose register 15 |
| 0x0030      | X16  | 64-bit General-purpose register 16 |
| ...         | ...  | ... |
| 0x003F      | X31  | 64-bit General-purpose register 31 |
| 0x0040      | SP_EL0 | Stack Pointer EL0 |
| 0x0041      | SP_EL1 | Stack Pointer EL1 |
| 0x0042      | SP_EL2 | Stack Pointer EL2 |
| 0x0043      | SP_EL3 | Stack Pointer EL3 |

## Conditional Compilation

Always wrap ARM-specific operations in appropriate conditional blocks:

```
IF PU_TYPE EQ CPU
    IF ARCH_TYPE EQ ARM
        // ARM-specific code using ARM operations
    ENDIF
ENDIF
```

## Related Components

- [CPU Operations](./cpu-operations.md) - General CPU operations
- [Extended ISA Overview](./overview.md) - Extended instruction set summary
- [Universal ISA](../isa-u/overview.md) - Platform-independent instructions
- [Memory Model](../core/memory-model.md) - Memory management and organization