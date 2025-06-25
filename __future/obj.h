/**
 * @file include/orion/obj.h
 * @brief Modern Orion++ Object Format - Designed for Speed, Size, and Simplicity
 * 
 * A completely modern object format designed from scratch with no legacy baggage.
 * Optimized for:
 * - Zero-copy memory mapping  
 * - Parallel parsing and linking
 * - Minimal relocations
 * - Efficient string storage
 * - Built-in compression and verification
 * - Cache-friendly layout
 * - Modern architecture support
 * - Security and integrity features
 */

#include <stddef.h> 
#include <stdint.h>

#ifndef __ORIONDEV_FILE_HANDLE_DEFINED
#define __ORIONDEV_FILE_HANDLE_DEFINED
#ifdef WIN32
  #include <windows.h>
  typedef HANDLE file_handle_t;
#else
  typedef int file_handle_t;
#endif
#endif // __ORIONDEV_FILE_HANDLE_DEFINED

// ================================ CONSTANTS ================================ //

// Version information
#define ORIONOBJ_VERSION_MAJOR 2
#define ORIONOBJ_VERSION_MINOR 1
#define ORIONOBJ_VERSION_PATCH 0

// Object Magic Number - updated for enhanced version
#define ORIONOBJ_MAGIC 0x4E49524F  // 'ORIN' in little endian
#define ORIONOBJ_ENDIAN_MARK 0x01020304

// Alignment constants - support for modern CPU requirements
#define ORIONOBJ_CACHE_LINE_SIZE 64
#define ORIONOBJ_MAX_ALIGNMENT 4096      // Support page alignment
#define ORIONOBJ_DEFAULT_ALIGNMENT 16    // Modern SIMD alignment

// Size limits - future-proofed
#define ORIONOBJ_MAX_SECTIONS 0xFFFFFFFF
#define ORIONOBJ_MAX_SYMBOLS 0xFFFFFFFF
#define ORIONOBJ_MAX_STRING_TABLE_SIZE 0xFFFFFFFFULL  // 64-bit for large projects

// ================================ FLAGS ================================ //

// Global object flags - enhanced with security and modern features
#define ORIONOBJ_FLAG_COMPRESSED        0x0001  // Object uses compression
#define ORIONOBJ_FLAG_PARALLEL_SAFE     0x0002  // Safe for parallel parsing
#define ORIONOBJ_FLAG_MEMORY_MAPPED     0x0004  // Optimized for mmap
#define ORIONOBJ_FLAG_INCREMENTAL       0x0008  // Contains incremental link data
#define ORIONOBJ_FLAG_VERIFIED          0x0010  // Contains verification data
#define ORIONOBJ_FLAG_HAS_SECTION_NAMES 0x0020  // Has optional section names
#define ORIONOBJ_FLAG_ENCRYPTED         0x0040  // Contains encrypted sections
#define ORIONOBJ_FLAG_SIGNED            0x0080  // Cryptographically signed
#define ORIONOBJ_FLAG_POSITION_INDEPENDENT 0x0100  // Position independent code
#define ORIONOBJ_FLAG_STACK_PROTECTION  0x0200  // Stack protection enabled
#define ORIONOBJ_FLAG_CFI_ENABLED       0x0400  // Control Flow Integrity
#define ORIONOBJ_FLAG_NUMA_AWARE        0x0800  // NUMA topology aware
#define ORIONOBJ_FLAG_64BIT_SAFE        0x1000  // Safe for 64-bit addressing
#define ORIONOBJ_FLAG_128BIT_READY      0x2000  // Ready for 128-bit addressing

// Section flags - enhanced 
#define ORIONOBJ_SECT_FLAG_COMPRESSED     0x0001  // Section is compressed
#define ORIONOBJ_SECT_FLAG_LAZY_LOAD      0x0002  // Load on demand
#define ORIONOBJ_SECT_FLAG_PARALLEL_PARSE 0x0004  // Can parse in parallel
#define ORIONOBJ_SECT_FLAG_HOT_DATA       0x0008  // Frequently accessed data
#define ORIONOBJ_SECT_FLAG_COLD_DATA      0x0010  // Rarely accessed data
#define ORIONOBJ_SECT_FLAG_ENCRYPTED      0x0020  // Section is encrypted
#define ORIONOBJ_SECT_FLAG_PREFETCH       0x0040  // Should be prefetched
#define ORIONOBJ_SECT_FLAG_HUGE_PAGES     0x0080  // Use huge pages if available

// Symbol flags - enhanced with modern linking features
#define ORIONOBJ_SYM_FLAG_LOCAL           0x0001  // Local symbol
#define ORIONOBJ_SYM_FLAG_GLOBAL          0x0002  // Global symbol
#define ORIONOBJ_SYM_FLAG_WEAK            0x0004  // Weak symbol
#define ORIONOBJ_SYM_FLAG_EXPORT          0x0008  // Exported symbol
#define ORIONOBJ_SYM_FLAG_IMPORT          0x0010  // Imported symbol
#define ORIONOBJ_SYM_FLAG_FUNCTION        0x0020  // Function symbol
#define ORIONOBJ_SYM_FLAG_DATA            0x0040  // Data symbol
#define ORIONOBJ_SYM_FLAG_THREAD_LOCAL    0x0080  // Thread-local symbol
#define ORIONOBJ_SYM_FLAG_IFUNC           0x0100  // Indirect function
#define ORIONOBJ_SYM_FLAG_UNIQUE_GLOBAL   0x0200  // Unique global symbol
#define ORIONOBJ_SYM_FLAG_CONSTRUCTOR     0x0400  // Constructor function
#define ORIONOBJ_SYM_FLAG_DESTRUCTOR      0x0800  // Destructor function
#define ORIONOBJ_SYM_FLAG_HIDDEN          0x1000  // Hidden visibility
#define ORIONOBJ_SYM_FLAG_PROTECTED       0x2000  // Protected visibility

// Segment flags - enhanced
#define ORIONOBJ_SEG_FLAG_READ            0x0001  // Readable segment
#define ORIONOBJ_SEG_FLAG_WRITE           0x0002  // Writable segment  
#define ORIONOBJ_SEG_FLAG_EXECUTE         0x0004  // Executable segment
#define ORIONOBJ_SEG_FLAG_STACK_EXEC      0x0008  // Stack executable (security concern)
#define ORIONOBJ_SEG_FLAG_RELRO           0x0010  // Read-only after relocation
#define ORIONOBJ_SEG_FLAG_HUGE_PAGES      0x0020  // Use huge pages
#define ORIONOBJ_SEG_FLAG_NUMA_NODE       0x0040  // NUMA node specific

// ================================ ENUMS ================================ //

enum orionobj_arch {
  ORIONOBJ_ARCH_UNKNOWN = 0,
  // x86 family
  ORIONOBJ_ARCH_80386 = 1,      // x86-32 (Intel 80386)
  ORIONOBJ_ARCH_AMD64 = 2,      // x86-64 (AMD64)
};

enum orionobj_file_type {
  ORIONOBJ_TYPE_NONE = 0,
  ORIONOBJ_TYPE_RELOCATABLE = 1,    // .o object file
  ORIONOBJ_TYPE_EXECUTABLE = 2,     // executable
  ORIONOBJ_TYPE_SHARED = 3,         // shared library
  ORIONOBJ_TYPE_STATIC = 4,         // static library
  ORIONOBJ_TYPE_CORE = 5,           // core dump
  ORIONOBJ_TYPE_KERNEL = 6,         // kernel module
  ORIONOBJ_TYPE_FIRMWARE = 7,       // firmware image
  ORIONOBJ_TYPE_MICROCODE = 8,      // microcode update
};

enum orionobj_section_type {
  ORIONOBJ_SECT_NULL = 0,
  
  // Code sections (for linking view)
  ORIONOBJ_SECT_ORIONPP = 1,        // Orion++ Instructions
  ORIONOBJ_SECT_ORION = 2,          // Orion.* Instructions  
  ORIONOBJ_SECT_NATIVE = 3,         // Native Machine Instructions
  ORIONOBJ_SECT_MICROCODE = 4,      // Microcode instructions
  ORIONOBJ_SECT_VDSO = 5,           // Virtual DSO code
  
  // Data sections (for linking view)
  ORIONOBJ_SECT_DATA = 10,          // Initialized data
  ORIONOBJ_SECT_BSS = 11,           // Uninitialized data
  ORIONOBJ_SECT_RODATA = 12,        // Read-only data
  ORIONOBJ_SECT_CONSTANTS = 13,     // Constant pool
  ORIONOBJ_SECT_TLS_DATA = 14,      // Thread-local initialized data
  ORIONOBJ_SECT_TLS_BSS = 15,       // Thread-local uninitialized data
  
  // Symbol/linking sections
  ORIONOBJ_SECT_SYMBOLS = 20,       // Symbol table
  ORIONOBJ_SECT_IMPORTS = 21,       // Import table
  ORIONOBJ_SECT_EXPORTS = 22,       // Export table
  ORIONOBJ_SECT_RELOCATIONS = 23,   // Relocation table
  ORIONOBJ_SECT_STRINGS = 24,       // String pool
  ORIONOBJ_SECT_DYNAMIC = 25,       // Dynamic linking info
  ORIONOBJ_SECT_PLT = 26,           // Procedure linkage table
  ORIONOBJ_SECT_GOT = 27,           // Global offset table
  
  // Metadata sections
  ORIONOBJ_SECT_TYPES = 30,         // Type information
  ORIONOBJ_SECT_ABI = 31,           // ABI information
  ORIONOBJ_SECT_METADATA = 32,      // IR metadata
  ORIONOBJ_SECT_ATTRIBUTES = 33,    // Section attributes
  ORIONOBJ_SECT_BUILD_INFO = 34,    // Build information
  
  // Debug sections
  ORIONOBJ_SECT_DEBUG_INFO = 40,    // Debug information
  ORIONOBJ_SECT_DEBUG_STRINGS = 41, // Debug strings
  ORIONOBJ_SECT_DEBUG_LINES = 42,   // Line number information
  ORIONOBJ_SECT_DEBUG_FRAME = 43,   // Frame information
  ORIONOBJ_SECT_DEBUG_RANGES = 44,  // Address ranges
  
  // Security sections
  ORIONOBJ_SECT_SIGNATURES = 50,    // Digital signatures
  ORIONOBJ_SECT_CHECKSUMS = 51,     // Integrity checksums
  ORIONOBJ_SECT_SECURITY_POLICY = 52, // Security policies
  ORIONOBJ_SECT_CFI_TABLE = 53,     // Control Flow Integrity
  
  // Extension sections
  ORIONOBJ_SECT_EXTENSIONS = 60,    // Format extensions
  ORIONOBJ_SECT_INCREMENTAL = 61,   // Incremental linking data
  ORIONOBJ_SECT_PROFILE_DATA = 62,  // Profile-guided optimization
  ORIONOBJ_SECT_ANNOTATIONS = 63,   // Code annotations
  
  // Note sections for various purposes
  ORIONOBJ_SECT_NOTE_GNU_PROPERTY = 70, // GNU property notes
  ORIONOBJ_SECT_NOTE_ABI_TAG = 71,      // ABI tag notes
  ORIONOBJ_SECT_NOTE_BUILD_ID = 72,     // Build ID notes
  ORIONOBJ_SECT_NOTE_GOLD_VERSION = 73, // Linker version
  
  // User-defined sections start here
  ORIONOBJ_SECT_USER_BASE = 1000,
};

enum orionobj_segment_type {
  ORIONOBJ_SEG_NULL = 0,
  ORIONOBJ_SEG_LOAD = 1,            // Loadable segment
  ORIONOBJ_SEG_DYNAMIC = 2,         // Dynamic linking information
  ORIONOBJ_SEG_INTERP = 3,          // Program interpreter
  ORIONOBJ_SEG_NOTE = 4,            // Auxiliary information
  ORIONOBJ_SEG_STACK = 5,           // Stack segment
  ORIONOBJ_SEG_TLS = 6,             // Thread-local storage
  ORIONOBJ_SEG_GNU_EH_FRAME = 7,    // Exception handling frame
  ORIONOBJ_SEG_GNU_STACK = 8,       // Stack permissions
  ORIONOBJ_SEG_GNU_RELRO = 9,       // Read-only after relocation
  ORIONOBJ_SEG_VDSO = 10,           // Virtual DSO
  ORIONOBJ_SEG_KERNEL = 11,         // Kernel segment
};

enum orionobj_compression {
  ORIONOBJ_COMPRESS_NONE = 0,
  ORIONOBJ_COMPRESS_LZ4 = 1,        // Fast decompression
  ORIONOBJ_COMPRESS_ZSTD = 2,       // Better compression ratios
  ORIONOBJ_COMPRESS_BROTLI = 3,     // High compression ratio
  ORIONOBJ_COMPRESS_LZO = 4,        // Very fast decompression
  ORIONOBJ_COMPRESS_SNAPPY = 5,     // Fast compression/decompression
};

enum orionobj_relocation_type {
  ORIONOBJ_RELOC_NONE = 0,
  ORIONOBJ_RELOC_ABSOLUTE = 1,      // Direct address
  ORIONOBJ_RELOC_RELATIVE = 2,      // PC-relative
  ORIONOBJ_RELOC_IMPORT = 3,        // Import table reference
  ORIONOBJ_RELOC_EXPORT = 4,        // Export table reference
  ORIONOBJ_RELOC_SECTION = 5,       // Section-relative
  ORIONOBJ_RELOC_GOT = 6,           // Global offset table
  ORIONOBJ_RELOC_PLT = 7,           // Procedure linkage table
  ORIONOBJ_RELOC_TLS = 8,           // Thread-local storage
  ORIONOBJ_RELOC_IFUNC = 9,         // Indirect function
};

enum orionobj_security_level {
  ORIONOBJ_SECURITY_NONE = 0,
  ORIONOBJ_SECURITY_BASIC = 1,      // Basic integrity checks
  ORIONOBJ_SECURITY_STANDARD = 2,   // Standard security features
  ORIONOBJ_SECURITY_HIGH = 3,       // High security with encryption
  ORIONOBJ_SECURITY_CRITICAL = 4,   // Critical system security
};

// ================================ CORE STRUCTURES ================================ //

/**
 * Main object header - enhanced for modern systems
 * All critical table offsets are in the header for O(1) access
 * Extended to 256 bytes for future expansion while maintaining alignment
 */
typedef struct __attribute__((packed)) orionobj_header {
  // Magic and identification (16 bytes)
  uint32_t magic;                   // ORIONOBJ_MAGIC
  uint32_t endian_mark;             // ORIONOBJ_ENDIAN_MARK
  uint16_t version_major;           // Major version
  uint16_t version_minor;           // Minor version
  uint16_t arch;                    // Target architecture
  uint16_t file_type;               // Object file type
  
  // Core properties (16 bytes)
  uint32_t flags;                   // Global flags
  uint32_t header_size;             // Size of this header
  uint64_t file_size;               // Total file size
  
  // Entry points and addresses (16 bytes)
  uint64_t entry_point;             // Entry point virtual address
  uint64_t base_address;            // Default base address for segments
  
  // Critical table offsets for O(1) access (64 bytes)
  uint32_t section_table_offset;    // Offset to section headers
  uint32_t section_count;           // Number of sections
  uint32_t segment_table_offset;    // Offset to segment headers  
  uint32_t segment_count;           // Number of segments
  uint32_t import_table_offset;     // Offset to import table
  uint32_t import_count;            // Number of imports
  uint32_t export_table_offset;     // Offset to export table
  uint32_t export_count;            // Number of exports
  uint32_t symbol_table_offset;     // Offset to symbol table
  uint32_t symbol_count;            // Number of symbols
  uint32_t string_table_offset;     // Offset to string pool
  uint64_t string_table_size;       // Size of string pool (64-bit)
  uint32_t relocation_table_offset; // Offset to relocations
  uint32_t relocation_count;        // Number of relocations
  
  // Security and verification (32 bytes)
  uint64_t content_hash;            // xxHash64 of entire file
  uint32_t header_checksum;         // CRC32 of header
  uint16_t compression_algo;        // Default compression algorithm
  uint16_t security_level;          // Security level
  uint64_t signature_offset;        // Offset to digital signature
  uint32_t signature_size;          // Size of signature
  uint32_t timestamp;               // Build timestamp
  
  // Modern system support (32 bytes)
  uint32_t numa_topology;           // NUMA topology hints
  uint32_t cache_line_size;         // Target cache line size
  uint32_t page_size;               // Target page size
  uint16_t cpu_features;            // Required CPU features
  uint16_t abi_version;             // ABI version
  uint64_t stack_size;              // Default stack size
  uint64_t heap_size;               // Default heap size
  
  // Extension support (16 bytes)
  uint32_t extension_offset;        // Offset to extensions
  uint32_t extension_count;         // Number of extensions
  uint32_t profile_data_offset;     // Offset to profile data
  uint32_t build_info_offset;       // Offset to build information
  
  // Future expansion (64 bytes reserved)
  uint64_t reserved[8];             // Reserved for future use
  
  // Total header size: 256 bytes (4 cache lines, still reasonable)
} orionobj_header_t;

/**
 * Section header - enhanced for modern systems
 */
typedef struct __attribute__((packed)) orionobj_section_header {
  uint16_t type;                    // Section type (primary identifier)
  uint16_t subtype;                 // Section subtype (refinement)
  uint32_t flags;                   // Section flags
  uint32_t name_offset;             // Offset in string table (0 = no name)
  
  // File layout
  uint64_t file_offset;             // Offset in file
  uint64_t file_size;               // Size in file
  uint64_t compressed_size;         // Compressed size (0 = uncompressed)
  
  // Memory layout and linking information
  uint64_t alignment;               // Required alignment
  uint64_t virtual_size;            // Size in memory (for BSS expansion)
  uint32_t link_section;            // Index of linked section
  uint32_t info;                    // Additional information
  
  // Performance and security
  uint64_t content_hash;            // xxHash64 of section content
  uint32_t dependency_mask;         // Bitmask of required sections
  uint16_t parse_priority;          // Higher = parse first
  uint16_t compression_algo;        // Compression algorithm
  uint16_t numa_node;               // Preferred NUMA node
  uint16_t cache_hint;              // Cache behavior hint
  
  // Reserved for future use
  uint32_t reserved[2];
} orionobj_section_header_t;

/**
 * Segment header - enhanced for modern memory management
 */
typedef struct __attribute__((packed)) orionobj_segment_header {
  uint16_t type;                    // Segment type
  uint16_t flags;                   // Segment flags (read/write/execute)
  uint32_t section_count;           // Number of sections in this segment
  
  // File layout
  uint64_t file_offset;             // Offset in file
  uint64_t file_size;               // Size in file
  
  // Memory layout - this is where virtual addresses are defined
  uint64_t virtual_addr;            // Virtual address in memory
  uint64_t physical_addr;           // Physical address (for embedded)
  uint64_t memory_size;             // Size in memory
  uint64_t alignment;               // Required alignment
  
  // Modern memory management
  uint16_t numa_node;               // Preferred NUMA node
  uint16_t memory_policy;           // Memory allocation policy
  uint32_t page_size_hint;          // Preferred page size
  
  // Security and performance
  uint32_t stack_canary;            // Stack protection canary
  uint32_t reserved;                // Reserved for future use
  
  // Section mapping - array of section indices follows this header
  // uint32_t section_indices[section_count];
} orionobj_segment_header_t;

// ================================ ENHANCED SYMBOL SYSTEM ================================ //

/**
 * Enhanced symbol with modern features
 */
typedef struct __attribute__((packed)) orionobj_symbol {
  uint32_t name_offset;             // Offset in string table
  uint32_t name_hash;               // Hash of name (for fast lookup)
  uint64_t value;                   // Address or offset  
  uint64_t size;                    // Symbol size in bytes (64-bit for large objects)
  uint32_t flags;                   // Symbol flags
  uint16_t section_index;           // Which section (0 = undefined)
  uint16_t type;                    // Symbol type
  uint16_t visibility;              // Symbol visibility
  uint16_t binding;                 // Symbol binding
  uint16_t version_index;           // Version index
  uint16_t reserved;                // Reserved for future use
} orionobj_symbol_t;

/**
 * Enhanced import table entry
 */
typedef struct __attribute__((packed)) orionobj_import {
  uint32_t name_offset;             // Offset in string table
  uint32_t name_hash;               // Hash of imported symbol name
  uint32_t local_symbol_index;      // Index in local symbol table
  uint32_t flags;                   // Import flags
  uint16_t version_major;           // Required major version
  uint16_t version_minor;           // Required minor version
  uint32_t library_name_offset;     // Library name in string table
  uint32_t checksum;                // Expected symbol checksum
} orionobj_import_t;

/**
 * Enhanced export table entry
 */
typedef struct __attribute__((packed)) orionobj_export {
  uint32_t name_offset;             // Offset in string table
  uint32_t name_hash;               // Hash of exported symbol name
  uint32_t local_symbol_index;      // Index in local symbol table
  uint32_t flags;                   // Export flags
  uint16_t version_major;           // Provided major version
  uint16_t version_minor;           // Provided minor version
  uint32_t feature_flags;           // Required CPU features
  uint32_t checksum;                // Symbol checksum
} orionobj_export_t;

// ================================ ENHANCED RELOCATIONS ================================ //

/**
 * Enhanced relocation entry
 */
typedef struct __attribute__((packed)) orionobj_relocation {
  uint64_t offset;                  // Where to apply relocation
  uint32_t symbol_index;            // Symbol index (or import index)
  uint16_t type;                    // Relocation type
  uint16_t flags;                   // Relocation flags
  int64_t addend;                   // Constant to add (64-bit)
  uint32_t reserved;                // Future use
} orionobj_relocation_t;

// ================================ SECURITY AND VERIFICATION ================================ //

/**
 * Digital signature header
 */
typedef struct __attribute__((packed)) orionobj_signature {
  uint16_t algorithm;               // Signature algorithm
  uint16_t key_size;                // Key size in bits
  uint32_t signature_size;          // Size of signature data
  uint64_t signing_timestamp;       // When it was signed
  uint32_t signer_id_offset;        // Signer ID in string table
  uint32_t certificate_offset;      // Certificate chain offset
  // Followed by signature data
} orionobj_signature_t;

/**
 * Build information for reproducible builds
 */
typedef struct __attribute__((packed)) orionobj_build_info {
  uint64_t timestamp;               // Build timestamp
  uint32_t compiler_name_offset;    // Compiler name in string table
  uint32_t compiler_version_offset; // Compiler version in string table
  uint32_t build_flags_offset;      // Build flags in string table
  uint32_t source_hash;             // Hash of source files
  uint32_t environment_hash;        // Hash of build environment
  uint32_t reserved;                // Reserved for future use
} orionobj_build_info_t;

// ================================ EXTENSIONS ================================ //

/**
 * Extension header for future extensibility
 */
typedef struct __attribute__((packed)) orionobj_extension {
  uint16_t extension_type;          // Extension type ID
  uint16_t extension_version;       // Extension version
  uint32_t extension_size;          // Size of extension data
  uint64_t extension_hash;          // Hash of extension data
  uint32_t required_flags;          // Required object flags
  uint32_t reserved;                // Reserved for future use
  // Followed by extension-specific data
} orionobj_extension_t;

// ================================ RUNTIME OBJECT ================================ //

/**
 * Runtime object structure for working with loaded objects
 * Enhanced with modern system support
 */
typedef struct orionobj {
  // File data
  void *mapped_memory;              // Memory-mapped file
  size_t mapped_size;               // Size of mapped region
  file_handle_t file_handle;        // File handle
  
  // Parsed pointers into mapped memory
  orionobj_header_t *header;
  orionobj_section_header_t *sections;
  orionobj_segment_header_t *segments;
  orionobj_symbol_t *symbols;
  orionobj_import_t *imports;
  orionobj_export_t *exports;
  orionobj_relocation_t *relocations;
  char *string_table;               // String pool
  
  // Section data pointers (array of pointers to each section's data)
  void **section_data;
  
  // Security and verification
  orionobj_signature_t *signature;
  orionobj_build_info_t *build_info;
  
  // Performance optimization
  void **symbol_hash_table;         // Hash table for fast symbol lookup
  size_t symbol_hash_size;          // Size of hash table
  
  // Runtime state
  int is_mapped;                    // Whether file is memory-mapped
  int is_verified;                  // Whether signature is verified
  int security_level;               // Current security level
  int numa_node;                    // Current NUMA node
} orionobj_t;

// ================================ FUNCTION DECLARATIONS ================================ //

// Object lifecycle
int orionobj_init(orionobj_t *obj);
int orionobj_create(orionobj_t *obj, enum orionobj_arch arch, enum orionobj_file_type type);
int orionobj_load(orionobj_t *obj, const char *filename);
int orionobj_load_fd(orionobj_t *obj, file_handle_t fd);
int orionobj_mmap(orionobj_t *obj, const char *filename);    // Memory-mapped loading
int orionobj_write(orionobj_t *obj, const char *filename);
int orionobj_write_fd(orionobj_t *obj, file_handle_t fd);
void orionobj_destroy(orionobj_t *obj);

// Section management
int orionobj_add_section(orionobj_t *obj, enum orionobj_section_type type, const void *data, size_t size);
orionobj_section_header_t *orionobj_get_section(orionobj_t *obj, enum orionobj_section_type type);
orionobj_section_header_t *orionobj_get_section_by_name(orionobj_t *obj, const char *name);
int orionobj_remove_section(orionobj_t *obj, int section_index);
int orionobj_resize_section(orionobj_t *obj, int section_index, size_t new_size);

// Segment management (for memory layout)
int orionobj_add_segment(orionobj_t *obj, enum orionobj_segment_type type, uint64_t virtual_addr, uint64_t size, uint32_t flags);
int orionobj_add_section_to_segment(orionobj_t *obj, int segment_index, int section_index);
int orionobj_set_segment_numa_policy(orionobj_t *obj, int segment_index, int numa_node, int policy);

// Enhanced symbol management
int orionobj_add_symbol(orionobj_t *obj, const char *name, uint64_t value, uint64_t size, uint32_t flags);
orionobj_symbol_t *orionobj_find_symbol(orionobj_t *obj, const char *name);
orionobj_symbol_t *orionobj_find_symbol_fast(orionobj_t *obj, uint32_t name_hash);
int orionobj_add_import(orionobj_t *obj, const char *name, const char *library, uint32_t flags);
int orionobj_add_export(orionobj_t *obj, const char *name, uint32_t symbol_index, uint32_t flags);
int orionobj_build_symbol_hash_table(orionobj_t *obj);  // Build hash table for fast lookups

// Relocation management
int orionobj_add_relocation(orionobj_t *obj, uint64_t offset, uint32_t symbol_index, enum orionobj_relocation_type type, int64_t addend);
int orionobj_apply_relocations(orionobj_t *obj);        // Apply relocations

// String management
uint32_t orionobj_add_string(orionobj_t *obj, const char *str);
const char *orionobj_get_string(orionobj_t *obj, uint32_t offset);
uint32_t orionobj_hash_string(const char *str);              // Hash function for strings
int orionobj_deduplicate_strings(orionobj_t *obj);          // Remove duplicate strings

// Security and verification
int orionobj_verify(orionobj_t *obj);                        // Verify object integrity
int orionobj_sign(orionobj_t *obj, const char *private_key_file);  // Sign object
int orionobj_verify_signature(orionobj_t *obj, const char *public_key_file);  // Verify signature
int orionobj_set_security_level(orionobj_t *obj, enum orionobj_security_level level);

// Compression and optimization
int orionobj_compress_section(orionobj_t *obj, int section_index, enum orionobj_compression algo);
int orionobj_decompress_section(orionobj_t *obj, int section_index);
int orionobj_compress_all_sections(orionobj_t *obj, enum orionobj_compression algo);

// Performance optimization
int orionobj_optimize_for_mmap(orionobj_t *obj);             // Optimize layout for memory mapping
int orionobj_sort_sections(orionobj_t *obj);                 // Sort sections for cache efficiency
int orionobj_align_sections(orionobj_t *obj);                // Align sections optimally
int orionobj_optimize_for_numa(orionobj_t *obj, int numa_node);  // NUMA optimization
int orionobj_prefetch_sections(orionobj_t *obj, uint32_t section_mask);  // Prefetch sections

// Modern system support
int orionobj_get_cpu_features(orionobj_t *obj);              // Get required CPU features
int orionobj_check_cpu_compatibility(orionobj_t *obj);       // Check CPU compatibility
int orionobj_set_numa_policy(orionobj_t *obj, int numa_node, int policy);
int orionobj_get_memory_requirements(orionobj_t *obj, size_t *min_memory, size_t *optimal_memory);

// Debug and introspection
void orionobj_dump_info(orionobj_t *obj, FILE *output);      // Dump object information
void orionobj_dump_sections(orionobj_t *obj, FILE *output);  // Dump section information
void orionobj_dump_symbols(orionobj_t *obj, FILE *output);   // Dump symbol table
int orionobj_validate_format(orionobj_t *obj);               // Validate format consistency

// Utility functions
const char *orionobj_arch_to_string(enum orionobj_arch arch);
const char *orionobj_section_type_to_string(enum orionobj_section_type type);
const char *orionobj_relocation_type_to_string(enum orionobj_relocation_type type);
int orionobj_is_big_endian(orionobj_t *obj);
int orionobj_get_word_size(orionobj_t *obj);                 // Get architecture word size

// Error handling
const char *orionobj_get_error_string(int error_code);
void orionobj_set_error_callback(void (*callback)(int error_code, const char *message));

#endif // ORION_OBJ_H