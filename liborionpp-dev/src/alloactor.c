#include "orionpp/core/allocator.h"
#include <stdlib.h>

// Default allocator implementation
static void* default_malloc(size_t size) { 
  return malloc(size); 
}

static void* default_realloc(void* ptr, size_t size) { 
  return realloc(ptr, size); 
}

static void default_free(void* ptr) { 
  free(ptr); 
}

const orionpp_allocator_t orionpp_default_allocator = {
  .malloc = default_malloc,
  .realloc = default_realloc,
  .free = default_free
};

orionpp_allocator_t orionpp_allocator_create(orionpp_malloc_fn malloc_fn,
                                              orionpp_realloc_fn realloc_fn,
                                              orionpp_free_fn free_fn) {
  orionpp_allocator_t allocator = {
    .malloc = malloc_fn ? malloc_fn : default_malloc,
    .realloc = realloc_fn ? realloc_fn : default_realloc,
    .free = free_fn ? free_fn : default_free
  };
  return allocator;
}