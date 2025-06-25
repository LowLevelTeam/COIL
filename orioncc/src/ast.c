/**
 * @file src/ast.c
 * @brief Abstract Syntax Tree implementation
 */

#include "ast.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* ast_create_node(ASTNodeType type) {
  ASTNode* node = safe_malloc(sizeof(ASTNode));
  memset(node, 0, sizeof(ASTNode));
  node->type = type;
  return node;
}

void ast_free_node(ASTNode* node) {
  if (!node) return;
  
  switch (node->type) {
    case AST_PROGRAM:
      for (size_t i = 0; i < node->program.statement_count; i++) {
        ast_free_node(node->program.statements[i]);
      }
      free(node->program.statements);
      break;
    
    case AST_FUNCTION:
      free(node->function.name);
      for (size_t i = 0; i < node->function.parameter_count; i++) {
        ast_free_node(node->function.parameters[i]);
      }
      free(node->function.parameters);
      ast_free_node(node->function.body);
      break;
    
    case AST_VARIABLE_DECL:
      free(node->variable_decl.name);
      ast_free_node(node->variable_decl.initializer);
      break;
    
    case AST_ASSIGNMENT:
      free(node->assignment.name);
      ast_free_node(node->assignment.value);
      break;
    
    case AST_BINARY_OP:
      ast_free_node(node->binary_op.left);
      ast_free_node(node->binary_op.right);
      break;
    
    case AST_UNARY_OP:
      ast_free_node(node->unary_op.operand);
      break;
    
    case AST_CALL:
      free(node->call.name);
      for (size_t i = 0; i < node->call.argument_count; i++) {
        ast_free_node(node->call.arguments[i]);
      }
      free(node->call.arguments);
      break;
    
    case AST_IDENTIFIER:
      free(node->identifier.name);
      break;
    
    case AST_STRING:
      free(node->string.value);
      break;
    
    case AST_BLOCK:
      for (size_t i = 0; i < node->block.statement_count; i++) {
        ast_free_node(node->block.statements[i]);
      }
      free(node->block.statements);
      break;
    
    case AST_IF:
      ast_free_node(node->if_stmt.condition);
      ast_free_node(node->if_stmt.then_branch);
      ast_free_node(node->if_stmt.else_branch);
      break;
    
    case AST_WHILE:
      ast_free_node(node->while_stmt.condition);
      ast_free_node(node->while_stmt.body);
      break;
    
    case AST_FOR:
      ast_free_node(node->for_stmt.init);
      ast_free_node(node->for_stmt.condition);
      ast_free_node(node->for_stmt.update);
      ast_free_node(node->for_stmt.body);
      break;
    
    case AST_RETURN:
      ast_free_node(node->return_stmt.value);
      break;
    
    case AST_EXPRESSION_STMT:
      ast_free_node(node->expression_stmt.expression);
      break;
    
    default:
      break;
  }
  
  free(node);
}

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }
}

static const char* data_type_to_string(DataType type) {
  switch (type) {
    case TYPE_VOID: return "void";
    case TYPE_INT: return "int";
    case TYPE_CHAR: return "char";
    default: return "unknown";
  }
}

static const char* binary_op_to_string(BinaryOperator op) {
  switch (op) {
    case BINOP_ADD: return "+";
    case BINOP_SUB: return "-";
    case BINOP_MUL: return "*";
    case BINOP_DIV: return "/";
    case BINOP_MOD: return "%";
    case BINOP_EQ: return "==";
    case BINOP_NE: return "!=";
    case BINOP_LT: return "<";
    case BINOP_LE: return "<=";
    case BINOP_GT: return ">";
    case BINOP_GE: return ">=";
    case BINOP_AND: return "&&";
    case BINOP_OR: return "||";
    default: return "?";
  }
}

static const char* unary_op_to_string(UnaryOperator op) {
  switch (op) {
    case UNOP_MINUS: return "-";
    case UNOP_NOT: return "!";
    case UNOP_PRE_INC: return "++";
    case UNOP_PRE_DEC: return "--";
    case UNOP_POST_INC: return "++";
    case UNOP_POST_DEC: return "--";
    default: return "?";
  }
}

void ast_print(const ASTNode* node, int indent) {
  if (!node) {
    print_indent(indent);
    printf("(null)\n");
    return;
  }
  
  print_indent(indent);
  
  switch (node->type) {
    case AST_PROGRAM:
      printf("Program\n");
      for (size_t i = 0; i < node->program.statement_count; i++) {
        ast_print(node->program.statements[i], indent + 1);
      }
      break;
    
    case AST_FUNCTION:
      printf("Function: %s -> %s\n", node->function.name, data_type_to_string(node->function.return_type));
      if (node->function.parameter_count > 0) {
        print_indent(indent + 1);
        printf("Parameters:\n");
        for (size_t i = 0; i < node->function.parameter_count; i++) {
          ast_print(node->function.parameters[i], indent + 2);
        }
      }
      print_indent(indent + 1);
      printf("Body:\n");
      ast_print(node->function.body, indent + 2);
      break;
    
    case AST_VARIABLE_DECL:
      printf("VarDecl: %s : %s\n", node->variable_decl.name, data_type_to_string(node->variable_decl.type));
      if (node->variable_decl.initializer) {
        print_indent(indent + 1);
        printf("Initializer:\n");
        ast_print(node->variable_decl.initializer, indent + 2);
      }
      break;
    
    case AST_ASSIGNMENT:
      printf("Assignment: %s =\n", node->assignment.name);
      ast_print(node->assignment.value, indent + 1);
      break;
    
    case AST_BINARY_OP:
      printf("BinaryOp: %s\n", binary_op_to_string(node->binary_op.operator));
      ast_print(node->binary_op.left, indent + 1);
      ast_print(node->binary_op.right, indent + 1);
      break;
    
    case AST_UNARY_OP:
      printf("UnaryOp: %s\n", unary_op_to_string(node->unary_op.operator));
      ast_print(node->unary_op.operand, indent + 1);
      break;
    
    case AST_CALL:
      printf("Call: %s\n", node->call.name);
      for (size_t i = 0; i < node->call.argument_count; i++) {
        ast_print(node->call.arguments[i], indent + 1);
      }
      break;
    
    case AST_IDENTIFIER:
      printf("Identifier: %s\n", node->identifier.name);
      break;
    
    case AST_NUMBER:
      printf("Number: %lld\n", (long long)node->number.value);
      break;
    
    case AST_STRING:
      printf("String: \"%s\"\n", node->string.value);
      break;
    
    case AST_CHAR:
      printf("Char: '%c'\n", node->character.value);
      break;
    
    case AST_BLOCK:
      printf("Block\n");
      for (size_t i = 0; i < node->block.statement_count; i++) {
        ast_print(node->block.statements[i], indent + 1);
      }
      break;
    
    case AST_IF:
      printf("If\n");
      print_indent(indent + 1);
      printf("Condition:\n");
      ast_print(node->if_stmt.condition, indent + 2);
      print_indent(indent + 1);
      printf("Then:\n");
      ast_print(node->if_stmt.then_branch, indent + 2);
      if (node->if_stmt.else_branch) {
        print_indent(indent + 1);
        printf("Else:\n");
        ast_print(node->if_stmt.else_branch, indent + 2);
      }
      break;
    
    case AST_WHILE:
      printf("While\n");
      print_indent(indent + 1);
      printf("Condition:\n");
      ast_print(node->while_stmt.condition, indent + 2);
      print_indent(indent + 1);
      printf("Body:\n");
      ast_print(node->while_stmt.body, indent + 2);
      break;
    
    case AST_FOR:
      printf("For\n");
      if (node->for_stmt.init) {
        print_indent(indent + 1);
        printf("Init:\n");
        ast_print(node->for_stmt.init, indent + 2);
      }
      if (node->for_stmt.condition) {
        print_indent(indent + 1);
        printf("Condition:\n");
        ast_print(node->for_stmt.condition, indent + 2);
      }
      if (node->for_stmt.update) {
        print_indent(indent + 1);
        printf("Update:\n");
        ast_print(node->for_stmt.update, indent + 2);
      }
      print_indent(indent + 1);
      printf("Body:\n");
      ast_print(node->for_stmt.body, indent + 2);
      break;
    
    case AST_RETURN:
      printf("Return\n");
      if (node->return_stmt.value) {
        ast_print(node->return_stmt.value, indent + 1);
      }
      break;
    
    case AST_EXPRESSION_STMT:
      printf("ExpressionStmt\n");
      ast_print(node->expression_stmt.expression, indent + 1);
      break;
    
    default:
      printf("Unknown node type: %d\n", node->type);
      break;
  }
}

ASTNode* ast_create_program(void) {
  return ast_create_node(AST_PROGRAM);
}

ASTNode* ast_create_function(const char* name, DataType return_type) {
  ASTNode* node = ast_create_node(AST_FUNCTION);
  node->function.name = safe_strdup(name);
  node->function.return_type = return_type;
  return node;
}

ASTNode* ast_create_variable_decl(const char* name, DataType type, ASTNode* initializer) {
  ASTNode* node = ast_create_node(AST_VARIABLE_DECL);
  node->variable_decl.name = safe_strdup(name);
  node->variable_decl.type = type;
  node->variable_decl.initializer = initializer;
  return node;
}

ASTNode* ast_create_assignment(const char* name, ASTNode* value) {
  ASTNode* node = ast_create_node(AST_ASSIGNMENT);
  node->assignment.name = safe_strdup(name);
  node->assignment.value = value;
  return node;
}

ASTNode* ast_create_binary_op(BinaryOperator op, ASTNode* left, ASTNode* right) {
  ASTNode* node = ast_create_node(AST_BINARY_OP);
  node->binary_op.operator = op;
  node->binary_op.left = left;
  node->binary_op.right = right;
  return node;
}

ASTNode* ast_create_unary_op(UnaryOperator op, ASTNode* operand) {
  ASTNode* node = ast_create_node(AST_UNARY_OP);
  node->unary_op.operator = op;
  node->unary_op.operand = operand;
  return node;
}

ASTNode* ast_create_identifier(const char* name) {
  ASTNode* node = ast_create_node(AST_IDENTIFIER);
  node->identifier.name = safe_strdup(name);
  return node;
}

ASTNode* ast_create_number(int64_t value) {
  ASTNode* node = ast_create_node(AST_NUMBER);
  node->number.value = value;
  return node;
}

ASTNode* ast_create_string(const char* value) {
  ASTNode* node = ast_create_node(AST_STRING);
  node->string.value = safe_strdup(value);
  return node;
}

ASTNode* ast_create_block(void) {
  return ast_create_node(AST_BLOCK);
}

ASTNode* ast_create_call(const char* name) {
  ASTNode* node = ast_create_node(AST_CALL);
  node->call.name = safe_strdup(name);
  node->call.arguments = NULL;
  node->call.argument_count = 0;
  return node;
}

void ast_add_statement(ASTNode* parent, ASTNode* statement) {
  if (!parent || !statement) return;
  
  if (parent->type == AST_PROGRAM) {
    parent->program.statements = safe_realloc(parent->program.statements, 
      (parent->program.statement_count + 1) * sizeof(ASTNode*));
    parent->program.statements[parent->program.statement_count++] = statement;
  } else if (parent->type == AST_BLOCK) {
    parent->block.statements = safe_realloc(parent->block.statements,
      (parent->block.statement_count + 1) * sizeof(ASTNode*));
    parent->block.statements[parent->block.statement_count++] = statement;
  }
}

void ast_add_argument(ASTNode* call, ASTNode* argument) {
  if (!call || !argument || call->type != AST_CALL) return;
  
  call->call.arguments = safe_realloc(call->call.arguments,
    (call->call.argument_count + 1) * sizeof(ASTNode*));
  call->call.arguments[call->call.argument_count++] = argument;
}