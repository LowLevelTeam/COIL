# ISA Reference

All instructions are encoded first and foremost with a 1 byte opcode signifying the class of instructions it belongs to and either partially or in full determines the type of the instruction.

After this could be another opcode if dealding with an extended opcode set otherwise it will go straight into the number of operands. The number of operands is optionally specified depending on if it is possible for the instruction to have an operand.

One good example is the no operation (NOP) instruction that can not and will never accept an operand making the operand count redundant.

Aside from these rare cases the operand count should always be passed first.

```
[opcode: uint8_t] [extended opcode: optional uint8_t] [operand_count: uint8_t] [[operand: Operand], ...operand_count]
```

Operands are a bit more complex then the instruction containing type information, values and possibly more. Operands will always be have the first byte be the type specification similar to opcode.

The Type Opcode will determine the following format, in cases where the type is not void and not a parameter a type extension should be expected which assigns properties to the type.

Following the type opcode and type extension on the 16th bit boundary is the type data. The type data has its size determined by the type opcode and extension and requires special parsing same with the default data.

The main separation between type data and data is that information beyond the type opcode and extension may be needed to fully explain the type like an anonymous composite type would have its members defined and a complex type have its size defined.

In cases where the type is immediate as defined by the type extension the full size of the type should be expected in the case where a variable is expected a uint64_t should be expected equal to the variable id and finally in the case where a symbol is expected a uint64_t should also be found with a value equal to the symbols position in the symbol table.

```
type Operand = [type opcode: uint8_t][type extension: optional uint8_t][type data: optional ...][data: optional ...]
```

## Related Components

- [COIL Type System](./type.md)
- [COIL Universal ISA](./isa-u/index.md)
- [COIL Extended ISA](./isa-e/index.md)
- [COIL Compiler ISA](./isa-c/index.md)

