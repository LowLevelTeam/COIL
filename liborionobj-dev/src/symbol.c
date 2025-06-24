#include "orionobj/symbol.h"
#include <string.h>
#include <stdlib.h>

orionobj_result_t orionobj_symbol_create(orionobj_symbol_t** symbol,
                                          const char* name,
                                          orionobj_symbol_binding_t binding,
                                          orionobj_symbol_type_t type,
                                          uint32_t section_index,
                                          uint64_t value,
                                          uint64_t size,
                                          orionobj_string_table_t* string_table,
                                          const orionobj_allocator_t* allocator) {
  if (!symbol || !name || !string_table) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  }
  if (!allocator) allocator = &orionobj_default_allocator;

  orionobj_symbol_t* sym = allocator->malloc(sizeof(orionobj_symbol_t));
  if (!sym) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_OUT_OF_MEMORY);

  memset(sym, 0, sizeof(orionobj_symbol_t));
  sym->allocator = *allocator;

  // Add name to string table
  orionobj_result_t result = orionobj_string_table_add(string_table, name, &sym->name_offset);
  if (ORIONOBJ_IS_ERROR(result)) {
    allocator->free(sym);
    return result;
  }

  sym->binding = binding;
  sym->type = type;
  sym->section_index = section_index;
  sym->value = value;
  sym->size = size;

  *symbol = sym;
  return ORIONOBJ_OK_PTR(sym);
}

void orionobj_symbol_destroy(orionobj_symbol_t* symbol) {
  if (!symbol) return;
  symbol->allocator.free(symbol);
}

const char* orionobj_symbol_get_name(const orionobj_symbol_t* symbol,
                                      const orionobj_string_table_t* string_table) {
  if (!symbol || !string_table) return NULL;
  return orionobj_string_table_get(string_table, symbol->name_offset);
}

orionobj_symbol_binding_t orionobj_symbol_get_binding(const orionobj_symbol_t* symbol) {
  return symbol ? symbol->binding : ORIONOBJ_SYMBOL_LOCAL;
}

orionobj_symbol_type_t orionobj_symbol_get_type(const orionobj_symbol_t* symbol) {
  return symbol ? symbol->type : ORIONOBJ_SYMBOL_NONE;
}

uint32_t orionobj_symbol_get_section_index(const orionobj_symbol_t* symbol) {
  return symbol ? symbol->section_index : 0;
}

uint64_t orionobj_symbol_get_value(const orionobj_symbol_t* symbol) {
  return symbol ? symbol->value : 0;
}

uint64_t orionobj_symbol_get_size(const orionobj_symbol_t* symbol) {
  return symbol ? symbol->size : 0;
}

orionobj_result_t orionobj_symbol_set_value(orionobj_symbol_t* symbol,
                                             uint64_t value) {
  if (!symbol) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  symbol->value = value;
  return ORIONOBJ_OK_INT(0);
}

orionobj_result_t orionobj_symbol_set_size(orionobj_symbol_t* symbol,
                                            uint64_t size) {
  if (!symbol) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  symbol->size = size;
  return ORIONOBJ_OK_INT(0);
}

bool orionobj_symbol_is_undefined(const orionobj_symbol_t* symbol) {
  return symbol && symbol->section_index == 0;
}

bool orionobj_symbol_is_global(const orionobj_symbol_t* symbol) {
  return symbol && symbol->binding == ORIONOBJ_SYMBOL_GLOBAL;
}

bool orionobj_symbol_is_local(const orionobj_symbol_t* symbol) {
  return symbol && symbol->binding == ORIONOBJ_SYMBOL_LOCAL;
}

bool orionobj_symbol_is_function(const orionobj_symbol_t* symbol) {
  return symbol && symbol->type == ORIONOBJ_SYMBOL_FUNC;
}

bool orionobj_symbol_is_object(const orionobj_symbol_t* symbol) {
  return symbol && symbol->type == ORIONOBJ_SYMBOL_OBJECT;
}

orionobj_result_t orionobj_symbol_validate(const orionobj_symbol_t* symbol) {
  if (!symbol) return ORIONOBJ_ERROR(ORIONOBJ_ERROR_NULL_POINTER);
  if (symbol->binding >= ORIONOBJ_SYMBOL_BINDING_MAX) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }
  if (symbol->type >= ORIONOBJ_SYMBOL_TYPE_MAX) {
    return ORIONOBJ_ERROR(ORIONOBJ_ERROR_INVALID_FORMAT);
  }
  return ORIONOBJ_OK_INT(0);
}

const char* orionobj_symbol_binding_name(orionobj_symbol_binding_t binding) {
  switch (binding) {
    case ORIONOBJ_SYMBOL_LOCAL: return orionobj_symbol_binding_names[ORIONOBJ_SYMBOL_LOCAL];
    case ORIONOBJ_SYMBOL_GLOBAL: return orionobj_symbol_binding_names[ORIONOBJ_SYMBOL_GLOBAL];
    case ORIONOBJ_SYMBOL_WEAK: return orionobj_symbol_binding_names[ORIONOBJ_SYMBOL_WEAK];
    default: return "unknown";
  }
}

const char* orionobj_symbol_type_name(orionobj_symbol_type_t type) {
  switch (type) {
    case ORIONOBJ_SYMBOL_NONE: return orionobj_symbol_type_names[ORIONOBJ_SYMBOL_NONE];
    case ORIONOBJ_SYMBOL_OBJECT: return orionobj_symbol_type_names[ORIONOBJ_SYMBOL_OBJECT];
    case ORIONOBJ_SYMBOL_FUNC: return orionobj_symbol_type_names[ORIONOBJ_SYMBOL_FUNC];
    case ORIONOBJ_SYMBOL_SECTION: return orionobj_symbol_type_names[ORIONOBJ_SYMBOL_SECTION];
    case ORIONOBJ_SYMBOL_FILE: return orionobj_symbol_type_names[ORIONOBJ_SYMBOL_FILE];
    default: return "unknown";
  }
}