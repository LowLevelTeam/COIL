#include "orionobj/strtab.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 1024
#define INITIAL_HASH_SIZE 128
#define LOAD_FACTOR_THRESHOLD 0.75

// Simple hash function (FNV-1a)
static uint32_t hash_string(const char* str, uint32_t length) {
  uint32_t hash = 2166136261u;
  for (uint32_t i = 0; i < length; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619u;
  }
  return hash;
}

orionobj_result_t orionobj_string_table_create(orionobj_string_table_t** table,
                                                const orionobj_allocator_t* allocator) {
  if (!table) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionobj_default_allocator;

  orionobj_string_table_t* t = allocator->malloc(sizeof(orionobj_string_table_t));
  if (!t) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);

  t->allocator = *allocator;
  t->capacity = INITIAL_CAPACITY;
  t->size = 1; // Reserve offset 0 for null/invalid
  t->data = t->allocator.malloc(t->capacity);

  if (!t->data) {
    t->allocator.free(t);
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }

  t->data[0] = '\0'; // Null string at offset 0

  // Initialize hash table
  t->hash_table_size = INITIAL_HASH_SIZE;
  t->hash_count = 0;
  t->hash_table = t->allocator.malloc(
    sizeof(struct orionobj_string_hash_entry) * t->hash_table_size);

  if (!t->hash_table) {
    t->allocator.free(t->data);
    t->allocator.free(t);
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
  }

  // Initialize all hash table entries as sentinel/head nodes
  memset(t->hash_table, 0, sizeof(struct orionobj_string_hash_entry) * t->hash_table_size);

  *table = t;
  return ORIONOBJ_OK_PTR(t);
}

void orionobj_string_table_destroy(orionobj_string_table_t* table) {
  if (!table) return;

  // Free hash table entries
  if (table->hash_table) {
    for (uint32_t i = 0; i < table->hash_table_size; i++) {
      struct orionobj_string_hash_entry* entry = table->hash_table[i].next;
      while (entry) {
        struct orionobj_string_hash_entry* next = entry->next;
        table->allocator.free(entry);
        entry = next;
      }
    }
    table->allocator.free(table->hash_table);
  }

  table->allocator.free(table->data);
  table->allocator.free(table);
}

orionobj_result_t orionobj_string_table_add_length(orionobj_string_table_t* table,
                                                    const char* str,
                                                    uint32_t length,
                                                    uint32_t* offset) {
  if (!table || !str || !offset) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  }

  if (length == 0) {
    *offset = 0; // Return null string
    return ORIONOBJ_OK_INT(0);
  }

  uint32_t hash = hash_string(str, length);
  uint32_t bucket = hash % table->hash_table_size;

  // Check for existing string (deduplication)
  struct orionobj_string_hash_entry* entry = table->hash_table[bucket].next;
  while (entry) {
    if (entry->length == length && entry->hash == hash) {
      if (memcmp(table->data + entry->offset, str, length) == 0) {
        *offset = entry->offset;
        return ORIONOBJ_OK_INT(entry->offset);
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
      return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
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
  struct orionobj_string_hash_entry* new_entry = table->allocator.malloc(sizeof(struct orionobj_string_hash_entry));
  if (new_entry) {
    new_entry->offset = string_offset;
    new_entry->length = length;
    new_entry->hash = hash;
    new_entry->next = table->hash_table[bucket].next;
    table->hash_table[bucket].next = new_entry;
    table->hash_count++;
  }

  *offset = string_offset;
  return ORIONOBJ_OK_INT(string_offset);
}

orionobj_result_t orionobj_string_table_add(orionobj_string_table_t* table,
                                             const char* str,
                                             uint32_t* offset) {
  if (!str) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  return orionobj_string_table_add_length(table, str, strlen(str), offset);
}

const char* orionobj_string_table_get(const orionobj_string_table_t* table,
                                       uint32_t offset) {
  if (!table || offset >= table->size) return NULL;
  return table->data + offset;
}

uint32_t orionobj_string_table_get_length(const orionobj_string_table_t* table,
                                           uint32_t offset) {
  if (!table || offset >= table->size) return 0;
  return strlen(table->data + offset);
}

bool orionobj_string_table_is_valid_offset(const orionobj_string_table_t* table,
                                            uint32_t offset) {
  return table && offset < table->size;
}

uint32_t orionobj_string_table_get_size(const orionobj_string_table_t* table) {
  return table ? table->size : 0;
}

orionobj_result_t orionobj_string_table_write_binary(const orionobj_string_table_t* table,
                                                      FILE* file) {
  if (!table || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  if (fwrite(table->data, 1, table->size, file) != table->size) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_string_table_read_binary(orionobj_string_table_t** table,
                                                     FILE* file,
                                                     uint32_t size,
                                                     const orionobj_allocator_t* allocator) {
  if (!table || !file) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);

  orionobj_result_t create_result = orionobj_string_table_create(table, allocator);
  if (ORIONOBJ_IS_ERROR(create_result)) return create_result;

  orionobj_string_table_t* t = *table;

  // Resize to accommodate the data
  if (size > t->capacity) {
    char* new_data = t->allocator.realloc(t->data, size);
    if (!new_data) {
      orionobj_string_table_destroy(t);
      *table = NULL;
      return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);
    }
    t->data = new_data;
    t->capacity = size;
  }

  if (fread(t->data, 1, size, file) != size) {
    orionobj_string_table_destroy(t);
    *table = NULL;
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_IO_ERROR);
  }

  t->size = size;
  return ORIONOBJ_OK_PTR(t);
}

orionobj_result_t orionobj_string_table_validate(const orionobj_string_table_t* table) {
  if (!table) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (!table->data) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_CORRUPT_DATA);
  if (table->size == 0) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_CORRUPT_DATA);
  if (table->data[0] != '\0') return ORIONOBJ_ERROR(ORIONOBJ_ERROR_CORRUPT_DATA);
  return ORIONOBJ_OK_INT(0);
}