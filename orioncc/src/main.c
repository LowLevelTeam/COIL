#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"
#include <getopt.h>
#include <errno.h>

void print_version(void) {
  printf("Orion C Compiler v%d.%d.%d\n", 
         ORION_VERSION_MAJOR, ORION_VERSION_MINOR, ORION_VERSION_PATCH);
  printf("A basic C compiler targeting Orion++ ISA\n");
}

void print_usage(const char *program_name) {
  printf("Usage: %s [options] <input.c> [output.hopp]\n\n", program_name);
  printf("Options:\n");
  printf("  -h, --help         Show this help message\n");
  printf("  -v, --version      Show version information\n");
  printf("  -d, --debug        Enable debug mode\n");
  printf("  -V, --verbose      Enable verbose output\n");
  printf("  --dump-tokens      Dump lexer tokens to stderr\n");
  printf("  --dump-ast         Dump AST to stderr\n");
  printf("  -o, --output FILE  Specify output file\n");
  printf("\n");
  printf("Examples:\n");
  printf("  %s program.c                # Compile to program.hopp\n", program_name);
  printf("  %s -o output.hopp input.c   # Compile to specific output\n", program_name);
  printf("  %s --dump-ast program.c     # Show AST structure\n", program_name);
}

OrionError parse_arguments(int argc, char *argv[], CompilerOptions *options) {
  // Initialize default options
  options->input_file = NULL;
  options->output_file = NULL;
  options->debug_mode = false;
  options->verbose = false;
  options->dump_ast = false;
  options->dump_tokens = false;
  
  struct option long_options[] = {
    {"help",        no_argument,       0, 'h'},
    {"version",     no_argument,       0, 'v'},
    {"debug",       no_argument,       0, 'd'},
    {"verbose",     no_argument,       0, 'V'},
    {"dump-tokens", no_argument,       0, 't'},
    {"dump-ast",    no_argument,       0, 'a'},
    {"output",      required_argument, 0, 'o'},
    {0, 0, 0, 0}
  };
  
  int c;
  while ((c = getopt_long(argc, argv, "hvdVo:", long_options, NULL)) != -1) {
    switch (c) {
      case 'h':
        print_usage(argv[0]);
        exit(ORION_SUCCESS);
        
      case 'v':
        print_version();
        exit(ORION_SUCCESS);
        
      case 'd':
        options->debug_mode = true;
        debug_enable(true);
        break;
        
      case 'V':
        options->verbose = true;
        break;
        
      case 't':
        options->dump_tokens = true;
        break;
        
      case 'a':
        options->dump_ast = true;
        break;
        
      case 'o':
        options->output_file = optarg;
        break;
        
      case '?':
        return ORION_ERROR_INVALID_ARGUMENT;
        
      default:
        return ORION_ERROR_INVALID_ARGUMENT;
    }
  }
  
  // Get input file
  if (optind < argc) {
    options->input_file = argv[optind++];
  } else {
    fprintf(stderr, "Error: No input file specified\n");
    return ORION_ERROR_INVALID_ARGUMENT;
  }
  
  // Get output file if not specified with -o
  if (!options->output_file && optind < argc) {
    options->output_file = argv[optind++];
  }
  
  // Generate default output filename if none provided
  if (!options->output_file) {
    static char output_buffer[256];
    const char *input_base = strrchr(options->input_file, '/');
    input_base = input_base ? input_base + 1 : options->input_file;
    
    const char *dot = strrchr(input_base, '.');
    if (dot) {
      size_t base_len = dot - input_base;
      snprintf(output_buffer, sizeof(output_buffer), "%.*s.hopp", (int)base_len, input_base);
    } else {
      snprintf(output_buffer, sizeof(output_buffer), "%s.hopp", input_base);
    }
    options->output_file = output_buffer;
  }
  
  return ORION_SUCCESS;
}

char *read_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file '%s': %s\n", filename, strerror(errno));
    return NULL;
  }
  
  // Get file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (size < 0) {
    fprintf(stderr, "Error: Cannot determine size of file '%s'\n", filename);
    fclose(file);
    return NULL;
  }
  
  // Read file contents
  char *content = safe_malloc(size + 1);
  size_t bytes_read = fread(content, 1, size, file);
  content[bytes_read] = '\0';
  
  fclose(file);
  return content;
}

OrionError compile_file(CompilerOptions *options) {
  OrionError error = ORION_SUCCESS;
  char *input_content = NULL;
  Lexer lexer = {0};
  Parser parser = {0};
  ASTNode *program = NULL;
  FILE *output_file = NULL;
  CodeGen codegen = {0};
  
  if (options->verbose) {
    printf("Compiling %s -> %s\n", options->input_file, options->output_file);
  }
  
  // Read input file
  input_content = read_file(options->input_file);
  if (!input_content) {
    error = ORION_ERROR_FILE_NOT_FOUND;
    goto cleanup;
  }
  
  // Initialize lexer
  error = lexer_init(&lexer, input_content, options->input_file);
  if (error != ORION_SUCCESS) {
    fprintf(stderr, "Error: Failed to initialize lexer\n");
    goto cleanup;
  }
  
  // Dump tokens if requested
  if (options->dump_tokens) {
    printf("=== TOKEN DUMP ===\n");
    lexer_dump_all_tokens(&lexer);
    printf("=== END TOKEN DUMP ===\n");
    
    // Re-initialize lexer after dumping
    lexer_cleanup(&lexer);
    error = lexer_init(&lexer, input_content, options->input_file);
    if (error != ORION_SUCCESS) goto cleanup;
  }
  
  // Initialize parser
  error = parser_init(&parser, &lexer, options);
  if (error != ORION_SUCCESS) {
    fprintf(stderr, "Error: Failed to initialize parser\n");
    goto cleanup;
  }
  
  // Parse the program
  program = parser_parse_program(&parser);
  if (!program || parser.had_error) {
    fprintf(stderr, "Error: Parsing failed with %d errors\n", parser.error_count);
    error = ORION_ERROR_SYNTAX;
    goto cleanup;
  }
  
  if (options->verbose) {
    printf("Parsing completed successfully\n");
  }
  
  // Dump AST if requested
  if (options->dump_ast) {
    printf("=== AST DUMP ===\n");
    ast_print_tree(program);
    printf("=== END AST DUMP ===\n");
  }
  
  // Open output file
  output_file = fopen(options->output_file, "w");
  if (!output_file) {
    fprintf(stderr, "Error: Cannot create output file '%s': %s\n", 
            options->output_file, strerror(errno));
    error = ORION_ERROR_FILE_NOT_FOUND;
    goto cleanup;
  }
  
  // Initialize code generator
  error = codegen_init(&codegen, output_file, options);
  if (error != ORION_SUCCESS) {
    fprintf(stderr, "Error: Failed to initialize code generator\n");
    goto cleanup;
  }
  
  // Generate code
  error = codegen_generate_program(&codegen, program);
  if (error != ORION_SUCCESS || codegen.had_error) {
    fprintf(stderr, "Error: Code generation failed with %d errors\n", codegen.error_count);
    error = ORION_ERROR_CODEGEN;
    goto cleanup;
  }
  
  if (options->verbose) {
    printf("Code generation completed successfully\n");
  }
  
  printf("Compilation successful: %s -> %s\n", options->input_file, options->output_file);
  
cleanup:
  if (input_content) free(input_content);
  lexer_cleanup(&lexer);
  parser_cleanup(&parser);
  if (program) ast_free_node(program);
  if (output_file) fclose(output_file);
  codegen_cleanup(&codegen);
  
  return error;
}

int main(int argc, char *argv[]) {
  CompilerOptions options = {0};
  
  OrionError error = parse_arguments(argc, argv, &options);
  if (error != ORION_SUCCESS) {
    print_usage(argv[0]);
    return error;
  }
  
  error = compile_file(&options);
  return error;
}
