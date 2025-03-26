# COIL Binary Encoding

## Purpose

This document defines the fundamental binary encoding rules for COIL instructions, providing the technical foundation for how instructions are represented in binary format.

## Instruction Encoding

Every COIL instruction is encoded as a sequence of bytes following this structure:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

The total instruction size varies based on the number and type of operands.

### Opcode Field

The opcode field is a single byte that identifies the operation to perform:
- Values 0x00-0xBF: Universal instructions
- Values 0xC0-0xFF: Processor-specific instructions

### Operand Count Field

A single byte indicating how many operands follow:
- 0x00: No operands
- 0x01: One operand
- 0x02: Two operands
- ...

Most instructions have a fixed operand count, but some (like CALL with parameters) may have a variable number.

## Operand Encoding

Each operand is encoded as:

```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

### Type Field

The type field is a 16-bit value divided into two parts:
- First 8 bits: Main Type - Identifies the primary type category
- Second 8 bits: Type Extensions - Provides qualifiers (const, volatile, etc.)

Example type encodings:
```
0x0300 - TYPE_INT32 (32-bit signed integer)
0x0301 - TYPE_INT32+CONST (constant 32-bit signed integer)
0x1400 - TYPE_UNT64 (64-bit unsigned integer)
0x9200 - TYPE_RGP (general purpose register)
```

### Type-Specific Data

Some types require additional data beyond the 16-bit type descriptor:

1. **Register types** include a register identifier:
   ```
   0x9200 0x05    ; TYPE_RGP=RDI (on x86-64)
   ```

2. **Composite types** include additional type information:
   ```
   0xD300 0x1000  ; TYPE_ARRAY=TYPE_UNT8
   ```

3. **Parameter types** include parameter-specific data:
   ```
   0xF800 0x0100 [ABI_ID]  ; TYPE_ABICTL=ABICTL_PARAM=abi_name
   ```

### Value Field

The value field contains the actual data, with size and encoding determined by the type:

1. **Immediate integer values** are encoded in little-endian format:
   ```
   0x1320 0x2A000000  ; TYPE_UNT32+IMM with value 42
   ```

2. **Floating-point values** use IEEE format:
   ```
   0x2520 [IEEE 754 encoding]  ; TYPE_FP32+IMM with value
   ```

3. **Symbol references** include a symbol ID:
   ```
   0x9100 [symbol_id]  ; TYPE_SYM with symbol ID
   ```

4. **Variable references** include a variable ID:
   ```
   0x9000 [variable_id]  ; TYPE_VAR with variable ID
   ```

## Alignment and Endianness

- Instructions are not required to be aligned on any particular boundary
- Multi-byte values in operands follow little-endian encoding by default
- No padding is inserted between instruction fields or between instructions

## Instruction Size

Instruction size varies based on:
1. Number of operands
2. Types of operands
3. Size of operand values

The minimum instruction size is 2 bytes (opcode + operand count with no operands).

## Encoding Examples

### Simple Instruction: NOP

```
; Assembly: NOP
; Binary:
0x00      ; Opcode for NOP
0x00      ; Zero operands
```
Total size: 2 bytes

### Basic Instruction: MOV with Register and Immediate

```
; Assembly: MOV TYPE_RGP=RAX, 42
; Binary:
0x10      ; Opcode for MOV
0x02      ; Two operands
0x9200    ; TYPE_RGP
0x00      ; Register ID for RAX
0x1320    ; TYPE_UNT32+IMM
0x2A000000 ; Value 42 (little-endian)
```
Total size: 10 bytes

### Variable Declaration

```
; Assembly: VAR TYPE_INT32, counter, 0
; Binary:
0x16      ; Opcode for VAR
0x03      ; Three operands
0x0300    ; TYPE_INT32
0x9100    ; TYPE_SYM for name
[sym_id]  ; Symbol ID for "counter"
0x1320    ; TYPE_UNT32+IMM
0x00000000 ; Value 0 (little-endian)
```
Total size: 11+ bytes (depending on symbol ID size)

### Comparison with Conditional Branch

```
; Assembly:
; CMP a, b
; BR_EQ label

; CMP Binary:
0x05      ; Opcode for CMP
0x02      ; Two operands
0x9000    ; TYPE_VAR
[var_id1] ; Variable ID for "a"
0x9000    ; TYPE_VAR
[var_id2] ; Variable ID for "b"

; BR_EQ Binary:
0x02      ; Opcode for BR
0x02      ; Two operands
0x9100    ; TYPE_SYM
[sym_id]  ; Symbol ID for "label"
0xF000    ; TYPE_PARAM5
0x00      ; BRANCH_COND_EQ
```
Total size: Variable based on ID sizes

## Binary Format Benefits

The COIL binary format is designed for:

1. **Efficiency**: Compact representation of instructions
2. **Flexibility**: Can represent all required operations and types
3. **Extensibility**: Structured to allow future additions
4. **Universality**: Works across all supported processor types
5. **Determinism**: Bijective mapping between assembly and binary

## Format Versioning

The binary format version is encoded in COIL object files. COIL processors should check version compatibility and support:
- Same major version: Full support required
- Higher minor version: Support core features, newer features optional