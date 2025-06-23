#ifndef ORION_BINARY_FORMAT_H
#define ORION_BINARY_FORMAT_H

#include "common.h"

// Magic number for Orion++ binary format
#define ORIONPP_MAGIC 0x4F50504F  // "OPPO" - Orion++ Binary

// Format version
#define ORIONPP_VERSION 1

// File header
typedef struct {
  uint32_t magic;                   // Magic number
  uint16_t version;                 // Format version
  uint16_t flags;                   // Format flags
  uint32_t string_table_offset;     // Offset to string table
  uint32_t string_table_size;       // Size of string table
  uint32_t symbol_table_offset;     // Offset to symbol table
  uint32_t symbol_table_size;       // Size of symbol table
  uint32_t function_table_offset;   // Offset to function table
  uint32_t function_table_size;     // Size of function table
  uint32_t instruction_data_offset; // Offset to instruction data
  uint32_t instruction_data_size;   // Size of instruction data
  uint32_t entry_point;             // Entry function ID
  uint32_t reserved[4];             // Reserved for future use
} OrionPPHeader;

// Symbol entry
typedef struct {
  uint32_t name_offset;     // Offset into string table
  uint8_t type;             // Symbol type
  uint8_t visibility;       // Symbol visibility
  uint16_t section;         // Section identifier
  uint32_t value;           // Symbol value/address
  uint32_t size;            // Symbol size
} OrionPPSymbol;

// Function entry
typedef struct {
  uint32_t name_offset;         // Offset into string table
  uint16_t param_count;         // Number of parameters
  uint16_t local_var_count;     // Number of local variables
  uint32_t instruction_offset;  // Offset into instruction data
  uint32_t instruction_count;   // Number of instructions
  uint8_t abi_type;             // ABI type
  uint8_t return_type;          // Return type
  uint16_t flags;               // Function flags
} OrionPPFunction;

// Symbol types
#define SYMBOL_TYPE_FUNCTION    0x01
#define SYMBOL_TYPE_OBJECT      0x02
#define SYMBOL_TYPE_SECTION     0x03

// Symbol visibility
#define SYMBOL_VIS_LOCAL        0x00
#define SYMBOL_VIS_GLOBAL       0x01
#define SYMBOL_VIS_WEAK         0x02

// ABI types
#define ABI_TYPE_C              0x01
#define ABI_TYPE_CUSTOM         0x02

// Return types
#define RETURN_TYPE_VOID        0x00
#define RETURN_TYPE_I32         0x01

// Instruction opcodes
#define OP_NOP                  0x00
#define OP_ENTER                0x01
#define OP_LEAVE                0x02
#define OP_RET                  0x03

// Variable operations
#define OP_LET                  0x10
#define OP_CONST                0x11
#define OP_MOV                  0x12

// Arithmetic operations
#define OP_ADD                  0x20
#define OP_SUB                  0x21
#define OP_MUL                  0x22
#define OP_DIV                  0x23
#define OP_MOD                  0x24
#define OP_NEG                  0x25

// Bitwise operations
#define OP_AND                  0x30
#define OP_OR                   0x31
#define OP_XOR                  0x32
#define OP_NOT                  0x33
#define OP_SHL                  0x34
#define OP_SHR                  0x35

// Control flow
#define OP_JMP                  0x40
#define OP_CALL                 0x41
#define OP_BEQ                  0x42
#define OP_BNE                  0x43
#define OP_BLT                  0x44
#define OP_BLE                  0x45
#define OP_BGT                  0x46
#define OP_BGE                  0x47
#define OP_BEQZ                 0x48
#define OP_BNEZ                 0x49

// ABI operations
#define OP_ABI_CALLEE_ENTER     0x50
#define OP_ABI_CALLEE_LEAVE     0x51
#define OP_ABI_CALLEE_GETARG    0x52
#define OP_ABI_CALLEE_SETRET    0x53
#define OP_ABI_CALLER_SETUP     0x54
#define OP_ABI_CALLER_CLEANUP   0x55
#define OP_ABI_CALLER_SETARG    0x56
#define OP_ABI_CALLER_GETRET    0x57

// Operand types
#define OPERAND_IMMEDIATE       0x00  // 00xxxxxx
#define OPERAND_VARIABLE        0x40  // 01xxxxxx
#define OPERAND_LABEL           0x80  // 10xxxxxx
#define OPERAND_STRING          0xC0  // 11xxxxxx

// Variable ID ranges
#define VAR_PARAM_BASE          0
#define VAR_LOCAL_BASE          256
#define VAR_TEMP_BASE           65536

// Binary writer structure
typedef struct BinaryWriter {
  FILE *file;
  size_t position;
  
  // String table
  char *string_table;
  size_t string_table_size;
  size_t string_table_capacity;
  
  // Symbol table
  OrionPPSymbol *symbols;
  size_t symbol_count;
  size_t symbol_capacity;
  
  // Function table
  OrionPPFunction *functions;
  size_t function_count;
  size_t function_capacity;
  
  // Instruction buffer
  uint8_t *instructions;
  size_t instruction_size;
  size_t instruction_capacity;
  
  // Variable mapping
  struct {
    char **names;
    uint32_t *ids;
    size_t count;
    size_t capacity;
  } variables;
  
  // Label mapping
  struct {
    char **names;
    uint32_t *offsets;
    bool *resolved;
    size_t count;
    size_t capacity;
  } labels;
  
  uint32_t next_temp_id;
} BinaryWriter;

// Binary writer API
OrionError binary_writer_init(BinaryWriter *writer, FILE *file);
void binary_writer_cleanup(BinaryWriter *writer);
OrionError binary_writer_finalize(BinaryWriter *writer);

// String table functions
uint32_t binary_writer_add_string(BinaryWriter *writer, const char *str);

// Variable management
uint32_t binary_writer_get_variable_id(BinaryWriter *writer, const char *name);
uint32_t binary_writer_get_temp_variable_id(BinaryWriter *writer);

// Label management
uint32_t binary_writer_add_label(BinaryWriter *writer, const char *name);
void binary_writer_resolve_label(BinaryWriter *writer, const char *name, uint32_t offset);

// Function management
OrionError binary_writer_begin_function(BinaryWriter *writer, const char *name, 
                                       uint16_t param_count, uint8_t return_type);
OrionError binary_writer_end_function(BinaryWriter *writer);

// Instruction emission
OrionError binary_writer_emit_opcode(BinaryWriter *writer, uint8_t opcode);
OrionError binary_writer_emit_operand_immediate(BinaryWriter *writer, int32_t value);
OrionError binary_writer_emit_operand_variable(BinaryWriter *writer, uint32_t var_id);
OrionError binary_writer_emit_operand_label(BinaryWriter *writer, const char *label);

// High-level instruction emission
OrionError binary_writer_emit_let(BinaryWriter *writer, uint32_t dest, uint32_t src);
OrionError binary_writer_emit_const(BinaryWriter *writer, uint32_t dest, int32_t value);
OrionError binary_writer_emit_mov(BinaryWriter *writer, uint32_t dest, uint32_t src);
OrionError binary_writer_emit_add(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_sub(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_mul(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_div(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_mod(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_neg(BinaryWriter *writer, uint32_t dest, uint32_t src);
OrionError binary_writer_emit_and(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_or(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_xor(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2);
OrionError binary_writer_emit_not(BinaryWriter *writer, uint32_t dest, uint32_t src);
OrionError binary_writer_emit_shl(BinaryWriter *writer, uint32_t dest, uint32_t src, uint32_t count);
OrionError binary_writer_emit_shr(BinaryWriter *writer, uint32_t dest, uint32_t src, uint32_t count);

// Control flow
OrionError binary_writer_emit_enter(BinaryWriter *writer);
OrionError binary_writer_emit_leave(BinaryWriter *writer);
OrionError binary_writer_emit_ret(BinaryWriter *writer);
OrionError binary_writer_emit_jmp(BinaryWriter *writer, const char *label);
OrionError binary_writer_emit_call(BinaryWriter *writer, const char *function);
OrionError binary_writer_emit_beq(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_bne(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_blt(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_ble(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_bgt(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_bge(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label);
OrionError binary_writer_emit_beqz(BinaryWriter *writer, uint32_t src, const char *label);
OrionError binary_writer_emit_bnez(BinaryWriter *writer, uint32_t src, const char *label);

// ABI operations
OrionError binary_writer_emit_abi_callee_enter(BinaryWriter *writer);
OrionError binary_writer_emit_abi_callee_leave(BinaryWriter *writer);
OrionError binary_writer_emit_abi_callee_getarg(BinaryWriter *writer, uint32_t dest, int index);
OrionError binary_writer_emit_abi_callee_setret(BinaryWriter *writer, uint32_t src, int index);
OrionError binary_writer_emit_abi_caller_setup(BinaryWriter *writer);
OrionError binary_writer_emit_abi_caller_cleanup(BinaryWriter *writer);
OrionError binary_writer_emit_abi_caller_setarg(BinaryWriter *writer, uint32_t src, int index);
OrionError binary_writer_emit_abi_caller_getret(BinaryWriter *writer, uint32_t dest, int index);

#endif // ORION_BINARY_FORMAT_H