# Orion++ Library API Reference

This document provides the complete programming interface for the Orion++ library (liborionpp), enabling developers to read, write, and manipulate Orion++ intermediate representation files.

## Table of Contents

1. [Library Overview](#library-overview)
2. [Core Types](#core-types)
3. [Reading API](#reading-api)
4. [Writing API](#writing-api)
5. [Utility Functions](#utility-functions)
6. [Error Handling](#error-handling)
7. [Examples](#examples)
8. [Language Bindings](#language-bindings)

## Library Overview

### Library Structure

```
liborionpp/
├── include/
│   ├── orionpp/reader.h          // Reading interface
│   ├── orionpp/writer.h          // Writing interface
│   ├── orionpp/types.h           // Core types and constants
│   ├── orionpp/util.h            // Utility functions
│   └── orionpp/orionpp.h         // Main header (includes all)
├── src/
│   ├── reader.c                  // Reader implementation
│   ├── writer.c                  // Writer implementation
│   ├── parser.c                  // Human format parser
│   ├── binary.c                  // Binary format handling
│   └── util.c                    // Utilities
└── examples/
    ├── read_file.c               // Basic file reading
    ├── write_file.c              // Basic file writing
    └── convert.c                 // Format conversion
```

### Library Features

- **Zero-Copy Reading**: Memory-mapped file access for performance
- **Incremental Writing**: Build files progressively without full buffering
- **Format Conversion**: Convert between human-readable and binary formats
- **Validation**: Comprehensive file format validation
- **Thread Safety**: Read operations are thread-safe, write operations require synchronization
- **Memory Efficient**: Minimal memory overhead for large files

### Installation

```bash
# Build from source
git clone https://github.com/orionlang/liborionpp.git
cd liborionpp
mkdir build && cd build
cmake ..
make install

# Using package manager
apt-get install liborionpp-dev      # Ubuntu/Debian
brew install liborionpp             # macOS
vcpkg install liborionpp            # Windows
```

## Core Types

### Basic Types

```c
#include <orionpp/types.h>

// Forward declarations
typedef struct OrionContext OrionContext;
typedef struct OrionFunction OrionFunction;
typedef struct OrionSymbol OrionSymbol;
typedef struct OrionVariable OrionVariable;
typedef struct OrionInstruction OrionInstruction;

// Writer types
typedef struct OrionWriter OrionWriter;
typedef struct OrionFunctionBuilder OrionFunctionBuilder;

// Result codes
typedef enum {
  ORION_OK = 0,
  ORION_ERROR_INVALID_FILE,
  ORION_ERROR_UNSUPPORTED_VERSION,
  ORION_ERROR_CORRUPTED_DATA,
  ORION_ERROR_OUT_OF_MEMORY,
  ORION_ERROR_INVALID_ARGUMENT,
  ORION_ERROR_FILE_NOT_FOUND,
  ORION_ERROR_PERMISSION_DENIED,
  ORION_ERROR_INVALID_OPERATION
} OrionResult;

// Data types
typedef enum {
  ORION_TYPE_VOID = 0,
  ORION_TYPE_I32 = 1
} OrionDataType;

// ABI types
typedef enum {
  ORION_ABI_C = 1,
  ORION_ABI_CUSTOM = 2
} OrionABIType;

// Symbol visibility
typedef enum {
  ORION_VISIBILITY_LOCAL = 0,
  ORION_VISIBILITY_GLOBAL = 1,
  ORION_VISIBILITY_WEAK = 2
} OrionVisibility;
```

### Instruction Opcodes

```c
// Core instructions
#define ORION_OP_NOP                0x00
#define ORION_OP_ENTER              0x01
#define ORION_OP_LEAVE              0x02
#define ORION_OP_RET                0x03

// Variable operations
#define ORION_OP_LET                0x10
#define ORION_OP_CONST              0x11
#define ORION_OP_MOV                0x12

// Arithmetic operations
#define ORION_OP_ADD                0x20
#define ORION_OP_SUB                0x21
#define ORION_OP_MUL                0x22
#define ORION_OP_DIV                0x23
#define ORION_OP_MOD                0x24
#define ORION_OP_NEG                0x25

// Control flow
#define ORION_OP_JMP                0x40
#define ORION_OP_CALL               0x41
#define ORION_OP_BEQ                0x42
#define ORION_OP_BNE                0x43
#define ORION_OP_BLT                0x44
#define ORION_OP_BLE                0x45
#define ORION_OP_BGT                0x46
#define ORION_OP_BGE                0x47
#define ORION_OP_BEQZ               0x48
#define ORION_OP_BNEZ               0x49

// ABI operations
#define ORION_OP_ABI_CALLEE_ENTER   0x50
#define ORION_OP_ABI_CALLEE_LEAVE   0x51
#define ORION_OP_ABI_CALLEE_GETARG  0x52
#define ORION_OP_ABI_CALLEE_SETRET  0x53
#define ORION_OP_ABI_CALLER_SETUP   0x54
#define ORION_OP_ABI_CALLER_CLEANUP 0x55
#define ORION_OP_ABI_CALLER_SETARG  0x56
#define ORION_OP_ABI_CALLER_GETRET  0x57
```

## Reading API

### Context Management

```c
#include <orionpp/reader.h>

// Load file and create context
OrionContext* orion_load_file(const char* filename);
OrionContext* orion_load_binary(const void* data, size_t size);
OrionContext* orion_load_human(const char* filename);

// Free context and associated resources
void orion_free_context(OrionContext* ctx);

// Get file information
uint32_t orion_get_version(const OrionContext* ctx);
uint32_t orion_get_entry_point(const OrionContext* ctx);
size_t orion_get_file_size(const OrionContext* ctx);
```

### Symbol Access

```c
// Get symbol count and access symbols
size_t orion_get_symbol_count(const OrionContext* ctx);
OrionSymbol* orion_get_symbol(const OrionContext* ctx, size_t index);
OrionSymbol* orion_find_symbol(const OrionContext* ctx, const char* name);

// Symbol properties
const char* orion_symbol_get_name(const OrionSymbol* symbol);
OrionVisibility orion_symbol_get_visibility(const OrionSymbol* symbol);
uint32_t orion_symbol_get_value(const OrionSymbol* symbol);
uint32_t orion_symbol_get_size(const OrionSymbol* symbol);
bool orion_symbol_is_function(const OrionSymbol* symbol);
bool orion_symbol_is_object(const OrionSymbol* symbol);
```

### Function Access

```c
// Get function count and access functions
size_t orion_get_function_count(const OrionContext* ctx);
OrionFunction* orion_get_function(const OrionContext* ctx, size_t index);
OrionFunction* orion_find_function(const OrionContext* ctx, const char* name);

// Function properties
const char* orion_function_get_name(const OrionFunction* func);
OrionABIType orion_function_get_abi(const OrionFunction* func);
OrionDataType orion_function_get_return_type(const OrionFunction* func);
uint16_t orion_function_get_param_count(const OrionFunction* func);
uint16_t orion_function_get_local_count(const OrionFunction* func);
uint32_t orion_function_get_first_var_id(const OrionFunction* func);
uint32_t orion_function_get_last_var_id(const OrionFunction* func);

// Function flags
bool orion_function_is_inline(const OrionFunction* func);
bool orion_function_is_pure(const OrionFunction* func);
bool orion_function_is_leaf(const OrionFunction* func);
```

### Instruction Access

```c
// Get instruction count and iterator
size_t orion_function_get_instruction_count(const OrionFunction* func);
OrionInstruction* orion_function_get_instructions(const OrionFunction* func);

// Instruction iterator
typedef struct OrionInstructionIterator OrionInstructionIterator;

OrionInstructionIterator* orion_instruction_iterator_create(const OrionFunction* func);
void orion_instruction_iterator_free(OrionInstructionIterator* iter);
bool orion_instruction_iterator_next(OrionInstructionIterator* iter);
OrionInstruction* orion_instruction_iterator_get(const OrionInstructionIterator* iter);

// Instruction properties
uint8_t orion_instruction_get_opcode(const OrionInstruction* inst);
uint8_t orion_instruction_get_flags(const OrionInstruction* inst);
size_t orion_instruction_get_operand_count(const OrionInstruction* inst);

// Operand access
typedef enum {
  ORION_OPERAND_IMMEDIATE,
  ORION_OPERAND_VARIABLE,
  ORION_OPERAND_LABEL,
  ORION_OPERAND_SYMBOL
} OrionOperandType;

typedef struct {
  OrionOperandType type;
  union {
    int32_t immediate;
    uint32_t variable_id;
    uint32_t label_offset;
    const char* symbol_name;
  };
} OrionOperand;

OrionOperand orion_instruction_get_operand(const OrionInstruction* inst, size_t index);
```

### Variable Access

```c
// Get variable information
size_t orion_get_variable_count(const OrionContext* ctx);
OrionVariable* orion_get_variable(const OrionContext* ctx, uint32_t var_id);

// Variable properties
uint32_t orion_variable_get_id(const OrionVariable* var);
OrionDataType orion_variable_get_type(const OrionVariable* var);
const char* orion_variable_get_name(const OrionVariable* var);
uint32_t orion_variable_get_scope_id(const OrionVariable* var);
bool orion_variable_is_parameter(const OrionVariable* var);
bool orion_variable_is_local(const OrionVariable* var);
bool orion_variable_is_temporary(const OrionVariable* var);
```

## Writing API

### Writer Management

```c
#include <orionpp/writer.h>

// Create and destroy writer
OrionWriter* orion_writer_create(void);
void orion_writer_free(OrionWriter* writer);

// Set global properties
void orion_writer_set_entry_point(OrionWriter* writer, const char* function_name);
void orion_writer_set_next_var_id(OrionWriter* writer, uint32_t next_id);

// Write to files
OrionResult orion_write_binary(OrionWriter* writer, const char* filename);
OrionResult orion_write_human(OrionWriter* writer, const char* filename);
OrionResult orion_write_binary_buffer(OrionWriter* writer, void** buffer, size_t* size);
```

### Symbol Management

```c
// Add symbols
OrionResult orion_add_symbol(OrionWriter* writer, const char* name, 
                            OrionVisibility visibility);
OrionResult orion_add_function_symbol(OrionWriter* writer, const char* name,
                                     OrionVisibility visibility);
OrionResult orion_add_object_symbol(OrionWriter* writer, const char* name,
                                   OrionVisibility visibility, uint32_t size);
```

### Function Building

```c
// Create function builder
OrionFunctionBuilder* orion_add_function(OrionWriter* writer, const char* name);
void orion_function_builder_free(OrionFunctionBuilder* builder);

// Set function properties
void orion_set_function_abi(OrionFunctionBuilder* builder, OrionABIType abi);
void orion_set_return_type(OrionFunctionBuilder* builder, OrionDataType type);
void orion_add_parameter(OrionFunctionBuilder* builder, OrionDataType type);
void orion_set_function_flags(OrionFunctionBuilder* builder, uint16_t flags);

// Variable management
uint32_t orion_allocate_variable(OrionFunctionBuilder* builder);
uint32_t orion_allocate_parameter(OrionFunctionBuilder* builder, OrionDataType type);
uint32_t orion_allocate_local(OrionFunctionBuilder* builder, OrionDataType type);
void orion_set_variable_name(OrionFunctionBuilder* builder, uint32_t var_id, const char* name);
```

### Instruction Building

```c
// Core instructions
void orion_add_nop(OrionFunctionBuilder* builder);
void orion_add_enter(OrionFunctionBuilder* builder);
void orion_add_leave(OrionFunctionBuilder* builder);
void orion_add_ret(OrionFunctionBuilder* builder);

// Variable operations
void orion_add_const(OrionFunctionBuilder* builder, uint32_t dest, int32_t value);
void orion_add_let(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src);
void orion_add_mov(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src);

// Arithmetic operations
void orion_add_add(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src1, uint32_t src2);
void orion_add_sub(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src1, uint32_t src2);
void orion_add_mul(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src1, uint32_t src2);
void orion_add_div(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src1, uint32_t src2);
void orion_add_mod(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src1, uint32_t src2);
void orion_add_neg(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src);

// Control flow
uint32_t orion_add_label(OrionFunctionBuilder* builder, const char* name);
void orion_add_jmp(OrionFunctionBuilder* builder, uint32_t label);
void orion_add_call(OrionFunctionBuilder* builder, const char* function_name);
void orion_add_beq(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_bne(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_blt(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_ble(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_bgt(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_bge(OrionFunctionBuilder* builder, uint32_t src1, uint32_t src2, uint32_t label);
void orion_add_beqz(OrionFunctionBuilder* builder, uint32_t src, uint32_t label);
void orion_add_bnez(OrionFunctionBuilder* builder, uint32_t src, uint32_t label);

// ABI operations
void orion_add_callee_enter(OrionFunctionBuilder* builder);
void orion_add_callee_leave(OrionFunctionBuilder* builder);
void orion_add_callee_getarg(OrionFunctionBuilder* builder, uint32_t dest, uint32_t index);
void orion_add_callee_setret(OrionFunctionBuilder* builder, uint32_t index, uint32_t src);
void orion_add_caller_setup(OrionFunctionBuilder* builder);
void orion_add_caller_cleanup(OrionFunctionBuilder* builder);
void orion_add_caller_setarg(OrionFunctionBuilder* builder, uint32_t index, uint32_t src);
void orion_add_caller_getret(OrionFunctionBuilder* builder, uint32_t dest, uint32_t index);
```

## Utility Functions

### Format Conversion

```c
#include <orionpp/util.h>

// Convert between formats
OrionResult orion_convert_to_binary(const char* human_file, const char* binary_file);
OrionResult orion_convert_to_human(const char* binary_file, const char* human_file);

// Validation
OrionResult orion_validate_file(const char* filename);
OrionResult orion_validate_context(const OrionContext* ctx);
```

### Pretty Printing

```c
// Print file contents
void orion_print_summary(const OrionContext* ctx, FILE* output);
void orion_print_symbols(const OrionContext* ctx, FILE* output);
void orion_print_functions(const OrionContext* ctx, FILE* output);
void orion_print_function(const OrionFunction* func, FILE* output);
void orion_print_instructions(const OrionFunction* func, FILE* output);

// Print specific instruction
void orion_print_instruction(const OrionInstruction* inst, FILE* output);
const char* orion_opcode_to_string(uint8_t opcode);
```

### String Utilities

```c
// String management
char* orion_escape_string(const char* input);
char* orion_unescape_string(const char* input);
bool orion_is_valid_symbol_name(const char* name);
bool orion_is_valid_variable_id(uint32_t var_id);
```

## Error Handling

### Error Information

```c
// Get error information
const char* orion_result_to_string(OrionResult result);
const char* orion_get_last_error(void);
void orion_clear_error(void);

// Error callback
typedef void (*OrionErrorCallback)(OrionResult result, const char* message, void* user_data);
void orion_set_error_callback(OrionErrorCallback callback, void* user_data);
```

### Common Error Patterns

```c
// Pattern 1: Simple error checking
OrionContext* ctx = orion_load_file("program.orionpp");
if (!ctx) {
  fprintf(stderr, "Failed to load file: %s\n", orion_get_last_error());
  return 1;
}

// Pattern 2: Detailed error handling
OrionResult result = orion_validate_file("program.orionpp");
switch (result) {
  case ORION_OK:
    printf("File is valid\n");
    break;
  case ORION_ERROR_INVALID_FILE:
    fprintf(stderr, "Invalid file format\n");
    break;
  case ORION_ERROR_CORRUPTED_DATA:
    fprintf(stderr, "File appears to be corrupted\n");
    break;
  default:
    fprintf(stderr, "Validation failed: %s\n", orion_result_to_string(result));
    break;
}

// Pattern 3: Custom error callback
void my_error_handler(OrionResult result, const char* message, void* user_data) {
  MyLogger* logger = (MyLogger*)user_data;
  log_error(logger, "Orion++ Error %d: %s", result, message);
}

orion_set_error_callback(my_error_handler, my_logger);
```

## Examples

### Reading a File

```c
#include <orionpp/orionpp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file.orionpp>\n", argv[0]);
    return 1;
  }
  
  // Load the file
  OrionContext* ctx = orion_load_file(argv[1]);
  if (!ctx) {
    fprintf(stderr, "Failed to load %s: %s\n", argv[1], orion_get_last_error());
    return 1;
  }
  
  // Print summary information
  printf("Orion++ File: %s\n", argv[1]);
  printf("Version: %u\n", orion_get_version(ctx));
  printf("Functions: %zu\n", orion_get_function_count(ctx));
  printf("Symbols: %zu\n", orion_get_symbol_count(ctx));
  
  // Iterate through functions
  size_t func_count = orion_get_function_count(ctx);
  for (size_t i = 0; i < func_count; i++) {
    OrionFunction* func = orion_get_function(ctx, i);
    printf("\nFunction: %s\n", orion_function_get_name(func));
    printf("  ABI: %s\n", orion_function_get_abi(func) == ORION_ABI_C ? "C" : "Custom");
    printf("  Parameters: %u\n", orion_function_get_param_count(func));
    printf("  Instructions: %zu\n", orion_function_get_instruction_count(func));
    
    // Print instructions
    OrionInstructionIterator* iter = orion_instruction_iterator_create(func);
    while (orion_instruction_iterator_next(iter)) {
      OrionInstruction* inst = orion_instruction_iterator_get(iter);
      printf("    ");
      orion_print_instruction(inst, stdout);
      printf("\n");
    }
    orion_instruction_iterator_free(iter);
  }
  
  orion_free_context(ctx);
  return 0;
}
```

### Writing a File

```c
#include <orionpp/orionpp.h>
#include <stdio.h>

int main(void) {
  // Create writer
  OrionWriter* writer = orion_writer_create();
  if (!writer) {
    fprintf(stderr, "Failed to create writer\n");
    return 1;
  }
  
  // Create main function
  OrionFunctionBuilder* main_func = orion_add_function(writer, "main");
  orion_set_function_abi(main_func, ORION_ABI_C);
  orion_set_return_type(main_func, ORION_TYPE_I32);
  
  // Add function body
  orion_add_callee_enter(main_func);
  orion_add_enter(main_func);
  
  // Create variables: result = 5 + 3
  uint32_t var1 = orion_allocate_local(main_func, ORION_TYPE_I32);
  uint32_t var2 = orion_allocate_local(main_func, ORION_TYPE_I32);
  uint32_t result = orion_allocate_local(main_func, ORION_TYPE_I32);
  
  orion_set_variable_name(main_func, var1, "a");
  orion_set_variable_name(main_func, var2, "b");
  orion_set_variable_name(main_func, result, "result");
  
  // Add instructions
  orion_add_const(main_func, var1, 5);
  orion_add_const(main_func, var2, 3);
  orion_add_add(main_func, result, var1, var2);
  orion_add_callee_setret(main_func, 0, result);
  
  orion_add_leave(main_func);
  orion_add_callee_leave(main_func);
  orion_add_ret(main_func);
  
  // Set entry point
  orion_writer_set_entry_point(writer, "main");
  
  // Write files
  OrionResult result_code = orion_write_binary(writer, "output.orionpp");
  if (result_code != ORION_OK) {
    fprintf(stderr, "Failed to write binary: %s\n", orion_result_to_string(result_code));
    orion_writer_free(writer);
    return 1;
  }
  
  result_code = orion_write_human(writer, "output.hopp");
  if (result_code != ORION_OK) {
    fprintf(stderr, "Failed to write human format: %s\n", orion_result_to_string(result_code));
    orion_writer_free(writer);
    return 1;
  }
  
  printf("Successfully created output.orionpp and output.hopp\n");
  
  orion_writer_free(writer);
  return 0;
}
```

### Format Conversion

```c
#include <orionpp/orionpp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return 1;
  }
  
  const char* input = argv[1];
  const char* output = argv[2];
  
  // Determine conversion direction based on file extensions
  bool to_binary = (strstr(output, ".orionpp") != NULL);
  bool from_binary = (strstr(input, ".orionpp") != NULL);
  
  OrionResult result;
  if (from_binary && !to_binary) {
    // Binary to human
    result = orion_convert_to_human(input, output);
  } else if (!from_binary && to_binary) {
    // Human to binary
    result = orion_convert_to_binary(input, output);
  } else {
    fprintf(stderr, "Cannot determine conversion type from file extensions\n");
    return 1;
  }
  
  if (result != ORION_OK) {
    fprintf(stderr, "Conversion failed: %s\n", orion_result_to_string(result));
    return 1;
  }
  
  printf("Successfully converted %s to %s\n", input, output);
  return 0;
}
```
