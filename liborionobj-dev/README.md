# Orion Object Development Library

A C library for reading, writing, and manipulating Orion object files - the native object format for the Orion toolchain.

## Overview

The Orion Object Development Library (`liborionobj-dev`) provides a comprehensive API for working with Orion object files. These files can contain multiple intermediate representations including:

- **Orion++**: High-level device-agnostic intermediate representation
- **Orion Variants**: Target-specific intermediate representations (orion.x86, orion.ARM, etc.)
- **Native Code**: Final machine code for specific architectures

## Features

- ✅ **Multi-IR Support**: Handle multiple intermediate representations in a single object file
- ✅ **ELF-Inspired Design**: Familiar structure with sections, symbols, and string tables
- ✅ **Cross-Platform**: Works on Linux, macOS, and Windows
- ✅ **Memory Efficient**: Lazy loading and efficient memory management
- ✅ **Extensible**: Easy to add new section types and architectures
- ✅ **Debug Friendly**: Built-in utilities for inspecting objects

## Quick Start

### Building the Library

```bash
# Clone or extract the library
cd liborionobj-dev

# Build library and examples
make all

# Run the example
make test
```

### Basic Usage

```c
#include "orionobj.h"
#include "orionobj_utils.h"

// Create a new Orion++ object
OrionObject* obj = orion_create_orionpp_object();

// Get a section and add some code
OrionSection* text = orion_obj_get_section(obj, ".orionpp.text");
const char* code = "isa.let a, 10\nisa.let b, 20\nisa.add result, a, b\n";
orion_section_set_data(text, code, strlen(code));

// Add a symbol
uint32_t text_idx = orion_get_section_index(obj, ".orionpp.text");
orion_add_global_function(obj, "my_function", text_idx, 0, strlen(code));

// Save the object
orion_obj_save(obj, "output.orion");

// Clean up
orion_obj_free(obj);
```

## API Reference

### Core Functions

#### Object Management
```c
OrionObject* orion_obj_create(OrionObjectType type, OrionTargetArch target);
OrionObject* orion_obj_load(const char* filename);
int orion_obj_save(OrionObject* obj, const char* filename);
void orion_obj_free(OrionObject* obj);
```

#### Section Management
```c
OrionSection* orion_obj_add_section(OrionObject* obj, const char* name, OrionSectionType type);
OrionSection* orion_obj_get_section(OrionObject* obj, const char* name);
int orion_section_set_data(OrionSection* section, const void* data, size_t size);
void* orion_section_get_data(OrionSection* section);
```

#### Symbol Management
```c
int orion_obj_add_symbol(OrionObject* obj, const char* name, OrionSymbolType type,
                         OrionSymbolBinding binding, uint16_t section_index,
                         uint64_t value, uint64_t size);
OrionSymbol* orion_obj_get_symbol(OrionObject* obj, const char* name);
```

### Convenience Functions

The `orionobj_utils.h` header provides high-level convenience functions:

```c
// Create pre-configured objects
OrionObject* orion_create_orionpp_object(void);
OrionObject* orion_create_orion_variant_object(OrionVariantType variant);
OrionObject* orion_create_native_object(OrionTargetArch target);

// Easy symbol creation
int orion_add_global_function(OrionObject* obj, const char* name, 
                              uint16_t section_index, uint64_t addr, uint64_t size);
int orion_add_local_data(OrionObject* obj, const char* name,
                         uint16_t section_index, uint64_t addr, uint64_t size);

// Inspection utilities
void orion_print_object_info(OrionObject* obj);
int orion_has_orionpp(OrionObject* obj);
int orion_has_native(OrionObject* obj);
```

## Object Types

### Orion++ Objects
High-level intermediate representation, device-agnostic:
```c
OrionObject* obj = orion_create_orionpp_object();
// Contains: .orionpp.text, .orionpp.data, .orionpp.bss sections
```

### Orion Variant Objects
Target-specific intermediate representations:
```c
OrionObject* obj = orion_create_orion_variant_object(ORION_VAR_X86);
// Contains: .orion.x86.text, .orion.x86.data, .orion.x86.bss sections
```

### Native Objects
Final machine code for specific architectures:
```c
OrionObject* obj = orion_create_native_object(ORION_TARGET_X86_64);
// Contains: .text, .data, .bss sections
```

## Supported Architectures

### Target Architectures (for native code)
- `ORION_TARGET_X86` - x86 32-bit
- `ORION_TARGET_X86_64` - x86 64-bit  
- `ORION_TARGET_ARM` - ARM 32-bit
- `ORION_TARGET_ARM64` - ARM 64-bit
- `ORION_TARGET_RISCV32` - RISC-V 32-bit
- `ORION_TARGET_RISCV64` - RISC-V 64-bit
- `ORION_TARGET_RISCV128` - RISC-V 128-bit

### Orion Variants
- `ORION_VAR_X86` - Orion.x86 variant
- `ORION_VAR_ARM` - Orion.ARM variant  
- `ORION_VAR_RISCV6` - Orion.RISCV6 variant

## Examples

The `examples/` directory contains:

- **example.c**: Comprehensive example showing object creation, manipulation, and saving
- Run with `make test` to see the library in action

## Error Handling

The library uses a global error state that can be checked:

```c
OrionObject* obj = orion_obj_load("file.orion");
if (!obj) {
  OrionError error = orion_get_last_error();
  printf("Error: %s\n", orion_get_error_string(error));
}
```

### Error Codes
- `ORION_OK` - Success
- `ORION_ERR_INVALID_MAGIC` - Invalid magic number
- `ORION_ERR_INVALID_VERSION` - Unsupported version
- `ORION_ERR_FILE_NOT_FOUND` - File not found
- `ORION_ERR_OUT_OF_MEMORY` - Memory allocation failed
- `ORION_ERR_INVALID_SECTION` - Invalid section
- `ORION_ERR_INVALID_SYMBOL` - Invalid symbol

## Integration with Orion Toolchain

This library is designed to integrate seamlessly with the Orion toolchain:

```
[.c] -> orioncc -> [.orionpp] -> oc++ -> [.*.orion] -> oc -> [.o] -> ol -> [executable]
```

- **orioncc**: Produces `.orionpp` files with Orion++ IR
- **oc++**: Adds Orion variant sections to existing objects
- **oc**: Adds native machine code sections
- **ol**: Links final executables

Each stage can read existing sections and add new ones without losing previous intermediate representations.

## File Format

Orion objects use a custom format inspired by ELF:

- **Header**: Contains object metadata and section table location
- **Section Headers**: Describe each section (type, size, flags, etc.)
- **Section Data**: The actual content (code, data, symbols, strings)

See `docs/OBJECT_FORMAT.md` for detailed format specification.

## Building and Installation

### Prerequisites
- GCC or Clang with C99 support
- Make

### Build Options

```bash
make all        # Build library and examples
make library    # Build only the library  
make examples   # Build only examples
make test       # Build and run example
make clean      # Clean build artifacts
make debug      # Build with debug info
make release    # Build optimized version
make install    # Install to system (requires permissions)
```

### Installation

```bash
# Install library and headers
sudo make install

# Or manually copy
cp build/liborionobj-dev.a /usr/local/lib/
cp include/*.h /usr/local/include/
```

## License

This library is part of the Orion toolchain. See the main project for license information.

## Contributing

1. Follow the existing code style (2-space tabs as specified in preferences)
2. Add tests for new functionality
3. Update documentation for API changes
4. Ensure backward compatibility when possible

## Troubleshooting

### Common Issues

**"Invalid magic number" error**
- File is not a valid Orion object
- File may be corrupted

**"Out of memory" error**  
- Object file too large for available memory
- Memory leak in user code

**"File not found" error**
- Check file path and permissions
- Ensure file exists and is readable

### Debug Build

Use the debug build for more verbose error reporting:

```bash
make clean debug
./build/example
```

## Support

For issues related to this library:
1. Check the examples in `examples/` directory
2. Review the API documentation in headers
3. Consult `docs/OBJECT_FORMAT.md` for format details
4. Open an issue in the main Orion toolchain repository