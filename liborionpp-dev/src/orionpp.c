#include "../include/orionpp.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Context implementation
struct OrionPPContext {
  // String table
  char* string_table;
  size_t string_table_size;
  size_t string_table_capacity;
  
  // Variables
  OrionPPVariable* variables;
  size_t variable_count;
  size_t variable_capacity;
  uint32_t next_var_id;
  
  // Symbols
  OrionPPSymbol* symbols;
  size_t symbol_count;
  size_t symbol_capacity;
  
  // Functions
  OrionPPFunction* functions;
  size_t function_count;
  size_t function_capacity;
  
  // Current scope tracking
  uint32_t current_scope_id;
  uint32_t next_scope_id;
  
  // Error state
  OrionPPError last_error;
};

static void set_error(OrionPPContext* ctx, OrionPPError error) {
  if (ctx) {
    ctx->last_error = error;
  }
}

static void* safe_malloc(size_t size) {
  void* ptr = malloc(size);
  if (!ptr && size > 0) {
    fprintf(stderr, "Fatal: Memory allocation failed\n");
    exit(1);
  }
  return ptr;
}

static void* safe_realloc(void* ptr, size_t size) {
  void* new_ptr = realloc(ptr, size);
  if (!new_ptr && size > 0) {
    fprintf(stderr, "Fatal: Memory reallocation failed\n");
    exit(1);
  }
  return new_ptr;
}

OrionPPContext* orionpp_context_create(void) {
  OrionPPContext* ctx = safe_malloc(sizeof(OrionPPContext));
  memset(ctx, 0, sizeof(OrionPPContext));
  
  // Initialize string table with null string at offset 0
  ctx->string_table_capacity = 256;
  ctx->string_table = safe_malloc(ctx->string_table_capacity);
  ctx->string_table[0] = '\0';
  ctx->string_table_size = 1;
  
  // Initialize variable table
  ctx->variable_capacity = 64;
  ctx->variables = safe_malloc(sizeof(OrionPPVariable) * ctx->variable_capacity);
  ctx->next_var_id = 1;  // Start from 1, 0 is reserved
  
  // Initialize symbol table
  ctx->symbol_capacity = 32;
  ctx->symbols = safe_malloc(sizeof(OrionPPSymbol) * ctx->symbol_capacity);
  
  // Initialize function table
  ctx->function_capacity = 16;
  ctx->functions = safe_malloc(sizeof(OrionPPFunction) * ctx->function_capacity);
  
  // Initialize scope tracking
  ctx->next_scope_id = 1;
  ctx->current_scope_id = 0;  // Global scope
  
  ctx->last_error = ORIONPP_OK;
  return ctx;
}

void orionpp_context_destroy(OrionPPContext* ctx) {
  if (!ctx) return;
  
  free(ctx->string_table);
  free(ctx->variables);
  free(ctx->symbols);
  free(ctx->functions);
  free(ctx);
}

OrionPPError orionpp_get_last_error(OrionPPContext* ctx) {
  return ctx ? ctx->last_error : ORIONPP_ERR_INVALID_FORMAT;
}

const char* orionpp_get_error_string(OrionPPError error) {
  switch (error) {
    case ORIONPP_OK: return "Success";
    case ORIONPP_ERR_INVALID_MAGIC: return "Invalid magic number";
    case ORIONPP_ERR_INVALID_VERSION: return "Invalid version";
    case ORIONPP_ERR_INVALID_FORMAT: return "Invalid format";
    case ORIONPP_ERR_FILE_NOT_FOUND: return "File not found";
    case ORIONPP_ERR_PERMISSION_DENIED: return "Permission denied";
    case ORIONPP_ERR_OUT_OF_MEMORY: return "Out of memory";
    case ORIONPP_ERR_INVALID_FUNCTION: return "Invalid function";
    case ORIONPP_ERR_INVALID_VARIABLE: return "Invalid variable";
    case ORIONPP_ERR_INVALID_SYMBOL: return "Invalid symbol";
    case ORIONPP_ERR_BUFFER_TOO_SMALL: return "Buffer too small";
    default: return "Unknown error";
  }
}

uint32_t orionpp_add_string(OrionPPContext* ctx, const char* str) {
  if (!ctx || !str) return 0;
  
  // Check if string already exists
  for (size_t i = 1; i < ctx->string_table_size; ) {
    const char* existing = &ctx->string_table[i];
    if (strcmp(existing, str) == 0) {
      return (uint32_t)i;
    }
    i += strlen(existing) + 1;
  }
  
  // Add new string
  size_t len = strlen(str) + 1;
  uint32_t offset = (uint32_t)ctx->string_table_size;
  
  // Expand table if needed
  if (ctx->string_table_size + len > ctx->string_table_capacity) {
    while (ctx->string_table_capacity < ctx->string_table_size + len) {
      ctx->string_table_capacity *= 2;
    }
    ctx->string_table = safe_realloc(ctx->string_table, ctx->string_table_capacity);
  }
  
  strcpy(&ctx->string_table[offset], str);
  ctx->string_table_size += len;
  
  return offset;
}

const char* orionpp_get_string(OrionPPContext* ctx, uint32_t offset) {
  if (!ctx || offset >= ctx->string_table_size) return NULL;
  return &ctx->string_table[offset];
}

uint32_t orionpp_create_variable(OrionPPContext* ctx, OrionPPVariableType type,
                                const char* debug_name, uint8_t data_type) {
  if (!ctx) return 0;
  
  // Expand variable table if needed
  if (ctx->variable_count >= ctx->variable_capacity) {
    ctx->variable_capacity *= 2;
    ctx->variables = safe_realloc(ctx->variables, 
                                 sizeof(OrionPPVariable) * ctx->variable_capacity);
  }
  
  uint32_t var_id = ctx->next_var_id++;
  OrionPPVariable* var = &ctx->variables[ctx->variable_count++];
  
  var->id = var_id;
  var->type = type;
  var->name_offset = debug_name ? orionpp_add_string(ctx, debug_name) : 0;
  var->data_type = data_type;
  var->scope_id = ctx->current_scope_id;
  
  set_error(ctx, ORIONPP_OK);
  return var_id;
}

uint32_t orionpp_create_temp_variable(OrionPPContext* ctx, uint8_t data_type) {
  static int temp_counter = 0;
  char temp_name[32];
  snprintf(temp_name, sizeof(temp_name), "$temp_%d", temp_counter++);
  return orionpp_create_variable(ctx, ORIONPP_VAR_TEMP, temp_name, data_type);
}

OrionPPVariable* orionpp_get_variable(OrionPPContext* ctx, uint32_t var_id) {
  if (!ctx) return NULL;
  
  for (size_t i = 0; i < ctx->variable_count; i++) {
    if (ctx->variables[i].id == var_id) {
      return &ctx->variables[i];
    }
  }
  
  set_error(ctx, ORIONPP_ERR_INVALID_VARIABLE);
  return NULL;
}

const char* orionpp_get_variable_debug_name(OrionPPContext* ctx, uint32_t var_id) {
  OrionPPVariable* var = orionpp_get_variable(ctx, var_id);
  if (!var || !var->name_offset) return NULL;
  return orionpp_get_string(ctx, var->name_offset);
}

uint32_t orionpp_add_symbol(OrionPPContext* ctx, const char* name, uint8_t type,
                           uint8_t visibility, uint16_t section, uint32_t value, uint32_t size) {
  if (!ctx || !name) return 0;
  
  // Expand symbol table if needed
  if (ctx->symbol_count >= ctx->symbol_capacity) {
    ctx->symbol_capacity *= 2;
    ctx->symbols = safe_realloc(ctx->symbols, 
                               sizeof(OrionPPSymbol) * ctx->symbol_capacity);
  }
  
  uint32_t index = (uint32_t)ctx->symbol_count;
  OrionPPSymbol* symbol = &ctx->symbols[ctx->symbol_count++];
  
  symbol->name_offset = orionpp_add_string(ctx, name);
  symbol->type = type;
  symbol->visibility = visibility;
  symbol->section = section;
  symbol->value = value;
  symbol->size = size;
  
  set_error(ctx, ORIONPP_OK);
  return index;
}

OrionPPSymbol* orionpp_get_symbol(OrionPPContext* ctx, uint32_t index) {
  if (!ctx || index >= ctx->symbol_count) {
    if (ctx) set_error(ctx, ORIONPP_ERR_INVALID_SYMBOL);
    return NULL;
  }
  
  return &ctx->symbols[index];
}

OrionPPSymbol* orionpp_find_symbol(OrionPPContext* ctx, const char* name) {
  if (!ctx || !name) return NULL;
  
  for (size_t i = 0; i < ctx->symbol_count; i++) {
    const char* symbol_name = orionpp_get_string(ctx, ctx->symbols[i].name_offset);
    if (symbol_name && strcmp(symbol_name, name) == 0) {
      return &ctx->symbols[i];
    }
  }
  
  set_error(ctx, ORIONPP_ERR_INVALID_SYMBOL);
  return NULL;
}

uint32_t orionpp_begin_function(OrionPPContext* ctx, const char* name,
                               uint16_t param_count, uint8_t return_type) {
  if (!ctx || !name) return 0;
  
  // Expand function table if needed
  if (ctx->function_count >= ctx->function_capacity) {
    ctx->function_capacity *= 2;
    ctx->functions = safe_realloc(ctx->functions,
                                 sizeof(OrionPPFunction) * ctx->function_capacity);
  }
  
  // Add function symbol
  uint32_t symbol_index = orionpp_add_symbol(ctx, name, ORIONPP_SYMBOL_FUNCTION,
                                            ORIONPP_VIS_GLOBAL, 0, 0, 0);
  
  uint32_t function_index = (uint32_t)ctx->function_count;
  OrionPPFunction* func = &ctx->functions[ctx->function_count++];
  
  func->symbol_offset = symbol_index;
  func->param_count = param_count;
  func->local_count = 0;  // Will be updated when function ends
  func->instruction_offset = 0;  // Will be set by writer
  func->instruction_count = 0;   // Will be set by writer
  func->abi_type = ORIONPP_ABI_C;
  func->return_type = return_type;
  func->flags = 0;
  func->first_var_id = ctx->next_var_id;
  func->last_var_id = ctx->next_var_id;  // Will be updated
  
  // Enter new scope for function
  ctx->current_scope_id = ctx->next_scope_id++;
  
  set_error(ctx, ORIONPP_OK);
  return function_index;
}

OrionPPError orionpp_end_function(OrionPPContext* ctx) {
  if (!ctx || ctx->function_count == 0) {
    if (ctx) set_error(ctx, ORIONPP_ERR_INVALID_FUNCTION);
    return ORIONPP_ERR_INVALID_FUNCTION;
  }
  
  OrionPPFunction* func = &ctx->functions[ctx->function_count - 1];
  func->last_var_id = ctx->next_var_id - 1;
  
  // Count local variables in this function's scope
  uint16_t local_count = 0;
  for (size_t i = 0; i < ctx->variable_count; i++) {
    if (ctx->variables[i].scope_id == ctx->current_scope_id &&
        (ctx->variables[i].type == ORIONPP_VAR_LOCAL || 
         ctx->variables[i].type == ORIONPP_VAR_TEMP)) {
      local_count++;
    }
  }
  func->local_count = local_count;
  
  // Exit function scope
  ctx->current_scope_id = 0;  // Return to global scope
  
  set_error(ctx, ORIONPP_OK);
  return ORIONPP_OK;
}

OrionPPFunction* orionpp_get_function(OrionPPContext* ctx, uint32_t index) {
  if (!ctx || index >= ctx->function_count) {
    if (ctx) set_error(ctx, ORIONPP_ERR_INVALID_FUNCTION);
    return NULL;
  }
  
  return &ctx->functions[index];
}

// Context accessor functions
size_t orionpp_context_get_string_table_size(OrionPPContext* ctx) {
  return ctx ? ctx->string_table_size : 0;
}

size_t orionpp_context_get_symbol_count(OrionPPContext* ctx) {
  return ctx ? ctx->symbol_count : 0;
}

size_t orionpp_context_get_function_count(OrionPPContext* ctx) {
  return ctx ? ctx->function_count : 0;
}

size_t orionpp_context_get_variable_count(OrionPPContext* ctx) {
  return ctx ? ctx->variable_count : 0;
}

const char* orionpp_context_get_string_table(OrionPPContext* ctx) {
  return ctx ? ctx->string_table : NULL;
}

const OrionPPSymbol* orionpp_context_get_symbols(OrionPPContext* ctx) {
  return ctx ? ctx->symbols : NULL;
}

const OrionPPFunction* orionpp_context_get_functions(OrionPPContext* ctx) {
  return ctx ? ctx->functions : NULL;
}

const OrionPPVariable* orionpp_context_get_variables(OrionPPContext* ctx) {
  return ctx ? ctx->variables : NULL;
}

uint32_t orionpp_context_get_next_var_id(OrionPPContext* ctx) {
  return ctx ? ctx->next_var_id : 0;
}