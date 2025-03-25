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

### 1.1 Type Data Syntax

In COIL-ASM, type data can be specified in two ways:

1. **Single Equals Syntax**: `TYPE=data` where `data` is the type data
   ```
   TYPE_RGP=RAX        ; Register type with RAX as type data
   TYPE_ARRAY=TYPE_UNT8  ; Array type with UNT8 as element type
   ```

2. **Double Equals Syntax**: `TYPE=typedata=followingdata` where the first part after the equals is type data and the second part is following data
   ```
   TYPE_ABICTL=ABICTL_PARAM=linux_x86_64  ; ABI control with parameter type and specific ABI
   TYPE_PTR=TYPE_INT32=0x1000             ; Pointer type with INT32 as pointed type and address
   ```

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
TYPE_ABICTL = 0xF8  // ABI control parameter

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

### 4.4 abi_ctrl_t
```
ABICTL_STANDARD = 0x00  // Use specified ABI convention
ABICTL_PARAM    = 0x01  // Passing parameters via ABI
ABICTL_RET      = 0x02  // Returning values via ABI
ABICTL_VARIADIC = 0x03  // Variadic function support
ABICTL_CUSTOM   = 0x04  // Custom ABI handling
```

### 4.5 Memory Control
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

## 7. Type Examples

### 7.1 Basic Type Usage
```
; Simple integer variable
VAR TYPE_INT32, counter, 0

; Register type with explicit register
MOV TYPE_RGP=RAX, 42

; Array type with element type
VAR TYPE_ARRAY=TYPE_UNT8, buffer, "Hello"

; Pointer type with pointed-to type
VAR TYPE_PTR=TYPE_INT32, data_ptr

; Vector type with element type
VAR TYPE_V128=TYPE_FP32, vector, (1.0, 2.0, 3.0, 4.0)
```

### 7.2 Type Extensions
```
; Constant value
VAR TYPE_INT32+CONST, pi, 314159

; Volatile memory access
MOV [TYPE_PTR+VOLATILE], TYPE_INT32, 42

; Symbol reference
MOV TYPE_RGP=RAX, TYPE_INT32+SYM, data_symbol
```

### 7.3 ABI Control Parameters
```
; Standard ABI usage for function declaration
SYM add_function, TYPE_ABICTL=ABICTL_STANDARD=linux_x86_64

; Parameter passing via ABI
CALL function, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, param1, param2

; Return value via ABI
RET TYPE_ABICTL=ABICTL_RET=linux_x86_64, result

; Getting parameter from ABI
MOV param1, TYPE_ABICTL=ABICTL_PARAM=linux_x86_64, 0

; Getting return value from ABI
MOV result, TYPE_ABICTL=ABICTL_RET=linux_x86_64
```

### 7.4 Complex Type Data
```
; Structure type with field definitions
STRUCT TYPE_STRUCT=person, TYPE_INT32, age, TYPE_ARRAY=TYPE_UNT8, name, TYPE_INT32, id

; Accessing a structure field
GET TYPE_INT32, age, person_instance, age

; Array with explicit size
VAR TYPE_ARRAY=TYPE_INT32=10, int_array

; Pointer with both type and address info
VAR TYPE_PTR=TYPE_INT32=0x1000, mem_ptr
```

## 8. Binary Encoding of Types

This section illustrates how types are encoded in the COIL binary format:

```
; Type: TYPE_INT32
Binary: 0x03 0x00

; Type: TYPE_INT32+CONST
Binary: 0x03 0x01

; Type: TYPE_RGP=RAX
Binary: 0x92 0x00  ; Where 0x00 is the register ID for RAX

; Type: TYPE_ARRAY=TYPE_UNT8
Binary: 0xD3 0x00 0x10 0x00  ; 0xD3 for array, followed by element type

; Type: TYPE_ABICTL=ABICTL_PARAM=linux_x86_64
Binary: 0xF8 0x00 0x01 0x00 [ABI_ID]  ; 0xF8 for ABI control, 0x01 for PARAM, followed by ABI ID
```

The binary encoding of types with type data requires additional bytes to represent the type data and following data as applicable. The exact format depends on the specific type and data being encoded.