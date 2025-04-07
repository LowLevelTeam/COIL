# COIL Execution Model

## Overview

The COIL Execution Model defines how COIL programs are executed, establishing consistent behavior across different implementations while allowing for platform-specific optimizations.

## Program Structure

COIL programs follow a deterministic structure:

```
[Program Entry Point]
  ├── [Initialization]
  ├── [Main Execution]
  └── [Termination]
```

### Entry Point

Every executable COIL program has a single entry point:

- Specified in the object file header (`entry_point` field)
- First instruction executed when the program starts
- Implementation may add platform-specific setup before calling entry point

### Execution Flow

1. **Program Initialization**:
   - Runtime environment setup
   - Static initializers executed
   - Global variable initialization

2. **Main Execution**:
   - Control flow follows instructions sequence
   - Function calls create new stack frames
   - Exception handling follows the specified model

3. **Program Termination**:
   - Exit code returned to the environment
   - Static finalizers executed
   - Resources released

## Instruction Execution

### Execution Cycle

Every instruction follows this basic execution cycle:

1. **Instruction Fetch**: The next instruction is retrieved from memory
2. **Instruction Decode**: Opcode and operands are parsed
3. **Operand Fetch**: Operand values are retrieved
4. **Execution**: The operation is performed
5. **Result Write**: Results are stored as required
6. **Flag Update**: Condition flags are updated according to result
7. **Program Counter Update**: Point to next instruction

### Atomicity

Instructions follow these atomicity rules:

1. **Basic Atomicity**: Each instruction is atomic with respect to other instructions
2. **Operand Atomicity**: Multiple operand updates may not be atomic unless explicitly specified
3. **Atomic Operations**: Instructions with the ATOMIC flag guarantee full memory model atomicity

## Control Flow

### Sequential Execution

By default, instructions execute sequentially from memory location N to N+length.

### Conditional Execution

Conditional execution relies on the flag register:

1. **Flag Setting**: Instructions set flags based on their results
2. **Conditional Jump**: BR and CALL can be conditional based on flag state
3. **Predication**: Some extended instruction sets may support predicated execution

### Function Calls

Function calls follow this execution model:

1. **Call**: Save return address and transfer control to target address
2. **Argument Passing**: According to the applicable ABI specification
3. **Execution**: Function code executes with its own stack frame
4. **Return**: Restore previous context and return control to caller
5. **Return Values**: Passed according to the applicable ABI specification

## Exception Handling

### Exception Types

COIL defines several standard exception types:

1. **Hardware Exceptions**:
   - Memory access violations
   - Divide-by-zero
   - Invalid instruction

2. **Software Exceptions**:
   - Explicitly thrown by program
   - ABI-defined exceptions

### Exception Propagation

When an exception occurs:

1. Control transfers to the nearest exception handler
2. If no handler exists in the current function, the exception propagates up the call stack
3. If the exception is unhandled at the top level, the program terminates

## Concurrency Model

### Execution Contexts

COIL supports multiple independent execution contexts:

1. **Thread Model**: Standard preemptive multithreading
2. **Fiber Model**: Cooperative multitasking within threads
3. **Task Model**: Asynchronous task execution

### Memory Consistency

COIL defines these memory consistency guarantees:

1. **Sequential Consistency**: Within a single thread
2. **Release-Acquire**: Between threads with proper synchronization
3. **Relaxed**: For operations marked as relaxed

### Synchronization Primitives

Built-in synchronization mechanisms include:

1. **Atomic Operations**: Instructions with ATOMIC flag
2. **Memory Barriers**: Full, acquire, and release barriers
3. **Lock Operations**: Compare-and-swap, load-linked/store-conditional

## Implementation Requirements

A conforming implementation must:

1. **Precise Execution**: Execute instructions with the specified semantics
2. **Flag Behavior**: Correctly implement flag setting/testing
3. **Exception Model**: Support the defined exception model
4. **Atomicity**: Honor atomicity guarantees
5. **Concurrency**: Implement the required concurrency model

## Processing Unit Variations

Different processing units may implement execution differently:

1. **CPU**: Von Neumann/Harvard architecture with sequential execution
2. **GPU**: SIMD/SIMT execution model with parallel processing
3. **NPU**: Neural network optimized execution paths
4. **DSP**: Signal processing optimized data paths

Each PU defines execution model extensions in its respective documentation.
