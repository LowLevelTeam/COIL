# Type Operations (0xA0-0xAF)

## Purpose

This document defines the type operation instructions in COIL, which provide functionality for working with the type system, converting between types, and manipulating composite types.

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0xA0   | TYPEOF   | `TYPEOF dest, src[, TYPE_PARAM5]` | Get type information |
| 0xA1   | SIZEOF   | `SIZEOF dest, src[, TYPE_PARAM5]` | Get size in bytes |
| 0xA2   | ALIGNOF  | `ALIGNOF dest, src[, TYPE_PARAM5]` | Get alignment requirement |
| 0xA3   | CONVERT  | `CONVERT dest, src[, TYPE_PARAM5]` | Convert between types |
| 0xA4   | CAST     | `CAST dest, src[, TYPE_PARAM5]` | Reinterpret bit pattern |
| 0xA5   | STRUCT   | `STRUCT dest, field_types_and_names...` | Define structure type |
| 0xA6   | GET      | `GET dest, src, field[, TYPE_PARAM5]` | Get structure field |
| 0xA7   | SET      | `SET dest, field, value[, TYPE_PARAM5]` | Set structure field |
| 0xA8   | INDEX    | `INDEX dest, src, index[, TYPE_PARAM5]` | Access array element |
| 0xA9   | UPDT     | `UPDT dest, index, value[, TYPE_PARAM5]` | Update array element |
| 0xAA   | TYPEDEF  | `TYPEDEF dest, src` | Create type alias |
| 0xAB   | ARRAY    | `ARRAY dest, element_type, size` | Define array type |

## Detailed Descriptions

### TYPEOF (0xA0)
Get the type of a value.

#### Assembly Syntax
```
TYPEOF dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA0                  ; Opcode for TYPEOF
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
TYPEOF type_id, variable

; Binary
0xA0      ; TYPEOF
0x02      ; Two operands
0x1400    ; TYPE_UNT64
[id]      ; Type ID identifier
0x9000    ; TYPE_VAR
[id]      ; Variable identifier
```

### SIZEOF (0xA1)
Get the size of a type or value in bytes.

#### Assembly Syntax
```
SIZEOF dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA1                  ; Opcode for SIZEOF
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (size of a type)
SIZEOF size_bytes, TYPE_INT64    ; size_bytes = 8

; Binary
0xA1      ; SIZEOF
0x02      ; Two operands
0x1400    ; TYPE_UNT64
[id]      ; Size bytes identifier
0x0400    ; TYPE_INT64
0x00      ; No specific value needed for type information

; Assembly (size of a variable's type)
SIZEOF elem_size, array_element

; Binary
0xA1      ; SIZEOF
0x02      ; Two operands
0x1400    ; TYPE_UNT64
[id]      ; Element size identifier
0x9000    ; TYPE_VAR
[id]      ; Array element identifier
```

### CONVERT (0xA3)
Convert a value from one type to another with proper value preservation.

#### Assembly Syntax
```
CONVERT dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA3                  ; Opcode for CONVERT
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Conversion Rules
- Numeric types are converted with appropriate rules (rounding, sign extension, etc.)
- Conversions preserve the semantic value as closely as possible
- Incompatible conversions generate errors

#### Example
```
; Assembly (integer to float)
CONVERT TYPE_FP32, float_result, TYPE_INT32, int_value

; Binary
0xA3      ; CONVERT
0x02      ; Two operands
0x2500    ; TYPE_FP32
[id]      ; Float result identifier
0x0300    ; TYPE_INT32
[id]      ; Int value identifier
```

### CAST (0xA4)
Reinterpret a value as another type without changing the bit pattern.

#### Assembly Syntax
```
CAST dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA4                  ; Opcode for CAST
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source
[src_value]           ; Source value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Cast Rules
- Types must have the same size
- No value conversion is performed (pure bit pattern reinterpretation)
- Used for low-level bit manipulation or type punning

#### Example
```
; Assembly (float to int for bit manipulation)
CAST TYPE_UNT32, bits, TYPE_FP32, float_value

; Binary
0xA4      ; CAST
0x02      ; Two operands
0x1300    ; TYPE_UNT32
[id]      ; Bits identifier
0x2500    ; TYPE_FP32
[id]      ; Float value identifier
```

### STRUCT (0xA5)
Define a structure type.

#### Assembly Syntax
```
STRUCT dest, field1_type, field1_name, field2_type, field2_name, ...
```

#### Binary Encoding
```
0xA5                  ; Opcode for STRUCT
0x01+                 ; One plus 2*N operands (N = number of fields)
[dest_type]           ; TYPE_STRUCT
[dest_value]          ; Struct type ID
[field1_type]         ; Type of first field
[field1_name]         ; Name of first field
[field2_type]         ; Type of second field
[field2_name]         ; Name of second field
...
```

#### Example
```
; Assembly (define point structure)
STRUCT TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y

; Binary
0xA5      ; STRUCT
0x05      ; Five operands (dest + 2*2 field definitions)
0xD000    ; TYPE_STRUCT
[id]      ; Struct type ID for "point"
0x0300    ; TYPE_INT32
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "x"
0x0300    ; TYPE_INT32
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "y"
```

### GET (0xA6)
Access a field in a structure.

#### Assembly Syntax
```
GET dest, src, field[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA6                  ; Opcode for GET
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source structure
[src_value]           ; Source structure value
[field_type]          ; TYPE_SYM for field name
[field_value]         ; Field name value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
GET x_coord, point, x

; Binary
0xA6      ; GET
0x03      ; Three operands
0x0300    ; TYPE_INT32
[id]      ; X coordinate identifier
0xD000    ; TYPE_STRUCT=point
[id]      ; Point structure identifier
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "x"
```

### SET (0xA7)
Set a field in a structure.

#### Assembly Syntax
```
SET dest, field, value[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA7                  ; Opcode for SET
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination structure
[dest_value]          ; Destination structure value
[field_type]          ; TYPE_SYM for field name
[field_value]         ; Field name value
[value_type]          ; Type of value to set
[value_value]         ; Value to set
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
SET point, y, 20

; Binary
0xA7      ; SET
0x03      ; Three operands
0xD000    ; TYPE_STRUCT=point
[id]      ; Point structure identifier
0x9100    ; TYPE_SYM
[id]      ; Symbol ID for "y"
0x0320    ; TYPE_INT32+IMM
0x14000000 ; Value 20
```

### INDEX (0xA8)
Access an element in an array.

#### Assembly Syntax
```
INDEX dest, src, index[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA8                  ; Opcode for INDEX
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination
[dest_value]          ; Destination value
[src_type]            ; Type of source array
[src_value]           ; Source array value
[index_type]          ; Type of index
[index_value]         ; Index value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
INDEX element, numbers, 2

; Binary
0xA8      ; INDEX
0x03      ; Three operands
0x0300    ; TYPE_INT32
[id]      ; Element identifier
0xD303    ; TYPE_ARRAY=TYPE_INT32
[id]      ; Numbers array identifier
0x1320    ; TYPE_UNT32+IMM
0x02000000 ; Index 2
```

### UPDT (0xA9)
Update an element in an array.

#### Assembly Syntax
```
UPDT dest, index, value[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0xA9                  ; Opcode for UPDT
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination array
[dest_value]          ; Destination array value
[index_type]          ; Type of index
[index_value]         ; Index value
[value_type]          ; Type of value to set
[value_value]         ; Value to set
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
UPDT numbers, 1, 25

; Binary
0xA9      ; UPDT
0x03      ; Three operands
0xD303    ; TYPE_ARRAY=TYPE_INT32
[id]      ; Numbers array identifier
0x1320    ; TYPE_UNT32+IMM
0x01000000 ; Index 1
0x0320    ; TYPE_INT32+IMM
0x19000000 ; Value 25
```

## Type Manipulation Patterns

### Type Conversion Patterns

```
; Numeric type conversions
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_value

; Integer to floating-point
CONVERT float_value, int_value   ; float_value = 42.0

; Type system introspection
VAR TYPE_UNT64, type_size
SIZEOF type_size, TYPE_INT32     ; type_size = 4
```

### Structure Manipulation

```
; Define a person structure
STRUCT TYPE_STRUCT=person, 
       TYPE_ARRAY=TYPE_UNT8, name,
       TYPE_UNT8, age,
       TYPE_FP32, height

; Create and populate a person
VAR TYPE_STRUCT=person, p
VAR TYPE_ARRAY=TYPE_UNT8, name_field, "John"
SET p, name, name_field
SET p, age, 30
SET p, height, 1.75

; Access structure fields
VAR TYPE_UNT8, person_age
GET person_age, p, age
```

### Array Operations

```
; Create an array
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)

; Access array element
VAR TYPE_INT32, third_element
INDEX third_element, numbers, 2  ; Zero-based indexing, gets 30

; Update array element
UPDT numbers, 2, 35              ; Changes third element to 35
```

### Type-Safe Memory Access

```
; Safely access memory through typed pointers
VAR TYPE_PTR=TYPE_INT32, int_ptr, address
VAR TYPE_INT32, value

; Load value from typed pointer
MOV value, [int_ptr]

; Store value to typed pointer
MOV [int_ptr], 42
```

## Type Safety Considerations

The type system ensures safe operations by enforcing compatibility rules:

1. **Direct Assignment**: Types must be compatible for direct assignment
   ```
   MOV dest, source  ; Type of dest must be compatible with type of source
   ```

2. **Type Conversion**: CONVERT should be used for semantically meaningful conversions
   ```
   CONVERT float_value, int_value  ; Proper conversion preserving value
   ```

3. **Type Reinterpretation**: CAST should be used only when raw bit manipulation is needed
   ```
   CAST int_bits, float_value  ; Reinterprets bits without semantic conversion
   ```

4. **Bounds Checking**: Array operations include automatic bounds checking
   ```
   INDEX element, array, index  ; Fails if index >= array.length
   UPDT array, index, value     ; Fails if index >= array.length
   ```