/**
 * @file src/utils.c
 * @brief Utility functions implementation
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void* safe_malloc(size_t size) {
  void* ptr = malloc(size);
  if (!ptr) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(1);
  }
  return ptr;
}

void* safe_realloc(void* ptr, size_t size) {
  void* new_ptr = realloc(ptr, size);
  if (!new_ptr && size > 0) {
    fprintf(stderr, "Error: Memory reallocation failed\n");
    exit(1);
  }
  return new_ptr;
}

char* safe_strdup(const char* str) {
  if (!str) return NULL;
  
  size_t len = strlen(str);
  char* copy = safe_malloc(len + 1);
  memcpy(copy, str, len + 1);
  return copy;
}

bool str_equals(const char* a, const char* b) {
  if (a == b) return true;
  if (!a || !b) return false;
  return strcmp(a, b) == 0;
}

char* str_slice(const char* str, int start, int length) {
  if (!str) return NULL;
  
  int str_len = (int)strlen(str);
  if (start < 0 || start >= str_len || length <= 0) {
    return safe_strdup("");
  }
  
  if (start + length > str_len) {
    length = str_len - start;
  }
  
  char* slice = safe_malloc(length + 1);
  memcpy(slice, str + start, length);
  slice[length] = '\0';
  return slice;
}

void str_trim(char* str) {
  if (!str) return;
  
  // Trim leading whitespace
  char* start = str;
  while (isspace(*start)) {
    start++;
  }
  
  // Move trimmed string to beginning
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }
  
  // Trim trailing whitespace
  char* end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) {
    *end = '\0';
    end--;
  }
}

char* read_file(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    return NULL;
  }
  
  // Get file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (size < 0) {
    fclose(file);
    return NULL;
  }
  
  // Allocate buffer
  char* buffer = safe_malloc(size + 1);
  
  // Read file
  size_t bytes_read = fread(buffer, 1, size, file);
  fclose(file);
  
  if (bytes_read != (size_t)size) {
    free(buffer);
    return NULL;
  }
  
  buffer[size] = '\0';
  return buffer;
}

bool write_file(const char* filename, const char* content, size_t length) {
  FILE* file = fopen(filename, "wb");
  if (!file) {
    return false;
  }
  
  size_t bytes_written = fwrite(content, 1, length, file);
  fclose(file);
  
  return bytes_written == length;
}

void report_error(const char* filename, int line, int column, const char* message) {
  if (filename) {
    fprintf(stderr, "%s:%d:%d: error: %s\n", filename, line, column, message);
  } else {
    fprintf(stderr, "error: %s\n", message);
  }
}

void report_info(const char* message) {
  printf("info: %s\n", message);
}

void report_warning(const char* message) {
  fprintf(stderr, "warning: %s\n", message);
}