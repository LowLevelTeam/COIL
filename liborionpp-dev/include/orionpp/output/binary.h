#ifndef ORIONPP_OUTPUT_BINARY_H
#define ORIONPP_OUTPUT_BINARY_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/module/module.h"
#include "orionpp/instr/instr.h"
#include "orionpp/value/value.h"
#include <stdio.h>

// Binary format header
typedef struct {
  uint32_t magic;               // ORIONPP_BINARY_MAGIC
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t version_patch;
  orionpp_features_t features;  // Enabled features bitmask
  uint32_t string_table_size;
  uint32_t instruction_count;
  uint32_t reserved[4];         // For future expansion
} orionpp_binary_header_t;

// Instruction in binary format
typedef struct {
  uint8_t feature;              // First opcode (feature module)
  uint8_t opcode;               // Second opcode (instruction within module)
  uint16_t flags;               // Instruction-specific flags
  uint32_t data_size;           // Size of following instruction data
  // Followed by instruction-specific data
} orionpp_binary_instruction_t;

// Module binary I/O operations
orionpp_result_t orionpp_module_write_binary(const orionpp_module_t* module,
                                              FILE* file);

orionpp_result_t orionpp_module_read_binary(orionpp_module_t** module,
                                             FILE* file,
                                             const orionpp_allocator_t* allocator);

// Header operations
orionpp_result_t orionpp_binary_write_header(const orionpp_module_t* module, FILE* file);
orionpp_result_t orionpp_binary_read_header(orionpp_binary_header_t* header, FILE* file);

// Module validation for binary format
orionpp_result_t orionpp_module_validate_binary_header(const orionpp_binary_header_t* header);

// Version compatibility checking
bool orionpp_binary_is_compatible(uint16_t major, uint16_t minor, uint32_t patch);

// Individual component I/O
orionpp_result_t orionpp_value_write_binary(const orionpp_value_t* value, FILE* file);
orionpp_result_t orionpp_value_read_binary(orionpp_value_t* value, 
                                           FILE* file,
                                           const orionpp_allocator_t* allocator);

orionpp_result_t orionpp_instruction_write_binary(const orionpp_instruction_t* instruction,
                                                   FILE* file);
orionpp_result_t orionpp_instruction_read_binary(orionpp_instruction_t** instruction,
                                                  FILE* file,
                                                  const orionpp_allocator_t* allocator);

#endif // ORIONPP_OUTPUT_BINARY_H