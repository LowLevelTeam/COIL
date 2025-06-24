#ifndef ORIONPP_MODULE_MODULE_H
#define ORIONPP_MODULE_MODULE_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/string/strtab.h"
#include "orionpp/instr/instr.h"

// Module structure
struct orionpp_module {
  orionpp_features_t features;      // Enabled features bitmask
  orionpp_string_table_t* strings;  // String table for all strings
  
  // Instructions
  orionpp_instruction_t** instructions;
  uint32_t instruction_count;
  uint32_t instruction_capacity;
  
  // Memory management
  orionpp_allocator_t allocator;
  
  // Version information
  uint16_t version_major;
  uint16_t version_minor;
  uint32_t version_patch;
};

// Module lifecycle
orionpp_result_t orionpp_module_create(orionpp_module_t** module,
                                        orionpp_features_t features,
                                        const orionpp_allocator_t* allocator);

void orionpp_module_destroy(orionpp_module_t* module);

// Feature management
bool orionpp_module_has_feature(const orionpp_module_t* module, orionpp_feature_t feature);
orionpp_result_t orionpp_module_enable_feature(orionpp_module_t* module, orionpp_feature_t feature);
orionpp_result_t orionpp_module_disable_feature(orionpp_module_t* module, orionpp_feature_t feature);

// Instruction management
orionpp_result_t orionpp_module_add_instruction(orionpp_module_t* module,
                                                 orionpp_instruction_t* instruction);

orionpp_result_t orionpp_module_insert_instruction(orionpp_module_t* module,
                                                    uint32_t index,
                                                    orionpp_instruction_t* instruction);

orionpp_result_t orionpp_module_remove_instruction(orionpp_module_t* module, uint32_t index);

orionpp_instruction_t* orionpp_module_get_instruction(const orionpp_module_t* module, uint32_t index);
uint32_t orionpp_module_get_instruction_count(const orionpp_module_t* module);

// String table access
orionpp_string_table_t* orionpp_module_get_string_table(orionpp_module_t* module);

// Module validation
orionpp_result_t orionpp_module_validate(const orionpp_module_t* module);

// Module statistics
typedef struct {
  uint32_t total_instructions;
  uint32_t obj_instructions;
  uint32_t isa_instructions;
  uint32_t abi_instructions;
  uint32_t hint_instructions;
  uint32_t string_table_size;
  uint32_t estimated_binary_size;
} orionpp_module_stats_t;

orionpp_result_t orionpp_module_get_stats(const orionpp_module_t* module,
                                           orionpp_module_stats_t* stats);

#endif // ORIONPP_MODULE_MODULE_H