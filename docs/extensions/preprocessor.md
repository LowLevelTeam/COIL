# Preprocessor Extension

## Overview

The Preprocessor Extension enhances COIL with compile-time code manipulation capabilities, including conditional compilation, symbol definition, and inclusion of external sources. This enables code generation patterns, platform-specific adaptations, and modular code organization.

## Extension Information

- **Extension ID**: 0x04
- **Extension Name**: Preproc
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Preprocessor Directives

Preprocessor directives are processed at compile time by the COIL processor and do not generate runtime instructions. All directives begin with `!`.

### Basic Directives

| Directive | Description | Format |
|-----------|-------------|--------|
| !define   | Define a symbol or macro | !define SYMBOL [value] |
| !undef    | Undefine a symbol | !undef SYMBOL |
| !ifdef    | Conditional compilation if symbol defined | !ifdef SYMBOL |
| !ifndef   | Conditional compilation if symbol not defined | !ifndef SYMBOL |
| !if       | Conditional compilation based on expression | !if expression |
| !elif     | Else if condition | !elif expression |
| !else     | Alternative conditional branch | !else |
| !endif    | End conditional compilation | !endif |
| !include  | Include file contents | !include "file" |
| !error    | Generate compilation error | !error "message" |
| !warning  | Generate compilation warning | !warning "message" |

### Advanced Directives

| Directive | Description | Format |
|-----------|-------------|--------|
| !target   | Set target processing unit | !target PU_TYPE |
| !mode     | Set architecture mode | !mode MODE_TYPE |
| !arch     | Set target architecture | !arch ARCH_TYPE |
| !macro    | Define parameterized macro | !macro NAME(params) body !endmacro |
| !eval     | Evaluate compile-time expression | !eval expression |
| !stringize| Convert to string | !stringize(expression) |
| !concat   | Concatenate tokens | !concat(a, b) |

## Instruction Set Extensions

The Preprocessor Extension uses opcodes in the 0xF0-0xF9 range:

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xF0   | PPCOND   | Conditional compilation | 2 |
| 0xF1   | PPDEF    | Define symbol | 2-3 |
| 0xF2   | PPUNDEF  | Undefine symbol | 1 |
| 0xF3   | PPEVAL   | Evaluate preprocessor expression | 2 |
| 0xF4   | PPIF     | Begin conditional block | 1 |
| 0xF5   | PPELSE   | Alternative conditional branch | 0 |
| 0xF6   | PPENDIF  | End conditional block | 0 |
| 0xF7   | PPINC    | Include file | 1 |
| 0xF9   | PPTARG   | Set target platform | 1-3 |

## Detailed Directive Behaviors

### !define

The define directive creates a preprocessor symbol or macro:

```
!define SYMBOL [value]
```

Examples:
```
!define DEBUG
!define VERSION 1.0
!define MAX_SIZE 1024
!define SQUARED(x) ((x) * (x))
```

### !if, !elif, !else, !endif

The conditional directives control compilation based on conditions:

```
!if expression
  ; Code compiled if expression is true
!elif another_expression
  ; Code compiled if first expression is false and this one is true
!else
  ; Code compiled if all previous conditions are false
!endif
```

The expression can include:
- Defined symbols
- Numeric values
- Comparison operators (==, !=, <, >, <=, >=)
- Logical operators (&&, ||, !)
- Parentheses for grouping

Examples:
```
!if defined(DEBUG) && VERSION > 1.0
  ; Debug code for version > 1.0
!elif defined(TEST)
  ; Test code
!else
  ; Production code
!endif
```

### !target, !arch, !mode

The targeting directives specify platform characteristics:

```
!target PU_TYPE
!arch ARCH_TYPE
!mode MODE_TYPE
```

Examples:
```
!target PU_CPU
!arch X86_64
!mode LONG_64

; CPU-specific code here

!target PU_GPU
!arch NVIDIA
!mode COMPUTE

; GPU-specific code here
```

## Predefined Symbols

The preprocessor provides several predefined symbols:

| Symbol | Description | Example Value |
|--------|-------------|---------------|
| \_\_COIL\_VERSION\_\_ | COIL version number | 10000 (for v1.0.0) |
| \_\_FILE\_\_ | Current source file | "main.casm" |
| \_\_LINE\_\_ | Current source line | 42 |
| \_\_DATE\_\_ | Compilation date | "Mar 15 2024" |
| \_\_TIME\_\_ | Compilation time | "14:30:45" |
| \_\_PU\_\_ | Current processing unit | "CPU" |
| \_\_ARCH\_\_ | Current architecture | "x86_64" |
| \_\_MODE\_\_ | Current architecture mode | "LONG_64" |
| \_\_ENDIAN\_\_ | Current endianness | "LITTLE" |

## Macro System

### Macro Definition

```
!macro NAME(param1, param2, ...)
  ; Macro body with parameters
!endmacro
```

### Macro Invocation

```
NAME(arg1, arg2, ...)
```

### Stringization and Concatenation

```
!define GREETING(name) !stringize(Hello, name)
!define CONCAT_TOKENS(a, b) !concat(a, b)

GREETING(World)  // Expands to "Hello, World"
CONCAT_TOKENS(high, low)  // Expands to highlow
```

## Processing Phases

The preprocessor executes in multiple phases:

1. **Lexical Analysis**:
   - Tokenize source code
   - Identify preprocessor directives

2. **Macro Expansion**:
   - Process !define directives
   - Expand macros in source code

3. **Conditional Compilation**:
   - Evaluate !if, !ifdef, etc.
   - Include or exclude code blocks

4. **File Inclusion**:
   - Process !include directives
   - Recursively preprocess included files

5. **Platform Targeting**:
   - Process !target, !arch, !mode directives
   - Configure compilation environment

6. **Final Output**:
   - Generate preprocessed code for compiler

## Usage Examples

### Conditional Compilation

```
!define DEBUG

!ifdef DEBUG
    ; Include debugging code
    LOG "Debug mode enabled"
    TRAP_ENABLE
!else
    ; Regular code path
    OPTIM_ENABLE
!endif
```

### Platform-Specific Code

```
!if defined(__PU__) && __PU__ == "CPU"
    ; CPU-specific implementation
    MOV r0, [mem_addr]
    ADD r0, r1
!elif defined(__PU__) && __PU__ == "GPU"
    ; GPU-specific implementation
    GPUMOV r0, [mem_addr]
    GPUADD r0, r1
!else
    !error "Unsupported processing unit"
!endif
```

### Macro Usage

```
!macro SWAP(a, b)
    PUSH a
    MOV a, b
    POP b
!endmacro

; Usage
SWAP(r0, r1)    ; Expands to the three instructions
```

### Configuration

```
!define CPU_THREADS 4
!define VECTOR_WIDTH 128
!define USE_SIMD

!if VECTOR_WIDTH >= 256 && defined(USE_SIMD)
    !define VECTOR_IMPL "AVX"
!elif VECTOR_WIDTH >= 128 && defined(USE_SIMD)
    !define VECTOR_IMPL "SSE"
!else
    !define VECTOR_IMPL "SCALAR"
!endif
```