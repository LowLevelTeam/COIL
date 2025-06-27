#include <stddef.h>
#include <stdint.h>

#ifndef __ORIONDEV_FILE_HANDLE_DEFINED
#define __ORIONDEV_FILE_HANDLE_DEFINED
#ifdef WIN32
  #include <windows.h>
  typedef HANDLE file_handle_t;
#else
  typedef int file_handle_t;
#endif
#endif // __ORIONDEV_FILE_HANDLE_DEFINED

// -------------------------------- DEFS -------------------------------- //
#define ORIONPP_MAGIC_LEN 4
#define ORIONPP_MAGIC { 'O', 'P', 'P', 0xD4 } // Orion Plus Plus 0xD4 (i play the london... i am sorry)

// -------------------------------- ENUMS -------------------------------- //

// ---------------- FEATURES ---------------- //
enum orionpp_feature {
  ORIONPP_FEATURE_ISA  = 1 << 0, // Base Instruction Set
  ORIONPP_FEATURE_OBJ  = 1 << 1, // Object Functionality
  ORIONPP_FEATURE_HINT = 1 << 2, // Compiler Hints
  ORIONPP_FEATURE_TYPE = 1 << 3, // Type System / Type Table
  ORIONPP_FEATURE_C    = 1 << 4, // C types and help
  ORIONPP_FEATURE_ABI  = 1 << 5, // ABI Calling Convention
};
typedef uint64_t orionpp_feature_t;

// ---------------- ISA ---------------- //
enum orionpp_opcode {
  ORIONPP_OP_ISA,
  ORIONPP_OP_OBJ,
  ORIONPP_OP_HINT,
  ORIONPP_OP_TYPE,
  ORIONPP_OP_ABI,
  ORIONPP_OP_END = 0xFF, // universal opcode for end of instruction
};
typedef uint8_t orionpp_opcode_t;

enum orionpp_opcode_obj {
  ORIONPP_OP_OBJ_SECT,
  ORIONPP_OP_OBJ_SYM,
  ORIONPP_OP_OBJ_RAW,
  ORIONPP_OP_OBJ_RESV,
};
enum orionpp_opcode_isa {
  // memory
  ORIONPP_OP_ISA_VAR, // var i {= {value}}
  ORIONPP_OP_ISA_CONST, // const i {= {value}}
  ORIONPP_OP_ISA_MOV, // i = i
  ORIONPP_OP_ISA_LEA, // i = &i

  // control flow
  ORIONPP_OP_ISA_LABEL, // auto computed label: (cant be far away as it is computed at runtime if you wish to jump to a far target use a symbol)
  ORIONPP_OP_ISA_JMP, // goto
  ORIONPP_OP_ISA_BREQ, // i == i
  ORIONPP_OP_ISA_BRNEQ, // i != i
  ORIONPP_OP_ISA_BRGT, // i > i
  ORIONPP_OP_ISA_BRGE, // i >= i 
  ORIONPP_OP_ISA_BRLT, // i < i
  ORIONPP_OP_ISA_BRLE, // i <= i
  ORIONPP_OP_ISA_BRZ, // i == 0
  ORIONPP_OP_ISA_BRNZ, // i != 0

  ORIONPP_OP_ISA_CALL, // uncondtional jump to target and save a return address
  ORIONPP_OP_ISA_RET, // return to the address saved by call instruction

  // arithmetic
  ORIONPP_OP_ISA_ADD, // i + i
  ORIONPP_OP_ISA_SUB, // i - i
  ORIONPP_OP_ISA_MUL, // i * i
  ORIONPP_OP_ISA_DIV, // i / i
  ORIONPP_OP_ISA_MOD, // i % i
  ORIONPP_OP_ISA_INC, // ++i
  ORIONPP_OP_ISA_DEC, // --i
  ORIONPP_OP_ISA_INCp, // i++
  ORIONPP_OP_ISA_DECp, // i--

  // bit operations
  ORIONPP_OP_ISA_AND, // i & i
  ORIONPP_OP_ISA_OR,  // i | i
  ORIONPP_OP_ISA_XOR, // i ^ i
  ORIONPP_OP_ISA_NOT, // ~i
  ORIONPP_OP_ISA_SHL, // i << i
  ORIONPP_OP_ISA_SHR, // i >> i
};
enum orionpp_opcode_hint {
  // functions dont have an end as they are defined with a function symbol in the symbol table
  // a orion++ compiler will compile each instruction it finds and seperate into blocks for optimization
  // this funcend will help an optimizer seperate blocks
  ORIONPP_OP_HINT_FUNCEND,
};
enum orionpp_opcode_type {
  ORIONPP_OP_TYPE_DEF, // define a type
  // more will come as this IR develops a need for more complex type systems
};
enum orionpp_opcode_abi {
  // ABI Table
  // thie definition relies partly on embedded orion and requires the feature enabled
  // this is because specific register knowledge is required by orion++ which is not common.
  // to ensure compilers can complete the tasks you should define the ORIN feature as well if using this.
  // ORIONPP_OP_ABI_DEFINE, // define a custom ABI (requires specific knowledge of target system)

  // callee
  ORIONPP_OP_ABI_ESETUP, // call at the start of a function
  ORIONPP_OP_ABI_ECLEANUP, // call at the end of a function
  ORIONPP_OP_ABI_GETARG, // assign a local variable to a passed argument
  ORIONPP_OP_ABI_SETRET, // assign a return value

  // caller
  ORIONPP_OP_ABI_RSETUP, // call before a function call
  ORIONPP_OP_ABI_RCLEANUP, // call after a function call
  ORIONPP_OP_ABI_SETARG, // set an argument
  ORIONPP_OP_ABI_GETRET, // assign a local variable to a passed return value
};

typedef uint8_t orionpp_opcode_module_t;

// ---------------- TYPE ---------------- //

// [ORIONPP_OP_ISA][ORIONPP_OP_ISA_CONST][orionpp_variable_id_t][ORIONPP_TYPE_WORD][binary of metadata.wordbytesize]
enum orionpp_type {
  ORIONPP_TYPE_AUTO, // use as much as possible for compact binaries

  // Special
  ORIONPP_TYPE_VARID,
  ORIONPP_TYPE_LABELID,
  ORIONPP_TYPE_SYMBOL,
  ORIONPP_TYPE_STRING,

  // Machine Sized
  // word sized i.e. 1 byte on 8 bit cpu, 2 byte on 16 bit cpu, 4 byte on 32 bit, etc...
  ORIONPP_TYPE_WORD,

  ORIONPP_TYPE_FUNC, // [uint8_t retlen][[type]...retlen][uint8_t param][[type]...paramlen]
  ORIONPP_TYPE_BOOL, // allows the optimizer to pack multiple booleans as the compiler can now they only require 1 bit

  // double value opcode sections (certain modules may define types)
  // (they will use this selection for having a second opcode)
  ORIONPP_TYPE_DOUBLE_SECTION, // currently not used as no double opcodes exist
  ORIONPP_TYPE_C,

  ORIONPP_TYPE_EQ = 0xFE, // type followed by immediate value
  ORIONPP_TYPE_END = 0xFF, // used to end statement
};
typedef uint8_t orionpp_type_t;

// Orion C Feature Types
enum orionpp_type_c {
  ORIONPP_TYPE_C_INT8, 
  ORIONPP_TYPE_C_UINT8, 
  ORIONPP_TYPE_C_INT16, 
  ORIONPP_TYPE_C_UINT16,
  ORIONPP_TYPE_C_INT32, 
  ORIONPP_TYPE_C_UINT32,
  ORIONPP_TYPE_C_INT64, 
  ORIONPP_TYPE_C_UINT64,
  ORIONPP_TYPE_C_FLOAT,    // C.. =32 %f,%F,%e,%E,%g,%G [FLT_MIN, FLT_MAX] float
  ORIONPP_TYPE_C_FLOAT_MIN,    // 1.175494e-38
  ORIONPP_TYPE_C_FLOAT_MAX,    // 3.402823e+38
  ORIONPP_TYPE_C_DOUBLE,   // C.. =64 %lf,%lF,%le,%lE,%lg,%lG [DBL_MIN, DBL_MAX] double
  ORIONPP_TYPE_C_DOUBLE_MIN,   // 2.225074e-308
  ORIONPP_TYPE_C_DOUBLE_MAX,   // 1.797693e+308
  ORIONPP_TYPE_C_LDOUBLE,  // C.. >=64 %Lf,%LF,%Le,%LE,%Lg,%LG [LDBL_MIN, LDBL_MAX] long double
  ORIONPP_TYPE_C_LDOUBLE_MIN,  // implementation-defined
  ORIONPP_TYPE_C_LDOUBLE_MAX,  // implementation-defined

  // Qualifiers (another type opcode follows these types)
  ORIONPP_TYPE_C_PTR,
  ORIONPP_TYPE_C_CONST,
  ORIONPP_TYPE_C_VOLATILE,

  // Composite
  ORIONPP_TYPE_C_STRUCT, // types follows as a list (anonymous structures)
  ORIONPP_TYPE_C_UNION, // types follow as a list (anonymous unions)
  ORIONPP_TYPE_C_STRUCTREF, // type has been defined elsewhere (followed by an id for the type)
  ORIONPP_TYPE_C_UNIONREF, // type has been defined elsewhere
  ORIONPP_TYPE_C_TYPEREF, // type has been defined elsewhere
  ORIONPP_TYPE_C_ARRAY, // type follows and then size
  ORIONPP_TYPE_C_VLARRAY, // type follows and then value for size

  // Other
  ORIONPP_TYPE_C_VOID,
  ORIONPP_TYPE_C_VARADIC, // varadic type
  ORIONPP_TYPE_C_FUNCTION, // followed by return type, param_count: u16 and parameters
};
typedef uint8_t orionpp_type_module_t;

// ---------------- ABI ---------------- //
enum orionpp_abi {
  ORIONPP_ABI_SYSTEM, // (alias for the standard system call convention)
  ORIONPP_ABI_C, // __cdecl (the abi used in libc)
  // ORIONPP_ABI_CLR // __clrcall
  // ORIONPP_ABI_FAST, // __fastcall
  // ORIONPP_ABI_THIS, // __thiscall
  // ORIONPP_ABI_VECTOR, // __vectorcall
};
typedef uint8_t orionpp_abi_t;

// -------------------------------- TYPES -------------------------------- //

typedef uint32_t orionpp_variable_id_t;
typedef uint32_t orionpp_label_id_t;
typedef uint32_t orionpp_symbol_t; // offset in string table for symbol name
typedef uint32_t orionpp_string_t; // offset in string table
