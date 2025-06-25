/**
 * @file src/vm.c
 * @brief Enhanced Orion++ Virtual Machine implementation with conditional branch support
 */

#include "vm.h"
#include "executor.h"
#include "validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef WIN32
  #include <windows.h>
  #include <io.h>
#else
  #include <unistd.h>
  #include <fcntl.h>
#endif

int ovm_init(OrionVM* vm) {
  if (!vm) return -1;
  
  memset(vm, 0, sizeof(OrionVM));
  
  // Initialize instruction storage
  vm->instruction_capacity = 1000;
  vm->instructions = malloc(vm->instruction_capacity * sizeof(orinopp_instruction_t));
  if (!vm->instructions) {
    return -1;
  }
  
  // Initialize variable storage
  vm->variables = malloc(OVM_MAX_VARIABLES * sizeof(VMVariable));
  if (!vm->variables) {
    free(vm->instructions);
    return -1;
  }
  
  // Initialize label storage
  vm->labels = malloc(OVM_MAX_LABELS * sizeof(VMLabel));
  if (!vm->labels) {
    free(vm->instructions);
    free(vm->variables);
    return -1;
  }
  
  // Initialize call stack
  vm->call_stack = malloc(OVM_MAX_CALL_DEPTH * sizeof(VMFrame));
  if (!vm->call_stack) {
    free(vm->instructions);
    free(vm->variables);
    free(vm->labels);
    return -1;
  }
  
  // Initialize state
  vm->pc = 0;
  vm->running = false;
  vm->error = false;
  vm->instruction_count = 0;
  vm->variable_count = 0;
  vm->label_count = 0;
  vm->call_depth = 0;
  vm->memory_used = sizeof(OrionVM);
  vm->debug_mode = false;
  vm->strict_mode = false;
  vm->debug_output = NULL;
  
  // Initialize return value
  memset(&vm->return_value, 0, sizeof(VMVariable));
  
  return 0;
}

void ovm_destroy(OrionVM* vm) {
  if (!vm) return;
  
  // Free instructions
  if (vm->instructions) {
    for (size_t i = 0; i < vm->instruction_count; i++) {
      if (vm->instructions[i].values) {
        for (size_t j = 0; j < vm->instructions[i].value_count; j++) {
          // Note: We don't free value bytes as they might be allocated by orionpp library
        }
        free(vm->instructions[i].values);
      }
    }
    free(vm->instructions);
  }
  
  // Free variables
  if (vm->variables) {
    for (size_t i = 0; i < vm->variable_count; i++) {
      if (vm->variables[i].type == ORIONPP_TYPE_STRING && vm->variables[i].value.str) {
        free(vm->variables[i].value.str);
      }
    }
    free(vm->variables);
  }
  
  // Free labels
  if (vm->labels) {
    free(vm->labels);
  }
  
  // Free call stack
  if (vm->call_stack) {
    for (size_t i = 0; i < vm->call_depth; i++) {
      if (vm->call_stack[i].function_name) {
        free(vm->call_stack[i].function_name);
      }
    }
    free(vm->call_stack);
  }
  
  // Free return value string if needed
  if (vm->return_value.type == ORIONPP_TYPE_STRING && vm->return_value.value.str) {
    free(vm->return_value.value.str);
  }
  
  memset(vm, 0, sizeof(OrionVM));
}

int ovm_load_file(OrionVM* vm, const char* filename) {
  if (!vm || !filename) return -1;
  
#ifdef WIN32
  HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file == INVALID_HANDLE_VALUE) {
    ovm_error(vm, "Cannot open file: %s", filename);
    return -1;
  }
#else
  int file = open(filename, O_RDONLY);
  if (file == -1) {
    ovm_error(vm, "Cannot open file: %s", filename);
    return -1;
  }
#endif
  
  int result = ovm_load_from_handle(vm, file);
  
#ifdef WIN32
  CloseHandle(file);
#else
  close(file);
#endif
  
  return result;
}

int ovm_load_from_handle(OrionVM* vm, file_handle_t handle) {
  if (!vm) return -1;
  
  // Reset VM state
  ovm_reset(vm);
  
  // Read instructions one by one
  orinopp_instruction_t instr;
  while (true) {
    memset(&instr, 0, sizeof(instr));
    orionpp_readf(handle, &instr);
    
    // Check for end of file or error
    if (instr.root == 0 && instr.child == 0 && instr.value_count == 0) {
      break;
    }
    
    // Check if we need to resize instruction array
    if (vm->instruction_count >= vm->instruction_capacity) {
      vm->instruction_capacity *= 2;
      orinopp_instruction_t* new_instructions = realloc(vm->instructions, 
        vm->instruction_capacity * sizeof(orinopp_instruction_t));
      if (!new_instructions) {
        ovm_error(vm, "Out of memory expanding instruction array");
        return -1;
      }
      vm->instructions = new_instructions;
    }
    
    // Store instruction
    vm->instructions[vm->instruction_count] = instr;
    vm->instruction_count++;
    
    // Update memory usage
    vm->memory_used += sizeof(orinopp_instruction_t);
    if (instr.values) {
      vm->memory_used += instr.value_count * sizeof(orinopp_value_t);
      for (size_t i = 0; i < instr.value_count; i++) {
        vm->memory_used += instr.values[i].bytesize;
      }
    }
    
    // Check memory limit
    if (vm->memory_used > OVM_MAX_MEMORY_SIZE) {
      ovm_error(vm, "Memory limit exceeded while loading program");
      return -1;
    }
  }
  
  if (vm->debug_mode && vm->debug_output) {
    fprintf(vm->debug_output, "Loaded %zu instructions\n", vm->instruction_count);
  }
  
  return 0;
}

void ovm_reset(OrionVM* vm) {
  if (!vm) return;
  
  // Reset execution state
  vm->pc = 0;
  vm->running = false;
  vm->error = false;
  vm->error_message[0] = '\0';
  
  // Clear variables
  for (size_t i = 0; i < vm->variable_count; i++) {
    if (vm->variables[i].type == ORIONPP_TYPE_STRING && vm->variables[i].value.str) {
      free(vm->variables[i].value.str);
    }
  }
  vm->variable_count = 0;
  
  // Clear labels
  vm->label_count = 0;
  
  // Clear call stack
  for (size_t i = 0; i < vm->call_depth; i++) {
    if (vm->call_stack[i].function_name) {
      free(vm->call_stack[i].function_name);
    }
  }
  vm->call_depth = 0;
  
  // Reset return value
  if (vm->return_value.type == ORIONPP_TYPE_STRING && vm->return_value.value.str) {
    free(vm->return_value.value.str);
  }
  memset(&vm->return_value, 0, sizeof(VMVariable));
  
  // Reset memory usage (but keep allocated structures)
  vm->memory_used = sizeof(OrionVM);
}

int ovm_run(OrionVM* vm) {
  if (!vm) return -1;
  
  vm->running = true;
  vm->pc = 0;
  
  // First pass: register all labels
  for (size_t i = 0; i < vm->instruction_count; i++) {
    const orinopp_instruction_t* instr = &vm->instructions[i];
    if (instr->root == ORIONPP_OP_ISA && instr->child == ORIONPP_OP_ISA_LABEL) {
      if (instr->value_count > 0 && instr->values[0].root == ORIONPP_TYPE_LABELID) {
        orionpp_label_id_t label_id;
        if (ovm_extract_label_id(&instr->values[0], &label_id) == 0) {
          ovm_register_label(vm, label_id, i);
        }
      }
    }
  }
  
  // Main execution loop
  while (vm->running && !vm->error && vm->pc < vm->instruction_count) {
    if (ovm_step(vm) != 0) {
      return -1;
    }
  }
  
  if (vm->error) {
    return -1;
  }
  
  return 0;
}

int ovm_step(OrionVM* vm) {
  if (!vm || !vm->running || vm->error) return -1;
  
  if (vm->pc >= vm->instruction_count) {
    vm->running = false;
    return 0;
  }
  
  const orinopp_instruction_t* instr = &vm->instructions[vm->pc];
  
  if (vm->debug_mode && vm->debug_output) {
    fprintf(vm->debug_output, "PC=%zu: ", vm->pc);
    ovm_print_instruction(vm, instr);
  }
  
  // Validate instruction execution
  ValidationResult validation = ovm_validate_execution_safety(vm);
  if (validation != OVM_VALID) {
    ovm_error(vm, "Safety validation failed: %s", ovm_validation_result_to_string(validation));
    return -1;
  }
  
  // Execute instruction
  int result = ovm_execute_instruction(vm, instr);
  if (result != 0) {
    return -1;
  }
  
  // Don't automatically increment PC for control flow instructions
  if (instr->root == ORIONPP_OP_ISA) {
    switch (instr->child) {
      case ORIONPP_OP_ISA_JMP:
      case ORIONPP_OP_ISA_BREQ:
      case ORIONPP_OP_ISA_BRNEQ:
      case ORIONPP_OP_ISA_BRGT:
      case ORIONPP_OP_ISA_BRGE:
      case ORIONPP_OP_ISA_BRLT:
      case ORIONPP_OP_ISA_BRLE:
      case ORIONPP_OP_ISA_BRZ:
      case ORIONPP_OP_ISA_BRNZ:
      case ORIONPP_OP_ISA_CALL:
      case ORIONPP_OP_ISA_RET:
        // PC already updated by instruction
        break;
      default:
        vm->pc++;
        break;
    }
  } else {
    vm->pc++;
  }
  
  return 0;
}

void ovm_set_debug_mode(OrionVM* vm, bool debug, FILE* output) {
  if (!vm) return;
  vm->debug_mode = debug;
  vm->debug_output = output;
}

void ovm_set_strict_mode(OrionVM* vm, bool strict) {
  if (!vm) return;
  vm->strict_mode = strict;
}

VMVariable* ovm_get_variable(OrionVM* vm, orionpp_variable_id_t id) {
  if (!vm) return NULL;
  
  for (size_t i = 0; i < vm->variable_count; i++) {
    if (vm->variables[i].id == id) {
      return &vm->variables[i];
    }
  }
  
  return NULL;
}

VMVariable* ovm_create_variable(OrionVM* vm, orionpp_variable_id_t id, orionpp_type_t type) {
  if (!vm || vm->variable_count >= OVM_MAX_VARIABLES) {
    if (vm) ovm_error(vm, "Maximum variable count exceeded");
    return NULL;
  }
  
  VMVariable* var = &vm->variables[vm->variable_count++];
  memset(var, 0, sizeof(VMVariable));
  var->id = id;
  var->type = type;
  var->is_initialized = false;
  
  return var;
}

int ovm_set_variable_value(OrionVM* vm, orionpp_variable_id_t id, const void* data, size_t size) {
  if (!vm || !data) return -1;
  
  VMVariable* var = ovm_get_variable(vm, id);
  if (!var) {
    ovm_error(vm, "Variable %u not found", id);
    return -1;
  }
  
  // Set value based on type
  switch (var->type) {
    case ORIONPP_TYPE_WORD:
    case ORIONPP_TYPE_SIZE:
      if (size >= sizeof(int64_t)) {
        var->value.i64 = *(int64_t*)data;
      } else if (size >= sizeof(int32_t)) {
        var->value.i64 = *(int32_t*)data;
      } else {
        ovm_error(vm, "Invalid data size for integer variable");
        return -1;
      }
      break;
    case ORIONPP_TYPE_STRING:
      if (var->value.str) {
        free(var->value.str);
      }
      var->value.str = malloc(size + 1);
      if (!var->value.str) {
        ovm_error(vm, "Out of memory for string variable");
        return -1;
      }
      memcpy(var->value.str, data, size);
      var->value.str[size] = '\0';
      break;
    default:
      ovm_error(vm, "Unsupported variable type");
      return -1;
  }
  
  var->is_initialized = true;
  return 0;
}

int ovm_register_label(OrionVM* vm, orionpp_label_id_t id, size_t instruction_index) {
  if (!vm || vm->label_count >= OVM_MAX_LABELS) {
    if (vm) ovm_error(vm, "Maximum label count exceeded");
    return -1;
  }
  
  vm->labels[vm->label_count].id = id;
  vm->labels[vm->label_count].instruction_index = instruction_index;
  vm->label_count++;
  
  return 0;
}

size_t ovm_find_label(OrionVM* vm, orionpp_label_id_t id) {
  if (!vm) return SIZE_MAX;
  
  for (size_t i = 0; i < vm->label_count; i++) {
    if (vm->labels[i].id == id) {
      return vm->labels[i].instruction_index;
    }
  }
  
  return SIZE_MAX;
}

void ovm_error(OrionVM* vm, const char* format, ...) {
  if (!vm) return;
  
  va_list args;
  va_start(args, format);
  vsnprintf(vm->error_message, sizeof(vm->error_message), format, args);
  va_end(args);
  
  vm->error = true;
  vm->running = false;
}

const char* ovm_get_error(OrionVM* vm) {
  if (!vm) return "Invalid VM";
  return vm->error_message;
}

bool ovm_has_error(OrionVM* vm) {
  if (!vm) return true;
  return vm->error;
}

const char* ovm_opcode_to_string(orionpp_opcode_t opcode, orionpp_opcode_module_t child) {
  switch (opcode) {
    case ORIONPP_OP_ISA:
      switch (child) {
        case ORIONPP_OP_ISA_VAR: return "VAR";
        case ORIONPP_OP_ISA_CONST: return "CONST";
        case ORIONPP_OP_ISA_MOV: return "MOV";
        case ORIONPP_OP_ISA_LEA: return "LEA";
        case ORIONPP_OP_ISA_LABEL: return "LABEL";
        case ORIONPP_OP_ISA_JMP: return "JMP";
        case ORIONPP_OP_ISA_BREQ: return "BR.EQ";
        case ORIONPP_OP_ISA_BRNEQ: return "BR.NEQ";
        case ORIONPP_OP_ISA_BRGT: return "BR.GT";
        case ORIONPP_OP_ISA_BRGE: return "BR.GE";
        case ORIONPP_OP_ISA_BRLT: return "BR.LT";
        case ORIONPP_OP_ISA_BRLE: return "BR.LE";
        case ORIONPP_OP_ISA_BRZ: return "BR.Z";
        case ORIONPP_OP_ISA_BRNZ: return "BR.NZ";
        case ORIONPP_OP_ISA_CALL: return "CALL";
        case ORIONPP_OP_ISA_RET: return "RET";
        case ORIONPP_OP_ISA_ADD: return "ADD";
        case ORIONPP_OP_ISA_SUB: return "SUB";
        case ORIONPP_OP_ISA_MUL: return "MUL";
        case ORIONPP_OP_ISA_DIV: return "DIV";
        case ORIONPP_OP_ISA_MOD: return "MOD";
        case ORIONPP_OP_ISA_INC: return "INC";
        case ORIONPP_OP_ISA_DEC: return "DEC";
        case ORIONPP_OP_ISA_INCp: return "INC++";
        case ORIONPP_OP_ISA_DECp: return "DEC++";
        case ORIONPP_OP_ISA_AND: return "AND";
        case ORIONPP_OP_ISA_OR: return "OR";
        case ORIONPP_OP_ISA_XOR: return "XOR";
        case ORIONPP_OP_ISA_NOT: return "NOT";
        case ORIONPP_OP_ISA_SHL: return "SHL";
        case ORIONPP_OP_ISA_SHR: return "SHR";
        default: return "UNKNOWN_ISA";
      }
    case ORIONPP_OP_HINT: return "HINT";
    case ORIONPP_OP_TYPE: return "TYPE";
    case ORIONPP_OP_ABI: return "ABI";
    case ORIONPP_OP_OBJ: return "OBJ";
    default: return "UNKNOWN";
  }
}

const char* ovm_type_to_string(orionpp_type_t type) {
  switch (type) {
    case ORIONPP_TYPE_VARID: return "VARID";
    case ORIONPP_TYPE_LABELID: return "LABELID";
    case ORIONPP_TYPE_SYMBOL: return "SYMBOL";
    case ORIONPP_TYPE_STRING: return "STRING";
    case ORIONPP_TYPE_WORD: return "WORD";
    case ORIONPP_TYPE_SIZE: return "SIZE";
    case ORIONPP_TYPE_SSIZE: return "SSIZE";
    case ORIONPP_TYPE_C: return "C";
    default: return "UNKNOWN_TYPE";
  }
}

void ovm_print_instruction(OrionVM* vm, const orinopp_instruction_t* instr) {
  if (!vm || !instr || !vm->debug_output) return;
  
  fprintf(vm->debug_output, "%s", ovm_opcode_to_string(instr->root, instr->child));
  
  for (size_t i = 0; i < instr->value_count; i++) {
    fprintf(vm->debug_output, " %s:", ovm_type_to_string(instr->values[i].root));
    
    switch (instr->values[i].root) {
      case ORIONPP_TYPE_VARID:
      case ORIONPP_TYPE_LABELID:
        if (instr->values[i].bytesize >= sizeof(uint32_t)) {
          uint32_t id = *(uint32_t*)instr->values[i].bytes;
          fprintf(vm->debug_output, "%u", id);
        }
        break;
      case ORIONPP_TYPE_WORD:
      case ORIONPP_TYPE_SIZE:
        if (instr->values[i].bytesize >= sizeof(int32_t)) {
          int32_t val = *(int32_t*)instr->values[i].bytes;
          fprintf(vm->debug_output, "%d", val);
        }
        break;
      case ORIONPP_TYPE_STRING:
      case ORIONPP_TYPE_SYMBOL:
        fprintf(vm->debug_output, "\"%.*s\"", (int)instr->values[i].bytesize, instr->values[i].bytes);
        break;
      default:
        fprintf(vm->debug_output, "(%zu bytes)", instr->values[i].bytesize);
        break;
    }
  }
  
  fprintf(vm->debug_output, "\n");
}

void ovm_print_state(OrionVM* vm) {
  if (!vm || !vm->debug_output) return;
  
  fprintf(vm->debug_output, "=== VM State ===\n");
  fprintf(vm->debug_output, "PC: %zu/%zu\n", vm->pc, vm->instruction_count);
  fprintf(vm->debug_output, "Running: %s\n", vm->running ? "true" : "false");
  fprintf(vm->debug_output, "Error: %s\n", vm->error ? vm->error_message : "none");
  fprintf(vm->debug_output, "Variables: %zu\n", vm->variable_count);
  fprintf(vm->debug_output, "Labels: %zu\n", vm->label_count);
  fprintf(vm->debug_output, "Call depth: %zu\n", vm->call_depth);
  fprintf(vm->debug_output, "Memory used: %zu bytes\n", vm->memory_used);
  
  if (vm->variable_count > 0) {
    fprintf(vm->debug_output, "Variables:\n");
    for (size_t i = 0; i < vm->variable_count; i++) {
      VMVariable* var = &vm->variables[i];
      fprintf(vm->debug_output, "  %u: %s = ", var->id, ovm_type_to_string(var->type));
      if (var->is_initialized) {
        switch (var->type) {
          case ORIONPP_TYPE_WORD:
          case ORIONPP_TYPE_SIZE:
            fprintf(vm->debug_output, "%lld", (long long)var->value.i64);
            break;
          case ORIONPP_TYPE_STRING:
            fprintf(vm->debug_output, "\"%s\"", var->value.str ? var->value.str : "(null)");
            break;
          default:
            fprintf(vm->debug_output, "(unhandled type)");
            break;
        }
      } else {
        fprintf(vm->debug_output, "(uninitialized)");
      }
      fprintf(vm->debug_output, "\n");
    }
  }
  
  fprintf(vm->debug_output, "================\n");
}