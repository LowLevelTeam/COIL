# COIL Instruction Set Architecture Overview

## 1. Introduction

The COIL Instruction Set Architecture (ISA) defines the complete set of operations that can be performed in COIL. The instruction set is organized into logical categories, with clear separation between universal operations and architecture-specific operations.

### 1.1 Key Concepts

#### Binary Format Primacy
COIL is fundamentally a binary instruction format. This binary format is the actual "language" of COIL, with COIL Assembly (COIL-ASM) being a textual representation for human readability.

#### Type-Determined Instructions
COIL follows a "type-determined instruction" philosophy where instructions derive their behavior from the types of their operands. This allows for a compact set of opcodes that can operate on many different data types.

#### Variable System
COIL provides a robust variable system that abstracts over registers and memory:
- Variables are declared with specific types
- Variables are automatically allocated to registers or memory
- Variables are managed in lexical scopes for automatic lifetime control
- The COIL processor optimizes variable storage based on usage patterns

#### ABI System
The Application Binary Interface (ABI) system eliminates the need for manual register management during function calls:
- Parameter passing is automated based on ABI definitions
- Return values are managed according to ABI rules
- Register preservation is handled by the processor

## 2. Instruction Format

COIL instructions follow a consistent binary format:

```
[Opcode (8 bits)] [Operand Count (8 bits)] [Operands...]
```

Each operand consists of:
```
[Type (16 bits)] [Type-Specific Data (variable)] [Value (variable)]
```

## 3. Opcode Organization

The 8-bit opcode space is organized into functional categories:

```
0x00      - No Operation
0x01-0x0F - Control Flow
0x10-0x2F - Memory Operations
0x30-0x4F - Reserved for Multi-Device Operations (v3)
0x50-0x5F - Bit Manipulation
0x60-0x8F - Arithmetic
0x90-0x9F - Vector/Array Operations
0xA0-0xAF - Type Instructions
0xB0-0xBF - Directive Instructions
0xC0-0xFE - Architecture/Processor-Specific Instructions
0xFF      - COIL Processor-Specific Extensions
```

## 4. Instruction Categories

### 4.1 Control Flow (0x01-0x0F)

Instructions for controlling program execution flow:
- Symbol definition
- Conditional and unconditional branching
- Function calls and returns
- Comparison operations for setting flags

[Detailed documentation](./isa/cf.md)

### 4.2 Memory Operations (0x10-0x2F)

Instructions for memory access and management:
- Data movement
- Stack operations
- Memory allocation and scope management
- Variable declarations
- Memory copying, comparison, and manipulation

The variable system is a key component of memory operations in COIL, providing an abstraction layer over registers and memory that enables architecture-independent code.

[Detailed documentation](./isa/memops.md)

### 4.3 Reserved Instructions (0x30-0x4F)

This range is reserved for multi-device operations in COIL v3.

[Placeholder documentation](./isa/resv.md)

### 4.4 Bit Manipulation (0x50-0x5F)

Instructions for binary operations:
- Logical operations (AND, OR, XOR, NOT)
- Shift and rotate operations
- Bit counting and manipulation

[Detailed documentation](./isa/bit.md)

### 4.5 Arithmetic (0x60-0x8F)

Instructions for mathematical operations:
- Basic arithmetic (add, subtract, multiply, divide)
- Advanced arithmetic (fused multiply-add, etc.)
- Comparison operations
- Conversion operations

[Detailed documentation](./isa/arith.md)

### 4.6 Vector/Array Operations (0x90-0x9F)

Instructions for operations on multiple values:
- Vector dot products
- Array operations
- SIMD-style operations

[Detailed documentation](./isa/vec.md)

### 4.7 Type Instructions (0xA0-0xAF)

Instructions for type manipulation:
- Type queries and information
- Type conversions
- Composite type operations
- Field and element access

[Detailed documentation](./isa/type.md)

### 4.8 Directive Instructions (0xB0-0xBF)

Instructions for controlling the compilation process:
- Architecture and processor selection
- Alignment and section control
- Conditional compilation
- ABI definitions
- Data insertion

[Detailed documentation](./isa/dir.md)

### 4.9 Special Instructions (0x00, 0xC0-0xFF)

Instructions specific to certain processors or architectures:
- No Operation (0x00)
- Processor-specific instructions (0xC0-0xFE)
- COIL processor extensions (0xFF)

[Detailed documentation](./isa/spec.md)

## 5. Variable System

The variable system is one of COIL's most powerful features:

### 5.1 Variable Declaration

Variables are declared using the `VAR` instruction:
```
VAR TYPE_INT32, counter, 0  ; Declare and initialize a variable
```

### 5.2 Variable Scoping

Variables exist within lexical scopes defined by `SCOPEE` and `SCOPEL`:
```
SCOPEE
    VAR TYPE_INT32, temp, 0
    ; temp is accessible here
SCOPEL  ; temp is automatically deallocated here
```

### 5.3 Variable Usage

Variables can be used directly in instructions without specifying registers:
```
ADD counter, counter, 1  ; Increment counter
```

The COIL processor automatically determines the best storage location (register or memory) for each variable based on usage patterns and available resources.

## 6. ABI System

The ABI system enables architecture-independent function calls:

### 6.1 Function Declaration with ABI

```
SYM add_function, TYPE_PARAM0=BRANCH_CTRL_ABI
    SCOPEE
    VAR TYPE_INT32, a
    VAR TYPE_INT32, b
    MOV a, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 0
    MOV b, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 1
    
    VAR TYPE_INT32, result
    ADD result, a, b
    
    RET TYPE_PARAM0=BRANCH_CTRL_ABI_RET, result
    SCOPEL
```

### 6.2 Function Call with ABI

```
VAR TYPE_INT32, x, 10
VAR TYPE_INT32, y, 20
CALL add_function, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, x, y
VAR TYPE_INT32, sum
MOV sum, TYPE_PARAM0=BRANCH_CTRL_ABI_RET
```

The ABI system automatically handles:
- Parameter passing (register selection or stack usage)
- Return value handling
- Register preservation
- Stack frame setup and teardown

## 7. Implementation Requirements

A COIL v1 processor must:
1. Implement all instructions in categories 0x00-0x2F and 0x50-0xBF
2. Implement appropriate architecture-specific instructions for supported architectures
3. Reject instructions from the reserved range (0x30-0x4F)
4. Handle conditional execution correctly
5. Implement the variable system with automatic allocation optimization
6. Support the ABI system for function calls and returns
7. Follow the operand type rules for each instruction

## 8. Best Practices

### 8.1 Variable Usage

1. Use variables instead of direct register references whenever possible
2. Declare variables at the beginning of their scope
3. Use appropriate scoping to manage variable lifetimes
4. Provide type information for better optimization
5. Consider variable promotion/demotion for performance-critical code

### 8.2 ABI Usage

1. Use the ABI system for all function calls
2. Define parameters and return values clearly
3. Use standard ABIs when interfacing with external code
4. Document custom ABIs thoroughly

### 8.3 Architecture Independence

1. Use conditional compilation for architecture-specific code
2. Prefer platform-independent types (TYPE_INT, TYPE_UNT)
3. Avoid direct register references unless absolutely necessary
4. Test code on multiple target architectures

## 9. Example: Portable Function

```
; Architecture-independent function
SYM calculate_sum, TYPE_PARAM0=BRANCH_CTRL_ABI
    SCOPEE
    ; Get parameters via ABI
    VAR TYPE_INT, a
    VAR TYPE_INT, b
    MOV a, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 0
    MOV b, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 1
    
    ; Calculate sum using variables
    VAR TYPE_INT, result
    ADD result, a, b
    
    ; Return via ABI
    RET TYPE_PARAM0=BRANCH_CTRL_ABI_RET, result
    SCOPEL
```

This function will work correctly on any architecture supported by COIL, as the variable system and ABI system handle the architecture-specific details automatically.

## 10. Comparison with Traditional Assembly

To illustrate COIL's advantages, here's a comparison with traditional assembly:

### Traditional x86-64 Assembly:
```
; Function that adds two numbers in x86-64 assembly
add_function:
    ; Parameters in RDI, RSI per System V ABI
    ; Need to know register conventions
    mov rax, rdi      ; First parameter to RAX
    add rax, rsi      ; Add second parameter
    ret               ; Return in RAX
```

### COIL Equivalent:
```
; Same function in COIL with variables and ABI
SYM add_function, TYPE_PARAM0=BRANCH_CTRL_ABI
    SCOPEE
    ; Get parameters through ABI
    VAR TYPE_INT64, a
    VAR TYPE_INT64, b
    MOV a, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 0
    MOV b, TYPE_PARAM0=BRANCH_CTRL_ABI_PARAM, 1
    
    ; Add values (no register knowledge needed)
    VAR TYPE_INT64, result
    ADD result, a, b
    
    ; Return through ABI
    RET TYPE_PARAM0=BRANCH_CTRL_ABI_RET, result
    SCOPEL
```

### Advantages of COIL:
- Architecture-independent code
- No need to know register conventions
- Variables with automatic memory management
- Explicit parameter passing through ABI
- Same code works on any supported architecture