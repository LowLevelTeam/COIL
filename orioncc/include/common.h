#ifndef ORION_COMMON_H
#define ORION_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

// Version information
#define ORION_VERSION_MAJOR 0
#define ORION_VERSION_MINOR 1
#define ORION_VERSION_PATCH 0

// Compiler limits
#define MAX_IDENTIFIER_LENGTH 256
#define MAX_FUNCTIONS 1024
#define MAX_PARAMETERS 32
#define MAX_STATEMENTS 1024
#define MAX_ARGUMENTS 32
#define INITIAL_CAPACITY 16

// Error codes
typedef enum {
  ORION_SUCCESS = 0,
  ORION_ERROR_FILE_NOT_FOUND,
  ORION_ERROR_MEMORY_ALLOCATION,
  ORION_ERROR_SYNTAX,
  ORION_ERROR_SEMANTIC,
  ORION_ERROR_CODEGEN,
  ORION_ERROR_INVALID_ARGUMENT
} OrionError;

// Position tracking for error reporting
typedef struct {
  int line;
  int column;
  const char *filename;
} SourcePosition;

// Output format types
typedef enum {
  OUTPUT_BINARY,  // Orion++ binary format (.orionpp)
  OUTPUT_HUMAN    // Human-readable assembly (.hopp)
} OutputFormat;

// Compiler options
typedef struct {
  const char *input_file;
  const char *output_file;
  bool debug_mode;
  bool verbose;
  bool dump_ast;
  bool dump_tokens;
  OutputFormat output_format;
} CompilerOptions;

// Utility functions
char *string_duplicate(const char *str);
void *safe_malloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
void error_exit(const char *message, SourcePosition pos);
void warning(const char *message, SourcePosition pos);
void debug_enable(bool enabled);
void debug_print(const char *format, ...);

// Memory management helpers
void **dynamic_array_create(size_t initial_capacity, size_t *capacity);
void **dynamic_array_resize(void **array, size_t *capacity);
void dynamic_array_free(void **array);

#endif // ORION_COMMON_H