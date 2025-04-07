# Parameter definitions

## Overview

## Parameters

### flags_p

Used for conditional execution:

| Value  | Name           | Description                  |
|--------|----------------|------------------------------|
| 0x00   | FLAG_COND_EQ   | Equal                        |
| 0x01   | FLAG_COND_NEQ  | Not equal                    |
| 0x02   | FLAG_COND_GT   | Greater than                 |
| 0x03   | FLAG_COND_GTE  | Greater than or equal        |
| 0x04   | FLAG_COND_LT   | Less than                    |
| 0x05   | FLAG_COND_LTE  | Less than or equal           |
| 0x06   | FLAG_COND_O    | Overflow                     |
| 0x07   | FLAG_COND_NO   | No Overflow                  |
| 0x08   | FLAG_COND_S    | Sign                         |
| 0x09   | FLAG_COND_NS   | Not sign                     |
| 0x0A   | FLAG_COND_Z    | Equal Zero                   |
| 0x0B   | FLAG_COND_NZ   | Not Equal Zero               |
| 0xFF   | FLAG_COND_DEF  | Is value defined             |


### pu_p

Specify the target processing unit:

| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | PU_CPU   | Central Processing Unit      |
| 0x01   | PU_GPU   | Graphic Processing Unit      | (Planned)

### arch_p

Specify the target architecture (values depend on PU):

For PU_CPU:
| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | CPU_X86  | x86 architecture             |
| 0x01   | CPU_ARM  | ARM architecture             |

For PU_GPU
| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | GPU_NV   | Nvidia architecture (SASS)   |
| 0x01   | GPU_AMD  | AMD architecture (GCN,RDNA)  |


### mode_p

Specify the architecture mode:

| Value  | Name     | Description                  |
|--------|----------|------------------------------|
| 0x00   | MODE_8   | 8-bit mode                   |
| 0x01   | MODE_16  | 16-bit mode                  |
| 0x02   | MODE_32  | 32-bit mode                  |
| 0x03   | MODE_64  | 64-bit mode                  |
| 0x04   | MODE_128 | 128-bit mode                 |

### section_p

Specify section capabilities as a bitmap:

| Value  | Name       | Description                |
|--------|------------|----------------------------|
| bit 0  | SECT_EXEC  | Executable                 |
| bit 1  | SECT_READ  | Readable                   |
| bit 2  | SECT_WRITE | Writeable                  |
| bit 3  | SECT_INIT  | Initalized Data            |
| bit 4  | SECT_UNIT  | Uninitalized Data          |
| bit 5  | SECT_ALLC  | Allocated At Runtime       |
| bit 6  | Reserved   | Reserved          |
| bit 7  | Reserved   | Reserved          |

### abi_p

Specify the functionality of the ABI compiler instruction

| Value  | Name       | Description                |
|--------|------------|----------------------------|
| 0x00   | Definition | Executable                 |
| 0x01   | Set Global | Readable                   |
| 0x02   | Set Next   | Writeable                  |
| 0x03   | Get Param  | Get Parameter of function  |
| 0x04   | Set Return | Set Return of function     |
| 0x05   | Set Param  | Set parameter for call     |
| 0x06   | Get Return | Get return from call       |

