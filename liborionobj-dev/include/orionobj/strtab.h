#ifndef ORIONOBJ_STRTAB_H
#define ORIONOBJ_STRTAB_H

#include "orionobj/types.h"

// String table for efficient string storage and deduplication
struct orionobj_string_table {
  char* data;
  uint32_t size;
  uint32_t capacity;
  orionobj_allocator_t allocator;
  
  // Hash table for deduplication
  struct orionobj_string_hash_entry* hash_table;
  uint32_t hash_table_size;
  uint32_t hash_count;
};

// Hash table entry for string deduplication
struct orionobj_string_hash_entry {
  uint32_t offset;
  uint32_t length;
  uint32_t hash;
  struct orionobj_string_hash_entry* next;
};

// String table API
orionobj_result_t orionobj_string_table_create(orionobj_string_table_t** table, 
                                                const orionobj_allocator_t* allocator);

void orionobj_string_table_destroy(orionobj_string_table_t* table);

// Add string and return offset (deduplicates automatically)
orionobj_result_t orionobj_string_table_add(orionobj_string_table_t* table, 
                                             const char* str, 
                                             uint32_t* offset);

// Add string with known length
orionobj_result_t orionobj_string_table_add_length(orionobj_string_table_t* table,
                                                    const char* str,
                                                    uint32_t length,
                                                    uint32_t* offset);

// Get string by offset
const char* orionobj_string_table_get(const orionobj_string_table_t* table, 
                                       uint32_t offset);

// Get string length by offset
uint32_t orionobj_string_table_get_length(const orionobj_string_table_t* table,
                                           uint32_t offset);

// Validate offset
bool orionobj_string_table_is_valid_offset(const orionobj_string_table_t* table,
                                            uint32_t offset);

// String table properties
uint32_t orionobj_string_table_get_size(const orionobj_string_table_t* table);

// Serialize/deserialize for binary format
orionobj_result_t orionobj_string_table_write_binary(const orionobj_string_table_t* table,
                                                      FILE* file);

orionobj_result_t orionobj_string_table_read_binary(orionobj_string_table_t** table,
                                                     FILE* file,
                                                     uint32_t size,
                                                     const orionobj_allocator_t* allocator);

// String table validation
orionobj_result_t orionobj_string_table_validate(const orionobj_string_table_t* table);

#endif // ORIONOBJ_STRTAB_H