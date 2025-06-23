# Orion++ Development Library

A C library for reading, writing, and manipulating Orion++ files - the intermediate representation format for the Orion toolchain.

## Overview

The Orion++ Development Library (`liborionpp-dev`) provides a comprehensive API for working with Orion++ files in both binary (.orionpp) and human-readable (.hopp) formats. Orion++ is a device-agnostic intermediate representation designed for optimization and compatibility across different target architectures.

## Features

- ✅ **Dual Format Support**: Handle both binary (.orionpp) and human-readable (.hopp) formats
- ✅ **Integer Variable System**: Variables are referenced by numeric IDs for efficiency
- ✅ **Symbol System**: Symbols use @"name" syntax for maximum flexibility
- ✅ **Cross-Platform**: Works on Linux, macOS, and Windows
- ✅ **Memory Efficient**: Optimized for performance with proper resource management
- ✅ **Extensible**: Easy to add new instructions and features
- ✅ **Type Safe**: Strong typing for variables and symbols

## Quick Start

### Building the Library

```bash
# Build library and examples
make all

# Run the example
make test
```

### Basic Usage

```c
#include "orionpp.h"
#include "orionpp_writer.h"

// Create a human-readable Orion++ file
FILE* output = fopen("example.hopp", "w");
OrionPPWriter* writer = orionpp_writer_create(output, ORIONPP_FORMAT_HUMAN);

// Begin a function
orionpp_writer_begin_function(writer, "add", 2, ORIONPP_TYPE_I32);

// Create variables
uint32_t param_a = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "a", ORIONPP_TYPE_I32);
uint32_t param_b = orionpp_writer_create_variable(writer, ORIONPP_VAR_PARAM, "b", ORIONPP_TYPE_I32);
uint32_t result = orionpp_writer_create_variable(writer, ORIONPP_VAR_LOCAL, "result", ORIONPP_TYPE_I32);

// Emit instructions
orionpp_writer_emit_abi_callee_enter(writer);
orionpp_writer_emit_enter(writer);
orionpp_writer_emit_abi_callee_getarg(writer, param_a, 0);
orionpp_writer_emit_abi_callee_getarg(writer, param_b, 1);
orionpp_writer_emit_add(writer, result, param_a, param_b);
orionpp_writer_emit_abi_callee_setret(writer, result, 0);
orionpp_writer_emit_leave(writer);
orionpp_writer_emit_abi_callee_leave(writer);
orionpp_writer_emit_ret(writer);

orionpp_writer_end_function(writer);
orionpp_writer_finalize(writer);

orionpp_writer_destroy(writer);
fclose(output);
```

## New Syntax Overview

### Variables

Variables are now referenced by numeric IDs instead of string names:

```assembly
// Human format (.hopp)
isa.const $1, 10          // Create variable $1 with value 10
isa.let $2, $1            // Copy $1 to $2
isa.add $3, $1, $2        // Add $1 + $2, store in $3
```

Variable types:
- **$1-$255**: Parameters (passed to functions)
- **$256+**: Local and temporary variables
- Debug names are stored separately for human readability

### Symbols

Symbols use the @"name" syntax for maximum flexibility:

```assembly
// Function symbols
obj.sym[global,func] @"main":
obj.sym[local,func] @"helper_function":

// Data symbols  
obj.sym[global,object] @"global_data":
obj.sym[local,object] @"local_buffer":

// Call symbols
isa.call @"printf"
isa.call @"my_function"
```

Symbol names can contain any characters except quotes, allowing for:
- Special characters: @"operator+"
- Namespaces: @"std::vector::push_back"
- Unicode: @"函数名"

### Function Structure

```assembly
obj.sym[global,func] @"function_name":
  .hint-abi(c)
  .hint-args(i32, i32)
  .hint-rets(i32)
  
  abi.callee.enter
  isa.enter
    // Get parameters
    isa.let $1, abi.callee.getarg.0
    isa.let $2, abi.callee.getarg.1
    
    // Function body
    isa.add $3, $1, $2
    
    // Return value
    abi.callee.setret.0 $3
  isa.leave
  abi.callee.leave
  isa.RET
.hint-endfunc()
```

## API Reference

### Core Types

```c
typedef struct OrionPPContext OrionPPContext;
typedef struct OrionPPWriter OrionPPWriter;
typedef struct OrionPPReader OrionPPReader;

typedef enum {
  ORIONPP_FORMAT_BINARY,    // .orionpp files
  ORIONPP_FORMAT_HUMAN      // .hopp files
} OrionPPFormat;

typedef enum {
  ORIONPP_VAR_TEMP = 0,     // Temporary variables
  ORIONPP_VAR_LOCAL,        // Local variables
  ORIONPP_VAR_PARAM,        // Function parameters
  ORIONPP_VAR_GLOBAL        // Global variables
} OrionPPVariableType;
```

### Writer API

```c
// Writer lifecycle
OrionPPWriter* orionpp_writer_create(FILE* output, OrionPPFormat format);
void orionpp_writer_destroy(OrionPPWriter* writer);
OrionPPError orionpp_writer_finalize(OrionPPWriter* writer);

// Program structure
OrionPPError orionpp_writer_begin_program(OrionPPWriter* writer);
OrionPPError orionpp_writer_end_program(OrionPPWriter* writer);

// Function management
OrionPPError orionpp_writer_begin_function(OrionPPWriter* writer, const char* name,
                                          uint16_t param_count, uint8_t return_type);
OrionPPError orionpp_writer_end_function(OrionPPWriter* writer);

// Variable creation
uint32_t orionpp_writer_create_variable(OrionPPWriter* writer, OrionPPVariableType type,
                                       const char* debug_name, uint8_t data_type);
uint32_t orionpp_writer_create_temp(OrionPPWriter* writer, uint8_t data_type);

// Instruction emission
OrionPPError orionpp_writer_emit_const(OrionPPWriter* writer, uint32_t dest, int32_t value);
OrionPPError orionpp_writer_emit_add(OrionPPWriter* writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionPPError orionpp_writer_emit_call(OrionPPWriter* writer, const char* symbol);
// ... more instructions
```

### Reader API

```c
// Reader lifecycle
OrionPPReader* orionpp_reader_create(FILE* input);
void orionpp_reader_destroy(OrionPPReader* reader);
OrionPPError orionpp_reader_load(OrionPPReader* reader);

// Data access
const OrionPPHeader* orionpp_reader_get_header(OrionPPReader* reader);
const char* orionpp_reader_get_string(OrionPPReader* reader, uint32_t offset);
const OrionPPSymbol* orionpp_reader_find_symbol(OrionPPReader* reader, const char* name);
const OrionPPFunction* orionpp_reader_find_function(OrionPPReader* reader, const char* name);

// Instruction decoding
OrionPPError orionpp_reader_decode_instruction(OrionPPReader* reader, size_t offset,
                                              OrionPPInstruction* instruction, size_t* next_offset);

// Disassembly
OrionPPError orionpp_reader_disassemble_all(OrionPPReader* reader, FILE* output);
```

## File Formats

### Binary Format (.orionpp)

Binary format is optimized for size and parsing speed:

- Magic number: `0x4F50504F` ("OPPO")
- Version: `2`
- Contains string table, symbol table, function table, variable table, and instruction data
- Variable references use 32-bit IDs
- Symbols reference string table offsets

### Human Format (.hopp)

Human-readable format for debugging and manual editing:

- Text-based assembly syntax
- Variables shown as `$ID` or debug names
- Symbols shown as `@"name"`
- Comments and hints for readability

## Integration with Orion Toolchain

The library integrates with the complete Orion toolchain:

```
[.c] -> orioncc -> [.hopp/.orionpp] -> orion++ -> [.*.orion] -> orionc -> [native]
```

- **orioncc**: Compiles C to Orion++ format using this library
- **orion++**: Processes Orion++ to target-specific formats
- **orionc**: Compiles target-specific to native machine code

## Examples

The `examples/` directory contains:

- **example.c**: Comprehensive example showing library usage
- **syntax_examples/**: Various Orion++ syntax examples

## Error Handling

```c
typedef enum {
  ORIONPP_OK = 0,
  ORIONPP_ERR_INVALID_MAGIC,
  ORIONPP_ERR_INVALID_VERSION,
  ORIONPP_ERR_INVALID_FORMAT,
  ORIONPP_ERR_FILE_NOT_FOUND,
  ORIONPP_ERR_OUT_OF_MEMORY,
  ORIONPP_ERR_INVALID_VARIABLE,
  ORIONPP_ERR_INVALID_SYMBOL
} OrionPPError;

const char* orionpp_get_error_string(OrionPPError error);
```

## Building and Installation

### Prerequisites
- GCC or Clang with C11 support
- Make or compatible build system

### Build Options

```bash
make all        # Build library and examples
make library    # Build only the library
make examples   # Build only examples
make test       # Build and run examples
make clean      # Clean build artifacts
make install    # Install to system (requires permissions)
```

## Documentation

See the `docs/` directory for detailed documentation:

- **SYNTAX.md**: Complete Orion++ syntax reference
- **BINARY_FORMAT.md**: Binary file format specification
- **API.md**: Complete API documentation
- **EXAMPLES.md**: Usage examples and tutorials

## Contributing

1. Follow the existing code style (2-space indentation)
2. Add tests for new functionality
3. Update documentation for API changes
4. Ensure backward compatibility when possible

## License

This library is part of the Orion toolchain. See the main project for license information.