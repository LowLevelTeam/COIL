#include "orionpp/arena.h"
#include <string.h>

/**
 * @file arena_io.c
 * @brief Arena I/O operations implementation
 * 
 * Implements reading and writing operations for arenas, treating
 * them as sequential data streams with position tracking.
 */

static orionpp_error_t arena_validate_io(const orionpp_arena_t *arena) {
  if (!arena) return ORIONPP_ERROR_INVALID_ARG;
  if (!arena->initialized) return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  return ORIONPP_ERROR_GOOD;
}

static const arena_bucket_t *find_bucket_at_position(const orionpp_arena_t *arena, size_t position, size_t *bucket_offset) {
  size_t current_pos = 0;
  const arena_bucket_t *bucket = arena->head;
  
  while (bucket && current_pos + bucket->used <= position) {
    current_pos += bucket->used;
    bucket = bucket->next;
  }
  
  if (bucket_offset) {
    *bucket_offset = current_pos;
  }
  
  return bucket;
}

orionpp_error_t orionpp_arena_seek(orionpp_arena_t *arena, size_t position) {
  orionpp_error_t err = arena_validate_io(arena);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  size_t bucket_offset;
  const arena_bucket_t *bucket = find_bucket_at_position(arena, position, &bucket_offset);
  
  if (!bucket) {
    return ORIONPP_ERROR_INVALID_ARG; // Position beyond allocated data
  }
  
  arena->io_bucket = (arena_bucket_t *)bucket;
  arena->io_position = position - bucket_offset;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_tell(const orionpp_arena_t *arena, size_t *position) {
  if (!position) return ORIONPP_ERROR_INVALID_ARG;
  
  orionpp_error_t err = arena_validate_io(arena);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  if (!arena->io_bucket) {
    *position = 0;
    return ORIONPP_ERROR_GOOD;
  }
  
  // Calculate absolute position
  size_t abs_pos = 0;
  const arena_bucket_t *bucket = arena->head;
  
  while (bucket && bucket != arena->io_bucket) {
    abs_pos += bucket->used;
    bucket = bucket->next;
  }
  
  if (bucket == arena->io_bucket) {
    abs_pos += arena->io_position;
  }
  
  *position = abs_pos;
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_rewind(orionpp_arena_t *arena) {
  orionpp_error_t err = arena_validate_io(arena);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  arena->io_bucket = arena->head;
  arena->io_position = 0;
  arena->io_mode = 0;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_read(orionpp_arena_t *arena, void *buffer, size_t size, size_t *bytes_read) {
  if (!buffer || !bytes_read) return ORIONPP_ERROR_INVALID_ARG;
  
  orionpp_error_t err = arena_validate_io(arena);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  *bytes_read = 0;
  if (!arena->io_bucket || size == 0) return ORIONPP_ERROR_GOOD;
  
  arena->io_mode = 0; // Read mode
  char *dest = (char*)buffer;
  size_t remaining = size;
  
  while (remaining > 0 && arena->io_bucket) {
    size_t available_in_bucket = arena->io_bucket->used - arena->io_position;
    
    if (available_in_bucket == 0) {
      // Move to next bucket
      arena->io_bucket = arena->io_bucket->next;
      arena->io_position = 0;
      continue;
    }
    
    size_t to_copy = (remaining < available_in_bucket) ? remaining : available_in_bucket;
    
    memcpy(dest, (char*)arena->io_bucket->data + arena->io_position, to_copy);
    
    dest += to_copy;
    arena->io_position += to_copy;
    remaining -= to_copy;
    *bytes_read += to_copy;
  }
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_write(orionpp_arena_t *arena, const void *buffer, size_t size, size_t *bytes_written) {
  if (!buffer || !bytes_written) return ORIONPP_ERROR_INVALID_ARG;
  
  orionpp_error_t err = arena_validate_io(arena);
  if (err != ORIONPP_ERROR_GOOD) return err;
  
  *bytes_written = 0;
  if (size == 0) return ORIONPP_ERROR_GOOD;
  
  arena->io_mode = 1; // Write mode
  const char *src = (const char*)buffer;
  size_t remaining = size;
  
  while (remaining > 0) {
    if (!arena->io_bucket) {
      arena->io_bucket = arena->head;
      arena->io_position = 0;
    }
    
    size_t available_in_bucket = arena->io_bucket->size - arena->io_position;
    
    if (available_in_bucket == 0) {
      // Try to move to next bucket or create new one
      if (arena->io_bucket->next) {
        arena->io_bucket = arena->io_bucket->next;
        arena->io_position = 0;
        continue;
      } else {
        // Need to add a new bucket
        if (arena->total_allocated + arena->bucket_size > arena->max_size) {
          return ORIONPP_ERROR_ARENA_FULL;
        }
        
        arena_bucket_t *bucket = malloc(sizeof(arena_bucket_t));
        if (!bucket) return ORIONPP_ERROR_OUT_OF_MEMORY;
        
        bucket->data = malloc(arena->bucket_size);
        if (!bucket->data) {
          free(bucket);
          return ORIONPP_ERROR_OUT_OF_MEMORY;
        }
        
        bucket->size = arena->bucket_size;
        bucket->used = 0;
        bucket->next = NULL;
        
        arena->io_bucket->next = bucket;
        arena->io_bucket = bucket;
        arena->io_position = 0;
        arena->total_allocated += arena->bucket_size;
        available_in_bucket = arena->bucket_size;
      }
    }
    
    size_t to_copy = (remaining < available_in_bucket) ? remaining : available_in_bucket;
    
    memcpy((char*)arena->io_bucket->data + arena->io_position, src, to_copy);
    
    src += to_copy;
    arena->io_position += to_copy;
    remaining -= to_copy;
    *bytes_written += to_copy;
    
    // Update used space in bucket
    if (arena->io_position > arena->io_bucket->used) {
      arena->io_bucket->used = arena->io_position;
    }
  }
  
  return ORIONPP_ERROR_GOOD;
}