# COIL Type System

The COIL type system is central to the architecture, allowing for precise control over data representation and operations. Types are encoded in 16 bits, with the first 8 bits representing the main type and the second 8 bits representing type extensions.

The type system is designed to be complete yet extensible, supporting all common data types while allowing for architectural specialization when needed.

## 1. Type Encoding

Types are encoded in a 16-bit format:
- First 8 bits: Main Type - Defines the primary type category
- Second 8 bits: Type Extensions - Provides qualifiers and additional type information

Some types require additional data beyond the 16-bit type descriptor:
- Composite types (structures, arrays) include structure definitions
- Complex types include width specifications
- Special types may include register identifiers or other implementation-specific data

## 2. Main Types (First 8 bits)

```
// Fixed width signed integers
TYPE_INT8   = 0x01  // 8-bit signed integer
TYPE_INT16  = 0x02  // 16-bit signed integer
TYPE_INT32  = 0x03  // 32-bit signed integer
TYPE_INT64  = 0x04  // 64-bit signed integer
TYPE_INT128 = 0x05  // 128-bit signed integer

// Fixed width unsigned integers
TYPE_UNT8   = 0x10  // 8-bit unsigned integer
TYPE_UNT16  = 0x12  // 16-bit unsigned integer
TYPE_UNT32  = 0x13  // 32-bit unsigned integer
TYPE_UNT64  = 0x14  // 64-bit unsigned integer
TYPE_UNT128 = 0x15  // 128-bit unsigned integer
  
// Fixed width floating point
TYPE_FP8e5m2 = 0x20  // 8-bit float (5-bit exponent, 2-bit mantissa)
TYPE_FP8e4m3 = 0x21  // 8-bit float (4-bit exponent, 3-bit mantissa)
TYPE_FP16b   = 0x22  // bfloat16
TYPE_FP16    = 0x23  // IEEE 754 half precision
TYPE_FP32t   = 0x24  // tensor float 32
TYPE_FP32    = 0x25  // IEEE 754 single precision
TYPE_FP64    = 0x26  // IEEE 754 double precision
TYPE_FP80    = 0x27  // x87 extended precision
TYPE_FP128   = 0x28  // IEEE 754 quadruple precision

// Fixed Width Vectors
TYPE_V128 = 0x30  // 128-bit vector
TYPE_V256 = 0x31  // 256-bit vector
TYPE_V512 = 0x32  // 512-bit vector

// Boolean type
TYPE_BIT = 0x40  // 1-bit boolean, special type to allow for creating bitmaps behind the scenes 

// COIL Special Types
TYPE_VAR = 0x90  // Value is a variable ID
TYPE_SYM = 0x91  // Value is a symbol rather than value at symbol
TYPE_RGP = 0x92  // Register general purpose
TYPE_RFP = 0x93  // Register floating point
TYPE_RV  = 0x94  // Register vector
TYPE_RS  = 0x95  // Register segment
TYPE_SAR = 0x96  // State - all registers
TYPE_SAF = 0x97  // State - all flags
TYPE_SES = 0x98  // State - extended processor state
TYPE_SS  = 0x99  // State - complete state
TYPE_IP  = 0x9A  // Register Instruction Pointer
TYPE_SP  = 0x9B  // Register Stack Pointer
TYPE_BP  = 0x9C  // Register Base Pointer

// Platform Dependent Types
TYPE_INT  = 0xA0  // Default integer for current platform
TYPE_UNT  = 0xA1  // Default unsigned for current platform
TYPE_FP   = 0xA2  // Default float for current platform
TYPE_LINT = 0xA3  // Largest integer for current platform
TYPE_LUNT = 0xA4  // Largest unsigned for current platform
TYPE_LFP  = 0xA5  // Largest float point for current platform
TYPE_PTR  = 0xA6  // Default pointer size for current platform

// Complex Types
TYPE_CINT = 0xB0  // Complex integer
TYPE_CUNT = 0xB1  // Complex unsigned
TYPE_CFP  = 0xB2  // Complex floating point

// Composite Types
TYPE_STRUCT = 0xD0  // Structure type
TYPE_PACK   = 0xD1  // Packed structure (no padding)
TYPE_UNION  = 0xD2  // Union type
TYPE_ARRAY  = 0xD3  // Array type

// Parameter Types
TYPE_PARAM5 = 0xF0  // Parameter type 5
TYPE_PARAM4 = 0xFA  // Parameter type 4
TYPE_PARAM3 = 0xFB  // Parameter type 3
TYPE_PARAM2 = 0xFC  // Parameter type 2
TYPE_PARAM1 = 0xFD  // Parameter type 1
TYPE_PARAM0 = 0xFE  // Parameter type 0

// Void Type
TYPE_VOID = 0xFF  // Void type (no value)
```

## 3. Type Extensions (Second 8 bits)

Type extensions provide additional qualifiers for type values:

```
TYPEEXT_CONST    = (1 << 0)  // 0x01 - Constant value (read-only)
TYPEEXT_VOLATILE = (1 << 1)  // 0x02 - Volatile access (not optimizable)
TYPEEXT_VOID     = (1 << 4)  // 0x10 - No Value
TYPEEXT_IMM      = (1 << 5)  // 0x20 - Immediate value
TYPEEXT_VAR      = (1 << 6)  // 0x40 - Variable ID
TYPEEXT_SYM      = (1 << 7)  // 0x80 - Symbol ID
```

Type extensions not only provide hints about the type but also define the format of the type's encoding. For example:

- `TYPEEXT_SYM` with a non-`TYPE_SYM` main type indicates that the value should be loaded from the memory location specified by the symbol.
- `TYPEEXT_VAR` indicates that the value represents a variable identifier.
- `TYPEEXT_CONST` indicates that the value cannot be modified after initialization.

Note: Using `TYPEEXT_SYM` with `TYPE_SYM` is an error, as it would imply loading a symbol from a symbol address, which is not meaningful in COIL.

## 4. Parameter Definitions

Parameters modify instruction behavior and provide additional context for operations.

### 4.1 symbol_parameter0_t
```
TMP  = 0x00 // symbol is used only in this context
FILE = 0x01 // symbol is used around the file
GLOB = 0x02 // symbol is used in other files
```

### 4.2 branch_condition_t
```
BRANCH_COND_EQ = 0x00  // Equal
BRANCH_COND_NE = 0x01  // Not equal
BRANCH_COND_GE = 0x02  // Greater than or equal
BRANCH_COND_LT = 0x03  // Less than
BRANCH_COND_GT = 0x04  // Greater than
BRANCH_COND_LE = 0x05  // Less than or equal
BRANCH_COND_Z  = 0x06  // Zero flag set
BRANCH_COND_NZ = 0x07  // Zero flag not set
BRANCH_COND_C  = 0x08  // Carry flag set
BRANCH_COND_NC = 0x09  // Carry flag not set
BRANCH_COND_O  = 0x0A  // Overflow flag set
BRANCH_COND_NO = 0x0B  // Overflow flag not set
BRANCH_COND_S  = 0x0C  // Sign flag set
BRANCH_COND_NS = 0x0D  // Sign flag not set
```

### 4.3 branch_ctrl_t
```
BRANCH_CTRL_FAR       = 0x00  // Far jump/call
BRANCH_CTRL_INL       = 0x01  // Inline
BRANCH_CTRL_ABI       = 0x02  // Use ABI conventions
BRANCH_CTRL_ABI_PARAM = 0x03  // Following operands are parameters
BRANCH_CTRL_ABI_RET   = 0x04  // Following operands are return destinations
```

### 4.4 Memory Control
```
MEMORY_CTRL_ATOMIC    = 0x01  // Atomic operation
MEMORY_CTRL_VOLATILE  = 0x02  // Volatile access
MEMORY_CTRL_ALIGNED   = 0x03  // Enforce alignment
MEMORY_CTRL_UNALIGNED = 0x04  // Allow unaligned access
```

## 5. Type Handling Rules

### 5.1 Type Compatibility

Types are compatible under the following conditions:
1. The main types match exactly
2. One type is a platform-dependent type and the other is its currently selected fixed-width equivalent
3. Both types are integer types with the same signedness and the destination type has equal or greater width
4. Both types are floating-point types and the destination type has equal or greater precision

### 5.2 Type Conversions

Explicit type conversions are performed using the `CONVERT` and `CAST` instructions:
- `CONVERT` performs a value-preserving conversion (may involve rounding, truncation, etc.)
- `CAST` reinterprets the bit pattern without changing the underlying data

### 5.3 Composite Type Definitions

Composite types (structures, unions, arrays) are defined using specific instructions:
- `STRUCT` defines a structure type with named fields
- Arrays include their element type and size information

## 6. Implementation Notes

COIL processors must implement the full type system but may optimize internal representations:

1. Platform-dependent types must map to appropriate fixed-width types based on the target architecture
2. Vector types may be implemented using SIMD hardware where available
3. Boolean values may be packed for efficiency
4. Register types map to the actual hardware registers of the target architecture