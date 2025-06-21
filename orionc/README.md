# Orion

If you are a frontend compiler developer you will very rarely have to look at the Orion Variations as orion++ is the universal instruction set for high level languages like you are probably building.

Cases you may have to know about this is any inline assembly or your own assembly syntax.


## Orion Variations

#### Orion.x86

ISAs: x86, x86-32, x86-64

Orion x86 is the asbtracted ISA for the 16, 32 and 64 bit variations of the x86 language.

Why Orion x86 is used over manually compiling down to the x86 32 and 64 bit variations is to allow for great instruction optimization and optimization.
this system also allows for legacy 16 bit systems to be easier to support.

The general gist of the Orion.x86 variation is all features of a full x86-64 with all extensions should be assumed.
From here the orion compiler will check for native features either at runtime (JIT) or compile time (AOT) and create a final binary with all features possible.
This allows for the greatest optimizations utilizing SIMD and 64 bit integers when generating and having the orion compiler emulate instructions where possible.
Emulate can sound scary but in this case all it means is for example expanding a normal 64 bit operation to the combination of 32 bit operations or vectors to simply expanding vector instructions the abstractions will always be costless.

Certain situations may require complex features. For example atomics, floats, protection and all the complex features of modern CPUs.
These features are not currently supported anyway but as the instruction set expands so will the support for these.

#### Orion.ARM

ISAs: ARM Thumb, ARM32, ARM64

#### Orion.RISCV6

ISAs: RISC-V 32, RISC-V 64, RISC-V 128




