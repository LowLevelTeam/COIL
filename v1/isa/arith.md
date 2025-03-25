# Arithmetic Instructions (0x60-0x8F)

## Instructions

For instructinos that have a destination with a left and right source you can shorten the instruction to only two operands and have the destination operand be both the left operand in tandem with the destination.

#### ADD (0x60)
Add two values and store the result.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### SUB (0x61)
Subtract the second value from the first and store the result.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### MUL (0x62)
Multiply two values and store the result.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### DIV (0x63)
Divide the first value by the second and store the result.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### MOD (0x64)
Compute the remainder of dividing the first value by the second.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### INC (0x65)
Increment a value by 1.

Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### DEC (0x66)
Decrement a value by 1.

Operands:
- Destination: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### NEG (0x67)
Negate a value.

Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### ABS (0x68)
Compute the absolute value.

Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### SQRT (0x69)
Compute the square root.

Operands:
- Destination: Non TYPE_VOID (floating-point type)
- Source: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)

// More advanced arithmetic (0x6A-0x7F)
#### FMA (0x6A)
Fused multiply-add: (a * b) + c with a single rounding.

Operands:
- Destination: Non TYPE_VOID (floating-point type)
- A: Non TYPE_VOID (floating-point type)
- B: Non TYPE_VOID (floating-point type)
- C: Non TYPE_VOID (floating-point type)
- TYPE_PARAM5: branch_condition_t (optional)

#### MIN (0x7B)
Compute the minimum of two values.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)

#### MAX (0x7C)
Compute the maximum of two values.

Operands:
- Destination: Non TYPE_VOID
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
