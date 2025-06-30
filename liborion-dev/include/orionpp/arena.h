#ifndef ORIONPP_ARENA_H
#define ORIONPP_ARENA_H

#include <stddef.h>
#include <orionpp/error.h>

/**
 * @file arena.h
 * @brief Memory arena allocator for efficient memory management
 * 
 * Provides bucket-based memory allocation with sequential access patterns
 * and built-in I/O capabilities for reading/writing data streams.
 */

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
  
  // I/O position tracking
  arena_bucket_t *io_bucket;
  size_t io_position;
  int io_mode; // 0 = read, 1 = write
} orionpp_arena_t;

// Core arena management
orionpp_error_t orionpp_arena_init(orionpp_arena_t *arena);
orionpp_error_t orionpp_arena_create(orionpp_arena_t *arena, size_t max_size, size_t bucket_size);
orionpp_error_t orionpp_arena_destroy(orionpp_arena_t *arena);
orionpp_error_t orionpp_arena_reset(orionpp_arena_t *arena);

// Memory allocation
orionpp_error_t orionpp_arena_alloc(orionpp_arena_t *arena, size_t size, void **ptr);
orionpp_error_t orionpp_arena_pop(orionpp_arena_t *arena, size_t size);

// I/O position management
orionpp_error_t orionpp_arena_seek(orionpp_arena_t *arena, size_t position);
orionpp_error_t orionpp_arena_tell(const orionpp_arena_t *arena, size_t *position);
orionpp_error_t orionpp_arena_rewind(orionpp_arena_t *arena);

// Data I/O operations
orionpp_error_t orionpp_arena_read(orionpp_arena_t *arena, void *buffer, size_t size, size_t *bytes_read);
orionpp_error_t orionpp_arena_write(orionpp_arena_t *arena, const void *buffer, size_t size, size_t *bytes_written);

// Utility functions
size_t orionpp_arena_used(const orionpp_arena_t *arena);
size_t orionpp_arena_available(const orionpp_arena_t *arena);

#endif // ORIONPP_ARENA_H