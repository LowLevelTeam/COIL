#include "common.h"
#include <stdarg.h>

// Global debug flag
static bool g_debug_enabled = false;

void debug_enable(bool enabled) {
  g_debug_enabled = enabled;
}

char *string_duplicate(const char *str) {
  if (!str) return NULL;
  
  size_t len = strlen(str);
  char *copy = safe_malloc(len + 1);
  strcpy(copy, str);
  return copy;
}

void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr && size > 0) {
    fprintf(stderr, "Fatal error: Memory allocation failed for %zu bytes\n", size);
    exit(ORION_ERROR_MEMORY_ALLOCATION);
  }
  return ptr;
}

void *safe_realloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (!new_ptr && size > 0) {
    fprintf(stderr, "Fatal error: Memory reallocation failed for %zu bytes\n", size);
    exit(ORION_ERROR_MEMORY_ALLOCATION);
  }
  return new_ptr;
}

void error_exit(const char *message, SourcePosition pos) {
  if (pos.filename) {
    fprintf(stderr, "Error in %s at line %d, column %d: %s\n", 
            pos.filename, pos.line, pos.column, message);
  } else {
    fprintf(stderr, "Error at line %d, column %d: %s\n", 
            pos.line, pos.column, message);
  }
  exit(ORION_ERROR_SYNTAX);
}

void warning(const char *message, SourcePosition pos) {
  if (pos.filename) {
    fprintf(stderr, "Warning in %s at line %d, column %d: %s\n", 
            pos.filename, pos.line, pos.column, message);
  } else {
    fprintf(stderr, "Warning at line %d, column %d: %s\n", 
            pos.line, pos.column, message);
  }
}

void debug_print(const char *format, ...) {
  if (!g_debug_enabled) return;
  
  va_list args;
  va_start(args, format);
  fprintf(stderr, "[DEBUG] ");
  vfprintf(stderr, format, args);
  fprintf(stderr, "\n");
  va_end(args);
}

// Dynamic array helpers
void **dynamic_array_create(size_t initial_capacity, size_t *capacity) {
  if (initial_capacity == 0) initial_capacity = INITIAL_CAPACITY;
  
  void **array = safe_malloc(sizeof(void*) * initial_capacity);
  *capacity = initial_capacity;
  return array;
}

void **dynamic_array_resize(void **array, size_t *capacity) {
  size_t new_capacity = (*capacity) * 2;
  void **new_array = safe_realloc(array, sizeof(void*) * new_capacity);
  *capacity = new_capacity;
  return new_array;
}

void dynamic_array_free(void **array) {
  if (array) {
    free(array);
  }
}
