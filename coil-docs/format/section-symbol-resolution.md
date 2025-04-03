# Section Merging and Symbol Resolution

## Overview

This document specifies the precise algorithms for section merging and symbol resolution during the linking process. These specifications ensure consistent behavior across different implementations of COIL linkers.

## Section Table Structure

### Section Entry Layout

Each section entry in the section table has the following structure:

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
    uint32_t padding;    // Padding to 8-byte boundary (must be 0)
}
```

The total size of each section entry is 48 bytes, ensuring 8-byte alignment.

## Section Merging Algorithm

### Same-Named Section Merge Algorithm

When merging sections with the same name during linking:

1. **Group sections by name**
   - Collect all sections with the same name from all input objects
   - Sort them by input object order to ensure deterministic output

2. **Validate compatibility**
   - Check that all sections have compatible types
   - Check that all sections have compatible flags
     - Read-only and read-write sections cannot be merged
     - Executable and non-executable sections cannot be merged
   - If incompatible, generate an error with details

3. **Calculate final section properties**
   - Size: Sum of individual section sizes plus padding for alignment
   - Alignment: Maximum alignment of any input section
   - Flags: Combination using most restrictive flags
     - If any section is read-only, result is read-only
     - If any section requires specific alignment, use maximum

4. **Allocate output section**
   - Create a new section in the output file
   - Set size and alignment properties
   - Initialize section data to zero

5. **Copy section contents**
   - For each input section:
     - Calculate alignment padding needed at current position
     - Add padding if required
     - Copy section contents from input to output
     - Update current position

6. **Update symbol references**
   - For each symbol referencing an input section:
     - Recalculate its offset within the merged section
     - Update symbol value to point to new location

### Pseudocode for Section Merging

```
function MergeSections(inputSections):
    // Group sections by name
    sectionGroups = GroupByName(inputSections)
    
    foreach nameGroup in sectionGroups:
        // Validate compatibility
        if !AreCompatible(nameGroup):
            throw IncompatibleSectionError
            
        // Calculate properties
        totalSize = 0
        maxAlignment = 1
        mergedFlags = nameGroup[0].flags
        
        foreach section in nameGroup:
            alignedSize = AlignUp(totalSize, section.align)
            totalSize = alignedSize + section.size
            maxAlignment = Max(maxAlignment, section.align)
            mergedFlags = CombineFlags(mergedFlags, section.flags)
            
        // Create output section
        outputSection = CreateSection(
            nameGroup[0].name,
            nameGroup[0].type,
            mergedFlags,
            totalSize,
            maxAlignment
        )
        
        // Copy contents with alignment
        currentOffset = 0
        foreach section in nameGroup:
            alignedOffset = AlignUp(currentOffset, section.align)
            padding = alignedOffset - currentOffset
            
            // Add padding if needed
            if padding > 0:
                WriteZeros(outputSection, currentOffset, padding)
                
            // Copy section data
            CopyData(
                outputSection, 
                alignedOffset, 
                section, 
                0, 
                section.size
            )
            
            // Update symbols
            foreach symbol in section.symbols:
                newValue = alignedOffset + (symbol.value - section.addr)
                UpdateSymbol(symbol, outputSection, newValue)
                
            currentOffset = alignedOffset + section.size
            
        return outputSection
```

### Special Cases

1. **Empty Sections**
   - Empty sections (size = 0) still participate in merging
   - Their alignment requirements are still considered
   - Any symbols defined in empty sections must be preserved

2. **Sections with Different Attributes**
   - Sections with same name but different types: Error
   - Sections with incompatible flags: Error with specific details
   - Sections with different element sizes (in array sections): Error

## Symbol Resolution

### Symbol Resolution Rules

Symbol resolution follows these precise rules when combining symbols from multiple object files:

1. **Symbol Categories**
   - **Strong Global**: Binding type GLOBAL (0x01)
   - **Weak Global**: Binding type WEAK (0x02)
   - **Common**: Type COMMON (0x05)
   - **Undefined**: Not defined in current module (section_idx = 0)

2. **Resolution Order**
   - Process all strong global symbols first
   - Then process weak global symbols
   - Then process common symbols
   - Finally, resolve undefined symbols

3. **Resolution Algorithm**

```
function ResolveSymbols(allSymbols):
    // Group symbols by name
    symbolGroups = GroupByName(allSymbols)
    resolvedSymbols = {}
    
    foreach name in symbolGroups:
        symbols = symbolGroups[name]
        
        // Find all definitions by category
        strongDefs = FindByBinding(symbols, GLOBAL)
        weakDefs = FindByBinding(symbols, WEAK)
        commonDefs = FindByType(symbols, COMMON)
        undefinedRefs = FindUndefinedSymbols(symbols)
        
        // Apply resolution rules
        if strongDefs.length > 1:
            throw MultipleDefinitionError(name, strongDefs)
            
        else if strongDefs.length == 1:
            resolvedSymbols[name] = strongDefs[0]
            
        else if weakDefs.length > 0:
            // Take first weak definition encountered
            resolvedSymbols[name] = weakDefs[0]
            
        else if commonDefs.length > 0:
            // Take common symbol with largest size
            resolvedSymbols[name] = FindLargestSize(commonDefs)
            
        else if undefinedRefs.length > 0:
            if isExecutable:
                throw UndefinedSymbolError(name)
            else:
                // For shared objects, leave as undefined
                resolvedSymbols[name] = undefinedRefs[0]
                
    return resolvedSymbols
```

### Symbol Visibility

Symbol visibility affects resolution as follows:

1. **Default (0x00)**
   - Follows standard binding rules as described above

2. **Internal (0x01)**
   - Treated as if it has local binding
   - Not considered for external symbol resolution

3. **Hidden (0x02)**
   - Not visible outside its module
   - Not considered for external symbol resolution

4. **Protected (0x03)**
   - Visible but cannot be overridden by outside definitions
   - If a protected symbol would conflict with an external symbol, generate an error

### Common Symbol Handling

For common symbols (type = 0x05):

1. **Size Matters**
   - When multiple common symbols exist, choose the one with the largest size
   - If sizes differ, emit a warning but continue

2. **Alignment Requirements**
   - Use the strictest alignment requirement from any common definition
   - Align the final common symbol according to this requirement

3. **Strong vs. Common**
   - A strong definition always takes precedence over common symbols
   - A weak definition takes precedence over common symbols

## Implementation Requirements

1. **Deterministic Output**
   - Given the same input files, identical output must be produced
   - Section and symbol order must be preserved across implementations

2. **Clear Error Messages**
   - Symbol resolution errors must specify conflicting definitions
   - Section compatibility errors must detail the specific incompatibility

3. **Warnings**
   - Emit warnings for size mismatches in common symbols
   - Emit warnings when a weak definition is chosen over another weak definition

4. **Diagnostics**
   - Provide detailed diagnostics for symbol resolution issues
   - Include source file information where available