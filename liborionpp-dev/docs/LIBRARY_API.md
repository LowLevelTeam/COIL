# Orion++ API Reference

## Overview

The Orion++ library provides a minimal, efficient API for reading and writing Orion++ files.

## Core Types

```c
#include <orionpp.h>

// Opaque handles
typedef struct OrionContext OrionContext;
typedef struct OrionWriter OrionWriter;
typedef struct OrionFunction OrionFunction;

// Data types
typedef enum {
  ORION_TYPE_VOID = 0,
  ORION_TYPE_WORD = 1
} OrionType;

// Result codes
typedef enum {
  ORION_OK = 0,
  ORION_ERROR_FILE,
  ORION_ERROR_MEMORY,
  ORION_ERROR_FORMAT,
  ORION_ERROR_INVALID
} OrionResult;

// Instruction structure
typedef struct {
  uint8_t opcode;
  uint8_t operand_count;
  uint32_t operands[4];  // Max 4 operands per instruction
} OrionInstruction;
```

## Reading API

### Context Management

```c
// Load files (auto-detects format)
OrionContext* orion_load(const char* filename);
void orion_free(OrionContext* ctx);

// Get basic info
const char* orion_get_error(void);
size_t orion_get_function_count(OrionContext* ctx);
```

### Function Access

```c
// Function iteration
OrionFunction* orion_get_function(OrionContext* ctx, size_t index);
OrionFunction* orion_find_function(OrionContext* ctx, const char* name);

// Function properties
const char* orion_function_name(OrionFunction* func);
OrionType orion_function_return_type(OrionFunction* func);
size_t orion_function_param_count(OrionFunction* func);
```

### Instruction Access

```c
// Get instruction array (read-only)
const OrionInstruction* orion_function_instructions(OrionFunction* func);
size_t orion_function_instruction_count(OrionFunction* func);

// Instruction utilities
const char* orion_opcode_name(uint8_t opcode);
bool orion_opcode_is_branch(uint8_t opcode);
```

## Writing API

### Writer Management

```c
// Create/destroy writer
OrionWriter* orion_writer_create(void);
void orion_writer_free(OrionWriter* writer);

// Output functions
OrionResult orion_write_binary(OrionWriter* writer, const char* filename);
OrionResult orion_write_human(OrionWriter* writer, const char* filename);
```

### Function Building

```c
// Function creation
uint32_t orion_add_function(OrionWriter* writer, const char* name, 
                           OrionType return_type);
void orion_add_parameter(OrionWriter* writer, uint32_t func_id, OrionType type);
void orion_set_entry_point(OrionWriter* writer, uint32_t func_id);

// Variable allocation
uint32_t orion_alloc_local(OrionWriter* writer, uint32_t func_id);

// Instruction building
void orion_add_const(OrionWriter* writer, uint32_t func_id, 
                    uint32_t dest, int64_t value);
void orion_add_let(OrionWriter* writer, uint32_t func_id,
                  uint32_t dest, uint32_t src);
void orion_add_add(OrionWriter* writer, uint32_t func_id,
                  uint32_t dest, uint32_t a, uint32_t b);
void orion_add_call(OrionWriter* writer, uint32_t func_id, 
                   const char* target, uint32_t result);
void orion_add_ret(OrionWriter* writer, uint32_t func_id, uint32_t value);

// Control flow  
uint32_t orion_add_label(OrionWriter* writer, uint32_t func_id);
void orion_add_jump(OrionWriter* writer, uint32_t func_id, uint32_t label);
void orion_add_branch_eq(OrionWriter* writer, uint32_t func_id,
                        uint32_t a, uint32_t b, uint32_t label);
```

## Utility Functions

### Format Conversion

```c
// Convert between formats
OrionResult orion_convert_to_binary(const char* human_file, const char* binary_file);
OrionResult orion_convert_to_human(const char* binary_file, const char* human_file);

// Validation
OrionResult orion_validate(const char* filename);
```

### Pretty Printing

```c
// Print to FILE*
void orion_print_function(OrionFunction* func, FILE* output);
void orion_print_instruction(const OrionInstruction* inst, FILE* output);
```

## Examples

### Reading a File

```c
#include <orionpp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    return 1;
  }
  
  OrionContext* ctx = orion_load(argv[1]);
  if (!ctx) {
    fprintf(stderr, "Error: %s\n", orion_get_error());
    return 1;
  }
  
  size_t func_count = orion_get_function_count(ctx);
  printf("Functions: %zu\n", func_count);
  
  for (size_t i = 0; i < func_count; i++) {
    OrionFunction* func = orion_get_function(ctx, i);
    printf("Function: %s\n", orion_function_name(func));
    
    const OrionInstruction* insts = orion_function_instructions(func);
    size_t inst_count = orion_function_instruction_count(func);
    
    for (size_t j = 0; j < inst_count; j++) {
      printf("  %s\n", orion_opcode_name(insts[j].opcode));
    }
  }
  
  orion_free(ctx);
  return 0;
}
```

### Writing a File

```c
#include <orionpp.h>

int main(void) {
  OrionWriter* writer = orion_writer_create();
  
  // Create main function: word main() { return 42; }
  uint32_t main_func = orion_add_function(writer, "main", ORION_TYPE_WORD);
  orion_set_entry_point(writer, main_func);
  
  // Add instructions
  uint32_t result = orion_alloc_local(writer, main_func);
  orion_add_const(writer, main_func, result, 42);
  orion_add_ret(writer, main_func, result);
  
  // Write output
  if (orion_write_binary(writer, "output.orionpp") != ORION_OK) {
    fprintf(stderr, "Write failed: %s\n", orion_get_error());
    return 1;
  }
  
  orion_writer_free(writer);
  return 0;
}
```

### Format Conversion

```c
#include <orionpp.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return 1;
  }
  
  OrionResult result;
  
  // Determine conversion direction by file extension
  const char* input = argv[1];
  const char* output = argv[2];
  
  if (strstr(output, ".hopp")) {
    result = orion_convert_to_human(input, output);
  } else {
    result = orion_convert_to_binary(input, output);
  }
  
  if (result != ORION_OK) {
    fprintf(stderr, "Conversion failed: %s\n", orion_get_error());
    return 1;
  }
  
  printf("Converted %s to %s\n", input, output);
  return 0;
}
```

## Build Integration

### CMake

```cmake
find_package(OrionPP REQUIRED)

add_executable(myprogram main.c)
target_link_libraries(myprogram OrionPP::orionpp)
```

### Makefile

```makefile
CFLAGS += -I/usr/local/include
LDFLAGS += -L/usr/local/lib -lorionpp

myprogram: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
```

---

*This API provides everything needed to work with Orion++ files efficiently. The simplified design focuses on core functionality while maintaining performance.*