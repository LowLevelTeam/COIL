#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/stat.h>

// Include Orion object library
#include "../../liborionobj-dev/include/orionobj.h"
#include "../../liborionobj-dev/include/orionobj_utils.h"

// Orion++ binary format definitions
#define ORIONPP_MAGIC 0x4F50504F  // "OPPO"

typedef struct {
  uint32_t magic;
  uint16_t version;
  uint16_t flags;
  uint32_t string_table_offset;
  uint32_t string_table_size;
  uint32_t symbol_table_offset;
  uint32_t symbol_table_size;
  uint32_t function_table_offset;
  uint32_t function_table_size;
  uint32_t instruction_data_offset;
  uint32_t instruction_data_size;
  uint32_t entry_point;
  uint32_t reserved[4];
} OrionPPHeader;

typedef struct {
  const char *filename;
  bool show_header;
  bool show_sections;
  bool show_symbols;
  bool show_functions;
  bool show_instructions;
  bool show_stats;
  bool validate_only;
  bool verbose;
  bool hex_dump;
  const char *output_format; // "human", "json", "xml"
} DumpOptions;

typedef enum {
  FILE_TYPE_UNKNOWN,
  FILE_TYPE_ORION_OBJECT,    // .orion files (liborionobj-dev format)
  FILE_TYPE_ORIONPP_BINARY,  // .orionpp files (Orion++ binary format)
  FILE_TYPE_ORIONPP_HUMAN    // .hopp files (Orion++ human format)
} FileType;

void print_version(void) {
  printf("Orion Object Dump Utility v1.0\n");
  printf("Supports Orion objects (.orion) and Orion++ binary files (.orionpp)\n");
}

void print_usage(const char *program_name) {
  printf("Usage: %s [options] <file>\n\n", program_name);
  printf("Options:\n");
  printf("  -h, --help              Show this help message\n");
  printf("  -v, --version           Show version information\n");
  printf("  -V, --verbose           Enable verbose output\n");
  printf("  -H, --header            Show file header only\n");
  printf("  -s, --sections          Show section information\n");
  printf("  -S, --symbols           Show symbol table\n");
  printf("  -f, --functions         Show function information\n");
  printf("  -i, --instructions      Show instruction disassembly\n");
  printf("  -t, --stats             Show file statistics\n");
  printf("  -c, --validate          Validate file format only\n");
  printf("  -x, --hex-dump          Show hex dump of sections\n");
  printf("  -o, --output FORMAT     Output format (human, json, xml)\n");
  printf("  -a, --all               Show all information (default)\n");
  printf("\n");
  printf("File Types Supported:\n");
  printf("  .orion                  Orion object files (liborionobj-dev format)\n");
  printf("  .orionpp                Orion++ binary files\n");
  printf("  .hopp                   Orion++ human-readable files\n");
  printf("\n");
  printf("Examples:\n");
  printf("  %s program.orion              # Full dump of Orion object\n", program_name);
  printf("  %s -H program.orionpp         # Header only of Orion++ binary\n", program_name);
  printf("  %s -S -f program.orion        # Symbols and functions\n", program_name);
  printf("  %s -c *.orion                 # Validate multiple files\n", program_name);
  printf("  %s -o json program.orionpp    # JSON output format\n", program_name);
}

FileType detect_file_type(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    return FILE_TYPE_UNKNOWN;
  }
  
  // Check for Orion object magic (ORION_MAGIC = 0x4F52494F)
  uint32_t magic;
  if (fread(&magic, sizeof(uint32_t), 1, file) == 1) {
    if (magic == 0x4F52494F) { // ORION_MAGIC
      fclose(file);
      return FILE_TYPE_ORION_OBJECT;
    } else if (magic == ORIONPP_MAGIC) { // ORIONPP_MAGIC
      fclose(file);
      return FILE_TYPE_ORIONPP_BINARY;
    }
  }
  
  fclose(file);
  
  // Check file extension for human format
  const char *ext = strrchr(filename, '.');
  if (ext && strcmp(ext, ".hopp") == 0) {
    return FILE_TYPE_ORIONPP_HUMAN;
  }
  
  return FILE_TYPE_UNKNOWN;
}

void print_file_stats(const char *filename) {
  struct stat st;
  if (stat(filename, &st) != 0) {
    printf("Cannot get file statistics\n");
    return;
  }
  
  printf("=== File Statistics ===\n");
  printf("Filename: %s\n", filename);
  printf("Size: %ld bytes\n", st.st_size);
  printf("Type: ");
  
  FileType type = detect_file_type(filename);
  switch (type) {
    case FILE_TYPE_ORION_OBJECT:
      printf("Orion Object (.orion)\n");
      break;
    case FILE_TYPE_ORIONPP_BINARY:
      printf("Orion++ Binary (.orionpp)\n");
      break;
    case FILE_TYPE_ORIONPP_HUMAN:
      printf("Orion++ Human (.hopp)\n");
      break;
    default:
      printf("Unknown\n");
      break;
  }
  printf("\n");
}

int validate_orion_object(const char *filename) {
  OrionObject *obj = orion_obj_load(filename);
  if (!obj) {
    OrionError error = orion_get_last_error();
    printf("INVALID: %s - %s\n", filename, orion_get_error_string(error));
    return 1;
  }
  
  printf("VALID: %s - Orion object with %u sections\n", filename, orion_obj_get_section_count(obj));
  orion_obj_free(obj);
  return 0;
}

int validate_orionpp_binary(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("INVALID: %s - Cannot open file\n", filename);
    return 1;
  }
  
  OrionPPHeader header;
  if (fread(&header, sizeof(header), 1, file) != 1) {
    printf("INVALID: %s - Cannot read header\n", filename);
    fclose(file);
    return 1;
  }
  
  if (header.magic != ORIONPP_MAGIC) {
    printf("INVALID: %s - Invalid magic number 0x%08X\n", filename, header.magic);
    fclose(file);
    return 1;
  }
  
  if (header.version != 1) {
    printf("INVALID: %s - Unsupported version %d\n", filename, header.version);
    fclose(file);
    return 1;
  }
  
  // Basic sanity checks
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  
  if (header.string_table_offset + header.string_table_size > file_size ||
      header.symbol_table_offset + header.symbol_table_size > file_size ||
      header.function_table_offset + header.function_table_size > file_size ||
      header.instruction_data_offset + header.instruction_data_size > file_size) {
    printf("INVALID: %s - Table offsets exceed file size\n", filename);
    fclose(file);
    return 1;
  }
  
  size_t function_count = header.function_table_size / 20; // sizeof(OrionPPFunction)
  printf("VALID: %s - Orion++ binary with %zu functions\n", filename, function_count);
  fclose(file);
  return 0;
}

void dump_orion_object_header(OrionObject *obj, bool verbose) {
  printf("=== Orion Object Header ===\n");
  printf("Object Type: %d\n", obj->header.obj_type);
  printf("Target Architecture: %s\n", orion_get_target_name(obj->header.target_arch));
  printf("Variant Type: %s\n", orion_get_variant_name(obj->header.variant_type));
  printf("Section Count: %u\n", obj->header.section_count);
  printf("Entry Point: 0x%lx\n", obj->header.entry_point);
  
  if (verbose) {
    printf("Magic: 0x%08X\n", obj->header.magic);
    printf("Version: %d\n", obj->header.version);
    printf("Flags: 0x%08X\n", obj->header.flags);
    printf("Section Header Offset: 0x%08X\n", obj->header.section_header_offset);
    printf("Section Header Size: %u\n", obj->header.section_header_size);
    printf("String Table Index: %u\n", obj->header.string_table_index);
  }
  printf("\n");
}

void dump_orion_object_sections(OrionObject *obj, bool verbose, bool hex_dump) {
  printf("=== Sections ===\n");
  for (uint32_t i = 0; i < orion_obj_get_section_count(obj); i++) {
    OrionSection *section = orion_obj_get_section_by_index(obj, i);
    if (!section) continue;
    
    printf("Section %u: %s\n", i, section->name ? section->name : "<NULL>");
    printf("  Type: %u\n", section->header.type);
    printf("  Size: %zu bytes\n", orion_section_get_size(section));
    printf("  Flags: 0x%lx\n", orion_section_get_flags(section));
    
    if (verbose) {
      printf("  Address: 0x%lx\n", section->header.addr);
      printf("  Offset: 0x%lx\n", section->header.offset);
      printf("  Link: %u\n", section->header.link);
      printf("  Info: %u\n", section->header.info);
      printf("  Alignment: %lu\n", section->header.alignment);
      printf("  Entry Size: %lu\n", section->header.entry_size);
    }
    
    if (hex_dump && orion_section_get_size(section) > 0) {
      printf("  Data (hex):\n");
      uint8_t *data = (uint8_t*)orion_section_get_data(section);
      size_t size = orion_section_get_size(section);
      for (size_t j = 0; j < size; j += 16) {
        printf("    %04zx: ", j);
        for (size_t k = 0; k < 16 && j + k < size; k++) {
          printf("%02x ", data[j + k]);
        }
        printf("\n");
      }
    }
    printf("\n");
  }
}

void dump_orion_object_symbols(OrionObject *obj) {
  printf("=== Symbols ===\n");
  uint32_t symbol_count = orion_obj_get_symbol_count(obj);
  printf("Symbol Count: %u\n\n", symbol_count);
  
  for (uint32_t i = 0; i < symbol_count; i++) {
    OrionSymbol *symbol = orion_obj_get_symbol_by_index(obj, i);
    if (!symbol) continue;
    
    const char *name = orion_obj_get_string(obj, symbol->name_offset);
    printf("Symbol %u: %s\n", i, name ? name : "<NULL>");
    printf("  Type: %u\n", ORION_SYM_TYPE(symbol->info));
    printf("  Binding: %u\n", ORION_SYM_BIND(symbol->info));
    printf("  Section: %u\n", symbol->section_index);
    printf("  Value: 0x%lx\n", symbol->value);
    printf("  Size: %lu\n", symbol->size);
    printf("\n");
  }
}

void dump_orionpp_header(const OrionPPHeader *header, bool verbose) {
  printf("=== Orion++ Binary Header ===\n");
  printf("Magic: 0x%08X", header->magic);
  if (header->magic == ORIONPP_MAGIC) {
    printf(" (OPPO - Valid)\n");
  } else {
    printf(" (Invalid)\n");
  }
  printf("Version: %u\n", header->version);
  printf("Flags: 0x%04X\n", header->flags);
  printf("Entry Point: %u\n", header->entry_point);
  
  if (verbose) {
    printf("String Table: offset=0x%X, size=%u bytes\n", 
           header->string_table_offset, header->string_table_size);
    printf("Symbol Table: offset=0x%X, size=%u bytes\n", 
           header->symbol_table_offset, header->symbol_table_size);
    printf("Function Table: offset=0x%X, size=%u bytes\n", 
           header->function_table_offset, header->function_table_size);
    printf("Instruction Data: offset=0x%X, size=%u bytes\n", 
           header->instruction_data_offset, header->instruction_data_size);
  }
  printf("\n");
}

void dump_orionpp_functions(FILE *file, const OrionPPHeader *header, const char *string_table) {
  if (header->function_table_size == 0) {
    printf("No functions found.\n");
    return;
  }
  
  size_t function_count = header->function_table_size / 20; // sizeof(OrionPPFunction)
  printf("=== Functions (%zu) ===\n", function_count);
  
  fseek(file, header->function_table_offset, SEEK_SET);
  
  for (size_t i = 0; i < function_count; i++) {
    struct {
      uint32_t name_offset;
      uint16_t param_count;
      uint16_t local_var_count;
      uint32_t instruction_offset;
      uint32_t instruction_count;
      uint8_t abi_type;
      uint8_t return_type;
      uint16_t flags;
    } func;
    
    if (fread(&func, 20, 1, file) != 1) {
      printf("Error reading function %zu\n", i);
      continue;
    }
    
    const char *name = "<invalid>";
    if (string_table && func.name_offset < header->string_table_size) {
      name = &string_table[func.name_offset];
    }
    
    printf("Function %zu: %s\n", i, name);
    printf("  Parameters: %u\n", func.param_count);
    printf("  Local Variables: %u\n", func.local_var_count);
    printf("  Instructions: offset=0x%X, count=%u bytes\n", 
           func.instruction_offset, func.instruction_count);
    printf("  ABI Type: %u (%s)\n", func.abi_type, 
           func.abi_type == 1 ? "C" : "Unknown");
    printf("  Return Type: %u (%s)\n", func.return_type,
           func.return_type == 0 ? "void" : func.return_type == 1 ? "i32" : "unknown");
    printf("  Flags: 0x%04X\n", func.flags);
    printf("\n");
  }
}

int dump_orion_object(const char *filename, const DumpOptions *options) {
  OrionObject *obj = orion_obj_load(filename);
  if (!obj) {
    OrionError error = orion_get_last_error();
    printf("Error loading Orion object: %s\n", orion_get_error_string(error));
    return 1;
  }
  
  if (options->show_stats) {
    print_file_stats(filename);
  }
  
  if (options->show_header) {
    dump_orion_object_header(obj, options->verbose);
  }
  
  if (options->show_sections) {
    dump_orion_object_sections(obj, options->verbose, options->hex_dump);
  }
  
  if (options->show_symbols) {
    dump_orion_object_symbols(obj);
  }
  
  if (options->show_functions) {
    // Orion objects don't have separate function tables like Orion++ binaries
    // Functions are symbols with type ORION_SYM_FUNC
    printf("=== Functions (from symbol table) ===\n");
    uint32_t symbol_count = orion_obj_get_symbol_count(obj);
    int func_count = 0;
    
    for (uint32_t i = 0; i < symbol_count; i++) {
      OrionSymbol *symbol = orion_obj_get_symbol_by_index(obj, i);
      if (symbol && ORION_SYM_TYPE(symbol->info) == ORION_SYM_FUNC) {
        const char *name = orion_obj_get_string(obj, symbol->name_offset);
        printf("Function %d: %s\n", func_count++, name ? name : "<NULL>");
        printf("  Section: %u\n", symbol->section_index);
        printf("  Address: 0x%lx\n", symbol->value);
        printf("  Size: %lu bytes\n", symbol->size);
        printf("  Binding: %s\n", 
               ORION_SYM_BIND(symbol->info) == 0 ? "local" :
               ORION_SYM_BIND(symbol->info) == 1 ? "global" : "weak");
        printf("\n");
      }
    }
    
    if (func_count == 0) {
      printf("No functions found in symbol table.\n\n");
    }
  }
  
  orion_obj_free(obj);
  return 0;
}

int dump_orionpp_binary(const char *filename, const DumpOptions *options) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error: Cannot open file '%s'\n", filename);
    return 1;
  }
  
  OrionPPHeader header;
  if (fread(&header, sizeof(header), 1, file) != 1) {
    printf("Error: Cannot read header\n");
    fclose(file);
    return 1;
  }
  
  if (header.magic != ORIONPP_MAGIC) {
    printf("Error: Invalid Orion++ binary file (magic: 0x%08X)\n", header.magic);
    fclose(file);
    return 1;
  }
  
  // Load string table
  char *string_table = NULL;
  if (header.string_table_size > 0) {
    string_table = malloc(header.string_table_size);
    if (string_table) {
      fseek(file, header.string_table_offset, SEEK_SET);
      if (fread(string_table, 1, header.string_table_size, file) != header.string_table_size) {
        printf("Warning: Could not read complete string table\n");
        free(string_table);
        string_table = NULL;
      }
    }
  }
  
  if (options->show_stats) {
    print_file_stats(filename);
  }
  
  if (options->show_header) {
    dump_orionpp_header(&header, options->verbose);
  }
  
  if (options->show_functions) {
    dump_orionpp_functions(file, &header, string_table);
  }
  
  if (options->show_instructions) {
    printf("=== Instruction Disassembly ===\n");
    printf("(Instruction disassembly not yet implemented)\n");
    printf("Instruction data: %u bytes at offset 0x%X\n", 
           header.instruction_data_size, header.instruction_data_offset);
    printf("\n");
  }
  
  free(string_table);
  fclose(file);
  return 0;
}

int dump_orionpp_human(const char *filename, const DumpOptions *options) {
  if (options->show_stats) {
    print_file_stats(filename);
  }
  
  printf("=== Orion++ Human-Readable File ===\n");
  printf("File: %s\n", filename);
  printf("This is a text-based Orion++ assembly file.\n");
  printf("Use a text editor or 'cat' to view the contents.\n");
  printf("\n");
  
  if (options->verbose) {
    printf("File contents (first 1000 characters):\n");
    printf("----------------------------------------\n");
    FILE *file = fopen(filename, "r");
    if (file) {
      char buffer[1001];
      size_t bytes_read = fread(buffer, 1, 1000, file);
      buffer[bytes_read] = '\0';
      printf("%s", buffer);
      if (bytes_read == 1000) {
        printf("...\n");
      }
      fclose(file);
    }
    printf("----------------------------------------\n");
  }
  
  return 0;
}

int main(int argc, char *argv[]) {
  DumpOptions options = {0};
  bool show_all = true;
  
  // Default options
  options.output_format = "human";
  
  struct option long_options[] = {
    {"help",         no_argument,       0, 'h'},
    {"version",      no_argument,       0, 'v'},
    {"verbose",      no_argument,       0, 'V'},
    {"header",       no_argument,       0, 'H'},
    {"sections",     no_argument,       0, 's'},
    {"symbols",      no_argument,       0, 'S'},
    {"functions",    no_argument,       0, 'f'},
    {"instructions", no_argument,       0, 'i'},
    {"stats",        no_argument,       0, 't'},
    {"validate",     no_argument,       0, 'c'},
    {"hex-dump",     no_argument,       0, 'x'},
    {"output",       required_argument, 0, 'o'},
    {"all",          no_argument,       0, 'a'},
    {0, 0, 0, 0}
  };
  
  int c;
  while ((c = getopt_long(argc, argv, "hvVHsSfitcxo:a", long_options, NULL)) != -1) {
    switch (c) {
      case 'h':
        print_usage(argv[0]);
        return 0;
        
      case 'v':
        print_version();
        return 0;
        
      case 'V':
        options.verbose = true;
        break;
        
      case 'H':
        options.show_header = true;
        show_all = false;
        break;
        
      case 's':
        options.show_sections = true;
        show_all = false;
        break;
        
      case 'S':
        options.show_symbols = true;
        show_all = false;
        break;
        
      case 'f':
        options.show_functions = true;
        show_all = false;
        break;
        
      case 'i':
        options.show_instructions = true;
        show_all = false;
        break;
        
      case 't':
        options.show_stats = true;
        break;
        
      case 'c':
        options.validate_only = true;
        break;
        
      case 'x':
        options.hex_dump = true;
        break;
        
      case 'o':
        options.output_format = optarg;
        break;
        
      case 'a':
        show_all = true;
        break;
        
      case '?':
        return 1;
        
      default:
        return 1;
    }
  }
  
  // Set default options if showing all
  if (show_all) {
    options.show_header = true;
    options.show_sections = true;
    options.show_symbols = true;
    options.show_functions = true;
    options.show_stats = true;
  }
  
  // Check for input files
  if (optind >= argc) {
    fprintf(stderr, "Error: No input files specified\n");
    print_usage(argv[0]);
    return 1;
  }
  
  int exit_code = 0;
  
  // Process each file
  for (int i = optind; i < argc; i++) {
    options.filename = argv[i];
    
    if (argc - optind > 1) {
      printf("=== %s ===\n", options.filename);
    }
    
    FileType type = detect_file_type(options.filename);
    
    if (options.validate_only) {
      switch (type) {
        case FILE_TYPE_ORION_OBJECT:
          exit_code |= validate_orion_object(options.filename);
          break;
        case FILE_TYPE_ORIONPP_BINARY:
          exit_code |= validate_orionpp_binary(options.filename);
          break;
        case FILE_TYPE_ORIONPP_HUMAN:
          printf("VALID: %s - Orion++ human-readable file\n", options.filename);
          break;
        default:
          printf("UNKNOWN: %s - Cannot determine file type\n", options.filename);
          exit_code = 1;
          break;
      }
    } else {
      switch (type) {
        case FILE_TYPE_ORION_OBJECT:
          exit_code |= dump_orion_object(options.filename, &options);
          break;
        case FILE_TYPE_ORIONPP_BINARY:
          exit_code |= dump_orionpp_binary(options.filename, &options);
          break;
        case FILE_TYPE_ORIONPP_HUMAN:
          exit_code |= dump_orionpp_human(options.filename, &options);
          break;
        default:
          printf("Error: Unknown file type for '%s'\n", options.filename);
          exit_code = 1;
          break;
      }
    }
    
    if (argc - optind > 1 && i < argc - 1) {
      printf("\n");
    }
  }
  
  return exit_code;
}