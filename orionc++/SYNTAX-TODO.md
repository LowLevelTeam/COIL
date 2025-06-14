# Specifications on the SYNTAX which could be subject to change

Certain changes will be made to the primitive types. Offloading emulation of types to the standard library could be incredibly beneficial.
Native and Emulated types should not be needed to be specified by the user, types like vec128 are always available to orion++ compiler to compile into and will then use native or emulation at JIT.

Certain emulated types may rely on the standard library wich should be made optional.





