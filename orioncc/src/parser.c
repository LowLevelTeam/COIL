/**
 * @file src/parser.c
 * @brief Recursive descent parser implementation
 */

#include "parser.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser_init(Parser* parser, Lexer* lexer) {
  parser->lexer = lexer;
  parser->had_error = false;
  parser->panic_mode = false;
  parser_advance(parser);
}

void parser_error(Parser* parser, const char* message) {
  if (parser->panic_mode) return;
  
  parser->panic_mode = true;
  parser->had_error = true;
  
  fprintf(stderr, "[line %d] Error", parser->previous_token.line);
  
  if (parser->previous_token.type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (parser->previous_token.type == TOKEN_ERROR) {
    // Nothing
  } else {
    char* text = token_to_string(&parser->previous_token);
    fprintf(stderr, " at '%s'", text);
    free(text);
  }
  
  fprintf(stderr, ": %s\n", message);
}

void parser_advance(Parser* parser) {
  parser->previous_token = parser->current_token;
  
  for (;;) {
    parser->current_token = lexer_next_token(parser->lexer);
    if (parser->current_token.type != TOKEN_ERROR) break;
    
    parser_error(parser, parser->current_token.start);
  }
}

bool parser_check(Parser* parser, TokenType type) {
  return parser->current_token.type == type;
}

bool parser_match(Parser* parser, TokenType type) {
  if (!parser_check(parser, type)) return false;
  parser_advance(parser);
  return true;
}

void parser_consume(Parser* parser, TokenType type, const char* message) {
  if (parser->current_token.type == type) {
    parser_advance(parser);
    return;
  }
  
  parser_error(parser, message);
}

void parser_synchronize(Parser* parser) {
  parser->panic_mode = false;
  
  while (parser->current_token.type != TOKEN_EOF) {
    if (parser->previous_token.type == TOKEN_SEMICOLON) return;
    
    switch (parser->current_token.type) {
      case TOKEN_IF:
      case TOKEN_FOR:
      case TOKEN_WHILE:
      case TOKEN_RETURN:
      case TOKEN_INT:
      case TOKEN_CHAR_KW:
      case TOKEN_VOID:
        return;
      default:
        break;
    }
    
    parser_advance(parser);
  }
}

DataType token_to_data_type(TokenType type) {
  switch (type) {
    case TOKEN_INT: return TYPE_INT;
    case TOKEN_CHAR_KW: return TYPE_CHAR;
    case TOKEN_VOID: return TYPE_VOID;
    default: return TYPE_VOID;
  }
}

ASTNode* parse_program(Parser* parser) {
  ASTNode* program = ast_create_program();
  
  while (!parser_check(parser, TOKEN_EOF)) {
    if (parser->panic_mode) parser_synchronize(parser);
    
    ASTNode* decl = parse_declaration(parser);
    if (decl) {
      ast_add_statement(program, decl);
    }
  }
  
  return program;
}

ASTNode* parse_declaration(Parser* parser) {
  // Check for type specifiers
  if (parser_match(parser, TOKEN_INT) || parser_match(parser, TOKEN_CHAR_KW) || parser_match(parser, TOKEN_VOID)) {
    DataType type = token_to_data_type(parser->previous_token.type);
    
    if (!parser_check(parser, TOKEN_IDENTIFIER)) {
      parser_error(parser, "Expected identifier after type specifier.");
      return NULL;
    }
    
    char* name = token_to_string(&parser->current_token);
    parser_advance(parser);
    
    if (parser_match(parser, TOKEN_LEFT_PAREN)) {
      // Function declaration
      ASTNode* func = ast_create_function(name, type);
      free(name);
      
      // Parse parameters
      if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
        do {
          // Parse parameter type
          if (parser_match(parser, TOKEN_INT) || parser_match(parser, TOKEN_CHAR_KW) || parser_match(parser, TOKEN_VOID)) {
            DataType param_type = token_to_data_type(parser->previous_token.type);
            
            // Parse parameter name
            if (parser_check(parser, TOKEN_IDENTIFIER)) {
              char* param_name = token_to_string(&parser->current_token);
              parser_advance(parser);
              
              // Create parameter node
              ASTNode* param = ast_create_variable_decl(param_name, param_type, NULL);
              free(param_name);
              
              // Add to function parameters
              func->function.parameters = safe_realloc(func->function.parameters, 
                (func->function.parameter_count + 1) * sizeof(ASTNode*));
              func->function.parameters[func->function.parameter_count++] = param;
            } else {
              parser_error(parser, "Expected parameter name.");
              break;
            }
          } else {
            parser_error(parser, "Expected parameter type.");
            break;
          }
        } while (parser_match(parser, TOKEN_COMMA));
      }
      
      parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after parameters.");
      
      // Parse function body
      if (parser_check(parser, TOKEN_LEFT_BRACE)) {
        parser_advance(parser); // consume '{'
        func->function.body = parse_block(parser);
      } else {
        parser_error(parser, "Expected '{' before function body.");
        return func;
      }
      
      return func;
    } else {
      // Variable declaration
      ASTNode* initializer = NULL;
      if (parser_match(parser, TOKEN_ASSIGN)) {
        initializer = parse_expression(parser);
      }
      
      parser_consume(parser, TOKEN_SEMICOLON, "Expected ';' after variable declaration.");
      
      ASTNode* var_decl = ast_create_variable_decl(name, type, initializer);
      free(name);
      return var_decl;
    }
  }
  
  return parse_statement(parser);
}

ASTNode* parse_function(Parser* parser) {
  // This function is now integrated into parse_declaration
  return parse_declaration(parser);
}

ASTNode* parse_variable_declaration(Parser* parser) {
  // This function is now integrated into parse_declaration  
  return parse_declaration(parser);
}

ASTNode* parse_statement(Parser* parser) {
  if (parser_match(parser, TOKEN_IF)) {
    return parse_if_statement(parser);
  }
  if (parser_match(parser, TOKEN_WHILE)) {
    return parse_while_statement(parser);
  }
  if (parser_match(parser, TOKEN_FOR)) {
    return parse_for_statement(parser);
  }
  if (parser_match(parser, TOKEN_RETURN)) {
    return parse_return_statement(parser);
  }
  if (parser_match(parser, TOKEN_LEFT_BRACE)) {
    return parse_block(parser);
  }
  
  return parse_expression_statement(parser);
}

ASTNode* parse_block(Parser* parser) {
  ASTNode* block = ast_create_block();
  
  while (!parser_check(parser, TOKEN_RIGHT_BRACE) && !parser_check(parser, TOKEN_EOF)) {
    if (parser->panic_mode) parser_synchronize(parser);
    
    ASTNode* stmt = parse_declaration(parser);
    if (stmt) {
      ast_add_statement(block, stmt);
    }
  }
  
  parser_consume(parser, TOKEN_RIGHT_BRACE, "Expected '}' after block.");
  return block;
}

ASTNode* parse_if_statement(Parser* parser) {
  parser_consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'if'.");
  ASTNode* condition = parse_expression(parser);
  parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after if condition.");
  
  ASTNode* then_branch = parse_statement(parser);
  ASTNode* else_branch = NULL;
  
  if (parser_match(parser, TOKEN_ELSE)) {
    else_branch = parse_statement(parser);
  }
  
  ASTNode* if_stmt = ast_create_node(AST_IF);
  if_stmt->if_stmt.condition = condition;
  if_stmt->if_stmt.then_branch = then_branch;
  if_stmt->if_stmt.else_branch = else_branch;
  
  return if_stmt;
}

ASTNode* parse_while_statement(Parser* parser) {
  parser_consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'while'.");
  ASTNode* condition = parse_expression(parser);
  parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after while condition.");
  
  ASTNode* body = parse_statement(parser);
  
  ASTNode* while_stmt = ast_create_node(AST_WHILE);
  while_stmt->while_stmt.condition = condition;
  while_stmt->while_stmt.body = body;
  
  return while_stmt;
}

ASTNode* parse_for_statement(Parser* parser) {
  parser_consume(parser, TOKEN_LEFT_PAREN, "Expected '(' after 'for'.");
  
  ASTNode* init = NULL;
  if (!parser_check(parser, TOKEN_SEMICOLON)) {
    if (parser_check(parser, TOKEN_INT) || parser_check(parser, TOKEN_CHAR_KW)) {
      init = parse_declaration(parser);
    } else {
      init = parse_expression_statement(parser);
    }
  } else {
    parser_advance(parser);
  }
  
  ASTNode* condition = NULL;
  if (!parser_check(parser, TOKEN_SEMICOLON)) {
    condition = parse_expression(parser);
  }
  parser_consume(parser, TOKEN_SEMICOLON, "Expected ';' after for loop condition.");
  
  ASTNode* update = NULL;
  if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
    update = parse_expression(parser);
  }
  parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after for clauses.");
  
  ASTNode* body = parse_statement(parser);
  
  ASTNode* for_stmt = ast_create_node(AST_FOR);
  for_stmt->for_stmt.init = init;
  for_stmt->for_stmt.condition = condition;
  for_stmt->for_stmt.update = update;
  for_stmt->for_stmt.body = body;
  
  return for_stmt;
}

ASTNode* parse_return_statement(Parser* parser) {
  ASTNode* value = NULL;
  if (!parser_check(parser, TOKEN_SEMICOLON)) {
    value = parse_expression(parser);
  }
  
  parser_consume(parser, TOKEN_SEMICOLON, "Expected ';' after return value.");
  
  ASTNode* return_stmt = ast_create_node(AST_RETURN);
  return_stmt->return_stmt.value = value;
  
  return return_stmt;
}

ASTNode* parse_expression_statement(Parser* parser) {
  ASTNode* expr = parse_expression(parser);
  parser_consume(parser, TOKEN_SEMICOLON, "Expected ';' after expression.");
  
  ASTNode* expr_stmt = ast_create_node(AST_EXPRESSION_STMT);
  expr_stmt->expression_stmt.expression = expr;
  
  return expr_stmt;
}

ASTNode* parse_expression(Parser* parser) {
  return parse_assignment(parser);
}

ASTNode* parse_assignment(Parser* parser) {
  ASTNode* expr = parse_logical_or(parser);
  
  if (parser_match(parser, TOKEN_ASSIGN)) {
    if (expr->type != AST_IDENTIFIER) {
      parser_error(parser, "Invalid assignment target.");
      return expr;
    }
    
    ASTNode* value = parse_assignment(parser);
    return ast_create_assignment(expr->identifier.name, value);
  }
  
  return expr;
}

ASTNode* parse_logical_or(Parser* parser) {
  ASTNode* expr = parse_logical_and(parser);
  
  while (parser_match(parser, TOKEN_LOGICAL_OR)) {
    ASTNode* right = parse_logical_and(parser);
    expr = ast_create_binary_op(BINOP_OR, expr, right);
  }
  
  return expr;
}

ASTNode* parse_logical_and(Parser* parser) {
  ASTNode* expr = parse_equality(parser);
  
  while (parser_match(parser, TOKEN_LOGICAL_AND)) {
    ASTNode* right = parse_equality(parser);
    expr = ast_create_binary_op(BINOP_AND, expr, right);
  }
  
  return expr;
}

ASTNode* parse_equality(Parser* parser) {
  ASTNode* expr = parse_comparison(parser);
  
  while (parser_match(parser, TOKEN_EQUAL) || parser_match(parser, TOKEN_NOT_EQUAL)) {
    BinaryOperator op = (parser->previous_token.type == TOKEN_EQUAL) ? BINOP_EQ : BINOP_NE;
    ASTNode* right = parse_comparison(parser);
    expr = ast_create_binary_op(op, expr, right);
  }
  
  return expr;
}

ASTNode* parse_comparison(Parser* parser) {
  ASTNode* expr = parse_term(parser);
  
  while (parser_match(parser, TOKEN_GREATER) || parser_match(parser, TOKEN_GREATER_EQUAL) ||
         parser_match(parser, TOKEN_LESS) || parser_match(parser, TOKEN_LESS_EQUAL)) {
    BinaryOperator op;
    switch (parser->previous_token.type) {
      case TOKEN_GREATER: op = BINOP_GT; break;
      case TOKEN_GREATER_EQUAL: op = BINOP_GE; break;
      case TOKEN_LESS: op = BINOP_LT; break;
      case TOKEN_LESS_EQUAL: op = BINOP_LE; break;
      default: op = BINOP_LT; break;
    }
    ASTNode* right = parse_term(parser);
    expr = ast_create_binary_op(op, expr, right);
  }
  
  return expr;
}

ASTNode* parse_term(Parser* parser) {
  ASTNode* expr = parse_factor(parser);
  
  while (parser_match(parser, TOKEN_MINUS) || parser_match(parser, TOKEN_PLUS)) {
    BinaryOperator op = (parser->previous_token.type == TOKEN_PLUS) ? BINOP_ADD : BINOP_SUB;
    ASTNode* right = parse_factor(parser);
    expr = ast_create_binary_op(op, expr, right);
  }
  
  return expr;
}

ASTNode* parse_factor(Parser* parser) {
  ASTNode* expr = parse_unary(parser);
  
  while (parser_match(parser, TOKEN_DIVIDE) || parser_match(parser, TOKEN_MULTIPLY) || parser_match(parser, TOKEN_MODULO)) {
    BinaryOperator op;
    switch (parser->previous_token.type) {
      case TOKEN_MULTIPLY: op = BINOP_MUL; break;
      case TOKEN_DIVIDE: op = BINOP_DIV; break;
      case TOKEN_MODULO: op = BINOP_MOD; break;
      default: op = BINOP_MUL; break;
    }
    ASTNode* right = parse_unary(parser);
    expr = ast_create_binary_op(op, expr, right);
  }
  
  return expr;
}

ASTNode* parse_unary(Parser* parser) {
  if (parser_match(parser, TOKEN_LOGICAL_NOT) || parser_match(parser, TOKEN_MINUS)) {
    UnaryOperator op = (parser->previous_token.type == TOKEN_LOGICAL_NOT) ? UNOP_NOT : UNOP_MINUS;
    ASTNode* operand = parse_unary(parser);
    return ast_create_unary_op(op, operand);
  }
  
  if (parser_match(parser, TOKEN_INCREMENT) || parser_match(parser, TOKEN_DECREMENT)) {
    UnaryOperator op = (parser->previous_token.type == TOKEN_INCREMENT) ? UNOP_PRE_INC : UNOP_PRE_DEC;
    ASTNode* operand = parse_call(parser);
    return ast_create_unary_op(op, operand);
  }
  
  return parse_call(parser);
}

ASTNode* parse_call(Parser* parser) {
  ASTNode* expr = parse_primary(parser);
  
  while (true) {
    if (parser_match(parser, TOKEN_LEFT_PAREN)) {
      // Function call
      if (expr->type != AST_IDENTIFIER) {
        parser_error(parser, "Only identifiers can be called.");
        return expr;
      }
      
      ASTNode* call = ast_create_call(expr->identifier.name);
      
      // Parse arguments
      if (!parser_check(parser, TOKEN_RIGHT_PAREN)) {
        do {
          ASTNode* arg = parse_expression(parser);
          if (arg) {
            ast_add_argument(call, arg);
          }
        } while (parser_match(parser, TOKEN_COMMA));
      }
      
      parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after arguments.");
      
      ast_free_node(expr);
      expr = call;
    } else if (parser_match(parser, TOKEN_INCREMENT) || parser_match(parser, TOKEN_DECREMENT)) {
      UnaryOperator op = (parser->previous_token.type == TOKEN_INCREMENT) ? UNOP_POST_INC : UNOP_POST_DEC;
      expr = ast_create_unary_op(op, expr);
    } else {
      break;
    }
  }
  
  return expr;
}

ASTNode* parse_primary(Parser* parser) {
  if (parser_match(parser, TOKEN_NUMBER)) {
    char* text = token_to_string(&parser->previous_token);
    int64_t value = strtoll(text, NULL, 10);
    free(text);
    return ast_create_number(value);
  }
  
  if (parser_match(parser, TOKEN_STRING)) {
    char* text = token_to_string(&parser->previous_token);
    // Remove quotes
    size_t len = strlen(text);
    if (len >= 2) {
      text[len - 1] = '\0';
      ASTNode* string_node = ast_create_string(text + 1);
      free(text);
      return string_node;
    }
    free(text);
    return ast_create_string("");
  }
  
  if (parser_match(parser, TOKEN_CHAR)) {
    char* text = token_to_string(&parser->previous_token);
    char value = (strlen(text) >= 3) ? text[1] : '\0'; // Skip opening quote
    free(text);
    ASTNode* char_node = ast_create_node(AST_CHAR);
    char_node->character.value = value;
    return char_node;
  }
  
  if (parser_match(parser, TOKEN_IDENTIFIER)) {
    char* name = token_to_string(&parser->previous_token);
    ASTNode* identifier = ast_create_identifier(name);
    free(name);
    return identifier;
  }
  
  if (parser_match(parser, TOKEN_LEFT_PAREN)) {
    ASTNode* expr = parse_expression(parser);
    parser_consume(parser, TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
    return expr;
  }
  
  parser_error(parser, "Expected expression.");
  return NULL;
}