# liborionobj-dev

A compact, modular object format library for the Orion toolchain.

## Overview

`liborionobj-dev` is a universal object format library designed to handle:
- **OrionPP binary format** - The native OrionPP object format
- **Orion variations** - Object files for Orion virtual architectures (orion.x86, orion.arm, orion.riscv)
- **Cross-compilation support** - Basic support for native architectures (x86, ARM, RISC-V)

## Features

- **Simple API** - Clean, minimal interface focused on core functionality
- **Memory efficient** - String deduplication and compact data structures
- **Modular design** - Small, focused files with clear dependencies
- **Cross-platform** - Standard C with minimal dependencies

## Architecture

The library is organized into focused modules:

- `types.h` - Core types and constants
- `strtab.h` - String table management with deduplication
- `section.h` - Section creation and data management
- `symbol.h` - Symbol table operations
- `object.h` - Main object file operations
- `binary.h` - Binary format I/O

## Usage

```c
#include "orionobj/orionobj.h"

// Create a new object
orionobj_object_t* object;
orionobj_object_create(&object, ORIONOBJ_FORMAT_ORIONPP, ORIONOBJ_ARCH_ORION_X86, NULL);

// Add a text section
orionobj_section_t* text_section;
orionobj_object_add_section(object, ORIONOBJ_SECTION_TEXT, ".text", 
                             ORIONOBJ_SECTION_FLAG_ALLOC | ORIONOBJ_SECTION_FLAG_EXEC,
                             &text_section);

// Add a symbol
orionobj_symbol_t* main_symbol;
orionobj_object_add_symbol(object, "main", ORIONOBJ_SYMBOL_GLOBAL, ORIONOBJ_SYMBOL_FUNC,
                            1, 0x1000, 64, &main_symbol);

// Write to file
orionobj_object_write_to_file(object, "output.orion");

// Clean up
orionobj_object_destroy(object);
```

## Object Formats

### OrionPP Format
- Magic: `OPPN` (0x4F50504E)
- Used for OrionPP intermediate representation
- Supports all Orion virtual architectures

### Orion Format  
- Magic: `ORNX` (0x4F524E58)
- Extended format with ISA metadata and ABI information
- Supports link-time ABI selection
- Optimized for Orion toolchain integration

## Supported Architectures

### Orion Variations
- `orion.x86` - Orion virtual x86 architecture
- `orion.arm` - Orion virtual ARM architecture  
- `orion.riscv` - Orion virtual RISC-V architecture

### Native Architectures (Cross-compilation)
- `x86`, `x86-32`, `x86-64` - Intel x86 family
- `arm32`, `arm64` - ARM family
- `riscv32`, `riscv64` - RISC-V family

## Building

```bash
# Using the mate build system
./mate

# Or manually with GCC
gcc -Iinclude -c src/*.c
ar rcs liborionobj-dev.a *.o

# Run tests
gcc -Iinclude tests/test.c -L. -lorionobj-dev -o test
./test
```

## Integration

This library is designed to be used throughout the Orion toolchain:

1. **Frontend compilers** generate OrionPP objects
2. **OrionPP processors** transform between Orion variations  
3. **Cross-compilers** convert to native architectures
4. **Orion linker** consumes objects for final linking

The modular design allows each tool to use only the components it needs, keeping binary sizes small and dependencies minimal.

## License

This library is part of the Orion toolchain project.