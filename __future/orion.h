/**
* @file include/orion/orion.h
* @brief functionality for reading and writing orion*
*
* This header file contains the base functionality for writing orion base and its subsets.
*
* Base orion consists of a simple assembly language structued similarly to WASM in its functioanlity
* limiting itself to standard programming features like arithmetic, control flow and memory addressing.
*
* From here orion has a subset which is closer to the real underlying targets assembly language like orion.x86
* which as the name suggests is for the x86 architecture family like x86 (8086), i386 and AMD64 which targets
* a lot of similar processor ISAs.
*
* After the subset is simply the direct machine code for the specific target. the orion base and subsets are simple
* with efficient instructions that allow this to be used for JIT which can then store the output for the device
* specifically supported by orion objects and linker which can call back on the orion compiler (if listed in the object)
* to recompile the orion at whichever stage needed to recreate and reoptimize the machine code to then be stored.
*
* This can be incredibly beneficial in cases like devices which will be changed out often i.e. GPUs or FPGAs.
*
*
*/