# ABI Definition Directives

## Overview

Application Binary Interface (ABI) definition directives allow COIL programs to specify how functions interact with each other and with the system. These directives enable consistent calling conventions, parameter passing, and stack management across different compilation units and platforms.

## Directive Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xF9   | ABI      | Begin ABI definition block |

```
# Define an ABI
[0xF9][0x00]

# Set an ABI Globally for the following code
[0xF9][0x01][abi_id: uint16_t]

# Next symbol is a function that will utilize this ABI
[0xF9][0x02][abi_id: uint16_t]

[0xF9][0x03][var_id: uint16_t][type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...]

[0xF9][0x04][var_id: uint16_t][type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]

[0xF9][0x05][var_id: uint16_t][type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...] [data: optional ...]

[0xF9][0x06][var_id: uint16_t][type opcode: uint8_t] [type extension: optional uint8_t] [type data: optional ...]
```

One thing which may be obvious to notice is that although an ABI defines a way to get parameters there is no function signature or defined return values and parameters. This is because this is an assembly like language and limiting a function to have to stick to some parameters would limit functionality.

You are of course not expected to compile in COIL so this wouldn't matter as no program makes mistakes (unless you program it to of course). Compile time checking should happen in higher level languages when dealing with higher level features like functions.

## Sub-Directives

Within an ABI block, the following sub-directives are available:

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x00   | PARAMS   | Define parameter passing registers |
| 0x01   | RETS     | Define return value registers |
| 0x02   | CRSAVE   | Define caller-saved registers |
| 0x03   | CESAVE   | Define callee-saved registers |
| 0x04   | SALLIN   | Define stack alignment requirements |
| 0x05   | RZONE    | Define red zone size |
| 0xFE   | SETID    | Set the ABI ID for referncing |
| 0xFF   | EABI     | End ABI definition block |

Implement more for name mangling and other ABI specifics like varadics etc...

## Detailed Specifications

### ABI (0xF9)

Begins an ABI definition block.

**Encoding:**
```
[0xF9][0x01][abi_id: Operand]
```

**Behavior:**
- Starts a new ABI definition with the specified ID
- All subsequent sub-directives apply to this ABI until EABI
- ABI ID is a unique identifier used to reference this ABI
- ABI definitions must be under appropriate processing unit, architecture, and mode specifications
- Multiple ABI definitions with the same ID are treated as errors

### PARAMS (0x00)

Defines parameter passing registers.

**Encoding:**
```
[0x00][0x01][register_count: uint16_t][registers: array of uint16_t]
```

**Behavior:**
- Specifies which registers are used for parameter passing
- Register count indicates the number of registers in the list
- Registers are listed in order of parameter assignment
- Parameters beyond available registers use stack according to implementation-defined rules
- Register IDs are architecture-specific (defined in ISA-E)

### RETS (0x01)

Defines return value registers.

**Encoding:**
```
[0x01][0x01][register_count: uint16_t][registers: array of uint16_t]
```

**Behavior:**
- Specifies which registers are used for returning values
- Register count indicates the number of registers in the list
- Multiple registers may be used for large return values
- Return values beyond available registers use implementation-defined mechanisms
- Register IDs are architecture-specific (defined in ISA-E)

### CRSAVE (0x02)

Defines caller-saved registers.

**Encoding:**
```
[0x02][0x01][register_count: uint16_t][registers: array of uint16_t]
```

**Behavior:**
- Specifies which registers must be saved by the caller
- Register count indicates the number of registers in the list
- Caller-saved registers may be modified by the callee without preservation
- Function calls automatically generate code to save and restore these registers when needed
- Register IDs are architecture-specific (defined in ISA-E)

### CESAVE (0x03)

Defines callee-saved registers.

**Encoding:**
```
[0x03][0x01][register_count: uint16_t][registers: array of uint16_t]
```

**Behavior:**
- Specifies which registers must be preserved by the callee
- Register count indicates the number of registers in the list
- Callee-saved registers must be saved and restored by functions that use them
- Function entry/exit points automatically generate code to save and restore these registers when needed
- Register IDs are architecture-specific (defined in ISA-E)

### SALLIN (0x04)

Defines stack alignment requirements.

**Encoding:**
```
[0x04][0x01][alignment: uint16_t]
```

**Behavior:**
- Specifies the required stack alignment in bytes
- Stack pointer must maintain this alignment at function calls
- Alignment is typically a power of 2 (8, 16, etc.)
- Affects code generation for function entry and exit points

### RZONE (0x05)

Defines the red zone size.

**Encoding:**
```
[0x05][0x01][size: uint16_t]
```

**Behavior:**
- Specifies the size of the red zone in bytes
- Red zone is an area below the stack pointer that is guaranteed not to be modified by asynchronous events
- Functions can use the red zone for temporary storage without adjusting the stack pointer
- Size of 0 indicates no red zone is available

### SETID (0xFE)

Set ABI ID

**Encoding:**
```
[0xFE][id: uint16_t]
```

It is recommended to utilize an ID of 1000+ as the first 1000 may be reserved by the standard library.

**Behavior:**
- The id value will be used to reference this ABI

### EABI (0xFF)

Ends an ABI definition block.

**Encoding:**
```
[0xFF]
```

**Behavior:**
- Marks the end of the current ABI definition
- Must match a preceding ABI directive
- Compilation continues normally after this directive

## Custom ABI Registration

Custom ABIs can be defined and used with the following process:

1. Define the ABI using the ABI directive and sub-directives
2. Reference the ABI by ID in function declarations
3. Ensure consistent ABI usage across compilation units

## Related Components

- [Code Organization](./code-organization.md) - INCL, SECT, DATA, PADD directives
- [Extended ISA](../isa-e/overview.md) - Architecture-specific instructions and registers
- [Binary Format](../core/binary-format.md) - COIL binary encoding specification