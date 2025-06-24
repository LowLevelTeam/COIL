#ifndef ORIONPP_STRING_TABLE_H
#define ORIONPP_STRING_TABLE_H

#include "orionpp/types.h"

// String table for efficient string storage and deduplication
struct orionpp_string_table {
  char* data;
  uint32_t size;
  uint32_t capacity;
  orionpp_allocator_t allocator;
  
  // Hash table for deduplication (optional optimization)
  struct string_hash_entry* hash_table;
  uint32_t hash_table_size;
  uint32_t hash_count;
};

// Hash table entry for string deduplication
struct string_hash_entry {
  uint32_t offset;
  uint32_t length;
  uint32_t hash;
  struct string_hash_entry* next;
};

// String table API
orionpp_result_t orionpp_string_table_create(orionpp_string_table_t** table, 
                                              const orionpp_allocator_t* allocator);
void orionpp_string_table_destroy(orionpp_string_table_t* table);

// Add string and return offset (deduplicates automatically)
orionpp_result_t orionpp_string_table_add(orionpp_string_table_t* table, 
                                           const char* str, 
                                           uint32_t* offset);

// Add string with known length
orionpp_result_t orionpp_string_table_add_length(orionpp_string_table_t* table,
                                                  const char* str,
                                                  uint32_t length,
                                                  uint32_t* offset);

// Get string by offset
const char* orionpp_string_table_get(const orionpp_string_table_t* table, 
                                      uint32_t offset);

// Get string length by offset
uint32_t orionpp_string_table_get_length(const orionpp_string_table_t* table,
                                          uint32_t offset);

// Validate offset
bool orionpp_string_table_is_valid_offset(const orionpp_string_table_t* table,
                                           uint32_t offset);

// Serialize/deserialize for binary format
orionpp_result_t orionpp_string_table_write_binary(orionpp_string_table_t* table,
                                                    FILE* file);
orionpp_result_t orionpp_string_table_read_binary(orionpp_string_table_t** table,
                                                   FILE* file,
                                                   uint32_t size,
                                                   const orionpp_allocator_t* allocator);

#endif // ORIONPP_STRING_TABLE_H