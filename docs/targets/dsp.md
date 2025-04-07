# DSP Processing Unit

## Overview

The Digital Signal Processing (DSP) Processing Unit defines COIL instructions for specialized signal processing hardware. These instructions enable efficient execution of signal processing algorithms while maintaining compatibility with the COIL ecosystem.

## DSP Models

DSP instructions support different processing models:

| Model Value | Name | Description |
|-------------|------|-------------|
| 0x01 | Fixed-Point | Fixed-point arithmetic optimized DSP |
| 0x02 | Floating-Point | Floating-point optimized DSP |
| 0x03 | Mixed-Precision | Supports both fixed and floating-point operations |
| 0x04 | Vector | SIMD/vector processing optimized |

## Instruction Categories

DSP-specific instructions use opcodes in the 0xA0-0xDF range, divided into categories:

1. **Processing Unit Instructions (0xA0-0xBF)**: Available on all DSP implementations
2. **Architecture-Specific Instructions (0xC0-0xCF)**: Specific to certain DSP architectures
3. **Model-Specific Instructions (0xD0-0xDF)**: Available only in specific DSP models

## Processing Unit Instructions (0xA0-0xBF)

### Signal Processing (0xA0-0xA7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA0   | FFT      | Fast Fourier Transform | 2-3 |
| 0xA1   | IFFT     | Inverse Fast Fourier Transform | 2-3 |
| 0xA2   | DCT      | Discrete Cosine Transform | 2-3 |
| 0xA3   | IDCT     | Inverse Discrete Cosine Transform | 2-3 |
| 0xA4   | FILTER   | Apply filter operation | 3-4 |
| 0xA5   | CONVOLVE | Convolution operation | 3-4 |
| 0xA6   | CORRELATE| Correlation operation | 3-4 |
| 0xA7   | WINDOW   | Apply window function | 3 |

### Fixed-Point Operations (0xA8-0xAF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xA8   | FXCONV   | Fixed-point conversion | 2-3 |
| 0xA9   | FXMUL    | Fixed-point multiplication | 3-4 |
| 0xAA   | FXMAC    | Fixed-point multiply-accumulate | 3-4 |
| 0xAB   | FXSAT    | Fixed-point saturation | 2-3 |
| 0xAC   | FXROUND  | Fixed-point rounding | 2-3 |
| 0xAD   | FXSCALE  | Fixed-point scaling | 3 |
| 0xAE   | FXDIV    | Fixed-point division | 3-4 |
| 0xAF   | FXNORM   | Fixed-point normalization | 2-3 |

### Vector Processing (0xB0-0xB7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB0   | VDOT     | Vector dot product | 3 |
| 0xB1   | VSUM     | Vector sum/accumulate | 2-3 |
| 0xB2   | VMUL     | Vector multiply | 3-4 |
| 0xB3   | VFIR     | Vector FIR filter | 3-4 |
| 0xB4   | VIIR     | Vector IIR filter | 3-5 |
| 0xB5   | VSCALE   | Vector scaling | 3 |
| 0xB6   | VSHIFT   | Vector shift operation | 3-4 |
| 0xB7   | VBLEND   | Vector blend operation | 4 |

### DSP Management (0xB8-0xBF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xB8   | DSPINFO  | Query DSP information | 2 |
| 0xB9   | DSPCAP   | Query DSP capabilities | 2 |
| 0xBA   | DSPMEM   | Query DSP memory | 2 |
| 0xBB   | DSPTYPE  | Query DSP type | 1 |
| 0xBC   | DSPPOWER | Set/get DSP power mode | 1-2 |
| 0xBD   | DSPLOAD  | Load DSP program | 2-3 |
| 0xBE   | DSPRUN   | Run DSP program | 2-3 |
| 0xBF   | DSPRESET | Reset DSP state | 0-1 |

## Architecture-Specific Instructions (0xC0-0xCF)

### TI C6x Architecture (0xC0-0xC3)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC0   | C6MAC    | TI C6x MAC operation | 3-4 |
| 0xC1   | C6PACK   | TI C6x pack operation | 3 |
| 0xC2   | C6UNPACK | TI C6x unpack operation | 3 |
| 0xC3   | C6SIMD   | TI C6x SIMD operation | 3-4 |

### ADI Sharc Architecture (0xC4-0xC7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC4   | SHARCMAC | Sharc MAC operation | 3-4 |
| 0xC5   | SHARCALU | Sharc ALU operation | 3-4 |
| 0xC6   | SHARCSHF | Sharc shifter operation | 3-4 |
| 0xC7   | SHARCMUL | Sharc multiplier operation | 3-4 |

### Ceva Architecture (0xC8-0xCB)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xC8   | CEVAMAC  | Ceva MAC operation | 3-4 |
| 0xC9   | CEVAALU  | Ceva ALU operation | 3-4 |
| 0xCA   | CEVAFFT  | Ceva FFT operation | 2-3 |
| 0xCB   | CEVAVEC  | Ceva vector operation | 3-4 |

## Model-Specific Instructions (0xD0-0xDF)

### Fixed-Point Model Instructions (0xD0-0xD7)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD0   | QFORMAT  | Set/get Q format | 1-2 |
| 0xD1   | QMUL     | Q format multiplication | 3-4 |
| 0xD2   | QDIV     | Q format division | 3-4 |
| 0xD3   | QCONV    | Q format conversion | 2-3 |
| 0xD4   | QMAC     | Q format multiply-accumulate | 3-5 |
| 0xD5   | QROUND   | Q format rounding | 2-3 |
| 0xD6   | QSAT     | Q format saturation control | 2-3 |
| 0xD7   | QSCALE   | Q format scaling | 3-4 |

### Vector/SIMD Model Instructions (0xD8-0xDF)

| Opcode | Mnemonic | Description | Operands |
|--------|----------|-------------|----------|
| 0xD8   | SIMDLOAD | SIMD load operation | 2-3 |
| 0xD9   | SIMDSTORE| SIMD store operation | 2-3 |
| 0xDA   | SIMDPACK | SIMD pack operation | 3-4 |
| 0xDB   | SIMDUNPACK| SIMD unpack operation | 3-4 |
| 0xDC   | SIMDMAC  | SIMD multiply-accumulate | 3-5 |
| 0xDD   | SIMDADD  | SIMD addition | 3 |
| 0xDE   | SIMDMUL  | SIMD multiplication | 3 |
| 0xDF   | SIMDSHIFT| SIMD shift operation | 3-4 |

## Detailed Instruction Behaviors

### Signal Processing Instructions

#### FFT (0xA0)
```
FFT dest, src [, options]
```
- Performs Fast Fourier Transform on source data
- First operand: Destination buffer for transformed data
- Second operand: Source data buffer
- Third operand (optional): Options including:
  - Direction (forward)
  - Normalization mode
  - Window function
  - Zero padding
- Flag effects: None

#### IFFT (0xA1)
```
IFFT dest, src [, options]
```
- Performs Inverse Fast Fourier Transform
- First operand: Destination buffer for time-domain data
- Second operand: Source frequency-domain data
- Third operand (optional): Options including:
  - Normalization mode
  - Window function
  - Zero padding
- Flag effects: None

### Fixed-Point Operations

#### FXMAC (0xAA)
```
FXMAC dest, a, b, [, format]
```
- Performs fixed-point multiply-accumulate
- First operand: Destination for result
- Second operand: First multiplicand
- Third operand: Second multiplicand
- Fourth operand (optional): Fixed-point format specification
- Accumulation semantics: `dest += a * b`
- Flag effects:
  - V: Set if overflow occurs
  - S: Set if result is negative
  - Z: Set if result is zero

## Memory Model

DSP operations follow a specialized memory model:

1. **Data Memory**: Primary memory for data processing
2. **Program Memory**: Stores DSP code and coefficients
3. **Unified Memory**: Combined data and program memory on some architectures
4. **Circular Buffers**: Hardware-accelerated circular addressing

## Directive-Based Targeting

To target DSP-specific features in code, use the preprocessor directives:

```
!if defined(PU_DSP)
  !target DSP
  ; DSP-specific code here
!endif
```

For specific DSP architectures:

```
!if defined(PU_DSP) && defined(ARCH_TI_C6X)
  !target DSP.TI_C6X
  ; TI C6x-specific code here
!endif
```

## Usage Examples

### FFT Processing Example

```
; Load data into DSP memory
DSPLOAD input_buffer, sample_data, { size: 1024 }

; Create output buffer
VAR output_buffer, array<complex<f32>, 1024>

; Perform FFT with Hanning window
FFT output_buffer, input_buffer, { window: HANNING, normalize: TRUE }

; Process frequency domain data
FILTER output_buffer, output_buffer, filter_coefficients

; Transform back to time domain
IFFT result_buffer, output_buffer

; Store results
DSPSTORE result_data, result_buffer
```

### Fixed-Point Filtering Example

```
!target DSP
!mode FIXED_POINT

; Q15 format (15 fractional bits) fixed-point data
QFORMAT Q15, { integer_bits: 1, fractional_bits: 15 }

; Define filter coefficients in Q15 format
VAR coeffs, array<q15, 32>
FXCONV coeffs, float_coeffs, Q15

; Apply FIR filter
VFIR output, input, coeffs, { saturation: TRUE }
```

### SIMD Processing Example

```
!target DSP
!mode VECTOR

; Load multiple channels at once
SIMDLOAD channels, audio_data, { channels: 4, samples: 256 }

; Process all channels simultaneously
SIMDMUL channels, channels, gain_factors
FILTER_ALL channels, channels, eq_filter

; Store processed data
SIMDSTORE output_buffer, channels
```