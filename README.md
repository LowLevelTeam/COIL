# Computer Oriented Intermediate Language (COIL)

## Overview

COIL is a universal binary instruction format designed for portable, efficient computing across diverse processing architectures. It bridges the gap between high-level languages and hardware-specific machine code through a type-determined instruction model.

## Key Components

- **COIL**: Universal binary instruction format for direct device interaction
- **CASM**: Human-readable assembly representation of COIL
- **CBC**: Specialized byte code format for debugging and JIT compilation

## Core Philosophy

- **Direct Device Interaction**: Programs interact with hardware without OS dependencies
- **Type-Determined Instructions**: Instructions adapt behavior based on operand types
- **Architectural Independence**: Clean separation between universal and platform-specific operations
- **Capability-Based Design**: Features based on abstract capabilities that can be checked at compile time

## Getting Started

See the [Getting Started Guide](/docs/guides/getting-started.md) for information on how to begin working with COIL.

## Documentation

- [Core Specification](/docs/core/): Type system, instruction set, memory model, etc.
- [Extensions](/docs/extensions/): Multi-dimensional types, composite types, ABI, etc.
- [Processing Units](/docs/targets/): CPU, GPU, NPU implementations
- [Tools](/docs/tools/): CASM and CBC documentation
- [Standard Library](/docs/stdlib/): OS abstractions and utility functions

## License

COIL is released under the Unlicense. See [LICENSE](/LICENSE) for details.

## Contributing

Guidelines for contributing can be found in [CONTRIBUTING.md](/CONTRIBUTING.md).

## Roadmap

For planned development and future directions, see [ROADMAP.md](/ROADMAP.md).