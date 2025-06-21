#include "lexer.h"

// Keyword table for fast lookup
typedef struct {
  const char *word;
  TokenType type;
} Keyword;

static const Keyword keywords[] = {
  {"if", TOK_IF},
  {"else", TOK_ELSE},
  {"while", TOK_WHILE},
  {"for", TOK_FOR},
  {"return", TOK_RETURN},
  {"int", TOK_INT_TYPE},
  {"void", TOK_VOID},
  {"break", TOK_BREAK},
  {"continue", TOK_CONTINUE},
  {NULL, TOK_EOF}  // Sentinel
};

// Token type names for debugging
static const char *token_names[] = {
  [TOK_EOF] = "EOF",
  [TOK_ERROR] = "ERROR",
  [TOK_INT_LITERAL] = "INT_LITERAL",
  [TOK_IDENT] = "IDENTIFIER",
  [TOK_IF] = "IF",
  [TOK_ELSE] = "ELSE",
  [TOK_WHILE] = "WHILE",
  [TOK_FOR] = "FOR",
  [TOK_RETURN] = "RETURN",
  [TOK_INT_TYPE] = "INT",
  [TOK_VOID] = "VOID",
  [TOK_BREAK] = "BREAK",
  [TOK_CONTINUE] = "CONTINUE",
  [TOK_LBRACE] = "{",
  [TOK_RBRACE] = "}",
  [TOK_LPAREN] = "(",
  [TOK_RPAREN] = ")",
  [TOK_LBRACKET] = "[",
  [TOK_RBRACKET] = "]",
  [TOK_SEMICOLON] = ";",
  [TOK_COMMA] = ",",
  [TOK_ASSIGN] = "=",
  [TOK_PLUS_ASSIGN] = "+=",
  [TOK_MINUS_ASSIGN] = "-=",
  [TOK_MUL_ASSIGN] = "*=",
  [TOK_DIV_ASSIGN] = "/=",
  [TOK_MOD_ASSIGN] = "%=",
  [TOK_EQ] = "==",
  [TOK_NE] = "!=",
  [TOK_LT] = "<",
  [TOK_LE] = "<=",
  [TOK_GT] = ">",
  [TOK_GE] = ">=",
  [TOK_PLUS] = "+",
  [TOK_MINUS] = "-",
  [TOK_MULTIPLY] = "*",
  [TOK_DIVIDE] = "/",
  [TOK_MODULO] = "%",
  [TOK_INCREMENT] = "++",
  [TOK_DECREMENT] = "--",
  [TOK_AND] = "&",
  [TOK_OR] = "|",
  [TOK_XOR] = "^",
  [TOK_NOT] = "~",
  [TOK_LSHIFT] = "<<",
  [TOK_RSHIFT] = ">>",
  [TOK_LOGICAL_AND] = "&&",
  [TOK_LOGICAL_OR] = "||",
  [TOK_LOGICAL_NOT] = "!"
};

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static bool is_alnum(char c) {
  return is_alpha(c) || is_digit(c);
}

static bool is_at_end(Lexer *lexer) {
  return lexer->position >= lexer->length;
}

static char peek(Lexer *lexer) {
  if (is_at_end(lexer)) return '\0';
  return lexer->input[lexer->position];
}

static char peek_next(Lexer *lexer) {
  if (lexer->position + 1 >= lexer->length) return '\0';
  return lexer->input[lexer->position + 1];
}

static char advance(Lexer *lexer) {
  if (is_at_end(lexer)) return '\0';
  
  char c = lexer->input[lexer->position++];
  if (c == '\n') {
    lexer->current_pos.line++;
    lexer->current_pos.column = 1;
  } else {
    lexer->current_pos.column++;
  }
  return c;
}

static void skip_whitespace(Lexer *lexer) {
  while (true) {
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

static Token make_token(Lexer *lexer, TokenType type) {
  Token token = {0};
  token.type = type;
  token.pos = lexer->current_pos;
  return token;
}

static Token make_error_token(Lexer *lexer, const char *message) {
  Token token = make_token(lexer, TOK_ERROR);
  token.value = string_duplicate(message);
  return token;
}

static TokenType check_keyword(const char *text, size_t length) {
  for (int i = 0; keywords[i].word != NULL; i++) {
    if (strlen(keywords[i].word) == length && 
        strncmp(keywords[i].word, text, length) == 0) {
      return keywords[i].type;
    }
  }
  return TOK_IDENT;
}

static Token make_identifier(Lexer *lexer, size_t start) {
  size_t length = lexer->position - start;
  char *text = safe_malloc(length + 1);
  strncpy(text, &lexer->input[start], length);
  text[length] = '\0';
  
  TokenType type = check_keyword(text, length);
  
  Token token = make_token(lexer, type);
  token.value = text;
  return token;
}

static Token make_number(Lexer *lexer, size_t start) {
  size_t length = lexer->position - start;
  char *text = safe_malloc(length + 1);
  strncpy(text, &lexer->input[start], length);
  text[length] = '\0';
  
  Token token = make_token(lexer, TOK_INT_LITERAL);
  token.value = text;
  token.data.int_value = atoi(text);
  return token;
}

static Token scan_token(Lexer *lexer) {
  skip_whitespace(lexer);
  
  if (is_at_end(lexer)) {
    return make_token(lexer, TOK_EOF);
  }
  
  size_t start = lexer->position;
  SourcePosition token_pos = lexer->current_pos;
  char c = advance(lexer);
  
  // Update token position
  lexer->current_pos = token_pos;
  
  if (is_alpha(c)) {
    while (is_alnum(peek(lexer))) advance(lexer);
    return make_identifier(lexer, start);
  }
  
  if (is_digit(c)) {
    while (is_digit(peek(lexer))) advance(lexer);
    return make_number(lexer, start);
  }
  
  switch (c) {
    case '(': return make_token(lexer, TOK_LPAREN);
    case ')': return make_token(lexer, TOK_RPAREN);
    case '{': return make_token(lexer, TOK_LBRACE);
    case '}': return make_token(lexer, TOK_RBRACE);
    case '[': return make_token(lexer, TOK_LBRACKET);
    case ']': return make_token(lexer, TOK_RBRACKET);
    case ';': return make_token(lexer, TOK_SEMICOLON);
    case ',': return make_token(lexer, TOK_COMMA);
    case '~': return make_token(lexer, TOK_NOT);
    case '^': return make_token(lexer, TOK_XOR);
    
    case '+':
      if (peek(lexer) == '+') {
        advance(lexer);
        return make_token(lexer, TOK_INCREMENT);
      } else if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_PLUS_ASSIGN);
      }
      return make_token(lexer, TOK_PLUS);
      
    case '-':
      if (peek(lexer) == '-') {
        advance(lexer);
        return make_token(lexer, TOK_DECREMENT);
      } else if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_MINUS_ASSIGN);
      }
      return make_token(lexer, TOK_MINUS);
      
    case '*':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_MUL_ASSIGN);
      }
      return make_token(lexer, TOK_MULTIPLY);
      
    case '/':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_DIV_ASSIGN);
      }
      return make_token(lexer, TOK_DIVIDE);
      
    case '%':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_MOD_ASSIGN);
      }
      return make_token(lexer, TOK_MODULO);
      
    case '!':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_NE);
      }
      return make_token(lexer, TOK_LOGICAL_NOT);
      
    case '=':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_EQ);
      }
      return make_token(lexer, TOK_ASSIGN);
      
    case '<':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_LE);
      } else if (peek(lexer) == '<') {
        advance(lexer);
        return make_token(lexer, TOK_LSHIFT);
      }
      return make_token(lexer, TOK_LT);
      
    case '>':
      if (peek(lexer) == '=') {
        advance(lexer);
        return make_token(lexer, TOK_GE);
      } else if (peek(lexer) == '>') {
        advance(lexer);
        return make_token(lexer, TOK_RSHIFT);
      }
      return make_token(lexer, TOK_GT);
      
    case '&':
      if (peek(lexer) == '&') {
        advance(lexer);
        return make_token(lexer, TOK_LOGICAL_AND);
      }
      return make_token(lexer, TOK_AND);
      
    case '|':
      if (peek(lexer) == '|') {
        advance(lexer);
        return make_token(lexer, TOK_LOGICAL_OR);
      }
      return make_token(lexer, TOK_OR);
  }
  
  return make_error_token(lexer, "Unexpected character");
}

// Public API implementations
OrionError lexer_init(Lexer *lexer, const char *input, const char *filename) {
  if (!lexer || !input) return ORION_ERROR_INVALID_ARGUMENT;
  
  lexer->input = input;
  lexer->filename = filename;
  lexer->position = 0;
  lexer->length = strlen(input);
  lexer->current_pos.line = 1;
  lexer->current_pos.column = 1;
  lexer->current_pos.filename = filename;
  lexer->has_current_token = false;
  
  return ORION_SUCCESS;
}

void lexer_cleanup(Lexer *lexer) {
  if (!lexer) return;
  
  if (lexer->has_current_token && lexer->current_token.value) {
    free(lexer->current_token.value);
  }
}

OrionError lexer_next_token(Lexer *lexer) {
  if (!lexer) return ORION_ERROR_INVALID_ARGUMENT;
  
  if (lexer->has_current_token && lexer->current_token.value) {
    free(lexer->current_token.value);
  }
  
  lexer->current_token = scan_token(lexer);
  lexer->has_current_token = true;
  
  return (lexer->current_token.type == TOK_ERROR) ? ORION_ERROR_SYNTAX : ORION_SUCCESS;
}

Token lexer_peek_token(Lexer *lexer) {
  if (!lexer->has_current_token) {
    lexer_next_token(lexer);
  }
  return lexer->current_token;
}

bool lexer_at_end(Lexer *lexer) {
  if (!lexer->has_current_token) {
    lexer_next_token(lexer);
  }
  return lexer->current_token.type == TOK_EOF;
}

const char *token_type_to_string(TokenType type) {
  if (type >= 0 && type < sizeof(token_names) / sizeof(token_names[0])) {
    return token_names[type];
  }
  return "UNKNOWN";
}

void token_free(Token *token) {
  if (token && token->value) {
    free(token->value);
    token->value = NULL;
  }
}

Token token_copy(const Token *token) {
  Token copy = *token;
  if (token->value) {
    copy.value = string_duplicate(token->value);
  }
  return copy;
}

bool token_is_keyword(const Token *token) {
  return token->type >= TOK_IF && token->type <= TOK_CONTINUE;
}

bool token_is_operator(const Token *token) {
  return token->type >= TOK_ASSIGN && token->type <= TOK_LOGICAL_NOT;
}

bool token_is_literal(const Token *token) {
  return token->type == TOK_INT_LITERAL;
}

void lexer_dump_token(const Token *token) {
  printf("Token: %s", token_type_to_string(token->type));
  if (token->value) {
    printf(" ('%s')", token->value);
  }
  printf(" at line %d, column %d\n", token->pos.line, token->pos.column);
}

void lexer_dump_all_tokens(Lexer *lexer) {
  printf("=== Token Dump ===\n");
  while (!lexer_at_end(lexer)) {
    lexer_next_token(lexer);
    lexer_dump_token(&lexer->current_token);
  }
  printf("=== End Token Dump ===\n");
}
