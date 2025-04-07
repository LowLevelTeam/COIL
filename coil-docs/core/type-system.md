# COIL Type System (TS)

The COIL TS is a needed complexity in the development ecosystem allowing for the instruction set to be kept relatively simple. instead of addl, addw, adds all being seperate the COIL TS allows just one add instruction and have the real instruction or inention be worked out by the processor. This kind of language is really good for not only writing simple code but also keeping files small, however in certain cases where a fast runtime compilation of the language is needed you should instead check out COIL Byte Code (CBC). A more instruction complex set with limited extensibility and possibly bloated in sizse but allows for extermely fast runtime compilation. 

COIL Byte Code can be made from COIL by a COIL Processor where normal COIL instructions may be extended out into multiple COIL Byte Code instructions.

While reading you may discover certain types under an extension classification, this is to help COIL processors implement part of this incredibly complex type system, major extensions would be composite types, multi dimensional types and complex types all of which require complex implementation in code generation but also in the COIL processor TS implementation.


