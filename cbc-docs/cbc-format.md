# CBC Binary Format

## Overview

The CBC binary format is designed for efficient parsing and execution by interpreters and JIT compilers. It uses a streamlined instruction encoding optimized for fast decoding, while maintaining sufficient expressiveness for complex programs.

## CBC File Format

CBC files (.cbc) use the following header structure:

```
struct CBCHeader {
    char     magic[4];        // "CBC\0"
    uint8_t  major;           // Major version
    uint8_t  minor;           // Minor version
    uint8_t  patch;           // Patch version
    uint8_t  flags;           // Format flags
    uint32_t constant_offset; // Offset to constant pool
    uint32_t code_offset;     // Offset to code section
    uint32_t metadata_offset; // Offset to metadata (0 if none)
    uint32_t file_size;       // Total file size
}
```

Format flags:
- `0x01`: Contains debug information
- `0x02`: Uses extended constant pool
- `0x04`: Contains metadata
- `0x08`: Big-endian encoding (default is little-endian)

## Instruction Encoding

CBC instructions use a more compact encoding than full COIL:

```
[Opcode (8 bits)] [Type Code (4 bits)] [Format (4 bits)] [Operands (variable)]
```

Type codes:
- `0x0`: Void (no type)
- `0x1`: Integer
- `0x2`: Floating-point
- `0x3`: Vector
- `0x4`: Reference
- `0x5`: Composite
- `0x6-0xF`: Reserved for future use

Format codes determine operand layout:
- `0x0`: No operands
- `0x1`: One 8-bit operand
- `0x2`: One 16-bit operand
- `0x3`: One 32-bit operand
- `0x4`: Two 8-bit operands
- `0x5`: One 8-bit, one 16-bit operand
- `0x6`: One 8-bit, one 32-bit operand
- `0x7`: Three 8-bit operands
- `0x8-0xF`: Reserved for future use

## Constant Pool

The constant pool stores frequently used values:

```
struct ConstantPool {
    uint32_t count;           // Number of constants
    uint8_t  type_codes[];    // Type of each constant (8 bits each)
    uint8_t  constants[];     // Constants data (variable size)
}
```

Constants are referenced by their index in the pool.

### Constant Types

| Type Code | Constant Type | Size | Description |
|-----------|---------------|------|-------------|
| 0x01 | Integer (32-bit) | 4 bytes | 32-bit signed integer |
| 0x02 | Integer (64-bit) | 8 bytes | 64-bit signed integer |
| 0x03 | Float (32-bit) | 4 bytes | IEEE 754 single precision |
| 0x04 | Float (64-bit) | 8 bytes | IEEE 754 double precision |
| 0x05 | String | Variable | UTF-8 string (prefixed with length) |
| 0x06 | Type | 2 bytes | COIL type descriptor |
| 0x07 | Function Reference | 4 bytes | Index to function table |

## Function Table

Functions are stored in a dedicated function table:

```
struct FunctionTable {
    uint32_t count;           // Number of functions
    Function entries[];       // Function entries
}

struct Function {
    uint16_t name_index;      // Index to name in string table
    uint16_t signature_index; // Index to type signature
    uint32_t code_offset;     // Offset to code
    uint16_t param_count;     // Number of parameters
    uint16_t local_count;     // Number of local variables
    uint32_t code_size;       // Size of function code
}
```

## Metadata Section

Optional metadata provides additional information:

```
struct Metadata {
    uint32_t source_offset;   // Source file information
    uint32_t debug_offset;    // Debug information
    uint32_t attr_offset;     // Custom attributes
}
```

This section supports debugging, profiling, and additional tools.

## Binary Encoding Examples

### Variable Declaration and Initialization

```
; var #1 = 42 (32-bit integer)
2E 14 01 0000002A  ; var.i32 #1, const(42)

; var #2 = 3.14 (32-bit float)
30 24 02 4048F5C3  ; var.f32 #2, const(3.14)
```

### Addition Operation

```
; #3 = #1 + #2 (integer)
2E 11 03           ; var.i32 #3
40 17 03 01 02     ; add.i32 #3, #1, #2
```

### Function Call

```
; call function at index 5 with parameters #1, #2
0F 05 01 02        ; call function_index, param_count, params...
```