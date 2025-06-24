#include "orionpp/text.h"
#include "orionpp/instr.h"
#include "orionpp/value.h"
#include <string.h>

const orionpp_text_options_t orionpp_default_text_options = {
  .show_comments = true,
  .show_line_numbers = false,
  .compact_format = false,
  .indent_size = 2
};

static const char* section_type_names[] = {
  [ORIONPP_SECTION_TEXT] = "TEXT",
  [ORIONPP_SECTION_DATA] = "DATA",
  [ORIONPP_SECTION_BSS] = "BSS",
  [ORIONPP_SECTION_RODATA] = "RODATA",
  [ORIONPP_SECTION_SYMTAB] = "SYMTAB",
  [ORIONPP_SECTION_STRTAB] = "STRTAB",
  [ORIONPP_SECTION_RELTAB] = "RELTAB",
  [ORIONPP_SECTION_DEBUG] = "DEBUG",
};

static const char* symbol_binding_names[] = {
  [ORIONPP_SYMBOL_LOCAL] = "LOCAL",
  [ORIONPP_SYMBOL_GLOBAL] = "GLOBAL",
  [ORIONPP_SYMBOL_WEAK] = "WEAK",
  [ORIONPP_SYMBOL_COMMON] = "COMMON",
};

static const char* symbol_type_names[] = {
  [ORIONPP_SYMBOL_NONE] = "NULL",
  [ORIONPP_SYMBOL_OBJECT] = "OBJ",
  [ORIONPP_SYMBOL_FUNC] = "FUNC",
  [ORIONPP_SYMBOL_SECTION] = "SECTION",
  [ORIONPP_SYMBOL_FILE] = "FILE",
  [ORIONPP_SYMBOL_TLS] = "TLS",
};

static void write_indent(FILE* file, uint32_t level, uint32_t indent_size) {
  for (uint32_t i = 0; i < level * indent_size; i++) {
    fputc(' ', file);
  }
}

static void write_section_flags(FILE* file, orionpp_section_flags_t flags) {
  bool first = true;
  fprintf(file, "[");
  
  if (flags & ORIONPP_SECTION_FLAG_WRITE) {
    if (!first) fprintf(file, "|");
    fprintf(file, "WRITE");
    first = false;
  }
  if (flags & ORIONPP_SECTION_FLAG_ALLOC) {
    if (!first) fprintf(file, "|");
    fprintf(file, "ALLOC");
    first = false;
  }
  if (flags & ORIONPP_SECTION_FLAG_EXEC) {
    if (!first) fprintf(file, "|");
    fprintf(file, "EXEC");
    first = false;
  }
  if (flags & ORIONPP_SECTION_FLAG_MERGE) {
    if (!first) fprintf(file, "|");
    fprintf(file, "MERGE");
    first = false;
  }
  if (flags & ORIONPP_SECTION_FLAG_STRINGS) {
    if (!first) fprintf(file, "|");
    fprintf(file, "STRINGS");
    first = false;
  }
  if (flags & ORIONPP_SECTION_FLAG_TLS) {
    if (!first) fprintf(file, "|");
    fprintf(file, "TLS");
    first = false;
  }
  
  fprintf(file, "]");
}

orionpp_result_t orionpp_instruction_write_text(const orionpp_instruction_t* instruction,
                                                 const orionpp_string_table_t* string_table,
                                                 FILE* file) {
  if (!instruction || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  switch (instruction->feature) {
    case ORIONPP_FEATURE_OBJ:
      switch (instruction->opcode) {
        case ORIONPP_OBJ_SECTION: {
          const char* section_name = orionpp_string_table_get(string_table, instruction->obj_section.name_offset);
          const char* type_name = "UNKNOWN";
          
          if (instruction->obj_section.section_type < sizeof(section_type_names) / sizeof(section_type_names[0])) {
            type_name = section_type_names[instruction->obj_section.section_type];
          }
          
          fprintf(file, "obj.section %s ", type_name);
          write_section_flags(file, instruction->obj_section.section_flags);
          fprintf(file, " \"%s\"", section_name ? section_name : "<invalid>");
          break;
        }
        
        case ORIONPP_OBJ_SYMBOL: {
          const char* symbol_name = orionpp_string_table_get(string_table, instruction->obj_symbol.name_offset);
          const char* binding_name = "UNKNOWN";
          const char* type_name = "UNKNOWN";
          
          if (instruction->obj_symbol.binding < sizeof(symbol_binding_names) / sizeof(symbol_binding_names[0])) {
            binding_name = symbol_binding_names[instruction->obj_symbol.binding];
          }
          if (instruction->obj_symbol.symbol_type < sizeof(symbol_type_names) / sizeof(symbol_type_names[0])) {
            type_name = symbol_type_names[instruction->obj_symbol.symbol_type];
          }
          
          fprintf(file, "obj.sym %s %s \"%s\"", binding_name, type_name, symbol_name ? symbol_name : "<invalid>");
          break;
        }
        
        case ORIONPP_OBJ_BYTE:
          fprintf(file, ".byte ");
          orionpp_value_write_text(&instruction->obj_data.value, string_table, file);
          break;
          
        case ORIONPP_OBJ_WORD:
          fprintf(file, ".word ");
          orionpp_value_write_text(&instruction->obj_data.value, string_table, file);
          break;
          
        case ORIONPP_OBJ_DWORD:
          fprintf(file, ".dword ");
          orionpp_value_write_text(&instruction->obj_data.value, string_table, file);
          break;
          
        case ORIONPP_OBJ_QWORD:
          fprintf(file, ".qword ");
          orionpp_value_write_text(&instruction->obj_data.value, string_table, file);
          break;
          
        default:
          fprintf(file, "%s", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ISA:
      switch (instruction->opcode) {
        case ORIONPP_ISA_SCOPE_ENTER:
          fprintf(file, "isa.scope_enter");
          break;
          
        case ORIONPP_ISA_SCOPE_LEAVE:
          fprintf(file, "isa.scope_leave");
          break;
          
        case ORIONPP_ISA_LET:
          fprintf(file, "isa.let $%u", instruction->isa_let.variable_id);
          break;
          
        case ORIONPP_ISA_MOV:
          fprintf(file, "isa.mov ");
          orionpp_value_write_text(&instruction->isa_mov.dest, string_table, file);
          fprintf(file, ", ");
          orionpp_value_write_text(&instruction->isa_mov.src, string_table, file);
          break;
          
        case ORIONPP_ISA_ADD:
        case ORIONPP_ISA_SUB:
        case ORIONPP_ISA_MUL:
        case ORIONPP_ISA_DIV:
        case ORIONPP_ISA_AND:
        case ORIONPP_ISA_OR:
        case ORIONPP_ISA_XOR:
        case ORIONPP_ISA_SHL:
        case ORIONPP_ISA_SHR:
          fprintf(file, "%s ", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          orionpp_value_write_text(&instruction->isa_binary_op.dest, string_table, file);
          fprintf(file, ", ");
          orionpp_value_write_text(&instruction->isa_binary_op.src1, string_table, file);
          fprintf(file, ", ");
          orionpp_value_write_text(&instruction->isa_binary_op.src2, string_table, file);
          break;
          
        case ORIONPP_ISA_BR_EQ:
        case ORIONPP_ISA_BR_NE:
        case ORIONPP_ISA_BR_LT:
        case ORIONPP_ISA_BR_LE:
        case ORIONPP_ISA_BR_GT:
        case ORIONPP_ISA_BR_GE:
          fprintf(file, "%s ", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          orionpp_value_write_text(&instruction->isa_branch.label, string_table, file);
          fprintf(file, ", ");
          orionpp_value_write_text(&instruction->isa_branch.left, string_table, file);
          fprintf(file, ", ");
          orionpp_value_write_text(&instruction->isa_branch.right, string_table, file);
          break;
          
        case ORIONPP_ISA_JMP:
          fprintf(file, "isa.jmp ");
          orionpp_value_write_text(&instruction->isa_jump.target, string_table, file);
          break;
          
        case ORIONPP_ISA_LABEL:
          fprintf(file, "isa.label ");
          orionpp_value_write_text(&instruction->isa_label.label, string_table, file);
          break;
          
        case ORIONPP_ISA_CALL:
          fprintf(file, "isa.call ");
          orionpp_value_write_text(&instruction->isa_call.target, string_table, file);
          break;
          
        case ORIONPP_ISA_RET:
          fprintf(file, "isa.ret");
          break;
          
        case ORIONPP_ISA_NOP:
          fprintf(file, "isa.nop");
          break;
          
        default:
          fprintf(file, "%s", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ABI:
      switch (instruction->opcode) {
        case ORIONPP_ABI_CALLEE:
        case ORIONPP_ABI_CALLER: {
          const char* abi_name = orionpp_string_table_get(string_table, instruction->abi_declaration.abi_name_offset);
          fprintf(file, "%s(%s)", 
                  orionpp_instruction_get_name(instruction->feature, instruction->opcode),
                  abi_name ? abi_name : "<invalid>");
          break;
        }
        
        case ORIONPP_ABI_ARGS:
        case ORIONPP_ABI_RETS:
          fprintf(file, "%s [", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          for (uint32_t i = 0; i < instruction->abi_args_rets.count; i++) {
            if (i > 0) fprintf(file, ", ");
            orionpp_value_write_text(&instruction->abi_args_rets.values[i], string_table, file);
          }
          fprintf(file, "]");
          break;
          
        case ORIONPP_ABI_SETUP:
          fprintf(file, "abi.setup");
          break;
          
        case ORIONPP_ABI_CLEANUP:
          fprintf(file, "abi.cleanup");
          break;
          
        default:
          fprintf(file, "%s", orionpp_instruction_get_name(instruction->feature, instruction->opcode));
          break;
      }
      break;
      
    case ORIONPP_FEATURE_HINT:
      switch (instruction->opcode) {
        case ORIONPP_HINT_SYMEND:
          fprintf(file, "hint.symend");
          break;
          
        default: {
          const char* hint_data = orionpp_string_table_get(string_table, instruction->hint_generic.hint_data_offset);
          fprintf(file, "%s(%s)", 
                  orionpp_instruction_get_name(instruction->feature, instruction->opcode),
                  hint_data ? hint_data : "");
          break;
        }
      }
      break;
      
    default:
      fprintf(file, "unknown_instruction");
      break;
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_write_text(const orionpp_module_t* module,
                                            FILE* file,
                                            const orionpp_text_options_t* options) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!options) options = &orionpp_default_text_options;
  
  uint32_t line_number = 1;
  
  // Write header comment
  if (options->show_comments) {
    fprintf(file, "// OrionPP Module v%u.%u.%u\n", 
            module->version_major, module->version_minor, module->version_patch);
    fprintf(file, "// Features: ");
    
    bool first = true;
    if (module->features & ORIONPP_FEATURES_OBJ) {
      if (!first) fprintf(file, ", ");
      fprintf(file, "OBJ");
      first = false;
    }
    if (module->features & ORIONPP_FEATURES_ISA) {
      if (!first) fprintf(file, ", ");
      fprintf(file, "ISA");
      first = false;
    }
    if (module->features & ORIONPP_FEATURES_ABI) {
      if (!first) fprintf(file, ", ");
      fprintf(file, "ABI");
      first = false;
    }
    if (module->features & ORIONPP_FEATURES_HINT) {
      if (!first) fprintf(file, ", ");
      fprintf(file, "HINT");
      first = false;
    }
    
    fprintf(file, "\n\n");
    line_number += 3;
  }
  
  // Write FEATURES block
  fprintf(file, "FEATURES {\n");
  line_number++;
  
  if (module->features & ORIONPP_FEATURES_OBJ) {
    write_indent(file, 1, options->indent_size);
    fprintf(file, "OBJ\n");
    line_number++;
  }
  if (module->features & ORIONPP_FEATURES_ISA) {
    write_indent(file, 1, options->indent_size);
    fprintf(file, "ISA\n");
    line_number++;
  }
  if (module->features & ORIONPP_FEATURES_ABI) {
    write_indent(file, 1, options->indent_size);
    fprintf(file, "ABI\n");
    line_number++;
  }
  if (module->features & ORIONPP_FEATURES_HINT) {
    write_indent(file, 1, options->indent_size);
    fprintf(file, "HINT\n");
    line_number++;
  }
  
  fprintf(file, "}\n\n");
  line_number += 2;
  
  // Write instructions
  uint32_t indent_level = 0;
  
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    orionpp_instruction_t* inst = module->instructions[i];
    if (!inst) continue;
    
    // Adjust indentation based on instruction type
    bool is_symbol_start = (inst->feature == ORIONPP_FEATURE_OBJ && inst->opcode == ORIONPP_OBJ_SYMBOL);
    bool is_symbol_end = (inst->feature == ORIONPP_FEATURE_HINT && inst->opcode == ORIONPP_HINT_SYMEND);
    bool is_scope_enter = (inst->feature == ORIONPP_FEATURE_ISA && inst->opcode == ORIONPP_ISA_SCOPE_ENTER);
    bool is_scope_leave = (inst->feature == ORIONPP_FEATURE_ISA && inst->opcode == ORIONPP_ISA_SCOPE_LEAVE);
    
    // Decrease indent before certain instructions
    if (is_symbol_end || is_scope_leave) {
      if (indent_level > 0) indent_level--;
    }
    
    // Write line number if requested
    if (options->show_line_numbers) {
      fprintf(file, "%4u: ", line_number);
    }
    
    // Write indentation
    if (!options->compact_format) {
      write_indent(file, indent_level, options->indent_size);
    }
    
    // Write instruction
    orionpp_result_t result = orionpp_instruction_write_text(inst, module->strings, file);
    if (ORIONPP_IS_ERROR(result)) return result;
    
    // Add newline
    fprintf(file, "\n");
    line_number++;
    
    // Increase indent after certain instructions
    if (is_symbol_start || is_scope_enter) {
      indent_level++;
    } else if (is_symbol_end) {
      if (!options->compact_format) {
        fprintf(file, "\n"); // Extra newline after symbol end
        line_number++;
      }
    }
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_write_stats(const orionpp_module_t* module,
                                             FILE* file) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  orionpp_module_stats_t stats;
  orionpp_result_t result = orionpp_module_get_stats(module, &stats);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  fprintf(file, "OrionPP Module Statistics\n");
  fprintf(file, "========================\n");
  fprintf(file, "Version: %u.%u.%u\n", 
          module->version_major, module->version_minor, module->version_patch);
  
  fprintf(file, "Features: ");
  bool first = true;
  if (module->features & ORIONPP_FEATURES_OBJ) {
    if (!first) fprintf(file, ", ");
    fprintf(file, "OBJ");
    first = false;
  }
  if (module->features & ORIONPP_FEATURES_ISA) {
    if (!first) fprintf(file, ", ");
    fprintf(file, "ISA");
    first = false;
  }
  if (module->features & ORIONPP_FEATURES_ABI) {
    if (!first) fprintf(file, ", ");
    fprintf(file, "ABI");
    first = false;
  }
  if (module->features & ORIONPP_FEATURES_HINT) {
    if (!first) fprintf(file, ", ");
    fprintf(file, "HINT");
    first = false;
  }
  fprintf(file, "\n\n");
  
  fprintf(file, "Instructions:\n");
  fprintf(file, "  Total: %u\n", stats.total_instructions);
  fprintf(file, "  OBJ:   %u\n", stats.obj_instructions);
  fprintf(file, "  ISA:   %u\n", stats.isa_instructions);
  fprintf(file, "  ABI:   %u\n", stats.abi_instructions);
  fprintf(file, "  HINT:  %u\n", stats.hint_instructions);
  fprintf(file, "\n");
  
  fprintf(file, "String Table: %u bytes\n", stats.string_table_size);
  fprintf(file, "Estimated Binary Size: %u bytes\n", stats.estimated_binary_size);
  
  return ORIONPP_OK_INT(0);
}