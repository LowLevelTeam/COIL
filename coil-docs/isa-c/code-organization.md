# Code Organization Directives

## Overview

Code organization directives control how COIL code is structured, organized, and laid out in the binary. These directives support modular development, section management, and low-level memory layout control.

## Directive Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0xF4   | INCL     | Include external file |
| 0xF5   | SECT     | Define code section |
| 0xF6   | DATA     | Insert raw data |
| 0xF7   | PADD     | Pad section to specified size |

## Detailed Specifications

### INCL (0xF4)

Includes code from an external file.

**Encoding:**
```
[0xF4][0x01][file_id: STRING]
```

**Behavior:**
- Includes the contents of the specified file at the current position
- File ID references a string in the string table
- Included file is processed as if its contents appeared at the inclusion point
- Recursive inclusion is implementation-defined (may be limited)
- Used primarily for modular code organization and reuse

### SECT (0xF5)

Defines a code section.

**Encoding:**
```
// Basic section definition
[0xF5][0x01][section_name: STRING]

// Section with flags
[0xF5][0x02][section_name: STRING][flags: PARAM0]
```

**Behavior:**
- Begins a new section in the binary
- All following code is placed in this section until another SECT directive
- Section name references a string in the string table
- Optional flags specify section properties (executable, read-only, etc.)
- Sections with the same name are merged according to implementation-defined rules

### DATA (0xF6)

Inserts raw data bytes.

**Encoding:**
```
[0xF6][0x02][count: Operand][bytes: array of uint8_t]
```

**Behavior:**
- Inserts raw bytes directly into the binary
- Count specifies the number of bytes to insert
- Bytes are placed at the current position in the current section
- Useful for inserting constants, lookup tables, and other raw data

### PADD (0xF7)

Pads the current section to a specified size.

**Encoding:**
```
[0xF7][0x01][size: Operand]
```

**Behavior:**
- Pads the current section with zeros until it reaches the specified size
- If the section is already larger than the specified size, a warning is issued
- Used for alignment, memory layout control, and fixed-size sections
- Particularly useful for embedded systems and legacy compatibility

## Section Flags

Section flags specify the properties of a section:

| Bit  | Name        | Description                 |
|------|-------------|-----------------------------|
| 0    | SECT_EXEC   | Section is executable       |
| 1    | SECT_READ   | Section is readable         |
| 2    | SECT_WRITE  | Section is writable         |
| 3    | SECT_INIT   | Section contains initialized data |
| 4    | SECT_UNINIT | Section contains uninitialized data |

Multiple flags can be combined with bitwise OR as you would a normal bitmap


## Standard Sections

While COIL allows custom section names, these standard sections are commonly used:

| Section Name | Typical Flags | Purpose |
|--------------|---------------|---------|
| "code"       | READ, EXEC    | Executable code |
| "data"       | READ, WRITE, INIT | Initialized data |
| "bss"        | READ, WRITE, UNINIT | Uninitialized data |
| "rodata"     | READ, INIT    | Read-only data |
| "reloc"      | READ          | Relocation information |
| "symtab"     | READ          | Symbol table |
| "debug"      | READ          | Debug information |

## Modular Development

Code organization directives support modular development patterns:

1. **Include Files**: Shared code can be placed in separate files and included where needed
2. **Section Control**: Related code and data can be grouped into appropriate sections
3. **Library Development**: Reusable components can be packaged as includable files
4. **Conditional Organization**: Sections can be conditionally included based on target

## Related Components

- [Conditional Compilation](./conditional-compilation.md) - IF, ELIF, ELSE, EIF directives
- [ABI Definition](./abi-definition.md) - ABI and related directives
- [Binary Format](../core/binary-format.md) - COIL binary encoding specification