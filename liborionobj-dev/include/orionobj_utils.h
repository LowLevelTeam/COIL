#ifndef ORIONOBJ_UTILS_H
#define ORIONOBJ_UTILS_H

#include "orionobj.h"

// Convenience functions for creating common object configurations

/**
 * Create an Orion++ object with standard sections (.orionpp.text, .orionpp.data, .orionpp.bss)
 */
OrionObject* orion_create_orionpp_object(void);

/**
 * Create an Orion variant object with standard sections based on variant type
 */
OrionObject* orion_create_orion_variant_object(OrionVariantType variant);

/**
 * Create a native object with standard sections (.text, .data, .bss) for target architecture
 */
OrionObject* orion_create_native_object(OrionTargetArch target);

// Symbol convenience functions

/**
 * Add a function symbol to the object
 */
int orion_add_function_symbol(OrionObject* obj, const char* name,
                              OrionSymbolBinding binding, uint16_t section_index,
                              uint64_t addr, uint64_t size);

/**
 * Add a data symbol to the object
 */
int orion_add_data_symbol(OrionObject* obj, const char* name,
                          OrionSymbolBinding binding, uint16_t section_index,
                          uint64_t addr, uint64_t size);

/**
 * Add a global function symbol
 */
int orion_add_global_function(OrionObject* obj, const char* name, uint16_t section_index,
                              uint64_t addr, uint64_t size);

/**
 * Add a local function symbol
 */
int orion_add_local_function(OrionObject* obj, const char* name, uint16_t section_index,
                             uint64_t addr, uint64_t size);

/**
 * Add a global data symbol
 */
int orion_add_global_data(OrionObject* obj, const char* name, uint16_t section_index,
                          uint64_t addr, uint64_t size);

/**
 * Add a local data symbol
 */
int orion_add_local_data(OrionObject* obj, const char* name, uint16_t section_index,
                         uint64_t addr, uint64_t size);

// Helper functions

/**
 * Get section index by name (returns 0 if not found)
 */
uint32_t orion_get_section_index(OrionObject* obj, const char* name);

/**
 * Print object information for debugging
 */
void orion_print_object_info(OrionObject* obj);

/**
 * Check if object contains Orion++ IR
 */
int orion_has_orionpp(OrionObject* obj);

/**
 * Check if object contains Orion variant IR
 */
int orion_has_orion_variant(OrionObject* obj);

/**
 * Check if object contains native machine code
 */
int orion_has_native(OrionObject* obj);

/**
 * Get human-readable target architecture name
 */
const char* orion_get_target_name(OrionTargetArch target);

/**
 * Get human-readable variant type name
 */
const char* orion_get_variant_name(OrionVariantType variant);

/**
 * Check if section type is a BSS section (uninitialized data)
 */
int orion_is_bss_section(OrionSectionType type);

#endif // ORIONOBJ_UTILS_H