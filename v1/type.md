# Type System

The COIL type system is central to the architecture, allowing for precise control over data representation and operations. Types are encoded in 16 bits, with the first 8 bits representing the main type and the second 8 bits representing type extensions.

## Main Types (First 8 bits)

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

// Optimizable types
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
TYPE_CUNT = 0xB1  // Complex unsigned (pronounced (see-unt))
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

## Type Extensions (Second 8 bits)

Type extensions provide additional qualifiers for type values:

```
TYPEEXT_CONST    = (1 << 0)  // 0x01 - Constant value (read-only)
TYPEEXT_VOLATILE = (1 << 1)  // 0x02 - Volatile access (not optimizable)
TYPEEXT_VOID     = (1 << 4)  // 0x10 - No Value
TYPEEXT_IMM      = (1 << 5)  // 0x20 - Immediate value
TYPEEXT_VAR      = (1 << 6)  // 0x40 - Variable ID
TYPEEXT_SYM      = (1 << 7)  // 0x80 - Symbol ID
```

Where the type defines the underlying type the type extension not only gives hints about the type but also gives the format of the types encoding. 

One confusing parameter may be the TYPEEXT_SYM when there is already a symbol type. Well in this case it refers to a value at the symbols location rather then the symbol as the value itself. If TYPEEXT_SYM is used in par with TYPE_SYM then an error will occur but if you had a program with string data at the symbols location and you used a integral fixed width 8 array type with a symbol extension then the program will automatically dereference the data at the symbols location. TYPE_SYM can not be used as of course symbols are linker time and can not be affected for runtime values which would happen if a symbol could be located at a symbol since a symbol doesn't exist in program memory there is no way to derefence this.

Variables don't have the same feature as the variable id can be gotten with a self explanatory TYPE_VOID with TYPEEXT_VAR where the variable id cacn be obvious in being passed. if it was TYPE_VOID with TYPEEXT_SYM it could be mistaken as the symbol having void data as the symbol can be thought of similar to a pointer or address.

### Parameter Definitions

Parameters modify instruction behavior and provide additional context for operations.

#### symbol_parameter0_t
```
TMP  = 0x00 // symbol is used only in this context
FILE = 0x01 // symbol is used around the file
GLOB = 0x02 // symbol is used in other files
```

#### branch_condition_t
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

#### branch_ctrl_t
```
BRANCH_CTRL_FAR       = 0x00  // Far jump/call
BRANCH_CTRL_INL       = 0x01  // Inline
BRANCH_CTRL_ABI       = 0x02  // Use ABI conventions
BRANCH_CTRL_ABI_PARAM = 0x03  // Following operands are parameters
BRANCH_CTRL_ABI_RET   = 0x04  // Following operands are return destinations
```

#### Memory Control
```
MEMORY_CTRL_ATOMIC    = 0x01  // Atomic operation
MEMORY_CTRL_VOLATILE  = 0x02  // Volatile access
MEMORY_CTRL_ALIGNED   = 0x03  // Enforce alignment
MEMORY_CTRL_UNALIGNED = 0x04  // Allow unaligned access
```


