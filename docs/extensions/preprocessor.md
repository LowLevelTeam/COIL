# Preprocessor Extension

## Overview

The Preprocessor Extension enhances COIL with compile-time code manipulation capabilities, including conditional compilation, symbol definition, and inclusion of external sources. This enables code generation patterns, platform-specific adaptations, and modular code organization.

## Extension Information

- **Extension ID**: 0x04
- **Extension Name**: Preproc
- **Version**: 1.0.0
- **Dependencies**: None (Core COIL only)

## Preprocessor Instructions

The preprocessor extension provides instructions for compile-time code manipulation that are encoded in the COIL binary format. These are part of the core COIL specification, not just textual CASM directives.

## Instruction Set 

The Preprocessor Extension uses opcodes in the 0xF0-0xF9 range:

| Opcode | Mnemonic | Description | Operands | CASM Syntax |
|--------|----------|-------------|----------|-------------|
| 0xF0   | PPCOND   | Conditional compilation | 2 | !if, !ifdef, !ifndef |
| 0xF1   | PPDEF    | Define symbol | 2-3 | !define |
| 0xF2   | PPUNDEF  | Undefine symbol | 1 | !undef |
| 0xF3   | PPEVAL   | Evaluate preprocessor expression | 2 | !eval |
| 0xF4   | PPIF     | Begin conditional block | 1 | !if |
| 0xF5   | PPELSE   | Alternative conditional branch | 0 | !else |
| 0xF6   | PPENDIF  | End conditional block | 0 | !endif |
| 0xF7   | PPINC    | Include file | 1 | !include |
| 0xF8   | PPMACRO  | Define macro | 2+ | !macro |
| 0xF9   | PPTARG   | Set target platform | 1-3 | !target, !arch, !mode |

## CASM Directive Syntax

For human readability, CASM provides directive syntax (beginning with `!`) that maps directly to the COIL binary instructions:

| CASM Directive | Description | Format |
|----------------|-------------|--------|
| !define        | Maps to PPDEF | !define SYMBOL [value] |
| !undef         | Maps to PPUNDEF | !undef SYMBOL |
| !ifdef         | Maps to PPCOND | !ifdef SYMBOL |
| !ifndef        | Maps to PPCOND | !ifndef SYMBOL |
| !if            | Maps to PPIF | !if expression |
| !elif          | Maps to PPELSE+PPIF | !elif expression |
| !else          | Maps to PPELSE | !else |
| !endif         | Maps to PPENDIF | !endif |
| !include       | Maps to PPINC | !include "file" |
| !target        | Maps to PPTARG | !target PU_TYPE |
| !mode          | Maps to PPTARG | !mode MODE_TYPE |
| !arch          | Maps to PPTARG | !arch ARCH_TYPE |
| !macro         | Maps to PPMACRO | !macro NAME(params) body !endmacro |

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

## Detailed Instruction Behaviors

### PPDEF (0xF1)

The PPDEF instruction defines a preprocessor symbol or value:

```
PPDEF symbol_name, [value]
```

Binary encoding:
```
[0xF1][0x02/0x03][symbol_name_operand][optional_value_operand]
```

Where:
- `symbol_name_operand`: Symbol identifier (usually STRING type)
- `optional_value_operand`: Optional value to associate with the symbol

CASM Equivalent:
```
!define SYMBOL [value]
```

Examples:
```
; CASM:
!define DEBUG
!define VERSION 1.0
!define MAX_SIZE 1024

; Binary COIL (conceptual):
[0xF1][0x02][STRING:"DEBUG"]
[0xF1][0x03][STRING:"VERSION"][FP32:1.0]
[0xF1][0x03][STRING:"MAX_SIZE"][INT32:1024]
```

### PPIF, PPELSE, PPENDIF (0xF4, 0xF5, 0xF6)

These instructions control conditional compilation:

```
PPIF condition
  ; Conditionally compiled code
PPELSE
  ; Alternative code
PPENDIF
```

Binary encoding:
```
[0xF4][0x01][condition_operand]
  ; Code block
[0xF5][0x00]
  ; Alternative code block
[0xF6][0x00]
```

Where:
- `condition_operand`: Expression to evaluate (often using PPEVAL)

CASM Equivalent:
```
!if expression
  ; Code compiled if expression is true
!else
  ; Code compiled if all previous conditions are false
!endif
```

### PPTARG (0xF9)

The PPTARG instruction sets target platform characteristics:

```
PPTARG target_type, target_value
```

Binary encoding:
```
[0xF9][0x02][target_type_operand][target_value_operand]
```

Where:
- `target_type_operand`: Type of target (PU, ARCH, MODE)
- `target_value_operand`: Value to set

CASM Equivalent:
```
!target PU_TYPE
!arch ARCH_TYPE
!mode MODE_TYPE
```

Examples:
```
; CASM:
!target PU_CPU
!arch X86_64
!mode LONG_64

; Binary COIL (conceptual):
[0xF9][0x02][INT8:0x01][INT8:0x01]  ; PU_TYPE = CPU
[0xF9][0x02][INT8:0x02][INT8:0x01]  ; ARCH_TYPE = X86_64
[0xF9][0x02][INT8:0x03][INT8:0x03]  ; MODE_TYPE = LONG_64
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

## Advanced Preprocessor Operations

### PPMACRO (0xF8)

The PPMACRO instruction defines a parameterized macro:

```
PPMACRO name, param_count, param1, param2, ..., body
```

Binary encoding:
```
[0xF8][0x03+param_count][name_operand][param_count_operand][param1_operand][param2_operand]...[body_operand]
```

Where:
- `name_operand`: Macro name (STRING type)
- `param_count_operand`: Number of parameters (INT type)
- `paramX_operand`: Parameter names (STRING type)
- `body_operand`: Macro body (STRING or code block)

CASM Equivalent:
```
!macro NAME(param1, param2, ...)
  ; Macro body with parameters
!endmacro
```

### PPEVAL (0xF3)

The PPEVAL instruction evaluates a preprocessor expression:

```
PPEVAL dest, expression
```

Binary encoding:
```
[0xF3][0x02][dest_operand][expression_operand]
```

Where:
- `dest_operand`: Destination for result
- `expression_operand`: Expression to evaluate

CASM provides specialized syntactic sugar for common operations:
```
!stringize(expression)  // Converts to string
!concat(a, b)           // Concatenates tokens
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

## Binary Encoding Examples

### Conditional Compilation

```
; Define DEBUG flag
[0xF1][0x02][STRING:"DEBUG"]

; Check if DEBUG is defined
[0xF0][0x02][CONDITION_TYPE:DEFINED][STRING:"DEBUG"]

; If true branch (PPIF)
[0xF4][0x01][CONDITION_RESULT:TRUE]
  [0x20][0x02][STRING_TYPE][STRING:"Debug mode enabled"][VARIABLE_ID:log_dest]  ; LOG instruction
  [0x94][0x01][INT8:0x01]  ; TRAP_ENABLE instruction
  
; Else branch (PPELSE)
[0xF5][0x00]
  [0x94][0x01][INT8:0x02]  ; OPTIM_ENABLE instruction
  
; End conditional (PPENDIF)
[0xF6][0x00]
```

### Processor Unit Targeting

```
; Check processing unit type
[0xF3][0x03][RESULT_ID][VARIABLE:"__PU__"][STRING:"CPU"]
[0xF4][0x01][RESULT_ID]  ; PPIF with previous comparison result
  
  ; CPU-specific code
  [0x20][0x02][VARIABLE_ID:r0][MEMORY_REF:mem_addr]  ; MOV r0, [mem_addr]
  [0x40][0x02][VARIABLE_ID:r0][VARIABLE_ID:r1]       ; ADD r0, r1
  
[0xF5][0x00]  ; PPELSE
  
  ; Non-CPU fallback
  [0x20][0x02][VARIABLE_ID:r0][VARIABLE_ID:r1]       ; MOV r0, r1
  
[0xF6][0x00]  ; PPENDIF
```

### Macro Definition and Expansion

```
; Define SWAP macro
[0xF8][0x05][STRING:"SWAP"][INT8:0x02][STRING:"a"][STRING:"b"][CODE_BLOCK:{
  [0x21][0x01][PARAM_REF:"a"]  ; PUSH a
  [0x20][0x02][PARAM_REF:"a"][PARAM_REF:"b"]  ; MOV a, b
  [0x22][0x01][PARAM_REF:"b"]  ; POP b
}]

; Macro expansion (processed at compile time)
; SWAP(r0, r1) expands to:
[0x21][0x01][VARIABLE_ID:r0]  ; PUSH r0
[0x20][0x02][VARIABLE_ID:r0][VARIABLE_ID:r1]  ; MOV r0, r1
[0x22][0x01][VARIABLE_ID:r1]  ; POP r1
```

## Integration with Compilers

Compiler implementations can directly generate preprocessor instructions in the COIL binary format, bypassing CASM entirely. This enables:

1. **Direct Compilation**: Generate COIL binaries in a single compilation step
2. **Conditional Code Generation**: Use preprocessor instructions for platform-specific code paths
3. **Macro Expansion**: Implement textual macros directly in the binary format
4. **Platform Targeting**: Set processing unit, architecture, and mode attributes