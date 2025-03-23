# Application Binary Interface (ABI)
COIL supports creating your own custom calling conventions called an ABI. You define the underlying registers you wish to map parameters and return values for either using a virtual register system for each processing unit or an architectural specific system using the [architectural register system](./reg.md).

COIL understands that systems control calling conventions based on the architecture but the similarities between the C ABI allows you to use directives to get the architecture at compile time and control which ABI should be used.

The ABI is optionally advanced controlling red stack zones, displacement and much more or can be as simple as defining a couple of registers for parameters and a couple of registers for return values.

To define an ABI you need to enter an ABI definition block which has its own opcode set until you use the exit opcode. the ABI opcode set is defined below.

## TODO...
