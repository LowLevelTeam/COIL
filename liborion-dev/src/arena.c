#include "orionpp/arena.h"
#include "orionpp/error.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

orionpp_error_t orionpp_arena_init(orionpp_arena_t *arena) {
  if (!arena) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  memset(arena, 0, sizeof(orionpp_arena_t));
  arena->initialized = 1;
  arena->io_bucket = NULL;
  arena->io_position = 0;
  arena->io_mode = 0;
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_create(orionpp_arena_t *arena, size_t max_size, size_t bucket_size) {
  if (!arena || max_size == 0 || bucket_size == 0) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  orionpp_error_t err = orionpp_arena_init(arena);
  if (err != ORIONPP_ERROR_GOOD) {
    return err;
  }
  
  arena->max_size = max_size;
  arena->bucket_size = bucket_size;
  
  // Create initial bucket
  arena_bucket_t *bucket = malloc(sizeof(arena_bucket_t));
  if (!bucket) {
    return ORIONPP_ERROR_OUT_OF_MEMORY;
  }
  
  bucket->data = malloc(bucket_size);
  if (!bucket->data) {
    free(bucket);
    return ORIONPP_ERROR_OUT_OF_MEMORY;
  }
  
  bucket->size = bucket_size;
  bucket->used = 0;
  bucket->next = NULL;
  
  arena->head = bucket;
  arena->current = bucket;
  arena->total_allocated = bucket_size;
  arena->io_bucket = bucket;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_destroy(orionpp_arena_t *arena) {
  if (!arena || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  arena_bucket_t *current = arena->head;
  while (current) {
    arena_bucket_t *next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
  
  memset(arena, 0, sizeof(orionpp_arena_t));
  return ORIONPP_ERROR_GOOD;
}

static orionpp_error_t arena_add_bucket(orionpp_arena_t *arena) {
  if (arena->total_allocated + arena->bucket_size > arena->max_size) {
    return ORIONPP_ERROR_ARENA_FULL;
  }
  
  arena_bucket_t *bucket = malloc(sizeof(arena_bucket_t));
  if (!bucket) {
    return ORIONPP_ERROR_OUT_OF_MEMORY;
  }
  
  bucket->data = malloc(arena->bucket_size);
  if (!bucket->data) {
    free(bucket);
    return ORIONPP_ERROR_OUT_OF_MEMORY;
  }
  
  bucket->size = arena->bucket_size;
  bucket->used = 0;
  bucket->next = NULL;
  
  // Find the last bucket and append
  arena_bucket_t *last = arena->head;
  while (last->next) {
    last = last->next;
  }
  last->next = bucket;
  
  arena->current = bucket;
  arena->total_allocated += arena->bucket_size;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_alloc(orionpp_arena_t *arena, size_t size, void **ptr) {
  if (!arena || !ptr || size == 0) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  if (!arena->initialized) {
    return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  }
  
  if (!arena->current) {
    return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  }
  
  // Align size to 8-byte boundary
  size_t aligned_size = (size + 7) & ~7;
  
  // Check if current bucket has enough space
  if (arena->current->used + aligned_size > arena->current->size) {
    // Try to add a new bucket
    orionpp_error_t err = arena_add_bucket(arena);
    if (err != ORIONPP_ERROR_GOOD) {
      return err;
    }
  }
  
  // Allocate from current bucket
  *ptr = (char*)arena->current->data + arena->current->used;
  arena->current->used += aligned_size;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_pop(orionpp_arena_t *arena, size_t size) {
  if (!arena || size == 0) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  if (!arena->initialized || !arena->current) {
    return ORIONPP_ERROR_ARENA_NOT_INITIALIZED;
  }
  
  // Align size to 8-byte boundary
  size_t aligned_size = (size + 7) & ~7;
  
  if (aligned_size > arena->current->used) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  arena->current->used -= aligned_size;
  
  return ORIONPP_ERROR_GOOD;
}

// IO position management functions
orionpp_error_t orionpp_arena_seek(orionpp_arena_t *arena, size_t position) {
  if (!arena || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Find the bucket containing this position
  size_t current_pos = 0;
  arena_bucket_t *bucket = arena->head;
  
  while (bucket && current_pos + bucket->used <= position) {
    current_pos += bucket->used;
    bucket = bucket->next;
  }
  
  if (!bucket) {
    return ORIONPP_ERROR_INVALID_ARG; // Position beyond allocated data
  }
  
  arena->io_bucket = bucket;
  arena->io_position = position - current_pos;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_tell(const orionpp_arena_t *arena, size_t *position) {
  if (!arena || !position || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
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
  if (!arena || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  arena->io_bucket = arena->head;
  arena->io_position = 0;
  
  return ORIONPP_ERROR_GOOD;
}

orionpp_error_t orionpp_arena_read(orionpp_arena_t *arena, void *buffer, size_t size, size_t *bytes_read) {
  if (!arena || !buffer || !bytes_read || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  *bytes_read = 0;
  
  if (!arena->io_bucket || size == 0) {
    return ORIONPP_ERROR_GOOD;
  }
  
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
  if (!arena || !buffer || !bytes_written || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  *bytes_written = 0;
  
  if (size == 0) {
    return ORIONPP_ERROR_GOOD;
  }
  
  arena->io_mode = 1; // Write mode
  const char *src = (const char*)buffer;
  size_t remaining = size;
  
  while (remaining > 0) {
    if (!arena->io_bucket) {
      // Need to allocate or move to first bucket
      arena->io_bucket = arena->head;
      arena->io_position = 0;
    }
    
    size_t available_in_bucket = arena->io_bucket->size - arena->io_position;
    
    if (available_in_bucket == 0) {
      // Current bucket is full, try to move to next or create new
      if (arena->io_bucket->next) {
        arena->io_bucket = arena->io_bucket->next;
        arena->io_position = 0;
        continue;
      } else {
        // Need to add a new bucket
        orionpp_error_t err = arena_add_bucket(arena);
        if (err != ORIONPP_ERROR_GOOD) {
          return err;
        }
        arena->io_bucket = arena->current; // Point to new bucket
        arena->io_position = 0;
        available_in_bucket = arena->io_bucket->size;
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

orionpp_error_t orionpp_arena_reset(orionpp_arena_t *arena) {
  if (!arena || !arena->initialized) {
    return ORIONPP_ERROR_INVALID_ARG;
  }
  
  // Reset all buckets to unused
  arena_bucket_t *bucket = arena->head;
  while (bucket) {
    bucket->used = 0;
    bucket = bucket->next;
  }
  
  // Reset current and IO pointers to head
  arena->current = arena->head;
  arena->io_bucket = arena->head;
  arena->io_position = 0;
  
  return ORIONPP_ERROR_GOOD;
}

size_t orionpp_arena_used(const orionpp_arena_t *arena) {
  if (!arena || !arena->initialized) {
    return 0;
  }
  
  size_t total_used = 0;
  const arena_bucket_t *bucket = arena->head;
  while (bucket) {
    total_used += bucket->used;
    bucket = bucket->next;
  }
  
  return total_used;
}

size_t orionpp_arena_available(const orionpp_arena_t *arena) {
  if (!arena || !arena->initialized) {
    return 0;
  }
  
  return arena->max_size - arena->total_allocated;
}