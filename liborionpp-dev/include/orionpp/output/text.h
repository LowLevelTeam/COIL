#ifndef ORIONPP_OUTPUT_TEXT_H
#define ORIONPP_OUTPUT_TEXT_H

#include "orionpp/core/types.h"
#include "orionpp/core/result.h"
#include "orionpp/module/module.h"
#include "orionpp/instr/instr.h"
#include "orionpp/value/value.h"
#include <stdio.h>

// Text format output options
typedef struct {
  bool show_comments;           // Include helpful comments
  bool show_line_numbers;       // Show line numbers
  bool compact_format;          // Minimize whitespace
  uint32_t indent_size;         // Number of spaces for indentation
} orionpp_text_options_t;

// Default text options
extern const orionpp_text_options_t orionpp_default_text_options;

// Module text output
orionpp_result_t orionpp_module_write_text(const orionpp_module_t* module,
                                            FILE* file,
                                            const orionpp_text_options_t* options);

// Individual instruction text output
orionpp_result_t orionpp_instruction_write_text(const orionpp_instruction_t* instruction,
                                                 const orionpp_string_table_t* string_table,
                                                 FILE* file);

// Value text output
orionpp_result_t orionpp_value_write_text(const orionpp_value_t* value,
                                           const orionpp_string_table_t* string_table,
                                           FILE* file);

// Module statistics as text
orionpp_result_t orionpp_module_write_stats(const orionpp_module_t* module,
                                             FILE* file);

#endif // ORIONPP_OUTPUT_TEXT_H