# Device Interaction Model

## Overview

The COIL Device Interaction Model defines how COIL programs interact directly with hardware devices without operating system mediation. This model ensures consistent access to device features while maintaining COIL's platform independence.

## Core Principles

COIL follows these principles for device interaction:

1. **Direct Device Access**: Programs interact directly with hardware when possible
2. **OS Independence**: Core operations do not rely on operating system services
3. **Capability-Based Approach**: Feature usage based on device capabilities, not OS abstractions
4. **Standard Library Delegation**: OS-specific interactions delegated to standard library

## Device Interaction Levels

COIL defines three interaction levels:

### 1. Direct Hardware Access

For bare-metal environments or privileged code:
- Direct memory-mapped I/O
- Direct port I/O (where applicable)
- Direct device register access
- No operating system involvement

### 2. Device Abstraction Layer

For device-independent code:
- Abstracts hardware differences while avoiding OS dependencies
- Maps to direct hardware instructions where possible
- Provides fallback implementations for unsupported hardware

### 3. Standard Library Mediated

For OS-specific operations:
- Not part of core COIL, but provided by standard library
- Wraps OS-specific functionality
- Used only when direct device interaction is not feasible

## Device Memory Model

### Memory-Mapped I/O

For accessing device registers and buffers:

```
// Map device memory range
DEVMAP device_ptr, device_address, size, flags

// Access device memory
MOV device_register, value   // Write to device
MOV value, device_register   // Read from device
```

Memory-mapped I/O follows these rules:
1. Volatile by default (not cached)
2. Specific alignment requirements based on device
3. Memory barriers inserted as needed

### Port-Based I/O

For architectures with dedicated I/O ports:

```
// Write to port
OUTPORT port_number, value

// Read from port
INPORT value, port_number
```

## Device Capabilities

Device capabilities are exposed through the COIL configuration:

1. **Intrinsic Capabilities**: Direct hardware features
2. **Abstracted Capabilities**: Features that may be emulated
3. **Device-Specific Capabilities**: Features unique to certain devices

## Processing Unit Instructions

### Device Control Instructions (0x90-0x9F)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0x90   | DEVMAP   | Map device memory region | 3-4 | Core |
| 0x91   | DEVUNMAP | Unmap device memory region | 1 | Core |
| 0x92   | INPORT   | Input from I/O port | 2 | Core |
| 0x93   | OUTPORT  | Output to I/O port | 2 | Core |
| 0x94   | DEVCFG   | Configure device | 2-3 | Core |
| 0x95   | DEVSTAT  | Get device status | 2 | Core |
| 0x96   | DEVRESET | Reset device | 1-2 | Core |
| 0x97   | DEVCTL   | Device control operation | 2-3 | Core |

## Operation Without Operating System

COIL's device interaction enables:

1. **Bare-Metal Programming**: Direct execution on hardware without OS
2. **Kernel Development**: Building operating system kernels
3. **Device Drivers**: Implementing device drivers
4. **Embedded Systems**: Programming resource-constrained devices

## OS-Independent Synchronization

For device synchronization without OS dependencies:

1. **Polling**: Check device status registers directly
2. **Hardware Interrupts**: Configure and handle interrupts directly
3. **Memory Barriers**: Use ATOMFENCE for appropriate memory ordering
4. **Wait States**: Use hardware-specific wait mechanisms (when available)

## Implementation Requirements

COIL implementations must:

1. **Direct Access**: Provide direct device access where hardware allows
2. **Capability Reporting**: Accurately report device interaction capabilities
3. **Memory Ordering**: Ensure correct memory ordering for device interactions
4. **Error Handling**: Provide mechanisms to detect and handle device errors
5. **Privilege Levels**: Respect hardware protection mechanisms

## Standard Library Delegation

For operations that cannot be reasonably implemented without OS mediation:

1. Clearly document which operations require standard library
2. Provide standard library interfaces for common OS services
3. Maintain core/standard library separation

## Security Considerations

Direct device access introduces several security considerations:

1. **Privilege Enforcement**: Respect hardware privilege levels
2. **Resource Protection**: Prevent unintended access to devices
3. **Isolation**: Maintain isolation between different execution contexts
4. **Secure Boot**: Support for secure boot mechanisms (when available)