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
   - Debug sections: Either preserved intact or stripped based on options

4. **Section Layout Algorithm**:
   - Text sections placed first (lowest addresses)
   - Read-only data sections placed next
   - Writable data sections placed next
   - BSS sections placed last (highest addresses)
   - Each section aligned according to its requirements

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

4. **Relocation Table Generation**:
   - For shared objects, some relocations may need to be applied at load time
   - Generate a dynamic relocation table for these entries
   - Include necessary symbol information

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

4. **Size (R_SIZE)**:
   ```
   V = Z + A
   ```
   Where Z is the size of the symbol

### 5. Output Generation

Generate the final linked output:

1. **Header Creation**:
   - Generate appropriate file header
   - Set entry point if executable
   - Include necessary metadata

2. **Section Layout**:
   - Organize sections according to layout rules
   - Apply alignment requirements to each section
   - Compute final addresses (virtual and file offsets)

3. **Symbol Table**:
   - Generate final symbol table for debugging or dynamic linking
   - Update symbol values to reference final addresses
   - Strip local symbols if requested

4. **Relocation Info**:
   - Keep or discard relocation info based on output type
   - For executables: Apply all relocations, discard table
   - For shared objects: Keep dynamic relocations

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
   - Version scripts can further refine exports

3. **Version Information**:
   - Optional versioning of symbols
   - Multiple versions of same symbol
   - Compatible with symbol versioning schemes

4. **Initialization**:
   - Entry points for loading and unloading
   - Constructor/destructor functions
   - TLS (Thread Local Storage) setup

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
   - Set up TLS data
   - Call constructor functions

## Symbol Namespaces

COIL organizes symbols into distinct namespaces:

1. **Global Namespace**:
   - Default for all global and weak symbols
   - Flat namespace shared across all objects
   - Potential for naming conflicts

2. **Module Namespace**:
   - Symbols internal to a module (hidden/internal visibility)
   - Protected from external conflicts
   - Not exposed in dynamic linking

3. **Function Namespace**:
   - Local labels within a function
   - Not visible outside function scope
   - Not exported in symbol table

Symbol visibility specifies scope:
- **Default**: Globally visible, can be overridden
- **Protected**: Visible but not overridable
- **Hidden**: Visible only within the module
- **Internal**: Same as hidden but may be optimized more aggressively

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

## Executable Loading Process

When a COIL executable is loaded:

1. **Memory Mapping**:
   - Map sections to appropriate memory regions
   - Apply permissions (read, write, execute)
   - Align sections according to requirements

2. **Relocation**:
   - Apply any remaining dynamic relocations
   - Resolve external symbols

3. **Initialization**:
   - Setup runtime environment
   - Stack initialization (size, location)
   - Heap initialization
   - Environment variable setup
   - TLS initialization

4. **Execution**:
   - Transfer control to the entry point
   - Call main function with appropriate arguments

## Platform Integration

COIL supports integration with different platform binary formats:

- **ELF**: Standard on Linux, BSD, and many Unix-like systems
- **PE/COFF**: Standard on Windows
- **Mach-O**: Standard on macOS and iOS
- **Raw Binary**: For embedded systems without an OS

Each format:
- Has a translation layer to/from COIL format
- Preserves all necessary COIL semantics
- Maps COIL constructs to native equivalents

## Linking Directives

Linking behavior can be controlled with compiler directives:

- **SECT**: Define sections and their attributes
  ```
  SECT ".text.init", EXEC | READ  // Executable read-only section
  ```

- **DEF**: Define symbols for the linker
  ```
  DEF VERSION, "1.0.0"  // Define VERSION symbol
  ```

- **ABI**: Specify Application Binary Interface
  ```
  ABI SystemV  // Use SystemV calling convention
  ```

## Complete Linking Example

Consider two object files:

**main.o:**
- Sections: .text, .data, .bss
- Symbols: main (defined), printf (undefined)

**utils.o:**
- Sections: .text, .rodata
- Symbols: print_message (defined), message (defined)

Linking process:

1. **Symbol Resolution**:
   - main: Defined in main.o
   - printf: Undefined, must be provided by external library
   - print_message: Defined in utils.o
   - message: Defined in utils.o (rodata)

2. **Section Merging**:
   - .text: main.o.text + utils.o.text
   - .data: main.o.data
   - .rodata: utils.o.rodata
   - .bss: main.o.bss

3. **Relocation**:
   - Update calls from main to print_message
   - Update references to message
   - Keep printf as dynamic relocation

4. **Output**:
   - Executable with entry point at main
   - Dynamic linkage for printf

## Related Components

- [Object Format](../format/object-format.md) - COIL object file specification
- [Binary Format](../format/binary-format.md) - COIL binary encoding
- [Code Organization](../isa-c/code-organization.md) - Section management directives
- [Memory Model](./memory-model.md) - Memory organization