# NPU Processing Unit

## Overview

The Neural Processing Unit (NPU) defines COIL instructions for specialized neural network accelerators. These instructions enable efficient execution of machine learning workloads while maintaining compatibility with the COIL ecosystem.

## NPU Models

NPU instructions support different computational models:

| Model Value | Name | Description |
|-------------|------|-------------|
| 0x01 | Inference | Optimized for neural network inference |
| 0x02 | Training  | Supports both inference and training |
| 0x03 | Quantized | Specialized for quantized neural networks |
| 0x04 | Sparse    | Optimized for sparse neural networks |

## Instruction Categories

NPU-specific instructions use opcodes in the 0xA0-0xDF range, divided into categories:

1. **Processing Unit Instructions (0xA0-0xBF)**: Available on all NPU implementations
2. **Architecture-Specific Instructions (0xC0-0xCF)**: Specific to certain NPU architectures
3. **Model-Specific Instructions (0xD0-0xDF)**: Available only in specific NPU computational models

## Processing Unit Instructions (0xA0-0xBF)

### Network Operations (0xA0-0xA7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA0   | NNLOAD   | Load neural network | 2-3 |
| 0xA1   | NNRUN    | Run neural network inference | 2-3 |
| 0xA2   | NNWAIT   | Wait for network completion | 0-1 |
| 0xA3   | NNSYNC   | Synchronize NPU execution | 0-1 |
| 0xA4   | NNINFO   | Query network information | 2 |
| 0xA5   | NNRELEASE| Release neural network | 1 |
| 0xA6   | NNUPDATE | Update network parameters | 2-3 |
| 0xA7   | NNCOMPILE| Compile neural network | 2-3 |

### Tensor Operations (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA8   | TSNEW    | Create tensor | 2-3 |
| 0xA9   | TSDEL    | Delete tensor | 1 |
| 0xAA   | TSLOAD   | Load data into tensor | 2-3 |
| 0xAB   | TSSTORE  | Store tensor data | 2-3 |
| 0xAC   | TSALLOC  | Allocate tensor memory | 2 |
| 0xAD   | TSFREE   | Free tensor memory | 1 |
| 0xAE   | TSSHAPE  | Get/set tensor shape | 2-3 |
| 0xAF   | TSINFO   | Query tensor information | 2 |

### Layer Operations (0xB0-0xB7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB0   | CONV     | Convolution operation | 3-5 |
| 0xB1   | POOL     | Pooling operation | 3-4 |
| 0xB2   | MATMUL   | Matrix multiplication | 3-4 |
| 0xB3   | ACTIVATN | Activation function | 2-3 |
| 0xB4   | NORMALIZN| Normalization | 2-4 |
| 0xB5   | SOFTMAX  | Softmax function | 2-3 |
| 0xB6   | DROPOUT  | Dropout operation | 2-4 |
| 0xB7   | LAYEROP  | Generic layer operation | 3-5 |

### NPU Management (0xB8-0xBF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB8   | NPUINFO  | Query NPU information | 2 |
| 0xB9   | NPUCAP   | Query NPU capabilities | 2 |
| 0xBA   | NPUMEM   | Query NPU memory | 2 |
| 0xBB   | NPUTYPE  | Query NPU type | 1 |
| 0xBC   | NPUPOWER | Set/get NPU power mode | 1-2 |
| 0xBF   | NPURESET | Reset NPU state | 0-1 |

## Architecture-Specific Instructions (0xC0-0xCF)

### Google TPU Architecture (0xC0-0xC3)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC0   | TPUMATRIX| TPU matrix operation | 3-4 |
| 0xC1   | TPUMMA   | TPU matrix multiply accumulate | 3-4 |
| 0xC2   | TPUWEIGHT| TPU weight management | 2-3 |
| 0xC3   | TPUCONV  | TPU specialized convolution | 3-5 |

### Nvidia Tensor Core Architecture (0xC4-0xC7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC4   | TCMATRIX | Tensor core matrix operation | 3-4 |
| 0xC5   | TCMMA    | Tensor core matrix multiply accumulate | 3-4 |
| 0xC6   | TCCONV   | Tensor core convolution | 3-5 |
| 0xC7   | TCCAST   | Tensor core precision casting | 2-3 |

### ARM NPU Architecture (0xC8-0xCB)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC8   | ARMNEON  | ARM NEON acceleration | 3-4 |
| 0xC9   | ARMMLA   | ARM matrix multiply accumulate | 3-4 |
| 0xCA   | ARMSVD   | ARM Dot product instruction | 3 |
| 0xCB   | ARMSPEC  | ARM NPU specific instruction | 2-4 |

## Model-Specific Instructions (0xD0-0xDF)

### Inference Model Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD0   | INFER    | Run inference | 2-3 |
| 0xD1   | BATCHINF | Batch inference | 2-4 |
| 0xD2   | CACHEWGT | Cache weights | 1-2 |
| 0xD3   | FUSEOPINF| Fused operation inference | 3-5 |
| 0xD7   | INFSTAT  | Inference statistics | 2 |

### Training Model Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD8   | TRAIN    | Train network | 3-5 |
| 0xD9   | BACKWARD | Backward pass | 2-3 |
| 0xDA   | GRADIENT | Compute gradient | 3-4 |
| 0xDB   | OPTIMIZE | Apply optimizer | 3-4 |
| 0xDC   | WGTUPDATN| Update weights | 3-4 |
| 0xDD   | BATCHTRAN| Batch training | 3-5 |
| 0xDE   | CHECKPNT | Create/load checkpoint | 2-3 |
| 0xDF   | TRAINSTAT| Training statistics | 2 |

## Detailed Instruction Behaviors

### Network Operations

#### NNLOAD (0xA0)
```
NNLOAD dest, source [, options]
```
- Loads a neural network model into the NPU
- First operand: Destination handle
- Second operand: Source data (model)
- Third operand (optional): Load options
- Returns: Success/failure status in destination handle
- Network remains loaded until explicitly released with NNRELEASE

#### NNRUN (0xA1)
```
NNRUN network, input, output
```
- Executes inference on a loaded neural network
- First operand: Network handle
- Second operand: Input tensor
- Third operand: Output tensor
- May execute asynchronously, use NNWAIT to ensure completion
- Flag effects: None, status returned in output tensor metadata

### Tensor Operations

#### TSNEW (0xA8)
```
TSNEW handle, shape [, type]
```
- Creates a new tensor with specified shape and type
- First operand: Destination handle
- Second operand: Shape descriptor (dimensions)
- Third operand (optional): Data type and flags
- Returns: Tensor handle in destination
- Memory may be allocated on the NPU or in host memory based on flags

#### MATMUL (0xB2)
```
MATMUL dest, a, b [, flags]
```
- Performs matrix multiplication between two tensors
- First operand: Destination tensor
- Second operand: First source tensor (A)
- Third operand: Second source tensor (B)
- Fourth operand (optional): Operation flags
- C = A × B, with dimensions validated at runtime

## Directive-Based Targeting

To target NPU-specific features in code, use the preprocessor directives:

```
!if defined(PU_NPU)
  !target NPU
  ; NPU-specific code here
!endif
```

For specific NPU architectures:

```
!if defined(PU_NPU) && defined(ARCH_TPU)
  !target NPU.TPU
  ; TPU-specific code here
!endif
```

## Tensor Data Types

NPU operations support various tensor data types:

| Type | Description | Size (bits) |
|------|-------------|-------------|
| FP32 | 32-bit floating-point | 32 |
| FP16 | 16-bit floating-point | 16 |
| BF16 | Brain floating-point | 16 |
| INT8 | 8-bit integer | 8 |
| INT4 | 4-bit integer | 4 |
| INT2 | 2-bit integer | 2 |
| INT1 | 1-bit integer (binary) | 1 |

## Memory Model

NPU operations follow a specialized memory model:

1. **Host Memory**: System memory accessible by CPU
2. **NPU Memory**: Dedicated memory on the neural accelerator
3. **Unified Memory**: Memory visible to both CPU and NPU
4. **Weight Memory**: Specialized memory optimized for neural network weights

## Usage Examples

### Inference Example

```
; Load a neural network model
NNLOAD network_handle, model_data, { format: ONNX }

; Create input and output tensors
TSNEW input_tensor, [1, 3, 224, 224], FP32   ; Batch=1, Channels=3, Height=224, Width=224
TSNEW output_tensor, [1, 1000], FP32         ; Batch=1, Classes=1000

; Load input data
TSLOAD input_tensor, image_data

; Run inference
NNRUN network_handle, input_tensor, output_tensor
NNWAIT                                       ; Wait for completion

; Get the output
TSSTORE result_data, output_tensor

; Cleanup
TSDEL input_tensor
TSDEL output_tensor
NNRELEASE network_handle
```

### Low-Precision Inference

```
!target NPU
!mode QUANTIZED

; Load quantized model
NNLOAD qmodel, quant_data, { format: TFLITE, precision: INT8 }

; Create tensors with appropriate precision
TSNEW qinput, [1, 3, 224, 224], INT8
TSNEW qoutput, [1, 1000], INT8

; Load and quantize input
TSLOAD qinput, image_data, { quantize: TRUE, zero_point: 128, scale: 0.5 }

; Run optimized inference
INFER qmodel, qinput, qoutput
NNWAIT

; Dequantize output
TSSTORE result_data, qoutput, { dequantize: TRUE }

; Cleanup
TSDEL qinput
TSDEL qoutput
NNRELEASE qmodel
```