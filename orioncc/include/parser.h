#ifndef ORION_PARSER_H
#define ORION_PARSER_H

#include "common.h"
#include "lexer.h"
#include "ast.h"

// Parser state
typedef struct {
  Lexer *lexer;
  bool had_error;
  bool panic_mode;
  int error_count;
  CompilerOptions *options;
} Parser;

// Parser API
OrionError parser_init(Parser *parser, Lexer *lexer, CompilerOptions *options);
void parser_cleanup(Parser *parser);
ASTNode *parser_parse_program(Parser *parser);

// Error handling
void parser_error(Parser *parser, const char *message);
void parser_error_at_current(Parser *parser, const char *message);
void parser_error_at_previous(Parser *parser, const char *message);
void parser_synchronize(Parser *parser);

// Token consumption
bool parser_check(Parser *parser, TokenType type);
bool parser_match(Parser *parser, TokenType type);
OrionError parser_consume(Parser *parser, TokenType type, const char *message);
void parser_advance(Parser *parser);

// Parsing functions - Declarations
ASTNode *parser_parse_function_declaration(Parser *parser);
ASTNode *parser_parse_parameter_list(Parser *parser);
ASTNode *parser_parse_parameter(Parser *parser);

// Parsing functions - Statements
ASTNode *parser_parse_statement(Parser *parser);
ASTNode *parser_parse_compound_statement(Parser *parser);
ASTNode *parser_parse_if_statement(Parser *parser);
ASTNode *parser_parse_while_statement(Parser *parser);
ASTNode *parser_parse_for_statement(Parser *parser);
ASTNode *parser_parse_return_statement(Parser *parser);
ASTNode *parser_parse_break_statement(Parser *parser);
ASTNode *parser_parse_continue_statement(Parser *parser);
ASTNode *parser_parse_expression_statement(Parser *parser);
ASTNode *parser_parse_variable_declaration(Parser *parser);

// Parsing functions - Expressions (precedence climbing)
ASTNode *parser_parse_expression(Parser *parser);
ASTNode *parser_parse_assignment(Parser *parser);
ASTNode *parser_parse_logical_or(Parser *parser);
ASTNode *parser_parse_logical_and(Parser *parser);
ASTNode *parser_parse_equality(Parser *parser);
ASTNode *parser_parse_comparison(Parser *parser);
ASTNode *parser_parse_bitwise_or(Parser *parser);
ASTNode *parser_parse_bitwise_xor(Parser *parser);
ASTNode *parser_parse_bitwise_and(Parser *parser);
ASTNode *parser_parse_shift(Parser *parser);
ASTNode *parser_parse_term(Parser *parser);
ASTNode *parser_parse_factor(Parser *parser);
ASTNode *parser_parse_unary(Parser *parser);
ASTNode *parser_parse_postfix(Parser *parser);
ASTNode *parser_parse_primary(Parser *parser);

// Utility functions
ASTNode *parser_parse_call_arguments(Parser *parser, ASTNode *callee);
TypeInfo parser_parse_type_specifier(Parser *parser);
int parser_get_operator_precedence(TokenType type);
bool parser_is_assignment_operator(TokenType type);

// Parser debugging
void parser_dump_current_token(Parser *parser);

#endif // ORION_PARSER_H
