# Type Instructions (0xA0-0xAF)

#### TYPEOF (0xA0)
Get the type of a value.

Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)

#### SIZEOF (0xA1)
Get the size of a type or value in bytes.

Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)

#### ALIGNOF (0xA2)
Get the alignment requirement of a type or value in bytes.

Operands:
- Destination: Non TYPE_VOID
- Source: Any type
- TYPE_PARAM5: branch_condition_t (optional)

#### CONVERT (0xA3)
Convert a value from one type to another with possible precision loss.

Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### CAST (0xA4)
Reinterpret a value as another type without changing the bit pattern.

Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### STRUCT (0xA5)
Define a structure type.

Operands:
- Destination: TYPE_STRUCT
- Field1Type: TYPE_*
- Field1Name: TYPE_SYM
- Field2Type: TYPE_*
- Field2Name: TYPE_SYM
- ...

#### GET (0xA6)
Access a field in a structure or union.

Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_STRUCT or TYPE_UNION
- Field: TYPE_SYM
- TYPE_PARAM5: branch_condition_t (optional)

#### INDEX (0xA7)
Access an element in an array.

Operands:
- Destination: Non TYPE_VOID
- Source: TYPE_ARRAY
- Index: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
