# TBD Overview

The TBD Instruction set is focused on being assembly+. Instead of these high level functional languages like C TBD works to bring cross architecture capabilities to the lowest level possible.

Important to note is TBD is built for high level processing units like CPUs, GPUs and TPUs which implement arithmetic, memory and branching. FPGAs and specialized processing units will require direct CBC usage.

## TBD Instruction Set Architecture (TBD-ISA)

TBD Instructions are type inferred similar to higher level languages. A full fledge type system will be built later into the TBD standard to help with optimizations relating to the higher level features like type inheritance, methods and virtual methoding.

#### Type System

##### Integral

**Type Opcodes**
```
I8 = 0x00  , U8 = 0x01
I16 = 0x02 , U16 = 0x03
I32 = 0x04 , U32 = 0x05
I64 = 0x06 , U64 = 0x07
I128 = 0x08, U128 = 0x09
I24 = 0x0A , U24 = 0xB
I48 = 0x0C , U24 = 0xD
INT = 0x0C , UNT = 0xD // fastest/default signed and unsigned integers
```

##### Floating

**Type Opcodes**
```c
// IEEE 754 Standard Floating Point Formats
struct binary16 {
  unsigned int sign : 1;
  unsigned int exponent : 5;
  unsigned int precision : 10;
} = 0x20;

struct binary32 {
  unsigned int sign : 1;
  unsigned int exponent : 8;
  unsigned int precision : 23;
} = 0x21;

struct binary64 {
  unsigned int sign : 1;
  unsigned int exponent : 11;
  unsigned int precision : 52;
} = 0x22;

struct binary128 {
  unsigned int sign : 1;
  unsigned int exponent : 15;
  unsigned int precision : 112;
} = 0x23;

struct binary256 {
  unsigned int sign : 1;
  unsigned int exponent : 19;
  unsigned int precision : 236;
} = 0x24;

// IEEE 754 Decimal Formats
struct decimal32 {
  unsigned int sign : 1;
  unsigned int combination : 11;  // Combined exponent and coefficient bits
  unsigned int coefficient : 20;
} = 0x25;

struct decimal64 {
  unsigned int sign : 1;
  unsigned int combination : 13;  // Combined exponent and coefficient bits
  unsigned int coefficient : 50;
} = 0x26;

struct decimal128 {
  unsigned int sign : 1;
  unsigned int combination : 17;  // Combined exponent and coefficient bits
  unsigned int coefficient : 110;
} = 0x27;

// AI/ML Era Formats

// Google Brain bfloat16
struct bfloat16 {
  unsigned int sign : 1;
  unsigned int exponent : 8;
  unsigned int precision : 7;
} = 0x28;

// NVIDIA TensorFloat-32 (stored as 32-bit, computed as reduced precision)
struct tensorfloat32 {
  unsigned int sign : 1;
  unsigned int exponent : 8;
  unsigned int precision : 10;
  unsigned int padding : 13;  // Unused bits in 32-bit storage
} = 0x29;

// 8-bit Floating Point Formats
struct fp8_e4m3 {
  unsigned int sign : 1;
  unsigned int exponent : 4;
  unsigned int precision : 3;
} = 0x2A;

struct fp8_e5m2 {
  unsigned int sign : 1;
  unsigned int exponent : 5;
  unsigned int precision : 2;
} = 0x2B;

// 6-bit Floating Point Formats
struct fp6_e3m2 {
  unsigned int sign : 1;
  unsigned int exponent : 3;
  unsigned int precision : 2;
} = 0x2C;

struct fp6_e2m3 {
  unsigned int sign : 1;
  unsigned int exponent : 2;
  unsigned int precision : 3;
} = 0x2D;

// 4-bit Floating Point Formats
struct fp4_e2m1 {
  unsigned int sign : 1;
  unsigned int exponent : 2;
  unsigned int precision : 1;
} = 0x2E;

struct nf4 {
  unsigned int value : 4;  // 4-bit normalized float (special encoding)
} = 0x2F;

// Microsoft Floating Point (MSFP) - Block Floating Point
struct msfp_element8 {
  unsigned int mantissa : 8;  // Integer mantissa, scaled by shared exponent
} = 0x00; // (parameter to msfp_block)

struct msfp_element12 {
  unsigned int mantissa : 12;  // Integer mantissa, scaled by shared exponent
} = 0x01;

struct msfp_element16 {
  unsigned int mantissa : 16;  // Integer mantissa, scaled by shared exponent
} = 0x02;

struct msfp_block {
  unsigned int shared_exponent : 8;  // Shared exponent for entire block
  // Followed by array of mantissa elements
} = 0x30;

// OCP MX (Microscaling) Formats - 32 elements per block
struct mx_scale_factor {
  unsigned int exponent : 8;  // Shared 8-bit scale factor
} = 0x31;

struct mxfp4_element {
  unsigned int sign : 1;
  unsigned int exponent : 2;
  unsigned int precision : 1;
} = 0x32;

struct mxfp6_e3m2_element {
  unsigned int sign : 1;
  unsigned int exponent : 3;
  unsigned int precision : 2;
} = 0x33;

struct mxfp6_e2m3_element {
  unsigned int sign : 1;
  unsigned int exponent : 2;
  unsigned int precision : 3;
} = 0x34;

struct mxfp8_e4m3_element {
  unsigned int sign : 1;
  unsigned int exponent : 4;
  unsigned int precision : 3;
} = 0x35;

struct mxfp8_e5m2_element {
  unsigned int sign : 1;
  unsigned int exponent : 5;
  unsigned int precision : 2;
} = 0x36;

struct mxint8_element {
  signed int value : 8;  // Signed integer scaled by shared exponent
} = 0x02;

// MX Block Structure (32 elements + 1 scale factor)
struct mx_block_fp4 {
  struct mx_scale_factor scale;
  struct mxfp4_element elements[32];
} = 0x37;

struct mx_block_fp6_e3m2 {
  struct mx_scale_factor scale;
  struct mxfp6_e3m2_element elements[32];
} = 0x38;

struct mx_block_fp6_e2m3 {
  struct mx_scale_factor scale;
  struct mxfp6_e2m3_element elements[32];
} = 0x39;

struct mx_block_fp8_e4m3 {
  struct mx_scale_factor scale;
  struct mxfp8_e4m3_element elements[32];
} = 0x3A;

struct mx_block_fp8_e5m2 {
  struct mx_scale_factor scale;
  struct mxfp8_e5m2_element elements[32];
} = 0x3B;

struct mx_block_int8 {
  struct mx_scale_factor scale;
  struct mxint8_element elements[32];
} = 0x3C;

// Historical Formats

// IBM Hexadecimal Floating Point
struct ibm_hfp_single {
  unsigned int sign : 1;
  unsigned int exponent : 7;      // Excess-64, base-16
  unsigned int fraction : 24;     // Hexadecimal fraction
} = 0x3D;

struct ibm_hfp_double {
  unsigned int sign : 1;
  unsigned int exponent : 7;      // Excess-64, base-16
  unsigned int fraction : 56;     // Hexadecimal fraction
} = 0x3E;

// IBM Extended HFP (two 64-bit words)
struct ibm_hfp_extended {
  struct ibm_hfp_double high_order;
  struct ibm_hfp_double low_order;   // Exponent is 14 less than high-order
} = 0x3F;

// DEC VAX Formats
struct vax_f_floating {
  unsigned int fraction_high : 7;   // VAX has unusual bit ordering
  unsigned int exponent : 8;        // Excess-128
  unsigned int sign : 1;
  unsigned int fraction_low : 16;
} = 0x40;

struct vax_d_floating {
  unsigned int fraction_high : 7;   // High 7 bits of 56-bit fraction
  unsigned int exponent : 8;        // Excess-128
  unsigned int sign : 1;
  unsigned int fraction_mid_high : 16;
  unsigned int fraction_mid_low : 16;
  unsigned int fraction_low : 16;
} = 0x41;

struct vax_g_floating {
  unsigned int fraction_high : 4;   // High 4 bits of 53-bit fraction
  unsigned int exponent : 11;       // Excess-1024
  unsigned int sign : 1;
  unsigned int fraction_mid_high : 16;
  unsigned int fraction_mid_low : 16;
  unsigned int fraction_low : 16;
} = 0x42;

struct vax_h_floating {
  unsigned int fraction_high : 15;  // Quad precision
  unsigned int exponent : 15;       // Excess-16384
  unsigned int sign : 1;
  unsigned int fraction_word2 : 16;
  unsigned int fraction_word3 : 16;
  unsigned int fraction_word4 : 16;
  unsigned int fraction_word5 : 16;
  unsigned int fraction_word6 : 16;
  unsigned int fraction_word7 : 16;
  unsigned int fraction_low : 16;
} = 0x43;

// Posit Formats (Variable-length regime and exponent)
struct posit8 {
  unsigned int sign : 1;
  unsigned int regime_and_exponent_and_fraction : 7;  // Variable layout
} = 0x44;

struct posit16 {
  unsigned int sign : 1;
  unsigned int regime_and_exponent_and_fraction : 15;  // Variable layout
} = 0x45;

struct posit32 {
  unsigned int sign : 1;
  unsigned int regime_and_exponent_and_fraction : 31;  // Variable layout
} = 0x46;

struct posit64 {
  unsigned int sign : 1;
  unsigned int regime_and_exponent_and_fraction : 63;  // Variable layout
} = 0x47;

// Unum Type I (Variable width)
struct unum_type1 {
  // Variable width structure - cannot be represented as fixed bit fields
  // Conceptual layout:
  // [sign][exponent of variable size][fraction of variable size][ubit][exponent_size][fraction_size]
  unsigned int data[];  // Variable-length data
} = 0x48;

// ARM Alternative Half-Precision (no infinity/NaN encoding)
struct arm_alt_half {
  unsigned int sign : 1;
  unsigned int exponent : 5;      // No special values (all patterns valid)
  unsigned int precision : 10;
} = 0x49;
```

##### High Level Types

Vector, Matricies and Tensors may be optimized to hardware accelerated types. 
N is the length of elements.
List is a linked list, Map is a 

**Type Opcodes**
```
Vec<T>
Vec<T, N>
Mat<T>
Mat<T, Nx, Ny>
Mat3D<T>
Mat3D<T, Nx, Ny, Nz>
Tensor<T>
Tensor<T, N...>
List<N>
Map<Tkey, Tvalue, FHasher>
Dict<TKey, FHasher>
```

##### Special

**Type Opcodes**
```
FLOAT   = 0xEA,  ///< Default float (normally 32 bit)
PTR     = 0xEB,  ///< Pointer type (platform width)
SIZE    = 0xEC,  ///< Unsigned type (platform width)
SSIZE   = 0xED,  ///< Signed type (platform width)
UINTPTR = 0xEE,  ///< Pointer-sized unsigned integer
INTPTR  = 0xEF,  ///< Pointer-sized signed integer
VAR   = 0xF0,    ///< Value is u64 Variable ID
SYM   = 0xF1,    ///< Value is u64 symbol reference
EXP   = 0xF2,    ///< Value is u64 Expression ID
REG   = 0xF3,    ///< Value is u32 register reference
STR   = 0xF4,    ///< Value is u64 string table offset
LABEL = 0xF5,    ///< Value is u64 label reference
FUNC  = 0xF6,    ///< Value is u64 function reference
FUNCPTR = 0xF7,  ///< Value is a function pointer
GLOBAL  = 0xF8,  ///< Value is u64 global value reference
THREAD  = 0xF9,  ///< Value is u64 thread value reference
STACK   = 0xFA,  ///< Value is the stack pointer
BIT = 0xFD       ///< Value is can be optimized to 1 bit slots when available
AUTO = 0xFE      ///< Type should be inferred (only when type is explicity known)
VOID = 0xFF      ///< Type is unknown and not important
```

#### Type Extensions

```
VOLATILE = 1 << 0 // value will not be optimized
CONSTANT = 1 << 1 // value will not be modified
ATOMIC   = 1 << 2 // all operations will attempt to be synchronized
REGISTR  = 1 << 3 // hint to store on register (hot variable)
```

#### Opcode

##### Control FLow (0x00-0x0F)
```c++
template <T>
typedef simple_containers_t = [PTR<T>, UINTPTR<T>, INTPTR<T>, VAR<T>, SYM<T>, EXP<T>, REG<T>, STR<T>, LABEL<T>, GLOBAL<T>, THREAD<T>];

typedef simple_func_t = [ FUNC,FUNCPTR,PTR<self>,UINTPTR<self>,INTPTR<self> ];
typedef func_t = [simple_func_t..., ];

typedef simple_len_t = [ SIZE, U*, UINTPTR ];
typedef len_t = [ simple_len_t..., simple_containers_t<simple_len_t...>... ];

typedef simple_target_t = [FUNCPTR, FUNC, LABEL<self>, STR<self>, SYM<self>, INTPTR<self>, UINTPTR<self>, SSIZE, SIZE, PTR<self>, I*, U*];
typedef target_t = [simple_target_t..., simple_containers_t<simple_target_t...>...];

template <T>
typedef md_containers_t = [Vec<T>, Vec<T, N>, Mat<T>, Mat<T, Nx, Ny>, Mat3D<T>, Mat3D<T, Nx, Ny, Nz>, Tensor<T>, Tensor<T, N...>, List<N>, Map<Tkey, Tvalue, func_t>, Dict<TKey, func_t>];

template <T>
typedef all_containers = [simple_containers_t<T>..., md_containers_t<T>...];

typedef simple_any_t = [
  BIT, THREAD, GLOBAL, FUNCPTR, FUNC, LABEL<self>, STR<self>, REG<self>, EXP<self>, SYM<self>, VAR<self>, INTPTR<self>, UINTPTR<self>, SSIZE, SIZE, PTR<self>, I*, U*, F*
];
// stack is in any_t because arithmetic and memory operations can be completed on the stack pointer
typedef any_t = [
  simple_any_t..., all_containers<simple_any_t...>..., void, STACK
];

typedef dest_t = [ target_t...,  ];
```

```
NOP { bytes: u8 }
BR  { target: target_t, condition: u8, leftsrc: any, rightsrc: opt<any> }
CALL { target: target_t }
RET { void }
```

##### Memory (0x10-0x1F)

MOV destination has to be a container or target memory address
MOV source can by anything

```
MOV  { dest: [simple_containers_t<any_t...>..., target_t...], src: any_t }
SCOPE
SCOPL
```

##### Arithmetic (0x20-0x4F)

##### Logical (0x50-0x5F)

##### MultiDimensional (0x60-0x6F)

##### Cryptography and Pseudo-Random (0x70-0x7F)

##### Reserved (0x80-0xCF)

##### Type (0xD0-0xDF)

##### Directives (0xE0-0xFF)

#### Opcode Extensions

```
VOLATILE = 1 << 0 // instruction will not be optimized
CONDITIONAL = 1 << 1, // instruction is extended with the three arguments{condition: u8, leftsrc: any, rightsrc: any} to control execution of the single instruction.
INLINE = 1 << 2 // Attempt to work out instruction at compile time
```

#### Instruction Formatting

#### Encoding

#### Decoding

