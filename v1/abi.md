# Application Binary Interface (ABI)

## 1. Introduction

COIL supports creating custom calling conventions through Application Binary Interface (ABI) definitions. The ABI system allows for precise control over function calls, parameter passing, and return value handling across different architectures and execution environments.

An ABI definition specifies:
- Which registers are used for parameter passing
- Which registers are used for return values
- Which registers are caller-saved vs. callee-saved
- Stack alignment requirements
- Red zone size (if applicable)
- Other architecture-specific calling convention details

## 2. ABI Definition Format

### 2.1 Text Representation

A text-based ABI definition looks like the following:

```
abi standard {
  param_regs: %r0, %r1, %r2, %r3       ; Registers for parameters
  return_regs: %r0, %r1                ; Registers for return values
  caller_saved: %r0, %r1, %r2, %r3     ; Caller-saved registers
  callee_saved: %r4, %r5, %r6, %r7     ; Callee-saved registers
  stack_align: 16                      ; Stack alignment in bytes
  red_zone: 128                        ; Red zone size in bytes
}
```

### 2.2 Binary Encoding

When the `ABI` directive is used, the instruction set temporarily changes to use a special set of ABI definition opcodes until the `EXIT` opcode is encountered:

```
0x00 EXIT     - End ABI definition block
0x01 PARAMS   - Define register mapping for parameter values
0x02 RETS     - Define register mapping for return values
0x03 CALLER   - Define caller-saved registers
0x04 CALLEE   - Define callee-saved registers
0x05 SALLIGN  - Define stack alignment
0x06 RZONE    - Define the red zone size
```

## 3. Using ABIs

### 3.1 ABI Selection

ABIs can be selected at different levels:

1. **Global Default**: Set through configuration
2. **File-Level**: Set with a directive at the file beginning
3. **Function-Level**: Specified in the function declaration
4. **Call-Site**: Specified in the call instruction

### 3.2 Function Declaration with ABI

To declare a function with an ABI:

```
SYM function_name, TYPE_PARAM0=BRANCH_CTRL_ABI
```

Parameter and return types can be specified following the ABI declaration.

### 3.3 Function Call with ABI

To call a function using a specific ABI:

```
CALL function_name, TYPE_PARAM0=BRANCH_CTRL_ABI
```

### 3.4 Architecture-Specific ABI Selection

For code that targets multiple architectures, conditional ABI selection can be used:

```
ABI standard_x86 {
  // x86-specific ABI definition
}

ABI standard_arm {
  // ARM-specific ABI definition
}

// Then use conditionals to select the appropriate ABI:
IF ARCH == X86
  CALL function, TYPE_PARAM0=BRANCH_CTRL_ABI, standard_x86
ELIF ARCH == ARM
  CALL function, TYPE_PARAM0=BRANCH_CTRL_ABI, standard_arm
ENDIF
```

## 4. Standard ABI Definitions

COIL does not mandate specific ABIs but recognizes common calling conventions for major architectures. Implementations may provide built-in definitions for:

- System V AMD64 ABI (x86-64 Linux/BSD/macOS)
- Microsoft x64 ABI (Windows)
- ARM AAPCS
- RISC-V calling conventions

## 5. ABI Interoperability

### 5.1 Foreign Function Interface

ABIs enable interoperability with external code:

1. Call foreign functions by defining an ABI that matches their calling convention
2. Export COIL functions for external use by declaring them with the appropriate ABI

### 5.2 Mixed ABI Usage

Multiple ABIs can be used within the same program to interface with different components:

- Operating system system calls
- External libraries
- Architecture-specific optimized routines

## 6. Implementation Requirements

A COIL v1 processor must:

1. Support the full ABI definition format
2. Correctly implement ABI-controlled function calls and returns
3. Provide a mechanism for defining custom ABIs
4. Handle the standard ABIs for each supported architecture

## 7. Best Practices

1. Use consistent ABIs for related functions
2. Document custom ABIs thoroughly
3. Prefer standard ABIs where possible for better interoperability
4. Use the configuration system to manage ABIs across multiple architectures
5. Test ABI compliance thoroughly, especially when interfacing with external code