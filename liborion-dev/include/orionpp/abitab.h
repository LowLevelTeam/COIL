/**
* @file abitab.h  
* @brief Orion++ ABI Table - Platform calling convention management
* 
* Manages Application Binary Interface definitions for cross-platform
* function calling with detailed parameter passing rules and conventions.
*/

#ifndef ORIONPP_ABITAB_H
#define ORIONPP_ABITAB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <orionpp/detail.h>

// -------------------------------- Targeting -------------------------------- //

// taret is defined as so. 
// architecture-os-environment

enum orionpp_target_arch {
  ORIONPP_ARCH_x86, // x86-16
  ORIONPP_ARCH_i386, // x86-32
  ORIONPP_ARCH_amd64, // x86-64
  // more will come in the futue. as of version 1.0.0 only the x86 family is supported
};
typedef uint16_t orionpp_arch_t;

enum orionpp_target_os {
  ORIONPP_OS_unknown,
  ORIONPP_OS_none,
  ORIONPP_OS_linux,
  // more will come in the future. as of version 1.0.0 only linux is suppported
};
typedef uint8_t orionpp_os_t;

enum orionpp_target_env {
  ORIONPP_ENV_unknown,
};
typedef uint8_t orionpp_env_t;

typedef struct orionpp_target {
  orionpp_arch_t arch;
  orionpp_os_t os;
  orionpp_env_t env; 
  // env may be deprecated in the future in place of something else
  // as its use cases are minimal in practicality
} orionpp_target_t;

// -------------------------------- Register -------------------------------- //

enum orionpp_register_x86 {
  STACK = 0, // this is not the stack pointer but used in cases where either stack or register is used like return address. SP would be the stack pointer for x86 and defined below
  // registers for the x86 family (used for targets x86,i386,amd64)
  AL,
  AH,
  AX,
  // TODO... fill these in
};

typedef uint16_t orionpp_register_t;

// -------------------------------- Core Types -------------------------------- //

/**
* @brief ABI table entry defining calling convention
* 
* Each ABI can support multiple target platforms with different
* parameter passing rules and register usage conventions.
*/
typedef struct orionpp_abi_entry {
  orionpp_offset_t name;            // ABI name in string table (optional, only needed if externalized)
  orionpp_abi_t abi_id;             // Unique ABI identifier (used internally by instructions)
  uint32_t target_count;            // Number of target variants
  struct orionpp_abi_target targets[]; // Variable-length target array (used by the orion++ compiler to resolve the proper call convention)
} orionpp_abi_entry_t;

/**
 * @brief Target-specific ABI variant
 */
typedef struct orionpp_abi_target {
  // TODO: More information is needed for the full calling convention
  // in the future even situations like having an optional reference to some constant compile time function that could name mangle
  // to allow for easier integrations with different languages. (not needed for right now but maybe a placeholder for future development)
  // beyond the name mangling more items make up a complete ABI calling convention and this will need to define them all
  orionpp_target_t target;          // Target identifier
  orionpp_register_t return_address; // register it is stored in (lr on arm) to ensure its not overwritten in compiler optimization of the function. 0 / STACK, for return address stored on stack
  uint32_t stack_alignment;         // Stack alignment requirement
  uint32_t redzone;                 // stack redzone in bytes
  uint16_t max_arguments; // 0 for unspecified.
  uint16_t max_returns; // 0 for unspecified
  uint16_t argreg_count;            // the amount of registers for arguments
  uint16_t retreg_count;            // the amount of registers for returning
  orionpp_register_t argument_registers[];
  orionpp_register_t ret_registers[];
} orionpp_abi_target_t;

/**
 * @brief Complete ABI table for lookups
 */
typedef struct orionpp_abitab {
  uint64_t entry_count;             // Number of ABI entries
  orionpp_abi_entry_t *entries;     // Array of ABI entries
} orionpp_abitab_t;


// -------------------------------- Standard ABIs -------------------------------- //

// These ABIs can be externally referenced if using the compile time standard library.
#define ORIONPP_ABI_SYSV     1    // System V ABI (covers amd64, i386, arm32 and arm64)
#define ORIONPP_ABI_CABI     2    // ABI used in libc

// -------------------------------- API Functions -------------------------------- //

/**
 * @brief Initialize ABI table structure
 * @param table ABI table to initialize
 * @return Error code
 */
orionpp_error_t orionpp_abitab_init(orionpp_abitab_t *table);

/**
 * @brief Free ABI table resources
 * @param table ABI table to free
 */
void orionpp_abitab_free(orionpp_abitab_t *table);

/**
 * @brief Add ABI entry to table
 * @param table ABI table
 * @param entry ABI entry to add
 * @return Error code
 */
orionpp_error_t orionpp_abitab_add_entry(orionpp_abitab_t *table, const orionpp_abi_entry_t *entry);

/**
 * @brief Look up ABI entry by ID
 * @param table ABI table
 * @param abi_id ABI identifier to find
 * @return Pointer to entry or NULL if not found
 */
const orionpp_abi_entry_t *orionpp_abitab_lookup(const orionpp_abitab_t *table, orionpp_abi_t abi_id);

/**
 * @brief Find ABI target for specific platform
 * @param entry ABI entry to search
 * @param target Target platform specification
 * @return Pointer to target or NULL if not found
 */
const orionpp_abi_target_t *orionpp_abi_find_target(const orionpp_abi_entry_t *entry, const orionpp_target_t *target);

#endif // ORIONPP_ABITAB_H