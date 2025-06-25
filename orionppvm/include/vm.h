/**
 * @file include/vm.h
 * @brief Orion++ Virtual Machine definitions
 */

#ifndef VM_H
#define VM_H

#include "orion/orionpp.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Maximum limits for safety
#define OVM_MAX_VARIABLES 10000
#define OVM_MAX_LABELS 1000
#define OVM_MAX_STACK_SIZE 10000
#define OVM_MAX_CALL_DEPTH 1000
#define OVM_MAX_MEMORY_SIZE (1024 * 1024 * 16) // 16MB

// Variable storage
typedef struct {
  orionpp_variable_id_t id;
  orionpp_type_t type;
  orionpp_type_module_t subtype;
  union {
    int64_t i64;
    uint64_t u64;
    double f64;
    char* str;
    void* ptr;
  } value;
  bool is_initialized;
} VMVariable;

// Label mapping
typedef struct {
  orionpp_label_id_t id;
  size_t instruction_index;
} VMLabel;

// Call stack frame
typedef struct {
  size_t return_address;
  size_t variable_base;
  char* function_name;
} VMFrame;

// Virtual Machine state
typedef struct {
  // Program storage
  orinopp_instruction_t* instructions;
  size_t instruction_count;
  size_t instruction_capacity;
  
  // Execution state
  size_t pc; // program counter
  bool running;
  bool error;
  char error_message[256];
  
  // Variable storage
  VMVariable* variables;
  size_t variable_count;
  
  // Label mapping
  VMLabel* labels;
  size_t label_count;
  
  // Call stack
  VMFrame* call_stack;
  size_t call_depth;
  
  // Return value
  VMVariable return_value;
  
  // Memory management
  size_t memory_used;
  
  // Runtime options
  bool debug_mode;
  bool strict_mode;
  FILE* debug_output;
} OrionVM;

// VM lifecycle
int ovm_init(OrionVM* vm);
void ovm_destroy(OrionVM* vm);
int ovm_load_file(OrionVM* vm, const char* filename);
int ovm_load_from_handle(OrionVM* vm, file_handle_t handle);
void ovm_reset(OrionVM* vm);

// Execution
int ovm_run(OrionVM* vm);
int ovm_step(OrionVM* vm);
void ovm_set_debug_mode(OrionVM* vm, bool debug, FILE* output);
void ovm_set_strict_mode(OrionVM* vm, bool strict);

// Variable management
VMVariable* ovm_get_variable(OrionVM* vm, orionpp_variable_id_t id);
VMVariable* ovm_create_variable(OrionVM* vm, orionpp_variable_id_t id, orionpp_type_t type);
int ovm_set_variable_value(OrionVM* vm, orionpp_variable_id_t id, const void* data, size_t size);

// Label management
int ovm_register_label(OrionVM* vm, orionpp_label_id_t id, size_t instruction_index);
size_t ovm_find_label(OrionVM* vm, orionpp_label_id_t id);

// Error handling
void ovm_error(OrionVM* vm, const char* format, ...);
const char* ovm_get_error(OrionVM* vm);
bool ovm_has_error(OrionVM* vm);

// Utility functions
const char* ovm_opcode_to_string(orionpp_opcode_t opcode, orionpp_opcode_module_t child);
const char* ovm_type_to_string(orionpp_type_t type);
void ovm_print_instruction(OrionVM* vm, const orinopp_instruction_t* instr);
void ovm_print_state(OrionVM* vm);

#endif // VM_H