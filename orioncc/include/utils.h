/**
 * @file include/utils.h
 * @brief Utility functions for the compiler
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

// Memory management
void* safe_malloc(size_t size);
void* safe_realloc(void* ptr, size_t size);
char* safe_strdup(const char* str);

// String utilities
bool str_equals(const char* a, const char* b);
char* str_slice(const char* str, int start, int length);
void str_trim(char* str);

// File utilities
char* read_file(const char* filename);
bool write_file(const char* filename, const char* content, size_t length);

// Error reporting
void report_error(const char* filename, int line, int column, const char* message);
void report_info(const char* message);
void report_warning(const char* message);

#endif // UTILS_H