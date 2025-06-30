#ifndef __ORIONPP_ARENA
#define __ORIONPP_ARENA

#include <stddef.h>
#include <orionpp/error.h>

typedef struct orionpp_arena orionpp_arena_t;

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