#include "orionpp/output/binary.h"
#include <string.h>

bool orionpp_binary_is_compatible(uint16_t major, uint16_t minor, uint32_t patch) {
  // Major version must match exactly
  if (major != ORIONPP_VERSION_MAJOR) {
    return false;
  }
  
  // Minor version must be <= current (backward compatibility)
  if (minor > ORIONPP_VERSION_MINOR) {
    return false;
  }
  
  // Patch version doesn't affect compatibility
  (void)patch; // Suppress unused parameter warning
  return true;
}

orionpp_result_t orionpp_module_validate_binary_header(const orionpp_binary_header_t* header) {
  if (!header) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  if (header->magic != ORIONPP_BINARY_MAGIC) {
    return ORIONPP_ERROR(ORIONPP_ERROR_CORRUPT_DATA);
  }
  
  if (!orionpp_binary_is_compatible(header->version_major, header->version_minor, header->version_patch)) {
    return ORIONPP_ERROR(ORIONPP_ERROR_VERSION_MISMATCH);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_binary_write_header(const orionpp_module_t* module, FILE* file) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  orionpp_binary_header_t header = {0};
  header.magic = ORIONPP_BINARY_MAGIC;
  header.version_major = module->version_major;
  header.version_minor = module->version_minor;
  header.version_patch = module->version_patch;
  header.features = module->features;
  header.string_table_size = orionpp_string_table_get_size(module->strings);
  header.instruction_count = module->instruction_count;
  
  if (fwrite(&header, sizeof(orionpp_binary_header_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_binary_read_header(orionpp_binary_header_t* header, FILE* file) {
  if (!header || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  if (fread(header, sizeof(orionpp_binary_header_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  return orionpp_module_validate_binary_header(header);
}

orionpp_result_t orionpp_value_write_binary(const orionpp_value_t* value, FILE* file) {
  if (!value || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Write value type
  if (fwrite(&value->type, sizeof(orionpp_value_type_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Write value data based on type
  switch (value->type) {
    case ORIONPP_VALUE_VARIABLE:
      if (fwrite(&value->variable_id, sizeof(uint32_t), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_SYMBOL:
      if (fwrite(&value->symbol, sizeof(value->symbol), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_NUMERIC:
      if (fwrite(&value->numeric, sizeof(value->numeric), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_LABEL:
      if (fwrite(&value->label, sizeof(value->label), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_STRING:
      if (fwrite(&value->string, sizeof(value->string), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_ARRAY:
      // Write array count
      if (fwrite(&value->array.count, sizeof(uint32_t), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      
      // Write each array element
      for (uint32_t i = 0; i < value->array.count; i++) {
        orionpp_result_t result = orionpp_value_write_binary(&value->array.values[i], file);
        if (ORIONPP_IS_ERROR(result)) return result;
      }
      break;
      
    case ORIONPP_VALUE_NONE:
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_value_read_binary(orionpp_value_t* value, 
                                           FILE* file,
                                           const orionpp_allocator_t* allocator) {
  if (!value || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  memset(value, 0, sizeof(orionpp_value_t));
  
  // Read value type
  if (fread(&value->type, sizeof(orionpp_value_type_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Read value data based on type
  switch (value->type) {
    case ORIONPP_VALUE_VARIABLE:
      if (fread(&value->variable_id, sizeof(uint32_t), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_SYMBOL:
      if (fread(&value->symbol, sizeof(value->symbol), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_NUMERIC:
      if (fread(&value->numeric, sizeof(value->numeric), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_LABEL:
      if (fread(&value->label, sizeof(value->label), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_STRING:
      if (fread(&value->string, sizeof(value->string), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      break;
      
    case ORIONPP_VALUE_ARRAY: {
      // Read array count
      if (fread(&value->array.count, sizeof(uint32_t), 1, file) != 1) {
        return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
      }
      
      if (value->array.count > 0) {
        value->array.values = allocator->malloc(sizeof(orionpp_value_t) * value->array.count);
        if (!value->array.values) {
          return ORIONPP_ERROR(ORIONPP_ERROR_OUT_OF_MEMORY);
        }
        
        // Read each array element
        for (uint32_t i = 0; i < value->array.count; i++) {
          orionpp_result_t result = orionpp_value_read_binary(&value->array.values[i], file, allocator);
          if (ORIONPP_IS_ERROR(result)) {
            // Clean up partial array
            for (uint32_t j = 0; j < i; j++) {
              orionpp_value_free(&value->array.values[j], allocator);
            }
            allocator->free(value->array.values);
            return result;
          }
        }
      } else {
        value->array.values = NULL;
      }
      break;
    }
    
    case ORIONPP_VALUE_NONE:
    default:
      return ORIONPP_ERROR(ORIONPP_ERROR_INVALID_INSTRUCTION);
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_write_binary(const orionpp_instruction_t* instruction,
                                                   FILE* file) {
  if (!instruction || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Write instruction header
  orionpp_binary_instruction_t bin_instr = {0};
  bin_instr.feature = instruction->feature;
  bin_instr.opcode = instruction->opcode;
  bin_instr.flags = instruction->flags;
  bin_instr.data_size = 0; // Will be calculated based on instruction type
  
  // Calculate and write data size first
  size_t data_start = ftell(file) + sizeof(orionpp_binary_instruction_t);
  if (fwrite(&bin_instr, sizeof(orionpp_binary_instruction_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Write instruction data and track size
  size_t before_data = ftell(file);
  
  switch (instruction->feature) {
    case ORIONPP_FEATURE_OBJ:
      switch (instruction->opcode) {
        case ORIONPP_OBJ_SECTION:
          if (fwrite(&instruction->obj_section, sizeof(instruction->obj_section), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_OBJ_SYMBOL:
          if (fwrite(&instruction->obj_symbol, sizeof(instruction->obj_symbol), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_OBJ_BYTE:
        case ORIONPP_OBJ_WORD:
        case ORIONPP_OBJ_DWORD:
        case ORIONPP_OBJ_QWORD: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->obj_data.value, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        default:
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ISA:
      switch (instruction->opcode) {
        case ORIONPP_ISA_LET:
          if (fwrite(&instruction->isa_let, sizeof(instruction->isa_let), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_ISA_MOV: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_mov.dest, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_mov.src, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_ADD:
        case ORIONPP_ISA_SUB:
        case ORIONPP_ISA_MUL:
        case ORIONPP_ISA_DIV:
        case ORIONPP_ISA_AND:
        case ORIONPP_ISA_OR:
        case ORIONPP_ISA_XOR:
        case ORIONPP_ISA_SHL:
        case ORIONPP_ISA_SHR: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_binary_op.dest, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_binary_op.src1, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_binary_op.src2, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_NOT: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_unary_op.dest, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_unary_op.src, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_BR_EQ:
        case ORIONPP_ISA_BR_NE:
        case ORIONPP_ISA_BR_LT:
        case ORIONPP_ISA_BR_LE:
        case ORIONPP_ISA_BR_GT:
        case ORIONPP_ISA_BR_GE: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_branch.label, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_branch.left, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          result = orionpp_value_write_binary(&instruction->isa_branch.right, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_JMP: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_jump.target, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_LABEL: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_label.label, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_CALL: {
          orionpp_result_t result = orionpp_value_write_binary(&instruction->isa_call.target, file);
          if (ORIONPP_IS_ERROR(result)) return result;
          break;
        }
        case ORIONPP_ISA_SCOPE_ENTER:
        case ORIONPP_ISA_SCOPE_LEAVE:
        case ORIONPP_ISA_RET:
        case ORIONPP_ISA_NOP:
          // No data to write
          break;
        default:
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ABI:
      switch (instruction->opcode) {
        case ORIONPP_ABI_CALLEE:
        case ORIONPP_ABI_CALLER:
          if (fwrite(&instruction->abi_declaration, sizeof(instruction->abi_declaration), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_ABI_ARGS:
        case ORIONPP_ABI_RETS:
          if (fwrite(&instruction->abi_args_rets.count, sizeof(uint32_t), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          for (uint32_t i = 0; i < instruction->abi_args_rets.count; i++) {
            orionpp_result_t result = orionpp_value_write_binary(&instruction->abi_args_rets.values[i], file);
            if (ORIONPP_IS_ERROR(result)) return result;
          }
          break;
        case ORIONPP_ABI_SETUP:
        case ORIONPP_ABI_CLEANUP:
          // No data to write
          break;
        default:
          break;
      }
      break;
      
    case ORIONPP_FEATURE_HINT:
      switch (instruction->opcode) {
        case ORIONPP_HINT_SYMEND:
          // No data to write
          break;
        default:
          if (fwrite(&instruction->hint_generic, sizeof(instruction->hint_generic), 1, file) != 1) {
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
      }
      break;
      
    default:
      break;
  }
  
  // Calculate actual data size and update header
  size_t after_data = ftell(file);
  uint32_t actual_data_size = (uint32_t)(after_data - before_data);
  
  // Go back and update the data size
  fseek(file, data_start + offsetof(orionpp_binary_instruction_t, data_size), SEEK_SET);
  if (fwrite(&actual_data_size, sizeof(uint32_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Return to end of instruction
  fseek(file, after_data, SEEK_SET);
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_instruction_read_binary(orionpp_instruction_t** instruction,
                                                  FILE* file,
                                                  const orionpp_allocator_t* allocator) {
  if (!instruction || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Read instruction header
  orionpp_binary_instruction_t bin_instr;
  if (fread(&bin_instr, sizeof(orionpp_binary_instruction_t), 1, file) != 1) {
    return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
  }
  
  // Create instruction
  orionpp_instruction_t* inst;
  orionpp_result_t result = orionpp_instruction_create(&inst, bin_instr.feature, bin_instr.opcode, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  inst->flags = bin_instr.flags;
  
  // Read instruction data (similar to write but reading instead)
  // This is a simplified version - full implementation would mirror the write logic
  switch (inst->feature) {
    case ORIONPP_FEATURE_OBJ:
      switch (inst->opcode) {
        case ORIONPP_OBJ_SECTION:
          if (fread(&inst->obj_section, sizeof(inst->obj_section), 1, file) != 1) {
            orionpp_instruction_destroy(inst, allocator);
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_OBJ_SYMBOL:
          if (fread(&inst->obj_symbol, sizeof(inst->obj_symbol), 1, file) != 1) {
            orionpp_instruction_destroy(inst, allocator);
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_OBJ_BYTE:
        case ORIONPP_OBJ_WORD:
        case ORIONPP_OBJ_DWORD:
        case ORIONPP_OBJ_QWORD: {
          orionpp_result_t result = orionpp_value_read_binary(&inst->obj_data.value, file, allocator);
          if (ORIONPP_IS_ERROR(result)) {
            orionpp_instruction_destroy(inst, allocator);
            return result;
          }
          break;
        }
        default:
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ISA:
      switch (inst->opcode) {
        case ORIONPP_ISA_LET:
          if (fread(&inst->isa_let, sizeof(inst->isa_let), 1, file) != 1) {
            orionpp_instruction_destroy(inst, allocator);
            return ORIONPP_ERROR(ORIONPP_ERROR_IO_ERROR);
          }
          break;
        case ORIONPP_ISA_SCOPE_ENTER:
        case ORIONPP_ISA_SCOPE_LEAVE:
        case ORIONPP_ISA_RET:
        case ORIONPP_ISA_NOP:
          // No data to read
          break;
        default:
          // For more complex instructions, would need to read values
          // Skip the data for now
          if (bin_instr.data_size > 0) {
            fseek(file, bin_instr.data_size, SEEK_CUR);
          }
          break;
      }
      break;
      
    case ORIONPP_FEATURE_ABI:
    case ORIONPP_FEATURE_HINT:
    default:
      // Skip unknown data
      if (bin_instr.data_size > 0) {
        fseek(file, bin_instr.data_size, SEEK_CUR);
      }
      break;
  }
  
  *instruction = inst;
  return ORIONPP_OK_PTR(inst);
}

orionpp_result_t orionpp_module_write_binary(const orionpp_module_t* module,
                                              FILE* file) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  
  // Write header
  orionpp_result_t result = orionpp_binary_write_header(module, file);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Write string table
  result = orionpp_string_table_write_binary(module->strings, file);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Write instructions
  for (uint32_t i = 0; i < module->instruction_count; i++) {
    result = orionpp_instruction_write_binary(module->instructions[i], file);
    if (ORIONPP_IS_ERROR(result)) return result;
  }
  
  return ORIONPP_OK_INT(0);
}

orionpp_result_t orionpp_module_read_binary(orionpp_module_t** module,
                                             FILE* file,
                                             const orionpp_allocator_t* allocator) {
  if (!module || !file) return ORIONPP_ERROR(ORIONPP_ERROR_NULL_POINTER);
  if (!allocator) allocator = &orionpp_default_allocator;
  
  // Read header
  orionpp_binary_header_t header;
  orionpp_result_t result = orionpp_binary_read_header(&header, file);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  // Create module
  orionpp_module_t* mod;
  result = orionpp_module_create(&mod, header.features, allocator);
  if (ORIONPP_IS_ERROR(result)) return result;
  
  mod->version_major = header.version_major;
  mod->version_minor = header.version_minor;
  mod->version_patch = header.version_patch;
  
  // Read string table
  orionpp_string_table_destroy(mod->strings);
  result = orionpp_string_table_read_binary(&mod->strings, file, header.string_table_size, allocator);
  if (ORIONPP_IS_ERROR(result)) {
    orionpp_module_destroy(mod);
    return result;
  }
  
  // Read instructions
  for (uint32_t i = 0; i < header.instruction_count; i++) {
    orionpp_instruction_t* inst;
    result = orionpp_instruction_read_binary(&inst, file, allocator);
    if (ORIONPP_IS_ERROR(result)) {
      orionpp_module_destroy(mod);
      return result;
    }
    
    result = orionpp_module_add_instruction(mod, inst);
    if (ORIONPP_IS_ERROR(result)) {
      orionpp_instruction_destroy(inst, allocator);
      orionpp_module_destroy(mod);
      return result;
    }
  }
  
  *module = mod;
  return ORIONPP_OK_PTR(mod);
}