#ifndef ORIONPP_READER_H
#define ORIONPP_READER_H

#include "orionpp.h"

// Reader API for parsing Orion++ files
struct OrionPPReader {
  OrionPPContext* context;
  FILE* input;
  OrionPPFormat format;
  
  // Header information
  OrionPPHeader header;
  
  // Loaded data
  char* string_table;
  OrionPPSymbol* symbols;
  OrionPPFunction* functions;
  OrionPPVariable* variables;
  uint8_t* instructions;
  
  // Sizes
  size_t symbol_count;
  size_t function_count;
  size_t variable_count;
  
  // Current position for sequential reading
  size_t current_function;
  size_t current_instruction_offset;
};

// Reader lifecycle
OrionPPReader* orionpp_reader_create(FILE* input);
void orionpp_reader_destroy(OrionPPReader* reader);
OrionPPError orionpp_reader_load(OrionPPReader* reader);

// Header access
const OrionPPHeader* orionpp_reader_get_header(OrionPPReader* reader);

// String table access
const char* orionpp_reader_get_string(OrionPPReader* reader, uint32_t offset);

// Symbol access
size_t orionpp_reader_get_symbol_count(OrionPPReader* reader);
const OrionPPSymbol* orionpp_reader_get_symbol(OrionPPReader* reader, size_t index);
const OrionPPSymbol* orionpp_reader_find_symbol(OrionPPReader* reader, const char* name);

// Function access
size_t orionpp_reader_get_function_count(OrionPPReader* reader);
const OrionPPFunction* orionpp_reader_get_function(OrionPPReader* reader, size_t index);
const OrionPPFunction* orionpp_reader_find_function(OrionPPReader* reader, const char* name);

// Variable access
size_t orionpp_reader_get_variable_count(OrionPPReader* reader);
const OrionPPVariable* orionpp_reader_get_variable(OrionPPReader* reader, uint32_t var_id);
const char* orionpp_reader_get_variable_debug_name(OrionPPReader* reader, uint32_t var_id);

// Instruction access
const uint8_t* orionpp_reader_get_instructions(OrionPPReader* reader);
size_t orionpp_reader_get_instruction_size(OrionPPReader* reader);

// Instruction iteration
typedef struct {
  uint8_t opcode;
  uint8_t flags;
  size_t operand_count;
  struct {
    uint8_t type;
    union {
      int32_t immediate;
      uint32_t variable_id;
      uint32_t label_offset;
      uint32_t symbol_offset;
    } value;
  } operands[4];  // Max 4 operands per instruction
} OrionPPInstruction;

OrionPPError orionpp_reader_decode_instruction(OrionPPReader* reader, size_t offset, 
                                              OrionPPInstruction* instruction, size_t* next_offset);

// High-level iteration
typedef void (*orionpp_instruction_callback)(const OrionPPInstruction* instr, size_t offset, void* user_data);
OrionPPError orionpp_reader_iterate_instructions(OrionPPReader* reader, 
                                                orionpp_instruction_callback callback, void* user_data);

typedef void (*orionpp_function_callback)(const OrionPPFunction* func, size_t index, void* user_data);
OrionPPError orionpp_reader_iterate_functions(OrionPPReader* reader,
                                             orionpp_function_callback callback, void* user_data);

// Disassembly support
OrionPPError orionpp_reader_disassemble_function(OrionPPReader* reader, size_t function_index, FILE* output);
OrionPPError orionpp_reader_disassemble_all(OrionPPReader* reader, FILE* output);

// Human format parsing (for .hopp files)
typedef struct {
  FILE* input;
  int line_number;
  int column;
  char* current_line;
  size_t line_capacity;
} OrionPPHumanParser;

OrionPPHumanParser* orionpp_human_parser_create(FILE* input);
void orionpp_human_parser_destroy(OrionPPHumanParser* parser);
OrionPPError orionpp_human_parser_parse(OrionPPHumanParser* parser, OrionPPContext* context);

// Validation
OrionPPError orionpp_reader_validate(OrionPPReader* reader);
OrionPPError orionpp_reader_check_magic(FILE* input);

#endif // ORIONPP_READER_H