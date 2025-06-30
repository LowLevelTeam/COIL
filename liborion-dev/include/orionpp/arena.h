#ifndef __ORIONPP_ARENA
#define __ORIONPP_ARENA

typedef struct orionpp_arena {
  // TODO...
} orionpp_arena_t;

orionpp_error_t orionpp_arena_init(orion_arena_t *);
orionpp_error_t orionpp_arena_create(orion_arena_t *, size_t max_size, size_t bucket_size);
orionpp_error_t orionpp_arena_destroy(orion_arena_t *);

// push an immediate value
orionpp_error_t orionpp_arena_alloc(orion_arena_t *, size_t, void**);
orionpp_error_t orionpp_arena_pop(orion_arena_t *, size_t);

#endif // __ORIONPP_ARENA