# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for maximum portability while retaining native performance across diverse processing architectures. It bridges the gap between high-level programming languages and hardware-specific machine code.

## Projects

The relationship between CASM, COIL and CBC is simple. CASM is the standard human representation of COIL serving as the assembly language where COIL is the machine code. This does not mean compilers should utilize CASM instead of COIL when targeting a COIL backend quite the opposite but for anyone who wishes to create a coil disassembler then you could use CASM as the standard language or if you wanted to write direct COIL then CASM compilers/assemblers supports this functionality. CBC however is a file format meant to be after COIL, it does not technically have its own file format but is instead embedded into .coilo files for the purpose of JIT or interpretation. An interpreter could be made for standard COIL but it would be slightly slower trying to work out type inference rather the byte code is a compiled interpreted variation. CBC is needed to support runtime device selection. COIL is made foremost to be a language that can be used anywhere so yes it can compile into a no dependicy or you can utilize the standard library and special directives to support operating system communication and hetregenous computing. Select the device at runtime and the byte code compiled will then be simply converted to the selected device at the load time, like linking a dynamic library.

## Key Features

- **Type-Determined Instructions**: Instructions derive behavior from operand types
- **Architecture Independence**: Clean separation between universal and platform-specific code
- **Variable System**: Abstract management of registers and memory
- **ABI System**: Automated function call conventions
- **Performance-First**: No abstraction penalties for core operations

## Version Information

Current specification version: 1.0.0

The COIL specification follows semantic versioning:
- **Major**: Incompatible changes to the instruction set or binary format
- **Minor**: Backwards-compatible additions
- **Patch**: Clarifications and documentation improvements

## Contributing

For information on contributing to the COIL specification, please see [CONTRIBUTING.md](/CONTRIBUTING.md).

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.