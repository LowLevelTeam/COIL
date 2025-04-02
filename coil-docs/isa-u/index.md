# Universal Instruction Set Architecture

For globaly compatible code across all types of processing units, architectures and modes these instructions are only limited by imagination.

## ISA
### Control Flow (0x00-0x1F)
Control the execution of the program.

```c
NOP = 0x00
BR = 0x01
CALL = 0x02
RET = 0x03
CMP = 0x04
TEST = 0x05
```

#### NOP
Literally no operation.

```
[0x00]
```

#### BR
Switch context

```
[0x01][0x01][address: integral, symbol]
[0x01][0x02][0xFE][FLAG_CONDITION][address: integral, symbol]
```

PARAM0 = [flag_condition_p](./../type.md#flag_condition_p)

#### CALL
Set return address and Switch context

```
[0x02][0x01][address: integral, symbol]
[0x02][0x02][0xFE][FLAG_CONDITION][address: integral, symbol]
```

PARAM0 = [flag_condition_p](./../type.md#flag_condition_p)

#### RET
Switch context to return address

operand count is needed as return could be conditional.

```
[0x03][0x00]
[0x03][0x01][0xFE][FLAG_CONDITION]
```

PARAM0 = [flag_condition_p](./../type.md#flag_condition_p)

#### CMP
Compare two values to set flags

```
[0x04][0x02][op1: Operand][op2: Operand]
```

#### TEST
Simple compare and set flags

```
[0x05][0x02][op1: Operand][op2: Operand]
```

### Memory Operations (0x20-0x3F)
Allocate space, free space and move memory.

```c
MOV = 0x20
PUSH = 0x21
POP = 0x22
LEA = 0x23
SCOPE = 0x24
SCOPL = 0x25
VAR = 0x26
XCHG = 0x27
CAS = 0x28
```


### Arithmetic Operations (0x40-0x5F)
Act on allocated memory with arithmetic instructions.

```c
ADD = 0x40
SUB = 0x41
MUL = 0x42
DIV = 0x43
MOD = 0x44
INC = 0x45
DEC = 0x46
NEG = 0x47
ABS = 0x48
SQRT = 0x49
CEIL = 0x4A
FLOR = 0x4B
ROND = 0x4C
```

### Bit Manipulation (0x60-0x7F)
Act on allocated memory with bit manipulation instructions.

```c
AND = 0x60
OR = 0x61
XOR = 0x62
NOT = 0x63
SHL = 0x64
SHR = 0x65
SAL = 0x66
SAR = 0x67
```

### Vector/Array Operations (0x80-x8F)
Act on allocated memory with vector / multiple scalar specific instructions.

```c
VGET = 0x80 // get a vector element
VSET = 0x81 // set a vector element
VDOT = 0x82 // vector dot product
```

#### VGET
Get a vector element (copies value into destination)

```
[0x80][0x03][vector: vector type][index: integral][dest: typeof vector element non immediate]
```

#### VSET
Get a vector element (sets value in the vector to source)

```
[0x81][0x03][vector: vector type][index: integral][src: typeof vector element non immediate]
```

#### VDOT
Complete a dot product between two types of multiple scalars.

```
dest = dot(dest,src1) -> [0x82][0x02][dest: vector type][src1: vector type]
dest = dot(src1,src2) -> [0x82][0x03][dest: vector type][src1: vector type][src2: vector type]
```

### Special Operations (0x90-0x9F)
Special operations that don't fit anywhere else for example crypto operations.

No special operation is implemented right now and should cause a Not Implemented Yet error as of now.

## Related Components

- [Type System](../type.md)