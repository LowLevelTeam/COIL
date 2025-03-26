# COIL ISA Organization

## Purpose

This document defines the organization of the COIL Instruction Set Architecture (ISA), establishing the structure of the opcode space, the relationship between different instruction categories, and the principles behind the ISA design.

## Opcode Organization

COIL uses a single byte (8 bits) for opcodes, providing 256 possible instructions. This space is carefully organized into functional categories:

```
0x00      - No Operation
0x01-0x0F - Control Flow Instructions
0x10-0x2F - Memory Operations
0x30-0x4F - Reserved for Multi-Device Operations (v3)
0x50-0x5F - Bit Manipulation Instructions
0x60-0x8F - Arithmetic Instructions
0x90-0x9F - Vector/Array Operations
0xA0-0xAF - Type Instructions
0xB0-0xBF - Directive Instructions
0xC0-0xFE - Processor-Specific Instructions
0xFF      - COIL Processor-Specific Extensions
```

## Universal vs. Processor-Specific Instructions

COIL's instruction set is divided into two main categories:

### Universal Instructions (0x00-0xBF)

These instructions are available on all processing units and must be implemented by all COIL processors:

- **Control Flow (0x01-0x0F)**: Program flow, branching, function calls
- **Memory Operations (0x10-0x2F)**: Data movement, memory access, variable management
- **Reserved (0x30-0x4F)**: Reserved for future multi-device operations
- **Bit Manipulation (0x50-0x5F)**: Logical operations, bit shifting, etc.
- **Arithmetic (0x60-0x8F)**: Mathematical operations
- **Vector/Array (0x90-0x9F)**: Operations on multiple data elements
- **Type (0xA0-0xAF)**: Type system operations
- **Directive (0xB0-0xBF)**: Assembler/processor control

These instructions form the portable foundation of COIL and behave consistently across all processors.

### Processor-Specific Instructions (0xC0-0xFF)

These instructions are specific to certain processor types or architectures:

- **Processor-Specific (0xC0-0xFE)**: Operations that only apply to specific processor types or architectures
- **COIL Processor Extensions (0xFF)**: Implementation-specific extensions

These instructions provide access to specialized hardware features but may not be portable across different processors.

## Instruction Format

All COIL instructions follow a consistent format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

This consistent structure applies to all instructions across the entire opcode space.

## Type-Determined Instruction Philosophy

COIL's ISA follows a type-determined instruction philosophy, where the behavior of an instruction is derived from the types of its operands rather than having many specialized opcodes. This approach allows:

1. **Compact opcode space**: Fewer opcodes cover more functionality
2. **Consistency**: Operations work similarly across different data types
3. **Extensibility**: New types can be added without requiring new opcodes
4. **Simplified implementation**: Instruction decoding follows consistent patterns

For example, a single `ADD` opcode (0x60) works with integers, floating-point values, and vectors, with the specific addition behavior determined by the operand types.

## Conditional Execution

Most COIL instructions support conditional execution based on processor flags:

```
; Basic instruction
BR target

; Conditional variant
BR_EQ target  ; Branch if equal
```

Conditions are specified using the `TYPE_PARAM5` parameter with a `branch_condition_t` value:

```
BRANCH_COND_EQ = 0x00  // Equal
BRANCH_COND_NE = 0x01  // Not equal
BRANCH_COND_GE = 0x02  // Greater than or equal
...
```

## Operand Count Variability

While most instructions have a fixed number of operands, some can accept a variable number:

1. **Fixed operand count**: Most instructions (e.g., `ADD` always has 3 operands)
2. **Variable operand count**: Some instructions like `CALL` with ABI parameters

The operand count is always explicitly encoded in the instruction's binary format.

## Instruction Categories in Detail

### Control Flow Instructions (0x01-0x0F)

Core instructions for controlling program execution:
- **0x01 SYM**: Define a symbol
- **0x02 BR**: Branch to target
- **0x03 CALL**: Call a function
- **0x04 RET**: Return from function
- **0x05 CMP**: Compare values
- **0x06 TEST**: Test bits
- **0x07-0x0F**: Additional control flow operations

### Memory Operations (0x10-0x2F)

Instructions for memory and variable management:
- **0x10 MOV**: Move data between locations
- **0x11 PUSH**: Push to stack / demote variable
- **0x12 POP**: Pop from stack / promote variable
- **0x13 LEA**: Load effective address
- **0x14 SCOPEE**: Enter variable scope
- **0x15 SCOPEL**: Leave variable scope
- **0x16 VAR**: Declare variable
- **0x17 MEMCPY**: Copy memory block
- **0x18 MEMSET**: Set memory block
- **0x19 MEMCMP**: Compare memory blocks
- **0x1A-0x2F**: Additional memory operations

### Reserved Opcode Space (0x30-0x4F)

This range is reserved for multi-device operations in COIL v3. In v1, using these opcodes will result in an error.

### Bit Manipulation Instructions (0x50-0x5F)

Instructions for bitwise operations:
- **0x50 AND**: Bitwise AND
- **0x51 OR**: Bitwise OR
- **0x52 XOR**: Bitwise XOR
- **0x53 NOT**: Bitwise NOT
- **0x54 SHL**: Shift left
- **0x55 SHR**: Shift right logical
- **0x56 SAR**: Shift right arithmetic
- **0x57 ROL**: Rotate left
- **0x58 ROR**: Rotate right
- **0x59 POPCNT**: Population count
- **0x5A BSWAP**: Byte swap
- **0x5B-0x5F**: Additional bit operations

### Arithmetic Instructions (0x60-0x8F)

Mathematical operations:
- **0x60 ADD**: Addition
- **0x61 SUB**: Subtraction
- **0x62 MUL**: Multiplication
- **0x63 DIV**: Division
- **0x64 MOD**: Modulo
- **0x65 INC**: Increment
- **0x66 DEC**: Decrement
- **0x67 NEG**: Negate
- **0x68 ABS**: Absolute value
- **0x69 SQRT**: Square root
- **0x6A FMA**: Fused multiply-add
- **0x6B-0x8F**: Additional arithmetic operations

### Vector/Array Operations (0x90-0x9F)

Operations on multiple data elements:
- **0x90-0x9F**: Vector and array operations

### Type Instructions (0xA0-0xAF)

Operations related to the type system:
- **0xA0 TYPEOF**: Get type of value
- **0xA1 SIZEOF**: Get size of type
- **0xA2 ALIGNOF**: Get alignment of type
- **0xA3 CONVERT**: Convert between types
- **0xA4 CAST**: Reinterpret as different type
- **0xA5 STRUCT**: Define structure type
- **0xA6 GET**: Access structure field
- **0xA7 INDEX**: Access array element
- **0xA8-0xAF**: Additional type operations

### Directive Instructions (0xB0-0xBF)

Instructions that control the assembly and processing:
- **0xB0 ARCH**: Set target architecture
- **0xB1 PROC**: Set processor type
- **0xB3 ALIGN**: Align to boundary
- **0xB4 SECTION**: Define section
- **0xB5 DATA**: Insert data
- **0xB6 IF**: Conditional assembly start
- **0xB7 ELIF**: Conditional assembly alternative
- **0xB8 ELSE**: Conditional assembly default
- **0xB9 ENDIF**: Conditional assembly end
- **0xBA ABI**: ABI definition
- **0xBB-0xBF**: Additional directives

### Processor-Specific Instructions (0xC0-0xFE)

Instructions specific to certain processor types:
- **0xC0-0xFE**: Processor-specific operations

### COIL Processor Extensions (0xFF)

Vendor-specific extensions to the COIL processor:
- **0xFF**: Reserved for implementation-specific extensions

## Binary Format Example

To illustrate the binary encoding:

```
; COIL-ASM: ADD counter, counter, 1
; Binary:
0x60            ; Opcode for ADD
0x03            ; Three operands
0x9000          ; TYPE_VAR for counter
[variable_id]   ; Variable ID
0x9000          ; TYPE_VAR for counter
[variable_id]   ; Same variable ID
0x1320          ; TYPE_UNT32+IMM
0x01000000      ; Value 1 (little-endian)
```

## ISA Evolution

The COIL ISA is designed to evolve through versions while maintaining backward compatibility:

### Version 1
- Complete universal instruction set (0x00-0x2F, 0x50-0xBF)
- Processor-specific instructions for CPUs (subset of 0xC0-0xFE)
- Reserved space for future multi-device operations (0x30-0x4F)

### Version 2
- Enhanced instructions for standard library support
- Additional operations in existing categories
- No changes to opcode organization

### Version 3
- Implementation of multi-device operations (0x30-0x4F)
- Processor-specific instructions for GPUs, TPUs, etc.
- Maintained compatibility with v1 and v2 instructions