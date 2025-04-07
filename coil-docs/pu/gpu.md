# GPU Processing Unit

## Overview

The GPU Processing Unit defines COIL instructions for Graphics Processing Units and other massively parallel computation devices. These instructions enable high-performance parallel computing while maintaining the consistency and portability of the COIL ecosystem.

## GPU Models

GPU instructions support different computational models:

| Model Value | Name | Description |
|-------------|------|-------------|
| 0x01 | Graphics | Traditional graphics rendering pipeline |
| 0x02 | Compute | General-purpose computing (GPGPU) |
| 0x03 | Raytracing | Hardware-accelerated ray tracing |
| 0x04 | AI | Neural network acceleration |

## Instruction Categories

GPU-specific instructions use opcodes in the 0xA0-0xDF range, divided into three categories:

1. **Processing Unit Instructions (0xA0-0xBF)**: Instructions available on all GPU implementations
2. **Architecture-Specific Instructions (0xC0-0xCF)**: Instructions specific to certain GPU architectures
3. **Model-Specific Instructions (0xD0-0xDF)**: Instructions available only in specific GPU computation models

## Processing Unit Instructions (0xA0-0xBF)

### Execution Control (0xA0-0xA7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xA0   | GPULNCH  | Launch kernel | 2-3 | GPU |
| 0xA1   | GPUWAIT  | Wait for kernel completion | 0-1 | GPU |
| 0xA2   | GPUSYNC  | Synchronize execution | 0-1 | GPU |
| 0xA3   | BARRIER  | Execution barrier | 0-1 | GPU |
| 0xA4   | WGBARR   | Workgroup barrier | 0-1 | GPU |
| 0xA5   | WGSIZE   | Set/get workgroup size | 1-2 | GPU |
| 0xA6   | LOCMEM   | Allocate local memory | 2 | GPU |
| 0xA7   | PRVMEM   | Allocate private memory | 2 | GPU |

### Memory Management (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xA8   | GMEMCPY  | GPU memory copy | 3-4 | GPU |
| 0xA9   | GMEMSET  | GPU memory set | 3 | GPU |
| 0xAA   | GMEMMAP  | Map memory to GPU | 2-3 | GPU |
| 0xAB   | GMEMUMAP | Unmap memory from GPU | 1 | GPU |
| 0xAC   | GMEMIMP  | Import external memory | 2 | GPU |
| 0xAD   | GMEMEXP  | Export memory | 1-2 | GPU |
| 0xAE   | GMEMCOH  | Make memory coherent | 1-2 | GPU |
| 0xAF   | MEMFENCE | Memory access fence | 0-1 | GPU |

### Parallel Operations (0xB0-0xB7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xB0   | REDUCE   | Parallel reduction | 3-4 | GPU |
| 0xB1   | SCAN     | Parallel scan/prefix sum | 3-4 | GPU |
| 0xB2   | SHUFFLE  | Lane shuffle | 3-4 | GPU |
| 0xB3   | VOTE     | Sub-group vote | 2-3 | GPU |
| 0xB4   | BROADCAST| Sub-group broadcast | 3 | GPU |
| 0xB5   | LANEID   | Get lane ID | 1 | GPU |
| 0xB6   | WGID     | Get workgroup ID | 1-2 | GPU |
| 0xB7   | GSIZE    | Get grid size | 1-2 | GPU |

### GPU Query (0xB8-0xBF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xB8   | GPUINFO  | Query GPU information | 2 | GPU |
| 0xB9   | GPUCAP   | Query GPU capabilities | 2 | GPU |
| 0xBA   | GPUMEM   | Query GPU memory | 2 | GPU |
| 0xBB   | GPUTYPE  | Query GPU type | 1 | GPU |
| 0xBC   | GPUQUEUE | Query GPU queue properties | 2 | GPU |
| 0xBD   | GPUMODEL | Query GPU compute model | 1 | GPU |
| 0xBE   | GPUVER   | Query GPU version | 1 | GPU |
| 0xBF   | GPUSTAT  | Query GPU status | 1-2 | GPU |

## Architecture-Specific Instructions (0xC0-0xCF)

### NVIDIA Architecture (0xC0-0xC3)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC0   | NVPTX    | CUDA PTX instruction | 1-4 | GPU.NVIDIA |
| 0xC1   | NVWARP   | Warp-level operation | 2-3 | GPU.NVIDIA |
| 0xC2   | NVTEX    | Texture operation | 3-4 | GPU.NVIDIA |
| 0xC3   | NVSM     | Shared memory operation | 2-3 | GPU.NVIDIA |

### AMD Architecture (0xC4-0xC7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC4   | AMDGCN   | GCN instruction | 1-4 | GPU.AMD |
| 0xC5   | AMDWAVE  | Wavefront operation | 2-3 | GPU.AMD |
| 0xC6   | AMDLDS   | LDS operation | 2-3 | GPU.AMD |
| 0xC7   | AMDEXP   | Export operation | 2-4 | GPU.AMD |

### Intel Architecture (0xC8-0xCB)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xC8   | INTLEU   | Execution unit operation | 2-3 | GPU.Intel |
| 0xC9   | INTLSM   | Shared memory operation | 2-3 | GPU.Intel |
| 0xCA   | INTLSB   | Subgroup operation | 2-3 | GPU.Intel |
| 0xCB   | INTLSYNC | Synchronization primitive | 1-2 | GPU.Intel |

### Mobile/Embedded GPU (0xCC-0xCF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xCC   | IMGPU    | IMG PowerVR operation | 1-4 | GPU.IMG |
| 0xCD   | ARMGPU   | ARM Mali operation | 1-4 | GPU.ARM |
| 0xCE   | QUALCOMM | Qualcomm Adreno operation | 1-4 | GPU.Qualcomm |
| 0xCF   | VIVANTE  | Vivante operation | 1-4 | GPU.Vivante |

## Model-Specific Instructions (0xD0-0xDF)

### Graphics Model Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD0   | VTXPROC  | Vertex processing | 2-3 | GPU.Graphics |
| 0xD1   | FRGPROC  | Fragment processing | 2-3 | GPU.Graphics |
| 0xD2   | TEXLD    | Texture load | 3-4 | GPU.Graphics |
| 0xD3   | TEXST    | Texture store | 3-4 | GPU.Graphics |
| 0xD4   | SAMPLER  | Sampler operation | 3-4 | GPU.Graphics |
| 0xD5   | BLENDOP  | Blending operation | 3-5 | GPU.Graphics |
| 0xD6   | DEPTHOP  | Depth operation | 2-3 | GPU.Graphics |
| 0xD7   | STENCILOP| Stencil operation | 2-4 | GPU.Graphics |

### Ray Tracing Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands | Extension |
|--------|----------|-------------|----------|-----------|
| 0xD8   | BVHBUILD | Build BVH structure | 2-3 | GPU.RayTracing |
| 0xD9   | RAYCAST  | Cast ray | 3-4 | GPU.RayTracing |
| 0xDA   | RAYINT   | Ray intersection test | 3-4 | GPU.RayTracing |
| 0xDB   | RAYREFL  | Ray reflection | 3-4 | GPU.RayTracing |
| 0xDC   | RAYSCATT | Ray scattering | 3-5 | GPU.RayTracing |
| 0xDD   | RAYSHADE | Ray shading | 3-4 | GPU.RayTracing |
| 0xDE   | RAYTRACE | Trace ray path | 3-5 | GPU.RayTracing |
| 0xDF   | RAYQUERY | Ray query | 2-3 | GPU.RayTracing |

## Detailed Instruction Behaviors

### Execution Control Instructions

#### GPULNCH (0xA0)
- Launches a GPU kernel with specified parameters
- First operand: Kernel function reference
- Second operand: Work dimensions (1D, 2D, or 3D)
- Third operand (optional): Parameter block pointer
- Returns: Kernel execution handle or 0 on failure

#### BARRIER (0xA3)
- Synchronizes execution across work-items in a workgroup
- Ensures all memory operations before the barrier complete before any work-item proceeds
- Optional operand specifies memory scope (local, global, or both)
- Flag effects: None

### Memory Management Instructions

#### GMEMCPY (0xA8)
- Copies data between GPU memory locations
- First operand: Destination address
- Second operand: Source address
- Third operand: Size in bytes
- Fourth operand (optional): Flags controlling copy behavior
- Asynchronous operation, use GPUSYNC to ensure completion

## Directive-Based Targeting

To target GPU-specific features in code, use the preprocessor directives:

```
!if defined(PU_GPU)
  !target GPU
  ; GPU-specific code here
!endif
```

For specific GPU architectures:

```
!if defined(PU_GPU) && defined(ARCH_NVIDIA)
  !target GPU.NVIDIA
  ; NVIDIA-specific code here
!endif
```

## Kernel Programming Model

GPU code follows a kernel-based programming model:

1. **Kernels**: Self-contained functions executed in parallel
2. **Work-Items**: Individual parallel execution instances
3. **Workgroups**: Groups of work-items that can synchronize and share memory
4. **Memory Hierarchy**:
   - Global Memory: Accessible by all work-items
   - Local Memory: Shared within a workgroup
   - Private Memory: Exclusive to a single work-item

## Implementation Requirements

GPU implementations must adhere to these guidelines:

1. **Instruction Support**:
   - Processing Unit Instructions (0xA0-0xBF) must be implemented by all GPU targets
   - Architecture-Specific Instructions (0xC0-0xCF) are implemented only for the respective architecture
   - Model-Specific Instructions (0xD0-0xDF) are implemented only for supported computation models

2. **Error Handling**:
   - Failed operations must set appropriate error codes
   - Runtime errors must generate appropriate exceptions
   - Resource exhaustion must be reported through standard mechanisms

3. **Memory Model**:
   - Must respect the COIL memory model and visibility rules
   - Must support atomic operations on supported memory types
   - Must correctly implement memory fences and barriers
