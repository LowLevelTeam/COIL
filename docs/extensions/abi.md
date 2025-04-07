# Application Binary Interface (ABI) Extension

## Overview

The Application Binary Interface (ABI) Extension defines standardized calling conventions, data layout rules, and system interaction methods for COIL programs. This ensures binary compatibility between different COIL implementations and enables interoperability with other languages and systems.

## Extension Information

- **Extension ID**: 0x03
- **Extension Name**: ABI
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## ABI Identifiers

Each ABI has a unique identifier:

| ABI ID | Name | Description |
|--------|------|-------------|
| 0x01   | SystemV_x64 | System V AMD64 ABI (Linux, BSD, macOS) |
| 0x02   | Win64 | Windows x64 ABI |
| 0x03   | Win32 | Windows x86 (32-bit) ABI |
| 0x04   | AAPCS | ARM Procedure Call Standard |
| 0x05   | AAPCS64 | ARM64 Procedure Call Standard |
| 0x08   | COIL_Standard | COIL Default ABI |
| 0xFF   | Native | Use target platform's native ABI |

## Calling Conventions

### Parameter Passing

Each ABI defines specific parameter passing rules:

#### System V AMD64 ABI (0x01)

- Integer/pointer arguments: RDI, RSI, RDX, RCX, R8, R9
- Floating-point arguments: XMM0-XMM7
- Additional arguments: Stack (right-to-left)
- Return value: RAX (integer), XMM0 (floating-point)
- Caller-saved: RAX, RCX, RDX, RDI, RSI, R8-R11, XMM0-XMM15
- Callee-saved: RBX, RBP, R12-R15, RSP

#### Windows x64 ABI (0x02)

- First four arguments: RCX, RDX, R8, R9
- Floating-point arguments: XMM0-XMM3
- Additional arguments: Stack (right-to-left)
- Return value: RAX (integer), XMM0 (floating-point)
- Caller-saved: RAX, RCX, RDX, R8-R11, XMM0-XMM5
- Callee-saved: RBX, RBP, RDI, RSI, R12-R15, RSP, XMM6-XMM15

#### COIL Standard ABI (0x08)

- Architecture-neutral ABI
- Arguments: Virtual registers V0-V7, then stack
- Return value: V0
- Caller-saved: V0-V7
- Callee-saved: V8-V15
- Stack grows downward

### Stack Frame Layout

Each ABI defines a specific stack frame structure.

Example for System V AMD64 ABI:
```
Higher Address │ Previous Frame  │
               ├────────────────┤
               │ Return Address │
               ├────────────────┤
               │ Saved RBP      │
               ├────────────────┤
               │ Callee-Saved   │
               │ Registers      │
               ├────────────────┤
               │ Local Variables│
               ├────────────────┤
Lower Address  │ Stack Arguments│
```

## Data Type Representations

### Integer Types

| Type  | Size (bytes) | Alignment (bytes) | Representation |
|-------|--------------|-------------------|----------------|
| INT8  | 1            | 1                 | Two's complement, little-endian |
| INT16 | 2            | 2                 | Two's complement, little-endian |
| INT32 | 4            | 4                 | Two's complement, little-endian |
| INT64 | 8            | 8                 | Two's complement, little-endian |

### Floating-Point Types

| Type  | Size (bytes) | Alignment (bytes) | Representation |
|-------|--------------|-------------------|----------------|
| FP32  | 4            | 4                 | IEEE-754 single precision |
| FP64  | 8            | 8                 | IEEE-754 double precision |

### Composite Types

Structure and union layout rules:

1. **System V AMD64 ABI**:
   - Natural alignment of members
   - No tail padding optimization
   - Bit-fields packed according to specific rules

2. **Windows x64 ABI**:
   - Natural alignment of members
   - Structures with 1/2/4/8 byte size are aligned to that size
   - Other structures are 8-byte aligned

## Name Mangling

### Mangling Schemes

| Scheme ID | Name | Description |
|-----------|------|-------------|
| 0x01 | None | No name mangling (C compatible) |
| 0x02 | Itanium | Itanium C++ ABI mangling |
| 0x03 | MSVC | Microsoft Visual C++ mangling |
| 0x04 | COIL | COIL-specific mangling scheme |

### COIL Mangling Scheme (0x04)

Format: `_COIL_<name>_<parameters>_<attributes>`

Example: `_COIL_add_i32_i32_i32_StdCC`

## Exception Handling

### Exception Models

| Model ID | Name | Description |
|----------|------|-------------|
| 0x01 | Zero-cost | Itanium/GCC/LLVM zero-cost exception model |
| 0x02 | SEH | Windows Structured Exception Handling |
| 0x04 | COIL | COIL native exception model |

### COIL Exception Model (0x04)

Stack unwinding mechanism:
1. Exception tables embedded in object files
2. Try/catch regions with type information
3. Automatic stack cleanup during unwinding
4. Support for finally blocks

## ABI Instruction Set

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xF0   | ABICALL  | Call with specific ABI | 2+ |
| 0xF1   | ABIRET   | Return with specific ABI | 0-1 |
| 0xF2   | ABISEL   | Select active ABI | 1 |
| 0xF5   | EHTHROW  | Throw exception | 1-2 |
| 0xF6   | EHRETHROW| Rethrow current exception | 0 |

## Detailed Instruction Behaviors

### ABICALL (0xF0)

```
ABICALL abi_id, target, arg1, arg2, ...
```

- `abi_id`: ABI identifier or reference
- `target`: Function to call
- `arg1, arg2, ...`: Arguments to pass

Behavior:
1. Sets up stack frame according to the specified ABI
2. Places arguments in registers/stack according to ABI rules
3. Calls the target function
4. Handles return value according to ABI rules

### ABIRET (0xF1)

```
ABIRET [return_value]
```

- `return_value`: Optional value to return

Behavior:
1. Places return value according to ABI rules
2. Restores callee-saved registers
3. Cleans up stack frame
4. Returns control to caller

### EHTHROW (0xF5)

```
EHTHROW exception_object [, additional_info]
```

- `exception_object`: Exception to throw
- `additional_info`: Optional additional information

Behavior:
1. Captures current execution context
2. Searches for exception handler
3. Unwinds stack to handler
4. Transfers control to handler with exception information

## Foreign Function Interface

The ABI extension defines mechanisms for calling non-COIL functions:

### External Function Declaration

```
EXTFUNC function_name, abi_id, return_type, param_type1, param_type2, ...
```

### Dynamic Library Loading

```
DLOPEN library_path, handle_dest
DLSYM handle, symbol_name, function_dest
DLCLOSE handle
```

## Usage Examples

### Calling C Functions

```
; Call C function with SystemV ABI
EXTFUNC printf, SystemV_x64, i32, ptr, varargs

; Format string
.section data
format_str:
    .string "Value: %d\n"

; Function call
ABICALL SystemV_x64, printf, format_str, 42
```

### Exception Handling

```
; Try-catch block
EHTRY try_block
    ; Code that might throw
    ; ...
    
    ; Explicitly throw an exception
    EHTHROW exception_type, exception_data
    
EHCAT exception_type, catch_block
    ; Handle exception
    ; ...
    
EHFIN finally_block
    ; Cleanup code always executed
    ; ...
    
EHEND
```