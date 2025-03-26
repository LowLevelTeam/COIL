# Processor-Specific Instructions (0xC0-0xFE)

## Purpose

This document defines how processor-specific instructions are organized within COIL. These instructions enable direct access to specialized processor features while maintaining a clear separation from universal operations. Understanding this separation is key to writing both portable and optimized COIL code.

## Processor-Specific Instruction Space

COIL reserves the opcode range 0xC0-0xFE for processor-specific instructions:

```
0xC0-0xCF - Processing unit features (independent of architecture)
0xD0-0xFE - Architecture-specific instructions
```

These instructions are only valid in the context of the appropriate processor (set via the `PROC` directive) and possibly architecture (set via the `ARCH` directive).

## Instruction Availability

Processor-specific instructions are only available after setting the processor context:

```
PROC 0x01       ; Select CPU processor
ARCH 0x01, 0x03 ; Select x86-64 architecture
```

Using a processor-specific instruction outside of its appropriate context results in an error.

## CPU-Specific Instructions (Processor Type 0x01)

The following instructions are available when the processor type is set to CPU (0x01).

### CPU General Instructions (0xC0-0xCF)

These instructions are available on all CPU architectures, with behavior adapted to each architecture:

#### CPUID (0xC0)
Get processor identification and feature information.

```
Operands:
- Function: TYPE_UNT32
- Result: Multiple destinations for returned information
```

Example:
```
; Get CPU vendor string
VAR TYPE_UNT32, function, 0
VAR TYPE_UNT32, eax_result, ebx_result, ecx_result, edx_result
CPUID function, eax_result, ebx_result, ecx_result, edx_result

; The CPU vendor string is now in ebx_result, edx_result, ecx_result
```

#### RDTSC (0xC1)
Read time-stamp counter.

```
Operands:
- High: Destination for high 32 bits (optional)
- Low: Destination for low 32 bits (optional)
- Full: 64-bit destination (optional)
```

Example:
```
; Read timestamp counter into a 64-bit value
VAR TYPE_UNT64, timestamp
RDTSC timestamp

; Alternative: split into high and low parts
VAR TYPE_UNT32, high, low
RDTSC high, low
```

#### PREFETCH (0xC2)
Prefetch data into cache.

```
Operands:
- Address: Memory address to prefetch
- Type: Prefetch type
  - 0: Prefetch for read (NTA - non-temporal access)
  - 1: Prefetch for read (T1 - temporal locality level 1)
  - 2: Prefetch for read (T2 - temporal locality level 2)
  - 3: Prefetch for write
```

Example:
```
; Prefetch data for reading
VAR TYPE_PTR, data_ptr, large_array
PREFETCH data_ptr, 1  ; T1 read prefetch
```

#### FENCE (0xC3)
Memory fence/barrier for cross-thread synchronization.

```
Operands:
- Type: Fence type
  - 0: Full fence (StoreLoad barrier)
  - 1: Load fence (LoadLoad and LoadStore barrier)
  - 2: Store fence (StoreStore and LoadStore barrier)
```

Example:
```
; Full memory barrier
FENCE 0

; Store fence (ensure all previous stores are visible)
FENCE 2
```

#### ATOMIC (0xC4)
Atomic operations on memory.

```
Operands:
- Operation: Operation type
  - 0: Atomic add
  - 1: Atomic subtract
  - 2: Atomic increment
  - 3: Atomic decrement
  - 4: Atomic AND
  - 5: Atomic OR
  - 6: Atomic XOR
- Destination: Memory address
- Value: Value for operation (for operations that need it)
- Result: Destination for previous value (optional)
```

Example:
```
; Atomic increment
VAR TYPE_PTR=TYPE_INT32, counter_ptr, shared_counter
ATOMIC 2, counter_ptr  ; Increment atomically

; Atomic add with previous value
VAR TYPE_INT32, old_value
ATOMIC 0, counter_ptr, 10, old_value
```

#### LZCNT/TZCNT (0xC5/0xC6)
Count leading/trailing zeros.

```
Operands:
- Destination: Result
- Source: Value to count zeros in
```

Example:
```
; Count leading zeros
VAR TYPE_UNT32, value, 0x00001000  ; Binary: 0...01...0
VAR TYPE_UNT32, leading_zeros
LZCNT leading_zeros, value  ; Will be 19 for 32-bit value

; Count trailing zeros
VAR TYPE_UNT32, trailing_zeros
TZCNT trailing_zeros, value  ; Will be 12
```

### Architecture-Specific Instructions (0xD0-0xFE)

These instructions are specific to particular CPU architectures and are only available when the appropriate architecture is selected.

#### x86-Specific Instructions (ARCH 0x01)

Available when the processor type is CPU (0x01) and architecture is x86 (0x01):

##### RDRAND (0xD0)
Generate random number using hardware RNG.

```
Operands:
- Destination: Destination for random value
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x01       ; x86

; Generate random value
VAR TYPE_UNT64, random_value
RDRAND random_value
```

##### RDMSR/WRMSR (0xD1/0xD2)
Read/write Model Specific Register.

```
Operands for RDMSR:
- Register: MSR number
- Result High: Destination for high 32 bits
- Result Low: Destination for low 32 bits

Operands for WRMSR:
- Register: MSR number
- Value High: High 32 bits to write
- Value Low: Low 32 bits to write
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x01       ; x86

; Read from MSR 0xC0000082 (LSTAR on x86-64)
VAR TYPE_UNT32, msr_number, 0xC0000082
VAR TYPE_UNT32, high, low
RDMSR msr_number, high, low
```

##### XSAVE/XRSTOR (0xD3/0xD4)
Save/restore extended processor state.

```
Operands:
- Address: Memory address for state
- Mask: Components to save/restore
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x01       ; x86

; Save extended state
VAR TYPE_PTR, state_buffer, buffer_address
VAR TYPE_UNT64, mask, 0x03  ; Save x87 FPU and SSE state
XSAVE state_buffer, mask
```

#### ARM-Specific Instructions (ARCH 0x02)

Available when the processor type is CPU (0x01) and architecture is ARM (0x02):

##### DMBER (0xD0)
Data Memory Barrier.

```
Operands:
- Type: Barrier type
  - 0: Full barrier
  - 1: Inner Shareable barrier
  - 2: Outer Shareable barrier
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x02       ; ARM

; Full data memory barrier
DMBER 0
```

##### SEVL/WFE (0xD1/0xD2)
Send event locally/Wait for event.

```
Operands:
- None
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x02       ; ARM

; Send event locally
SEVL

; Wait for event
WFE
```

#### RISC-V-Specific Instructions (ARCH 0x03)

Available when the processor type is CPU (0x01) and architecture is RISC-V (0x03):

##### FENCE.I (0xD0)
Instruction fence.

```
Operands:
- None
```

Example:
```
PROC 0x01       ; CPU
ARCH 0x03       ; RISC-V

; Instruction fence
FENCE.I
```

##### LR/SC (0xD1/0xD2)
Load-Reserved/Store-Conditional for atomic operations.

```
Operands for LR:
- Destination: Register to receive value
- Address: Memory address

Operands for SC:
- Success: Register to receive success/failure (1/0)
- Address: Memory address
- Value: Value to store
```

Example: