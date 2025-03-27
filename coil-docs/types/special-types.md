# Special Types in COIL

## Reference Types

Reference types allow referring to program entities:

### Variable References

| Type ID | Name | Description |
|---------|------|-------------|
| 0x90    | TYPE_VAR | Reference to a variable |

Variable references are used extensively in COIL to access variables by name:

```
MOV counter, 0      ; "counter" is a TYPE_VAR reference
ADD counter, counter, 1
```

Binary encoding:
```
0x9000 [variable_id]  ; TYPE_VAR + variable identifier
```

### Symbol References

| Type ID | Name | Description |
|---------|------|-------------|
| 0x91    | TYPE_SYM | Reference to a symbol |

Symbol references are used for functions, labels, and other named entities:

```
CALL function_name   ; "function_name" is a TYPE_SYM reference
BR loop_start        ; "loop_start" is a TYPE_SYM reference
```

Binary encoding:
```
0x9100 [symbol_id]   ; TYPE_SYM + symbol identifier
```

## Register Types

Register types allow direct access to hardware registers:

| Type ID | Name | Description |
|---------|------|-------------|
| 0x92    | TYPE_RGP | General purpose register |
| 0x93    | TYPE_RFP | Floating point register |
| 0x94    | TYPE_RV  | Vector register |
| 0x9A    | TYPE_IP  | Instruction Pointer register |
| 0x9B    | TYPE_SP  | Stack Pointer register |
| 0x9C    | TYPE_BP  | Base Pointer register |

Register types are used for direct hardware access:

```
MOV TYPE_RGP=RAX, 42   ; Directly access RAX register
ADD TYPE_RGP=RBX, TYPE_RGP=RAX, TYPE_RGP=RCX
```

Binary encoding:
```
0x9200 [register_id]   ; TYPE_RGP + register identifier
```

## State Types

State types represent processor state:

| Type ID | Name | Description |
|---------|------|-------------|
| 0x96    | TYPE_SAR | State - all registers |
| 0x97    | TYPE_SAF | State - all flags |
| 0x98    | TYPE_SES | State - extended processor state |
| 0x99    | TYPE_SS  | State - complete state |

These types are used for advanced operations like:
- Context switching
- Exception handling
- State saving/restoring

## Parameter Types

Parameter types provide additional information to instructions:

| Type ID | Name | Description |
|---------|------|-------------|
| 0xFE    | TYPE_PARAM0 | Parameter 0 - General purpose |
| 0xFD    | TYPE_PARAM1 | Parameter 1 |
| 0xFC    | TYPE_PARAM2 | Parameter 2 |
| 0xF8    | TYPE_ABICTL | ABI control parameter |
| 0xF0    | TYPE_PARAM5 | Parameter 5 - Conditions and flags |

### Common Parameter Values

For TYPE_PARAM0 (Symbol parameters):
```
0x00  - TMP   ; Symbol is temporary
0x01  - FILE  ; Symbol is file-scoped
0x02  - GLOB  ; Symbol is globally visible
```

For TYPE_PARAM5 (Branch conditions):
```
0x00  - BRANCH_COND_EQ  ; Equal
0x01  - BRANCH_COND_NE  ; Not equal 
0x02  - BRANCH_COND_GE  ; Greater than or equal
0x03  - BRANCH_COND_LT  ; Less than
```

For TYPE_ABICTL (ABI control):
```
0x00  - ABICTL_STANDARD  ; Standard ABI
0x01  - ABICTL_PARAM     ; Parameter passing
0x02  - ABICTL_RET       ; Return value
```

## Pointer and Memory Types

Types for memory access:

| Type ID | Name | Size | Description |
|---------|------|------|-------------|
| 0xA6    | TYPE_PTR | Platform-dependent | Pointer type |
| 0x40    | TYPE_BIT | 1/8 byte | Bit (boolean) type |
| 0xFF    | TYPE_VOID | 0 | Void type (no value) |

Pointers can specify the pointed-to type:
```
TYPE_PTR=TYPE_INT32   ; Pointer to 32-bit integer
```

## Composite Types

Types composed of other types:

| Type ID | Name | Description |
|---------|------|-------------|
| 0xD0    | TYPE_STRUCT | Structure type |
| 0xD1    | TYPE_PACK   | Packed structure (no padding) |
| 0xD2    | TYPE_UNION  | Union type |
| 0xD3    | TYPE_ARRAY  | Array type |

These types require additional type information:
```
TYPE_STRUCT=point     ; Structure named "point"
TYPE_ARRAY=TYPE_UNT8  ; Array of unsigned 8-bit integers
```

## Type Extensions

Special types can use these extensions:

| Bit | Hex Value | Name | Description |
|-----|-----------|------|-------------|
| 0   | 0x01 | TYPEEXT_CONST    | Constant (read-only) |
| 1   | 0x02 | TYPEEXT_VOLATILE | Volatile (not optimizable) |
| 5   | 0x20 | TYPEEXT_IMM      | Immediate value |
| 6   | 0x40 | TYPEEXT_VAR      | Variable ID |
| 7   | 0x80 | TYPEEXT_SYM      | Symbol ID |