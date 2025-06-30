# OrionPP Library

A professional C library for handling Orion++ bytecode format, providing memory management, instruction processing, and I/O operations for compiler and virtual machine implementations.

## Overview

The OrionPP library is designed for high-performance applications that need to work with Orion++ bytecode, including:

- **Compilers** targeting Orion++ (C/C++, Rust, Go, etc.)
- **Virtual machines** executing Orion++ bytecode
- **Assemblers** Compile Orion++ to Orion.* and Orion Base
- **Development tools** for Orion++ analysis and manipulation

## Features

- **Memory Arena Allocator**: Efficient bucket-based memory management with I/O capabilities
- **Instruction Set Architecture**: Complete ISA definitions and operations
- **Type System**: Robust type definitions with nested type support
- **Serialization**: Binary format serialization/deserialization
- **File Format**: Header validation and feature management
- **Cross-platform**: Windows and Unix/Linux support

## Architecture

The library is organized into focused modules:

```
include/orionpp/
├── arena.h          # Memory arena allocator
├── error.h          # Error codes and messages
├── header.h         # File format headers
└── instruction.h    # Instructions and type system

src/
├── arena.c                    # Arena core functionality
├── arena_io.c                 # Arena I/O operations
├── error.c                    # Error message handling
├── header.c                   # Header validation
├── instruction.c              # Instruction lifecycle
├── instruction_format.c       # String formatting and debug
├── instruction_serialize.c    # Binary serialization
└── instruction_io.c          # File and arena I/O

tests/
├── test_framework.h          # Test framework
├── test_framework.c          # Test utilities
├── test_arena.c              # Arena tests
├── test_error.c              # Error tests
├── test_header.c             # Header tests
├── test_instruction.c        # Instruction tests
├── test_integration.c        # Integration tests
└── test_main.c              # Test runner
```

## Building

```bash
# Build Build System
cc mate.c -o mate

# Build
./mate

# Build and run tests
./mate -e
```

## Usage

### Basic Example

```c
#include <orionpp/arena.h>
#include <orionpp/instruction.h>

int main() {
  // Create memory arena
  orionpp_arena_t arena;
  orionpp_arena_create(&arena, 4096, 1024);
  
  // Create instruction
  orionpp_instruction_t instr;
  orionpp_instruction_init(&instr);
  instr.opcode.root = ORIONPP_OPCODE_ISA;
  instr.opcode.module_ = ORIONPP_OP_ISA_ADD;
  
  // Write to arena
  orionpp_write_arena(&arena, &instr);
  
  // Read back
  orionpp_arena_rewind(&arena);
  orionpp_instruction_t read_instr;
  orionpp_read_arena(&arena, &read_instr);
  
  // Debug output
  orionpp_print_instr(&read_instr);
  
  // Cleanup
  orionpp_instruction_destroy(&instr);
  orionpp_instruction_destroy(&read_instr);
  orionpp_arena_destroy(&arena);
  
  return 0;
}
```

### File I/O Example

```c
#include <orionpp/header.h>
#include <orionpp/instruction.h>

int main() {
  // Create and validate header
  orionpp_header_t header;
  orionpp_header_init(&header);
  header.features |= (1 << ORIONPP_FEATURE_ABI);
  
  if (orionpp_header_validate(&header) != ORIONPP_ERROR_GOOD) {
    fprintf(stderr, "Invalid header\n");
    return 1;
  }
  
  // Write to file
  int file = open("program.opp", O_CREAT | O_WRONLY, 0644);
  write(file, &header, sizeof(header));
  
  orionpp_instruction_t instr;
  orionpp_instruction_init(&instr);
  instr.opcode.root = ORIONPP_OPCODE_ISA;
  instr.opcode.module_ = ORIONPP_OP_ISA_RET;
  
  orionpp_write_file(file, &instr);
  close(file);
  
  orionpp_instruction_destroy(&instr);
  return 0;
}
```

## Error Handling

All functions return `orionpp_error_t` codes. Use `orionpp_strerr()` for human-readable messages:

```c
orionpp_error_t err = orionpp_arena_create(&arena, 1024, 256);
if (err != ORIONPP_ERROR_GOOD) {
  fprintf(stderr, "Arena creation failed: %s\n", orionpp_strerr(err));
  return 1;
}
```

## Memory Management

The library uses explicit memory management:

- **Arena allocators**: Use `orionpp_arena_create/destroy`
- **Instructions**: Use `orionpp_instruction_init/destroy`
- **Values**: Automatically managed within instructions
- **Types**: Nested types are automatically handled

Always pair create/init functions with their corresponding destroy functions.

## Testing

The library includes comprehensive tests covering:

- **Unit tests**: Each module tested independently
- **Integration tests**: Cross-module functionality
- **Error conditions**: Boundary cases and error paths
- **Memory safety**: Leak detection and proper cleanup

Run tests with:
```bash
./build/tests
```

## Performance Considerations

- **Arena allocation**: Pre-allocate sufficient arena size to avoid bucket creation overhead
- **Instruction reuse**: Initialize once, reuse instruction structures when possible
- **Serialization**: Use appropriate buffer sizes to avoid multiple allocations
- **I/O batching**: Batch multiple instructions for better I/O performance

## Platform Support

- **Linux**: Full support with GCC/Clang
- **macOS**: Full support with Clang
- **Windows**: Full support with MSVC/MinGW
- **Other Unix**: Should work with minimal adaptation

## Contributing

The library is designed for stability and production use. When contributing:

1. Maintain the single-responsibility principle for modules
2. Keep source files under 500 lines for maintainability
3. Add comprehensive tests for new functionality
4. Follow the existing code style and documentation patterns
5. Ensure cross-platform compatibility

## License

This library is part of the Orion project. See [LICENSE](../LICENSE) file for details.

## Future Roadmap

- **Advanced optimizations**: SIMD operations for bulk instruction processing
- **Compression**: Optional compression for instruction streams
- **Debugging support**: Enhanced debugging and profiling capabilities
- **Language bindings**: C++, Rust, and Go bindings

For questions, issues, or contributions, please refer to the project documentation or contact the development team.