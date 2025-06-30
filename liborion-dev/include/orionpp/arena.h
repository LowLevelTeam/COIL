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
  
  // IO position tracking
  arena_bucket_t *io_bucket;
  size_t io_position;
  int io_mode; // 0 = read, 1 = write
} orionpp_arena_t;

orionpp_error_t orionpp_arena_init(orionpp_arena_t *arena);
orionpp_error_t orionpp_arena_create(orionpp_arena_t *arena, size_t max_size, size_t bucket_size);
orionpp_error_t orionpp_arena_destroy(orionpp_arena_t *arena);

// Allocation functions
orionpp_error_t orionpp_arena_alloc(orionpp_arena_t *arena, size_t size, void **ptr);
orionpp_error_t orionpp_arena_pop(orionpp_arena_t *arena, size_t size);

// IO position management
orionpp_error_t orionpp_arena_seek(orionpp_arena_t *arena, size_t position);
orionpp_error_t orionpp_arena_tell(const orionpp_arena_t *arena, size_t *position);
orionpp_error_t orionpp_arena_rewind(orionpp_arena_t *arena);

// Data read/write functions
orionpp_error_t orionpp_arena_read(orionpp_arena_t *arena, void *buffer, size_t size, size_t *bytes_read);
orionpp_error_t orionpp_arena_write(orionpp_arena_t *arena, const void *buffer, size_t size, size_t *bytes_written);

// Additional utility functions
orionpp_error_t orionpp_arena_reset(orionpp_arena_t *arena);
size_t orionpp_arena_used(const orionpp_arena_t *arena);
size_t orionpp_arena_available(const orionpp_arena_t *arena);

#endif // __ORIONPP_ARENA