# COIL Documentation Standard

## Overview

This document defines standards for the COIL specification documentation to ensure consistency, clarity, and accessibility across all components of the specification.

## Core Principles

1. **Clarity**: Use precise, unambiguous technical language
2. **Consistency**: Maintain consistent terminology and structure
3. **Completeness**: Document all aspects of behavior explicitly
4. **Conciseness**: Avoid redundancy while preserving completeness
5. **Accessibility**: Optimize for both human and machine readers

## Document Organization

### Directory Structure

```
/
├── README.md                  # Project overview
├── documentation-standard.md  # This document
├── CONTRIBUTING.md            # Contribution guidelines
├── LICENSE                    # License information
├── coil-docs/                 # Core specification
│   ├── index.md               # Introduction to COIL
│   ├── core/                  # Core concepts
│   │   ├── binary-format.md   # Binary format specification
│   │   └── config-format.md   # Configuration format
│   ├── isa-u/                 # Universal instruction set
│   │   ├── overview.md        # ISA-U overview
│   │   ├── control-flow.md    # Control flow instructions
│   │   └── ...                # Other instruction categories
│   ├── isa-e/                 # Extended instruction set
│   │   ├── overview.md        # ISA-E overview
│   │   ├── cpu-operations.md  # CPU-specific instructions
│   │   └── ...                # Other processor/architecture docs
│   ├── isa-c/                 # Compiler directives
│   │   ├── overview.md        # ISA-C overview
│   │   └── ...                # Specific directive docs
│   └── types/                 # Type system
│       ├── type-system.md     # Type system overview
│       └── extended-types.md  # Advanced type definitions
```

### File Naming

- Use lowercase with hyphens for file names (e.g., `binary-format.md`)
- Use descriptive names that indicate the content
- Index files should be named `overview.md` or `index.md`
- Reference files should include "reference" in the name

## Document Structure

### Header Hierarchy

Every document must follow this header structure:

```markdown
# Document Title

## Overview
Brief description (1-3 sentences) of the document's purpose.

## Main Section 1
Content for section 1...

### Subsection 1.1
More detailed content...

## Main Section 2
Content for section 2...
```

### Standard Sections

Technical specifications should include these sections:

1. **Overview**: Brief introduction to the component
2. **Purpose**: Problem being solved and design goals
3. **Specification**: Technical details and requirements
4. **Related Components**: Links to related documentation

### Instruction Documentation Template

For instruction set documentation, use this template:

```markdown
### INSTRUCTION_NAME (Opcode)

Brief description of the instruction.

**Encoding:**
```
[opcode][operand_count][operand1][operand2]...
```

**Behavior:**
- Detailed behavior description
- Edge cases and special considerations
- Flags affected
```

## Content Guidelines

### Text Style

- Write in clear, technical English
- Use present tense ("The instruction adds two values" not "The instruction will add two values")
- Keep paragraphs short (3-5 sentences maximum)
- Use active voice when possible
- Avoid ambiguous pronouns (it, they, etc.)

### Tables

- Use tables for structured information
- Include headers for all columns
- Align content for readability
- Provide a brief description before the table

### Lists

- Use numbered lists for sequential steps
- Use bullet lists for unordered collections
- Keep list items parallel in structure
- Limit nesting to 2 levels for readability

## LLM Optimization

To make documentation more accessible to language models:

1. **Explicit Relationships**: Clearly state how components relate to each other
   ```markdown
   The Universal ISA provides instructions that work on all platforms, while
   the Extended ISA provides platform-specific optimizations.
   ```

2. **Consistent Terminology**: Use the same terms consistently
   ```markdown
   // Good
   The operand specifies the memory location. This operand must be...

   // Avoid
   The operand specifies the memory location. This parameter must be...
   ```

3. **Explicit Enumeration**: List all values for enumerated types
   ```markdown
   Valid memory models are:
   - Protected: Memory access validation
   - Flat: No protection mechanisms
   - Segmented: Segment-based memory
   ```

4. **Cross-References**: Use clear, descriptive links
   ```markdown
   For more details on operand types, see the [Type System](../types/type-system.md).
   ```

5. **Metadata Tagging**: Use section headers to provide clear context
   ```markdown
   ## Behavior for Signed Integer Operands
   
   When both operands are signed integers...
   ```

6. **Atomic Explanations**: Make each section self-contained where possible
   ```markdown
   // Good
   ## Matrix Multiplication
   
   Matrix multiplication (C = A × B) requires that the column count of A equals the row count of B.
   
   // Avoid (lacks context)
   ## Matrix Multiplication
   
   This operation requires matching dimensions.
   ```

7. **Summary Tables**: Provide overview tables for complex content
   ```markdown
   | Opcode | Mnemonic | Description | Operands |
   |--------|----------|-------------|----------|
   | 0x40   | ADD      | Addition    | 2-3      |
   | 0x41   | SUB      | Subtraction | 2-3      |
   ```

## Versioning

- **Major Version**: Incompatible changes to instruction set or binary format
- **Minor Version**: Backwards-compatible additions to the specification
- **Patch Version**: Clarifications and documentation improvements

When updating documentation, indicate which specification version introduced each feature:

```markdown
### VDOT (0x82) - Since v1.0.0

Calculates the dot product of vectors.

### VCROSS (0x83) - Since v1.2.0

Calculates the cross product of 3D vectors.
```

## Review Process

All documentation should undergo these review checks:

1. **Technical Accuracy**: Verified by domain experts
2. **Completeness**: All behaviors explicitly documented
3. **Consistency**: Terminology matches across documents
4. **Readability**: Clear to the target audience
5. **LLM Compatibility**: Optimized for machine reading

## Contribution Guidelines

Contributors should:

1. Follow this documentation standard
2. Provide examples for all new features
3. Update related documents for cross-references
4. Run documentation validation tools before submission
5. Keep individual documents focused on a single topic