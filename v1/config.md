# Configuration

The COIL Configuration Format is a unified interface for interaction with the COIL processor through a easy and shareable file format controlling all stages of the compilation cycle through specifying features of an architecture the target supports. Although specifiying a specific target is not possible in COIL you can specify the architecture to access low level features like the register system of your target.

COIL does not have a place for specifying a target as its seen as not needed, any features of the architecture you don't support can for the most part be emulated in either an inline or a statically available practice. Like vectors and cryptography, the COIL processor will use native features as available to it with backup plans when not supported to allow for a write once compile anywhere build system.

Transferring a static optimized COIL file is one goal of the COIL processor. The ability to take multiple COIL files, optimize them and join them into a single output COIL file allows for creating a library that can be passed across machines with the ability to have optimal performance on each as well as being incredibily low memory impact even allowing for some possibilities of being in a network protocol.

The configuration format is defined below as a simple '${ident} = ${value}\n' with important markers separated by square brace enclosed headers

## Device Targets

As of version 1 there is only support for compiling to a host device where the host device can have different targets. There is currently no support for other processing devices like GPU, TPU, QPU etc... Version 2 of COIL will implement this but will most likely come at the cost of some JIT and operating system support with larger binaries.

A device target separate from another device defines the features for a processing unit with multiple architectures. A more practical use case this is mainly designed for ARM32 with AMR64 and x86, x86 protected mode and x86 long mode. Switching between these is important in bootloaders and bare metal devices.

Defining the features for each mode of your proecssing unit will help in the COIL code. There is a directive for defining the mode of your processing unit correlating to a configuration id for a device target. This will change the native binary to be outputted as defined by the target mode.

## Targeting Multiple Separate Devices

## Sections
The Configuration Format will also have sections beside feature processing, each device having information about security features, memory layout and memory model, resources (threads, registers, etc...) and even power. On top of this copmlex device information there will also be configuration for the linker with a simple linker script format to be passed on through the COIL processor into a COIL compatible linker.