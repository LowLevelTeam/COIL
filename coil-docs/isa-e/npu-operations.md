# NPU Operations

**IMPORTANT**: Not a single instruction here is finalized

## Overview

NPU Operations are extended instructions (0xA0-0xBF) that work across all Neural Processing Units regardless of their specific architecture. These instructions provide access to hardware capabilities specialized for machine learning workloads, including tensor operations, activation functions, and quantization.

**IMPORTANT**: These operations are only available when targeting an NPU processing unit. Use conditional compilation to ensure portability.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xA0   | TCONV    | Tensor convolution |
| 0xA1   | TPROD    | Tensor product |
| 0xA2   | TSUM     | Tensor summation |
| 0xA3   | TMAX     | Tensor maximum |
| 0xA4   | TMIN     | Tensor minimum |
| 0xA5   | TQUANT   | Tensor quantization |
| 0xA6   | TPOOL    | Tensor pooling |
| 0xA7   | TNORM    | Tensor normalization |
| 0xA8   | ACTFN    | Activation function |
| 0xA9   | TDOT     | Tensor dot product |

## Detailed Specifications

### TCONV (0xA0)

Performs tensor convolution.

**Encoding:**
```
[0xA0][0x04][dest: Operand][input: Operand][kernel: Operand][options: Operand]
```

**Behavior:**
- Performs convolution between input tensor and kernel tensor
- Options specify stride, padding, dilation, and groups
- Destination receives result tensor with appropriately computed dimensions
- Commonly used for neural network convolutional layers
- Supports both 2D and 3D convolutions based on input dimensions

### TPROD (0xA1)

Performs tensor product.

**Encoding:**
```
[0xA1][0x03][dest: Operand][tensorA: Operand][tensorB: Operand]
```

**Behavior:**
- Calculates the tensor product of two tensors
- Dimension compatibility follows tensor product rules
- Result tensor has rank equal to the sum of input ranks
- Similar to outer product but generalized to higher dimensions
- For matrices, equivalent to matrix multiplication when using appropriate contractions

### TSUM (0xA2)

Performs tensor summation across dimensions.

**Encoding:**
```
[0xA2][0x03][dest: Operand][tensor: Operand][dims: Operand]
```

**Behavior:**
- Sums tensor elements across specified dimensions
- Dims specifies which dimensions to reduce
- Result tensor has reduced rank
- Can optionally keep dimensions with size 1 (specified in dims options)
- Useful for reduction operations in neural networks

### TMAX (0xA3)

Finds maximum values across tensor dimensions.

**Encoding:**
```
[0xA3][0x03][dest: Operand][tensor: Operand][dims: Operand]
```

**Behavior:**
- Finds maximum values across specified dimensions
- Dims specifies which dimensions to reduce
- Result tensor contains maximum values
- Can optionally return indices of maximum values (specified in dims options)
- Commonly used in pooling operations

### TMIN (0xA4)

Finds minimum values across tensor dimensions.

**Encoding:**
```
[0xA4][0x03][dest: Operand][tensor: Operand][dims: Operand]
```

**Behavior:**
- Finds minimum values across specified dimensions
- Dims specifies which dimensions to reduce
- Result tensor contains minimum values
- Can optionally return indices of minimum values (specified in dims options)
- Used in various neural network operations

### TQUANT (0xA5)

Performs tensor quantization.

**Encoding:**
```
[0xA5][0x04][dest: Operand][tensor: Operand][scale: Operand][zero_point: Operand]
```

**Behavior:**
- Quantizes a floating-point tensor to lower precision
- Scale and zero_point define the quantization parameters
- Destination receives quantized tensor
- Supports various quantization schemes (int8, uint8, etc.)
- Critical for model deployment on hardware with limited precision

### TPOOL (0xA6)

Performs pooling operation on a tensor.

**Encoding:**
```
[0xA6][0x04][dest: Operand][tensor: Operand][pool_type: Operand][options: Operand]
```

**Behavior:**
- Applies pooling operation to reduce spatial dimensions
- Pool_type specifies the pooling algorithm (max, average, etc.)
- Options specify kernel size, stride, and padding
- Commonly used in convolutional neural networks
- Reduces spatial dimensions while preserving important features

### TNORM (0xA7)

Performs tensor normalization.

**Encoding:**
```
[0xA7][0x04][dest: Operand][tensor: Operand][norm_type: Operand][params: Operand]
```

**Behavior:**
- Applies normalization to tensor values
- Norm_type specifies normalization algorithm (batch, layer, instance, etc.)
- Params provides normalization parameters (epsilon, momentum, etc.)
- Stabilizes and accelerates neural network training
- Commonly used between layers in deep networks

### ACTFN (0xA8)

Applies activation function to tensor.

**Encoding:**
```
[0xA8][0x03][dest: Operand][tensor: Operand][function: Operand]
```

**Behavior:**
- Applies specified activation function element-wise
- Function specifies the activation type (ReLU, sigmoid, tanh, etc.)
- Provides non-linearity in neural networks
- Can include parameters for parameterized activations (leaky ReLU, etc.)
- Essential component of neural network architecture

### TDOT (0xA9)

Computes tensor dot product with contraction.

**Encoding:**
```
[0xA9][0x04][dest: Operand][tensorA: Operand][tensorB: Operand][dims: Operand]
```

**Behavior:**
- Performs generalized dot product with tensor contraction
- Dims specifies which dimensions to contract
- For matrices, equivalent to matrix multiplication when contracting appropriate dimensions
- For vectors, equivalent to vector dot product
- Fundamental operation for neural network layers

## Tensor Formats

NPU operations support various tensor formats:

1. **NHWC**: Batch, Height, Width, Channels (common in TensorFlow)
2. **NCHW**: Batch, Channels, Height, Width (common in PyTorch)
3. **HWIO**: Height, Width, Input channels, Output channels (for convolution kernels)
4. **OHWI**: Output channels, Height, Width, Input channels (alternative kernel format)

The specific format used is determined by the NPU architecture and configuration.

## Precision Modes

NPU operations can work with different precision modes:

1. **FP32**: 32-bit floating point
2. **FP16**: 16-bit floating point
3. **BF16**: Brain floating point (16-bit)
4. **INT8**: 8-bit integer quantized
5. **MIXED**: Mixed precision operations

The specific mode used is determined by the operation and configuration.

## Conditional Compilation

Always wrap NPU operations in appropriate conditional blocks:

```
IF PU_TYPE EQ NPU
    // NPU-specific code using NPU operations
ELSE
    // Alternative implementation for other processing units
EIF
```

## Related Components

- [Extended ISA Overview](./overview.md) - Extended instruction set summary