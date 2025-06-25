/**
 * @file src/main.c
 * @brief Main entry point for Orion++ Virtual Machine
 */

#include "vm.h"
#include "validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  const char* input_file;
  bool debug_mode;
  bool strict_mode;
  bool validate_only;
  bool verbose;
  ValidationLevel validation_level;
} VMOptions;

static void print_usage(const char* program_name) {
  printf("Usage: %s [options] input_file.opp\n", program_name);
  printf("Options:\n");
  printf("  -d, --debug       Enable debug mode\n");
  printf("  -s, --strict      Enable strict mode\n");
  printf("  -v, --verbose     Enable verbose output\n");
  printf("  --validate-only   Only validate, don't execute\n");
  printf("  --validation-level LEVEL  Set validation level (0-3)\n");
  printf("                    0: None, 1: Basic, 2: Strict, 3: Paranoid\n");
  printf("  -h, --help        Show this help message\n");
  printf("\nExamples:\n");
  printf("  %s program.opp                    # Run program\n", program_name);
  printf("  %s -d program.opp                 # Run with debug output\n", program_name);
  printf("  %s --validate-only program.opp    # Just validate program\n", program_name);
}

static int parse_arguments(int argc, const char* argv[], VMOptions* options) {
  // Initialize default options
  options->input_file = NULL;
  options->debug_mode = false;
  options->strict_mode = false;
  options->validate_only = false;
  options->verbose = false;
  options->validation_level = OVM_VALIDATE_BASIC;
  
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
      options->debug_mode = true;
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--strict") == 0) {
      options->strict_mode = true;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      options->verbose = true;
    } else if (strcmp(argv[i], "--validate-only") == 0) {
      options->validate_only = true;
    } else if (strcmp(argv[i], "--validation-level") == 0) {
      if (i + 1 >= argc) {
        fprintf(stderr, "Error: --validation-level requires an argument\n");
        return 1;
      }
      int level = atoi(argv[++i]);
      if (level < 0 || level > 3) {
        fprintf(stderr, "Error: Invalid validation level %d (0-3 allowed)\n", level);
        return 1;
      }
      options->validation_level = (ValidationLevel)level;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      exit(0);
    } else if (argv[i][0] == '-') {
      fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
      return 1;
    } else {
      if (options->input_file != NULL) {
        fprintf(stderr, "Error: Multiple input files specified\n");
        return 1;
      }
      options->input_file = argv[i];
    }
  }
  
  if (options->input_file == NULL) {
    fprintf(stderr, "Error: No input file specified\n");
    print_usage(argv[0]);
    return 1;
  }
  
  return 0;
}

static void print_validation_result(ValidationResult result) {
  const char* result_str = ovm_validation_result_to_string(result);
  if (result == OVM_VALID) {
    printf("✓ Validation passed: %s\n", result_str);
  } else {
    printf("✗ Validation failed: %s\n", result_str);
  }
}

static int run_vm(const VMOptions* options) {
  OrionVM vm;
  int result = 0;
  
  if (options->verbose) {
    printf("Initializing Orion++ Virtual Machine...\n");
  }
  
  // Initialize VM
  if (ovm_init(&vm) != 0) {
    fprintf(stderr, "Error: Failed to initialize VM\n");
    return 1;
  }
  
  // Configure VM
  ovm_set_debug_mode(&vm, options->debug_mode, options->debug_mode ? stdout : NULL);
  ovm_set_strict_mode(&vm, options->strict_mode);
  ovm_set_validation_level(&vm, options->validation_level);
  
  if (options->verbose) {
    printf("Loading program: %s\n", options->input_file);
  }
  
  // Load program
  if (ovm_load_file(&vm, options->input_file) != 0) {
    fprintf(stderr, "Error: Failed to load file '%s': %s\n", 
            options->input_file, ovm_get_error(&vm));
    result = 1;
    goto cleanup;
  }
  
  if (options->verbose) {
    printf("Loaded %zu instructions\n", vm.instruction_count);
  }
  
  // Validate program
  if (options->verbose) {
    printf("Validating program...\n");
  }
  
  ValidationResult validation = ovm_validate_program(&vm);
  print_validation_result(validation);
  
  if (validation != OVM_VALID) {
    fprintf(stderr, "Error: Program validation failed\n");
    result = 1;
    goto cleanup;
  }
  
  if (options->validate_only) {
    printf("Validation complete. Exiting without execution.\n");
    goto cleanup;
  }
  
  // Execute program
  if (options->verbose) {
    printf("Starting execution...\n");
  }
  
  if (options->debug_mode) {
    printf("=== DEBUG MODE ENABLED ===\n");
    ovm_print_state(&vm);
    printf("=========================\n");
  }
  
  int exec_result = ovm_run(&vm);
  
  if (exec_result != 0) {
    fprintf(stderr, "Error: Execution failed: %s\n", ovm_get_error(&vm));
    result = 1;
    goto cleanup;
  }
  
  if (options->verbose || options->debug_mode) {
    printf("Execution completed successfully\n");
    printf("Return value: ");
    if (vm.return_value.is_initialized) {
      switch (vm.return_value.type) {
        case ORIONPP_TYPE_WORD:
        case ORIONPP_TYPE_SIZE:
          printf("%lld\n", (long long)vm.return_value.value.i64);
          break;
        case ORIONPP_TYPE_STRING:
          printf("\"%s\"\n", vm.return_value.value.str ? vm.return_value.value.str : "(null)");
          break;
        default:
          printf("(type: %s)\n", ovm_type_to_string(vm.return_value.type));
          break;
      }
    } else {
      printf("(void)\n");
    }
  }
  
  // Set exit code based on return value
  if (vm.return_value.is_initialized && vm.return_value.type == ORIONPP_TYPE_WORD) {
    result = (int)vm.return_value.value.i64;
  }
  
cleanup:
  ovm_destroy(&vm);
  return result;
}

int main(int argc, const char* argv[]) {
  VMOptions options;
  
  if (parse_arguments(argc, argv, &options) != 0) {
    return 1;
  }
  
  printf("Orion++ Virtual Machine v1.0\n");
  if (options.verbose) {
    printf("Input file: %s\n", options.input_file);
    printf("Debug mode: %s\n", options.debug_mode ? "enabled" : "disabled");
    printf("Strict mode: %s\n", options.strict_mode ? "enabled" : "disabled");
    printf("Validation level: %d\n", options.validation_level);
    printf("Validate only: %s\n", options.validate_only ? "yes" : "no");
    printf("---\n");
  }
  
  return run_vm(&options);
}