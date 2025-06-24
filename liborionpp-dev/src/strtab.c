#include "orionpp/strtab.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1024
#define INITIAL_HASH_SIZE 128
#define LOAD_FACTOR_THRESHOLD 0.75

// Default allocator implementation
static void* default_malloc(size_t size) { return malloc(size); }
static void* default_realloc(void* ptr, size_t size) { return realloc(ptr, size); }
static void default_free(void* ptr) { free(ptr); }

const orionpp_allocator_t orionpp_default_allocator = {
  .malloc = default_malloc,
  .realloc = default_realloc,
  .free = default_free
};

// Simple hash function (FNV-1a)
static uint32_t hash_string(const char* str, uint32_t length) {
  uint32_t hash = 2166136261u;
  for (uint32_t i = 0; i < length; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619u;
  }
  return hash;
}

// Resize hash table
static orionpp_result_t resize_hash_table(orionpp_string_table_t* table) {
  uint32_t old_size = table->hash_table_size;
  struct string_hash_entry* old_table = table->hash_table;
  
  table->hash_table_size *= 2;
  table->hash_table = table->allocator.malloc(
    sizeof(struct string_hash_entry*) * table->hash_table_size);
  
  if (!table->hash_table) {
    table->hash_table = old_table;
    table->hash_table_size = old_size;
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  memset(table->hash_table, 0, sizeof(struct string_hash_entry*) * table->hash_table_size);
  
  // Rehash all entries
  for (uint32_t i = 0; i < old_size; i++) {
    struct string_hash_entry* entry = old_table[i].next;
    while (entry) {
      struct string_hash_entry* next = entry->next;
      uint32_t bucket = entry->hash % table->hash_table_size;
      entry->next = table->hash_table[bucket].next;
      table->hash_table[bucket].next = entry;
      entry = next;
    }
  }
  
  table->allocator.free(old_table);
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_string_table_create(orionpp_string_table_t** table,
                                              const orionpp_allocator_t* allocator) {
  if (!table) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_string_table_t* t = allocator->malloc(sizeof(orionpp_string_table_t));
  if (!t) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  t->allocator = *allocator;
  t->capacity = INITIAL_CAPACITY;
  t->size = 1; // Reserve offset 0 for null/invalid
  t->data = t->allocator.malloc(t->capacity);
  
  if (!t->data) {
    t->allocator.free(t);
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  t->data[0] = '\0'; // Null string at offset 0
  
  // Initialize hash table
  t->hash_table_size = INITIAL_HASH_SIZE;
  t->hash_count = 0;
  t->hash_table = t->allocator.malloc(
    sizeof(struct string_hash_entry) * t->hash_table_size);
  
  if (!t->hash_table) {
    t->allocator.free(t->data);
    t->allocator.free(t);
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  memset(t->hash_table, 0, sizeof(struct string_hash_entry) * t->hash_table_size);
  
  *table = t;
  return ORIONPP_OK_PTR(t);
}

void orionpp_string_table_destroy(orionpp_string_table_t* table) {
  if (!table) return;
  
  // Free hash table entries
  if (table->hash_table) {
    for (uint32_t i = 0; i < table->hash_table_size; i++) {
      struct string_hash_entry* entry = table->hash_table[i].next;
      while (entry) {
        struct string_hash_entry* next = entry->next;
        table->allocator.free(entry);
        entry = next;
      }
    }
    table->allocator.free(table->hash_table);
  }
  
  table->allocator.free(table->data);
  table->allocator.free(table);
}

orionpp_result_t orionpp_string_table_add_length(orionpp_string_table_t* table,
                                                  const char* str,
                                                  uint32_t length,
                                                  uint32_t* offset) {
  if (!table || !str || !offset) {
    return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  }
  
  if (length == 0) {
    *offset = 0; // Return null string
    return ORIONPP_OK_INT(0);
  }
  
  uint32_t hash = hash_string(str, length);
  uint32_t bucket = hash % table->hash_table_size;
  
  // Check for existing string (deduplication)
  struct string_hash_entry* entry = &table->hash_table[bucket];
  while (entry) {
    if (entry->offset > 0 && entry->length == length && entry->hash == hash) {
      if (memcmp(table->data + entry->offset, str, length) == 0) {
        *offset = entry->offset;
        return ORIONPP_OK_INT(entry->offset);
      }
    }
    entry = entry->next;
  }
  
  // Ensure capacity
  uint32_t needed = length + 1; // +1 for null terminator
  if (table->size + needed > table->capacity) {
    uint32_t new_capacity = table->capacity;
    while (new_capacity < table->size + needed) {
      new_capacity *= 2;
    }
    
    char* new_data = table->allocator.realloc(table->data, new_capacity);
    if (!new_data) {
      return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
    }
    
    table->data = new_data;
    table->capacity = new_capacity;
  }
  
  // Add the string
  uint32_t string_offset = table->size;
  memcpy(table->data + string_offset, str, length);
  table->data[string_offset + length] = '\0';
  table->size += needed;
  
  // Add to hash table
  if ((double)table->hash_count / table->hash_table_size > LOAD_FACTOR_THRESHOLD) {
    orionpp_result_t resize_result = resize_hash_table(table);
    if (ORIONPP_IS_ERROR(resize_result)) {
      // Continue without rehashing - performance will degrade but it's not fatal
    }
    bucket = hash % table->hash_table_size; // Recalculate bucket after potential resize
  }
  
  struct string_hash_entry* new_entry = table->allocator.malloc(sizeof(struct string_hash_entry));
  if (new_entry) {
    new_entry->offset = string_offset;
    new_entry->length = length;
    new_entry->hash = hash;
    new_entry->next = table->hash_table[bucket].next;
    table->hash_table[bucket].next = new_entry;
    table->hash_count++;
  }
  // If allocation fails, we still have the string, just no deduplication for this entry
  
  *offset = string_offset;
  return ORIONPP_OK_INT(string_offset);
}

orionpp_result_t orionpp_string_table_add(orionpp_string_table_t* table,
                                           const char* str,
                                           uint32_t* offset) {
  if (!str) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  return orionpp_string_table_add_length(table, str, strlen(str), offset);
}

const char* orionpp_string_table_get(const orionpp_string_table_t* table,
                                      uint32_t offset) {
  if (!table || offset >= table->size) return NULL;
  return table->data + offset;
}

uint32_t orionpp_string_table_get_length(const orionpp_string_table_t* table,
                                          uint32_t offset) {
  if (!table || offset >= table->size) return 0;
  return strlen(table->data + offset);
}

bool orionpp_string_table_is_valid_offset(const orionpp_string_table_t* table,
                                           uint32_t offset) {
  return table && offset < table->size;
}

orionpp_result_t orionpp_string_table_write_binary(orionpp_string_table_t* table,
                                                    FILE* file) {
  if (!table || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  if (fwrite(table->data, 1, table->size, file) != table->size) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_string_table_read_binary(orionpp_string_table_t** table,
                                                   FILE* file,
                                                   uint32_t size,
                                                   const orionpp_allocator_t* allocator) {
  if (!table || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  orionpp_result_t create_result = orionpp_string_table_create(table, allocator);
  if (ORIONPP_IS_ERROR(create_result)) return create_result;
  
  orionpp_string_table_t* t = *table;
  
  // Resize to accommodate the data
  if (size > t->capacity) {
    char* new_data = t->allocator.realloc(t->data, size);
    if (!new_data) {
      orionpp_string_table_destroy(t);
      *table = NULL;
      return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
    }
    t->data = new_data;
    t->capacity = size;
  }
  
  if (fread(t->data, 1, size, file) != size) {
    orionpp_string_table_destroy(t);
    *table = NULL;
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  t->size = size;
  
  // Rebuild hash table for deduplication (optional optimization)
  // For now, we'll skip this since it's mainly for write performance
  
  return ORIONPP_OK_PTR(t);
}