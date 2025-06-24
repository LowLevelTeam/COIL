#ifndef ORIONPP_BINARY_H
#define ORIONPP_BINARY_H

#include "orionpp/types.h"
#include "orionpp/module.h"

// Forward declarations for value I/O
orionpp_result_t orionpp_value_write_binary(const orionpp_value_t* value, FILE* file);
orionpp_result_t orionpp_value_read_binary(orionpp_value_t* value, 
                                           FILE* file,
                                           const orionpp_allocator_t* allocator);

// Module binary I/O operations
orionpp_result_t orionpp_module_write_binary(const orionpp_module_t* module,
                                              FILE* file);

orionpp_result_t orionpp_module_read_binary(orionpp_module_t** module,
                                             FILE* file,
                                             const orionpp_allocator_t* allocator);

// Module validation for binary format
orionpp_result_t orionpp_module_validate_binary_header(const orionpp_binary_header_t* header);

// Version compatibility checking
bool orionpp_binary_is_compatible(uint16_t major, uint16_t minor, uint32_t patch);

// Binary format utilities
orionpp_result_t orionpp_binary_write_header(const orionpp_module_t* module, FILE* file);
orionpp_result_t orionpp_binary_read_header(orionpp_binary_header_t* header, FILE* file);

#endif // ORIONPP_BINARY_H