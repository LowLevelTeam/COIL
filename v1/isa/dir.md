# Directive Instructions (0xB0-0xBF)

#### ARCH (0xB0)
Set the target architecture.

Operands:
- Architecture: TYPE_UNT8 (architecture code)

#### PROC (0xB1)
Set the target processor.

Operands:
- Processor: TYPE_UNT8 (processor code)

#### MODE (0xB2)
Set the target processor mode. Use an ID provided by a configuration file to specify different feature sets for the same device. Switching between modes is hanlded by the user this just controls the native binary made at each location to allign with the specfication. This mode is somewhat cross architectural but not entirely, it is expected for most cases that PROC and ARCH is set before this but not required if a use case where you want specifically 32 bit code to be executed in some place and 64 bit in another then this could be useful.

[reference](./../config.md#device-targets)

#### ALIGN (0xB3)
Align the next instruction or data to a specified boundary.
Can be used to padd program to a specified index as needed by specification situations like a x86 BIOS bootloader.

Operands:
- Alignment: Non TYPE_VOID

#### SECTION (0xB4)
Define a new section in the output.

Operands:
- Name: TYPE_SYM
- Attributes: Non TYPE_VOID

#### DATA (0xB5)
Insert data into program memory.

Operands:
- Type: TYPE_*
- Value: Non TYPE_VOID

#### IF   (0xB6)
Compilation controlled if statement to help with code inclusion management. For scenarios where register management is essential but architecture compatibility is still wanted for multiple selected architectures. In this case using a compile time IF statement could be considered useful.
TODO...

#### ELIF (0xB7)
TODO...

#### ELSE (0xB8)
TODO...

#### ENDIF (0xB9)
TODO...

#### ABI
Inline ABI definition should be included behind multiple compilation controlled statements as it is meant to be defined in a configuration file. An ABI defines a calling convention with a name, architecture which can allow the processor to select the needed ABI at compile time. An example would be for a system creating a linux 32 bit and 64 bit compatible program and although the syscalls are different between 32 bit and 64 bit they still want it to be compatible with arm and x86 where the system calls are generally the same except for the registers. In this case you could define a common ABI controlled by the compilation targets to specify the standard arguments and return value registers and then call it in the same way in the COIL code.

TODO...

