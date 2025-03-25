# Directive Instructions (0xB0-0xBF)

Directive instructions control the compilation process rather than generating runtime code. They provide information to the COIL processor about how to handle the code, which architecture to target, and other aspects of the compilation environment.

## Core Directives

#### PROC (0xB1)
Set the target processor type.

```
Operands:
- Processor: TYPE_UNT8 (processor code)
```

Processor codes:
```
0x01 - CPU
0x02 - GPU (reserved for v3)
0x03 - TPU (reserved for v3)
0x04 - DSP (reserved for v3)
0x05 - FPGA (reserved for v3)
```

#### ARCH (0xB0)
Set the target architecture and optionally its mode. This directive must be preceded by the PROC directive as architecture is specific to a processor type.

```
Operands:
- Architecture: TYPE_UNT8 (architecture code)
- Mode: TYPE_UNT8 (mode code, optional)
```

CPU architecture codes:
```
0x01 - x86
0x02 - ARM
0x03 - RISC-V
0x04 - MIPS
0x05 - PowerPC
```

Mode codes for x86:
```
0x01 - 16-bit mode (Real mode)
0x02 - 32-bit mode (Protected mode)
0x03 - 64-bit mode (Long mode)
```

Mode codes for ARM:
```
0x01 - 32-bit mode (AArch32)
0x02 - 64-bit mode (AArch64)
```

Mode codes for RISC-V:
```
0x01 - 32-bit mode (RV32)
0x02 - 64-bit mode (RV64)
0x03 - 128-bit mode (RV128)
```

Examples:
```
PROC 0x01           ; Select CPU
ARCH 0x01, 0x03     ; Select x86 architecture in 64-bit mode
```

#### ALIGN (0xB3)
Align the next instruction or data to a specified boundary.
Can be used to pad program to a specified index as needed by specification situations like an x86 BIOS bootloader.

```
Operands:
- Alignment: Non TYPE_VOID
```

#### SECTION (0xB4)
Define a new section in the output.

```
Operands:
- Name: TYPE_SYM
- Attributes: Non TYPE_VOID
```

Section attributes are bit flags:
```
0x01 - Executable
0x02 - Writable
0x04 - Readable
0x08 - Initialized data
0x10 - Uninitialized data
```

#### DATA (0xB5)
Insert data into program memory.

```
Operands:
- Type: TYPE_*
- Value: Non TYPE_VOID
```

## Conditional Compilation

#### IF (0xB6)
Begin a conditional compilation block. This allows for architecture-specific code without complicating the runtime model.

```
Operands:
- Condition: Non TYPE_VOID
```

Common condition patterns:
```
IF ARCH == 0x01         ; If architecture is x86
IF ARCH == 0x01 && MODE == 0x03  ; If architecture is x86 in 64-bit mode
IF PROC == 0x01         ; If processor is CPU
```

#### ELIF (0xB7)
Alternative condition for conditional compilation.

```
Operands:
- Condition: Non TYPE_VOID
```

#### ELSE (0xB8)
Default case for conditional compilation.

```
Operands:
- None
```

#### ENDIF (0xB9)
End conditional compilation block.

```
Operands:
- None
```

## ABI Management

#### ABI (0xBA)
Define an Application Binary Interface (ABI). Once this directive is encountered, the instruction set temporarily changes to the ABI-specific instruction set until EXIT is encountered.

```
Operands:
- Name: TYPE_SYM
```

ABI-specific instructions:
```
0x00 EXIT    - End ABI definition block
0x01 PARAMS  - Parameter registers
0x02 RETS    - Return registers
0x03 CALLER  - Caller-saved registers
0x04 CALLEE  - Callee-saved registers
0x05 SALLIGN - Stack alignment
0x06 RZONE   - Red zone size
```

Example:
```
ABI standard_x86_64
  PARAMS RAX, RDI, RSI, RDX
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

## Implementation Notes

1. Directives take effect immediately when encountered during processing.
2. Directives have no runtime effect - they only influence the compilation process.
3. Conditional compilation directives may be nested up to 32 levels deep.
4. Sections defined with the SECTION directive are linked according to the linker configuration.