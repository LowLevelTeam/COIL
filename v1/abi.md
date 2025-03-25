# Application Binary Interface (ABI)
COIL supports creating your own custom calling conventions called an ABI. You define the underlying registers you wish to map parameters and return values for either using a virtual register system for each processing unit or an architectural specific system using the [architectural register system](./reg.md).

COIL understands that systems control calling conventions based on the architecture but the similarities between the C ABI allows you to use directives to get the architecture at compile time and control which ABI should be used.

The ABI is optionally advanced controlling red stack zones, displacement and much more or can be as simple as defining a couple of registers for parameters and a couple of registers for return values.

To define an ABI you need to enter an ABI definition block which has its own opcode set until you use the exit opcode. the ABI opcode set is defined below.

## Format
A text based format would look something like the following.
In practice when the ABI directive is called the instruction set will completly change to just 6 instructions as followed.

```
abi standard {
  param_regs: %r0, %r1, %r2, %r3       ; Registers for parameters
  return_regs: %r0, %r1                ; Registers for return values
  caller_saved: %r0, %r1, %r2, %r3     ; Caller-saved registers
  callee_saved: %r4, %r5, %r6, %r7     ; Callee-saved registers
  stack_align: 16                      ; Stack alignment in bytes
  red_zone: 128                        ; Red zone size in bytes
}
```


## Instructions

#### EXIT 0x00
End Scope

#### PARAMS 0x01
Define register mapping for parameter values.

#### RETS 0x02
Define register mapping for return values.

#### CALLER 0x03
Define registers potentially affected that should be save before hand.

#### CALLEE 0x04
Define registers potentially affected that should be save in the function.

#### SALLIGN 0x05
Define the stack allignment

#### RZONE 0x06
Define the red zone


