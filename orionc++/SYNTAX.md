# Orion++ Human Syntax Reference

This document provides a comprehensive reference for the Orion++ Human syntax, based on the examples and features demonstrated in the accompanying `.hopp` files.

## Table of Contents

- [Module System](#module-system)
- [Types](#types)
- [Variables](#variables)
- [Functions](#functions)
- [Control Flow](#control-flow)
- [Expressions](#expressions)
- [Comments](#comments)
- [Keywords Reference](#keywords-reference)

## Module System

### Module Declaration
### Loop Control
```orionpp
break;      // Exit loop
continue;   // Skip to next iteration

// Labeled breaks for nested loops
outer: for (i: int = 0; i < 10; i = i + 1) {
    inner: for (j: int = 0; j < 10; j = j + 1) {
        if (condition) {
            break outer;  // Break out of outer loop
        }
        if (otherCondition) {
            continue outer;  // Continue outer loop
        }
    }
}
```

## Design Philosophy and Compilation Targets

### Universal Language Support
Orion++ Human is designed as a universal intermediate representation that can serve as a compilation target for any programming language, from low-level systems languages like C and Rust to high-level dynamic languages like Python and JavaScript.

#### Key Design Principles:
1. **Compile-time vs Runtime Clarity**: Clear separation between operations that happen at compile time vs runtime
2. **Native vs Emulated Types**: Explicit distinction between machine-native types and software-emulated types
3. **Multi-paradigm Support**: Features from functional, object-oriented, procedural, and concurrent programming paradigms
4. **Optimization Opportunities**: Rich semantic information for compiler optimizations
5. **Human Readability**: Remains readable and debuggable while serving as an IR

### Compilation Pipeline
```
High-Level Language → Orion++ Human (.hopp) → Orion++ Binary (.orion) → Target Machine Code
```

#### Language Frontend Examples:
- **C/C++**: Direct mapping of constructs with compile-time template expansion
- **Python**: Dynamic typing support with runtime type checking
- **JavaScript**: Async/await, closures, and prototype-based inheritance
- **Rust**: Ownership, borrowing, and zero-cost abstractions
- **Go**: Goroutines, channels, and interface-based polymorphism
- **Java/C#**: Object-oriented features with generics and exceptions
- **Haskell**: Functional programming with lazy evaluation
- **Assembly**: Direct instruction mapping for low-level control

#### Optimization Opportunities:
- **Compile-time Evaluation**: `comptime` blocks and const functions
- **Native Type Selection**: Automatic selection of optimal native types
- **Dead Code Elimination**: Unused paths in `comptime if` statements
- **Loop Unrolling**: `comptime for` loops with known bounds
- **Specialization**: Generic functions specialized for specific types
- **Inlining**: Small functions and compile-time constants

### Target Architecture Support
The intermediate representation is designed to compile efficiently to various target architectures while maintaining semantic correctness:

#### Supported Targets:
- **x86/x86_64**: Intel/AMD processors with SSE/AVX support
- **ARM/AArch64**: ARM processors with NEON support  
- **RISC-V**: Open-source RISC instruction set
- **WebAssembly**: Browser and server-side execution
- **GPU**: CUDA, OpenCL, and compute shaders
- **FPGA**: Hardware description language generation
- **Custom**: Domain-specific processors and accelerators

#### Cross-Platform Features:
- **Endianness Handling**: Automatic byte order conversion
- **ABI Compliance**: Proper calling conventions for each platform
- **Vector Operations**: Map to platform-specific SIMD instructions
- **Atomic Operations**: Platform-appropriate memory ordering
- **Threading**: Map to platform threading primitives

### Future Extensions
The syntax and semantic model are designed to accommodate future language features and computing paradigms:

- **Quantum Computing**: Quantum gates and superposition states
- **Neural Networks**: Differentiable programming constructs
- **Distributed Computing**: Message passing and remote procedure calls
- **Real-time Systems**: Timing constraints and deterministic execution
- **Safety-Critical**: Formal verification and proof obligations

This comprehensive approach ensures that Orion++ Human can serve as a universal intermediate representation for the diverse landscape of programming languages while maintaining efficiency and semantic richness.orionpp
module ModuleName {
    // Module contents
}
```

### Imports
```orionpp
import ModuleName;                              // Import entire module
import ModuleName from "./path/to/module.hopp"; // Import from specific path
import { Type1, Type2 } from ModuleName;        // Import specific items
import ModuleName as Alias;                     // Import with alias
import { item as alias } from ModuleName;       // Import item with alias
```

### Exports
```orionpp
export let CONSTANT: int = 42;         // Export constant
export func functionName() { ... }     // Export function
export struct TypeName { ... }         // Export type
export { item1, item2 };              // Export multiple items
export item as alias;                  // Export with alias
```

### Nested Modules
```orionpp
module Parent {
    module Child {
        // Nested module contents
        export func childFunction() { ... }
    }
    
    // Access nested module
    Child.childFunction();
}
```

## Types

### Primitive Types

#### Integer Types

##### Static Integer Types (Fixed Bit Width)
```orionpp
// Signed integers
i8        // 8-bit signed integer (-128 to 127)
i16       // 16-bit signed integer (-32,768 to 32,767)
i32       // 32-bit signed integer (-2^31 to 2^31-1)
i64       // 64-bit signed integer (-2^63 to 2^63-1)
i128      // 128-bit signed integer
isize     // Pointer-sized signed integer (platform dependent)

// Unsigned integers
u8        // 8-bit unsigned integer (0 to 255)
u16       // 16-bit unsigned integer (0 to 65,535)
u32       // 32-bit unsigned integer (0 to 2^32-1)
u64       // 64-bit unsigned integer (0 to 2^64-1)
u128      // 128-bit unsigned integer
usize     // Pointer-sized unsigned integer (platform dependent)

// Arbitrary precision static integers
i<N>      // N-bit signed integer (compile-time constant N)
u<N>      // N-bit unsigned integer (compile-time constant N)

// Examples:
i<24>     // 24-bit signed integer
u<12>     // 12-bit unsigned integer
i<256>    // 256-bit signed integer
```

##### Dynamic Integer Types (Runtime Bit Width)
```orionpp
// Dynamic precision integers (bit width determined at runtime)
int<?N>   // Signed integer with runtime bit width N
uint<?N>  // Unsigned integer with runtime bit width N

// Bounded dynamic integers
int<?N, min, max>    // Dynamic signed integer with value bounds
uint<?N, max>        // Dynamic unsigned integer with maximum value

// Examples:
int<?32>             // Runtime-sized signed integer, up to 32 bits
uint<?64>            // Runtime-sized unsigned integer, up to 64 bits
int<?256, -1000, 1000>  // Dynamic signed integer, bounded values
```

##### Legacy Compatibility Types
```orionpp
int       // Default signed integer (typically i32)
uint      // Default unsigned integer (typically u32)
short     // Short integer (typically i16)
long      // Long integer (typically i64)
```

#### Floating-Point Types

##### Standard IEEE 754 Types
```orionpp
// IEEE 754 standard formats
f16       // Half precision (1 sign + 5 exp + 10 mantissa)
f32       // Single precision (1 sign + 8 exp + 23 mantissa)
f64       // Double precision (1 sign + 11 exp + 52 mantissa)
f128      // Quadruple precision (1 sign + 15 exp + 112 mantissa)

// Extended precision
f80       // x87 extended precision (1 sign + 15 exp + 64 mantissa)
```

##### Custom Floating-Point Formats
```orionpp
// Custom format: float<mantissa_bits, exponent_bits>
float<M, E>          // Custom float with M mantissa bits, E exponent bits

// Examples:
float<7, 8>          // bfloat16 format (Google Brain Float)
float<10, 5>         // Standard f16 format
float<23, 8>         // Standard f32 format
float<52, 11>        // Standard f64 format
float<112, 15>       // Standard f128 format

// Non-standard formats
float<19, 8>         // Custom 28-bit float
float<42, 10>        // Custom 53-bit float
```

##### Dynamic Floating-Point Types
```orionpp
// Runtime precision floats
float<?M, ?E>        // Dynamic mantissa and exponent precision
float<?M, E>         // Dynamic mantissa, fixed exponent
float<M, ?E>         // Fixed mantissa, dynamic exponent

// Bounded dynamic floats
float<?M, ?E, precision>  // Dynamic with minimum precision guarantee

// Examples:
float<?23, ?8>       // Runtime configurable precision
float<?52, 11>       // Dynamic mantissa, fixed double exponent
float<23, ?8>        // Fixed single mantissa, dynamic exponent
```

##### Decimal Floating-Point Types
```orionpp
// IEEE 754 decimal formats
decimal32            // 32-bit decimal float (7 decimal digits)
decimal64            // 64-bit decimal float (16 decimal digits)  
decimal128           // 128-bit decimal float (34 decimal digits)

// Custom decimal precision
decimal<D>           // D decimal digits precision
decimal<D, E>        // D digits precision, E exponent range

// Examples:
decimal<10>          // 10 decimal digits
decimal<15, 3>       // 15 digits, 3-digit exponent
```

##### Specialized Numeric Types
```orionpp
// Fixed-point arithmetic
fixed<I, F>          // Fixed-point: I integer bits, F fractional bits
ufixed<I, F>         // Unsigned fixed-point

// Examples:
fixed<16, 16>        // Q16.16 fixed-point
ufixed<8, 24>        // Unsigned 8.24 fixed-point

// Rational numbers
rational<N>          // Rational number with N-bit numerator/denominator
rational<?>          // Dynamic precision rational

// Complex numbers
complex<T>           // Complex number with component type T
complex<f32>         // Complex with f32 components
complex<f64>         // Complex with f64 components
```

#### Other Primitive Types
```orionpp
bool      // Boolean (true/false)
char      // Unicode character (UTF-32 code point)
char8     // UTF-8 code unit
char16    // UTF-16 code unit  
char32    // UTF-32 code unit (same as char)
string    // UTF-8 string
```

#### Type Constraints and Validation

```orionpp
// Constrained integer types
typedef SmallInt = i32 where value >= -1000 && value <= 1000;
typedef PositiveInt = uint<?> where value > 0;
typedef PowerOfTwo = u64 where isPowerOfTwo(value);

// Constrained floating-point types
typedef UnitFloat = f64 where value >= 0.0 && value <= 1.0;
typedef NonZeroFloat = float<?> where value != 0.0;

// Bit width constraints
typedef SafeInt<N> = i<N> where N >= 8 && N <= 128 && N % 8 == 0;
typedef EfficientFloat<M, E> = float<M, E> where M + E + 1 <= 64;

// Runtime validation
func validateRange<T>(value: T, min: T, max: T) -> T? 
    where T: Numeric 
{
    return if (value >= min && value <= max) value else null;
}
```

#### Type Conversion and Casting

```orionpp
// Explicit casting between different precisions
let narrow: i16 = cast<i16>(wideValue);      // May truncate
let widen: i64 = cast<i64>(narrowValue);     // Safe widening

// Dynamic precision conversion
let dynamicInt: int<?32> = cast<int<?32>>(staticInt);
let staticInt: i32 = cast<i32>(dynamicInt);  // Runtime validation

// Float format conversion
let single: f32 = cast<f32>(doubleValue);
let custom: float<19, 8> = cast<float<19, 8>>(standardFloat);

// Safe conversion with error handling
let safeConvert: Result<i16, ConversionError> = tryCast<i16>(largeValue);

// Precision-preserving conversion
let preserved: float<23, 8> = preserveCast<float<23, 8>>(f32Value);
```

#### Type Inference with Precision

```orionpp
// Automatic precision inference
let inferredInt = 42;                    // Inferred as i32 (default)
let inferredBig = 9223372036854775807;   // Inferred as i64 (too big for i32)
let inferredFloat = 3.14;                // Inferred as f64 (default)
let inferredHalf = 3.14h;                // Explicit f16 suffix

// Context-dependent inference
func processNumber<T: Numeric>(value: T) -> T { return value; }
let contextual = processNumber(42);      // T inferred from usage

// Minimum precision inference
let minPrec = computeMinPrecision(values);  // Returns minimum viable type
```

#### Numeric Literal Syntax

##### Integer Literals
```orionpp
// Decimal literals with type suffixes
42i8          // 8-bit signed integer
255u8         // 8-bit unsigned integer
1000i16       // 16-bit signed integer
65535u16      // 16-bit unsigned integer
1000000i32    // 32-bit signed integer
4294967295u32 // 32-bit unsigned integer
1000000000i64 // 64-bit signed integer

// Custom bit width literals
42i<24>       // 24-bit signed integer literal
255u<12>      // 12-bit unsigned integer literal

// Different bases
0b1010_1010u8     // Binary literal (170 in u8)
0o777i16          // Octal literal (511 in i16)
0xFF_FF_FF_FFu32  // Hexadecimal literal (max u32)

// Large integer literals with separators
1_000_000_000i64         // Underscore separators
0x1234_5678_9ABC_DEF0u64 // Hex with separators
```

##### Floating-Point Literals
```orionpp
// Standard float literals with suffixes
3.14f16       // Half-precision literal
3.14159f32    // Single-precision literal  
3.14159265f64 // Double-precision literal
3.14f128      // Quad-precision literal

// Custom format literals
3.14<19,8>    // Custom float<19,8> literal
2.71828<23,8> // Another custom format

// Scientific notation
1.23e-4f32    // 0.000123 in f32
6.022e23f64   // Avogadro's number in f64
1.0e308f64    // Large double

// Hexadecimal float literals (exact representation)
0x1.23p4f32   // Hexadecimal float: 1.23 * 2^4 = 18.1875
0x1.Fp-1f64   // Hexadecimal float: 1.9375 * 2^-1 = 0.96875

// Decimal float literals
123.45d32     // 32-bit decimal float
999.999d64    // 64-bit decimal float
3.14159d<15>  // 15-digit decimal precision
```

##### Fixed-Point Literals
```orionpp
// Fixed-point literals: value<integer_bits,fractional_bits>
0.5fixed<16,16>     // Q16.16 fixed-point
1.25ufixed<8,8>     // Unsigned 8.8 fixed-point
-2.75fixed<4,4>     // Q4.4 fixed-point

// Alternative syntax
0.5q16.16           // Q notation
1.25uq8.8           // Unsigned Q notation
```

##### Complex Number Literals
```orionpp
// Complex literals: real + imaginary*i
3.0 + 4.0i          // Complex with f64 components
1.5f32 + 2.5f32i    // Complex with f32 components
0 + 1i              // Pure imaginary (f64)
5.0 + 0i            // Real number as complex

// Alternative complex syntax
complex(3.0, 4.0)   // Function-style constructor
3.0 +: 4.0          // Operator-style (alternative)
```

##### Rational Number Literals
```orionpp
// Rational literals: numerator/denominator
22/7r               // Rational approximation of π
1/3r64              // Rational with 64-bit components
-355/113r           // Negative rational
1r                  // Rational integer (1/1)

// Exact decimal to rational conversion
0.333...r           // Repeating decimal to rational
0.(142857)r         // Repeating decimal notation
```

##### Special Numeric Values
```orionpp
// Infinity and NaN for floating-point
inf f32             // Positive infinity
-inf f64            // Negative infinity
nan f32             // Not a Number
nan<payload> f64    // NaN with payload

// Maximum and minimum values
i32.MAX             // Maximum i32 value
i32.MIN             // Minimum i32 value
f64.EPSILON         // Machine epsilon
f32.PI              // π constant
f64.E               // Euler's number
```

#### Native vs Emulated Type System

```orionpp
// Type classification based on machine capabilities
trait NativeType {
    // Types that map directly to machine instructions
    const IS_NATIVE: bool = true;
    const USES_INTRINSICS: bool = true;
}

trait EmulatedType {
    // Types that require software emulation
    const IS_NATIVE: bool = false; 
    const USES_INTRINSICS: bool = false;
    const EMULATION_STRATEGY: EmulationMethod;
}

// Machine-native integer types (use CPU instructions directly)
impl NativeType for i8, u8, i16, u16, i32, u32 {
    // These map to native CPU registers and ALU operations
    func add(self, other: Self) -> Self {
        return intrinsic_add(self, other);  // Direct CPU instruction
    }
    
    func multiply(self, other: Self) -> Self {
        return intrinsic_mul(self, other);  // Direct CPU instruction
    }
}

// Platform-dependent native types
impl NativeType for i64, u64 {
    // Native on 64-bit machines, may be emulated on 32-bit
    func add(self, other: Self) -> Self {
        if (TARGET_POINTER_WIDTH == 64) {
            return intrinsic_add64(self, other);  // Native on 64-bit
        } else {
            return emulated_add64(self, other);   // Emulated on 32-bit
        }
    }
}

// Always emulated types (require software implementation)
impl EmulatedType for i128, u128 {
    const EMULATION_STRATEGY: EmulationMethod = BigInteger;
    
    func add(self, other: Self) -> Self {
        // Multi-precision arithmetic using smaller native types
        return big_int_add(self.words, other.words);
    }
    
    func multiply(self, other: Self) -> Self {
        // Karatsuba or schoolbook multiplication
        return big_int_multiply(self.words, other.words);
    }
}

// Custom bit-width types (always emulated)
impl EmulatedType for i<N>, u<N> where N != 8, 16, 32, 64 {
    const EMULATION_STRATEGY: EmulationMethod = BitManipulation;
    
    func add(self, other: Self) -> Self {
        // Pack into larger native type, operate, then mask
        let native_size = next_power_of_2(N);
        let a_extended = self.signExtendTo(native_size);
        let b_extended = other.signExtendTo(native_size);
        let result = a_extended + b_extended;
        return result.truncateTo(N);
    }
}

// Dynamic precision types (always emulated, runtime allocation)
impl EmulatedType for int<?>, uint<?> {
    const EMULATION_STRATEGY: EmulationMethod = DynamicBigInteger;
    
    func add(self, other: Self) -> Self {
        // Runtime dispatch based on actual bit width
        if (self.bitWidth <= 64 && other.bitWidth <= 64) {
            return fast_add_small(self, other);
        } else {
            return big_int_add_dynamic(self, other);
        }
    }
}
```

#### Numeric Traits and Type Classes (Enhanced)

```orionpp
// Core numeric traits with native/emulated awareness
trait Numeric {
    const IS_NATIVE: bool;
    const BIT_WIDTH: usize;
    const IS_SIGNED: bool;
    
    func zero() -> Self;
    func one() -> Self;
    func add(self, other: Self) -> Self;
    func subtract(self, other: Self) -> Self;
    func multiply(self, other: Self) -> Self;
    
    // Performance characteristics
    func addCost() -> CostModel;      // O(1) for native, O(n) for emulated
    func multiplyCost() -> CostModel; // Varies by implementation
}

trait Integer: Numeric {
    func divide(self, other: Self) -> Self;
    func modulo(self, other: Self) -> Self;
    func bitAnd(self, other: Self) -> Self;
    func bitOr(self, other: Self) -> Self;
    func bitXor(self, other: Self) -> Self;
    func bitNot(self) -> Self;
    func shiftLeft(self, positions: usize) -> Self;
    func shiftRight(self, positions: usize) -> Self;
    
    // Native vs emulated optimizations
    func fastPath(self) -> bool {
        return Self.IS_NATIVE && Self.BIT_WIDTH <= TARGET_REGISTER_SIZE;
    }
}

trait Float: Numeric {
    const MANTISSA_BITS: usize;
    const EXPONENT_BITS: usize;
    const IS_IEEE754: bool;
    
    func divide(self, other: Self) -> Self;
    func power(self, exponent: Self) -> Self;
    func sqrt(self) -> Self;
    func isNaN(self) -> bool;
    func isInfinite(self) -> bool;
    func isFinite(self) -> bool;
    
    // Hardware support detection
    func hasNativeSqrt() -> bool {
        return Self.IS_NATIVE && TARGET_HAS_FPU;
    }
}

trait FixedPrecision: Numeric {
    const BIT_WIDTH: usize;
    
    func maxValue() -> Self;
    func minValue() -> Self;
    func canOverflow(a: Self, b: Self, op: Operation) -> bool;
}

trait DynamicPrecision: Numeric {
    func getBitWidth(self) -> usize;
    func withPrecision(precision: usize) -> Self;
    func canRepresent(value: Self) -> bool;
    func growPrecision(self, newPrecision: usize) -> Self;
    func shrinkPrecision(self, newPrecision: usize) -> Result<Self, PrecisionError>;
}

trait NativeIntegerOps: Integer + FixedPrecision {
    // Operations that map directly to CPU instructions
    func nativeAdd(self, other: Self) -> Self {
        return intrinsic_add(self, other);
    }
    
    func nativeSubtract(self, other: Self) -> Self {
        return intrinsic_sub(self, other);
    }
    
    func nativeMultiply(self, other: Self) -> Self {
        return intrinsic_mul(self, other);
    }
    
    func nativeDivide(self, other: Self) -> Self {
        return intrinsic_div(self, other);
    }
}

trait EmulatedIntegerOps: Integer {
    // Operations that require software implementation
    func emulatedAdd(self, other: Self) -> Self;
    func emulatedMultiply(self, other: Self) -> Self;
    func emulatedDivide(self, other: Self) -> Self;
    
    // Optimization hints
    func preferredChunkSize() -> usize;
    func canUseNativeChunks() -> bool;
}

// Implementation examples
impl NativeIntegerOps for i32 {
    // Direct mapping to CPU instructions
    func add(self, other: Self) -> Self {
        return self.nativeAdd(other);  // Single CPU instruction
    }
}

impl EmulatedIntegerOps for i128 {
    func add(self, other: Self) -> Self {
        // Split into 64-bit chunks, add with carry
        let (low_a, high_a) = self.split64();
        let (low_b, high_b) = other.split64();
        
        let (low_result, carry) = low_a.addWithCarry(low_b);
        let high_result = high_a + high_b + cast<i64>(carry);
        
        return Self.combine64(high_result, low_result);
    }
    
    func multiply(self, other: Self) -> Self {
        // Karatsuba multiplication for better complexity
        if (self.bitWidth() < 64 && other.bitWidth() < 64) {
            // Use native 64-bit multiplication
            return cast<Self>(cast<i64>(self) * cast<i64>(other));
        } else {
            return self.karatsubaMultiply(other);
        }
    }
}

impl DynamicPrecision for int<?> {
    func add(self, other: Self) -> Self {
        let result_precision = max(self.getBitWidth(), other.getBitWidth()) + 1;
        
        // Choose implementation based on size
        if (result_precision <= 64) {
            return self.addAsNative64(other);
        } else {
            return self.addAsBigInteger(other);
        }
    }
    
    func growPrecision(self, newPrecision: usize) -> Self {
        if (newPrecision <= self.getBitWidth()) {
            return self;  // No change needed
        }
        
        // Reallocate with larger precision
        return Self.withPrecision(newPrecision).copyFrom(self);
    }
}
```

#### Compile-time Type Selection and Optimization

```orionpp
// Compile-time selection of optimal implementation
func optimizedAdd<T: Integer>(a: T, b: T) -> T {
    comptime if (T.IS_NATIVE) {
        // Use native CPU instructions
        return a.nativeAdd(b);
    } else if (T.BIT_WIDTH <= 128) {
        // Use optimized multi-precision for small custom sizes
        return a.smallEmulatedAdd(b);
    } else {
        // Use general big-integer arithmetic
        return a.bigIntegerAdd(b);
    }
}

// Target-specific optimization
func platformOptimizedMultiply<T: Integer>(a: T, b: T) -> T {
    comptime match (TARGET_ARCHITECTURE, T.BIT_WIDTH) {
        case ("x86_64", 64):
            return a.x86_64_multiply(b);     // Use x86-64 MUL instruction
        case ("aarch64", 64):
            return a.aarch64_multiply(b);    // Use AArch64 MUL instruction
        case ("riscv64", 64):
            return a.riscv_multiply(b);      // Use RISC-V MUL instruction
        case (_, width) if width <= 32:
            return a.generic32BitMultiply(b); // Generic 32-bit implementation
        default:
            return a.genericBigIntMultiply(b); // Fallback big-integer
    }
}

// Runtime adaptation for dynamic types
func adaptiveOperation<T: DynamicPrecision>(a: T, b: T, op: Operation) -> T {
    let maxBitWidth = max(a.getBitWidth(), b.getBitWidth());
    
    // Switch implementation based on runtime size
    return switch (maxBitWidth) {
        case w if w <= 32:
            op.executeNative32(a, b);
        case w if w <= 64:  
            op.executeNative64(a, b);
        case w if w <= 128:
            op.executeEmulated128(a, b);
        default:
            op.executeBigInteger(a, b);
    };
}
```

#### Advanced Numeric Patterns

```orionpp
// Generic numeric constraints
func numericOperation<T>(a: T, b: T) -> T 
    where T: Integer + FixedPrecision + Signed
{
    return a + b;
}

// Precision-aware algorithms
func adaptivePrecisionSum(values: [Numeric]) -> Numeric {
    let requiredPrecision = analyzeRequiredPrecision(values);
    match (requiredPrecision) {
        case precision if precision <= 32:
            return sumAs<f32>(values);
        case precision if precision <= 64:
            return sumAs<f64>(values);
        default:
            return sumAs<decimal128>(values);
    }
}

// Overflow-safe arithmetic
func safeAdd<T: Integer>(a: T, b: T) -> Result<T, OverflowError> {
    if (willOverflow(a, b)) {
        return Err(OverflowError);
    }
    return Ok(a + b);
}

// Precision-preserving operations
func preciseDivision(a: decimal128, b: decimal128) -> decimal128 {
    return a / b;  // Maintains decimal precision
}

// Cross-precision operations
func mixedPrecisionAdd<T1, T2, R>(a: T1, b: T2) -> R
    where T1: Numeric, T2: Numeric, R: Numeric + From<T1> + From<T2>
{
    return R.from(a) + R.from(b);
}

// Compile-time precision selection
func selectPrecision<T>() -> Type
    where T: Numeric
{
    return match (T.BIT_WIDTH) {
        case w if w <= 32: f32,
        case w if w <= 64: f64,
        default: decimal128
    };
}
```

#### Practical Usage Examples

```orionpp
// Financial calculations with exact decimal arithmetic
module Finance {
    type Money = decimal64;
    type Rate = decimal<10>;  // 10 decimal places for rates
    
    func calculateInterest(principal: Money, rate: Rate, time: u32) -> Money {
        return principal * rate * cast<Money>(time);
    }
    
    func compoundInterest(principal: Money, rate: Rate, periods: u32) -> Money {
        let compound = (1.0d64 + rate).power(cast<decimal64>(periods));
        return principal * compound;
    }
}

// Graphics with fixed-point arithmetic
module Graphics {
    type Coordinate = fixed<16, 16>;  // Q16.16 for sub-pixel precision
    type Color = u8;
    
    struct Point {
        x: Coordinate;
        y: Coordinate;
    }
    
    func interpolate(p1: Point, p2: Point, t: Coordinate) -> Point {
        return Point {
            x: p1.x + (p2.x - p1.x) * t,
            y: p1.y + (p2.y - p1.y) * t
        };
    }
}

// Scientific computing with high precision
module Science {
    type HighPrecision = decimal128;
    type Vector3 = [HighPrecision; 3];
    
    func calculateDistance(a: Vector3, b: Vector3) -> HighPrecision {
        let dx = a[0] - b[0];
        let dy = a[1] - b[1];
        let dz = a[2] - b[2];
        return (dx*dx + dy*dy + dz*dz).sqrt();
    }
}

// Signal processing with complex numbers
module SignalProcessing {
    type Sample = complex<f32>;
    type Spectrum = [Sample];
    
    func dft(signal: [f32]) -> Spectrum {
        let N = signal.length;
        var spectrum: Spectrum = [];
        
        for (k: usize = 0; k < N; k = k + 1) {
            var sum: Sample = 0.0 + 0.0i;
            for (n: usize = 0; n < N; n = n + 1) {
                let angle = -2.0 * f32.PI * cast<f32>(k * n) / cast<f32>(N);
                let twiddle = angle.cos() + angle.sin() * 1i;
                sum = sum + signal[n] * twiddle;
            }
            spectrum.push(sum);
        }
        
        return spectrum;
    }
}

// Adaptive precision for iterative algorithms
module NumericalMethods {
    func adaptiveNewtonRaphson<T: Float + DynamicPrecision>(
        f: func(T) -> T,
        df: func(T) -> T,
        x0: T,
        tolerance: T
    ) -> T {
        var x = x0;
        var precision = T.getBitWidth(x);
        
        while (true) {
            let fx = f(x);
            let dfx = df(x);
            
            if (fx.abs() < tolerance) {
                break;
            }
            
            // Increase precision if convergence is slow
            if (fx.abs() > tolerance * 100.0) {
                precision = precision * 2;
                x = x.withPrecision(precision);
            }
            
            x = x - fx / dfx;
        }
        
        return x;
    }
}

// Cryptocurrency with arbitrary precision integers
module Crypto {
    type BigInt = int<?256>;  // 256-bit integers for cryptographic operations
    type Hash = u<256>;       // 256-bit hash values
    
    func modularExponentiation(base: BigInt, exponent: BigInt, modulus: BigInt) -> BigInt {
        var result: BigInt = 1;
        var base_mod = base % modulus;
        var exp = exponent;
        
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base_mod) % modulus;
            }
            exp = exp >> 1;
            base_mod = (base_mod * base_mod) % modulus;
        }
        
        return result;
    }
}
```

### Composite Types

#### Structs
```orionpp
struct Point2D {
    x: float;
    y: float;
    
    // Methods within struct
    func distance(self) -> float {
        return sqrt(self.x * self.x + self.y * self.y);
    }
}
```

#### Enums
```orionpp
// Simple enum
enum Color {
    Red,
    Green,
    Blue
}

// Enum with associated values
enum Shape {
    Circle(radius: float),
    Rectangle(width: float, height: float),
    Triangle(base: float, height: float)
}
```

#### Unions
```orionpp
union Number {
    Integer(value: int),
    Float(value: float)
}
```

#### Generic Types
```orionpp
struct Container<T> {
    value: T;
    
    func get(self) -> T {
        return self.value;
    }
}

// Multiple type parameters
struct Pair<T, U> {
    first: T;
    second: U;
}
```

### Array Types
```orionpp
[int]       // Dynamic array (slice)
[int; 5]    // Fixed-size array
```

### Optional Types
```orionpp
int?        // Optional integer (nullable)
string?     // Optional string
```

### Function Types
```orionpp
type BinaryOp = func(int, int) -> int;
```

### Reference Types
```orionpp
&Type       // Mutable reference
```

### Type Aliases
```orionpp
type Coordinate = Point2D;
type StringPair = Pair<string, string>;
```

### Traits (Interfaces)
```orionpp
trait Drawable {
    func draw(self);
    func getArea(self) -> float;
}

impl Drawable for Rectangle {
    func draw(self) {
        // Implementation
    }
    
    func getArea(self) -> float {
        return self.width * self.height;
    }
}
```

### Dynamic Type
```orionpp
dynamic     // Dynamic type for runtime type checking
```

## Variables

### Declaration
```orionpp
let name: type = value;     // Immutable variable (preferred)
var name: type = value;     // Mutable variable
let name = value;           // Type inference
```

### Scoping
```orionpp
let outer: int = 10;
{
    let inner: int = 20;    // Block scope
    // Can access 'outer' here
}
// 'inner' not accessible here
```

### Destructuring
```orionpp
// Tuple destructuring
let (a, b): (int, int) = (10, 20);

// Array destructuring
let [first, second, third]: [int; 3] = [1, 2, 3];

// Struct destructuring
let Point2D { x: pointX, y: pointY } = somePoint;
```

### Lazy Initialization
```orionpp
lazy let expensive: int = {
    // Computed when first accessed
    complexCalculation()
};
```

## Functions

### Declaration
```orionpp
func functionName(param1: Type1, param2: Type2) -> ReturnType {
    // Function body
    return value;
}

// No return type (void)
func procedureName(param: Type) {
    // Function body
}

// No parameters
func noParams() -> int {
    return 42;
}
```

### Compile-time vs Runtime Functions
```orionpp
// Compile-time function (const-evaluable)
const func computeArraySize(base: usize, multiplier: usize) -> usize {
    return base * multiplier;  // Must be evaluable at compile time
}

// Runtime function
func processUserInput(input: string) -> string {
    return input.trim();  // Evaluated at runtime
}

// Hybrid function (can be used in both contexts)
func add(a: i32, b: i32) -> i32 {
    return a + b;  // Simple enough for compile-time evaluation
}

// Usage examples
const BUFFER_SIZE: usize = computeArraySize(64, 16);    // Compile-time
let buffer: [u8; BUFFER_SIZE] = [0; BUFFER_SIZE];       // Size known at compile-time
let runtime_result: i32 = add(getUserInput(), 5);      // Runtime evaluation
```

### Generic Functions
```orionpp
func genericFunction<T>(value: T) -> T {
    return value;
}

// Compile-time generic specialization
const func genericCompute<T, const N: usize>(values: [T; N]) -> T 
    where T: Numeric + Copy
{
    // N is known at compile time, allows for loop unrolling
    var sum: T = T.zero();
    for (i: usize = 0; i < N; i = i + 1) {  // Can be unrolled
        sum = sum + values[i];
    }
    return sum;
}

// Runtime generic (type erasure or monomorphization)
func dynamicProcess<T>(values: [T]) -> T 
    where T: Numeric
{
    // Length unknown at compile time
    var sum: T = T.zero();
    for (value: T in values) {  // Runtime iteration
        sum = sum + value;
    }
    return sum;
}
```

### Variadic Functions (Detailed)

#### Runtime Variadic Functions (C-style)
```orionpp
// Runtime variadic - arguments processed at runtime
func printf_style(format: string, args: runtime...) -> i32 {
    // args is processed at runtime, type-erased
    // Similar to C's va_list mechanism
    var count: i32 = 0;
    for (arg: dynamic in args) {  // Runtime iteration over type-erased args
        match (arg.type()) {
            case i32:
                print(cast<i32>(arg));
            case f64:
                print(cast<f64>(arg));
            case string:
                print(cast<string>(arg));
            default:
                print("unknown");
        }
        count = count + 1;
    }
    return count;
}

// Usage: printf_style("Values: {} {} {}", 42, 3.14, "hello")
```

#### Compile-time Variadic Functions (C++ template-style)
```orionpp
// Compile-time variadic - arguments known at compile time
func compile_time_print<Args...>(args: Args...) {
    // Args is expanded at compile time
    // Each argument maintains its static type
    comptime for (arg in args) {  // Compile-time loop unrolling
        print(arg);  // Type-specific print selected at compile time
    }
}

// Compile-time variadic with type constraints
func sum_all<T, Args...>(first: T, rest: Args...) -> T 
    where T: Numeric, Args: All<Numeric>
{
    var result: T = first;
    comptime for (arg in rest) {  // Unrolled at compile time
        result = result + cast<T>(arg);  // Type conversion at compile time
    }
    return result;
}

// Usage: let total = sum_all(1, 2.5, 3); // Types known at compile time
```

#### Hybrid Variadic Functions
```orionpp
// Can handle both compile-time and runtime scenarios
func flexible_sum<T>(values: T...) -> T 
    where T: Numeric
{
    if (comptime values.isCompileTimeKnown()) {
        // Compile-time path - unroll the loop
        var sum: T = T.zero();
        comptime for (value in values) {
            sum = sum + value;
        }
        return sum;
    } else {
        // Runtime path - use normal iteration
        var sum: T = T.zero();
        for (value: T in values) {
            sum = sum + value;
        }
        return sum;
    }
}
```

#### Variadic Function Examples by Language Style

```orionpp
// C-style variadic (runtime type-erased)
func c_style_log(level: LogLevel, format: string, args: runtime...) {
    let formatted = format_string(format, args);  // Runtime formatting
    write_log(level, formatted);
}

// Python-style *args and **kwargs
func python_style(positional: runtime..., named: [string: dynamic]) {
    for (arg in positional) {
        process_positional(arg);
    }
    for (key, value in named) {
        process_named(key, value);
    }
}

// JavaScript-style arguments object
func js_style(...args: [dynamic]) {
    // args is a runtime array of dynamic values
    for (i: usize = 0; i < args.length; i = i + 1) {
        process_argument(args[i]);
    }
}

// Rust-style macro-like compile-time variadics
func rust_style_macro<Args...>(args: Args...) 
    where Args: All<Display>
{
    comptime {
        let format_str = generate_format_string(Args.types());
        println(format_str, args...);
    }
}

// Go-style interface{} variadic
func go_style(args: interface{}...) {
    for (arg in args) {
        match (arg) {
            case int(i):
                println("int:", i);
            case string(s):
                println("string:", s);
            default:
                println("unknown type");
        }
    }
}
```

## Control Flow

### Compile-time vs Runtime Control Flow

#### Compile-time Control Flow
```orionpp
// Compile-time conditionals (resolved during compilation)
comptime if (TARGET_ARCH == "x86_64") {
    // This code only exists in x86_64 builds
    func useX86Instructions() {
        // x86-specific implementation
    }
} else if (TARGET_ARCH == "aarch64") {
    // This code only exists in ARM64 builds  
    func useArmInstructions() {
        // ARM-specific implementation
    }
}

// Compile-time loops (unrolled at compile time)
comptime for (i: usize = 0; i < 4; i = i + 1) {
    // This loop is unrolled, generating 4 separate statements
    func processChannel{i}() {
        // Generates: processChannel0(), processChannel1(), etc.
    }
}

// Compile-time match (generates different code paths)
comptime match (TYPE_SIZE) {
    case 1:
        type OptimizedType = u8;
    case 2:
        type OptimizedType = u16;
    case 4:
        type OptimizedType = u32;
    case 8:
        type OptimizedType = u64;
    default:
        type OptimizedType = [u8; TYPE_SIZE];
}
```

#### Runtime Control Flow
```orionpp
// Runtime conditionals (evaluated during execution)
if (userInput > 0) {
    // Executed based on runtime value
    processPositiveInput(userInput);
} else {
    // Alternative runtime path
    handleNegativeInput(userInput);
}

// Runtime loops (iterate over runtime data)
for (item: Item in runtimeCollection) {
    // Loop body executes for each item at runtime
    processItem(item);
}

// Runtime match (pattern matching on runtime values)
match (runtimeValue) {
    case 0:
        return "zero";
    case 1..10:
        return "small";
    case x if x > 100:
        return "large";
    default:
        return "unknown";
}
```

### Conditional Statements
```orionpp
// if-else
if (condition) {
    // then branch
} else if (otherCondition) {
    // else if branch
} else {
    // else branch
}

// Conditional expression
let value: int = if (condition) 42 else 0;
```

### Loops

#### For Loop
```orionpp
// C-style for loop
for (i: int = 0; i < 10; i = i + 1) {
    // Loop body
}

// For-in loop (runtime iteration)
for (item: Type in collection) {
    // Loop body
}

// Compile-time for loop (unrolled)
comptime for (i: usize = 0; i < ARRAY_SIZE; i = i + 1) {
    // Unrolled at compile time
    array[i] = initValue;
}
```

#### While Loop
```orionpp
while (condition) {
    // Loop body
}
```

#### Infinite Loop
```orionpp
loop {
    // Loop body
    if (exitCondition) {
        break;
    }
}
```

### Switch vs Match Statements

#### Switch Statement (C-style, runtime)
```orionpp
// Traditional switch - runtime evaluation
switch (value) {
    case 1:
        // Handle case 1
        break;
    case 2:
    case 3:
        // Handle cases 2 and 3
        break;
    default:
        // Default case
}

// Switch with compile-time constants
switch (COMPILE_TIME_CONSTANT) {
    case OPTION_A:
        // Can be optimized away if not selected
        break;
    case OPTION_B:
        // Different code path
        break;
}
```

#### Match Statement (Pattern matching, compile-time optimized)
```orionpp
// Pattern matching - can be optimized at compile time
match (value) {
    case 0:
        return "zero";
    case 1..10:
        return "small";
    case x if x > 100:
        return "large";
    default:
        return "unknown";
}

// Enum matching with destructuring
match (shape) {
    case Circle(radius):
        return 3.14 * radius * radius;
    case Rectangle(width, height):
        return width * height;
    case Triangle(base, height):
        return 0.5 * base * height;
}

// Compile-time match (resolved at compile time)
comptime match (TARGET_FEATURES) {
    case features if features.contains("avx2"):
        func simdOperation() { /* AVX2 implementation */ }
    case features if features.contains("sse2"):
        func simdOperation() { /* SSE2 implementation */ }
    default:
        func simdOperation() { /* Scalar implementation */ }
}
```

### Advanced Control Flow

#### Compile-time Code Generation
```orionpp
// Generate different implementations based on compile-time parameters
comptime func generateOptimizedLoop<const N: usize>() {
    if (N <= 4) {
        // Unroll small loops
        comptime for (i: usize = 0; i < N; i = i + 1) {
            processElement(i);
        }
    } else if (N <= 64) {
        // Use vectorized implementation
        generateVectorizedLoop(N);
    } else {
        // Use chunked processing
        generateChunkedLoop(N);
    }
}
```

#### Conditional Compilation
```orionpp
// Feature-based compilation
#if feature(advanced_math)
    func advancedCalculation() -> f64 {
        // Advanced implementation
    }
#else
    func advancedCalculation() -> f64 {
        // Basic implementation
    }
#endif

// Platform-specific code
#if target_os = "windows"
    func platformSpecificCode() {
        // Windows implementation
    }
#elif target_os = "linux"
    func platformSpecificCode() {
        // Linux implementation
    }
#elif target_os = "macos"
    func platformSpecificCode() {
        // macOS implementation
    }
#endif
```

## Multi-Language Feature Support

Orion++ Human is designed to support language features from across the programming language spectrum, from low-level systems languages to high-level dynamic languages.

### C/C++ Style Features
```orionpp
// C-style pointers and memory management
func c_style_memory() {
    var ptr: *mut i32 = malloc(sizeof(i32));
    *ptr = 42;
    let value: i32 = *ptr;
    free(ptr);
}

// C++ style RAII and destructors
struct Resource {
    handle: Handle;
    
    func create(path: string) -> Self {
        return Self { handle: openResource(path) };
    }
    
    func drop(self) {  // Destructor called automatically
        closeResource(self.handle);
    }
}

// C++ template-style generics
func cpp_style_template<T, const N: usize>(array: [T; N]) -> T {
    comptime if (T.IS_NUMERIC) {
        var sum: T = T.zero();
        comptime for (i: usize = 0; i < N; i = i + 1) {
            sum = sum + array[i];  // Unrolled loop
        }
        return sum;
    } else {
        return array[0];  // Fallback for non-numeric types
    }
}
```

### Rust Style Features
```orionpp
// Ownership and borrowing
func rust_style_ownership() {
    let owned_data: String = String.new("Hello");
    let borrowed: &String = &owned_data;      // Immutable borrow
    let mut_borrowed: &mut String = &mut owned_data; // Mutable borrow
    
    // Move semantics
    let moved_data: String = owned_data;  // owned_data is no longer valid
}

// Result type and error propagation
func rust_style_errors() -> Result<i32, Error> {
    let file_content = readFile("config.txt")?;  // Propagate error
    let parsed_value = parseInt(file_content)?;   // Chain operations
    return Ok(parsed_value * 2);
}

// Pattern matching with guards
func rust_style_matching(value: Option<i32>) -> string {
    match (value) {
        case Some(x) if x > 0:
            return "positive";
        case Some(x) if x < 0:
            return "negative";
        case Some(0):
            return "zero";
        case None:
            return "no value";
    }
}
```

### Python Style Features
```orionpp
// Dynamic typing and duck typing
func python_style_dynamic(obj: dynamic) -> dynamic {
    // Duck typing - if it walks like a duck and quacks like a duck...
    if (obj.hasMethod("quack") && obj.hasMethod("walk")) {
        obj.quack();
        return obj.walk();
    }
    return null;
}

// List comprehensions and generator expressions
func python_style_comprehensions() {
    let numbers = [1, 2, 3, 4, 5];
    
    // List comprehension
    let squares = [x * x for x in numbers if x % 2 == 1];
    
    // Generator expression (lazy evaluation)
    let lazy_squares = (x * x for x in numbers if x % 2 == 1);
}

// Multiple assignment and unpacking
func python_style_unpacking() {
    let (a, b, c) = (1, 2, 3);
    let [first, ...rest] = [1, 2, 3, 4, 5];  // Rest contains [2, 3, 4, 5]
    
    // Swap variables
    (a, b) = (b, a);
}

// Decorators and metaprogramming
@memoize
@log_calls
func python_style_decorated(x: i32) -> i32 {
    return fibonacci(x);
}
```

### JavaScript Style Features
```orionpp
// Closures and first-class functions
func javascript_style_closures() {
    let outer_var = 42;
    
    let closure = func(x: i32) -> i32 {
        return x + outer_var;  // Captures outer_var
    };
    
    return closure;
}

// Async/await and promises
async func javascript_style_async() -> Promise<string> {
    let response = await fetch("https://api.example.com/data");
    let data = await response.json();
    return data.toString();
}

// Object-like syntax with dynamic properties
func javascript_style_objects() {
    let obj = dynamic {
        name: "John",
        age: 30,
        greet: func() { return "Hello, " + this.name; }
    };
    
    obj.newProperty = "dynamic addition";  // Add property at runtime
    let greeting = obj.greet();
}

// Prototype-based inheritance
struct Prototype {
    func extend(self, properties: [string: dynamic]) -> dynamic {
        let newObj = dynamic.create(self);
        for (key, value in properties) {
            newObj[key] = value;
        }
        return newObj;
    }
}
```

### Go Style Features
```orionpp
// Goroutines and channels
func go_style_concurrency() {
    let ch: chan<i32> = make_channel();
    
    // Spawn goroutine
    go func() {
        ch <- 42;  // Send to channel
    };
    
    let value = <-ch;  // Receive from channel
}

// Interface-based polymorphism
trait Writer {
    func write(self, data: [u8]) -> Result<usize, Error>;
}

func go_style_interfaces(w: Writer) {
    w.write(b"Hello, World!");  // Any type implementing Writer
}

// Multiple return values
func go_style_returns() -> (i32, Error?) {
    return (42, null);  // Success case
    // return (0, Error("failed"));  // Error case
}
```

### Java/C# Style Features
```orionpp
// Object-oriented programming with classes
class java_style_class {
    private field: i32;
    
    public func new(initialValue: i32) -> Self {
        return Self { field: initialValue };
    }
    
    public func getField(self) -> i32 {
        return self.field;
    }
    
    protected func setField(self, value: i32) {
        self.field = value;
    }
}

// Generics with constraints
func java_style_generics<T>(items: [T]) -> T 
    where T: Comparable + Clone
{
    let mut max_item = items[0].clone();
    for (item in items) {
        if (item > max_item) {
            max_item = item.clone();
        }
    }
    return max_item;
}

// Exception handling hierarchy
class CustomException: Exception {
    cause: string;
    
    func new(message: string, cause: string) -> Self {
        super.new(message);
        return Self { cause };
    }
}
```

### Haskell Style Features
```orionpp
// Pure functional programming
func haskell_style_pure<T, U>(list: [T], f: func(T) -> U) -> [U] {
    // Map function - pure, no side effects
    return [f(x) for x in list];
}

// Lazy evaluation and infinite lists
lazy let fibonacci: [i64] = {
    func fib_gen() -> Iterator<i64> {
        var (a, b) = (0, 1);
        loop {
            yield a;
            (a, b) = (b, a + b);
        }
    }
    return fib_gen().take(1000).collect();
};

// Pattern matching on algebraic data types
enum Tree<T> {
    Leaf(value: T),
    Node(left: Tree<T>, right: Tree<T>)
}

func haskell_style_tree_sum(tree: Tree<i32>) -> i32 {
    match (tree) {
        case Leaf(value):
            return value;
        case Node(left, right):
            return haskell_style_tree_sum(left) + haskell_style_tree_sum(right);
    }
}
```

### Lisp Style Features
```orionpp
// S-expression syntax support (alternative syntax)
func lisp_style_syntax() {
    // Traditional syntax
    let result = add(multiply(2, 3), 4);
    
    // S-expression syntax (compile-time macro)
    let result_sexp = sexp!(+ (* 2 3) 4);
    
    // Macro definition
    macro! define_operation(name, op) {
        func {name}(a: i32, b: i32) -> i32 {
            return a {op} b;
        }
    }
    
    define_operation!(add_op, +);
    define_operation!(mul_op, *);
}

// Code as data, data as code
func lisp_style_metaprogramming() {
    let code = quote! {
        let x = 10;
        let y = 20;
        x + y
    };
    
    let result = eval!(code);  // Execute code at runtime
}
```

### Language Integration Examples
```orionpp
// Multi-paradigm function supporting different calling conventions
func universal_function(
    // C-style parameters
    c_param: i32,
    // Python-style keyword arguments
    py_kwargs: [string: dynamic] = [:],
    // JavaScript-style callback
    js_callback: func(Error?, dynamic) -> void = null,
    // Go-style multiple returns enabled
    go_style: bool = false,
    // Rust-style Result return
    rust_result: bool = false
) -> dynamic | (dynamic, Error?) | Result<dynamic, Error> {
    
    let processed_value = c_param * 2;
    
    // Apply Python-style processing
    if (!py_kwargs.isEmpty()) {
        for (key, value in py_kwargs) {
            processed_value = process_with_key(processed_value, key, value);
        }
    }
    
    // Handle different return styles
    if (rust_result) {
        return Ok(processed_value);
    } else if (go_style) {
        return (processed_value, null);
    } else {
        // JavaScript-style callback
        if (js_callback != null) {
            js_callback(null, processed_value);
        }
        return processed_value;
    }
}
```

## Expressions

### Arithmetic
```orionpp
a + b       // Addition
a - b       // Subtraction
a * b       // Multiplication
a / b       // Division
a % b       // Modulo
```

### Comparison
```orionpp
a == b      // Equality
a != b      // Inequality
a < b       // Less than
a <= b      // Less than or equal
a > b       // Greater than
a >= b      // Greater than or equal
```

### Logical
```orionpp
a && b      // Logical AND
a || b      // Logical OR
!a          // Logical NOT
```

### Bitwise
```orionpp
a & b       // Bitwise AND
a | b       // Bitwise OR
a ^ b       // Bitwise XOR
~a          // Bitwise NOT
a << b      // Left shift
a >> b      // Right shift
```

### Assignment
```orionpp
a = b       // Assignment
a += b      // Add and assign
a -= b      // Subtract and assign
a *= b      // Multiply and assign
a /= b      // Divide and assign
a %= b      // Modulo and assign
```

### Type Operations
```orionpp
cast<Type>(value)           // Type casting
value is Type               // Type checking
value as Type               // Safe casting (returns optional)
```

### Range Operations
```orionpp
1..10       // Inclusive range
1..<10      // Exclusive range
```

## Comments

```orionpp
// Single-line comment

/*
 * Multi-line comment
 * Can span multiple lines
 */

/// Documentation comment for items
func documentedFunction() {
    // Implementation
}
```

## Keywords Reference

### Declaration Keywords
- `let` - Declare immutable variable
- `var` - Declare mutable variable
- `func` - Declare function
- `struct` - Declare structure
- `enum` - Declare enumeration
- `union` - Declare union type
- `trait` - Declare trait/interface
- `impl` - Implement trait for type
- `type` - Declare type alias
- `module` - Declare module

### Control Flow Keywords
- `if` - Conditional statement
- `else` - Alternative branch
- `while` - While loop
- `for` - For loop
- `loop` - Infinite loop
- `break` - Exit loop
- `continue` - Skip iteration
- `return` - Return from function
- `match` - Pattern matching
- `case` - Match case
- `default` - Default case
- `try` - Exception handling
- `catch` - Catch exception
- `finally` - Finally block
- `throw` - Throw exception

### Import/Export Keywords
- `import` - Import module or items
- `export` - Export items from module
- `from` - Specify import source
- `as` - Rename import/export

### Type Keywords
- `int` - Integer type
- `float` - Float type
- `bool` - Boolean type
- `char` - Character type
- `string` - String type
- `dynamic` - Dynamic type

### Special Keywords
- `self` - Reference to current instance
- `null` - Null value
- `true` - Boolean true
- `false` - Boolean false
- `cast` - Type casting
- `is` - Type checking
- `lazy` - Lazy initialization

### Modifiers
- `mut` - Mutable modifier (future extension)
- `pub` - Public visibility (future extension)
- `priv` - Private visibility (future extension)

### Preprocessor Directives
- `#if` - Conditional compilation
- `#else` - Alternative compilation
- `#endif` - End conditional compilation

## File Extension

Orion++ Human files use the `.hopp` extension (Human Orion Plus Plus).

## Syntax Conventions

1. **Indentation**: Use 2 or 4 spaces (consistent within project)
2. **Naming**: 
   - Variables and functions: `camelCase`
   - Types and modules: `PascalCase`
   - Constants: `UPPER_SNAKE_CASE`
3. **Braces**: Opening brace on same line as declaration
4. **Semicolons**: Required at end of statements
5. **Type annotations**: Explicit types preferred for clarity

This syntax reference covers the core features of Orion++ Human as demonstrated in the example files. The language is designed to be explicit, readable, and powerful enough to serve as an effective intermediate representation while remaining human-friendly for development and debugging.
