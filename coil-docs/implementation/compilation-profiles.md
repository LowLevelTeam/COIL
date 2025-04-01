# Compilation Profiles

## Overview

Compilation profiles define specific configurations and optimization strategies for the COIL toolchain. These profiles allow developers to tailor the compilation process for different devices, performance targets, and deployment scenarios.

## Optimization Levels

COIL defines standard optimization levels:

| Level | Name | Description |
|-------|------|-------------|
| `-O0` | None | No optimization, fastest compilation, best debugging |
| `-O1` | Basic | Simple optimizations, reasonable compilation speed |
| `-O2` | Moderate | Default level, good balance of performance and size |
| `-O3` | Aggressive | Maximum performance, slower compilation |
| `-Os` | Size | Optimize for size rather than speed |
| `-Oz` | Extreme Size | Minimum size, even at significant performance cost |

### Level Details

Each level enables specific optimizations:

#### -O0 (None)
- No optimizations
- Direct instruction mapping
- Preserve all variables
- Maintain original control flow
- Best debugging experience

#### -O1 (Basic)
- Constant folding and propagation
- Dead code elimination
- Basic register allocation
- Simple instruction combining
- Preserves most debugging information

#### -O2 (Moderate)
- Common subexpression elimination
- Loop-invariant code motion
- Strength reduction
- Aggressive register allocation
- Instruction scheduling
- Some inlining

#### -O3 (Aggressive)
- Function inlining (including large functions)
- Loop unrolling
- Vectorization
- Interprocedural optimization
- Speculative execution

#### -Os (Size)
- Optimize for code size
- Limited inlining
- Instruction size reduction
- Common tail merging
- Prioritize compact encodings

## Debug Profiles

Debug profiles control the amount of debugging information:

| Profile | Description |
|---------|-------------|
| `-g0` | No debugging information |
| `-g1` | Minimal debugging information (function boundaries only) |
| `-g2` | Standard debugging information (default with `-g`) |
| `-g3` | Extended debugging information (including macro expansion) |

## Multi-Device Compilation

### Device-Specific Compilation

Compile for different targets separately:

```bash
# Compile for CPU
casm -t cpu-x86-64 source.casm -o source.cpu.coil
coilp source.cpu.coil -o source.cpu.coilo

# Compile for GPU
casm -t gpu-cuda-sm75 source.casm -o source.gpu.coil
coilp source.gpu.coil -o source.gpu.coilo
```

This approach produces separate output files optimized for each target device.

### Unified Multi-Device Compilation

Compile for multiple targets in a single file:

```bash
# Compile for CPU and GPU
casm -t cpu-x86-64,gpu-cuda-sm75 source.casm -o source.coil
coilp source.coil -o source.coilo
```

The resulting `.coilo` file contains code sections for each target device, with runtime detection to select the appropriate implementation.

## Target-Specific Optimizations

### CPU Optimizations

```bash
# Enable specific CPU architecture features
coilp -t cpu-x86-64 -f avx,avx2,fma source.coil -o source.coilo
```

Common CPU optimization flags:
- `-f <feature>`: Enable specific hardware features (AVX, SSE4, etc.)
- `-mcpu=<cpu>`: Target specific CPU model
- `-tune=<cpu>`: Optimize for specific CPU model
- `-vectorize`: Enable auto-vectorization

### GPU Optimizations

```bash
# Enable specific GPU optimizations
coilp -t gpu-cuda-sm75 -f tensor-cores,fast-math source.coil -o source.coilo
```

Common GPU optimization flags:
- `-f <feature>`: Enable specific GPU features
- `-shared-mem=<size>`: Set shared memory size
- `-block-size=<size>`: Target specific block size
- `-unroll-factor=<n>`: Control loop unrolling

## Profile Configuration Files

Compilation profiles can be saved in configuration files:

```
# performance.profile
target = cpu-x86-64
optimization = 3
features = avx2,fma,lzcnt
vectorize = true
unroll_loops = true
inline_threshold = 250
```

Load profiles with:

```bash
coilp --profile=performance.profile source.coil -o source.coilo
```

## Specialized Profiles

### Embedded Systems Profile

```bash
# Optimize for embedded systems
coilp -Os -t cpu-arm-32 --no-exceptions --static source.coil -o source.coilo
```

Key embedded optimization flags:
- `-Os` or `-Oz`: Minimize code size
- `--no-exceptions`: Remove exception handling
- `--static`: Static linking
- `--no-stdlib`: No standard library
- `--flash-optimize`: Optimize for flash memory access

### Server Profile

```bash
# Optimize for server deployment
coilp -O3 -t cpu-x86-64 -f avx512 --tune=xeon-v4 source.coil -o source.coilo
```

Key server optimization flags:
- `-O3`: Maximum performance
- `--tune=<server-cpu>`: Specific CPU tuning
- `-f <feature>`: Enable server CPU features
- `--large-pages`: Use large memory pages
- `--prefetch`: Enable prefetch optimizations

### Real-Time Systems Profile

```bash
# Optimize for real-time systems
coilp -O2 -t cpu-arm-64 --predictable --no-speculate source.coil -o source.coilo
```

Key real-time optimization flags:
- `--predictable`: Ensure predictable execution
- `--no-speculate`: Avoid speculative execution
- `--bounded-loops`: Ensure loop bounds are known
- `--deterministic`: Ensure deterministic behavior

## JIT Compilation Profiles

For just-in-time compilation scenarios:

```bash
# Generate JIT-friendly output
coilp --jit-friendly -O2 source.coil -o source.coilo
```

JIT optimization flags:
- `--jit-friendly`: Optimize for JIT compilation
- `--lazy-compilation`: Enable lazy function compilation
- `--hotspot-threshold=<n>`: Threshold for hotspot optimization
- `--trace-regions`: Enable trace-based compilation

## Related Components

- [Command Interfaces](/coil-docs/implementation/command-interfaces.md) - Command-line options for specifying profiles
- [Toolchain Components](/coil-docs/implementation/toolchain-components.md) - Toolchain components that use these profiles
- [Device Targeting](/coil-docs/systems/device-targeting.md) - Detailed device targeting options