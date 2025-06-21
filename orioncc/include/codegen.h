#ifndef ORION_CODEGEN_H
#define ORION_CODEGEN_H

#include "common.h"
#include "ast.h"

// Symbol table entry
typedef struct Symbol {
  char *name;
  TypeInfo type;
  int scope_level;
  bool is_parameter;
  int parameter_index;  // For function parameters
  struct Symbol *next;  // For hash table chaining
} Symbol;

// Symbol table
typedef struct SymbolTable {
  Symbol **buckets;
  size_t bucket_count;
  int current_scope;
} SymbolTable;

// Code generator state
typedef struct CodeGen {
  FILE *output;
  SymbolTable *symbols;
  CompilerOptions *options;
  
  // Label and temporary variable generation
  int label_counter;
  int temp_counter;
  
  // Current function context
  const char *current_function;
  bool in_function;
  
  // Loop context for break/continue
  const char *current_break_label;
  const char *current_continue_label;
  
  // Error tracking
  bool had_error;
  int error_count;
} CodeGen;

// Code generator API
OrionError codegen_init(CodeGen *gen, FILE *output, CompilerOptions *options);
void codegen_cleanup(CodeGen *gen);
OrionError codegen_generate_program(CodeGen *gen, ASTNode *program);

// Symbol table management
SymbolTable *symbol_table_create(void);
void symbol_table_destroy(SymbolTable *table);
OrionError symbol_table_define(SymbolTable *table, const char *name, TypeInfo type, bool is_param, int param_index);
Symbol *symbol_table_lookup(SymbolTable *table, const char *name);
void symbol_table_enter_scope(SymbolTable *table);
void symbol_table_exit_scope(SymbolTable *table);

// Code generation functions
OrionError codegen_function(CodeGen *gen, ASTNode *func);
OrionError codegen_statement(CodeGen *gen, ASTNode *stmt);
OrionError codegen_expression(CodeGen *gen, ASTNode *expr, const char *dest_var);

// Statement code generation
OrionError codegen_compound_stmt(CodeGen *gen, ASTNode *stmt);
OrionError codegen_if_stmt(CodeGen *gen, ASTNode *stmt);
OrionError codegen_while_stmt(CodeGen *gen, ASTNode *stmt);
OrionError codegen_for_stmt(CodeGen *gen, ASTNode *stmt);
OrionError codegen_return_stmt(CodeGen *gen, ASTNode *stmt);
OrionError codegen_var_decl(CodeGen *gen, ASTNode *stmt);
OrionError codegen_expression_stmt(CodeGen *gen, ASTNode *stmt);

// Expression code generation
OrionError codegen_assign_expr(CodeGen *gen, ASTNode *expr, const char *dest_var);
OrionError codegen_binary_expr(CodeGen *gen, ASTNode *expr, const char *dest_var);
OrionError codegen_unary_expr(CodeGen *gen, ASTNode *expr, const char *dest_var);
OrionError codegen_call_expr(CodeGen *gen, ASTNode *expr, const char *dest_var);
OrionError codegen_var_ref(CodeGen *gen, ASTNode *expr, const char *dest_var);
OrionError codegen_integer_literal(CodeGen *gen, ASTNode *expr, const char *dest_var);

// Helper functions
const char *codegen_get_temp_var(CodeGen *gen);
const char *codegen_get_label(CodeGen *gen);
void codegen_emit_comment(CodeGen *gen, const char *comment);
void codegen_emit_label(CodeGen *gen, const char *label);
void codegen_emit_instruction(CodeGen *gen, const char *instr, const char *operands);

// Orion++ ISA instruction emission
void codegen_emit_let(CodeGen *gen, const char *var, const char *value);
void codegen_emit_const(CodeGen *gen, const char *var, int value);
void codegen_emit_mov(CodeGen *gen, const char *dest, const char *src);
void codegen_emit_add(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_sub(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_mul(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_div(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_mod(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_neg(CodeGen *gen, const char *dest, const char *src);
void codegen_emit_and(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_or(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_xor(CodeGen *gen, const char *dest, const char *src1, const char *src2);
void codegen_emit_not(CodeGen *gen, const char *dest, const char *src);
void codegen_emit_shl(CodeGen *gen, const char *dest, const char *src, const char *count);
void codegen_emit_shr(CodeGen *gen, const char *dest, const char *src, const char *count);

// Control flow instructions
void codegen_emit_jmp(CodeGen *gen, const char *label);
void codegen_emit_call(CodeGen *gen, const char *function);
void codegen_emit_ret(CodeGen *gen);
void codegen_emit_beq(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_bne(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_blt(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_ble(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_bgt(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_bge(CodeGen *gen, const char *src1, const char *src2, const char *label);
void codegen_emit_beqz(CodeGen *gen, const char *src, const char *label);
void codegen_emit_bnez(CodeGen *gen, const char *src, const char *label);

// Object and symbol management
void codegen_emit_section(CodeGen *gen, const char *type, const char *name);
void codegen_emit_symbol(CodeGen *gen, const char *visibility, const char *type, const char *name);
void codegen_emit_hint_abi(CodeGen *gen, const char *abi);
void codegen_emit_hint_args(CodeGen *gen, const char *args);
void codegen_emit_hint_rets(CodeGen *gen, const char *rets);
void codegen_emit_hint_endfunc(CodeGen *gen);

// ABI support
void codegen_emit_abi_callee_enter(CodeGen *gen);
void codegen_emit_abi_callee_leave(CodeGen *gen);
void codegen_emit_abi_callee_getarg(CodeGen *gen, const char *dest, int index);
void codegen_emit_abi_callee_setret(CodeGen *gen, const char *src, int index);
void codegen_emit_abi_caller_setup(CodeGen *gen);
void codegen_emit_abi_caller_cleanup(CodeGen *gen);
void codegen_emit_abi_caller_setarg(CodeGen *gen, const char *src, int index);
void codegen_emit_abi_caller_getret(CodeGen *gen, const char *dest, int index);

// Scope management
void codegen_emit_enter(CodeGen *gen);
void codegen_emit_leave(CodeGen *gen);

// Error handling
void codegen_error(CodeGen *gen, const char *message, SourcePosition pos);

#endif // ORION_CODEGEN_H
