/**
 * @file include/lexer.h
 * @brief Lexical analyzer for C source code
 */

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>

// Token types
typedef enum {
  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_CHAR,
  
  // Keywords
  TOKEN_INT,
  TOKEN_CHAR_KW,
  TOKEN_VOID,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_RETURN,
  TOKEN_CONST,
  
  // Operators
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_MODULO,
  TOKEN_ASSIGN,
  TOKEN_EQUAL,
  TOKEN_NOT_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LOGICAL_AND,
  TOKEN_LOGICAL_OR,
  TOKEN_LOGICAL_NOT,
  TOKEN_INCREMENT,
  TOKEN_DECREMENT,
  
  // Punctuation
  TOKEN_SEMICOLON,
  TOKEN_COMMA,
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
  
  // Special
  TOKEN_EOF,
  TOKEN_ERROR
} TokenType;

typedef struct Token {
  TokenType type;
  const char* start;
  int length;
  int line;
  int column;
} Token;

typedef struct Lexer {
  const char* source;
  const char* current;
  const char* start;
  int line;
  int column;
} Lexer;

// Function declarations
void lexer_init(Lexer* lexer, const char* source);
Token lexer_next_token(Lexer* lexer);
const char* token_type_to_string(TokenType type);
bool token_equals_text(const Token* token, const char* text);
char* token_to_string(const Token* token);

#endif // LEXER_H