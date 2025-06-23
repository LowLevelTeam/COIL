#include "../include/orionpp_reader.h"
#include <stdlib.h>
#include <string.h>

static void* safe_malloc(size_t size) {
  void* ptr = malloc(size);
  if (!ptr && size > 0) {
    fprintf(stderr, "Fatal: Memory allocation failed\n");
    exit(1);
  }
  return ptr;
}

OrionPPReader* orionpp_reader_create(FILE* input) {
  if (!input) return NULL;
  
  OrionPPReader* reader = safe_malloc(sizeof(OrionPPReader));
  memset(reader, 0, sizeof(OrionPPReader));
  
  reader->context = orionpp_context_create();
  reader->input = input;
  reader->format = ORIONPP_FORMAT_BINARY; // Default, may be detected
  
  return reader;
}

void orionpp_reader_destroy(OrionPPReader* reader) {
  if (!reader) return;
  
  orionpp_context_destroy(reader->context);
  free(reader->string_table);
  free(reader->symbols);
  free(reader->functions);
  free(reader->variables);
  free(reader->instructions);
  free(reader);
}

OrionPPError orionpp_reader_load(OrionPPReader* reader) {
  if (!reader || !reader->input) return ORIONPP_ERR_INVALID_FORMAT;
  
  // Read header
  if (fread(&reader->header, sizeof(OrionPPHeader), 1, reader->input) != 1) {
    return ORIONPP_ERR_INVALID_FORMAT;
  }
  
  // Verify magic number
  if (reader->header.magic != ORIONPP_MAGIC) {
    return ORIONPP_ERR_INVALID_MAGIC;
  }
  
  // Verify version
  if (reader->header.version != ORIONPP_VERSION) {
    return ORIONPP_ERR_INVALID_VERSION;
  }
  
  // Load string table
  if (reader->header.string_table_size > 0) {
    reader->string_table = safe_malloc(reader->header.string_table_size);
    fseek(reader->input, reader->header.string_table_offset, SEEK_SET);
    if (fread(reader->string_table, 1, reader->header.string_table_size, reader->input) != reader->header.string_table_size) {
      return ORIONPP_ERR_INVALID_FORMAT;
    }
  }
  
  // Load symbol table
  if (reader->header.symbol_table_size > 0) {
    reader->symbol_count = reader->header.symbol_table_size / sizeof(OrionPPSymbol);
    reader->symbols = safe_malloc(reader->header.symbol_table_size);
    fseek(reader->input, reader->header.symbol_table_offset, SEEK_SET);
    if (fread(reader->symbols, sizeof(OrionPPSymbol), reader->symbol_count, reader->input) != reader->symbol_count) {
      return ORIONPP_ERR_INVALID_FORMAT;
    }
  }
  
  // Load function table
  if (reader->header.function_table_size > 0) {
    reader->function_count = reader->header.function_table_size / sizeof(OrionPPFunction);
    reader->functions = safe_malloc(reader->header.function_table_size);
    fseek(reader->input, reader->header.function_table_offset, SEEK_SET);
    if (fread(reader->functions, sizeof(OrionPPFunction), reader->function_count, reader->input) != reader->function_count) {
      return ORIONPP_ERR_INVALID_FORMAT;
    }
  }
  
  // Load variable table
  if (reader->header.variable_table_size > 0) {
    reader->variable_count = reader->header.variable_table_size / sizeof(OrionPPVariable);
    reader->variables = safe_malloc(reader->header.variable_table_size);
    fseek(reader->input, reader->header.variable_table_offset, SEEK_SET);
    if (fread(reader->variables, sizeof(OrionPPVariable), reader->variable_count, reader->input) != reader->variable_count) {
      return ORIONPP_ERR_INVALID_FORMAT;
    }
  }
  
  // Load instruction data
  if (reader->header.instruction_data_size > 0) {
    reader->instructions = safe_malloc(reader->header.instruction_data_size);
    fseek(reader->input, reader->header.instruction_data_offset, SEEK_SET);
    if (fread(reader->instructions, 1, reader->header.instruction_data_size, reader->input) != reader->header.instruction_data_size) {
      return ORIONPP_ERR_INVALID_FORMAT;
    }
  }
  
  return ORIONPP_OK;
}

const OrionPPHeader* orionpp_reader_get_header(OrionPPReader* reader) {
  return reader ? &reader->header : NULL;
}

const char* orionpp_reader_get_string(OrionPPReader* reader, uint32_t offset) {
  if (!reader || !reader->string_table || offset >= reader->header.string_table_size) {
    return NULL;
  }
  return &reader->string_table[offset];
}

size_t orionpp_reader_get_symbol_count(OrionPPReader* reader) {
  return reader ? reader->symbol_count : 0;
}

const OrionPPSymbol* orionpp_reader_get_symbol(OrionPPReader* reader, size_t index) {
  if (!reader || index >= reader->symbol_count) return NULL;
  return &reader->symbols[index];
}

const OrionPPSymbol* orionpp_reader_find_symbol(OrionPPReader* reader, const char* name) {
  if (!reader || !name) return NULL;
  
  for (size_t i = 0; i < reader->symbol_count; i++) {
    const char* symbol_name = orionpp_reader_get_string(reader, reader->symbols[i].name_offset);
    if (symbol_name && strcmp(symbol_name, name) == 0) {
      return &reader->symbols[i];
    }
  }
  return NULL;
}

size_t orionpp_reader_get_function_count(OrionPPReader* reader) {
  return reader ? reader->function_count : 0;
}

const OrionPPFunction* orionpp_reader_get_function(OrionPPReader* reader, size_t index) {
  if (!reader || index >= reader->function_count) return NULL;
  return &reader->functions[index];
}

const OrionPPFunction* orionpp_reader_find_function(OrionPPReader* reader, const char* name) {
  if (!reader || !name) return NULL;
  
  for (size_t i = 0; i < reader->function_count; i++) {
    const OrionPPSymbol* symbol = orionpp_reader_get_symbol(reader, reader->functions[i].symbol_offset);
    if (symbol) {
      const char* func_name = orionpp_reader_get_string(reader, symbol->name_offset);
      if (func_name && strcmp(func_name, name) == 0) {
        return &reader->functions[i];
      }
    }
  }
  return NULL;
}

size_t orionpp_reader_get_variable_count(OrionPPReader* reader) {
  return reader ? reader->variable_count : 0;
}

const OrionPPVariable* orionpp_reader_get_variable(OrionPPReader* reader, uint32_t var_id) {
  if (!reader) return NULL;
  
  for (size_t i = 0; i < reader->variable_count; i++) {
    if (reader->variables[i].id == var_id) {
      return &reader->variables[i];
    }
  }
  return NULL;
}

const char* orionpp_reader_get_variable_debug_name(OrionPPReader* reader, uint32_t var_id) {
  const OrionPPVariable* var = orionpp_reader_get_variable(reader, var_id);
  if (!var || !var->name_offset) return NULL;
  return orionpp_reader_get_string(reader, var->name_offset);
}

const uint8_t* orionpp_reader_get_instructions(OrionPPReader* reader) {
  return reader ? reader->instructions : NULL;
}

size_t orionpp_reader_get_instruction_size(OrionPPReader* reader) {
  return reader ? reader->header.instruction_data_size : 0;
}

OrionPPError orionpp_reader_decode_instruction(OrionPPReader* reader, size_t offset,
                                              OrionPPInstruction* instruction, size_t* next_offset) {
  if (!reader || !instruction || !reader->instructions || offset >= reader->header.instruction_data_size) {
    return ORIONPP_ERR_INVALID_FORMAT;
  }
  
  memset(instruction, 0, sizeof(OrionPPInstruction));
  
  const uint8_t* data = &reader->instructions[offset];
  size_t pos = 0;
  
  // Read opcode and flags
  if (offset + pos + 2 > reader->header.instruction_data_size) {
    return ORIONPP_ERR_INVALID_FORMAT;
  }
  
  instruction->opcode = data[pos++];
  instruction->flags = data[pos++];
  
  // Decode operands based on opcode
  // This is a simplified decoder - full implementation would handle all opcodes
  switch (instruction->opcode) {
    case ORIONPP_OP_CONST:
      // const dest, immediate
      instruction->operand_count = 2;
      
      // First operand: variable
      if (offset + pos + 5 > reader->header.instruction_data_size) return ORIONPP_ERR_INVALID_FORMAT;
      if (data[pos] != ORIONPP_OPERAND_VARIABLE) return ORIONPP_ERR_INVALID_FORMAT;
      pos++;
      memcpy(&instruction->operands[0].value.variable_id, &data[pos], sizeof(uint32_t));
      instruction->operands[0].type = ORIONPP_OPERAND_VARIABLE;
      pos += sizeof(uint32_t);
      
      // Second operand: immediate
      if (offset + pos + 5 > reader->header.instruction_data_size) return ORIONPP_ERR_INVALID_FORMAT;
      if (data[pos] != ORIONPP_OPERAND_IMMEDIATE) return ORIONPP_ERR_INVALID_FORMAT;
      pos++;
      memcpy(&instruction->operands[1].value.immediate, &data[pos], sizeof(int32_t));
      instruction->operands[1].type = ORIONPP_OPERAND_IMMEDIATE;
      pos += sizeof(int32_t);
      break;
      
    case ORIONPP_OP_ADD:
    case ORIONPP_OP_SUB:
    case ORIONPP_OP_MUL:
      // Binary ops: dest, src1, src2
      instruction->operand_count = 3;
      for (int i = 0; i < 3; i++) {
        if (offset + pos + 5 > reader->header.instruction_data_size) return ORIONPP_ERR_INVALID_FORMAT;
        if (data[pos] != ORIONPP_OPERAND_VARIABLE) return ORIONPP_ERR_INVALID_FORMAT;
        pos++;
        memcpy(&instruction->operands[i].value.variable_id, &data[pos], sizeof(uint32_t));
        instruction->operands[i].type = ORIONPP_OPERAND_VARIABLE;
        pos += sizeof(uint32_t);
      }
      break;
      
    default:
      // For now, just skip unhandled opcodes
      instruction->operand_count = 0;
      break;
  }
  
  if (next_offset) {
    *next_offset = offset + pos;
  }
  
  return ORIONPP_OK;
}

OrionPPError orionpp_reader_iterate_instructions(OrionPPReader* reader,
                                                orionpp_instruction_callback callback, void* user_data) {
  if (!reader || !callback) return ORIONPP_ERR_INVALID_FORMAT;
  
  size_t offset = 0;
  while (offset < reader->header.instruction_data_size) {
    OrionPPInstruction instruction;
    size_t next_offset;
    
    OrionPPError error = orionpp_reader_decode_instruction(reader, offset, &instruction, &next_offset);
    if (error != ORIONPP_OK) {
      return error;
    }
    
    callback(&instruction, offset, user_data);
    
    if (next_offset <= offset) {
      // Prevent infinite loop
      break;
    }
    offset = next_offset;
  }
  
  return ORIONPP_OK;
}

OrionPPError orionpp_reader_iterate_functions(OrionPPReader* reader,
                                             orionpp_function_callback callback, void* user_data) {
  if (!reader || !callback) return ORIONPP_ERR_INVALID_FORMAT;
  
  for (size_t i = 0; i < reader->function_count; i++) {
    callback(&reader->functions[i], i, user_data);
  }
  
  return ORIONPP_OK;
}

static void disasm_print_operand(FILE* output, const OrionPPInstruction* instr, size_t operand_index, OrionPPReader* reader) {
  if (operand_index >= instr->operand_count) return;
  
  const struct {
    uint8_t type;
    union {
      int32_t immediate;
      uint32_t variable_id;
      uint32_t label_offset;
      uint32_t symbol_offset;
    } value;
  }* operand = &instr->operands[operand_index];
  
  switch (operand->type) {
    case ORIONPP_OPERAND_IMMEDIATE:
      fprintf(output, "%d", operand->value.immediate);
      break;
    case ORIONPP_OPERAND_VARIABLE: {
      const char* debug_name = orionpp_reader_get_variable_debug_name(reader, operand->value.variable_id);
      if (debug_name) {
        fprintf(output, "%s", debug_name);
      } else {
        fprintf(output, "$%u", operand->value.variable_id);
      }
      break;
    }
    case ORIONPP_OPERAND_SYMBOL: {
      const char* symbol_name = orionpp_reader_get_string(reader, operand->value.symbol_offset);
      if (symbol_name) {
        fprintf(output, "@\"%s\"", symbol_name);
      } else {
        fprintf(output, "symbol_%u", operand->value.symbol_offset);
      }
      break;
    }
    case ORIONPP_OPERAND_LABEL:
      fprintf(output, ".L%u", operand->value.label_offset);
      break;
    default:
      fprintf(output, "?");
      break;
  }
}

static void disasm_instruction_callback(const OrionPPInstruction* instr, size_t offset, void* user_data) {
  FILE* output = (FILE*)user_data;
  (void)offset; // Suppress unused parameter warning
  
  // This would need to be extended to handle all opcodes
  switch (instr->opcode) {
    case ORIONPP_OP_NOP:
      fprintf(output, "    isa.nop\n");
      break;
    case ORIONPP_OP_ENTER:
      fprintf(output, "  isa.enter\n");
      break;
    case ORIONPP_OP_LEAVE:
      fprintf(output, "  isa.leave\n");
      break;
    case ORIONPP_OP_RET:
      fprintf(output, "    isa.RET\n");
      break;
    case ORIONPP_OP_CONST:
      fprintf(output, "    isa.const ");
      if (instr->operand_count >= 2) {
        disasm_print_operand(output, instr, 0, (OrionPPReader*)user_data);
        fprintf(output, ", ");
        disasm_print_operand(output, instr, 1, (OrionPPReader*)user_data);
      }
      fprintf(output, "\n");
      break;
    case ORIONPP_OP_ADD:
      fprintf(output, "    isa.add ");
      if (instr->operand_count >= 3) {
        disasm_print_operand(output, instr, 0, (OrionPPReader*)user_data);
        fprintf(output, ", ");
        disasm_print_operand(output, instr, 1, (OrionPPReader*)user_data);
        fprintf(output, ", ");
        disasm_print_operand(output, instr, 2, (OrionPPReader*)user_data);
      }
      fprintf(output, "\n");
      break;
    default:
      fprintf(output, "    ; Unknown opcode 0x%02X\n", instr->opcode);
      break;
  }
}

OrionPPError orionpp_reader_disassemble_function(OrionPPReader* reader, size_t function_index, FILE* output) {
  if (!reader || function_index >= reader->function_count || !output) {
    return ORIONPP_ERR_INVALID_FUNCTION;
  }
  
  const OrionPPFunction* func = &reader->functions[function_index];
  const OrionPPSymbol* symbol = orionpp_reader_get_symbol(reader, func->symbol_offset);
  
  if (symbol) {
    const char* func_name = orionpp_reader_get_string(reader, symbol->name_offset);
    fprintf(output, "obj.sym[global,func] @\"%s\":\n", func_name ? func_name : "unknown");
  }
  
  // Disassemble function instructions
  // This is simplified - full implementation would decode the specific function's instructions
  fprintf(output, "  ; Function has %u instruction bytes\n", func->instruction_count);
  
  return ORIONPP_OK;
}

OrionPPError orionpp_reader_disassemble_all(OrionPPReader* reader, FILE* output) {
  if (!reader || !output) return ORIONPP_ERR_INVALID_FORMAT;
  
  fprintf(output, "// Disassembly of Orion++ binary\n\n");
  
  for (size_t i = 0; i < reader->function_count; i++) {
    OrionPPError error = orionpp_reader_disassemble_function(reader, i, output);
    if (error != ORIONPP_OK) {
      return error;
    }
    fprintf(output, "\n");
  }
  
  return ORIONPP_OK;
}

OrionPPError orionpp_reader_validate(OrionPPReader* reader) {
  if (!reader) return ORIONPP_ERR_INVALID_FORMAT;
  
  // Check magic number
  if (reader->header.magic != ORIONPP_MAGIC) {
    return ORIONPP_ERR_INVALID_MAGIC;
  }
  
  // Check version
  if (reader->header.version != ORIONPP_VERSION) {
    return ORIONPP_ERR_INVALID_VERSION;
  }
  
  // Validate table offsets and sizes
  // This is basic validation - could be more thorough
  
  return ORIONPP_OK;
}

OrionPPError orionpp_reader_check_magic(FILE* input) {
  if (!input) return ORIONPP_ERR_INVALID_FORMAT;
  
  uint32_t magic;
  long pos = ftell(input);
  
  if (fread(&magic, sizeof(uint32_t), 1, input) != 1) {
    fseek(input, pos, SEEK_SET);
    return ORIONPP_ERR_INVALID_FORMAT;
  }
  
  fseek(input, pos, SEEK_SET);
  
  if (magic == ORIONPP_MAGIC) {
    return ORIONPP_OK;
  } else {
    return ORIONPP_ERR_INVALID_MAGIC;
  }
}