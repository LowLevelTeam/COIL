# Universal Instruction Set Architecture

To help with keeping code more compatible a compile time system of instructions are implemented for conditional code compilation inline configurations and more. 

## ISA (0xF0-0xFE)

```c
IF   = 0xF0
ELIF = 0xF1
ELSE = 0xF2
EIF  = 0xF3 // ENDIF
INCL = 0xF4 // include a file 
SECT = 0xF5 // put this into a specific section
DATA = 0xF6 // insert data directly into program memory
PADD = 0xF7 // padd a program with zeros up to specified btye (warn if this is overflowed)
ABI = 0xF9 // define an ABI
```

### Compilation Conditionals
Compilation conditionals can help with cross platform and cross target code even when specifics are required. An example as given below on how to

```
# at least one parameter must be specified
[0xF0 | 0xF1][operand_count: uint8_t]
  [op1: any]
  [comparison: flag_condition_p]
  [op2: any]

# no arguments
[0xF2 | 0xF3]
```

Every parameter aside from the flag condition can be used as op1 or op2.
**PARAM0** = [flag_condition_p](./../type.md#flag_condition_p)
**PARAM1** = [pu_type_p](./../type.md#pu_type_p)
**PARAM2** = [arch_type_p](./../type.md#arch_type_p)
**PARAM3** = [mode_type_p](./../type.md#mode_type_p)

for example to find out if the target is a CPU you could use the following
```
[0xF0][0x02] // IF (2 operands)
  [0xFE][0x00] // PARAM0 EQ
  [0xFD][0x00] // PARAM1 CPU
[0xF4] // EIF
```

If there are only two operands it is expected that the implicit third operand is easy to work out like in the case above where it is made clear by utilizing PARAM1 it is checking the architecture of the current target.

### INCL
Include a file to help stop code duplication especially for abi definitions. Utilizes the string table.

```
[0xF4][0xF3][string table id: uint64_t]
```

### SECT
Define a linker section.

```
[0xF5][0xF3][string table id: uint64_t]
```

**PARAM0** = [sect_flag_p](./../type.md#sect_flag_p)

### DATA and PADD
Insert bytes into program

```
[0xF6][byte_count: Integral][bytes: uint8_t[byte_count]]
```

Padd bytes in the current section to specified size with zeros for anything which isn't take up by native code (ignored in interpreted environments). Warning if code is bigger then padd offset. (Instruction mainly here to help in situations like legacy x86 bios bootloader)

```
[0xF7][byte index: Integral]
```

### ABI
Define an ABI has to be under a processing unit specification, architecture specification and a mode specification.

```
[0xF9][abi_id: uint64_t]
```

This instruction works by entering a new kind of instruction scope. The abi_id is used to refer to this in scenarios like context switching call, branch, interrupts, etc... and also to control parameters, returns and the ABI specifics.

**ABI instructions**
```c
PARAMS = 0x00
RETS = 0x01
CRSAVE = 0x02 // caller saved registers
CESAVE = 0x03 // callee saved registers
SALLIN = 0x04 // stack allignment
RZONE  = 0x05 // red zone
// needs to be extended for varadic and structures
EABI = 0xFF // exit abi instruction scope
```

#### PARAMS
Define the parameter to register mapping.

```
[0x00][register_count: uint16_t][[register: uint16_t], ...register_count]
```

#### RETS
Define the return to register mapping.

```
[0x01][register_count: uint16_t][[register: uint16_t], ...register_count]
```

#### CRSAVE
Define the caller saved registers

```
[0x02][register_count: uint16_t][[register: uint16_t], ...register_count]
```

#### CESAVE
Define the callee saved registers

```
[0x03][register_count: uint16_t][[register: uint16_t], ...register_count]
```

#### SALLIN
Define the required stack allignment

```
[0x04][stack_allignment: uint16_t]
```

#### RZONE
Define the red zone

```
[0x05][red zone: uint16_t]
```

#### EABI
Exit ABI instruction scope

```
[0xFF]
```

## Related Components

- [Type System](../type.md)