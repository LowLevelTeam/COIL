/**
* @file include/orion/orinopp.h
* @brief functionality for reading and writing orion++
*
* In a lot of locations you will find (dynamic c arrays) / (pointer and size_t)
* these are used for ease of frontend and come at little to no cost thanks to the
* arena allocator backend allocating these values in a singular place.
*
* The arena allocator then becomes a free garbage collector stoppping all memory leaks
* as all lifetimes are known to be the same and freed in the same. the arrays are after all
* only partly dynamic as they can always be allocated in full on the parse of the instruction
* so more like a runtime static array.
* 
* Orion++ is stored in .opp files and does not exist in an object format and is instead just written
* simiarly to high level languages but utilize text (it can be viewed as text if needed for debugging but
* parsing orion++ disassemble text is not supported by orion++ compilers).
*
* Orion++ is made to be a simple high level target of compilers meaning you will lack
* a lot of specifics available in object formats though there are workarounds availble.
*
*
*/

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
  ORIONPP_FEATURE_COMP = 1 << 6, // Compiler time functionality 
  ORIONPP_FEATURE_ORIN = 1 << 7, // Embedded Orion
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
  // Special
  ORIONPP_TYPE_VARID,
  ORIONPP_TYPE_LABELID,
  ORIONPP_TYPE_SYMBOL,
  ORIONPP_TYPE_STRING,

  // Machine Sized
  // word sized i.e. 1 byte on 8 bit cpu, 2 byte on 16 bit cpu, 4 byte on 32 bit, etc...
  ORIONPP_TYPE_WORD,
  ORIONPP_TYPE_SIZE,
  ORIONPP_TYPE_SSIZE,

  // double value opcode sections (certain modules may define types)
  // (they will use this selection for having a second opcode)
  ORIONPP_TYPE_DOUBLE_SECTION, // currently not used as no double opcodes exist
  ORIONPP_TYPE_C,

  ORIONPP_TYPE_END = 0xFF, // used to end statement
};
typedef uint8_t orionpp_type_t;

// Orion C Feature Types
enum orionpp_type_c {
  ORIONPP_TYPE_C_BOOL,     // C23 =1  %d [false, true] bool
  ORIONPP_TYPE_C_BOOL_FALSE,
  ORIONPP_TYPE_C_BOOL_TRUE, 
  ORIONPP_TYPE_C_CHAR,     // C.. >=8 %c [CHAR_MIN, CHAR_MAX] char
  ORIONPP_TYPE_C_CHAR_MIN, // -127 (default character is not strictly defined to be signed or unsigned but sticking with c standard of no sign specificer is signed is clearer and seems to be the current C compiler standard)
  ORIONPP_TYPE_C_CHAR_MAX, // 127
  ORIONPP_TYPE_C_SCHAR,    // C.. >=8 %c [SCHAR_MIN, SCHAR_MAX] signed char
  ORIONPP_TYPE_C_SCHAR_MIN,    // -127
  ORIONPP_TYPE_C_SCHAR_MAX,    // 127
  ORIONPP_TYPE_C_UCHAR,    // C.. >=8 %c [0, UCHAR_MAX] unsigned char
  ORIONPP_TYPE_C_UCHAR_MIN,    // 0
  ORIONPP_TYPE_C_UCHAR_MAX,    // 255
  ORIONPP_TYPE_C_SHORT,    // C.. >=16 %hi,%hd [SHRT_MIN, SHRT_MAX] short, short int, signed short, signed short int
  ORIONPP_TYPE_C_SHORT_MIN,    // -32768
  ORIONPP_TYPE_C_SHORT_MAX,    // 32767
  ORIONPP_TYPE_C_USHORT,   // C.. >=16 %hu [0, USHRT_MAX] unsigned short, unsigned short int
  ORIONPP_TYPE_C_USHORT_MIN,   // 0
  ORIONPP_TYPE_C_USHORT_MAX,   // 65535
  ORIONPP_TYPE_C_INT,      // C.. >=16 %i,%d [INT_MIN, INT_MAX] int, signed, signed int
  ORIONPP_TYPE_C_INT_MIN,      // -2147483648
  ORIONPP_TYPE_C_INT_MAX,      // 2147483647
  ORIONPP_TYPE_C_UINT,     // C.. >=16 %u [0, UINT_MAX] unsigned, unsigned int
  ORIONPP_TYPE_C_UINT_MIN,     // 0
  ORIONPP_TYPE_C_UINT_MAX,     // 4294967295
  ORIONPP_TYPE_C_LONG,     // C.. >=32 %li,%ld [LONG_MIN, LONG_MAX] long, long int, signed long, signed long int
  ORIONPP_TYPE_C_LONG_MIN,     // -2147483648 (32-bit) or -9223372036854775808 (64-bit)
  ORIONPP_TYPE_C_LONG_MAX,     // 2147483647 (32-bit) or 9223372036854775807 (64-bit)
  ORIONPP_TYPE_C_ULONG,    // C.. >=32 %lu [0, ULONG_MAX] unsigned long, unsigned long int
  ORIONPP_TYPE_C_ULONG_MIN,    // 0
  ORIONPP_TYPE_C_ULONG_MAX,    // 4294967295 (32-bit) or 18446744073709551615 (64-bit)
  ORIONPP_TYPE_C_LLONG,    // C99 >=64 %lli,%lld [LLONG_MIN, LLONG_MAX] long long, long long int, signed long long, signed long long int
  ORIONPP_TYPE_C_LLONG_MIN,    // -9223372036854775808
  ORIONPP_TYPE_C_LLONG_MAX,    // 9223372036854775807
  ORIONPP_TYPE_C_ULLONG,   // C99 >=64 %llu [0, ULLONG_MAX] unsigned long long, unsigned long long int
  ORIONPP_TYPE_C_ULLONG_MIN,   // 0
  ORIONPP_TYPE_C_ULLONG_MAX,   // 18446744073709551615
  ORIONPP_TYPE_C_FLOAT,    // C.. =32 %f,%F,%e,%E,%g,%G [FLT_MIN, FLT_MAX] float
  ORIONPP_TYPE_C_FLOAT_MIN,    // 1.175494e-38
  ORIONPP_TYPE_C_FLOAT_MAX,    // 3.402823e+38
  ORIONPP_TYPE_C_DOUBLE,   // C.. =64 %lf,%lF,%le,%lE,%lg,%lG [DBL_MIN, DBL_MAX] double
  ORIONPP_TYPE_C_DOUBLE_MIN,   // 2.225074e-308
  ORIONPP_TYPE_C_DOUBLE_MAX,   // 1.797693e+308
  ORIONPP_TYPE_C_LDOUBLE,  // C.. >=64 %Lf,%LF,%Le,%LE,%Lg,%LG [LDBL_MIN, LDBL_MAX] long double
  ORIONPP_TYPE_C_LDOUBLE_MIN,  // implementation-defined
  ORIONPP_TYPE_C_LDOUBLE_MAX,  // implementation-defined
  ORIONPP_TYPE_C_SIZE_T,   // C.. unsigned %zu [0, SIZE_MAX] size_t
  ORIONPP_TYPE_C_SIZE_T_MIN,   // 0
  ORIONPP_TYPE_C_SIZE_T_MAX,   // SIZE_MAX (typically UINT_MAX or ULONG_MAX)
  ORIONPP_TYPE_C_PTRDIFF_T,// C.. signed %td [PTRDIFF_MIN, PTRDIFF_MAX] ptrdiff_t
  ORIONPP_TYPE_C_PTRDIFF_T_MIN,// PTRDIFF_MIN
  ORIONPP_TYPE_C_PTRDIFF_T_MAX,// PTRDIFF_MAX
  ORIONPP_TYPE_C_INTMAX_T, // C99 signed %jd [INTMAX_MIN, INTMAX_MAX] intmax_t
  ORIONPP_TYPE_C_INTMAX_T_MIN, // INTMAX_MIN
  ORIONPP_TYPE_C_INTMAX_T_MAX, // INTMAX_MAX  
  ORIONPP_TYPE_C_UINTMAX_T,// C99 unsigned %ju [0, UINTMAX_MAX] uintmax_t
  ORIONPP_TYPE_C_UINTMAX_T_MIN,// 0
  ORIONPP_TYPE_C_UINTMAX_T_MAX,// UINTMAX_MAX
  ORIONPP_TYPE_C_INTPTR_T, // C99 signed %jd [INTPTR_MIN, INTPTR_MAX] intptr_t
  ORIONPP_TYPE_C_INTPTR_T_MIN, // INTPTR_MIN
  ORIONPP_TYPE_C_INTPTR_T_MAX, // INTPTR_MAX
  ORIONPP_TYPE_C_UINTPTR_T,// C99 unsigned %ju [0, UINTPTR_MAX] uintptr_t
  ORIONPP_TYPE_C_UINTPTR_T_MIN,// 0
  ORIONPP_TYPE_C_UINTPTR_T_MAX,// UINTPTR_MAX
  ORIONPP_TYPE_C_WCHAR_T,  // C.. %lc [WCHAR_MIN, WCHAR_MAX] wchar_t
  ORIONPP_TYPE_C_WCHAR_T_MIN,  // WCHAR_MIN
  ORIONPP_TYPE_C_WCHAR_T_MAX,  // WCHAR_MAX
  ORIONPP_TYPE_C_DEC32,  // _Decimal32
  ORIONPP_TYPE_C_DEC32_MIN,  //
  ORIONPP_TYPE_C_DEC32_MAX,  //
  ORIONPP_TYPE_C_DEC64,  // _Decimal64
  ORIONPP_TYPE_C_DEC64_MIN,  //
  ORIONPP_TYPE_C_DEC64_MAX,  //
  ORIONPP_TYPE_C_DEC128,  // _Decimal128
  ORIONPP_TYPE_C_DEC128_MIN,  //
  ORIONPP_TYPE_C_DEC128_MAX,  //
  ORIONPP_TYPE_C_INT8,  
  ORIONPP_TYPE_C_UINT8, 
  ORIONPP_TYPE_C_INT16, 
  ORIONPP_TYPE_C_UINT16,
  ORIONPP_TYPE_C_INT32, 
  ORIONPP_TYPE_C_UINT32,
  ORIONPP_TYPE_C_INT64, 
  ORIONPP_TYPE_C_UINT64,

  // Qualifiers (another type opcode follows these types)
  ORIONPP_TYPE_C_PTR,
  ORIONPP_TYPE_C_CONST,
  ORIONPP_TYPE_C_VOLATILE,
  ORIONPP_TYPE_C_RESTRICT,
  ORIONPP_TYPE_C_ATOMIC,
  ORIONPP_TYPE_C_COMPLEX,
  ORIONPP_TYPE_C_IMAGINARY,
  ORIONPP_TYPE_C_BITINT,
  ORIONPP_TYPE_C_REGISTER,

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

// -------------------------------- STRUCTS -------------------------------- //

typedef struct orionpp_header {
  unsigned char magic[ORIONPP_MAGIC_LEN];
  unsigned char version[3];
  uint8_t wordbytesize;
  orionpp_feature_t features_1;
  orionpp_feature_t features_2; 
} orionpp_header_t;

typedef struct orinopp_value {
  // members ordered for efficiency not format consistency 
  const char *bytes;
  size_t bytesize;
  orionpp_type_t root;
  orionpp_type_module_t child;
} orinopp_value_t;

typedef struct orinopp_instruction {
  // members ordered for efficiency not format consistency
  orinopp_value_t *values;
  size_t value_count;
  orionpp_opcode_t root;
  orionpp_opcode_module_t child;
} orinopp_instruction_t;

// -------------------------------- FUNCS -------------------------------- //

// turn binary to a textual version for debugging
void orionpp_dissasemble(orinopp_instruction_t *instr);

// read in an instruction
void orionpp_readf(file_handle_t file, orinopp_instruction_t *dest);

// write out an instruction
void orionpp_writef(file_handle_t file, const orinopp_instruction_t *src);

// read in an instruction from a buffer
void orionpp_readbuf(char *buf, size_t bufsize, orinopp_instruction_t *dest);

// write out an instruction into a buffer
void orionpp_writebuf(const char *buf, size_t bufsize, const orinopp_instruction_t *src);

