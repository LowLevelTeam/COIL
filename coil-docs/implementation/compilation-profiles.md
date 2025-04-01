# Compilation Profiles

## Overview

Compilation profiles define specific configurations and optimization strategies for the COIL toolchain. These profiles allow developers to tailor the compilation process for different devices, performance targets, and deployment scenarios.

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

### Mixed Device Code

Code can be organized to optimize for different devices:

```
# Common code used by all devices
SECTION .common, 0x01 | 0x04

# CPU-specific implementation
TARGET 0x010103
SECTION .cpu_impl, 0x01 | 0x04, TARGET=0x010103

# GPU-specific implementation
TARGET 0x020104
SECTION .gpu_impl, 0x01 | 0x04, TARGET=0x020104
```

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

#### -Oz (Extreme Size)
- Maximum size reduction
- No inlining
- Aggressive code sharing
- Size over speed in all decisions

## Debug Profiles

Debug profiles control the amount of debugging information:

| Profile | Description |
|---------|-------------|
| `-g0` | No debugging information |
| `-g1` | Minimal debugging information (function boundaries only) |
| `-g2` | Standard debugging information (default with `-g`) |
| `-g3` | Extended debugging information (including macro expansion) |

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

## Cross-Compilation

Cross-compilation targets a different platform than the build machine:

```bash
# Cross-compile from x86-64 host to ARM64 target
casm -t cpu-arm-64 source.casm -o source.coil
coilp -t cpu-arm-64 source.coil -o source.coilo
```

Cross-compilation requires:
- Target device configuration
- Appropriate ABI specifications
- Any device-specific constraints

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
- `--max-inline-insns=<n>`: Control inline size
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

## Profile Selection Guidelines

Guidelines for selecting appropriate profiles:

1. **Development**: Use `-O0 -g` for best debugging experience
2. **Testing**: Use `-O1 -g` for reasonable performance with good debugging
3. **Release**: Use `-O2` or `-O3` depending on performance requirements
4. **Embedded**: Use `-Os` or `-Oz` to minimize size
5. **Mixed**: Use multi-device compilation for heterogeneous environments