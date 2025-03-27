# COIL Binary Encoding

## Purpose

This document defines the fundamental binary encoding rules for COIL instructions, providing the technical foundation for how instructions are represented in their raw binary format. This is distinct from the COIL object file format, which encapsulates these instructions within a larger structure.

## Core Principles

COIL is fundamentally a binary instruction format with several key principles:

1. **Binary Format Primacy**: The binary representation is the authoritative definition of COIL
2. **Direct Mapping**: Each CASM instruction maps directly to one binary instruction
3. **Type-Determined Behavior**: Instruction behavior is determined by operand types
4. **Consistent Encoding**: All instructions follow a uniform encoding pattern

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
   0x1320 0x2A000000  ; TYPE_UNT32+IMM with value 42
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

## Encoding Details by Instruction Category

### Control Flow Instructions (0x01-0x0F)

#### SYM (0x01)

```
0x01            ; Opcode for SYM
0x01/0x02       ; One or two operands
0x9100          ; TYPE_SYM
[symbol_id]     ; Symbol ID
[TYPE_PARAM0]   ; Optional symbol parameter
[parameter_value] ; Parameter value
```

#### BR (0x02)

```
0x02            ; Opcode for BR
0x01/0x02/0x03  ; Operand count
0x9100          ; TYPE_SYM (or other target type)
[target_id]     ; Target identifier
[TYPE_PARAM0]   ; Optional branch control
[control_value] ; Control value
[TYPE_PARAM5]   ; Optional condition
[condition_value] ; Condition value
```

#### CALL (0x03)

```
0x03            ; Opcode for CALL
0x01+           ; One plus parameter count
0x9100          ; TYPE_SYM for target
[target_id]     ; Symbol ID
[TYPE_ABICTL]   ; ABI control (if using ABI)
[abi_param]     ; ABICTL_PARAM value
[abi_id]        ; ABI identifier
[param_types...] ; Parameter types
[param_values...] ; Parameter values
```

### Memory Operations (0x10-0x2F)

#### MOV (0x10)

```
0x10            ; Opcode for MOV
0x02/0x03       ; Two or three operands
[dest_type]     ; Type of destination
[dest_value]    ; Destination value
[src_type]      ; Type of source
[src_value]     ; Source value
[TYPE_PARAM5]   ; Optional condition
[condition_value] ; Condition value
```

#### VAR (0x16)

```
0x16            ; Opcode for VAR
0x02/0x03       ; Two or three operands
[type]          ; Variable type
[name_type]     ; TYPE_SYM for name
[name_id]       ; Symbol ID for name
[init_type]     ; Type of initial value (if present)
[init_value]    ; Initial value (if present)
```

### Arithmetic Instructions (0x60-0x8F)

#### ADD (0x60)

```
0x60            ; Opcode for ADD
0x02/0x03/0x04  ; Two, three, or four operands
[dest_type]     ; Type of destination
[dest_value]    ; Destination value
[left_type]     ; Type of left operand (if three-operand form)
[left_value]    ; Left operand value (if three-operand form)
[right_type]    ; Type of right operand
[right_value]   ; Right operand value
[TYPE_PARAM5]   ; Optional condition
[condition_value] ; Condition value
```

### Directive Instructions (0xB0-0xBF)

#### PROC (0xB1)

```
0xB1            ; Opcode for PROC
0x01            ; One operand
[type]          ; Type of processor code
[value]         ; Processor code value
```

#### SECTION (0xB4)

```
0xB4            ; Opcode for SECTION
0x02            ; Two operands
[name_type]     ; Type of section name
[name_value]    ; Section name value
[attr_type]     ; Type of attributes
[attr_value]    ; Attributes value
```

## Alignment and Endianness

The binary format makes these guarantees:

1. **Instruction Alignment**: Instructions are not required to be aligned on any particular boundary
2. **Value Endianness**: Multi-byte values in operands follow little-endian encoding by default
3. **No Padding**: No padding is inserted between instruction fields or between instructions

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

## Binary Encoding Tools

The COIL toolchain includes two key tools for working with binary encoding:

1. **coilasm**: Converts CASM text to COIL binary format
2. **coildis**: Converts COIL binary to CASM text

Example usage:
```
coilasm source.casm -o output.coil    ; Assemble CASM to binary
coildis -c input.coil -o output.casm   ; Disassemble binary to CASM with comments
```

## Version Compatibility

The binary format version is encoded in COIL object files. COIL processors should check version compatibility:

- Same major version: Full support required
- Higher minor version: Support core features, newer features optional

## Relationship to COIL Object Format

It's important to distinguish between:

1. **Instruction Binary Encoding** (covered in this document): How individual COIL instructions are encoded as binary
2. **COIL Object Format** (.coil files): A container format that includes instructions along with sections, symbols, and metadata

The COIL object format contains encoded instructions within its code sections, along with additional information needed for linking and execution.

## Related Documentation

- [CASM Syntax](casm-syntax.md) - Human-readable syntax that maps to this binary encoding
- [Object Format](object-format.md) - Container format for COIL instructions
- [Type Reference](../reference/type-ref.md) - Complete type encoding reference
- [Instruction Reference](../reference/instruction-ref.md) - Complete instruction listing with opcodes