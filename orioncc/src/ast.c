#include "ast.h"

// AST node type names for debugging
static const char *ast_node_names[] = {
  [AST_PROGRAM] = "PROGRAM",
  [AST_FUNCTION] = "FUNCTION",
  [AST_PARAMETER] = "PARAMETER",
  [AST_COMPOUND_STMT] = "COMPOUND_STMT",
  [AST_IF_STMT] = "IF_STMT",
  [AST_WHILE_STMT] = "WHILE_STMT",
  [AST_FOR_STMT] = "FOR_STMT",
  [AST_RETURN_STMT] = "RETURN_STMT",
  [AST_BREAK_STMT] = "BREAK_STMT",
  [AST_CONTINUE_STMT] = "CONTINUE_STMT",
  [AST_EXPRESSION_STMT] = "EXPRESSION_STMT",
  [AST_VAR_DECL] = "VAR_DECL",
  [AST_ASSIGN_EXPR] = "ASSIGN_EXPR",
  [AST_BINARY_EXPR] = "BINARY_EXPR",
  [AST_UNARY_EXPR] = "UNARY_EXPR",
  [AST_CALL_EXPR] = "CALL_EXPR",
  [AST_VAR_REF] = "VAR_REF",
  [AST_INTEGER_LITERAL] = "INTEGER_LITERAL",
  [AST_ARRAY_ACCESS] = "ARRAY_ACCESS",
  [AST_POST_INCREMENT] = "POST_INCREMENT",
  [AST_POST_DECREMENT] = "POST_DECREMENT"
};

ASTNode *ast_create_node(ASTNodeType type, SourcePosition pos) {
  ASTNode *node = safe_malloc(sizeof(ASTNode));
  memset(node, 0, sizeof(ASTNode));
  node->type = type;
  node->pos = pos;
  node->type_info = type_create(TYPE_UNKNOWN);
  return node;
}

void ast_free_node(ASTNode *node) {
  if (!node) return;
  
  switch (node->type) {
    case AST_PROGRAM:
      for (size_t i = 0; i < node->program.function_count; i++) {
        ast_free_node(node->program.functions[i]);
      }
      free(node->program.functions);
      break;
      
    case AST_FUNCTION:
      free(node->function.name);
      for (size_t i = 0; i < node->function.param_count; i++) {
        ast_free_node(node->function.parameters[i]);
      }
      free(node->function.parameters);
      ast_free_node(node->function.body);
      break;
      
    case AST_PARAMETER:
      free(node->parameter.name);
      break;
      
    case AST_COMPOUND_STMT:
      for (size_t i = 0; i < node->compound.stmt_count; i++) {
        ast_free_node(node->compound.statements[i]);
      }
      free(node->compound.statements);
      break;
      
    case AST_IF_STMT:
      ast_free_node(node->if_stmt.condition);
      ast_free_node(node->if_stmt.then_stmt);
      ast_free_node(node->if_stmt.else_stmt);
      break;
      
    case AST_WHILE_STMT:
      ast_free_node(node->while_stmt.condition);
      ast_free_node(node->while_stmt.body);
      break;
      
    case AST_FOR_STMT:
      ast_free_node(node->for_stmt.init);
      ast_free_node(node->for_stmt.condition);
      ast_free_node(node->for_stmt.update);
      ast_free_node(node->for_stmt.body);
      break;
      
    case AST_RETURN_STMT:
      ast_free_node(node->return_stmt.expression);
      break;
      
    case AST_EXPRESSION_STMT:
      ast_free_node(node->expr_stmt.expression);
      break;
      
    case AST_VAR_DECL:
      free(node->var_decl.name);
      ast_free_node(node->var_decl.initializer);
      break;
      
    case AST_ASSIGN_EXPR:
      ast_free_node(node->assign.lvalue);
      ast_free_node(node->assign.rvalue);
      break;
      
    case AST_BINARY_EXPR:
      ast_free_node(node->binary.left);
      ast_free_node(node->binary.right);
      break;
      
    case AST_UNARY_EXPR:
      ast_free_node(node->unary.operand);
      break;
      
    case AST_CALL_EXPR:
      free(node->call.name);
      for (size_t i = 0; i < node->call.arg_count; i++) {
        ast_free_node(node->call.arguments[i]);
      }
      free(node->call.arguments);
      break;
      
    case AST_VAR_REF:
      free(node->var_ref.name);
      break;
      
    case AST_ARRAY_ACCESS:
      ast_free_node(node->array_access.array);
      ast_free_node(node->array_access.index);
      break;
      
    case AST_INTEGER_LITERAL:
    case AST_BREAK_STMT:
    case AST_CONTINUE_STMT:
    case AST_POST_INCREMENT:
    case AST_POST_DECREMENT:
      // No dynamic memory to free
      break;
  }
  
  free(node);
}

// Program creation
ASTNode *ast_create_program(SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_PROGRAM, pos);
  node->program.functions = safe_malloc(sizeof(ASTNode*) * INITIAL_CAPACITY);
  node->program.function_capacity = INITIAL_CAPACITY;
  return node;
}

OrionError ast_program_add_function(ASTNode *program, ASTNode *function) {
  if (!program || program->type != AST_PROGRAM || !function) {
    return ORION_ERROR_INVALID_ARGUMENT;
  }
  
  if (program->program.function_count >= program->program.function_capacity) {
    program->program.function_capacity *= 2;
    program->program.functions = safe_realloc(
      program->program.functions,
      sizeof(ASTNode*) * program->program.function_capacity
    );
  }
  
  program->program.functions[program->program.function_count++] = function;
  return ORION_SUCCESS;
}

// Function creation
ASTNode *ast_create_function(const char *name, TypeInfo return_type, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_FUNCTION, pos);
  node->function.name = string_duplicate(name);
  node->function.return_type = return_type;
  node->function.parameters = safe_malloc(sizeof(ASTNode*) * INITIAL_CAPACITY);
  node->function.param_capacity = INITIAL_CAPACITY;
  return node;
}

OrionError ast_function_add_parameter(ASTNode *function, ASTNode *parameter) {
  if (!function || function->type != AST_FUNCTION || !parameter) {
    return ORION_ERROR_INVALID_ARGUMENT;
  }
  
  if (function->function.param_count >= function->function.param_capacity) {
    function->function.param_capacity *= 2;
    function->function.parameters = safe_realloc(
      function->function.parameters,
      sizeof(ASTNode*) * function->function.param_capacity
    );
  }
  
  function->function.parameters[function->function.param_count++] = parameter;
  return ORION_SUCCESS;
}

void ast_function_set_body(ASTNode *function, ASTNode *body) {
  if (function && function->type == AST_FUNCTION) {
    function->function.body = body;
  }
}

// Statement creation
ASTNode *ast_create_compound_stmt(SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_COMPOUND_STMT, pos);
  node->compound.statements = safe_malloc(sizeof(ASTNode*) * INITIAL_CAPACITY);
  node->compound.stmt_capacity = INITIAL_CAPACITY;
  return node;
}

OrionError ast_compound_add_statement(ASTNode *compound, ASTNode *stmt) {
  if (!compound || compound->type != AST_COMPOUND_STMT || !stmt) {
    return ORION_ERROR_INVALID_ARGUMENT;
  }
  
  if (compound->compound.stmt_count >= compound->compound.stmt_capacity) {
    compound->compound.stmt_capacity *= 2;
    compound->compound.statements = safe_realloc(
      compound->compound.statements,
      sizeof(ASTNode*) * compound->compound.stmt_capacity
    );
  }
  
  compound->compound.statements[compound->compound.stmt_count++] = stmt;
  return ORION_SUCCESS;
}

ASTNode *ast_create_if_stmt(ASTNode *condition, ASTNode *then_stmt, ASTNode *else_stmt, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_IF_STMT, pos);
  node->if_stmt.condition = condition;
  node->if_stmt.then_stmt = then_stmt;
  node->if_stmt.else_stmt = else_stmt;
  return node;
}

ASTNode *ast_create_while_stmt(ASTNode *condition, ASTNode *body, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_WHILE_STMT, pos);
  node->while_stmt.condition = condition;
  node->while_stmt.body = body;
  return node;
}

ASTNode *ast_create_for_stmt(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_FOR_STMT, pos);
  node->for_stmt.init = init;
  node->for_stmt.condition = condition;
  node->for_stmt.update = update;
  node->for_stmt.body = body;
  return node;
}

ASTNode *ast_create_return_stmt(ASTNode *expression, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_RETURN_STMT, pos);
  node->return_stmt.expression = expression;
  return node;
}

ASTNode *ast_create_var_decl(const char *name, TypeInfo type, ASTNode *initializer, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_VAR_DECL, pos);
  node->var_decl.name = string_duplicate(name);
  node->var_decl.var_type = type;
  node->var_decl.initializer = initializer;
  node->type_info = type;
  return node;
}

// Expression creation
ASTNode *ast_create_assign_expr(ASTNode *lvalue, ASTNode *rvalue, TokenType op, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_ASSIGN_EXPR, pos);
  node->assign.lvalue = lvalue;
  node->assign.rvalue = rvalue;
  node->assign.assign_op = op;
  return node;
}

ASTNode *ast_create_binary_expr(TokenType op, ASTNode *left, ASTNode *right, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_BINARY_EXPR, pos);
  node->binary.operator = op;
  node->binary.left = left;
  node->binary.right = right;
  return node;
}

ASTNode *ast_create_unary_expr(TokenType op, ASTNode *operand, bool is_prefix, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_UNARY_EXPR, pos);
  node->unary.operator = op;
  node->unary.operand = operand;
  node->unary.is_prefix = is_prefix;
  return node;
}

ASTNode *ast_create_call_expr(const char *name, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_CALL_EXPR, pos);
  node->call.name = string_duplicate(name);
  node->call.arguments = safe_malloc(sizeof(ASTNode*) * INITIAL_CAPACITY);
  node->call.arg_capacity = INITIAL_CAPACITY;
  return node;
}

OrionError ast_call_add_argument(ASTNode *call, ASTNode *arg) {
  if (!call || call->type != AST_CALL_EXPR || !arg) {
    return ORION_ERROR_INVALID_ARGUMENT;
  }
  
  if (call->call.arg_count >= call->call.arg_capacity) {
    call->call.arg_capacity *= 2;
    call->call.arguments = safe_realloc(
      call->call.arguments,
      sizeof(ASTNode*) * call->call.arg_capacity
    );
  }
  
  call->call.arguments[call->call.arg_count++] = arg;
  return ORION_SUCCESS;
}

ASTNode *ast_create_var_ref(const char *name, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_VAR_REF, pos);
  node->var_ref.name = string_duplicate(name);
  return node;
}

ASTNode *ast_create_integer_literal(int value, SourcePosition pos) {
  ASTNode *node = ast_create_node(AST_INTEGER_LITERAL, pos);
  node->integer.value = value;
  node->type_info = type_create(TYPE_INT);
  return node;
}

// Type utilities
TypeInfo type_create(DataType base_type) {
  TypeInfo type = {0};
  type.base_type = base_type;
  type.array_size = 0;
  type.is_const = false;
  return type;
}

TypeInfo type_create_array(DataType base_type, int size) {
  TypeInfo type = {0};
  type.base_type = base_type;
  type.array_size = size;
  type.is_const = false;
  return type;
}

bool type_is_compatible(TypeInfo a, TypeInfo b) {
  return a.base_type == b.base_type && a.array_size == b.array_size;
}

const char *type_to_string(TypeInfo type) {
  static char buffer[256];
  const char *base_name;
  
  switch (type.base_type) {
    case TYPE_VOID: base_name = "void"; break;
    case TYPE_INT: base_name = "int"; break;
    case TYPE_INT_ARRAY: base_name = "int[]"; break;
    default: base_name = "unknown"; break;
  }
  
  if (type.array_size > 0) {
    snprintf(buffer, sizeof(buffer), "%s[%d]", base_name, type.array_size);
  } else {
    strcpy(buffer, base_name);
  }
  
  return buffer;
}

// AST utilities
const char *ast_node_type_to_string(ASTNodeType type) {
  if (type >= 0 && type < sizeof(ast_node_names) / sizeof(ast_node_names[0])) {
    return ast_node_names[type];
  }
  return "UNKNOWN";
}

bool ast_is_statement(const ASTNode *node) {
  if (!node) return false;
  return node->type >= AST_COMPOUND_STMT && node->type <= AST_VAR_DECL;
}

bool ast_is_expression(const ASTNode *node) {
  if (!node) return false;
  return node->type >= AST_ASSIGN_EXPR && node->type <= AST_POST_DECREMENT;
}

bool ast_is_lvalue(const ASTNode *node) {
  if (!node) return false;
  return node->type == AST_VAR_REF || node->type == AST_ARRAY_ACCESS;
}

// AST debugging
void ast_print_node(const ASTNode *node, int indent) {
  if (!node) return;
  
  for (int i = 0; i < indent; i++) printf("  ");
  
  printf("%s", ast_node_type_to_string(node->type));
  
  switch (node->type) {
    case AST_FUNCTION:
      printf(" '%s' -> %s", node->function.name, type_to_string(node->function.return_type));
      break;
    case AST_PARAMETER:
      printf(" '%s': %s", node->parameter.name, type_to_string(node->parameter.param_type));
      break;
    case AST_VAR_DECL:
      printf(" '%s': %s", node->var_decl.name, type_to_string(node->var_decl.var_type));
      break;
    case AST_VAR_REF:
      printf(" '%s'", node->var_ref.name);
      break;
    case AST_CALL_EXPR:
      printf(" '%s'", node->call.name);
      break;
    case AST_INTEGER_LITERAL:
      printf(" %d", node->integer.value);
      break;
    case AST_BINARY_EXPR:
      printf(" %s", token_type_to_string(node->binary.operator));
      break;
    case AST_UNARY_EXPR:
      printf(" %s (%s)", token_type_to_string(node->unary.operator), 
             node->unary.is_prefix ? "prefix" : "postfix");
      break;
    default:
      break;
  }
  
  printf("\n");
}

void ast_print_tree(const ASTNode *root) {
  if (!root) return;
  
  ast_print_node(root, 0);
  
  switch (root->type) {
    case AST_PROGRAM:
      for (size_t i = 0; i < root->program.function_count; i++) {
        ast_print_tree(root->program.functions[i]);
      }
      break;
      
    case AST_FUNCTION:
      for (size_t i = 0; i < root->function.param_count; i++) {
        ast_print_tree(root->function.parameters[i]);
      }
      ast_print_tree(root->function.body);
      break;
      
    case AST_COMPOUND_STMT:
      for (size_t i = 0; i < root->compound.stmt_count; i++) {
        ast_print_tree(root->compound.statements[i]);
      }
      break;
      
    case AST_IF_STMT:
      printf("  Condition:\n");
      ast_print_tree(root->if_stmt.condition);
      printf("  Then:\n");
      ast_print_tree(root->if_stmt.then_stmt);
      if (root->if_stmt.else_stmt) {
        printf("  Else:\n");
        ast_print_tree(root->if_stmt.else_stmt);
      }
      break;
      
    case AST_WHILE_STMT:
      printf("  Condition:\n");
      ast_print_tree(root->while_stmt.condition);
      printf("  Body:\n");
      ast_print_tree(root->while_stmt.body);
      break;
      
    case AST_RETURN_STMT:
      if (root->return_stmt.expression) {
        ast_print_tree(root->return_stmt.expression);
      }
      break;
      
    case AST_VAR_DECL:
      if (root->var_decl.initializer) {
        ast_print_tree(root->var_decl.initializer);
      }
      break;
      
    case AST_BINARY_EXPR:
      ast_print_tree(root->binary.left);
      ast_print_tree(root->binary.right);
      break;
      
    case AST_UNARY_EXPR:
      ast_print_tree(root->unary.operand);
      break;
      
    case AST_CALL_EXPR:
      for (size_t i = 0; i < root->call.arg_count; i++) {
        ast_print_tree(root->call.arguments[i]);
      }
      break;
      
    default:
      break;
  }
}

void ast_dump_to_file(const ASTNode *root, FILE *file) {
  FILE *old_stdout = stdout;
  stdout = file;
  ast_print_tree(root);
  stdout = old_stdout;
}

// AST traversal
void ast_traverse_preorder(ASTNode *root, ast_visitor_func visitor, void *context) {
  if (!root || !visitor) return;
  
  visitor(root, context);
  
  switch (root->type) {
    case AST_PROGRAM:
      for (size_t i = 0; i < root->program.function_count; i++) {
        ast_traverse_preorder(root->program.functions[i], visitor, context);
      }
      break;
      
    case AST_FUNCTION:
      for (size_t i = 0; i < root->function.param_count; i++) {
        ast_traverse_preorder(root->function.parameters[i], visitor, context);
      }
      ast_traverse_preorder(root->function.body, visitor, context);
      break;
      
    case AST_COMPOUND_STMT:
      for (size_t i = 0; i < root->compound.stmt_count; i++) {
        ast_traverse_preorder(root->compound.statements[i], visitor, context);
      }
      break;
      
    case AST_IF_STMT:
      ast_traverse_preorder(root->if_stmt.condition, visitor, context);
      ast_traverse_preorder(root->if_stmt.then_stmt, visitor, context);
      if (root->if_stmt.else_stmt) {
        ast_traverse_preorder(root->if_stmt.else_stmt, visitor, context);
      }
      break;
      
    case AST_WHILE_STMT:
      ast_traverse_preorder(root->while_stmt.condition, visitor, context);
      ast_traverse_preorder(root->while_stmt.body, visitor, context);
      break;
      
    case AST_RETURN_STMT:
      if (root->return_stmt.expression) {
        ast_traverse_preorder(root->return_stmt.expression, visitor, context);
      }
      break;
      
    case AST_VAR_DECL:
      if (root->var_decl.initializer) {
        ast_traverse_preorder(root->var_decl.initializer, visitor, context);
      }
      break;
      
    case AST_BINARY_EXPR:
      ast_traverse_preorder(root->binary.left, visitor, context);
      ast_traverse_preorder(root->binary.right, visitor, context);
      break;
      
    case AST_UNARY_EXPR:
      ast_traverse_preorder(root->unary.operand, visitor, context);
      break;
      
    case AST_CALL_EXPR:
      for (size_t i = 0; i < root->call.arg_count; i++) {
        ast_traverse_preorder(root->call.arguments[i], visitor, context);
      }
      break;
      
    default:
      break;
  }
}

void ast_traverse_postorder(ASTNode *root, ast_visitor_func visitor, void *context) {
  if (!root || !visitor) return;
  
  // Visit children first
  switch (root->type) {
    case AST_PROGRAM:
      for (size_t i = 0; i < root->program.function_count; i++) {
        ast_traverse_postorder(root->program.functions[i], visitor, context);
      }
      break;
      
    case AST_FUNCTION:
      for (size_t i = 0; i < root->function.param_count; i++) {
        ast_traverse_postorder(root->function.parameters[i], visitor, context);
      }
      ast_traverse_postorder(root->function.body, visitor, context);
      break;
      
    // ... similar for other node types
    default:
      break;
  }
  
  // Then visit the node itself
  visitor(root, context);
}
