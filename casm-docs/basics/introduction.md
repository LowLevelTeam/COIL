# Introduction to CASM

## What is CASM?

CASM (COIL Assembly) is the human-readable text representation of COIL (Computer Oriented Intermediate Language). It serves as an assembly language for writing programs that will be translated into COIL's binary format.

## Relationship to COIL

While COIL is fundamentally a binary instruction format, CASM provides a more developer-friendly way to interact with it:

- CASM is to COIL what assembly language is to machine code
- Every CASM instruction has a direct correlation to a COIL binary instruction
- CASM provides additional assembly-time conveniences and directives

## Key Features

### Readability

CASM is designed to be more readable than traditional assembly languages:

- Clear, descriptive instruction names
- Explicit type information
- Structured variable management
- Scope-based resource handling

### Type-Determined Instructions

Like COIL, CASM uses the type-determined instruction approach:

- The same instruction mnemonic (e.g., `ADD`) works on different data types
- No need for separate variants like ADDI, ADDF, ADDV
- Type information is explicit in the operands

### Architecture Independence

CASM code can be written to be architecture-independent:

- Universal operations work across all platforms
- Platform-specific code can be isolated using directives
- The variable system abstracts over registers
- The ABI system abstracts over calling conventions

## Basic Structure

CASM programs are text files with the extension `.casm`. They consist of:

- Directives for assembly-time control
- Symbolic labels for code locations
- Instructions for runtime operations
- Comments for documentation

Simple example:
```
; Simple CASM example
PROC 0x01                  ; CPU processor
ARCH 0x01, 0x03            ; x86-64 architecture

SECTION .text, 0x01 | 0x04 ; Executable and readable section
SYM main, TYPE_PARAM0=GLOB ; Global symbol 'main'
    SCOPEE
    VAR TYPE_INT32, counter, 0    ; Declare variable
    ADD counter, counter, 1       ; Increment counter
    SCOPEL
    RET
```

## CASM Workflow

The typical workflow with CASM:

1. Write program in CASM (`.casm` file)
2. Use COIL assembler to translate to COIL binary (`.coil` file)
3. Run the COIL binary using a COIL processor or compiler

```
source.casm → [COIL assembler] → program.coil → [COIL processor] → Execution
```

## Comparison to Traditional Assembly

Unlike traditional assembly languages, CASM:

- Uses a type-determined instruction approach
- Abstracts over registers with variables
- Manages variable lifetimes with scopes
- Automates calling conventions with ABIs
- Works consistently across different architectures

## Getting Started

To start writing CASM code:

1. Understand the basic syntax rules
2. Learn the core instructions and directives
3. Grasp the variable and scope system
4. Study the type system and ABI conventions
5. Practice with simple examples

For more details, see:
- [Syntax Fundamentals](syntax-fundamentals.md)
- [Variables](../features/variables.md)
- [Hello World Example](../examples/hello-world.md)