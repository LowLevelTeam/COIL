# Preprocessor Extension

## Overview

The Preprocessor Extension enhances COIL with compile-time code manipulation capabilities, including conditional compilation, symbol definition, and inclusion of external sources. This extension enables code generation patterns, platform-specific adaptations, and modular code organization.

## Extension Information

- **Extension ID**: 0x04
- **Extension Name**: Preproc
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Preprocessor Directives

Preprocessor directives are processed at compile time by the COIL processor and do not generate runtime instructions. All directives begin with `!`.

### Basic Directives

| Directive | Description | Format | Extension |
|-----------|-------------|--------|-----------|
| !define   | Define a symbol or macro | !define SYMBOL [value] | Preproc |
| !undef    | Undefine a symbol | !undef SYMBOL | Preproc |
| !ifdef    | Conditional compilation if symbol defined | !ifdef SYMBOL | Preproc |
| !ifndef   | Conditional compilation if symbol not defined | !ifndef SYMBOL | Preproc |
| !if       | Conditional compilation based on expression | !if expression | Preproc |
| !elif     | Else if condition | !elif expression | Preproc |
| !else     | Alternative conditional branch | !else | Preproc |
| !endif    | End conditional compilation | !endif | Preproc |
| !include  | Include file contents | !include "file" | Preproc |
| !error    | Generate compilation error | !error "message" | Preproc |
| !warning  | Generate compilation warning | !warning "message" | Preproc |
| !pragma   | Implementation-specific directive | !pragma name [args] | Preproc |
| !line     | Set source line number | !line number ["file"] | Preproc |

### Advanced Directives

| Directive | Description | Format | Extension |
|-----------|-------------|--------|-----------|
| !target   | Set target processing unit | !target PU_TYPE | Preproc |
| !mode     | Set architecture mode | !mode MODE_TYPE | Preproc |
| !arch     | Set target architecture | !arch ARCH_TYPE | Preproc |
| !macro    | Define parameterized macro | !macro NAME(params) body !endmacro | Preproc |
| !eval     | Evaluate compile-time expression | !eval expression | Preproc |
| !stringize| Convert to string | !stringize(expression) | Preproc |
| !concat   | Concatenate tokens | !concat(a, b) | Preproc |
| !counter  | Compile-time counter | !counter | Preproc |
| !scope    | Begin preprocessor scope | !scope | Preproc |
| !endscope | End preprocessor scope | !endscope | Preproc |

## Instruction Set Extensions

The Preprocessor Extension uses opcodes in the 0xF0-0xF9 range:

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xF0   | PPCOND   | Conditional compilation | 2 | Preproc |
| 0xF1   | PPDEF    | Define symbol | 2-3 | Preproc |
| 0xF2   | PPUNDEF  | Undefine symbol | 1 | Preproc |
| 0xF3   | PPEVAL   | Evaluate preprocessor expression | 2 | Preproc |
| 0xF4   | PPIF     | Begin conditional block | 1 | Preproc |
| 0xF5   | PPELSE   | Alternative conditional branch | 0 | Preproc |
| 0xF6   | PPENDIF  | End conditional block | 0 | Preproc |
| 0xF7   | PPINC    | Include file | 1 | Preproc |
| 0xF8   | PPMACRO  | Define macro | 2+ | Preproc |
| 0xF9   | PPTARG   | Set target platform | 1-3 | Preproc |

## Detailed Directive Behaviors

### !define

The define directive creates a preprocessor symbol or macro:

```
!define SYMBOL [value]
```

- `SYMBOL`: Name of the symbol to define
- `value`: Optional value to associate with the symbol

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
| \_\_CASM\_VERSION\_\_ | CASM version if used | 10000 |
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

## Directive Binary Representation

Preprocessor directives are encoded as instructions only visible to the COIL processor:

### PPCOND (0xF0)

```
[0xF0][0x02][condition_type][condition_value]
```

- `condition_type`: Type of condition (defined, equals, etc.)
- `condition_value`: Value to compare against

### PPDEF (0xF1)

```
[0xF1][0x02/0x03][symbol_name][optional_value]
```

- `symbol_name`: Name of symbol to define
- `optional_value`: Value to associate (if present)

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

## Implementation Requirements

Implementations of the Preprocessor Extension must:

1. **Directives**:
   - Support all standard preprocessor directives
   - Implement nested conditional compilation
   - Handle macro expansion with parameters

2. **Expressions**:
   - Support full expression evaluation
   - Handle complex conditional logic
   - Provide compile-time constant folding

3. **Targeting**:
   - Implement processing unit targeting
   - Support architecture-specific compilation
   - Handle mode-specific features

4. **Error Handling**:
   - Report clear error messages for directive misuse
   - Validate macro parameters
   - Detect circular dependencies in file inclusion
