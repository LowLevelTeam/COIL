#ifndef __ORIONPP_ARENA
#define __ORIONPP_ARENA

#include <stddef.h>
#include <orionpp/error.h>

typedef struct arena_bucket {
  void *data;
  size_t size;
  size_t used;
  struct arena_bucket *next;
} arena_bucket_t;

typedef struct orionpp_arena {
  arena_bucket_t *head;
  arena_bucket_t *current;
  size_t max_size;
  size_t bucket_size;
  size_t total_allocated;
  int initialized;
} orionpp_arena_t;

orionpp_error_t orionpp_arena_init(orionpp_arena_t *arena);
orionpp_error_t orionpp_arena_create(orionpp_arena_t *arena, size_t max_size, size_t bucket_size);
orionpp_error_t orionpp_arena_destroy(orionpp_arena_t *arena);

// Allocation functions
orionpp_error_t orionpp_arena_alloc(orionpp_arena_t *arena, size_t size, void **ptr);
orionpp_error_t orionpp_arena_pop(orionpp_arena_t *arena, size_t size);

// Additional utility functions
orionpp_error_t orionpp_arena_reset(orionpp_arena_t *arena);
size_t orionpp_arena_used(const orionpp_arena_t *arena);
size_t orionpp_arena_available(const orionpp_arena_t *arena);

#endif // __ORIONPP_ARENA