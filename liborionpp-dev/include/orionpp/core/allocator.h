#ifndef ORIONPP_CORE_ALLOCATOR_H
#define ORIONPP_CORE_ALLOCATOR_H

#include <stddef.h>

// Memory allocation function pointers (for custom allocators)
typedef void* (*orionpp_malloc_fn)(size_t size);
typedef void* (*orionpp_realloc_fn)(void* ptr, size_t size);
typedef void (*orionpp_free_fn)(void* ptr);

typedef struct {
  orionpp_malloc_fn malloc;
  orionpp_realloc_fn realloc;
  orionpp_free_fn free;
} orionpp_allocator_t;

// Default allocator
extern const orionpp_allocator_t orionpp_default_allocator;

// Utility functions
orionpp_allocator_t orionpp_allocator_create(orionpp_malloc_fn malloc_fn,
                                              orionpp_realloc_fn realloc_fn,
                                              orionpp_free_fn free_fn);

#endif // ORIONPP_CORE_ALLOCATOR_H