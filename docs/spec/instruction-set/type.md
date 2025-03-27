# Type Operations (0xA0-0xAF)

## Purpose

This document defines the type operation instructions in COIL, which provide functionality for working with the type system, querying type information, converting between types, and manipulating composite types such as structures and arrays.

## Key Concepts

- **Type Information**: How to query type attributes
- **Type Conversion**: How to convert between types
- **Composite Type Manipulation**: How to work with structures and arrays
- **Type-Safe Access**: How to safely access type fields and elements

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

### Type Information Instructions

#### TYPEOF (0xA0)
Get the type identifier of a value.

##### Syntax
```
TYPEOF dest, src[, TYPE_PARAM5=condition]
```

##### Example
```
; Get the type of a variable
VAR TYPE_UNT16, type_id
TYPEOF type_id, some_variable
```

#### SIZEOF (0xA1)
Get the size of a type or value in bytes.

##### Syntax
```
SIZEOF dest, src[, TYPE_PARAM5=condition]
```

##### Example
```
; Get the size of an integer type
VAR TYPE_UNT32, int_size
SIZEOF int_size, TYPE_INT32    ; int_size = 4

; Get the size of a structure
VAR TYPE_UNT32, struct_size
SIZEOF struct_size, my_struct
```

#### ALIGNOF (0xA2)
Get the alignment requirement of a type or value.

##### Syntax
```
ALIGNOF dest, src[, TYPE_PARAM5=condition]
```

##### Example
```
; Get alignment requirement
VAR TYPE_UNT32, alignment
ALIGNOF alignment, TYPE_INT64  ; alignment = 8
```

### Type Conversion Instructions

#### CONVERT (0xA3)
Convert a value from one type to another with proper value preservation.

##### Syntax
```
CONVERT dest, src[, TYPE_PARAM5=condition]
```

##### Examples
```
; Integer to float conversion
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_result
CONVERT float_result, int_value  ; float_result = 42.0f

; Float to integer conversion (truncation)
VAR TYPE_FP32, pi, 3.14159
VAR TYPE_INT32, int_pi
CONVERT int_pi, pi  ; int_pi = 3
```

#### CAST (0xA4)
Reinterpret a value's bit pattern as another type without changing the bits.

##### Syntax
```
CAST dest, src[, TYPE_PARAM5=condition]
```

##### Example
```
; Interpret float bits as an integer (bit pattern reinterpretation)
VAR TYPE_FP32, float_value, 1.0
VAR TYPE_UNT32, bit_pattern
CAST bit_pattern, float_value  ; Views the same bits as an integer
```

### Structure Operations

#### STRUCT (0xA5)
Define a structure type with fields.

##### Syntax
```
STRUCT dest, field1_type, field1_name, field2_type, field2_name, ...
```

##### Example
```
; Define a point structure with x,y coordinates
STRUCT TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y
```

#### GET (0xA6)
Access a field in a structure.

##### Syntax
```
GET dest, src, field[, TYPE_PARAM5=condition]
```

##### Example
```
; Get x coordinate from point
VAR TYPE_STRUCT=point, position, 10, 20
VAR TYPE_INT32, x_coord
GET x_coord, position, x  ; x_coord = 10
```

#### SET (0xA7)
Update a field in a structure.

##### Syntax
```
SET dest, field, value[, TYPE_PARAM5=condition]
```

##### Example
```
; Update y coordinate in point
VAR TYPE_STRUCT=point, position, 10, 20
SET position, y, 30  ; position now contains (10, 30)
```

### Array Operations

#### ARRAY (0xAB)
Define an array type.

##### Syntax
```
ARRAY dest, element_type, size
```

##### Example
```
; Define an array of 10 integers
ARRAY TYPE_ARRAY=int_array, TYPE_INT32, 10
```

#### INDEX (0xA8)
Access an element in an array.

##### Syntax
```
INDEX dest, src, index[, TYPE_PARAM5=condition]
```

##### Example
```
; Get the third element (zero-based indexing)
VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
VAR TYPE_INT32, third_element
INDEX third_element, numbers, 2  ; third_element = 3
```

#### UPDT (0xA9)
Update an element in an array.

##### Syntax
```
UPDT dest, index, value[, TYPE_PARAM5=condition]
```

##### Example
```
; Update the second element
VAR TYPE_ARRAY=TYPE_INT32, numbers, (1, 2, 3, 4, 5)
UPDT numbers, 1, 42  ; numbers now contains (1, 42, 3, 4, 5)
```

## Common Type Operation Patterns

### Safe Type Conversion

```
; Example: Safe conversion from int64 to int32 with bounds checking
VAR TYPE_INT64, large_value, 0x100000000
VAR TYPE_INT32, small_value, 0

; Check if value fits in destination type
VAR TYPE_INT64, max_int32, 0x7FFFFFFF
CMP large_value, max_int32
BR_GT conversion_error

; Perform the conversion
CONVERT small_value, large_value

SYM continuation
; Continue processing...

SYM conversion_error
; Handle conversion error
```

### Structure Manipulation

```
; Define a person structure
STRUCT TYPE_STRUCT=person, TYPE_ARRAY=TYPE_UNT8, name, TYPE_UNT8, age

; Create and populate a person
VAR TYPE_STRUCT=person, p
VAR TYPE_ARRAY=TYPE_UNT8, name_buffer, "John Doe", 0
SET p, name, name_buffer
SET p, age, 30

; Access fields
VAR TYPE_UNT8, person_age
GET person_age, p, age
```

### Dynamic Type Handling

```
; Example: Generic print function based on type
SYM print_value
    SCOPEE
    VAR TYPE_VAR, value
    MOV value, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
    
    ; Get the type of the value
    VAR TYPE_UNT16, type_id
    TYPEOF type_id, value
    
    ; Branch based on type
    CMP type_id, TYPE_INT32
    BR_EQ print_int
    
    CMP type_id, TYPE_FP32
    BR_EQ print_float
    
    ; Handle other types...
    BR print_unknown
    
    SYM print_int
    ; Integer-specific printing
    BR print_done
    
    SYM print_float
    ; Float-specific printing
    BR print_done
    
    SYM print_unknown
    ; Handle unknown type
    
    SYM print_done
    SCOPEL
    RET
```

## Type Safety Considerations

Type operations enforce several safety rules:

1. **Type Compatibility**: Only compatible types can be converted directly
2. **Bounds Checking**: Array operations include automatic bounds checking
3. **Field Validation**: Structure field accesses are validated against the structure definition
4. **Runtime Type Information**: Type query operations enable dynamic type checking

## Related Documentation

- [Type System](../spec/systems/type-system.md) - Core type system concepts
- [Type Reference](../reference/type-ref.md) - Complete type listing and details
- [Memory Operations](memory.md) - Memory access operations
- [CASM Syntax](../spec/assembly/syntax.md) - Syntax for type operations