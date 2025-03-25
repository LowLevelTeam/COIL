# COIL Register System

## 1. Introduction

The COIL register system provides a flexible and architecture-independent way to access and manipulate hardware registers. While COIL's variable system is the preferred mechanism for most code, direct register access is essential for system-level programming, bare metal development, and performance-critical code.

COIL v1 focuses on CPU register systems, with extension points for other processing units in future versions.

## 2. Register Types

COIL provides special types for register access:

```
TYPE_RGP = 0x92  // Register general purpose
TYPE_RFP = 0x93  // Register floating point
TYPE_RV  = 0x94  // Register vector
TYPE_RS  = 0x95  // Register segment
TYPE_SAR = 0x96  // State - all registers
TYPE_SAF = 0x97  // State - all flags
TYPE_SES = 0x98  // State - extended processor state
TYPE_SS  = 0x99  // State - complete state
TYPE_IP  = 0x9A  // Register Instruction Pointer
TYPE_SP  = 0x9B  // Register Stack Pointer
TYPE_BP  = 0x9C  // Register Base Pointer
```

These types are used with a register identifier that is specific to the currently selected architecture.

## 3. Setting the Register Environment

To use architecture-specific registers, you must first set the processor and architecture context:

1. Set the processor type using the `PROC` directive
2. Set the architecture using the `ARCH` directive
3. Optionally set the mode using the `MODE` directive

Example:
```
PROC 0x01  ; CPU
ARCH 0x02  ; x86-64
MODE 0x01  ; Long mode
```

## 4. CPU Architecture Register Systems

### 4.1 x86-64 Registers

#### General Purpose Registers
```
RAX = 0x00
RBX = 0x01
RCX = 0x02
RDX = 0x03
RSI = 0x04
RDI = 0x05
RSP = 0x06
RBP = 0x07
R8  = 0x08
R9  = 0x09
R10 = 0x0A
R11 = 0x0B
R12 = 0x0C
R13 = 0x0D
R14 = 0x0E
R15 = 0x0F
```

Lower portions of these registers are accessible through specific encoding.

#### Floating Point Registers
```
XMM0-XMM15 = 0x00-0x0F  ; SSE registers
YMM0-YMM15 = 0x10-0x1F  ; AVX registers
ZMM0-ZMM31 = 0x20-0x3F  ; AVX-512 registers
```

#### Segment Registers
```
CS = 0x00
DS = 0x01
ES = 0x02
FS = 0x03
GS = 0x04
SS = 0x05
```

### 4.2 ARM64 Registers

#### General Purpose Registers
```
X0-X30 = 0x00-0x1E  ; 64-bit general purpose
SP     = 0x1F       ; Stack pointer
```

Lower 32-bit portions (W0-W30) are accessible through specific encoding.

#### Floating Point/Vector Registers
```
V0-V31 = 0x00-0x1F  ; SIMD and FP registers
```

### 4.3 RISC-V Registers

#### General Purpose Registers
```
X0-X31 = 0x00-0x1F  ; General purpose registers
```

Where X0 is hardwired to zero, and other registers have conventional uses.

#### Floating Point Registers
```
F0-F31 = 0x00-0x1F  ; Floating point registers
```

## 5. Register Access

### 5.1 Direct Register Reference

To reference a register directly:

```
MOV TYPE_RGP=RAX, 42       ; Store value 42 in RAX (x86-64)
MOV dest, TYPE_RGP=X0      ; Read from X0 into dest (ARM64)
```

### 5.2 Register State Operations

Register state operations affect multiple registers at once:

```
PUSH TYPE_SAR              ; Push all registers
POP TYPE_SAF               ; Pop all flags
```

## 6. Best Practices

1. **Minimize Direct Register Use**: Use variables when possible to allow the COIL processor to optimize register allocation.

2. **Architecture Abstraction**: When targeting multiple architectures, use conditional compilation for architecture-specific register code:

   ```
   IF ARCH == X86_64
     MOV TYPE_RGP=RAX, value
   ELIF ARCH == ARM64
     MOV TYPE_RGP=X0, value
   ENDIF
   ```

3. **Register Conventions**: Follow the ABI register conventions for the target architecture.

4. **Register Preservation**: When using registers directly, ensure proper saving and restoring according to the ABI.

5. **Documentation**: Clearly document register usage, especially for code that will be called by others.

## 7. Future Extensions

COIL v3 will extend the register system to support:
- GPU register models
- TPU register models
- Other specialized accelerator registers

The basic type system and access patterns will remain consistent, but the register identifiers and capabilities will be specific to each processing unit type.