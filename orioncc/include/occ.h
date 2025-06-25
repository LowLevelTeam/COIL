/**
 * @file include/occ.h  
 * @brief Main header file for Orion C Compiler
 */

#ifndef OCC_H
#define OCC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Forward declarations
typedef struct Token Token;
typedef struct ASTNode ASTNode;
typedef struct Parser Parser;
typedef struct CodeGen CodeGen;

// Common includes
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include "codegen.h"
#include "utils.h"

// Compiler options
typedef struct {
  const char* input_file;
  const char* output_file;
  bool verbose;
  bool debug_tokens;
  bool debug_ast;
} CompilerOptions;

// Main compiler function
int compile_file(const CompilerOptions* options);

#endif // OCC_H