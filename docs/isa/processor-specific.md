# Processor-Specific Instructions (0xC0-0xFE)

## Purpose

This document defines the processor-specific instruction range in COIL, which provides access to specialized hardware features while maintaining a clear separation from universal operations. These instructions enable optimization through direct access to processor-specific capabilities.

## Opcode Organization

The processor-specific opcode space is organized as:

```
0xC0-0xCF - Processing unit features (independent of architecture)
0xD0-0xFE - Architecture-specific instructions
```

## Availability Rules

Processor-specific instructions are only available when:

1. The correct processor is selected with the `PROC` directive
2. For architecture-specific instructions, the correct architecture is selected with the `ARCH` directive
3. For mode-specific instructions, the correct mode is selected

Using processor-specific instructions outside their appropriate context results in an error.

## CPU-Specific Instructions (Processor Type 0x01)

### CPU General Instructions (0xC0-0xCF)

These instructions are available on all CPU architectures, with behavior adapted to each architecture.

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xC0   | CPUID    | `CPUID function, results...` | Get processor identification |
| 0xC1   | RDTSC    | `RDTSC [high], [low], [full]` | Read timestamp counter |
| 0xC2   | PREFETCH | `PREFETCH address, type` | Prefetch data into cache |
| 0xC3   | FENCE    | `FENCE type` | Memory fence/barrier |
| 0xC4   | ATOMIC   | `ATOMIC op, dest, [value], [result]` | Atomic operations |
| 0xC5   | LZCNT    | `LZCNT dest, src[, TYPE_PARAM5]` | Count leading zeros |
| 0xC6   | TZCNT    | `TZCNT dest, src[, TYPE_PARAM5]` | Count trailing zeros |

#### CPUID (0xC0)
Get processor identification and feature information.

##### Assembly Syntax
```
CPUID function, result1, result2, result3, result4
```

##### Binary Encoding
```
0xC0                  ; Opcode for CPUID
0x05                  ; Five operands
[func_type]           ; Type of function number
[func_value]          ; Function number value
[res1_type]           ; Type of first result
[res1_value]          ; First result value
[res2_type]           ; Type of second result
[res2_value]          ; Second result value
[res3_type]           ; Type of third result
[res3_value]          ; Third result value
[res4_type]           ; Type of fourth result
[res4_value]          ; Fourth result value
```

##### Example
```
; Assembly (x86)
VAR TYPE_UNT32, function, 0
VAR TYPE_UNT32, eax_result, ebx_result, ecx_result, edx_result
CPUID function, eax_result, ebx_result, ecx_result, edx_result

; Binary
0xC0      ; CPUID
0x05      ; Five operands
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "function"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "eax_result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "ebx_result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "ecx_result"
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "edx_result"
```

#### RDTSC (0xC1)
Read time-stamp counter.

##### Assembly Syntax
```
RDTSC [high], [low], [full]
```

The operands are optional but at least one must be provided.

##### Binary Encoding
```
0xC1                  ; Opcode for RDTSC
0x01-0x03             ; One to three operands
[high_type]           ; Type of high 32 bits destination (optional)
[high_value]          ; High 32 bits destination value (optional)
[low_type]            ; Type of low 32 bits destination (optional)
[low_value]           ; Low 32 bits destination value (optional)
[full_type]           ; Type of 64-bit destination (optional)
[full_value]          ; 64-bit destination value (optional)
```

##### Example
```
; Assembly (read into 64-bit value)
VAR TYPE_UNT64, timestamp
RDTSC timestamp

; Binary
0xC1      ; RDTSC
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "timestamp"
```

#### FENCE (0xC3)
Memory fence/barrier for cross-thread synchronization.

##### Assembly Syntax
```
FENCE type
```

##### Binary Encoding
```
0xC3                  ; Opcode for FENCE
0x01                  ; One operand
[type_field]          ; Type of fence
[type_value]          ; Fence type value
```

##### Fence Types
- `0x00`: Full fence (StoreLoad barrier)
- `0x01`: Load fence (LoadLoad and LoadStore barrier)
- `0x02`: Store fence (StoreStore and LoadStore barrier)

##### Example
```
; Assembly
FENCE 0  ; Full memory barrier

; Binary
0xC3      ; FENCE
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x00      ; Full fence (0x00)
```

### Architecture-Specific Instructions (0xD0-0xFE)

These instructions are specific to particular CPU architectures and are only available when the appropriate architecture is selected.

#### x86-Specific Instructions (ARCH 0x01)

Available when the processor type is CPU (0x01) and architecture is x86 (0x01):

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xD0   | RDRAND   | `RDRAND dest` | Generate random number using hardware RNG |
| 0xD1   | RDMSR    | `RDMSR register, high, low` | Read Model Specific Register |
| 0xD2   | WRMSR    | `WRMSR register, high, low` | Write Model Specific Register |
| 0xD3   | XSAVE    | `XSAVE address, mask` | Save extended processor state |
| 0xD4   | XRSTOR   | `XRSTOR address, mask` | Restore extended processor state |

##### RDRAND (0xD0)
Generate random number using hardware RNG.

###### Assembly Syntax
```
RDRAND dest
```

###### Binary Encoding
```
0xD0                  ; Opcode for RDRAND
0x01                  ; One operand
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
```

###### Example
```
; Assembly
VAR TYPE_UNT64, random_value
RDRAND random_value

; Binary
0xD0      ; RDRAND
0x01      ; One operand
0x9000    ; TYPE_VAR
[id]      ; Variable ID for "random_value"
```

#### ARM-Specific Instructions (ARCH 0x02)

Available when the processor type is CPU (0x01) and architecture is ARM (0x02):

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xD0   | DMBER    | `DMBER type` | Data Memory Barrier |
| 0xD1   | SEVL     | `SEVL` | Send event locally |
| 0xD2   | WFE      | `WFE` | Wait for event |
| 0xD3   | WFI      | `WFI` | Wait for interrupt |

##### DMBER (0xD0)
Data Memory Barrier.

###### Assembly Syntax
```
DMBER type
```

###### Binary Encoding
```
0xD0                  ; Opcode for DMBER
0x01                  ; One operand
[type_field]          ; Type of barrier
[type_value]          ; Barrier type value
```

###### Example
```
; Assembly
DMBER 0  ; Full data memory barrier

; Binary
0xD0      ; DMBER
0x01      ; One operand
0x1020    ; TYPE_UNT8+IMM
0x00      ; Full barrier (0x00)
```

#### RISC-V-Specific Instructions (ARCH 0x03)

Available when the processor type is CPU (0x01) and architecture is RISC-V (0x03):

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xD0   | FENCE.I  | `FENCE.I` | Instruction fence |
| 0xD1   | LR       | `LR dest, address` | Load-Reserved |
| 0xD2   | SC       | `SC success, address, value` | Store-Conditional |

##### FENCE.I (0xD0)
Instruction fence.

###### Assembly Syntax
```
FENCE.I
```

###### Binary Encoding
```
0xD0                  ; Opcode for FENCE.I
0x00                  ; No operands
```

## GPU-Specific Instructions (Processor Type 0x02, v3)

While COIL v1 focuses on CPU instructions, the opcode space 0xC0-0xFE for processor type 0x02 (GPU) is reserved for COIL v3.

## TPU-Specific Instructions (Processor Type 0x03, v3)

While COIL v1 focuses on CPU instructions, the opcode space 0xC0-0xFE for processor type 0x03 (TPU) is reserved for COIL v3.

## Implementation Guidance

### Using Processor-Specific Instructions

Processor-specific instructions should be used with care:

1. **Check processor support**: Use CPUID or equivalent to verify hardware support before using specialized instructions
2. **Provide fallbacks**: When using specialized instructions, provide fallback implementations for unsupported processors
3. **Use conditional assembly**: Wrap processor-specific code in conditional assembly blocks

Example with fallback:
```
; Check for RDRAND support and use it if available
IF ARCH == 0x01  ; x86
  ; Check CPUID for RDRAND support
  VAR TYPE_UNT32, function, 1
  VAR TYPE_UNT32, eax, ebx, ecx, edx
  CPUID function, eax, ebx, ecx, edx
  
  ; Test bit 30 of ECX for RDRAND support
  AND rdrand_support, ecx, 0x40000000  ; Bit 30
  CMP rdrand_support, 0
  BR_EQ no_rdrand_support
  
  ; Use RDRAND
  RDRAND random_value
  BR have_random
  
  SYM no_rdrand_support
ENDIF

; Fallback random implementation
; ...

SYM have_random
```

### Portable Code with Optimizations

For maximum portability with optimizations:

1. **Use universal instructions by default**
2. **Add processor-specific optimizations in conditional blocks**
3. **Always provide a fallback implementation**

Example:
```
; Function with architecture-specific optimizations
SYM optimized_function
  IF ARCH == 0x01  ; x86
    ; x86 optimized version
    ; ...
  ELIF ARCH == 0x02  ; ARM
    ; ARM optimized version
    ; ...
  ELSE
    ; Generic implementation
    ; ...
  ENDIF
```

## Extension Mechanism

### Custom Processor (0xFF)

The processor code 0xFF is reserved for custom processors. When used with PROC 0xFF, opcodes 0xC0-0xFE can be defined by the implementation.

### COIL Processor Extensions (0xFF)

The opcode 0xFF is reserved for COIL processor-specific extensions across all processor types. The behavior is defined by the specific COIL processor implementation.