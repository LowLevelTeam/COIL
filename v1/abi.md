# Application Binary Interface (ABI)

## 1. Introduction

COIL's ABI system is a transformative feature that eliminates the need for manual register management in function calls. The ABI system allows COIL code to be both portable across architectures and efficient in execution, solving one of the biggest challenges in assembly-level programming.

An ABI definition specifies:
- Which registers are used for parameter passing
- Which registers are used for return values
- Which registers are caller-saved vs. callee-saved
- Stack alignment requirements
- Red zone size (if applicable)
- Other architecture-specific calling convention details

Unlike traditional assembly languages where programmers must manually place values in specific registers to pass parameters, COIL's ABI system handles this automatically, enabling architecture-independent function calls while maintaining native performance.

## 2. ABI Definition Format

### 2.1 Text Representation

A text-based ABI definition looks like the following:

```
ABI linux_x86_64 {
  PARAMS %r0, %r1, %r2, %r3     ; Registers for parameters
  RETS %r0, %r1                 ; Registers for return values
  CALLER %r0, %r1, %r2, %r3     ; Caller-saved registers
  CALLEE %r4, %r5, %r6, %r7     ; Callee-saved registers
  SALLIGN 16                    ; Stack alignment in bytes
  RZONE 128                     ; Red zone size in bytes
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

## 3. ABI Parameter Type

COIL uses a dedicated parameter type for ABI operations:

```
TYPE_ABICTL = 0xF8  ; ABI control parameter
```

ABI control values:
```
ABICTL_STANDARD = 0x00  ; Use specified ABI convention
ABICTL_PARAM    = 0x01  ; Passing parameters via ABI
ABICTL_RET      = 0x02  ; Returning values via ABI
ABICTL_VARIADIC = 0x03  ; Variadic function support
ABICTL_CUSTOM   = 0x04  ; Custom ABI handling
```

## 4. Using ABIs

### 4.1 ABI Selection

ABIs can be selected at different levels:

1. **Global Default**: Set through configuration
2. **File-Level**: Set with a directive at the file beginning
3. **Function-Level**: Specified in the function declaration
4. **Call-Site**: Specified in the call instruction

### 4.2 Function Declaration with ABI

To declare a function with an ABI:

```
; Declare a function using the linux_x86_64 ABI
SYM function_name, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

Parameter and return types can be specified following the ABI declaration.

### 4.3 Function Call with ABI

To call a function using a specific ABI:

```
; Call a function using the linux_x86_64 ABI
CALL function_name, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
```

### 4.4 Passing Parameters Using ABI

COIL provides a special syntax for passing parameters through an ABI:

```
; Call with parameters using the linux_x86_64 ABI
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, param1, param2

; Receive parameters in the function
SCOPEE
  VAR TYPE_INT32, a
  VAR TYPE_INT32, b
  MOV a, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0  ; First parameter
  MOV b, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1  ; Second parameter
  
  ; Function body
SCOPEL
```

### 4.5 Returning Values Using ABI

COIL provides a special syntax for returning values through an ABI:

```
; Return a value using the linux_x86_64 ABI
VAR TYPE_INT32, result, 42
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result

; Receive the return value after a call
CALL function_name, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, param1, param2
VAR TYPE_INT32, return_value
MOV return_value, TYPE_ABICTL=ABICTL_RET=linux_x86_64
```

### 4.6 Architecture-Independent Code Using ABI

For code that targets multiple architectures, the ABI system enables truly architecture-independent function calls:

```
; This function works identically on any architecture
SYM add_numbers, TYPE_ABICTL=ABICTL_STANDARD=platform_default
  SCOPEE
  ; Get parameters via ABI (no architecture-specific code)
  VAR TYPE_INT, a
  VAR TYPE_INT, b
  MOV a, TYPE_ABICTL=ABICTL_PARAM=platform_default, 0
  MOV b, TYPE_ABICTL=ABICTL_PARAM=platform_default, 1
  
  ; Perform calculation
  VAR TYPE_INT, result
  ADD result, a, b
  
  ; Return via ABI (no architecture-specific code)
  RET TYPE_ABICTL=ABICTL_RET=platform_default, result
  SCOPEL
```

## 5. Standard ABI Definitions

COIL provides built-in ABI definitions for common calling conventions:

### 5.1 System V AMD64 ABI (Linux/BSD/macOS)

```
ABI linux_x86_64
  PARAMS RDI, RSI, RDX, RCX, R8, R9
  RETS RAX, RDX
  CALLER RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, R12, R13, R14, R15
  SALLIGN 16
  RZONE 128
EXIT
```

### 5.2 Microsoft x64 ABI (Windows)

```
ABI windows_x64
  PARAMS RCX, RDX, R8, R9
  RETS RAX
  CALLER RAX, RCX, RDX, R8, R9, R10, R11
  CALLEE RBX, RSP, RBP, RDI, RSI, R12, R13, R14, R15
  SALLIGN 16
  RZONE 0
EXIT
```

### 5.3 ARM AAPCS64

```
ABI arm_aapcs64
  PARAMS X0, X1, X2, X3, X4, X5, X6, X7
  RETS X0
  CALLER X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15
  CALLEE X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30
  SALLIGN 16
  RZONE 0
EXIT
```

### 5.4 RISC-V

```
ABI riscv_lp64d
  PARAMS a0, a1, a2, a3, a4, a5, a6, a7
  RETS a0, a1
  CALLER ra, t0, t1, t2, a0, a1, a2, a3, a4, a5, a6, a7, t3, t4, t5, t6
  CALLEE sp, s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11
  SALLIGN 16
  RZONE 0
EXIT
```

## 6. ABI Interoperability

### 6.1 Foreign Function Interface

ABIs enable interoperability with external code:

1. Call foreign functions by defining an ABI that matches their calling convention
2. Export COIL functions for external use by declaring them with the appropriate ABI

Example of calling a C function from COIL:

```
; Define C function prototype
; int printf(const char* format, ...);
SYM printf, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Call the C function
VAR TYPE_PTR, format_string, "Hello, %s!\n"
VAR TYPE_PTR, name, "COIL"
CALL printf, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, format_string, name

; Get return value (number of characters printed)
VAR TYPE_INT32, chars_printed
MOV chars_printed, TYPE_ABICTL=ABICTL_RET=linux_x86_64
```

### 6.2 Creating ABI-Compatible Libraries

COIL code can expose functions to be called from other languages:

```
; Export a COIL function with linux_x86_64 ABI
SYM coil_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get parameters
  VAR TYPE_INT32, a
  MOV a, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  
  ; Function implementation
  VAR TYPE_INT32, result
  MUL result, a, 2
  
  ; Return using standard ABI so external code can call this
  RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result
  SCOPEL
```

### 6.3 Mixed ABI Usage

Multiple ABIs can be used within the same program to interface with different components:

```
; Call Windows API using Microsoft ABI
CALL CreateFileW, TYPE_ABICTL=ABICTL_PARAM=windows_x64, filename, access, share, security, creation, flags, template

; Call Linux syscall using System V ABI
CALL open, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, path, flags, mode
```

## 7. Comparing with Traditional Function Calls

To illustrate the power of COIL's ABI system, here's a comparison:

### Traditional x86-64 Assembly Function Call:

```
; Caller must know register conventions for System V ABI
mov rdi, 10        ; First parameter in RDI
mov rsi, 20        ; Second parameter in RSI
call add_function  ; Call the function
; Result in RAX    ; Must know return is in RAX

; Function implementation must comply with ABI
add_function:
  mov rax, rdi     ; Get first parameter from RDI
  add rax, rsi     ; Add second parameter from RSI
  ret              ; Return result in RAX
```

### COIL ABI Function Call:

```
; No need to know register conventions
VAR TYPE_INT32, a, 10
VAR TYPE_INT32, b, 20
CALL add_function, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, a, b
VAR TYPE_INT32, result
MOV result, TYPE_ABICTL=ABICTL_RET=linux_x86_64

; Function implementation uses variable abstraction
SYM add_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  VAR TYPE_INT32, x
  VAR TYPE_INT32, y
  MOV x, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  MOV y, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
  
  VAR TYPE_INT32, sum
  ADD sum, x, y
  
  RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, sum
  SCOPEL
```

## 8. Implementation Requirements

A COIL v1 processor must:

1. Support the full ABI definition format
2. Correctly implement ABI-controlled function calls and returns
3. Provide a mechanism for defining custom ABIs
4. Handle the standard ABIs for each supported architecture
5. Automatically manage register preservation according to ABI rules
6. Generate appropriate parameter passing code for the target architecture
7. Support architecture-independent function calls through the ABI system

## 9. Best Practices

1. Use the ABI system for all function declarations and calls
2. Always specify which ABI is being used (e.g., linux_x86_64, windows_x64)
3. Prefer standard ABIs over custom definitions
4. Document any custom ABIs thoroughly
5. Use variable names that match parameter names for clarity
6. Declare parameters at the beginning of function scopes
7. Explicitly name return variables for readability
8. Test ABI compliance thoroughly, especially when interfacing with external code
9. Use platform_default ABI for maximum portability when appropriate

## 10. Advanced ABI Features

### 10.1 Variadic Functions

COIL supports variadic functions (functions with variable arguments) through the ABI system:

```
; Define a variadic function
SYM variadic_func, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get fixed parameters
  VAR TYPE_INT32, arg_count
  MOV arg_count, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  
  ; Process variable arguments
  VAR TYPE_INT32, i, 0
  VAR TYPE_INT32, sum, 0
  
  SYM process_args
  CMP i, arg_count
  BR_GE done_processing
  
  ; Get next argument (1-based index accounting for arg_count)
  VAR TYPE_INT32, next_arg
  ADD i, i, 1
  MOV next_arg, TYPE_ABICTL=ABICTL_VARIADIC=linux_x86_64, i
  
  ; Process argument
  ADD sum, sum, next_arg
  
  BR process_args
  
  SYM done_processing
  RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, sum
  SCOPEL
```

### 10.2 Multiple Return Values

COIL supports returning multiple values through ABIs that provide multiple return registers:

```
; Function that returns multiple values
SYM div_with_remainder, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64
  SCOPEE
  ; Get parameters
  VAR TYPE_INT32, dividend
  VAR TYPE_INT32, divisor
  MOV dividend, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0
  MOV divisor, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 1
  
  ; Calculate quotient and remainder
  VAR TYPE_INT32, quotient
  DIV quotient, dividend, divisor
  
  VAR TYPE_INT32, remainder
  MOD remainder, dividend, divisor
  
  ; Return multiple values
  RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, quotient, remainder
  SCOPEL

; Caller receiving multiple return values
CALL div_with_remainder, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, value, 10
VAR TYPE_INT32, result
VAR TYPE_INT32, remainder
MOV result, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 0    ; First return value
MOV remainder, TYPE_ABICTL=ABICTL_RET=linux_x86_64, 1 ; Second return value
```

## 11. TODO ABI Extensions to add for final version 1

1. **Structured Parameter Passing**: Direct support for structures and arrays
2. **SIMD Register Management**: Specialized handling for vector registers
3. **Exception Handling**: ABI integration with exception mechanisms
4. **Zero-overhead Abstraction**: Further optimizations for ABI performance
5. **Cross-Language Interface**: Enhanced interoperability with high-level languages