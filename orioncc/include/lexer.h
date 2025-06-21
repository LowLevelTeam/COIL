#ifndef ORION_LEXER_H
#define ORION_LEXER_H

#include "common.h"

// Token types for lexical analysis
typedef enum {
  // Special tokens
  TOK_EOF,
  TOK_ERROR,
  
  // Literals
  TOK_INT_LITERAL,    // integer literal
  TOK_IDENT,          // identifier
  
  // Keywords
  TOK_IF,             // if
  TOK_ELSE,           // else
  TOK_WHILE,          // while
  TOK_FOR,            // for
  TOK_RETURN,         // return
  TOK_INT_TYPE,       // int (type)
  TOK_VOID,           // void
  TOK_BREAK,          // break
  TOK_CONTINUE,       // continue
  
  // Delimiters
  TOK_LBRACE,         // {
  TOK_RBRACE,         // }
  TOK_LPAREN,         // (
  TOK_RPAREN,         // )
  TOK_LBRACKET,       // [
  TOK_RBRACKET,       // ]
  TOK_SEMICOLON,      // ;
  TOK_COMMA,          // ,
  
  // Assignment operators
  TOK_ASSIGN,         // =
  TOK_PLUS_ASSIGN,    // +=
  TOK_MINUS_ASSIGN,   // -=
  TOK_MUL_ASSIGN,     // *=
  TOK_DIV_ASSIGN,     // /=
  TOK_MOD_ASSIGN,     // %=
  
  // Comparison operators
  TOK_EQ,             // ==
  TOK_NE,             // !=
  TOK_LT,             // <
  TOK_LE,             // <=
  TOK_GT,             // >
  TOK_GE,             // >=
  
  // Arithmetic operators
  TOK_PLUS,           // +
  TOK_MINUS,          // -
  TOK_MULTIPLY,       // *
  TOK_DIVIDE,         // /
  TOK_MODULO,         // %
  TOK_INCREMENT,      // ++
  TOK_DECREMENT,      // --
  
  // Bitwise operators
  TOK_AND,            // &
  TOK_OR,             // |
  TOK_XOR,            // ^
  TOK_NOT,            // ~
  TOK_LSHIFT,         // <<
  TOK_RSHIFT,         // >>
  
  // Logical operators
  TOK_LOGICAL_AND,    // &&
  TOK_LOGICAL_OR,     // ||
  TOK_LOGICAL_NOT,    // !
  
  // Unary operators
  TOK_ADDRESS_OF,     // & (when used as unary)
  TOK_DEREFERENCE     // * (when used as unary)
} TokenType;

typedef struct {
  TokenType type;
  char *value;
  SourcePosition pos;
  union {
    int int_value;
    char *string_value;
  } data;
} Token;

typedef struct {
  const char *input;
  const char *filename;
  size_t position;
  size_t length;
  SourcePosition current_pos;
  Token current_token;
  bool has_current_token;
} Lexer;

// Lexer API
OrionError lexer_init(Lexer *lexer, const char *input, const char *filename);
void lexer_cleanup(Lexer *lexer);
OrionError lexer_next_token(Lexer *lexer);
Token lexer_peek_token(Lexer *lexer);
bool lexer_at_end(Lexer *lexer);

// Token utilities
const char *token_type_to_string(TokenType type);
void token_free(Token *token);
Token token_copy(const Token *token);
bool token_is_keyword(const Token *token);
bool token_is_operator(const Token *token);
bool token_is_literal(const Token *token);

// Lexer debugging
void lexer_dump_token(const Token *token);
void lexer_dump_all_tokens(Lexer *lexer);

#endif // ORION_LEXER_H
