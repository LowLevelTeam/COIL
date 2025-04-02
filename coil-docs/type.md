# COIL Type System

The COIL Type System is a complex but needed way to define operands real operations through type inference.

Utilizing a type opcode with type extensions uint16_t base the type system is incredibly powerful and can support multiple high level language features natively and will do so in the future.

```
[type opcode: uint8_t][type extension: optional uint8_t][type data: optional ...][data: optional ...]
```

## Type Opcode

The Type Opcodes are as follows
```c
// Fixed Width Integral Scalar (0x00 - 0x0F)
INT8 = 0x00
UNT8 = 0x01
INT16 = 0x02
UNT16 = 0x03
INT32 = 0x04
UNT32 = 0x05
INT64 = 0x06
UNT64 = 0x07

// Fixed Width Floating Scalar (0x10 - 0x1F)
FP32 = 0x10
FP64 = 0x11

// Fixed Width Vector (0x20 - 0x2F)
V128 = 0x20
V256 = 0x21
V512 = 0x22

// Complex (0x30 - 0x33)
CINT = 0x30
CUNT = 0x31
CFP  = 0x32
CVEC = 0x33

// Composite (0xA0 - 0xAF)
STRUCT = 0x00
PACK = 0x01
UNION = 0x02
// could be expanded with higher level types like classes...

// Optimized (0xF0 - 0xF9)
BIT = 0xF0
VARID = 0xF1
SYMBOL = 0xF2
STRING = 0xF3
// later on platform specific types will also be here
// like the largest possible integer, fastest floating point, default pointer type etc...
REG = 0xF9

// Special (0xFA - 0xFF)
PARAM4 = 0xFA
PARAM3 = 0xFB
PARAM2 = 0xFC
PARAM1 = 0xFD
PARAM0 = 0xFE
VOID = 0xFF
```

### Fixed Width Types
A fixed width type is a complex type shorthand utilizing common system types like integers with a size in the 8 times table. You can find that most fixed width types directly map to your language of choice (if not interpreted). Example float = FP32 and double = FP64 etc...

### Complex
A complex type is complex (surprise). They utilize information after known as type data to determine the size of the type, it could also be called in long form a complex fixed width type.

#### Integral
```
type_data = [bit size: uint16_t]
```

#### Floating
```
type_data = [mantissa bit size: uint16_t][exponenet bit size: uint16_t]
```

#### Vector
```
type_data = [type: Type][count: uint16_t]
```

### Composite
Composite types are the integration of high level features into COIL allowing for combination of types in the same structure utilize a variable identifier system for the members.

This type is not being worked on right now and is not expected to be completed for some time.

Any reference implementations should leave structures as a Not Implemented Yet error.

### Optimized
Optimized types do not have a real size but only explain how much size they need and leave the processor to define them some space. This is to allow COIL to optimize places with multiple boolean values to exist in the same byte, word, etc... in a bitmap structure. This allows for better runtime space optimization.

#### BIT
A binary value of either 0 or 1

#### Variable ID
In some situations the variable ID may be used as the value in which case this type can be used.

[0xF1][variable id: uint64_t]

#### Symbol
In some situations you don't want the value behind the symbol but the address of the symbol in which case this type can be used.

[0xF2][symbol table id: uint64_t]

#### String
In some situations an immediate or constant string may be needed these are handled in a string table and the data after the string it just a uint64_t id in the string table.

[0xF3][string table id: uint64_t]

#### Register
Code involving registers is mostly removed but undersatnding that there may still be some cases where register usage is mandatory there is a system to utilize registers in default operations.

If there register types is used it should look something like the following
```
[0xF9][register: uint16_t]
```

[For more information.](./isa-e/index.md)

### Special Types

#### VOID
The VOID type is literally as it sounds a way to explain the lack of a type. void has a size of '0' and nothing comes after it.

#### PARAMETERS
The parameter types are a special way to modify an instruction utilizing the operand list. Parameters are counted in the operand count, they have a value that depends on the instruction. All use cases of the parameter type is listed below with reference to the original function.

Parameter types do not have a type extension they are just 8 bit and then the data defined by the parameter type as defined [here](#parameter-types).

## Type Extensions
A type extension is stored as a bitmap of values following the following format.

If neither variable or symbol is set then it must be an immediate.

```c
CONST     = (1 << 0) // ensure no modifications
VOLATILE  = (1 << 1) // do not optimize expressions with the value
ATOMIC    = (1 << 2) // instruction should use atomic variant if possible
SATURATE  = (1 << 3) // no overflow instead the value is capped at largest value 
RESERVED  = (1 << 4)
RESERVED  = (1 << 5)
VARIABLE  = (1 << 6) // the type in question refers to a value at a variable id
SYMBOL    = (1 << 7) // the type in question refers to a value at a named memory address
```

## Parameter Types
Each instruction can only have one of each parameter type.

```
[type opcode: uint8_t][value: uint8_t]
```

#### flag_condition_p
The flag conditional is used for runtime conditionals. As defined below there are multiple values for the branch condition.

```c
FLAG_COND_EQ = 0x00
FLAG_COND_NEQ = 0x01
FLAG_COND_GT = 0x02
FLAG_COND_GTE = 0x03
FLAG_COND_LT = 0x04
FLAG_COND_LTE = 0x05
```

#### pu_type_p
The processing unit type is used to specify the processing unit for specific instructions.

```c
PU_CPU = 0x00
```

#### arch_type_p
The architecture type is used to specify the processing units architecture (different values based on current processing unit selected)

```c
if (PU_CPU) {
  CPU_X86 = 0x00
  CPU_ARM = 0x01
}
```

#### mode_type_p
The mode type is used to specifiy architecture mode.

```c
MODE_8   = 0x00
MODE_16  = 0x01
MODE_32  = 0x02
MODE_64  = 0x03
MODE_128 = 0x04
```
