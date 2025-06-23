# Orion++ Documentation

## Overview

Orion++ is a device-agnostic intermediate representation (IR) designed for modern compiler toolchains. It provides a clean abstraction layer between high-level source languages and target architectures, enabling powerful optimizations and cross-platform compatibility.

## Key Features

- **Device Agnostic**: Single IR works across different target architectures
- **Optimization Friendly**: Multiple optimization passes can be applied before code generation
- **Dual Format Support**: Human-readable format for debugging, compact binary format for production
- **Efficient Design**: Numeric variable IDs and symbol references for performance
- **Cross-Platform**: Consistent behavior across operating systems and architectures

## Formats

### Human-Readable Format (.hopp)
- **Purpose**: Debugging, testing, compiler development
- **Syntax**: Assembly-like syntax with symbolic names
- **Use Cases**: Manual inspection, compiler debugging, educational purposes
- **File Extension**: `.hopp` (Human Orion++)

### Binary Format (.orionpp)
- **Purpose**: Production use, deployment, distribution
- **Design**: Compact binary representation with fast loading
- **Use Cases**: Runtime execution, library distribution, optimized storage
- **File Extension**: `.orionpp`

## Architecture

```
Source Code
     ↓
Frontend Compiler
     ↓
Orion++ IR (.hopp/.orionpp)
     ↓
Optimization Passes
     ↓
Backend Code Generator
     ↓
Target Assembly/Machine Code
```

## Core Concepts

### Variables
- **Numeric IDs**: Variables referenced by numeric identifiers ($1, $256, etc.)
- **Type Ranges**: Parameters ($1-$255), locals ($256-$65535), temporaries ($65536+)
- **Efficiency**: Direct numeric references avoid string lookups

### Symbols
- **Flexible Naming**: Enclosed in `@"..."` to support any character set
- **Scope Management**: Global, local, and weak visibility
- **Unicode Support**: Full UTF-8 support for international development

### Functions
- **ABI Abstraction**: Support for different calling conventions
- **Type Hints**: Optional type information for optimization
- **Structured Entry/Exit**: Clear function boundaries and scope management

## Documentation Structure

This documentation is organized into several key sections:

1. **[Language Specification](./HUMAN_FORMAT.md)** - Complete syntax and semantics for .hopp format
2. **[Binary Format Specification](./BINARY_FORMAT.md)** - Detailed binary format layout and encoding
3. **[Library API Reference](./LIBRARY_API.md)** - Programming interfaces for reading/writing
4. **[Developer Guide](./DEV_GUIDE.md)** - Implementation guidelines and best practices
5. **[Examples & Tutorials](orion_examples)** - Practical examples and learning materials
6. **[Tools Reference](orion_tools)** - Command-line tools and utilities

## Quick Start

### Reading Orion++ Files

```c
#include <orionpp/reader.h>

// Load binary format
OrionContext* ctx = orion_load_file("program.orionpp");
if (!ctx) {
  fprintf(stderr, "Failed to load file\n");
  return -1;
}

// Access functions
size_t func_count = orion_get_function_count(ctx);
for (size_t i = 0; i < func_count; i++) {
  OrionFunction* func = orion_get_function(ctx, i);
  printf("Function: %s\n", orion_get_function_name(func));
}

orion_free_context(ctx);
```

### Writing Orion++ Files

```c
#include <orionpp/writer.h>

// Create new context
OrionWriter* writer = orion_writer_create();

// Add function
OrionFunctionBuilder* func = orion_add_function(writer, "main");
orion_set_function_abi(func, ORION_ABI_C);
orion_set_return_type(func, ORION_TYPE_I32);

// Add instructions
uint32_t var1 = orion_add_const(func, 42);
uint32_t var2 = orion_add_const(func, 8);
uint32_t result = orion_add_instruction(func, ORION_OP_ADD, var1, var2);
orion_set_return_value(func, result);

// Write to file
orion_write_binary(writer, "output.orionpp");
orion_writer_free(writer);
```

## Language Support

Orion++ is designed to be a target for various high-level languages:

- **C/C++**: Direct mapping of language constructs
- **Rust**: Zero-cost abstractions and memory safety
- **Go**: Goroutines and channel operations
- **JavaScript/TypeScript**: Dynamic typing and prototype chains
- **Python**: Dynamic dispatch and object model
- **Java/C#**: Object-oriented features and garbage collection

## Performance Characteristics

- **Binary Loading**: O(1) file mapping, minimal parsing overhead
- **Symbol Resolution**: O(1) symbol lookup via string table offsets
- **Memory Usage**: Compact representation, ~70% smaller than text format
- **Parsing Speed**: ~10x faster than human-readable format parsing

## Compatibility

- **Minimum Version**: Orion++ 2.0
- **Backward Compatibility**: Version 1.x files supported with automatic upgrade
- **Forward Compatibility**: Graceful handling of newer format features
- **Platform Support**: Windows, Linux, macOS, embedded systems

## License and Contributing

Orion++ is designed to be an open standard for intermediate representations. Implementations should follow the specifications outlined in this documentation to ensure compatibility across toolchains.

## Next Steps

- Start with the [Language Specification](orion_language_spec) to understand the syntax
- Review [Binary Format](orion_binary_spec) for implementation details
- Check [Library API](orion_library_api) for programming interfaces
- See [Examples](orion_examples) for practical usage patterns

---

*This documentation covers Orion++ version 2.0 and later. For legacy version 1.x support, see the compatibility guide.*