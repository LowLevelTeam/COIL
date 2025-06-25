/**
 * @file include/parser.h
 * @brief Recursive descent parser for C source code
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct Parser {
  Lexer* lexer;
  Token current_token;
  Token previous_token;
  bool had_error;
  bool panic_mode;
} Parser;

// Function declarations
void parser_init(Parser* parser, Lexer* lexer);
ASTNode* parse_program(Parser* parser);
void parser_error(Parser* parser, const char* message);
void parser_advance(Parser* parser);
bool parser_check(Parser* parser, TokenType type);
bool parser_match(Parser* parser, TokenType type);
void parser_consume(Parser* parser, TokenType type, const char* message);
void parser_synchronize(Parser* parser);

// Parsing functions
ASTNode* parse_declaration(Parser* parser);
ASTNode* parse_function(Parser* parser);
ASTNode* parse_variable_declaration(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_block(Parser* parser);
ASTNode* parse_if_statement(Parser* parser);
ASTNode* parse_while_statement(Parser* parser);
ASTNode* parse_for_statement(Parser* parser);
ASTNode* parse_return_statement(Parser* parser);
ASTNode* parse_expression_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_assignment(Parser* parser);
ASTNode* parse_logical_or(Parser* parser);
ASTNode* parse_logical_and(Parser* parser);
ASTNode* parse_equality(Parser* parser);
ASTNode* parse_comparison(Parser* parser);
ASTNode* parse_term(Parser* parser);
ASTNode* parse_factor(Parser* parser);
ASTNode* parse_unary(Parser* parser);
ASTNode* parse_call(Parser* parser);
ASTNode* parse_primary(Parser* parser);

DataType token_to_data_type(TokenType type);

#endif // PARSER_H