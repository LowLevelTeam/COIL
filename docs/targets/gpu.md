# GPU Processing Unit

## Overview

The GPU Processing Unit defines COIL instructions for Graphics Processing Units and massively parallel computation devices. These instructions enable high-performance parallel computing while maintaining the consistency and portability of the COIL ecosystem.

## GPU Models

GPU instructions support different computational models:

| Model Value | Name | Description |
|-------------|------|-------------|
| 0x01 | Graphics | Traditional graphics rendering pipeline |
| 0x02 | Compute | General-purpose computing (GPGPU) |
| 0x03 | Raytracing | Hardware-accelerated ray tracing |
| 0x04 | AI | Neural network acceleration |

## Instruction Categories

GPU-specific instructions use opcodes in the 0xA0-0xDF range, divided into categories:

1. **Processing Unit Instructions (0xA0-0xBF)**: Available on all GPU implementations
2. **Architecture-Specific Instructions (0xC0-0xCF)**: Specific to certain GPU architectures
3. **Model-Specific Instructions (0xD0-0xDF)**: Available only in specific GPU computation models

## Processing Unit Instructions (0xA0-0xBF)

### Execution Control (0xA0-0xA7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA0   | GPULNCH  | Launch kernel | 2-3 |
| 0xA1   | GPUWAIT  | Wait for kernel completion | 0-1 |
| 0xA2   | GPUSYNC  | Synchronize execution | 0-1 |
| 0xA3   | BARRIER  | Execution barrier | 0-1 |
| 0xA4   | WGBARR   | Workgroup barrier | 0-1 |
| 0xA5   | WGSIZE   | Set/get workgroup size | 1-2 |
| 0xA6   | LOCMEM   | Allocate local memory | 2 |
| 0xA7   | PRVMEM   | Allocate private memory | 2 |

### Memory Management (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA8   | GMEMCPY  | GPU memory copy | 3-4 |
| 0xA9   | GMEMSET  | GPU memory set | 3 |
| 0xAA   | GMEMMAP  | Map memory to GPU | 2-3 |
| 0xAB   | GMEMUMAP | Unmap memory from GPU | 1 |
| 0xAE   | GMEMCOH  | Make memory coherent | 1-2 |
| 0xAF   | MEMFENCE | Memory access fence | 0-1 |

### Parallel Operations (0xB0-0xB7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB0   | REDUCE   | Parallel reduction | 3-4 |
| 0xB1   | SCAN     | Parallel scan/prefix sum | 3-4 |
| 0xB2   | SHUFFLE  | Lane shuffle | 3-4 |
| 0xB3   | VOTE     | Sub-group vote | 2-3 |
| 0xB4   | BROADCAST| Sub-group broadcast | 3 |
| 0xB5   | LANEID   | Get lane ID | 1 |
| 0xB6   | WGID     | Get workgroup ID | 1-2 |
| 0xB7   | GSIZE    | Get grid size | 1-2 |

### GPU Query (0xB8-0xBF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB8   | GPUINFO  | Query GPU information | 2 |
| 0xB9   | GPUCAP   | Query GPU capabilities | 2 |
| 0xBA   | GPUMEM   | Query GPU memory | 2 |
| 0xBD   | GPUMODEL | Query GPU compute model | 1 |
| 0xBE   | GPUVER   | Query GPU version | 1 |
| 0xBF   | GPUSTAT  | Query GPU status | 1-2 |

## Architecture-Specific Instructions (0xC0-0xCF)

### NVIDIA Architecture (0xC0-0xC3)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC0   | NVPTX    | CUDA PTX instruction | 1-4 |
| 0xC1   | NVWARP   | Warp-level operation | 2-3 |
| 0xC2   | NVTEX    | Texture operation | 3-4 |
| 0xC3   | NVSM     | Shared memory operation | 2-3 |

### AMD Architecture (0xC4-0xC7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC4   | AMDGCN   | GCN instruction | 1-4 |
| 0xC5   | AMDWAVE  | Wavefront operation | 2-3 |
| 0xC6   | AMDLDS   | LDS operation | 2-3 |
| 0xC7   | AMDEXP   | Export operation | 2-4 |

### Intel Architecture (0xC8-0xCB)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC8   | INTLEU   | Execution unit operation | 2-3 |
| 0xC9   | INTLSM   | Shared memory operation | 2-3 |
| 0xCA   | INTLSB   | Subgroup operation | 2-3 |
| 0xCB   | INTLSYNC | Synchronization primitive | 1-2 |

## Model-Specific Instructions (0xD0-0xDF)

### Graphics Model Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD0   | VTXPROC  | Vertex processing | 2-3 |
| 0xD1   | FRGPROC  | Fragment processing | 2-3 |
| 0xD2   | TEXLD    | Texture load | 3-4 |
| 0xD3   | TEXST    | Texture store | 3-4 |
| 0xD4   | SAMPLER  | Sampler operation | 3-4 |
| 0xD5   | BLENDOP  | Blending operation | 3-5 |
| 0xD6   | DEPTHOP  | Depth operation | 2-3 |
| 0xD7   | STENCILOP| Stencil operation | 2-4 |

### Ray Tracing Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD8   | BVHBUILD | Build BVH structure | 2-3 |
| 0xD9   | RAYCAST  | Cast ray | 3-4 |
| 0xDA   | RAYINT   | Ray intersection test | 3-4 |
| 0xDB   | RAYREFL  | Ray reflection | 3-4 |
| 0xDF   | RAYQUERY | Ray query | 2-3 |

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

## Usage Examples

### Simple Compute Kernel

```
; Define a vector addition kernel
kernel_vec_add:
    SCOPE
    
    ; Get global ID
    WGID r0, 0        ; Get workgroup ID
    WGSIZE r1         ; Get workgroup size
    MUL r2, r0, r1    ; workgroup_id * workgroup_size
    LANEID r3         ; Get lane ID
    ADD r4, r2, r3    ; global_id = workgroup_offset + lane_id
    
    ; Load data
    GETE r5, input_a, r4    ; r5 = input_a[global_id]
    GETE r6, input_b, r4    ; r6 = input_b[global_id]
    
    ; Compute and store
    ADD r7, r5, r6          ; r7 = r5 + r6
    SETE output, r4, r7     ; output[global_id] = r7
    
    RET
    SCOPL

; Launch kernel
GMEMMAP input_a, host_a, size, READ_ONLY
GMEMMAP input_b, host_b, size, READ_ONLY
GMEMMAP output, host_result, size, WRITE_ONLY

WGSIZE 256            ; Set workgroup size to 256
GPULNCH kernel_vec_add, [array_size, 1, 1]  ; 1D launch with array_size work-items
GPUWAIT               ; Wait for completion

GMEMUMAP input_a
GMEMUMAP input_b
GMEMUMAP output
```

### Graphics Pipeline Example

```
!target GPU
!mode GRAPHICS

; Vertex shader
vertex_shader:
    SCOPE
    ; Vertex processing code
    SCOPL

; Fragment shader
fragment_shader:
    SCOPE
    ; Fragment processing code
    SCOPL

; Set up pipeline
VTXPROC vertex_shader
FRGPROC fragment_shader
BLENDOP BLEND_ALPHA
DEPTHOP DEPTH_LESS

; Execute pipeline
GPULNCH graphics_pipeline, [num_vertices]
GPUWAIT
```