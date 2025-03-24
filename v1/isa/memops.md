# Memory Operations (0x10-0x2F)
Memory operations...

## Instructions

#### MOV (0x10)
This function serves to transmit data from multiple formats via a source into a destination.

```
Operands:
- Destination: Non TYPE_VOID
- Source: Non TYPE_VOID
- TYPE_PARAM5: branch_condition_t (optional)
```

#### PUSH (0x11)
Push a state onto the stack.
Can be used with variables to demote its position. When pushing a variable the processor will priotise this variable on the stack and free up a register slot if using one.

Operand must be of a variable type

Operands:
- Source: Non TYPE_VOID, (!TYPEEXT_VAR)
- TYPE_PARAM5: branch_condition_t (optional)

#### POP (0x12)
Pop a state from the stack.
Can be used with variables to promoite its position. When popping a variable the processor will priotise this variable in a register.

Operand must be of a variable type

Operands:
- Source: Non TYPE_VOID, (!TYPEEXT_VAR)
- TYPE_PARAM5: branch_condition_t (optional)

#### LEA (0x13)

#### SCOPEE (0x14)

#### SCOPEL (0x15)

#### VAR (0x16)

#### MEMCPY (0x17)

#### MEMSET (0x18)

#### MEMCMP (0x19)

#### XCHG (0x1A)

#### CAS (0x1B)

#### PIN (0x2E)
Pin memory for DMA operations.

```
Operands:
- Address: TYPE_PTR
- Size: TYPE_*UNT*
```

#### UNPIN (0x2F)
Unpin memory after DMA operations.

```
Operands:
- Address: TYPE_PTR
```
