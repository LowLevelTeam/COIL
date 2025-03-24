# Control Flow (0x01-0x0F)
Control Flow operations exist to help with conditional execution. Manual control over what is executed at runtime is an incredibly valuable feature and these instructions faciltate the needed communication. Below are specific instructions to set flags for conditional executions, operations to act on flags with unique features and defining of targets which would otherwise be a directive

## Instructions

#### SYM (0x01)
Define a symbol. Worked out by either the linker or attempted to be worked out at COIL processor compilation into a program address.

Symbols should be defined with either no ABI or an ABI id. If no ABI then the following is not used. If an ABI then you can define parameters with types and a variable identifier and then define the return values. On a call to ret the return values will be automatically 

```
Operands:
- Symbol: TYPE_SYM
- TYPE_PARAM0: symbol_parameter0_t (optional)
```

#### BR (0x02)
Branch to a program address with parameters based on an ABI specification.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

#### CALL (0x03)
Enter procedure with parameters based on an ABI specification.

```
Operands:
- Target: TYPE_PTR | TYPE_SYM | TYPE_*UNT*
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

#### RET (0x04)
Return from procedure.

ABI compatible for returning values according to ABI specification.

```
Operands:
- TYPE_PARAM0: branch_ctrl_t (optional)
- TYPE_PARAM5: branch_condition_t (optional)
```

#### CMP (0x05)
Compare values and set flags without changing registers.

```
Operands:
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
```

#### TEST (0x06)
Bitwise AND and set flags without changing registers.

```
Operands:
- Left: Non TYPE_VOID
- Right: Non TYPE_VOID
```

## References
[type](./type.md)
[abi](./../abi.md)
