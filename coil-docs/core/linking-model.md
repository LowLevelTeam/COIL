# COIL Linking and Loading Model

## Overview

The COIL Linking and Loading Model defines how COIL object files are combined, resolved, and prepared for execution. This model enables modular development while maintaining consistent behavior across different environments.

## Linking Process

The linking process combines multiple COIL object files into a single executable or shared object:

### 1. Symbol Resolution

Symbols are resolved in the following order:

1. **Strong Definitions**: Symbols with global binding
2. **Weak Definitions**: Symbols with weak binding (used if no strong definition exists)
3. **External References**: References to symbols defined in other modules

Rules for symbol resolution:
- Multiple strong definitions of the same symbol generate an error
- Strong definition overrides weak definition
- Multiple weak definitions select one arbitrarily
- Unresolved external references generate errors (unless linking as shared object)

### 2. Section Merging

Like sections from different object files are merged:

- **Code Sections**: Combined with appropriate alignment
- **Data Sections**: Combined with appropriate alignment
- **BSS Sections**: Combined with appropriate alignment (no actual data)
- **Read-only Sections**: Combined with appropriate alignment

Special section handling:
- Entry point determination from designated section
- Thread-local storage sections properly organized
- Debug sections preserved or stripped as requested

### 3. Relocation Processing

For each relocation entry:

1. Find the symbol referenced by the relocation
2. Calculate the final address of the symbol
3. Apply the relocation formula based on relocation type
4. Patch the reference with the calculated value

Relocation types include:
- **Absolute**: Direct address reference
- **Relative**: PC-relative offset
- **GOT**: Global Offset Table reference
- **PLT**: Procedure Linkage Table reference

### 4. Output Generation

Generate the final linked output:

- **Header Creation**: Generate appropriate file header
- **Section Layout**: Organize sections with proper alignment
- **Symbol Table**: Generate final symbol table
- **Relocation Info**: Keep or discard relocation info based on output type

## Dynamic Linking

COIL supports dynamic linking for shared objects:

### Shared Object Creation

Shared objects have special linking requirements:

- **Position Independence**: All code must be position-independent
- **Symbol Visibility**: Explicit control over exported symbols
- **Version Information**: Optional versioning of symbols
- **Initialization Code**: Entry points for loading and unloading

### Dynamic Loading

The dynamic loading process:

1. **Loading**: Map shared object into memory
2. **Symbol Resolution**: Resolve symbols against already loaded modules
3. **Relocation**: Apply relocations to the loaded module
4. **Initialization**: Execute module initialization code

## Symbol Namespaces

COIL organizes symbols into namespaces:

1. **Global Namespace**: Default for all symbols
2. **Module Namespace**: Symbols internal to a module
3. **Function Namespace**: Local labels within a function

Symbol visibility controls:
- **Default**: Globally visible
- **Protected**: Visible but not overridable
- **Hidden**: Visible only within the module
- **Internal**: Same as hidden but may be optimized more aggressively

## Name Mangling

Function and variable names may be mangled to encode:

- Type information
- Namespace information
- Module information
- Compiler-specific details

Mangling scheme depends on target platform:
- C-compatible mangling for interoperability
- C++-compatible mangling for C++ interoperability
- COIL-specific mangling for enhanced type safety

## Executable Loading Process

When a COIL executable is loaded:

1. **Memory Mapping**: Map sections to appropriate memory regions
2. **Relocation**: Apply any remaining relocations
3. **Initialization**: Setup runtime environment, including:
   - Stack initialization
   - Heap initialization
   - Environment variable setup
4. **Execution**: Transfer control to the entry point

## Platform Integration

COIL supports integration with different platform binary formats:

- **ELF**: Standard on Linux, BSD, and many Unix-like systems
- **PE/COFF**: Standard on Windows
- **Mach-O**: Standard on macOS and iOS
- **Raw Binary**: For embedded systems without an OS

## Linking Directives

Linking behavior can be controlled with compiler directives:

- **SECT**: Define sections and their attributes
- **DEF**: Define symbols for the linker
- **ABI**: Specify Application Binary Interface

## Related Components

- [Object Format](../format/object-format.md) - COIL object file specification
- [Binary Format](../format/binary-format.md) - COIL binary encoding
- [Code Organization](../isa-c/code-organization.md) - Section management directives