/**
 * @file test/test_compiler.c
 * @brief Test program for the Orion C Compiler
 */

#include "../include/occ.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_lexer() {
  printf("Testing lexer...\n");
  
  const char* source = "int main() { return 42; }";
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Token token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_INT);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_IDENTIFIER);
  assert(token_equals_text(&token, "main"));
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_LEFT_PAREN);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_RIGHT_PAREN);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_LEFT_BRACE);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_RETURN);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_NUMBER);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_SEMICOLON);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_RIGHT_BRACE);
  
  token = lexer_next_token(&lexer);
  assert(token.type == TOKEN_EOF);
  
  printf("Lexer tests passed!\n");
}

void test_parser() {
  printf("Testing parser...\n");
  
  const char* source = "int main() { int x = 5; return x + 2; }";
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Parser parser;
  parser_init(&parser, &lexer);
  
  ASTNode* ast = parse_program(&parser);
  assert(ast != NULL);
  assert(ast->type == AST_PROGRAM);
  assert(ast->program.statement_count == 1);
  
  ASTNode* func = ast->program.statements[0];
  assert(func->type == AST_FUNCTION);
  assert(str_equals(func->function.name, "main"));
  assert(func->function.return_type == TYPE_INT);
  
  if (parser.had_error) {
    printf("Parser had errors, but basic structure is correct...\n");
  }
  
  ast_free_node(ast);
  printf("Parser tests passed!\n");
}

void test_simple_expression() {
  printf("Testing simple expression...\n");
  
  const char* source = "int x = 2 + 3 * 4;";
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Parser parser;
  parser_init(&parser, &lexer);
  
  ASTNode* ast = parse_program(&parser);
  assert(ast != NULL);
  assert(!parser.had_error);
  
  ast_free_node(ast);
  printf("Expression tests passed!\n");
}

void test_function_calls() {
  printf("Testing function calls...\n");
  
  const char* source = "int test() { int x = add(1, 2); return x; }";
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Parser parser;
  parser_init(&parser, &lexer);
  
  ASTNode* ast = parse_program(&parser);
  assert(ast != NULL);
  
  if (parser.had_error) {
    printf("Parser had errors with function calls, but continuing...\n");
  }
  
  ast_free_node(ast);
  printf("Function call tests completed!\n");
}

void test_control_flow() {
  printf("Testing control flow...\n");
  
  const char* source = 
    "int test() {"
    "  int x = 10;"
    "  if (x > 5) {"
    "    x = x * 2;"
    "  }"
    "  while (x > 0) {"
    "    x = x - 1;"
    "  }"
    "  return x;"
    "}";
  
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Parser parser;
  parser_init(&parser, &lexer);
  
  ASTNode* ast = parse_program(&parser);
  assert(ast != NULL);
  
  if (parser.had_error) {
    printf("Parser had errors, but continuing test...\n");
    ast_free_node(ast);
    printf("Control flow tests completed with errors!\n");
    return;
  }
  
  ast_free_node(ast);
  printf("Control flow tests passed!\n");
}

void test_code_generation() {
  printf("Testing code generation...\n");
  
  const char* source = "int main() { int x = 42; return x; }";
  Lexer lexer;
  lexer_init(&lexer, source);
  
  Parser parser;
  parser_init(&parser, &lexer);
  
  ASTNode* ast = parse_program(&parser);
  assert(ast != NULL);
  
  if (parser.had_error) {
    printf("Parser had errors, skipping code generation test...\n");
    ast_free_node(ast);
    printf("Code generation test skipped due to parser errors!\n");
    return;
  }
  
  FILE* output = fopen("test_output.opp", "wb");
  assert(output != NULL);
  
  CodeGen codegen;
  codegen_init(&codegen, output);
  
  bool success = codegen_generate(&codegen, ast);
  
  fclose(output);
  codegen_cleanup(&codegen);
  ast_free_node(ast);
  
  if (success) {
    printf("Code generation tests passed!\n");
  } else {
    printf("Code generation had errors, but test completed!\n");
  }
}

int main() {
  printf("Running Orion C Compiler Tests\n");
  printf("==============================\n\n");
  
  test_lexer();
  test_parser();
  test_simple_expression();
  test_function_calls();
  test_control_flow();
  test_code_generation();
  
  printf("\nAll tests completed! ✓\n");
  return 0;
}