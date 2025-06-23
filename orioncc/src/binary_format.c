#include "binary_format.h"

static void write_uint8(FILE *file, uint8_t value) {
  fwrite(&value, sizeof(uint8_t), 1, file);
}

static void write_uint16(FILE *file, uint16_t value) {
  fwrite(&value, sizeof(uint16_t), 1, file);
}

static void write_uint32(FILE *file, uint32_t value) {
  fwrite(&value, sizeof(uint32_t), 1, file);
}

static void write_int32(FILE *file, int32_t value) {
  fwrite(&value, sizeof(int32_t), 1, file);
}

OrionError binary_writer_init(BinaryWriter *writer, FILE *file) {
  if (!writer || !file) return ORION_ERROR_INVALID_ARGUMENT;
  
  memset(writer, 0, sizeof(BinaryWriter));
  writer->file = file;
  writer->position = 0;
  
  // Initialize string table with null string at offset 0
  writer->string_table_capacity = 1024;
  writer->string_table = safe_malloc(writer->string_table_capacity);
  writer->string_table[0] = '\0';
  writer->string_table_size = 1;
  
  // Initialize symbol table
  writer->symbol_capacity = 64;
  writer->symbols = safe_malloc(sizeof(OrionPPSymbol) * writer->symbol_capacity);
  
  // Initialize function table
  writer->function_capacity = 16;
  writer->functions = safe_malloc(sizeof(OrionPPFunction) * writer->function_capacity);
  
  // Initialize instruction buffer
  writer->instruction_capacity = 4096;
  writer->instructions = safe_malloc(writer->instruction_capacity);
  
  // Initialize variable table
  writer->variables.capacity = 256;
  writer->variables.names = safe_malloc(sizeof(char*) * writer->variables.capacity);
  writer->variables.ids = safe_malloc(sizeof(uint32_t) * writer->variables.capacity);
  
  // Initialize label table
  writer->labels.capacity = 128;
  writer->labels.names = safe_malloc(sizeof(char*) * writer->labels.capacity);
  writer->labels.offsets = safe_malloc(sizeof(uint32_t) * writer->labels.capacity);
  writer->labels.resolved = safe_malloc(sizeof(bool) * writer->labels.capacity);
  
  writer->next_temp_id = VAR_TEMP_BASE;
  
  return ORION_SUCCESS;
}

void binary_writer_cleanup(BinaryWriter *writer) {
  if (!writer) return;
  
  free(writer->string_table);
  free(writer->symbols);
  free(writer->functions);
  free(writer->instructions);
  
  // Clean up variable table
  for (size_t i = 0; i < writer->variables.count; i++) {
    free(writer->variables.names[i]);
  }
  free(writer->variables.names);
  free(writer->variables.ids);
  
  // Clean up label table
  for (size_t i = 0; i < writer->labels.count; i++) {
    free(writer->labels.names[i]);
  }
  free(writer->labels.names);
  free(writer->labels.offsets);
  free(writer->labels.resolved);
  
  memset(writer, 0, sizeof(BinaryWriter));
}

uint32_t binary_writer_add_string(BinaryWriter *writer, const char *str) {
  if (!writer || !str) return 0;
  
  // Check if string already exists
  for (size_t i = 1; i < writer->string_table_size; i++) {
    if (strcmp(&writer->string_table[i], str) == 0) {
      return i;
    }
    // Skip to next string
    i += strlen(&writer->string_table[i]);
  }
  
  // Add new string
  size_t len = strlen(str) + 1;
  uint32_t offset = writer->string_table_size;
  
  // Expand table if needed
  if (writer->string_table_size + len > writer->string_table_capacity) {
    while (writer->string_table_capacity < writer->string_table_size + len) {
      writer->string_table_capacity *= 2;
    }
    writer->string_table = safe_realloc(writer->string_table, writer->string_table_capacity);
  }
  
  strcpy(&writer->string_table[offset], str);
  writer->string_table_size += len;
  
  return offset;
}

uint32_t binary_writer_get_variable_id(BinaryWriter *writer, const char *name) {
  if (!writer || !name) return 0;
  
  // Look for existing variable
  for (size_t i = 0; i < writer->variables.count; i++) {
    if (strcmp(writer->variables.names[i], name) == 0) {
      return writer->variables.ids[i];
    }
  }
  
  // Add new variable
  if (writer->variables.count >= writer->variables.capacity) {
    writer->variables.capacity *= 2;
    writer->variables.names = safe_realloc(writer->variables.names, 
                                          sizeof(char*) * writer->variables.capacity);
    writer->variables.ids = safe_realloc(writer->variables.ids, 
                                        sizeof(uint32_t) * writer->variables.capacity);
  }
  
  uint32_t id = VAR_LOCAL_BASE + writer->variables.count;
  writer->variables.names[writer->variables.count] = string_duplicate(name);
  writer->variables.ids[writer->variables.count] = id;
  writer->variables.count++;
  
  return id;
}

uint32_t binary_writer_get_temp_variable_id(BinaryWriter *writer) {
  if (!writer) return 0;
  return writer->next_temp_id++;
}

uint32_t binary_writer_add_label(BinaryWriter *writer, const char *name) {
  if (!writer || !name) return 0;
  
  // Check if label already exists
  for (size_t i = 0; i < writer->labels.count; i++) {
    if (strcmp(writer->labels.names[i], name) == 0) {
      return i;
    }
  }
  
  // Add new label
  if (writer->labels.count >= writer->labels.capacity) {
    writer->labels.capacity *= 2;
    writer->labels.names = safe_realloc(writer->labels.names, 
                                       sizeof(char*) * writer->labels.capacity);
    writer->labels.offsets = safe_realloc(writer->labels.offsets, 
                                         sizeof(uint32_t) * writer->labels.capacity);
    writer->labels.resolved = safe_realloc(writer->labels.resolved, 
                                          sizeof(bool) * writer->labels.capacity);
  }
  
  uint32_t index = writer->labels.count;
  writer->labels.names[index] = string_duplicate(name);
  writer->labels.offsets[index] = 0;
  writer->labels.resolved[index] = false;
  writer->labels.count++;
  
  return index;
}

void binary_writer_resolve_label(BinaryWriter *writer, const char *name, uint32_t offset) {
  if (!writer || !name) return;
  
  for (size_t i = 0; i < writer->labels.count; i++) {
    if (strcmp(writer->labels.names[i], name) == 0) {
      writer->labels.offsets[i] = offset;
      writer->labels.resolved[i] = true;
      return;
    }
  }
}

OrionError binary_writer_begin_function(BinaryWriter *writer, const char *name, 
                                       uint16_t param_count, uint8_t return_type) {
  if (!writer || !name) return ORION_ERROR_INVALID_ARGUMENT;
  
  // Expand function table if needed
  if (writer->function_count >= writer->function_capacity) {
    writer->function_capacity *= 2;
    writer->functions = safe_realloc(writer->functions, 
                                    sizeof(OrionPPFunction) * writer->function_capacity);
  }
  
  OrionPPFunction *func = &writer->functions[writer->function_count];
  func->name_offset = binary_writer_add_string(writer, name);
  func->param_count = param_count;
  func->local_var_count = 0;  // Will be updated when function ends
  func->instruction_offset = writer->instruction_size;
  func->instruction_count = 0;  // Will be updated when function ends
  func->abi_type = ABI_TYPE_C;
  func->return_type = return_type;
  func->flags = 0;
  
  writer->function_count++;
  
  return ORION_SUCCESS;
}

OrionError binary_writer_end_function(BinaryWriter *writer) {
  if (!writer || writer->function_count == 0) return ORION_ERROR_INVALID_ARGUMENT;
  
  OrionPPFunction *func = &writer->functions[writer->function_count - 1];
  func->instruction_count = writer->instruction_size - func->instruction_offset;
  func->local_var_count = writer->variables.count;
  
  // Reset variable table for next function
  for (size_t i = 0; i < writer->variables.count; i++) {
    free(writer->variables.names[i]);
  }
  writer->variables.count = 0;
  
  return ORION_SUCCESS;
}

static OrionError ensure_instruction_capacity(BinaryWriter *writer, size_t additional) {
  if (writer->instruction_size + additional > writer->instruction_capacity) {
    while (writer->instruction_capacity < writer->instruction_size + additional) {
      writer->instruction_capacity *= 2;
    }
    writer->instructions = safe_realloc(writer->instructions, writer->instruction_capacity);
  }
  return ORION_SUCCESS;
}

OrionError binary_writer_emit_opcode(BinaryWriter *writer, uint8_t opcode) {
  if (!writer) return ORION_ERROR_INVALID_ARGUMENT;
  
  ensure_instruction_capacity(writer, 2);
  writer->instructions[writer->instruction_size++] = opcode;
  writer->instructions[writer->instruction_size++] = 0; // flags
  
  return ORION_SUCCESS;
}

OrionError binary_writer_emit_operand_immediate(BinaryWriter *writer, int32_t value) {
  if (!writer) return ORION_ERROR_INVALID_ARGUMENT;
  
  ensure_instruction_capacity(writer, 5);
  writer->instructions[writer->instruction_size++] = OPERAND_IMMEDIATE;
  memcpy(&writer->instructions[writer->instruction_size], &value, sizeof(int32_t));
  writer->instruction_size += sizeof(int32_t);
  
  return ORION_SUCCESS;
}

OrionError binary_writer_emit_operand_variable(BinaryWriter *writer, uint32_t var_id) {
  if (!writer) return ORION_ERROR_INVALID_ARGUMENT;
  
  ensure_instruction_capacity(writer, 5);
  writer->instructions[writer->instruction_size++] = OPERAND_VARIABLE;
  memcpy(&writer->instructions[writer->instruction_size], &var_id, sizeof(uint32_t));
  writer->instruction_size += sizeof(uint32_t);
  
  return ORION_SUCCESS;
}

OrionError binary_writer_emit_operand_label(BinaryWriter *writer, const char *label) {
  if (!writer || !label) return ORION_ERROR_INVALID_ARGUMENT;
  
  uint32_t label_index = binary_writer_add_label(writer, label);
  
  ensure_instruction_capacity(writer, 5);
  writer->instructions[writer->instruction_size++] = OPERAND_LABEL;
  memcpy(&writer->instructions[writer->instruction_size], &label_index, sizeof(uint32_t));
  writer->instruction_size += sizeof(uint32_t);
  
  return ORION_SUCCESS;
}

// High-level instruction emission
OrionError binary_writer_emit_let(BinaryWriter *writer, uint32_t dest, uint32_t src) {
  OrionError error = binary_writer_emit_opcode(writer, OP_LET);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_const(BinaryWriter *writer, uint32_t dest, int32_t value) {
  OrionError error = binary_writer_emit_opcode(writer, OP_CONST);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_immediate(writer, value);
}

OrionError binary_writer_emit_mov(BinaryWriter *writer, uint32_t dest, uint32_t src) {
  OrionError error = binary_writer_emit_opcode(writer, OP_MOV);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_add(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_ADD);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_sub(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_SUB);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_mul(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_MUL);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_div(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_DIV);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_mod(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_MOD);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_neg(BinaryWriter *writer, uint32_t dest, uint32_t src) {
  OrionError error = binary_writer_emit_opcode(writer, OP_NEG);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_and(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_AND);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_or(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_OR);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_xor(BinaryWriter *writer, uint32_t dest, uint32_t src1, uint32_t src2) {
  OrionError error = binary_writer_emit_opcode(writer, OP_XOR);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src2);
}

OrionError binary_writer_emit_not(BinaryWriter *writer, uint32_t dest, uint32_t src) {
  OrionError error = binary_writer_emit_opcode(writer, OP_NOT);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_shl(BinaryWriter *writer, uint32_t dest, uint32_t src, uint32_t count) {
  OrionError error = binary_writer_emit_opcode(writer, OP_SHL);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, count);
}

OrionError binary_writer_emit_shr(BinaryWriter *writer, uint32_t dest, uint32_t src, uint32_t count) {
  OrionError error = binary_writer_emit_opcode(writer, OP_SHR);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, count);
}

// Control flow
OrionError binary_writer_emit_enter(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_ENTER);
}

OrionError binary_writer_emit_leave(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_LEAVE);
}

OrionError binary_writer_emit_ret(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_RET);
}

OrionError binary_writer_emit_jmp(BinaryWriter *writer, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_JMP);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_call(BinaryWriter *writer, const char *function) {
  OrionError error = binary_writer_emit_opcode(writer, OP_CALL);
  if (error != ORION_SUCCESS) return error;
  
  uint32_t func_offset = binary_writer_add_string(writer, function);
  
  ensure_instruction_capacity(writer, 5);
  writer->instructions[writer->instruction_size++] = OPERAND_STRING;
  memcpy(&writer->instructions[writer->instruction_size], &func_offset, sizeof(uint32_t));
  writer->instruction_size += sizeof(uint32_t);
  
  return ORION_SUCCESS;
}

OrionError binary_writer_emit_beq(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BEQ);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_bne(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BNE);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_blt(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BLT);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_ble(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BLE);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_bgt(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BGT);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_bge(BinaryWriter *writer, uint32_t src1, uint32_t src2, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BGE);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src1);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src2);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_beqz(BinaryWriter *writer, uint32_t src, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BEQZ);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

OrionError binary_writer_emit_bnez(BinaryWriter *writer, uint32_t src, const char *label) {
  OrionError error = binary_writer_emit_opcode(writer, OP_BNEZ);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, src);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_label(writer, label);
}

// ABI operations
OrionError binary_writer_emit_abi_callee_enter(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_ABI_CALLEE_ENTER);
}

OrionError binary_writer_emit_abi_callee_leave(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_ABI_CALLEE_LEAVE);
}

OrionError binary_writer_emit_abi_callee_getarg(BinaryWriter *writer, uint32_t dest, int index) {
  OrionError error = binary_writer_emit_opcode(writer, OP_ABI_CALLEE_GETARG);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_immediate(writer, index);
}

OrionError binary_writer_emit_abi_callee_setret(BinaryWriter *writer, uint32_t src, int index) {
  OrionError error = binary_writer_emit_opcode(writer, OP_ABI_CALLEE_SETRET);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_immediate(writer, index);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_abi_caller_setup(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_ABI_CALLER_SETUP);
}

OrionError binary_writer_emit_abi_caller_cleanup(BinaryWriter *writer) {
  return binary_writer_emit_opcode(writer, OP_ABI_CALLER_CLEANUP);
}

OrionError binary_writer_emit_abi_caller_setarg(BinaryWriter *writer, uint32_t src, int index) {
  OrionError error = binary_writer_emit_opcode(writer, OP_ABI_CALLER_SETARG);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_immediate(writer, index);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_variable(writer, src);
}

OrionError binary_writer_emit_abi_caller_getret(BinaryWriter *writer, uint32_t dest, int index) {
  OrionError error = binary_writer_emit_opcode(writer, OP_ABI_CALLER_GETRET);
  if (error != ORION_SUCCESS) return error;
  
  error = binary_writer_emit_operand_variable(writer, dest);
  if (error != ORION_SUCCESS) return error;
  
  return binary_writer_emit_operand_immediate(writer, index);
}

OrionError binary_writer_finalize(BinaryWriter *writer) {
  if (!writer || !writer->file) return ORION_ERROR_INVALID_ARGUMENT;
  
  // Calculate offsets
  OrionPPHeader header = {0};
  header.magic = ORIONPP_MAGIC;
  header.version = ORIONPP_VERSION;
  header.flags = 0;
  
  size_t current_offset = sizeof(OrionPPHeader);
  
  // String table
  header.string_table_offset = current_offset;
  header.string_table_size = writer->string_table_size;
  current_offset += header.string_table_size;
  
  // Symbol table
  header.symbol_table_offset = current_offset;
  header.symbol_table_size = writer->symbol_count * sizeof(OrionPPSymbol);
  current_offset += header.symbol_table_size;
  
  // Function table
  header.function_table_offset = current_offset;
  header.function_table_size = writer->function_count * sizeof(OrionPPFunction);
  current_offset += header.function_table_size;
  
  // Instruction data
  header.instruction_data_offset = current_offset;
  header.instruction_data_size = writer->instruction_size;
  
  header.entry_point = 0; // First function is entry point
  
  // Write header
  fwrite(&header, sizeof(OrionPPHeader), 1, writer->file);
  
  // Write string table
  fwrite(writer->string_table, 1, writer->string_table_size, writer->file);
  
  // Write symbol table
  if (writer->symbol_count > 0) {
    fwrite(writer->symbols, sizeof(OrionPPSymbol), writer->symbol_count, writer->file);
  }
  
  // Write function table
  if (writer->function_count > 0) {
    fwrite(writer->functions, sizeof(OrionPPFunction), writer->function_count, writer->file);
  }
  
  // Write instruction data
  if (writer->instruction_size > 0) {
    fwrite(writer->instructions, 1, writer->instruction_size, writer->file);
  }
  
  return ORION_SUCCESS;
}