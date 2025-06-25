/**
 * @file src/lexer.c
 * @brief Lexical analyzer implementation
 */

#include "lexer.h"
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static bool is_at_end(Lexer* lexer) {
  return *lexer->current == '\0';
}

static char advance(Lexer* lexer) {
  if (is_at_end(lexer)) return '\0';
  
  char c = *lexer->current++;
  if (c == '\n') {
    lexer->line++;
    lexer->column = 1;
  } else {
    lexer->column++;
  }
  return c;
}

static char peek(Lexer* lexer) {
  return *lexer->current;
}

static char peek_next(Lexer* lexer) {
  if (is_at_end(lexer)) return '\0';
  return lexer->current[1];
}

static bool match(Lexer* lexer, char expected) {
  if (is_at_end(lexer)) return false;
  if (*lexer->current != expected) return false;
  advance(lexer);
  return true;
}

static Token make_token(Lexer* lexer, TokenType type) {
  Token token;
  token.type = type;
  token.start = lexer->start;
  token.length = (int)(lexer->current - lexer->start);
  token.line = lexer->line;
  token.column = lexer->column - token.length;
  return token;
}

static Token error_token(Lexer* lexer, const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = lexer->line;
  token.column = lexer->column;
  return token;
}

static void skip_whitespace(Lexer* lexer) {
  for (;;) {
    char c = peek(lexer);
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
      case '\n':
        advance(lexer);
        break;
      case '/':
        if (peek_next(lexer) == '/') {
          // Line comment
          while (peek(lexer) != '\n' && !is_at_end(lexer)) {
            advance(lexer);
          }
        } else if (peek_next(lexer) == '*') {
          // Block comment
          advance(lexer); // consume '/'
          advance(lexer); // consume '*'
          while (!is_at_end(lexer)) {
            if (peek(lexer) == '*' && peek_next(lexer) == '/') {
              advance(lexer); // consume '*'
              advance(lexer); // consume '/'
              break;
            }
            advance(lexer);
          }
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static TokenType check_keyword(const char* start, int length, const char* rest, TokenType type) {
  if (strlen(rest) == length - 1 && memcmp(start + 1, rest, length - 1) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(Lexer* lexer) {
  switch (lexer->start[0]) {
    case 'c':
      if (lexer->current - lexer->start > 4) {
        return check_keyword(lexer->start, lexer->current - lexer->start, "onst", TOKEN_CONST);
      }
      return check_keyword(lexer->start, lexer->current - lexer->start, "har", TOKEN_CHAR_KW);
    case 'e':
      return check_keyword(lexer->start, lexer->current - lexer->start, "lse", TOKEN_ELSE);
    case 'f':
      return check_keyword(lexer->start, lexer->current - lexer->start, "or", TOKEN_FOR);
    case 'i':
      if (lexer->current - lexer->start > 1) {
        switch (lexer->start[1]) {
          case 'f': return check_keyword(lexer->start, lexer->current - lexer->start, "f", TOKEN_IF);
          case 'n': return check_keyword(lexer->start, lexer->current - lexer->start, "nt", TOKEN_INT);
        }
      }
      break;
    case 'r':
      return check_keyword(lexer->start, lexer->current - lexer->start, "eturn", TOKEN_RETURN);
    case 'v':
      return check_keyword(lexer->start, lexer->current - lexer->start, "oid", TOKEN_VOID);
    case 'w':
      return check_keyword(lexer->start, lexer->current - lexer->start, "hile", TOKEN_WHILE);
  }
  return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer* lexer) {
  while (isalnum(peek(lexer)) || peek(lexer) == '_') {
    advance(lexer);
  }
  return make_token(lexer, identifier_type(lexer));
}

static Token number(Lexer* lexer) {
  while (isdigit(peek(lexer))) {
    advance(lexer);
  }
  return make_token(lexer, TOKEN_NUMBER);
}

static Token string(Lexer* lexer) {
  while (peek(lexer) != '"' && !is_at_end(lexer)) {
    if (peek(lexer) == '\n') lexer->line++;
    advance(lexer);
  }
  
  if (is_at_end(lexer)) {
    return error_token(lexer, "Unterminated string.");
  }
  
  // Consume the closing quote
  advance(lexer);
  return make_token(lexer, TOKEN_STRING);
}

static Token character(Lexer* lexer) {
  if (is_at_end(lexer)) {
    return error_token(lexer, "Unterminated character literal.");
  }
  
  advance(lexer); // consume the character
  
  if (peek(lexer) != '\'') {
    return error_token(lexer, "Unterminated character literal.");
  }
  
  advance(lexer); // consume the closing quote
  return make_token(lexer, TOKEN_CHAR);
}

void lexer_init(Lexer* lexer, const char* source) {
  lexer->source = source;
  lexer->start = source;
  lexer->current = source;
  lexer->line = 1;
  lexer->column = 1;
}

Token lexer_next_token(Lexer* lexer) {
  skip_whitespace(lexer);
  lexer->start = lexer->current;
  
  if (is_at_end(lexer)) {
    return make_token(lexer, TOKEN_EOF);
  }
  
  char c = advance(lexer);
  
  if (isalpha(c) || c == '_') {
    return identifier(lexer);
  }
  if (isdigit(c)) {
    return number(lexer);
  }
  
  switch (c) {
    case '(': return make_token(lexer, TOKEN_LEFT_PAREN);
    case ')': return make_token(lexer, TOKEN_RIGHT_PAREN);
    case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
    case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
    case '[': return make_token(lexer, TOKEN_LEFT_BRACKET);
    case ']': return make_token(lexer, TOKEN_RIGHT_BRACKET);
    case ';': return make_token(lexer, TOKEN_SEMICOLON);
    case ',': return make_token(lexer, TOKEN_COMMA);
    case '+':
      return make_token(lexer, match(lexer, '+') ? TOKEN_INCREMENT : TOKEN_PLUS);
    case '-':
      return make_token(lexer, match(lexer, '-') ? TOKEN_DECREMENT : TOKEN_MINUS);
    case '*': return make_token(lexer, TOKEN_MULTIPLY);
    case '/': return make_token(lexer, TOKEN_DIVIDE);
    case '%': return make_token(lexer, TOKEN_MODULO);
    case '!':
      return make_token(lexer, match(lexer, '=') ? TOKEN_NOT_EQUAL : TOKEN_LOGICAL_NOT);
    case '=':
      return make_token(lexer, match(lexer, '=') ? TOKEN_EQUAL : TOKEN_ASSIGN);
    case '<':
      return make_token(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return make_token(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '&':
      if (match(lexer, '&')) {
        return make_token(lexer, TOKEN_LOGICAL_AND);
      }
      break;
    case '|':
      if (match(lexer, '|')) {
        return make_token(lexer, TOKEN_LOGICAL_OR);
      }
      break;
    case '"': return string(lexer);
    case '\'': return character(lexer);
  }
  
  return error_token(lexer, "Unexpected character.");
}

const char* token_type_to_string(TokenType type) {
  switch (type) {
    case TOKEN_IDENTIFIER: return "IDENTIFIER";
    case TOKEN_NUMBER: return "NUMBER";
    case TOKEN_STRING: return "STRING";
    case TOKEN_CHAR: return "CHAR";
    case TOKEN_INT: return "INT";
    case TOKEN_CHAR_KW: return "CHAR_KW";
    case TOKEN_VOID: return "VOID";
    case TOKEN_IF: return "IF";
    case TOKEN_ELSE: return "ELSE";
    case TOKEN_WHILE: return "WHILE";
    case TOKEN_FOR: return "FOR";
    case TOKEN_RETURN: return "RETURN";
    case TOKEN_CONST: return "CONST";
    case TOKEN_PLUS: return "PLUS";
    case TOKEN_MINUS: return "MINUS";
    case TOKEN_MULTIPLY: return "MULTIPLY";
    case TOKEN_DIVIDE: return "DIVIDE";
    case TOKEN_MODULO: return "MODULO";
    case TOKEN_ASSIGN: return "ASSIGN";
    case TOKEN_EQUAL: return "EQUAL";
    case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
    case TOKEN_LESS: return "LESS";
    case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
    case TOKEN_GREATER: return "GREATER";
    case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";
    case TOKEN_LOGICAL_AND: return "LOGICAL_AND";
    case TOKEN_LOGICAL_OR: return "LOGICAL_OR";
    case TOKEN_LOGICAL_NOT: return "LOGICAL_NOT";
    case TOKEN_INCREMENT: return "INCREMENT";
    case TOKEN_DECREMENT: return "DECREMENT";
    case TOKEN_SEMICOLON: return "SEMICOLON";
    case TOKEN_COMMA: return "COMMA";
    case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
    case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
    case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
    case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
    case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
    case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
    case TOKEN_EOF: return "EOF";
    case TOKEN_ERROR: return "ERROR";
    default: return "UNKNOWN";
  }
}

bool token_equals_text(const Token* token, const char* text) {
  return (int)strlen(text) == token->length && 
         memcmp(token->start, text, token->length) == 0;
}

char* token_to_string(const Token* token) {
  char* str = safe_malloc(token->length + 1);
  memcpy(str, token->start, token->length);
  str[token->length] = '\0';
  return str;
}