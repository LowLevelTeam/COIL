# COIL Instruction Format

## Basic Structure

Every COIL instruction follows a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

This format applies to all instructions, providing a regular and predictable structure.

## Opcode Field

The first byte of each instruction is the opcode, which identifies the operation to perform:

- **0x00-0xBF**: Universal instructions (available on all processor types)
- **0xC0-0xFE**: Processor-specific instructions
- **0xFF**: Implementation-specific extensions

## Operand Count Field

The second byte indicates how many operands follow:

- **0x00**: No operands
- **0x01**: One operand
- **0x02**: Two operands
- **0x03**: Three operands
- And so on...

Most instructions have a fixed operand count, but some (like function calls) may have a variable number of operands.

## Operand Format

Each operand consists of:

```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

### Type Field

The 16-bit type field is divided into two parts:

- **First 8 bits**: Main Type - Primary type category (e.g., INT32, FP64)
- **Second 8 bits**: Type Extensions - Qualifiers (const, volatile, immediate)

For example:
- `0x0300`: TYPE_INT32 (32-bit signed integer)
- `0x0301`: TYPE_INT32+CONST (constant 32-bit integer)
- `0x2520`: TYPE_FP32+IMM (immediate 32-bit float)

### Type-Specific Data

Some types require additional information beyond the 16-bit type descriptor:

1. **Register types** include a register identifier:
   ```
   0x9200 0x00    ; TYPE_RGP=RAX (on x86-64)
   ```

2. **Composite types** include additional type information:
   ```
   0xD300 0x10 0x00  ; TYPE_ARRAY=TYPE_UNT8
   ```

3. **Parameter types** include parameter-specific data:
   ```
   0xF800 0x01 [ABI_ID]  ; TYPE_ABICTL=ABICTL_PARAM=abi_name
   ```

### Value Field

The value field contains the actual data, with encoding determined by the type:

1. **Immediate integers** use little-endian format:
   ```
   0x1320 0x2A000000  ; TYPE_UNT32+IMM with value 42
   ```

2. **Floating-point values** use IEEE 754 format:
   ```
   0x2520 [IEEE 754 encoding]  ; TYPE_FP32+IMM
   ```

3. **References** include an identifier:
   ```
   0x9100 [symbol_id]     ; TYPE_SYM with symbol ID
   0x9000 [variable_id]   ; TYPE_VAR with variable ID
   ```

## Complete Examples

### MOV Instruction

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

## Instruction Encoding Rules

1. **Alignment**: Instructions do not need to be aligned on any boundary
2. **Endianness**: Multi-byte values use little-endian encoding by default
3. **No Padding**: No padding between instruction fields or between instructions
4. **Operand Order**: Destination operand typically comes first