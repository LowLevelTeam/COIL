# COIL Byte Code (CBC)

## Overview

COIL Byte Code (CBC) is a compact, portable binary representation designed for debugging, runtime code generation, and heterogeneous computing support. It is not the primary compilation target but serves specialized purposes within the COIL ecosystem.

## Purpose

CBC addresses several specific needs:

1. **Debugging Support**: Enable inspection and manipulation at runtime
2. **JIT Compilation**: Facilitate runtime code generation
3. **Heterogeneous Computing**: Support execution across multiple device types
4. **Runtime Device Selection**: Allow deferring device targeting decisions
5. **Standard Library Integration**: Enable interaction with multiple processing units

## Design Principles

CBC follows these design principles:

1. **Specialization**: Optimized for specific purposes, not as a universal format
2. **Pre-Optimization**: Generated after analysis and optimization
3. **Device Targeting**: Each CBC file targets a specific processing unit
4. **Debugging Focus**: Contains metadata to support debugging
5. **Performance**: Optimized for fast loading and JIT compilation

## Generation Process

CBC is generated through this workflow:

```
Source → Compiler → COIL → [Source.o && Source.cbc]
```

Where:
- Source.o: Native object binary with code for the host system
- Source.cbc: Byte code for device code or debugging purposes

## Relationship to COIL

CBC is derived from COIL but differs in several key ways:

| Aspect       | COIL                     | CBC                           |
|--------------|--------------------------|-------------------------------|
| Purpose      | Universal IR             | Specialized execution format  |
| Primary Use  | Compilation to native    | Debugging and JIT compilation |
| Optimization | Platform-independent     | Device-specific               |
| Encoding     | Type-rich, explicit      | Compact, implicit             |
| Structure    | Object-oriented          | Stack-oriented                |
| Scope        | Complete program         | Execution units               |

## Format Specification

CBC uses a specialized binary format:

```
[CBC Header]
[Metadata Section]
[Symbol Table]
[Type Table]
[Instruction Stream]
[Debug Information] (optional)
```

The CBC header identifies:
1. Target processing unit
2. Target architecture
3. Required capabilities
4. Optimization level
5. Debugging information presence

## Use Cases

### Debugging

CBC facilitates debugging by:
1. Maintaining symbolic information
2. Preserving type information
3. Supporting single-stepping and breakpoints
4. Enabling runtime code inspection

### JIT Compilation

For JIT scenarios, CBC provides:
1. Pre-optimized instruction sequences
2. Type information for runtime specialization
3. Dependency information for linking
4. Efficient encoding for fast parsing

### Heterogeneous Computing

For multi-device scenarios, CBC enables:
1. Device-specific code representation
2. Runtime device selection
3. Device-appropriate optimizations
4. Consistent execution across diverse environments

## Standard Library Integration

The standard library uses CBC to:
1. Invoke the COIL JIT Compiler (`coil::jit::Compiler`)
2. Load device-specific code at runtime
3. Execute code on appropriate processing units
4. Manage heterogeneous execution environments

## Implementation Requirements

CBC implementations must:
1. Support efficient parsing for JIT compilation
2. Maintain debugging information when required
3. Precisely encode all COIL semantics
4. Support device-specific optimizations
5. Integrate with the standard library JIT mechanisms

## Relationship with CASM

While CBC is a binary format, CASM can be used to:
1. Visualize CBC content
2. Debug CBC execution
3. Generate CBC for testing purposes