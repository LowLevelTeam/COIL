# Type Operations (0xA0-0xAF)

## Purpose

This document defines the type operation instructions in COIL, which provide functionality for working with the type system, converting between types, and manipulating composite types. These instructions form the foundation for type-safe operations and enable flexible type manipulation.

## Type Information Instructions

### TYPEOF (0xA0)
Get the type of a value.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)
```

The destination receives a numeric value representing the type of the source operand, including both the main type and type extensions.

Example:
```
; Get the type of a value
TYPEOF type_id, variable

; Use type information for conditional logic
TYPEOF type_id, unknown_value
CMP type_id, TYPE_INT32
BR_EQ handle_int32
```

Binary encoding:
```
0xA0      ; Opcode for TYPEOF
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### SIZEOF (0xA1)
Get the size of a type or value in bytes.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)
```

The destination receives the size in bytes of the source operand's type.

Example:
```
; Get the size of a type
SIZEOF size_bytes, TYPE_INT64    ; size_bytes = 8

; Get the size of a variable's type
SIZEOF arr_elem_size, array_element

; Use size for memory allocation
SIZEOF elem_size, TYPE_STRUCT=point
MUL total_size, elem_size, count
```

Binary encoding:
```
0xA1      ; Opcode for SIZEOF
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### ALIGNOF (0xA2)
Get the alignment requirement of a type or value in bytes.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)
```

The destination receives the alignment requirement in bytes of the source operand's type.

Example:
```
; Get the alignment of a type
ALIGNOF align_bytes, TYPE_V128   ; align_bytes = 16

; Get alignment for memory allocation
ALIGNOF struct_align, my_struct
```

Binary encoding:
```
0xA2      ; Opcode for ALIGNOF
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Type Conversion Instructions

### CONVERT (0xA3)
Convert a value from one type to another with proper value preservation.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

CONVERT performs an appropriate conversion based on the source and destination types, using value-preserving semantics:
- Integer to integer: Sign-extension or truncation
- Integer to floating-point: Mathematical conversion
- Floating-point to integer: Rounding to nearest
- Floating-point to floating-point: Precision adjustment

Example:
```
; Convert integer to floating-point
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_value
CONVERT float_value, int_value    ; float_value = 42.0

; Convert between different integer widths
VAR TYPE_INT8, byte_value
CONVERT byte_value, int_value     ; With range checking/truncation

; Convert floating-point precision
VAR TYPE_FP64, double_value, 3.14159265359
VAR TYPE_FP32, float_value
CONVERT float_value, double_value ; With precision adjustment
```

Binary encoding:
```
0xA3      ; Opcode for CONVERT
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

### CAST (0xA4)
Reinterpret a value as another type without changing the bit pattern.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

CAST reinterprets the bit pattern of the source value as the destination type, without modifying the bits:
- Types must have the same size
- No semantic conversion is performed
- Useful for bit-level manipulations and type punning

Example:
```
; Reinterpret float as integer (for bit manipulation)
VAR TYPE_FP32, float_value, 1.0
VAR TYPE_UNT32, bits
CAST bits, float_value    ; Access bit representation

; Manipulate bits
OR bits, bits, 0x80000000  ; Set sign bit
VAR TYPE_FP32, modified_float
CAST modified_float, bits  ; Convert back to float (-1.0)

; Cast between pointer types
VAR TYPE_PTR=TYPE_UNT8, byte_ptr, buffer
VAR TYPE_PTR=TYPE_INT32, int_ptr
CAST int_ptr, byte_ptr    ; Reinterpret as pointer to integers
```

Binary encoding:
```
0xA4      ; Opcode for CAST
0x02/0x03 ; Two or three operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source
[value2]  ; Source value
[type3]   ; TYPE_PARAM5 (if conditional)
[value3]  ; Condition code (if conditional)
```

## Composite Type Instructions

### STRUCT (0xA5)
Define a structure type.

```
Operands:
- Destination: TYPE_STRUCT
- Field1Type: TYPE_*
- Field1Name: TYPE_SYM
- Field2Type: TYPE_*
- Field2Name: TYPE_SYM
- ...
```

STRUCT defines a new structure type with the specified fields and field types.

Example:
```
; Define a point structure type
STRUCT TYPE_STRUCT=point, TYPE_INT32, x, TYPE_INT32, y

; Define a more complex structure
STRUCT TYPE_STRUCT=rectangle, 
       TYPE_STRUCT=point, top_left,
       TYPE_STRUCT=point, bottom_right,
       TYPE_FP32, area
```

Binary encoding:
```
0xA5      ; Opcode for STRUCT
0x01+     ; One plus 2*N operands (N = number of fields)
0xD000    ; TYPE_STRUCT
[type_id] ; Type ID for structure
[type2]   ; Type of first field
[value2]  ; Symbol ID for first field name
[type3]   ; Type of second field
[value3]  ; Symbol ID for second field name
...
```

### GET (0xA6)
Access a field in a structure or union.

```
Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_STRUCT or TYPE_UNION
- Field: TYPE_SYM
- TYPE_PARAM5: branch_condition_t (optional)
```

GET extracts a field from a structure or union and places it in the destination.

Example:
```
; Access structure fields
VAR TYPE_STRUCT=point, p, 10, 20
VAR TYPE_INT32, x_coord
GET x_coord, p, x     ; x_coord = p.x (= 10)

; Conditional field access
GET_NZ value, record, active_field
```

Binary encoding:
```
0xA6      ; Opcode for GET
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source structure
[value2]  ; Source structure value
0x9100    ; TYPE_SYM for field name
[value3]  ; Symbol ID for field name
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### SET (0xA7)
Set a field in a structure or union.

```
Operands:
- Destination: TYPE_STRUCT or TYPE_UNION
- Field: TYPE_SYM
- Value: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

SET updates a field in a structure or union with the specified value.

Example:
```
; Set structure fields
VAR TYPE_STRUCT=point, p, 0, 0
VAR TYPE_INT32, new_x, 15
SET p, x, new_x      ; p.x = 15

; Set multiple fields
SET p, x, 10
SET p, y, 20

; Conditional field update
SET_NZ record, modified, 1
```

Binary encoding:
```
0xA7      ; Opcode for SET
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination structure
[value1]  ; Destination structure value
0x9100    ; TYPE_SYM for field name
[value2]  ; Symbol ID for field name
[type3]   ; Type of value
[value3]  ; Value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### INDEX (0xA8)
Access an element in an array.

```
Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_ARRAY
- Index: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

INDEX extracts an element from an array at the specified index and places it in the destination.

Example:
```
; Access array element
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)
VAR TYPE_INT32, value
INDEX value, numbers, 2     ; value = numbers[2] (= 30)

; Use variable as index
VAR TYPE_UNT32, i, 1
INDEX value, numbers, i     ; value = numbers[1] (= 20)

; Conditional array access
INDEX_NZ value, array, index
```

Binary encoding:
```
0xA8      ; Opcode for INDEX
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination
[value1]  ; Destination value
[type2]   ; Type of source array
[value2]  ; Source array value
[type3]   ; Type of index
[value3]  ; Index value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

### UPDT (0xA9)
Update an element in an array.

```
Operands:
- Destination: TYPE_ARRAY
- Index: Non TYPE_VOID
- Value: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

UPDT updates an element in an array at the specified index with the provided value.

Example:
```
; Update array element
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)
VAR TYPE_INT32, new_value, 25
UPDT numbers, 1, new_value  ; numbers[1] = 25

; Use variable as index
VAR TYPE_UNT32, i, 3
UPDT numbers, i, 45         ; numbers[3] = 45

; Conditional array update
UPDT_NZ array, index, value
```

Binary encoding:
```
0xA9      ; Opcode for UPDT
0x03/0x04 ; Three or four operands
[type1]   ; Type of destination array
[value1]  ; Destination array value
[type2]   ; Type of index
[value2]  ; Index value
[type3]   ; Type of value
[value3]  ; Value
[type4]   ; TYPE_PARAM5 (if conditional)
[value4]  ; Condition code (if conditional)
```

## Type Creation and Manipulation

### TYPEDEF (0xAA)
Create a type alias.

```
Operands:
- Destination: TYPE_SYM
- Source: TYPE_*
```

TYPEDEF creates a named alias for a type.

Example:
```
; Create type aliases
TYPEDEF integer, TYPE_INT32
TYPEDEF coordinates, TYPE_STRUCT=point

; Use the type alias
VAR integer, counter, 0
```

Binary encoding:
```
0xAA      ; Opcode for TYPEDEF
0x02      ; Two operands
0x9100    ; TYPE_SYM for type name
[value1]  ; Symbol ID for type name
[type2]   ; Type to alias
```

### ARRAY (0xAB)
Define an array type.

```
Operands:
- Destination: TYPE_ARRAY
- ElementType: TYPE_*
- Size: Non TYPE_VOID
```

ARRAY defines a new array type with the specified element type and size.

Example:
```
; Define array types
ARRAY TYPE_ARRAY=int_array, TYPE_INT32, 10
ARRAY TYPE_ARRAY=matrix, TYPE_ARRAY=TYPE_FP32, (4, 4)

; Use the array type
VAR TYPE_ARRAY=int_array, numbers
```

Binary encoding:
```
0xAB      ; Opcode for ARRAY
0x03      ; Three operands
0xD300    ; TYPE_ARRAY
[type_id] ; Type ID for array
[type2]   ; Element type
[type3]   ; Size type
[value3]  ; Size value
```

## Type Operations Examples

### Type Conversion Examples

```
; Numeric type conversions
VAR TYPE_INT32, int_value, 42
VAR TYPE_FP32, float_value
VAR TYPE_FP64, double_value
VAR TYPE_INT8, byte_value

; Integer to floating-point
CONVERT float_value, int_value   ; float_value = 42.0

; Widen floating-point
CONVERT double_value, float_value

; Narrow integer with potential truncation
CONVERT byte_value, int_value

; Integer-to-integer sign conversion
VAR TYPE_INT32, signed_value, -10
VAR TYPE_UNT32, unsigned_value
CONVERT unsigned_value, signed_value  ; Conversion handles sign difference
```

### Type Reinterpretation

```
; IEEE 754 floating-point bit manipulation
VAR TYPE_FP32, original, 1.0
VAR TYPE_UNT32, bits

; Extract bits from float
CAST bits, original

; Manipulate exponent (bits 23-30)
VAR TYPE_UNT32, exponent_mask, 0x7F800000
VAR TYPE_UNT32, exponent
AND exponent, bits, exponent_mask
ADD exponent, exponent, 0x00800000  ; Increase exponent by 1

; Clear old exponent and set new one
NOT exponent_mask, exponent_mask
AND bits, bits, exponent_mask
OR bits, bits, exponent

; Convert back to float
VAR TYPE_FP32, result
CAST result, bits                    ; result = 2.0 (doubled)
```

### Structure Manipulation

```
; Define a person structure
STRUCT TYPE_STRUCT=person, 
       TYPE_ARRAY=TYPE_UNT8, name,
       TYPE_UNT8, age,
       TYPE_FP32, height

; Create and populate a person variable
VAR TYPE_STRUCT=person, p
VAR TYPE_ARRAY=TYPE_UNT8, name_field, "John"
SET p, name, name_field
SET p, age, 30
SET p, height, 1.75

; Access structure fields
VAR TYPE_UNT8, person_age
GET person_age, p, age

; Nested structures
STRUCT TYPE_STRUCT=employee,
       TYPE_STRUCT=person, person_info,
       TYPE_INT32, employee_id,
       TYPE_FP32, salary

; Create employee and access fields
VAR TYPE_STRUCT=employee, emp
SET emp, person_info, p
SET emp, employee_id, 12345
SET emp, salary, 50000.0

; Access nested field
VAR TYPE_FP32, emp_height
VAR TYPE_STRUCT=person, person_info
GET person_info, emp, person_info
GET emp_height, person_info, height
```

### Array Operations

```
; Create an array of integers
VAR TYPE_ARRAY=TYPE_INT32, numbers, (10, 20, 30, 40, 50)

; Access array elements
VAR TYPE_INT32, third_element
INDEX third_element, numbers, 2  ; Zero-based indexing

; Update array element
VAR TYPE_INT32, new_value, 35
UPDT numbers, 2, new_value

; Two-dimensional array
VAR TYPE_ARRAY=TYPE_ARRAY=TYPE_INT32, matrix, (
    (1, 2, 3),
    (4, 5, 6),
    (7, 8, 9)
)

; Access 2D array element
VAR TYPE_ARRAY=TYPE_INT32, row
VAR TYPE_INT32, element
INDEX row, matrix, 1       ; Get second row
INDEX element, row, 2      ; Get third element of second row

; Update 2D array element
UPDT row, 2, 99            ; Update third element of second row
UPDT matrix, 1, row        ; Update second row in matrix
```

### Type Information and Memory Management

```
; Use type information for memory allocation
VAR TYPE_INT32, element_count, 100
VAR TYPE_ARRAY=TYPE_FP32, data_array

; Get size of element type
VAR TYPE_UNT32, element_size
SIZEOF element_size, TYPE_FP32

; Calculate total size
VAR TYPE_UNT32, total_size
MUL total_size, element_size, element_count

; Allocate memory (pseudocode, as memory allocation is implementation-defined)
VAR TYPE_PTR, memory_ptr
ALLOC memory_ptr, total_size

; Get alignment for proper memory access
VAR TYPE_UNT32, alignment
ALIGNOF alignment, TYPE_FP32
```

## Type Safety Considerations

### Type Compatibility

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

### Bounds Checking

Array operations include automatic bounds checking:

```
; Array bounds are checked
INDEX element, array, index  ; Fails if index >= array.length
UPDT array, index, value     ; Fails if index >= array.length
```

The COIL processor determines how to handle bounds violations (error, exception, etc.).

### Type-Driven Design

COIL's type system encourages a type-driven design approach:

1. Define types that capture domain concepts
2. Use structures and arrays for complex data
3. Write functions with explicit type signatures
4. Allow the COIL processor to optimize based on type information

## Processor-Specific Considerations

While type operations are universal across all processor types, implementation details may vary:

1. **Type Size and Alignment**:
   - Different processors may have different natural sizes and alignment requirements
   - Platform-dependent types (TYPE_INT, TYPE_PTR) adapt to the processor

2. **Type Conversion Performance**:
   - Some processors have dedicated hardware for certain conversions
   - COIL processor may optimize conversions based on available instructions

3. **Memory Layout**:
   - Structure packing and alignment may vary by processor
   - COIL processor ensures consistent memory layout when needed

4. **Type Information Overhead**:
   - Debug builds may include full type information
   - Release builds may optimize out type operations where possible

The COIL processor is responsible for mapping type operations to the most efficient implementation for the target processor while maintaining consistent behavior across platforms.