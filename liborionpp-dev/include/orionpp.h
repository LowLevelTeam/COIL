#ifndef ORIONPP_H
#define ORIONPP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

// Orion++ Magic Number
#define ORIONPP_MAGIC 0x4F50504F  // "OPPO" - Orion++ Binary

// Format version
#define ORIONPP_VERSION 2  // Updated for new variable system

// Error codes
typedef enum {
  ORIONPP_OK = 0,
  ORIONPP_ERR_INVALID_MAGIC,
  ORIONPP_ERR_INVALID_VERSION,
  ORIONPP_ERR_INVALID_FORMAT,
  ORIONPP_ERR_FILE_NOT_FOUND,
  ORIONPP_ERR_PERMISSION_DENIED,
  ORIONPP_ERR_OUT_OF_MEMORY,
  ORIONPP_ERR_INVALID_FUNCTION,
  ORIONPP_ERR_INVALID_VARIABLE,
  ORIONPP_ERR_INVALID_SYMBOL,
  ORIONPP_ERR_BUFFER_TOO_SMALL
} OrionPPError;

// Variable types
typedef enum {
  ORIONPP_VAR_TEMP = 0,     // Temporary variables (compiler generated)
  ORIONPP_VAR_LOCAL,        // Local variables
  ORIONPP_VAR_PARAM,        // Function parameters
  ORIONPP_VAR_GLOBAL        // Global variables
} OrionPPVariableType;

// Variable information
typedef struct {
  uint32_t id;              // Unique variable ID
  OrionPPVariableType type; // Variable type
  uint32_t name_offset;     // Offset into string table (for debugging)
  uint8_t data_type;        // Data type (i32, void, etc.)
  uint32_t scope_id;        // Scope identifier
} OrionPPVariable;

// Symbol information
typedef struct {
  uint32_t name_offset;     // Offset into string table
  uint8_t type;             // Symbol type (function, object, etc.)
  uint8_t visibility;       // Symbol visibility (local, global, weak)
  uint16_t section;         // Section identifier
  uint32_t value;           // Symbol value/address
  uint32_t size;            // Symbol size
} OrionPPSymbol;

// Function information
typedef struct {
  uint32_t symbol_offset;   // Offset to symbol entry
  uint16_t param_count;     // Number of parameters
  uint16_t local_count;     // Number of local variables
  uint32_t instruction_offset; // Offset into instruction data
  uint32_t instruction_count;  // Number of instruction bytes
  uint8_t abi_type;         // ABI type
  uint8_t return_type;      // Return type
  uint16_t flags;           // Function flags
  uint32_t first_var_id;    // First variable ID in this function
  uint32_t last_var_id;     // Last variable ID in this function
} OrionPPFunction;

// File header
typedef struct {
  uint32_t magic;           // Magic number
  uint16_t version;         // Format version
  uint16_t flags;           // Format flags
  uint32_t string_table_offset;
  uint32_t string_table_size;
  uint32_t symbol_table_offset;
  uint32_t symbol_table_size;
  uint32_t function_table_offset;
  uint32_t function_table_size;
  uint32_t variable_table_offset;  // New: variable table
  uint32_t variable_table_size;
  uint32_t instruction_data_offset;
  uint32_t instruction_data_size;
  uint32_t entry_point;     // Entry function index
  uint32_t next_var_id;     // Next available variable ID
  uint32_t reserved[2];     // Reserved for future use
} OrionPPHeader;

// Forward declarations
typedef struct OrionPPContext OrionPPContext;
typedef struct OrionPPWriter OrionPPWriter;
typedef struct OrionPPReader OrionPPReader;

// Context management
OrionPPContext* orionpp_context_create(void);
void orionpp_context_destroy(OrionPPContext* ctx);
OrionPPError orionpp_get_last_error(OrionPPContext* ctx);
const char* orionpp_get_error_string(OrionPPError error);

// String table management
uint32_t orionpp_add_string(OrionPPContext* ctx, const char* str);
const char* orionpp_get_string(OrionPPContext* ctx, uint32_t offset);

// Variable management
uint32_t orionpp_create_variable(OrionPPContext* ctx, OrionPPVariableType type, 
                                const char* debug_name, uint8_t data_type);
uint32_t orionpp_create_temp_variable(OrionPPContext* ctx, uint8_t data_type);
OrionPPVariable* orionpp_get_variable(OrionPPContext* ctx, uint32_t var_id);
const char* orionpp_get_variable_debug_name(OrionPPContext* ctx, uint32_t var_id);

// Symbol management  
uint32_t orionpp_add_symbol(OrionPPContext* ctx, const char* name, uint8_t type,
                           uint8_t visibility, uint16_t section, uint32_t value, uint32_t size);
OrionPPSymbol* orionpp_get_symbol(OrionPPContext* ctx, uint32_t index);
OrionPPSymbol* orionpp_find_symbol(OrionPPContext* ctx, const char* name);

// Function management
uint32_t orionpp_begin_function(OrionPPContext* ctx, const char* name, 
                               uint16_t param_count, uint8_t return_type);
OrionPPError orionpp_end_function(OrionPPContext* ctx);
OrionPPFunction* orionpp_get_function(OrionPPContext* ctx, uint32_t index);

// Output format types
typedef enum {
  ORIONPP_FORMAT_BINARY,    // Binary .orionpp format
  ORIONPP_FORMAT_HUMAN      // Human-readable .hopp format
} OrionPPFormat;

// Context accessor functions
size_t orionpp_context_get_string_table_size(OrionPPContext* ctx);
size_t orionpp_context_get_symbol_count(OrionPPContext* ctx);
size_t orionpp_context_get_function_count(OrionPPContext* ctx);
size_t orionpp_context_get_variable_count(OrionPPContext* ctx);
const char* orionpp_context_get_string_table(OrionPPContext* ctx);
const OrionPPSymbol* orionpp_context_get_symbols(OrionPPContext* ctx);
const OrionPPFunction* orionpp_context_get_functions(OrionPPContext* ctx);
const OrionPPVariable* orionpp_context_get_variables(OrionPPContext* ctx);
uint32_t orionpp_context_get_next_var_id(OrionPPContext* ctx);

// High-level API
#include "orionpp_writer.h"
#include "orionpp_reader.h"

// Instruction opcodes (same as before but with variable ID operands)
#define ORIONPP_OP_NOP          0x00
#define ORIONPP_OP_ENTER        0x01
#define ORIONPP_OP_LEAVE        0x02
#define ORIONPP_OP_RET          0x03

#define ORIONPP_OP_LET          0x10
#define ORIONPP_OP_CONST        0x11
#define ORIONPP_OP_MOV          0x12

#define ORIONPP_OP_ADD          0x20
#define ORIONPP_OP_SUB          0x21
#define ORIONPP_OP_MUL          0x22
#define ORIONPP_OP_DIV          0x23
#define ORIONPP_OP_MOD          0x24
#define ORIONPP_OP_NEG          0x25

#define ORIONPP_OP_AND          0x30
#define ORIONPP_OP_OR           0x31
#define ORIONPP_OP_XOR          0x32
#define ORIONPP_OP_NOT          0x33
#define ORIONPP_OP_SHL          0x34
#define ORIONPP_OP_SHR          0x35

#define ORIONPP_OP_JMP          0x40
#define ORIONPP_OP_CALL         0x41
#define ORIONPP_OP_BEQ          0x42
#define ORIONPP_OP_BNE          0x43
#define ORIONPP_OP_BLT          0x44
#define ORIONPP_OP_BLE          0x45
#define ORIONPP_OP_BGT          0x46
#define ORIONPP_OP_BGE          0x47
#define ORIONPP_OP_BEQZ         0x48
#define ORIONPP_OP_BNEZ         0x49

#define ORIONPP_OP_ABI_CALLEE_ENTER     0x50
#define ORIONPP_OP_ABI_CALLEE_LEAVE     0x51
#define ORIONPP_OP_ABI_CALLEE_GETARG    0x52
#define ORIONPP_OP_ABI_CALLEE_SETRET    0x53
#define ORIONPP_OP_ABI_CALLER_SETUP     0x54
#define ORIONPP_OP_ABI_CALLER_CLEANUP   0x55
#define ORIONPP_OP_ABI_CALLER_SETARG    0x56
#define ORIONPP_OP_ABI_CALLER_GETRET    0x57

// Operand types
#define ORIONPP_OPERAND_IMMEDIATE       0x00  // 00xxxxxx
#define ORIONPP_OPERAND_VARIABLE        0x40  // 01xxxxxx  
#define ORIONPP_OPERAND_LABEL           0x80  // 10xxxxxx
#define ORIONPP_OPERAND_SYMBOL          0xC0  // 11xxxxxx

// Data types
#define ORIONPP_TYPE_VOID       0x00
#define ORIONPP_TYPE_I32        0x01

// Symbol types
#define ORIONPP_SYMBOL_FUNCTION 0x01
#define ORIONPP_SYMBOL_OBJECT   0x02
#define ORIONPP_SYMBOL_SECTION  0x03

// Symbol visibility
#define ORIONPP_VIS_LOCAL       0x00
#define ORIONPP_VIS_GLOBAL      0x01
#define ORIONPP_VIS_WEAK        0x02

// ABI types
#define ORIONPP_ABI_C           0x01
#define ORIONPP_ABI_CUSTOM      0x02

#endif // ORIONPP_H