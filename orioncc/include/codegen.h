/**
 * @file include/codegen.h
 * @brief Code generator for Orion++ IR
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "orion/orionpp.h"
#include <stdio.h>

// Symbol table entry
typedef struct Symbol {
  char* name;
  DataType type;
  orionpp_variable_id_t var_id;
  struct Symbol* next;
} Symbol;

// Code generator state
typedef struct CodeGen {
  FILE* output;
  Symbol* symbols;
  orionpp_variable_id_t next_var_id;
  orionpp_label_id_t next_label_id;
  bool had_error;
} CodeGen;

// Function declarations
void codegen_init(CodeGen* codegen, FILE* output);
void codegen_cleanup(CodeGen* codegen);
bool codegen_generate(CodeGen* codegen, const ASTNode* ast);
void codegen_error(CodeGen* codegen, const char* message);

// Symbol table functions
Symbol* codegen_find_symbol(CodeGen* codegen, const char* name);
Symbol* codegen_add_symbol(CodeGen* codegen, const char* name, DataType type);
orionpp_variable_id_t codegen_get_temp_var(CodeGen* codegen);
orionpp_label_id_t codegen_get_label(CodeGen* codegen);

// Code generation functions
void codegen_program(CodeGen* codegen, const ASTNode* node);
void codegen_function(CodeGen* codegen, const ASTNode* node);
void codegen_variable_decl(CodeGen* codegen, const ASTNode* node);
void codegen_statement(CodeGen* codegen, const ASTNode* node);
void codegen_block(CodeGen* codegen, const ASTNode* node);
void codegen_if_statement(CodeGen* codegen, const ASTNode* node);
void codegen_while_statement(CodeGen* codegen, const ASTNode* node);
void codegen_for_statement(CodeGen* codegen, const ASTNode* node);
void codegen_return_statement(CodeGen* codegen, const ASTNode* node);
void codegen_assignment(CodeGen* codegen, const ASTNode* node);
orionpp_variable_id_t codegen_expression(CodeGen* codegen, const ASTNode* node);
orionpp_variable_id_t codegen_binary_op(CodeGen* codegen, const ASTNode* node);
orionpp_variable_id_t codegen_unary_op(CodeGen* codegen, const ASTNode* node);

// Instruction emission functions
void emit_instruction(CodeGen* codegen, orionpp_opcode_t opcode, orionpp_opcode_module_t child);
void emit_var_instruction(CodeGen* codegen, orionpp_variable_id_t var_id, orionpp_type_t type);
void emit_const_instruction(CodeGen* codegen, orionpp_variable_id_t var_id, orionpp_type_t type, const void* data, size_t size);
void emit_mov_instruction(CodeGen* codegen, orionpp_variable_id_t dest, orionpp_variable_id_t src);
void emit_binary_instruction(CodeGen* codegen, orionpp_opcode_module_t op, orionpp_variable_id_t dest, orionpp_variable_id_t left, orionpp_variable_id_t right);
void emit_unary_instruction(CodeGen* codegen, orionpp_opcode_module_t op, orionpp_variable_id_t dest, orionpp_variable_id_t operand);
void emit_label_instruction(CodeGen* codegen, orionpp_label_id_t label_id);
void emit_jump_instruction(CodeGen* codegen, orionpp_label_id_t label_id);
void emit_branch_instruction(CodeGen* codegen, orionpp_variable_id_t condition, orionpp_label_id_t label_id);
void emit_call_instruction(CodeGen* codegen, const char* function_name, orionpp_variable_id_t* args, size_t arg_count, orionpp_variable_id_t result);

#endif // CODEGEN_H