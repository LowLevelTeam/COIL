#ifndef ORIONPP_OUTPUT_ORIONOBJ_H
#define ORIONPP_OUTPUT_ORIONOBJ_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/core/allocator.h"
#include "orionpp/module/module.h"
#include <stdio.h>

// Forward declarations for OrionObj types
typedef struct orionobj_object orionobj_object_t;
typedef struct orionobj_section orionobj_section_t;
typedef struct orionobj_symbol orionobj_symbol_t;
typedef struct orionobj_string_table orionobj_string_table_t;

// OrionObj compilation options
typedef struct {
  orionpp_format_t output_format;   // ORIONPP_FORMAT_ORIONPP or ORIONPP_FORMAT_ORION
  orionpp_arch_t target_arch;       // Target architecture
  bool optimize_strings;            // Whether to optimize string table
  bool generate_debug_info;         // Whether to include debug information
  const char* entry_point;          // Entry point symbol name (optional)
} orionpp_orionobj_options_t;

// Default OrionObj options
extern const orionpp_orionobj_options_t orionpp_default_orionobj_options;

// Compile OrionPP module to OrionObj object
orionpp_result_t orionpp_module_compile_to_orionobj(const orionpp_module_t* module,
                                                     orionobj_object_t** object,
                                                     const orionpp_orionobj_options_t* options,
                                                     const orionpp_allocator_t* allocator);

// Write OrionPP module as OrionObj to file
orionpp_result_t orionpp_module_write_orionobj(const orionpp_module_t* module,
                                                const char* filename,
                                                const orionpp_orionobj_options_t* options);

// Write OrionPP module as OrionObj to stream
orionpp_result_t orionpp_module_write_orionobj_stream(const orionpp_module_t* module,
                                                       FILE* file,
                                                       const orionpp_orionobj_options_t* options);

// Conversion utilities
orionpp_result_t orionpp_section_type_to_orionobj(orionpp_section_type_t pp_type,
                                                   uint32_t* obj_type);

orionpp_result_t orionpp_section_flags_to_orionobj(orionpp_section_flags_t pp_flags,
                                                    uint32_t* obj_flags);

orionpp_result_t orionpp_symbol_binding_to_orionobj(orionpp_symbol_binding_t pp_binding,
                                                     uint32_t* obj_binding);

orionpp_result_t orionpp_symbol_type_to_orionobj(orionpp_symbol_type_t pp_type,
                                                  uint32_t* obj_type);

// Code generation utilities
orionpp_result_t orionpp_generate_code_from_isa(const orionpp_module_t* module,
                                                 orionobj_section_t* text_section,
                                                 const orionpp_orionobj_options_t* options);

orionpp_result_t orionpp_resolve_symbols_and_labels(const orionpp_module_t* module,
                                                     orionobj_object_t* object);

#endif // ORIONPP_OUTPUT_ORIONOBJ_H