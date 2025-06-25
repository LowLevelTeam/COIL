/**
 * @file src/main.c
 * @brief Main entry point for Orion C Compiler
 */

#include "occ.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char* program_name) {
  printf("Usage: %s [options] input_file\n", program_name);
  printf("Options:\n");
  printf("  -o <file>     Output file (default: out.opp)\n");
  printf("  -v            Verbose output\n");
  printf("  --debug-tokens Debug tokenization\n");
  printf("  --debug-ast   Debug AST generation\n");
  printf("  -h, --help    Show this help message\n");
}

static void parse_arguments(int argc, const char* argv[], CompilerOptions* options) {
  // Initialize default options
  options->input_file = NULL;
  options->output_file = "out.opp";
  options->verbose = false;
  options->debug_tokens = false;
  options->debug_ast = false;
  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Error: -o requires an argument\n");
        exit(1);
      }
      options->output_file = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0) {
      options->verbose = true;
    } else if (strcmp(argv[i], "--debug-tokens") == 0) {
      options->debug_tokens = true;
    } else if (strcmp(argv[i], "--debug-ast") == 0) {
      options->debug_ast = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      exit(0);
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
      exit(1);
    } else {
      if (options->input_file != NULL) {
        fprintf(stderr, "Error: Multiple input files specified\n");
        exit(1);
      }
      options->input_file = argv[i];
    }
  }
  
  if (options->input_file == NULL) {
    fprintf(stderr, "Error: No input file specified\n");
    print_usage(argv[0]);
    exit(1);
  }
}

int compile_file(const CompilerOptions* options) {
  if (options->verbose) {
    report_info("Starting compilation...");
    printf("Input: %s\n", options->input_file);
    printf("Output: %s\n", options->output_file);
  }
  
  // Read source file
  char* source = read_file(options->input_file);
  if (!source) {
    fprintf(stderr, "Error: Could not read file '%s'\n", options->input_file);
    return 1;
  }
  
  // Initialize lexer
  Lexer lexer;
  lexer_init(&lexer, source);
  
  // Debug tokenization if requested
  if (options->debug_tokens) {
    printf("=== TOKENS ===\n");
    Lexer debug_lexer = lexer;
    Token token;
    do {
      token = lexer_next_token(&debug_lexer);
      printf("Line %d, Col %d: %s", token.line, token.column, token_type_to_string(token.type));
      if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER || 
          token.type == TOKEN_STRING || token.type == TOKEN_CHAR) {
        char* text = token_to_string(&token);
        printf(" '%s'", text);
        free(text);
      }
      printf("\n");
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    printf("=== END TOKENS ===\n\n");
  }
  
  // Initialize parser
  Parser parser;
  parser_init(&parser, &lexer);
  
  // Parse source code
  ASTNode* ast = parse_program(&parser);
  if (!ast || parser.had_error) {
    fprintf(stderr, "Error: Parsing failed\n");
    free(source);
    if (ast) ast_free_node(ast);
    return 1;
  }
  
  // Debug AST if requested
  if (options->debug_ast) {
    printf("=== AST ===\n");
    ast_print(ast, 0);
    printf("=== END AST ===\n\n");
  }
  
  // Open output file
  FILE* output = fopen(options->output_file, "wb");
  if (!output) {
    fprintf(stderr, "Error: Could not open output file '%s'\n", options->output_file);
    free(source);
    ast_free_node(ast);
    return 1;
  }
  
  // Initialize code generator
  CodeGen codegen;
  codegen_init(&codegen, output);
  
  // Generate code
  bool success = codegen_generate(&codegen, ast);
  if (!success) {
    fprintf(stderr, "Error: Code generation failed\n");
    fclose(output);
    codegen_cleanup(&codegen);
    free(source);
    ast_free_node(ast);
    return 1;
  }
  
  // Cleanup
  fclose(output);
  codegen_cleanup(&codegen);
  free(source);
  ast_free_node(ast);
  
  if (options->verbose) {
    report_info("Compilation completed successfully");
  }
  
  return 0;
}

int main(int argc, const char* argv[]) {
  CompilerOptions options;
  parse_arguments(argc, argv, &options);
  
  return compile_file(&options);
}