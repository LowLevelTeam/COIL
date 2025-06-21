# Orion

Orion toolchain is an intermediate representation built for optimizations and compatibility.


## Example Toolchain
Source Language
Orion++ the high level IR
Orion.* the low level IR
Native ISA


Example toolchain
```
[.c] -> orioncc -> [.orionpp] -> oc++ -> [.*.orion] -> oc -> [.o] -> ol -> [.exe,.out,.dll,.lib,.a,.so]
```


Differences in human and binary variations.

In human variations it may be common to see quite assembly looking assembly structure. for example defining symbols at abstract points. These are mainly to facilitate easy viewing.
Keep in mind that the binary variations exist in an object format where symbols are specified in tables.


