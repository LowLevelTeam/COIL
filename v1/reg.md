# Register System

COIL has its main goal of being partially processing unit independent and fully architectural independent but this is the real world and differences do exist even when they are fundamentally the same. Differences can be abstracted away and worked and left to COIL to emulate but why? Why does a virtual register system need to exist? What use can you get out of virtual registers when a full fledged variable system exists.

For the most part direct usage of the register system won't even be effective in optimizing code, you limit the COIL processor from optimizing because you may be able to optimize this better yourself.

We understand that direct register manipulation is a part of low level programming and sometimes unavoidable, we try with as many abstractions for registers as possible but sometimes they just are not enough. In the case where registers are imperative to your program the register system can be used.

The register system is architecture dependent, a different register system exists in the architecture specific section of COIL and you will need to look through this for the specific architecture you are targeting.


## Setting up register environment
To setup the register environment you will have to first set the processor target context to the processing unit the architecture belongs to. After this has been set you can then set the architecture target context to the architecture you wish to use. The instruction is detailed in the ISA but it will consist of a binary signature for an architecture like x86-64 or ARM-64 specified below.

The register system defined here is also used in ABI definitions to define the target registers for parameters and return.
Understanding the register system is important for programmers creating bare metal code and we understand this need and support it fully.

## CPU Architectures
#### x86-16
#### x86-32  
#### x86-64
#### ARM-32
#### ARM-64
#### RISCV-32
#### RISCV-64
