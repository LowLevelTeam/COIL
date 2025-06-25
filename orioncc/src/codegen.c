/**
 * @file src/codegen.c
 * @brief Code generator implementation for Orion++ IR
 */

#include "codegen.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
  #include <io.h>
#endif

// Helper function to get file handle
static file_handle_t get_file_handle(FILE* file) {
#ifdef WIN32
  return (HANDLE)_get_osfhandle(_fileno(file));
#else
  return fileno(file);
#endif
}

void codegen_init(CodeGen* codegen, FILE* output) {
  codegen->output = output;
  codegen->symbols = NULL;
  codegen->next_var_id = 0;
  codegen->next_label_id = 0;
  codegen->had_error = false;
}

void codegen_cleanup(CodeGen* codegen) {
  Symbol* current = codegen->symbols;
  while (current) {
    Symbol* next = current->next;
    free(current->name);
    free(current);
    current = next;
  }
}

void codegen_error(CodeGen* codegen, const char* message) {
  codegen->had_error = true;
  fprintf(stderr, "CodeGen Error: %s\n", message);
}

Symbol* codegen_find_symbol(CodeGen* codegen, const char* name) {
  Symbol* current = codegen->symbols;
  while (current) {
    if (strcmp(current->name, name) == 0) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

Symbol* codegen_add_symbol(CodeGen* codegen, const char* name, DataType type) {
  Symbol* symbol = safe_malloc(sizeof(Symbol));
  symbol->name = safe_strdup(name);
  symbol->type = type;
  symbol->var_id = codegen->next_var_id++;
  symbol->next = codegen->symbols;
  codegen->symbols = symbol;
  return symbol;
}

orionpp_variable_id_t codegen_get_temp_var(CodeGen* codegen) {
  return codegen->next_var_id++;
}

orionpp_label_id_t codegen_get_label(CodeGen* codegen) {
  return codegen->next_label_id++;
}

void emit_instruction(CodeGen* codegen, orionpp_opcode_t opcode, orionpp_opcode_module_t child) {
  orinopp_instruction_t instr;
  instr.root = opcode;
  instr.child = child;
  instr.values = NULL;
  instr.value_count = 0;
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
}

void emit_var_instruction(CodeGen* codegen, orionpp_variable_id_t var_id, orionpp_type_t type) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_VAR;
  instr.value_count = 2;
  instr.values = safe_malloc(2 * sizeof(orinopp_value_t));
  
  // Variable ID
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&var_id;
  instr.values[0].bytesize = sizeof(var_id);
  
  // Type
  instr.values[1].root = type;
  instr.values[1].child = 0;
  instr.values[1].bytes = NULL;
  instr.values[1].bytesize = 0;
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_const_instruction(CodeGen* codegen, orionpp_variable_id_t var_id, orionpp_type_t type, const void* data, size_t size) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_CONST;
  instr.value_count = 3;
  instr.values = safe_malloc(3 * sizeof(orinopp_value_t));
  
  // Variable ID
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&var_id;
  instr.values[0].bytesize = sizeof(var_id);
  
  // Type
  instr.values[1].root = type;
  instr.values[1].child = 0;
  instr.values[1].bytes = NULL;
  instr.values[1].bytesize = 0;
  
  // Value
  instr.values[2].root = type;
  instr.values[2].child = 0;
  instr.values[2].bytes = (char*)data;
  instr.values[2].bytesize = size;
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_mov_instruction(CodeGen* codegen, orionpp_variable_id_t dest, orionpp_variable_id_t src) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_MOV;
  instr.value_count = 2;
  instr.values = safe_malloc(2 * sizeof(orinopp_value_t));
  
  // Destination
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&dest;
  instr.values[0].bytesize = sizeof(dest);
  
  // Source
  instr.values[1].root = ORIONPP_TYPE_VARID;
  instr.values[1].child = 0;
  instr.values[1].bytes = (char*)&src;
  instr.values[1].bytesize = sizeof(src);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_binary_instruction(CodeGen* codegen, orionpp_opcode_module_t op, orionpp_variable_id_t dest, orionpp_variable_id_t left, orionpp_variable_id_t right) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = op;
  instr.value_count = 3;
  instr.values = safe_malloc(3 * sizeof(orinopp_value_t));
  
  // Destination
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&dest;
  instr.values[0].bytesize = sizeof(dest);
  
  // Left operand
  instr.values[1].root = ORIONPP_TYPE_VARID;
  instr.values[1].child = 0;
  instr.values[1].bytes = (char*)&left;
  instr.values[1].bytesize = sizeof(left);
  
  // Right operand
  instr.values[2].root = ORIONPP_TYPE_VARID;
  instr.values[2].child = 0;
  instr.values[2].bytes = (char*)&right;
  instr.values[2].bytesize = sizeof(right);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_unary_instruction(CodeGen* codegen, orionpp_opcode_module_t op, orionpp_variable_id_t dest, orionpp_variable_id_t operand) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = op;
  instr.value_count = 2;
  instr.values = safe_malloc(2 * sizeof(orinopp_value_t));
  
  // Destination
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&dest;
  instr.values[0].bytesize = sizeof(dest);
  
  // Operand
  instr.values[1].root = ORIONPP_TYPE_VARID;
  instr.values[1].child = 0;
  instr.values[1].bytes = (char*)&operand;
  instr.values[1].bytesize = sizeof(operand);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_label_instruction(CodeGen* codegen, orionpp_label_id_t label_id) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_LABEL;
  instr.value_count = 1;
  instr.values = safe_malloc(sizeof(orinopp_value_t));
  
  // Label ID
  instr.values[0].root = ORIONPP_TYPE_LABELID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&label_id;
  instr.values[0].bytesize = sizeof(label_id);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_jump_instruction(CodeGen* codegen, orionpp_label_id_t label_id) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_JMP;
  instr.value_count = 1;
  instr.values = safe_malloc(sizeof(orinopp_value_t));
  
  // Label ID
  instr.values[0].root = ORIONPP_TYPE_LABELID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&label_id;
  instr.values[0].bytesize = sizeof(label_id);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_branch_instruction(CodeGen* codegen, orionpp_variable_id_t condition, orionpp_label_id_t label_id) {
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_BR;
  instr.value_count = 2;
  instr.values = safe_malloc(2 * sizeof(orinopp_value_t));
  
  // Condition variable
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&condition;
  instr.values[0].bytesize = sizeof(condition);
  
  // Label ID
  instr.values[1].root = ORIONPP_TYPE_LABELID;
  instr.values[1].child = 0;
  instr.values[1].bytes = (char*)&label_id;
  instr.values[1].bytesize = sizeof(label_id);
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

void emit_call_instruction(CodeGen* codegen, const char* function_name, orionpp_variable_id_t* args, size_t arg_count, orionpp_variable_id_t result) {
  // For simplicity, emit call with function name as symbol
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_CALL;
  instr.value_count = 2 + arg_count;
  instr.values = safe_malloc((2 + arg_count) * sizeof(orinopp_value_t));
  
  // Result variable
  instr.values[0].root = ORIONPP_TYPE_VARID;
  instr.values[0].child = 0;
  instr.values[0].bytes = (char*)&result;
  instr.values[0].bytesize = sizeof(result);
  
  // Function name as symbol
  instr.values[1].root = ORIONPP_TYPE_SYMBOL;
  instr.values[1].child = 0;
  instr.values[1].bytes = (char*)function_name;
  instr.values[1].bytesize = strlen(function_name) + 1;
  
  // Arguments
  for (size_t i = 0; i < arg_count; i++) {
    instr.values[2 + i].root = ORIONPP_TYPE_VARID;
    instr.values[2 + i].child = 0;
    instr.values[2 + i].bytes = (char*)&args[i];
    instr.values[2 + i].bytesize = sizeof(args[i]);
  }
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  free(instr.values);
}

bool codegen_generate(CodeGen* codegen, const ASTNode* ast) {
  if (!ast) {
    codegen_error(codegen, "AST is null");
    return false;
  }
  
  codegen_program(codegen, ast);
  return !codegen->had_error;
}

void codegen_program(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_PROGRAM) {
    codegen_error(codegen, "Expected program node");
    return;
  }
  
  for (size_t i = 0; i < node->program.statement_count; i++) {
    const ASTNode* stmt = node->program.statements[i];
    if (stmt->type == AST_FUNCTION) {
      codegen_function(codegen, stmt);
    } else if (stmt->type == AST_VARIABLE_DECL) {
      codegen_variable_decl(codegen, stmt);
    } else {
      codegen_statement(codegen, stmt);
    }
  }
}

void codegen_function(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_FUNCTION) {
    codegen_error(codegen, "Expected function node");
    return;
  }
  
  // Add function symbol
  codegen_add_symbol(codegen, node->function.name, node->function.return_type);
  
  // Add function parameters to symbol table
  for (size_t i = 0; i < node->function.parameter_count; i++) {
    const ASTNode* param = node->function.parameters[i];
    if (param->type == AST_VARIABLE_DECL) {
      Symbol* param_symbol = codegen_add_symbol(codegen, param->variable_decl.name, param->variable_decl.type);
      
      // Emit parameter variable declaration
      orionpp_type_t opp_type;
      switch (param->variable_decl.type) {
        case TYPE_INT:
          opp_type = ORIONPP_TYPE_WORD;
          break;
        case TYPE_CHAR:
          opp_type = ORIONPP_TYPE_C;
          break;
        default:
          opp_type = ORIONPP_TYPE_WORD;
          break;
      }
      emit_var_instruction(codegen, param_symbol->var_id, opp_type);
    }
  }
  
  // Generate function body
  if (node->function.body) {
    codegen_statement(codegen, node->function.body);
  }
  
  // Emit function end hint
  emit_instruction(codegen, ORIONPP_OP_HINT, ORIONPP_OP_HINT_FUNCEND);
}

void codegen_variable_decl(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_VARIABLE_DECL) {
    codegen_error(codegen, "Expected variable declaration node");
    return;
  }
  
  Symbol* symbol = codegen_add_symbol(codegen, node->variable_decl.name, node->variable_decl.type);
  
  // Determine Orion++ type
  orionpp_type_t opp_type;
  switch (node->variable_decl.type) {
    case TYPE_INT:
      opp_type = ORIONPP_TYPE_WORD;
      break;
    case TYPE_CHAR:
      opp_type = ORIONPP_TYPE_C;
      break;
    default:
      opp_type = ORIONPP_TYPE_WORD;
      break;
  }
  
  if (node->variable_decl.initializer) {
    // Generate initializer expression
    orionpp_variable_id_t init_var = codegen_expression(codegen, node->variable_decl.initializer);
    
    // Emit variable declaration
    emit_var_instruction(codegen, symbol->var_id, opp_type);
    
    // Emit assignment
    emit_mov_instruction(codegen, symbol->var_id, init_var);
  } else {
    // Emit variable declaration without initialization
    emit_var_instruction(codegen, symbol->var_id, opp_type);
  }
}

void codegen_statement(CodeGen* codegen, const ASTNode* node) {
  if (!node) return;
  
  switch (node->type) {
    case AST_BLOCK:
      codegen_block(codegen, node);
      break;
    case AST_IF:
      codegen_if_statement(codegen, node);
      break;
    case AST_WHILE:
      codegen_while_statement(codegen, node);
      break;
    case AST_FOR:
      codegen_for_statement(codegen, node);
      break;
    case AST_RETURN:
      codegen_return_statement(codegen, node);
      break;
    case AST_ASSIGNMENT:
      codegen_assignment(codegen, node);
      break;
    case AST_EXPRESSION_STMT:
      codegen_expression(codegen, node->expression_stmt.expression);
      break;
    case AST_VARIABLE_DECL:
      codegen_variable_decl(codegen, node);
      break;
    case AST_FUNCTION:
      codegen_function(codegen, node);
      break;
    default:
      codegen_error(codegen, "Unknown statement type");
      break;
  }
}

void codegen_block(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_BLOCK) {
    codegen_error(codegen, "Expected block node");
    return;
  }
  
  for (size_t i = 0; i < node->block.statement_count; i++) {
    codegen_statement(codegen, node->block.statements[i]);
  }
}

void codegen_if_statement(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_IF) {
    codegen_error(codegen, "Expected if statement node");
    return;
  }
  
  // Generate condition
  orionpp_variable_id_t condition_var = codegen_expression(codegen, node->if_stmt.condition);
  
  // Create labels
  orionpp_label_id_t else_label = codegen_get_label(codegen);
  orionpp_label_id_t end_label = codegen_get_label(codegen);
  
  // Branch to else if condition is false (we need to negate the condition)
  orionpp_variable_id_t not_condition = codegen_get_temp_var(codegen);
  emit_unary_instruction(codegen, ORIONPP_OP_ISA_NOT, not_condition, condition_var);
  emit_branch_instruction(codegen, not_condition, else_label);
  
  // Generate then branch
  codegen_statement(codegen, node->if_stmt.then_branch);
  
  // Jump to end
  emit_jump_instruction(codegen, end_label);
  
  // Else label
  emit_label_instruction(codegen, else_label);
  
  // Generate else branch if it exists
  if (node->if_stmt.else_branch) {
    codegen_statement(codegen, node->if_stmt.else_branch);
  }
  
  // End label
  emit_label_instruction(codegen, end_label);
}

void codegen_while_statement(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_WHILE) {
    codegen_error(codegen, "Expected while statement node");
    return;
  }
  
  // Create labels
  orionpp_label_id_t loop_label = codegen_get_label(codegen);
  orionpp_label_id_t end_label = codegen_get_label(codegen);
  
  // Loop label
  emit_label_instruction(codegen, loop_label);
  
  // Generate condition
  orionpp_variable_id_t condition_var = codegen_expression(codegen, node->while_stmt.condition);
  
  // Branch to end if condition is false
  orionpp_variable_id_t not_condition = codegen_get_temp_var(codegen);
  emit_unary_instruction(codegen, ORIONPP_OP_ISA_NOT, not_condition, condition_var);
  emit_branch_instruction(codegen, not_condition, end_label);
  
  // Generate body
  codegen_statement(codegen, node->while_stmt.body);
  
  // Jump back to loop
  emit_jump_instruction(codegen, loop_label);
  
  // End label
  emit_label_instruction(codegen, end_label);
}

void codegen_for_statement(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_FOR) {
    codegen_error(codegen, "Expected for statement node");
    return;
  }
  
  // Generate initialization
  if (node->for_stmt.init) {
    codegen_statement(codegen, node->for_stmt.init);
  }
  
  // Create labels
  orionpp_label_id_t loop_label = codegen_get_label(codegen);
  orionpp_label_id_t end_label = codegen_get_label(codegen);
  
  // Loop label
  emit_label_instruction(codegen, loop_label);
  
  // Generate condition
  if (node->for_stmt.condition) {
    orionpp_variable_id_t condition_var = codegen_expression(codegen, node->for_stmt.condition);
    orionpp_variable_id_t not_condition = codegen_get_temp_var(codegen);
    emit_unary_instruction(codegen, ORIONPP_OP_ISA_NOT, not_condition, condition_var);
    emit_branch_instruction(codegen, not_condition, end_label);
  }
  
  // Generate body
  codegen_statement(codegen, node->for_stmt.body);
  
  // Generate update
  if (node->for_stmt.update) {
    codegen_expression(codegen, node->for_stmt.update);
  }
  
  // Jump back to loop
  emit_jump_instruction(codegen, loop_label);
  
  // End label
  emit_label_instruction(codegen, end_label);
}

void codegen_return_statement(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_RETURN) {
    codegen_error(codegen, "Expected return statement node");
    return;
  }
  
  orionpp_variable_id_t return_var = 0;
  if (node->return_stmt.value) {
    return_var = codegen_expression(codegen, node->return_stmt.value);
  }
  
  // Emit return instruction with value
  orinopp_instruction_t instr;
  instr.root = ORIONPP_OP_ISA;
  instr.child = ORIONPP_OP_ISA_RET;
  
  if (node->return_stmt.value) {
    instr.value_count = 1;
    instr.values = safe_malloc(sizeof(orinopp_value_t));
    instr.values[0].root = ORIONPP_TYPE_VARID;
    instr.values[0].child = 0;
    instr.values[0].bytes = (char*)&return_var;
    instr.values[0].bytesize = sizeof(return_var);
  } else {
    instr.value_count = 0;
    instr.values = NULL;
  }
  
  orionpp_writef(get_file_handle(codegen->output), &instr);
  if (instr.values) free(instr.values);
}

void codegen_assignment(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_ASSIGNMENT) {
    codegen_error(codegen, "Expected assignment node");
    return;
  }
  
  Symbol* symbol = codegen_find_symbol(codegen, node->assignment.name);
  if (!symbol) {
    codegen_error(codegen, "Undefined variable");
    return;
  }
  
  orionpp_variable_id_t value_var = codegen_expression(codegen, node->assignment.value);
  emit_mov_instruction(codegen, symbol->var_id, value_var);
}

orionpp_variable_id_t codegen_expression(CodeGen* codegen, const ASTNode* node) {
  if (!node) {
    codegen_error(codegen, "Expression node is null");
    return 0;
  }
  
  switch (node->type) {
    case AST_BINARY_OP:
      return codegen_binary_op(codegen, node);
    case AST_UNARY_OP:
      return codegen_unary_op(codegen, node);
    case AST_IDENTIFIER: {
      Symbol* symbol = codegen_find_symbol(codegen, node->identifier.name);
      if (!symbol) {
        codegen_error(codegen, "Undefined variable");
        return 0;
      }
      return symbol->var_id;
    }
    case AST_NUMBER: {
      orionpp_variable_id_t temp_var = codegen_get_temp_var(codegen);
      int32_t value = (int32_t)node->number.value;
      emit_const_instruction(codegen, temp_var, ORIONPP_TYPE_WORD, &value, sizeof(value));
      return temp_var;
    }
    case AST_CALL: {
      // Generate function call
      orionpp_variable_id_t result_var = codegen_get_temp_var(codegen);
      
      // Generate argument expressions
      orionpp_variable_id_t* arg_vars = NULL;
      if (node->call.argument_count > 0) {
        arg_vars = safe_malloc(node->call.argument_count * sizeof(orionpp_variable_id_t));
        for (size_t i = 0; i < node->call.argument_count; i++) {
          arg_vars[i] = codegen_expression(codegen, node->call.arguments[i]);
        }
      }
      
      // Emit call instruction
      emit_call_instruction(codegen, node->call.name, arg_vars, node->call.argument_count, result_var);
      
      if (arg_vars) free(arg_vars);
      return result_var;
    }
    case AST_STRING: {
      orionpp_variable_id_t temp_var = codegen_get_temp_var(codegen);
      emit_const_instruction(codegen, temp_var, ORIONPP_TYPE_STRING, 
                           node->string.value, strlen(node->string.value) + 1);
      return temp_var;
    }
    case AST_CHAR: {
      orionpp_variable_id_t temp_var = codegen_get_temp_var(codegen);
      char value = node->character.value;
      emit_const_instruction(codegen, temp_var, ORIONPP_TYPE_C, &value, sizeof(value));
      return temp_var;
    }
    case AST_ASSIGNMENT: {
      codegen_assignment(codegen, node);
      // Return the variable being assigned to
      Symbol* symbol = codegen_find_symbol(codegen, node->assignment.name);
      return symbol ? symbol->var_id : 0;
    }
    default:
      codegen_error(codegen, "Unknown expression type");
      return 0;
  }
}

orionpp_variable_id_t codegen_binary_op(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_BINARY_OP) {
    codegen_error(codegen, "Expected binary operation node");
    return 0;
  }
  
  orionpp_variable_id_t left_var = codegen_expression(codegen, node->binary_op.left);
  orionpp_variable_id_t right_var = codegen_expression(codegen, node->binary_op.right);
  orionpp_variable_id_t result_var = codegen_get_temp_var(codegen);
  
  orionpp_opcode_module_t op;
  switch (node->binary_op.operator) {
    case BINOP_ADD: op = ORIONPP_OP_ISA_ADD; break;
    case BINOP_SUB: op = ORIONPP_OP_ISA_SUB; break;
    case BINOP_MUL: op = ORIONPP_OP_ISA_MUL; break;
    case BINOP_DIV: op = ORIONPP_OP_ISA_DIV; break;
    case BINOP_MOD: op = ORIONPP_OP_ISA_MOD; break;
    // For comparison operators, we'll use subtraction and then check the result
    case BINOP_EQ:
    case BINOP_NE:
    case BINOP_LT:
    case BINOP_LE:
    case BINOP_GT:
    case BINOP_GE: {
      // For now, emit subtraction and let a later pass handle comparison logic
      emit_binary_instruction(codegen, ORIONPP_OP_ISA_SUB, result_var, left_var, right_var);
      return result_var;
    }
    case BINOP_AND:
    case BINOP_OR: {
      // Logical operations - use bitwise for now
      op = (node->binary_op.operator == BINOP_AND) ? ORIONPP_OP_ISA_AND : ORIONPP_OP_ISA_OR;
      break;
    }
    default:
      codegen_error(codegen, "Unsupported binary operator");
      return result_var;
  }
  
  emit_binary_instruction(codegen, op, result_var, left_var, right_var);
  return result_var;
}

orionpp_variable_id_t codegen_unary_op(CodeGen* codegen, const ASTNode* node) {
  if (node->type != AST_UNARY_OP) {
    codegen_error(codegen, "Expected unary operation node");
    return 0;
  }
  
  orionpp_variable_id_t operand_var = codegen_expression(codegen, node->unary_op.operand);
  orionpp_variable_id_t result_var = codegen_get_temp_var(codegen);
  
  switch (node->unary_op.operator) {
    case UNOP_MINUS: {
      // Negate by subtracting from zero
      orionpp_variable_id_t zero_var = codegen_get_temp_var(codegen);
      int32_t zero = 0;
      emit_const_instruction(codegen, zero_var, ORIONPP_TYPE_WORD, &zero, sizeof(zero));
      emit_binary_instruction(codegen, ORIONPP_OP_ISA_SUB, result_var, zero_var, operand_var);
      break;
    }
    case UNOP_NOT: {
      emit_unary_instruction(codegen, ORIONPP_OP_ISA_NOT, result_var, operand_var);
      break;
    }
    case UNOP_PRE_INC: {
      emit_unary_instruction(codegen, ORIONPP_OP_ISA_INC, result_var, operand_var);
      break;
    }
    case UNOP_PRE_DEC: {
      emit_unary_instruction(codegen, ORIONPP_OP_ISA_DEC, result_var, operand_var);
      break;
    }
    case UNOP_POST_INC: {
      emit_unary_instruction(codegen, ORIONPP_OP_ISA_INCp, result_var, operand_var);
      break;
    }
    case UNOP_POST_DEC: {
      emit_unary_instruction(codegen, ORIONPP_OP_ISA_DECp, result_var, operand_var);
      break;
    }
    default:
      codegen_error(codegen, "Unsupported unary operator");
      break;
  }
  
  return result_var;
}