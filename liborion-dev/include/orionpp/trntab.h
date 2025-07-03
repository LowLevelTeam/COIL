/**
* @file trntab.h
* @brief Orion++ External Imports and Internal Exports Definition Table
*/

#ifndef ORIONPP_TRNTAB_H
#define ORIONPP_TRNTAB_H

#include <stdint.h>
#include <stddef.h>
#include <orionpp/error.h>
#include <orionpp/detail.h>

// -------------------------------- Reference Types -------------------------------- //

enum orionpp_reference_type {
  ORIONPP_REFTYPE_TYPE = 0x00,        // type definition
  ORIONPP_REFTYPE_ABI = 0x01,         // ABI definition
  ORIONPP_REFTYPE_FUNCTION = 0x02,    // function
  ORIONPP_REFTYPE_VARIABLE = 0x03,    // object
  ORIONPP_REFTYPE_CONSTANT = 0x04,    // compile time constant
};

typedef uint8_t orionpp_reftype_t;

// -------------------------------- External References -------------------------------- //

/**
 * @brief External reference entry for imports
 */
typedef struct orionpp_extern_entry {
  orionpp_offset_t name_offset;      // Required name in string table
  orionpp_reftype_t identifier_type; // Type of external reference
  orionpp_reference_t identifier;    // local Type/ABI/target identifier to assign this to
  uint32_t info_size;                // Size of additional information
  uint8_t info[];                    // Type-specific information
} orionpp_extern_entry_t;

/**
 * @brief External reference table
 */
typedef struct orionpp_extrntab {
  uint32_t entry_count;              // Number of external entries
  orionpp_extern_entry_t **entries;  // Array of pointers to entries
  orionpp_offset_t data_size;        // Total size of entry data
  uint8_t *data;                     // Raw entry data storage
} orionpp_extrntab_t;

// -------------------------------- Internal References -------------------------------- //

/**
 * @brief Internal reference entry for exports  
 */
typedef struct orionpp_intern_entry {
  orionpp_offset_t name_offset;      // Required name in string table
  orionpp_reftype_t identifier_type; // Type of internal reference
  orionpp_reference_t identifier;    // local Type/ABI/target identifier to externalize
  uint32_t info_size;                // Size of additional information
  uint8_t info[];                    // Type-specific information
} orionpp_intern_entry_t;

/**
 * @brief Internal reference table
 */
typedef struct orionpp_intrntab {
  uint32_t entry_count;              // Number of internal entries
  orionpp_intern_entry_t **entries;  // Array of pointers to entries
  orionpp_offset_t data_size;        // Total size of entry data
  uint8_t *data;                     // Raw entry data storage
} orionpp_intrntab_t;

// -------------------------------- Type-Specific Info Structures -------------------------------- //

/**
 * @brief Function reference information
 */
typedef struct orionpp_function_info {
  orionpp_reference_t abi_id;        // ABI to use for calling convention
  uint16_t param_count;              // Number of parameters
  uint16_t return_count;             // Number of return values
  orionpp_reference_t param_types[]; // Parameter types (variable length)
  // return types follow param_types in memory
} orionpp_function_info_t;

/**
 * @brief Variable reference information  
 */
typedef struct orionpp_variable_info {
  orionpp_reference_t type_id;       // Type of the variable
  uint32_t size;                     // Size in bytes
  uint32_t alignment;                // Required alignment
  bool is_mutable;                   // Whether variable can be modified
} orionpp_variable_info_t;

/**
 * @brief Constant reference information
 */
typedef struct orionpp_constant_info {
  orionpp_reference_t type_id;       // Type of the constant
  uint32_t value_size;               // Size of constant value in bytes
  uint8_t value[];                   // Constant value data
} orionpp_constant_info_t;

// -------------------------------- External Table API -------------------------------- //

/**
 * @brief Initialize external reference table
 * @param table External table to initialize
 * @return Error code
 */
orionpp_error_t orionpp_extrntab_init(orionpp_extrntab_t *table);

/**
 * @brief Free external reference table resources
 * @param table External table to free
 */
void orionpp_extrntab_free(orionpp_extrntab_t *table);

/**
 * @brief Add entry to external reference table
 * @param table External table
 * @param entry Entry to add
 * @return Error code
 */
orionpp_error_t orionpp_extrntab_add_entry(orionpp_extrntab_t *table, const orionpp_extern_entry_t *entry);

/**
 * @brief Look up external reference by name
 * @param table External table
 * @param name Name to search for
 * @return Pointer to entry or NULL if not found
 */
const orionpp_extern_entry_t *orionpp_extrntab_lookup(const orionpp_extrntab_t *table, const char *name);

/**
 * @brief Look up external reference by identifier
 * @param table External table
 * @param identifier Local identifier to search for
 * @return Pointer to entry or NULL if not found
 */
const orionpp_extern_entry_t *orionpp_extrntab_lookup_by_id(const orionpp_extrntab_t *table, orionpp_reference_t identifier);

// -------------------------------- Internal Table API -------------------------------- //

/**
 * @brief Initialize internal reference table
 * @param table Internal table to initialize
 * @return Error code
 */
orionpp_error_t orionpp_intrntab_init(orionpp_intrntab_t *table);

/**
 * @brief Free internal reference table resources
 * @param table Internal table to free
 */
void orionpp_intrntab_free(orionpp_intrntab_t *table);

/**
 * @brief Add entry to internal reference table
 * @param table Internal table
 * @param entry Entry to add
 * @return Error code
 */
orionpp_error_t orionpp_intrntab_add_entry(orionpp_intrntab_t *table, const orionpp_intern_entry_t *entry);

/**
 * @brief Look up internal reference by name
 * @param table Internal table
 * @param name Name to search for
 * @return Pointer to entry or NULL if not found
 */
const orionpp_intern_entry_t *orionpp_intrntab_lookup(const orionpp_intrntab_t *table, const char *name);

/**
 * @brief Look up internal reference by identifier
 * @param table Internal table
 * @param identifier Local identifier to search for
 * @return Pointer to entry or NULL if not found
 */
const orionpp_intern_entry_t *orionpp_intrntab_lookup_by_id(const orionpp_intrntab_t *table, orionpp_reference_t identifier);

// -------------------------------- Utility Functions -------------------------------- //

/**
 * @brief Get function info from entry
 * @param entry Entry to extract info from
 * @return Pointer to function info or NULL if wrong type
 */
const orionpp_function_info_t *orionpp_entry_get_function_info(const orionpp_extern_entry_t *entry);

/**
 * @brief Get variable info from entry
 * @param entry Entry to extract info from
 * @return Pointer to variable info or NULL if wrong type
 */
const orionpp_variable_info_t *orionpp_entry_get_variable_info(const orionpp_extern_entry_t *entry);

/**
 * @brief Get constant info from entry
 * @param entry Entry to extract info from
 * @return Pointer to constant info or NULL if wrong type
 */
const orionpp_constant_info_t *orionpp_entry_get_constant_info(const orionpp_extern_entry_t *entry);

/**
 * @brief Validate external reference table integrity
 * @param table External table to validate
 * @return Error code
 */
orionpp_error_t orionpp_extrntab_validate(const orionpp_extrntab_t *table);

/**
 * @brief Validate internal reference table integrity
 * @param table Internal table to validate
 * @return Error code
 */
orionpp_error_t orionpp_intrntab_validate(const orionpp_intrntab_t *table);

#endif // ORIONPP_TRNTAB_H