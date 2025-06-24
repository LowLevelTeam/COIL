#include "orionpp/module.h"
#include <string.h>
#include <stdlib.h>

#define INITIAL_INSTRUCTION_CAPACITY 64

orionpp_result_t orionpp_module_create(orionpp_module_t** module,
                                        orionpp_features_t features,
                                        const orionpp_allocator_t* allocator) {
  if (!module) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  orionpp_module_t* mod = allocator->malloc(sizeof(orionpp_module_t));
  if (!mod) return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  
  memset(mod, 0, sizeof(orionpp_module_t));
  mod->allocator = *allocator;
  mod->features = features;
  mod->version_major = ORIONPP_VERSION_MAJOR;
  mod->version_minor = ORIONPP_VERSION_MINOR;
  mod->version_patch = ORIONPP_VERSION_PATCH;
  
  // Create string table
  orionpp_result_t strings_result = orionpp_string_table_create(&mod->strings, allocator);
  if (ORIONPP_IS_ERROR(strings_result)) {
    allocator->free(mod);
    return strings_result;
  }
  
  // Initialize instruction array
  mod->instruction_capacity = INITIAL_INSTRUCTION_CAPACITY;
  mod->instructions = allocator->malloc(sizeof(orionpp_instruction_t*) * mod->instruction_capacity);
  if (!mod->instructions) {
    orionpp_string_table_destroy(mod->strings);
    allocator->free(mod);
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  memset(mod->instructions, 0, sizeof(orionpp_instruction_t*) * mod->instruction_capacity);
  
  *module = mod;
  return ORIONPP_OK_PTR(mod);
}

void orionpp_module_destroy(orionpp_module_t* module) {
  if (!module) return;
  
  // Destroy all instructions
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    if (module->instructions[i]) {
      orionpp_instruction_destroy(module->instructions[i], &module->allocator);
    }
  }
  
  // Free instruction array
  module->allocator.free(module->instructions);
  
  // Destroy string table
  orionpp_string_table_destroy(module->strings);
  
  // Free module
  module->allocator.free(module);
}

bool orionpp_module_has_feature(const orionpp_module_t* module, orionpp_feature_t feature) {
  if (!module) return false;
  return (module->features & (1U << feature)) != 0;
}

orionpp_result_t orionpp_module_enable_feature(orionpp_module_t* module, orionpp_feature_t feature) {
  if (!module) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (feature >= 32) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FEATURE);
  
  module->features |= (1U << feature);
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_disable_feature(orionpp_module_t* module, orionpp_feature_t feature) {
  if (!module) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (feature >= 32) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FEATURE);
  
  module->features &= ~(1U << feature);
  return ORIONPP_OK_INT(0);
}

static orionpp_result_t ensure_instruction_capacity(orionpp_module_t* module, uint32_t needed_capacity) {
  if (needed_capacity <= module->instruction_capacity) {
    return ORIONPP_OK_INT(0);
  }
  
  uint32_t new_capacity = module->instruction_capacity;
  while (new_capacity < needed_capacity) {
    new_capacity *= 2;
  }
  
  orionpp_instruction_t** new_instructions = module->allocator.realloc(
    module->instructions,
    sizeof(orionpp_instruction_t*) * new_capacity
  );
  
  if (!new_instructions) {
    return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
  }
  
  // Initialize new slots to NULL
  for (uint32_t i = module->instruction_capacity; i < new_capacity; i++) {
    new_instructions[i] = NULL;
  }
  
  module->instructions = new_instructions;
  module->instruction_capacity = new_capacity;
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_add_instruction(orionpp_module_t* module,
                                                 orionpp_feature_t feature,
                                                 uint8_t opcode,
                                                 orionpp_instruction_t** instruction) {
  if (!module || !instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Check if feature is enabled
  if (!orionpp_module_has_feature(module, feature)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FEATURE);
  }
  
  // Ensure capacity
  orionpp_result_t capacity_result = ensure_instruction_capacity(module, module->instruction_count + 1);
  if (ORIONPP_IS_ERROR(capacity_result)) return capacity_result;
  
  // Create instruction
  orionpp_instruction_t* inst;
  orionpp_result_t create_result = orionpp_instruction_create(&inst, feature, opcode, &module->allocator);
  if (ORIONPP_IS_ERROR(create_result)) return create_result;
  
  // Add to module
  module->instructions[module->instruction_count] = inst;
  module->instruction_count++;
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_module_insert_instruction(orionpp_module_t* module,
                                                    uint32_t index,
                                                    orionpp_feature_t feature,
                                                    uint8_t opcode,
                                                    orionpp_instruction_t** instruction) {
  if (!module || !instruction) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (index > module->instruction_count) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  // Check if feature is enabled  
  if (!orionpp_module_has_feature(module, feature)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FEATURE);
  }
  
  // Ensure capacity
  orionpp_result_t capacity_result = ensure_instruction_capacity(module, module->instruction_count + 1);
  if (ORIONPP_IS_ERROR(capacity_result)) return capacity_result;
  
  // Create instruction
  orionpp_instruction_t* inst;
  orionpp_result_t create_result = orionpp_instruction_create(&inst, feature, opcode, &module->allocator);
  if (ORIONPP_IS_ERROR(create_result)) return create_result;
  
  // Shift existing instructions
  for (uint32_t i = module->instruction_count; i > index; i--) {
    module->instructions[i] = module->instructions[i - 1];
  }
  
  // Insert new instruction
  module->instructions[index] = inst;
  module->instruction_count++;
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_module_remove_instruction(orionpp_module_t* module, uint32_t index) {
  if (!module) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (index >= module->instruction_count) return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  
  // Destroy the instruction
  orionpp_instruction_destroy(module->instructions[index], &module->allocator);
  
  // Shift remaining instructions
  for (uint32_t i = index; i < module->instruction_count - 1; i++) {
    module->instructions[i] = module->instructions[i + 1];
  }
  
  module->instruction_count--;
  module->instructions[module->instruction_count] = NULL;
  
  return ORIONPP_OK_INT(0);
}

orionpp_instruction_t* orionpp_module_get_instruction(const orionpp_module_t* module, uint32_t index) {
  if (!module || index >= module->instruction_count) return NULL;
  return module->instructions[index];
}

uint32_t orionpp_module_get_instruction_count(const orionpp_module_t* module) {
  if (!module) return 0;
  return module->instruction_count;
}

orionpp_string_table_t* orionpp_module_get_string_table(orionpp_module_t* module) {
  if (!module) return NULL;
  return module->strings;
}

orionpp_result_t orionpp_module_validate(const orionpp_module_t* module) {
  if (!module) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Validate string table
  if (!module->strings) {
    return ORIONPP_ERROR(ORIONPP_ERROR_CORRUPT_DATA);
  }
  
  // Validate all instructions
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (!inst) {
      return ORIONPP_ERROR(ORIONPP_ERROR_CORRUPT_DATA);
    }
    
    if (!orionpp_instruction_is_valid(inst)) {
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
    }
    
    // Check if instruction's feature is enabled
    if (!orionpp_module_has_feature(module, inst->feature)) {
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_FEATURE);
    }
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_get_stats(const orionpp_module_t* module,
                                           orionpp_module_stats_t* stats) {
  if (!module || !stats) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  memset(stats, 0, sizeof(orionpp_module_stats_t));
  
  stats->total_instructions = module->instruction_count;
  stats->string_table_size = module->strings->size;
  
  // Count instructions by feature
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (!inst) continue;
    
    switch (inst->feature) {
      case ORIONPP_FEATURE_OBJ:
        stats->obj_instructions++;
        break;
      case ORIONPP_FEATURE_ISA:
        stats->isa_instructions++;
        break;
      case ORIONPP_FEATURE_ABI:
        stats->abi_instructions++;
        break;
      case ORIONPP_FEATURE_HINT:
        stats->hint_instructions++;
        break;
    }
  }
  
  // Estimate binary size (rough calculation)
  stats->estimated_binary_size = sizeof(orionpp_binary_header_t) +
                                  stats->string_table_size +
                                  (stats->total_instructions * sizeof(orionpp_binary_instruction_t)) +
                                  (stats->total_instructions * 32); // Average instruction data size
  
  return ORIONPP_OK_INT(0);
}