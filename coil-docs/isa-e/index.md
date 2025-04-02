# Extended Instruction Set Architecture

For target specific code.

## ISA

### CPU Operations (0xA0-0xBF)
Works globally across all Central Processing Units.

```c
INT = 0xA0
IRET = 0xA2
SYSCALL = 0xA1 // SYSENTER on 32 bit
SYSRET = 0xA2 // SYSEXIT on 32 bit
PUSHA = 0xA3
POPA = 0xA4
PUSHFD = 0xA5
POPFD = 0xA6
```

#### x86-Specific Operations (0xC0-0xDF)

x86 instructions
```c
CPUID 0xC0
IN = 0xC1 // i think cpu ports are specific to x86
OUT = 0xC2
LIDT = 0xC3 // x86-32
```

x86 register system (due to x86 backwards compatibility all registers of a lower mode can be utilized in higher modes)

Utilize this with the [REG type](./../type.md#register)

```c
// available 16 bit
AX = 0x0000
AH = 0x0001
AL = 0x0002
BX = 0x0003
BH = 0x0004
BL = 0x0005
CX = 0x0006
CH = 0x0007
CL = 0x0008
DX = 0x0009
DH = 0x000A
DL = 0x000B

SP = 0x0100
SPL = 0x0101
BP = 0x0102
BPL = 0x0103
DI = 0x0104
DIL = 0x0105
SI = 0x0106
SIL = 0x0107

SS = 0x0200
CS = 0x0201
DS = 0x0202
ES = 0x0203
FS = 0x0204
GS = 0x0205

FL = 0x0300

// available 32 bit
EAX = 0x000C
EBX = 0x000D
ECX = 0x000E
EDX = 0x000F

ESP = 0x0108
EBP = 0x0109
EDI = 0x010A
ESI = 0x010B

EFL = 0x0301

// available 64 bit
RAX = 0x0010
RBX = 0x0011
RCX = 0x0012
RDX = 0x0013

RSP = 0x010C
RBP = 0x010D
RDI = 0x010E
RSI = 0x010F

RFL = 0x0302
```

#### ARM-Specific Operations (0xC0-0xDF)
TODO...


## Related Components
- [Type System](../type.md)

