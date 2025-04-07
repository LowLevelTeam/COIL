# COIL Linking and Loading Model

## Overview

The COIL Linking and Loading Model defines how COIL object files are combined, resolved, and prepared for execution. This model enables modular development while maintaining consistent behavior across different environments.

## Linking Process

The linking process combines multiple COIL object files into a single executable or shared object. The process follows these precise steps:

### 1. Input Processing

1. **Object File Loading**:
   - Parse all input object files
   - Verify file format and version compatibility
   - Extract section, symbol, and relocation tables

2. **Output Format Determination**:
   - Based on command-line options or default settings
   - Executable, shared object, or relocatable object
   - Target architecture and mode validation

### 2. Symbol Resolution

Symbol resolution follows a strict precedence hierarchy:

1. **Collect All Symbols**:
   - Read all symbol tables from all input objects
   - Index by name and categorize by binding type and definition status

2. **Resolution Order**:
   - Strong definitions (binding type Global)
   - Weak definitions (binding type Weak)
   - Common symbols (type Common)
   - Undefined symbols

3. **Rules**:
   - Multiple strong definitions of the same symbol generate an error
   - Strong definition overrides weak definition
   - First weak definition is chosen among multiple weak definitions
   - First common symbol is chosen among multiple common symbols
   - Common symbols yield to any defined symbol
   - Unresolved external references generate errors (unless linking as shared object)

4. **Symbol Visibility Processing**:
   - Default: Symbol is treated according to binding type
   - Protected: Symbol is visible but cannot be overridden by outside definitions
   - Hidden: Symbol is not visible outside its component
   - Internal: Similar to hidden but may be optimized more aggressively

### 3. Section Merging

Sections from different object files are merged according to these specific algorithms:

1. **Same-Named Section Merge Algorithm**:
   - Sort sections by name and object file order
   - For each unique section name:
     1. Group sections with same name
     2. Verify compatibility (type, flags)
     3. Calculate final size (sum of individual sizes + alignment padding)
     4. Allocate space in output file
     5. Copy contents from each input section, preserving alignment
     6. Update symbol values to reference new locations

2. **Section Alignment Rules**:
   - Each section is aligned to the maximum alignment of any input section
   - Alignment must be a power of 2
   - Padding bytes (zeros) are inserted to maintain alignment
   - No section starts at an unaligned address

3. **Special Section Handling**:
   - `.text` sections: Merged in order, function boundaries preserved
   - `.data` sections: Merged in order, variable boundaries preserved
   - `.bss` sections: Sizes summed (no data copied, zero-initialized at runtime)
   - `.rodata` sections: Merged in order, constant boundaries preserved

### 4. Relocation Processing

For each relocation entry:

1. **Symbol Resolution**:
   - Find the symbol referenced by the relocation
   - Determine the final address of the symbol
   - If symbol is undefined, error (except for shared objects)

2. **Relocation Type Processing**:
   - Each architecture defines its set of relocation types
   - Each type has a specific calculation formula
   - Common types include: absolute, relative, GOT-relative, PLT-relative

3. **Calculation and Application**:
   - Apply the relocation formula based on type
   - Compute the final value
   - Verify the value fits in the target field size
   - Patch the reference with the calculated value

#### Relocation Formulas

The general relocation formula is:

```
V = S + A
```

Where:
- V: Final value to write
- S: Symbol value (address)
- A: Addend value

Specialized formulas for different types:

1. **Absolute (R_ABS)**:
   ```
   V = S + A
   ```

2. **PC-relative (R_PCREL)**:
   ```
   V = S + A - P
   ```
   Where P is the position being modified

3. **GOT-relative (R_GOTREL)**:
   ```
   V = G + A - P
   ```
   Where G is the GOT offset of the symbol

## Object Format

COIL object files have the following structure:

```
[File Header]
[Section Table]
[Symbol Table]
[String Table]
[Relocation Table]
[Sections...]
```

### File Header

The file header identifies the file as a COIL object and provides essential metadata:

```c
struct CoilObjectHeader {
    char     magic[4];         // "COIL"
    uint32_t version;          // Version in format 0xMMmmpp (Major, minor, patch)
    uint32_t flags;            // File flags
    uint32_t target_pu;        // Target processing unit
    uint32_t target_arch;      // Target architecture
    uint32_t target_mode;      // Target mode
    uint64_t entry_point;      // Entry point (or 0 if not executable)
    uint32_t section_count;    // Number of sections
    uint32_t symbol_count;     // Number of symbols
    uint32_t reloc_count;      // Number of relocations
    uint64_t section_offset;   // Offset to section table
    uint64_t symbol_offset;    // Offset to symbol table
    uint64_t string_offset;    // Offset to string table
    uint64_t reloc_offset;     // Offset to relocation table
    uint8_t  endianness;       // 0 = little-endian, 1 = big-endian
    uint8_t  padding[7];       // Reserved for future use, must be 0
}
```

File flags:
- `0x0001`: Executable
- `0x0002`: Shared object
- `0x0004`: Position independent
- `0x0008`: Debug information included
- `0x0010`: Relocatable object
- `0x0020`: Contains CPU-specific code
- `0x0040`: Contains GPU-specific code
- `0x0080`: Contains NPU-specific code
- `0x0100`: Contains DSP-specific code

### Section Table

The section table maps the various sections within the file:

```c
struct SectionEntry {
    uint32_t type;       // Section type
    uint32_t flags;      // Section flags
    uint64_t offset;     // Offset from file start
    uint64_t size;       // Section size in bytes
    uint64_t addr;       // Virtual address (if applicable)
    uint64_t align;      // Section alignment
    uint32_t name_idx;   // Index into string table
    uint32_t link;       // Related section (depends on type)
    uint32_t info;       // Additional information
}
```

Section types:
- `0x01`: Code section (.text)
- `0x02`: Data section (.data)
- `0x03`: Read-only data (.rodata)
- `0x04`: BSS (uninitialized data, .bss)
- `0x05`: Symbol table (.symtab)
- `0x06`: String table (.strtab)
- `0x07`: Relocation table (.rel)

### Symbol Table

The symbol table defines global symbols and their attributes:

```c
struct SymbolEntry {
    uint32_t name_idx;   // Index into string table
    uint32_t section_idx; // Section containing the symbol (0=undefined)
    uint64_t value;      // Symbol value (offset or address)
    uint64_t size;       // Symbol size in bytes
    uint16_t type;       // Symbol type
    uint16_t bind;       // Symbol binding
    uint16_t visibility; // Symbol visibility
    uint16_t reserved;   // Reserved for future use, must be 0
}
```

Symbol types:
- `0x00`: No type
- `0x01`: Function
- `0x02`: Data object
- `0x03`: Section
- `0x04`: File
- `0x05`: Common block

Symbol binding:
- `0x00`: Local (not visible outside the object file)
- `0x01`: Global (visible to all objects being combined)
- `0x02`: Weak (visible to all objects but with lower precedence)
- `0x03`: Unique (merged with same-named symbols)

## Dynamic Linking

COIL supports dynamic linking for shared objects:

### Shared Object Creation

Shared objects have special linking requirements:

1. **Position Independence**:
   - All code must be position-independent
   - Relative addressing instead of absolute
   - GOT (Global Offset Table) for external references
   - PLT (Procedure Linkage Table) for external functions

2. **Symbol Visibility**:
   - Explicit control over exported symbols
   - Default: Export all global symbols
   - Hidden/Protected: Control symbol visibility granularly

3. **Initialization**:
   - Entry points for loading and unloading
   - Constructor/destructor functions

### Dynamic Loading

The dynamic loading process follows these steps:

1. **Loading**:
   - Map shared object into memory
   - Respect alignment and permission requirements
   - Read dynamic linking information

2. **Symbol Resolution**:
   - Resolve symbols against already loaded modules
   - Verify version compatibility
   - Generate error for unresolved symbols

3. **Relocation**:
   - Apply relocations to the loaded module
   - Update GOT and PLT entries
   - Fix direct references

4. **Initialization**:
   - Execute module initialization code
   - Set up data
   - Call constructor functions

## Name Mangling

Function and variable names may be mangled to encode:

- Type information
- Namespace information
- Module information
- Compiler-specific details

Mangling schemes:

1. **C-compatible (no mangling)**:
   - Symbol name is identical to source name
   - Used for C compatibility

2. **C++-compatible mangling**:
   - Encodes parameter types, namespaces, etc.
   - Compatible with C++ ABIs like Itanium ABI

3. **COIL-specific mangling**:
   - Encodes COIL-specific type information
   - Module qualification
   - More precise than C++ mangling
   - Used for COIL-to-COIL linking

## Implementation Requirements

1. **Symbol Resolution**: Correctly implement symbol binding precedence rules
2. **Section Merging**: Follow the precise section merging algorithm
3. **Relocation Processing**: Handle all relocation types for the target architecture
4. **Object Format**: Support the standard COIL object format
5. **Error Handling**: Report clear errors for undefined symbols, conflicting definitions, etc.
