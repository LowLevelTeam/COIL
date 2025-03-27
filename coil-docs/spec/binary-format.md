# COIL Binary Format

## Purpose

This document defines the fundamental binary encoding rules for COIL instructions, providing the technical foundation for how instructions are represented in their raw binary format. This is distinct from the COIL object file format, which is a container that encapsulates these instructions.

## Key Concepts

- **Binary Format Primacy**: The binary representation is the authoritative definition of COIL
- **Instruction Encoding**: How individual instructions are encoded in binary
- **Operand Encoding**: How instruction operands are represented
- **Type System Integration**: How type information is encoded in operands

## Instruction Encoding

Every COIL instruction is encoded as a sequence of bytes following this structure:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

### Opcode Field

The opcode field is a single byte that identifies the operation to perform:
- Values 0x00-0xBF: Universal instructions (available on all processor types)
- Values 0xC0-0xFE: Processor-specific instructions
- Value 0xFF: COIL processor-specific extensions

### Operand Count Field

A single byte indicating how many operands follow:
- 0x00: No operands
- 0x01: One operand
- 0x02: Two operands
- 0x03: Three operands
- etc.

Most instructions have a fixed operand count, but some (like CALL with parameters) may have a variable number of operands.

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
0x2520 - TYPE_FP32+IMM (immediate 32-bit float)
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
   0x1320 0x2A000000  ; TYPE_UNT32+IMM with value 42 (little-endian)
   ```

2. **Floating-point values** use IEEE 754 format:
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

## Type Extensions

Type extensions modify the behavior of types and are encoded in the second byte of the type field:

| Bit Position | Hex Value | Name | Description |
|--------------|-----------|------|-------------|
| Bit 0 | 0x01 | TYPEEXT_CONST    | Constant value (read-only) |
| Bit 1 | 0x02 | TYPEEXT_VOLATILE | Volatile access (not optimizable) |
| Bit 4 | 0x10 | TYPEEXT_VOID     | No Value |
| Bit 5 | 0x20 | TYPEEXT_IMM      | Immediate value |
| Bit 6 | 0x40 | TYPEEXT_VAR      | Variable ID |
| Bit 7 | 0x80 | TYPEEXT_SYM      | Symbol ID |

Examples:
```
0x0301 - TYPE_INT32+CONST  ; Constant integer
0x1420 - TYPE_UNT64+IMM    ; Immediate unsigned 64-bit value
0x0302 - TYPE_INT32+VOLATILE ; Volatile 32-bit integer
```

## Complete Encoding Examples

### Simple MOV Instruction

CASM:
```
MOV counter, 42
```

Binary:
```
0x10         ; Opcode for MOV
0x02         ; Two operands
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "counter"
0x1320       ; TYPE_UNT32+IMM
0x2A000000   ; Value 42 (little-endian)
```

### ADD Instruction

CASM:
```
ADD result, a, b
```

Binary:
```
0x60         ; Opcode for ADD
0x03         ; Three operands
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "result"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "a"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "b"
```

### Function Call with Parameters

CASM:
```
CALL calculate_sum, TYPE_ABICTL=ABICTL_PARAM=platform_default, x, y
```

Binary:
```
0x03         ; Opcode for CALL
0x04         ; Four operands
0x9100       ; TYPE_SYM for target
[sym_id]     ; Symbol ID for "calculate_sum"
0xF800       ; TYPE_ABICTL
0x01         ; ABICTL_PARAM
[abi_id]     ; ABI ID for "platform_default"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "x"
0x9000       ; TYPE_VAR
[var_id]     ; Variable ID for "y"
```

### Conditional Branch

CASM:
```
BR_EQ equal_branch
```

Binary:
```
0x02         ; Opcode for BR
0x02         ; Two operands
0x9100       ; TYPE_SYM
[sym_id]     ; Symbol ID for "equal_branch"
0xF000       ; TYPE_PARAM5
0x00         ; BRANCH_COND_EQ
```

## Special Encoding Cases

### ABI Directive

When the `ABI` directive is encountered, the instruction set temporarily changes to a special set of ABI definition opcodes until the `EXIT` opcode is encountered:

```
0xBA            ; Opcode for ABI
0x01            ; One operand
0x9100          ; TYPE_SYM for ABI name
[sym_id]        ; Symbol ID for ABI name

; ABI-specific instructions follow until EXIT
0x01            ; PARAMS
...
0x00            ; EXIT
```

### Conditional Assembly Directives

Conditional assembly directives (`IF`, `ELIF`, `ELSE`, `ENDIF`) are processed during assembly and do not appear in the final binary format.

## Alignment and Endianness

The binary format makes these guarantees:

1. **Instruction Alignment**: Instructions are not required to be aligned on any particular boundary
2. **Value Endianness**: Multi-byte values in operands follow little-endian encoding by default
3. **No Padding**: No padding is inserted between instruction fields or between instructions

## Version Compatibility

The binary format version is encoded in COIL object files. COIL processors should check version compatibility:

- Same major version: Full support required
- Higher minor version: Support core features, newer features optional

## Related Documentation

- [CASM Syntax](assembly/syntax.md) - Human-readable syntax that maps to this binary encoding
- [Object Format](implementation/object-format.md) - Container format for COIL instructions
- [Type System](systems/type-system.md) - Complete type system details
- [Instruction Reference](reference/instruction-ref.md) - Complete instruction listing with opcodes