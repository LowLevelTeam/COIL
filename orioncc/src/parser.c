#include "parser.h"

// Operator precedence table
typedef struct {
  TokenType token;
  int precedence;
} PrecedenceEntry;

static const PrecedenceEntry precedence_table[] = {
  {TOK_LOGICAL_OR, 1},
  {TOK_LOGICAL_AND, 2},
  {TOK_OR, 3},
  {TOK_XOR, 4},
  {TOK_AND, 5},
  {TOK_EQ, 6}, {TOK_NE, 6},
  {TOK_LT, 7}, {TOK_LE, 7}, {TOK_GT, 7}, {TOK_GE, 7},
  {TOK_LSHIFT, 8}, {TOK_RSHIFT, 8},
  {TOK_PLUS, 9}, {TOK_MINUS, 9},
  {TOK_MULTIPLY, 10}, {TOK_DIVIDE, 10}, {TOK_MODULO, 10},
  {TOK_EOF, 0}  // Sentinel
};

OrionError parser_init(Parser *parser, Lexer *lexer, CompilerOptions *options) {
  if (!parser || !lexer) return ORION_ERROR_INVALID_ARGUMENT;
  
  parser->lexer = lexer;
  parser->options = options;
  parser->had_error = false;
  parser->panic_mode = false;
  parser->error_count = 0;
  
  // Get the first token
  return lexer_next_token(lexer);
}

void parser_cleanup(Parser *parser) {
  if (parser) {
    // Nothing to clean up currently
  }
}

void parser_error(Parser *parser, const char *message) {
  if (!parser || parser->panic_mode) return;
  
  parser->panic_mode = true;
  parser->had_error = true;
  parser->error_count++;
  
  Token current = lexer_peek_token(parser->lexer);
  error_exit(message, current.pos);
}

void parser_error_at_current(Parser *parser, const char *message) {
  parser_error(parser, message);
}

void parser_error_at_previous(Parser *parser, const char *message) {
  parser_error(parser, message);
}

void parser_synchronize(Parser *parser) {
  parser->panic_mode = false;
  
  while (!lexer_at_end(parser->lexer)) {
    Token current = lexer_peek_token(parser->lexer);
    
    if (current.type == TOK_SEMICOLON) {
      lexer_next_token(parser->lexer);
      return;
    }
    
    switch (current.type) {
      case TOK_IF:
      case TOK_WHILE:
      case TOK_FOR:
      case TOK_RETURN:
      case TOK_INT_TYPE:
      case TOK_VOID:
        return;
      default:
        break;
    }
    
    lexer_next_token(parser->lexer);
  }
}

bool parser_check(Parser *parser, TokenType type) {
  if (lexer_at_end(parser->lexer)) return false;
  Token current = lexer_peek_token(parser->lexer);
  return current.type == type;
}

bool parser_match(Parser *parser, TokenType type) {
  if (!parser_check(parser, type)) return false;
  lexer_next_token(parser->lexer);
  return true;
}

OrionError parser_consume(Parser *parser, TokenType type, const char *message) {
  if (parser_check(parser, type)) {
    return lexer_next_token(parser->lexer);
  }
  
  parser_error_at_current(parser, message);
  return ORION_ERROR_SYNTAX;
}

void parser_advance(Parser *parser) {
  if (!lexer_at_end(parser->lexer)) {
    lexer_next_token(parser->lexer);
  }
}

ASTNode *parser_parse_program(Parser *parser) {
  SourcePosition pos = {1, 1, parser->lexer->filename};
  ASTNode *program = ast_create_program(pos);
  
  while (!lexer_at_end(parser->lexer) && !parser->had_error) {
    ASTNode *func = parser_parse_function_declaration(parser);
    if (func) {
      ast_program_add_function(program, func);
    }
    
    if (parser->panic_mode) {
      parser_synchronize(parser);
    }
  }
  
  return program;
}

ASTNode *parser_parse_function_declaration(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  // Parse return type
  TypeInfo return_type;
  if (parser_match(parser, TOK_INT_TYPE)) {
    return_type = type_create(TYPE_INT);
  } else if (parser_match(parser, TOK_VOID)) {
    return_type = type_create(TYPE_VOID);
  } else {
    parser_error(parser, "Expected return type");
    return NULL;
  }
  
  // Parse function name - FIX: Save name before advancing
  if (!parser_check(parser, TOK_IDENT)) {
    parser_error(parser, "Expected function name");
    return NULL;
  }
  
  Token name_token = lexer_peek_token(parser->lexer);
  char *func_name = string_duplicate(name_token.value);
  lexer_next_token(parser->lexer);
  
  ASTNode *function = ast_create_function(func_name, return_type, pos);
  free(func_name);
  
  // Parse parameter list
  parser_consume(parser, TOK_LPAREN, "Expected '(' after function name");
  
  if (!parser_check(parser, TOK_RPAREN)) {
    do {
      ASTNode *param = parser_parse_parameter(parser);
      if (param) {
        ast_function_add_parameter(function, param);
      }
    } while (parser_match(parser, TOK_COMMA));
  }
  
  parser_consume(parser, TOK_RPAREN, "Expected ')' after parameters");
  
  // Parse function body
  ASTNode *body = parser_parse_compound_statement(parser);
  ast_function_set_body(function, body);
  
  return function;
}

ASTNode *parser_parse_parameter(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  // Parse parameter type
  TypeInfo param_type;
  if (parser_match(parser, TOK_INT_TYPE)) {
    param_type = type_create(TYPE_INT);
  } else {
    parser_error(parser, "Expected parameter type");
    return NULL;
  }
  
  // Parse parameter name - FIX: Save name before advancing
  if (!parser_check(parser, TOK_IDENT)) {
    parser_error(parser, "Expected parameter name");
    return NULL;
  }
  
  Token name_token = lexer_peek_token(parser->lexer);
  char *param_name = string_duplicate(name_token.value);
  lexer_next_token(parser->lexer);
  
  ASTNode *param = ast_create_node(AST_PARAMETER, pos);
  param->parameter.name = param_name;  // Transfer ownership
  param->parameter.param_type = param_type;
  
  return param;
}

ASTNode *parser_parse_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  
  switch (current.type) {
    case TOK_LBRACE:
      return parser_parse_compound_statement(parser);
    case TOK_IF:
      return parser_parse_if_statement(parser);
    case TOK_WHILE:
      return parser_parse_while_statement(parser);
    case TOK_FOR:
      return parser_parse_for_statement(parser);
    case TOK_RETURN:
      return parser_parse_return_statement(parser);
    case TOK_BREAK:
      return parser_parse_break_statement(parser);
    case TOK_CONTINUE:
      return parser_parse_continue_statement(parser);
    case TOK_INT_TYPE:
      return parser_parse_variable_declaration(parser);
    default:
      return parser_parse_expression_statement(parser);
  }
}

ASTNode *parser_parse_compound_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  ASTNode *compound = ast_create_compound_stmt(pos);
  
  parser_consume(parser, TOK_LBRACE, "Expected '{'");
  
  while (!parser_check(parser, TOK_RBRACE) && !lexer_at_end(parser->lexer)) {
    ASTNode *stmt = parser_parse_statement(parser);
    if (stmt) {
      ast_compound_add_statement(compound, stmt);
    }
    
    if (parser->panic_mode) {
      parser_synchronize(parser);
    }
  }
  
  parser_consume(parser, TOK_RBRACE, "Expected '}'");
  return compound;
}

ASTNode *parser_parse_if_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_IF, "Expected 'if'");
  parser_consume(parser, TOK_LPAREN, "Expected '(' after 'if'");
  
  ASTNode *condition = parser_parse_expression(parser);
  parser_consume(parser, TOK_RPAREN, "Expected ')' after if condition");
  
  ASTNode *then_stmt = parser_parse_statement(parser);
  ASTNode *else_stmt = NULL;
  
  if (parser_match(parser, TOK_ELSE)) {
    else_stmt = parser_parse_statement(parser);
  }
  
  return ast_create_if_stmt(condition, then_stmt, else_stmt, pos);
}

ASTNode *parser_parse_while_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_WHILE, "Expected 'while'");
  parser_consume(parser, TOK_LPAREN, "Expected '(' after 'while'");
  
  ASTNode *condition = parser_parse_expression(parser);
  parser_consume(parser, TOK_RPAREN, "Expected ')' after while condition");
  
  ASTNode *body = parser_parse_statement(parser);
  
  return ast_create_while_stmt(condition, body, pos);
}

ASTNode *parser_parse_for_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_FOR, "Expected 'for'");
  parser_consume(parser, TOK_LPAREN, "Expected '(' after 'for'");
  
  // Parse init (can be declaration or expression)
  ASTNode *init = NULL;
  if (parser_check(parser, TOK_INT_TYPE)) {
    init = parser_parse_variable_declaration(parser);
  } else if (!parser_check(parser, TOK_SEMICOLON)) {
    init = parser_parse_expression_statement(parser);
  } else {
    parser_consume(parser, TOK_SEMICOLON, "Expected ';'");
  }
  
  // Parse condition
  ASTNode *condition = NULL;
  if (!parser_check(parser, TOK_SEMICOLON)) {
    condition = parser_parse_expression(parser);
  }
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after for condition");
  
  // Parse update
  ASTNode *update = NULL;
  if (!parser_check(parser, TOK_RPAREN)) {
    update = parser_parse_expression(parser);
  }
  parser_consume(parser, TOK_RPAREN, "Expected ')' after for clauses");
  
  ASTNode *body = parser_parse_statement(parser);
  
  return ast_create_for_stmt(init, condition, update, body, pos);
}

ASTNode *parser_parse_return_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_RETURN, "Expected 'return'");
  
  ASTNode *expression = NULL;
  if (!parser_check(parser, TOK_SEMICOLON)) {
    expression = parser_parse_expression(parser);
  }
  
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after return statement");
  return ast_create_return_stmt(expression, pos);
}

ASTNode *parser_parse_break_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_BREAK, "Expected 'break'");
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after 'break'");
  
  return ast_create_node(AST_BREAK_STMT, pos);
}

ASTNode *parser_parse_continue_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  parser_consume(parser, TOK_CONTINUE, "Expected 'continue'");
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after 'continue'");
  
  return ast_create_node(AST_CONTINUE_STMT, pos);
}

ASTNode *parser_parse_expression_statement(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  ASTNode *expr = parser_parse_expression(parser);
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after expression");
  
  ASTNode *stmt = ast_create_node(AST_EXPRESSION_STMT, pos);
  stmt->expr_stmt.expression = expr;
  return stmt;
}

ASTNode *parser_parse_variable_declaration(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  SourcePosition pos = current.pos;
  
  // Parse type
  TypeInfo var_type;
  if (parser_match(parser, TOK_INT_TYPE)) {
    var_type = type_create(TYPE_INT);
  } else {
    parser_error(parser, "Expected variable type");
    return NULL;
  }
  
  // Parse variable name - FIX: Save name before advancing
  if (!parser_check(parser, TOK_IDENT)) {
    parser_error(parser, "Expected variable name");
    return NULL;
  }
  
  Token name_token = lexer_peek_token(parser->lexer);
  char *var_name = string_duplicate(name_token.value);
  lexer_next_token(parser->lexer);
  
  // Parse optional initializer
  ASTNode *initializer = NULL;
  if (parser_match(parser, TOK_ASSIGN)) {
    initializer = parser_parse_expression(parser);
  }
  
  parser_consume(parser, TOK_SEMICOLON, "Expected ';' after variable declaration");
  
  ASTNode *var_decl = ast_create_var_decl(var_name, var_type, initializer, pos);
  free(var_name);
  return var_decl;
}

// Expression parsing with precedence climbing
ASTNode *parser_parse_expression(Parser *parser) {
  return parser_parse_assignment(parser);
}

ASTNode *parser_parse_assignment(Parser *parser) {
  ASTNode *expr = parser_parse_logical_or(parser);
  
  if (parser_is_assignment_operator(lexer_peek_token(parser->lexer).type)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    
    ASTNode *rvalue = parser_parse_assignment(parser);
    return ast_create_assign_expr(expr, rvalue, op, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_logical_or(Parser *parser) {
  ASTNode *expr = parser_parse_logical_and(parser);
  
  while (parser_match(parser, TOK_LOGICAL_OR)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = TOK_LOGICAL_OR;
    ASTNode *right = parser_parse_logical_and(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_logical_and(Parser *parser) {
  ASTNode *expr = parser_parse_equality(parser);
  
  while (parser_match(parser, TOK_LOGICAL_AND)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = TOK_LOGICAL_AND;
    ASTNode *right = parser_parse_equality(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_equality(Parser *parser) {
  ASTNode *expr = parser_parse_comparison(parser);
  
  while (parser_check(parser, TOK_EQ) || parser_check(parser, TOK_NE)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *right = parser_parse_comparison(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_comparison(Parser *parser) {
  ASTNode *expr = parser_parse_bitwise_or(parser);
  
  while (parser_check(parser, TOK_LT) || parser_check(parser, TOK_LE) ||
         parser_check(parser, TOK_GT) || parser_check(parser, TOK_GE)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *right = parser_parse_bitwise_or(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_bitwise_or(Parser *parser) {
  ASTNode *expr = parser_parse_bitwise_xor(parser);
  
  while (parser_match(parser, TOK_OR)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = TOK_OR;
    ASTNode *right = parser_parse_bitwise_xor(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_bitwise_xor(Parser *parser) {
  ASTNode *expr = parser_parse_bitwise_and(parser);
  
  while (parser_match(parser, TOK_XOR)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = TOK_XOR;
    ASTNode *right = parser_parse_bitwise_and(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_bitwise_and(Parser *parser) {
  ASTNode *expr = parser_parse_shift(parser);
  
  while (parser_match(parser, TOK_AND)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = TOK_AND;
    ASTNode *right = parser_parse_shift(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_shift(Parser *parser) {
  ASTNode *expr = parser_parse_term(parser);
  
  while (parser_check(parser, TOK_LSHIFT) || parser_check(parser, TOK_RSHIFT)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *right = parser_parse_term(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_term(Parser *parser) {
  ASTNode *expr = parser_parse_factor(parser);
  
  while (parser_check(parser, TOK_PLUS) || parser_check(parser, TOK_MINUS)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *right = parser_parse_factor(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_factor(Parser *parser) {
  ASTNode *expr = parser_parse_unary(parser);
  
  while (parser_check(parser, TOK_MULTIPLY) || parser_check(parser, TOK_DIVIDE) || 
         parser_check(parser, TOK_MODULO)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *right = parser_parse_unary(parser);
    expr = ast_create_binary_expr(op, expr, right, op_token.pos);
  }
  
  return expr;
}

ASTNode *parser_parse_unary(Parser *parser) {
  if (parser_check(parser, TOK_LOGICAL_NOT) || parser_check(parser, TOK_MINUS) ||
      parser_check(parser, TOK_NOT) || parser_check(parser, TOK_INCREMENT) ||
      parser_check(parser, TOK_DECREMENT)) {
    Token op_token = lexer_peek_token(parser->lexer);
    TokenType op = op_token.type;
    lexer_next_token(parser->lexer);
    ASTNode *operand = parser_parse_unary(parser);
    return ast_create_unary_expr(op, operand, true, op_token.pos);
  }
  
  return parser_parse_postfix(parser);
}

ASTNode *parser_parse_postfix(Parser *parser) {
  ASTNode *expr = parser_parse_primary(parser);
  
  while (true) {
    if (parser_check(parser, TOK_LPAREN)) {
      // Function call
      expr = parser_parse_call_arguments(parser, expr);
    } else if (parser_check(parser, TOK_LBRACKET)) {
      // Array access
      Token bracket_token = lexer_peek_token(parser->lexer);
      lexer_next_token(parser->lexer);
      ASTNode *index = parser_parse_expression(parser);
      parser_consume(parser, TOK_RBRACKET, "Expected ']'");
      
      ASTNode *array_access = ast_create_node(AST_ARRAY_ACCESS, bracket_token.pos);
      array_access->array_access.array = expr;
      array_access->array_access.index = index;
      expr = array_access;
    } else if (parser_check(parser, TOK_INCREMENT)) {
      Token op_token = lexer_peek_token(parser->lexer);
      lexer_next_token(parser->lexer);
      expr = ast_create_unary_expr(TOK_INCREMENT, expr, false, op_token.pos);
    } else if (parser_check(parser, TOK_DECREMENT)) {
      Token op_token = lexer_peek_token(parser->lexer);
      lexer_next_token(parser->lexer);
      expr = ast_create_unary_expr(TOK_DECREMENT, expr, false, op_token.pos);
    } else {
      break;
    }
  }
  
  return expr;
}

ASTNode *parser_parse_primary(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  
  // FIX: Save token data before advancing
  if (current.type == TOK_INT_LITERAL) {
    int value = current.data.int_value;
    SourcePosition pos = current.pos;
    parser_match(parser, TOK_INT_LITERAL);
    return ast_create_integer_literal(value, pos);
  }
  
  if (current.type == TOK_IDENT) {
    char *name = string_duplicate(current.value);
    SourcePosition pos = current.pos;
    parser_match(parser, TOK_IDENT);
    ASTNode *node = ast_create_var_ref(name, pos);
    free(name);
    return node;
  }
  
  if (parser_match(parser, TOK_LPAREN)) {
    ASTNode *expr = parser_parse_expression(parser);
    parser_consume(parser, TOK_RPAREN, "Expected ')' after expression");
    return expr;
  }
  
  parser_error(parser, "Expected expression");
  return NULL;
}

ASTNode *parser_parse_call_arguments(Parser *parser, ASTNode *callee) {
  if (callee->type != AST_VAR_REF) {
    parser_error(parser, "Invalid function call");
    return NULL;
  }
  
  Token paren_token = lexer_peek_token(parser->lexer);
  ASTNode *call = ast_create_call_expr(callee->var_ref.name, paren_token.pos);
  
  parser_consume(parser, TOK_LPAREN, "Expected '('");
  
  if (!parser_check(parser, TOK_RPAREN)) {
    do {
      ASTNode *arg = parser_parse_expression(parser);
      if (arg) {
        ast_call_add_argument(call, arg);
      }
    } while (parser_match(parser, TOK_COMMA));
  }
  
  parser_consume(parser, TOK_RPAREN, "Expected ')' after arguments");
  
  // Free the original variable reference node
  ast_free_node(callee);
  
  return call;
}

int parser_get_operator_precedence(TokenType type) {
  for (int i = 0; precedence_table[i].token != TOK_EOF; i++) {
    if (precedence_table[i].token == type) {
      return precedence_table[i].precedence;
    }
  }
  return 0;
}

bool parser_is_assignment_operator(TokenType type) {
  return type == TOK_ASSIGN || type == TOK_PLUS_ASSIGN || 
         type == TOK_MINUS_ASSIGN || type == TOK_MUL_ASSIGN ||
         type == TOK_DIV_ASSIGN || type == TOK_MOD_ASSIGN;
}

void parser_dump_current_token(Parser *parser) {
  Token current = lexer_peek_token(parser->lexer);
  lexer_dump_token(&current);
}
