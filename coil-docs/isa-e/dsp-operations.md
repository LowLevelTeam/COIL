# DSP Operations

**IMPORTANT**: Not a single instruction here is finalized

## Overview

DSP Operations are extended instructions (0xA0-0xBF) that work across all Digital Signal Processors regardless of their specific architecture. These instructions provide access to specialized capabilities for signal processing, including filtering, transforms, and complex number operations.

**IMPORTANT**: These operations are only available when targeting a DSP processing unit. Use conditional compilation to ensure portability.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xA0   | FFT      | Fast Fourier Transform |
| 0xA1   | IFFT     | Inverse Fast Fourier Transform |
| 0xA2   | FILTER   | Digital filter application |
| 0xA3   | CONV     | Convolution |
| 0xA4   | CORR     | Correlation |
| 0xA5   | CMUL     | Complex multiplication |
| 0xA6   | CDIV     | Complex division |
| 0xA7   | IMDCT    | Inverse Modified Discrete Cosine Transform |
| 0xA8   | WINDOW   | Apply window function |
| 0xA9   | INTERP   | Signal interpolation |

## Detailed Specifications

### FFT (0xA0)

Performs Fast Fourier Transform.

**Encoding:**
```
// Standard FFT
[0xA0][0x03][dest: Operand][src: Operand][size: Operand]

// FFT with options
[0xA0][0x04][dest: Operand][src: Operand][size: Operand][options: Operand]
```

**Behavior:**
- Transforms time domain data to frequency domain
- Source can be real or complex data
- Size must be a power of 2 for most efficient processing
- Options can specify windowing, scaling, and zero padding
- Result is complex data with real and imaginary components
- Sets flags based on operation success

### IFFT (0xA1)

Performs Inverse Fast Fourier Transform.

**Encoding:**
```
// Standard IFFT
[0xA1][0x03][dest: Operand][src: Operand][size: Operand]

// IFFT with options
[0xA1][0x04][dest: Operand][src: Operand][size: Operand][options: Operand]
```

**Behavior:**
- Transforms frequency domain data to time domain
- Source must be complex data (result of FFT)
- Size must be a power of 2 for most efficient processing
- Options can specify scaling and normalization
- Result can be complex or real data
- Sets flags based on operation success

### FILTER (0xA2)

Applies digital filter to signal data.

**Encoding:**
```
[0xA2][0x04][dest: Operand][src: Operand][coeffs: Operand][state: Operand]
```

**Behavior:**
- Applies filter defined by coefficients to source data
- Coeffs specifies filter coefficients (FIR or IIR)
- State provides filter state for IIR filters or multi-block processing
- Updates state for subsequent processing
- Supports various filter types based on coefficient arrangement
- Sets flags based on operation success or filter stability

### CONV (0xA3)

Performs convolution between signals.

**Encoding:**
```
[0xA3][0x04][dest: Operand][sigA: Operand][sigB: Operand][options: Operand]
```

**Behavior:**
- Calculates convolution between signals
- Can specify full, same, or valid convolution modes in options
- For long signals, may use FFT-based convolution automatically
- Critical operation for filtering and signal processing
- Sets flags based on operation success

### CORR (0xA4)

Performs correlation between signals.

**Encoding:**
```
[0xA4][0x04][dest: Operand][sigA: Operand][sigB: Operand][options: Operand]
```

**Behavior:**
- Calculates correlation between signals
- Can specify full, same, or valid correlation modes in options
- For long signals, may use FFT-based correlation automatically
- Used for pattern matching and signal alignment
- Sets flags based on operation success

### CMUL (0xA5)

Performs complex number multiplication.

**Encoding:**
```
// Three-operand form (dest = src1 * src2)
[0xA5][0x03][dest: Operand][src1: Operand][src2: Operand]

// Vector form (element-wise multiplication)
[0xA5][0x03][dest: Operand][vec1: Operand][vec2: Operand]
```

**Behavior:**
- Multiplies complex numbers or vectors of complex numbers
- For scalar form: dest = src1 * src2 using complex multiplication rules
- For vector form: performs element-wise complex multiplication
- Optimized for DSP complex number representation
- Sets flags based on operation success

### CDIV (0xA6)

Performs complex number division.

**Encoding:**
```
// Three-operand form (dest = src1 / src2)
[0xA6][0x03][dest: Operand][src1: Operand][src2: Operand]

// Vector form (element-wise division)
[0xA6][0x03][dest: Operand][vec1: Operand][vec2: Operand]
```

**Behavior:**
- Divides complex numbers or vectors of complex numbers
- For scalar form: dest = src1 / src2 using complex division rules
- For vector form: performs element-wise complex division
- Optimized for DSP complex number representation
- Sets flags based on operation success or division by zero

### IMDCT (0xA7)

Performs Inverse Modified Discrete Cosine Transform.

**Encoding:**
```
[0xA7][0x03][dest: Operand][src: Operand][size: Operand]
```

**Behavior:**
- Calculates IMDCT for audio compression applications
- Common in MP3, AAC, and other audio codecs
- Size specifies transform size
- Takes frequency domain coefficients and produces time domain samples
- Includes critical 50% overlap-add functionality
- Sets flags based on operation success

### WINDOW (0xA8)

Applies window function to signal data.

**Encoding:**
```
[0xA8][0x04][dest: Operand][src: Operand][window_type: Operand][size: Operand]
```

**Behavior:**
- Applies specified window function to signal data
- Window_type selects window function (Hann, Hamming, Blackman, etc.)
- Size specifies window size
- Used for spectral analysis and filter design
- Can be in-place if dest and src are the same
- Sets flags based on operation success

### INTERP (0xA9)

Performs signal interpolation.

**Encoding:**
```
[0xA9][0x05][dest: Operand][src: Operand][factor: Operand][method: Operand][params: Operand]
```

**Behavior:**
- Interpolates signal by specified factor
- Method selects interpolation algorithm (linear, cubic, sinc, etc.)
- Params provides algorithm-specific parameters
- Used for sample rate conversion and signal reconstruction
- Output signal length = input length * factor
- Sets flags based on operation success

## Complex Number Representation

DSP operations typically use one of these complex number representations:

1. **Split Complex**: Real and imaginary parts stored in separate arrays
2. **Interleaved Complex**: Real and imaginary parts alternating in a single array
3. **Struct Complex**: Real and imaginary parts as fields in a structure

The specific representation used is determined by the DSP architecture and configuration.

## Signal Types

DSP operations work with various signal types:

1. **Time Domain**: Raw signal samples
2. **Frequency Domain**: FFT results
3. **Fixed-Point**: Scaled integer representation
4. **Floating-Point**: Higher precision representation

The appropriate signal type depends on the specific operation and processing requirements.

## Conditional Compilation

Always wrap DSP operations in appropriate conditional blocks:

```
IF PU_TYPE EQ DSP
    // DSP-specific code using DSP operations
ELSE
    // Alternative implementation for other processing units
EIF
```

## Related Components

- [Extended ISA Overview](./overview.md) - Extended instruction set summary
- [Vector Operations](../isa-u/vector-operations.md) - Universal vector operations