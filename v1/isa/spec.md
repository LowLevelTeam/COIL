# Special Instructions (0xC0-0xFF)
In COIL there exists certain instructions that are not universally supported across all devices. Such instructions occupy the range of 0xC0-0xFE

0xFF is a special instruction meant to be used for COIL processor attributes and instructions specific to each COIL processor. Instructions like this are frowned upon in the COIL workflow but are seen as something which may be necessary in certain contexts.

Specific instructions are separated into architecture specific and processor specific. An architecture specific instruction would be something like register operations or cpuid which are fundamentally incompatible with each architecture where as an instruction that would be processor compatible is load interrupt table and create interrupt table which is fundamentally the same across architectures but fundamentally impossible on certain processors.

CPU's all share interrupts with one another but other processing units don't have anything similar this is why it would be processor specific.

For normal operations that can be used universally in COIL look into the other files certain control flow, arithmetic, certain memory operations, types and directives can all be used universally.


## Processing Unit Specific 0xB0-0xCF

### CPU
CPU Specific Instructions
## Architecture Specific 0xD0-0xFE
### CPU
CPU Architecture Specific Instructions.
#### 8086

CPU 8086 Specific Instructions

## COIL Processor Specific 0xFF-0xFF

Instructions that are specific to a COIL processor would look something like __attribute__(...) in a higher level language where it takes a parameter to control attributes specific to the COIL processor or just something that extends the functionality of the standard COIL specification.


