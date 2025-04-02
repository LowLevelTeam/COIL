# COIL Documentation Standard

## Overview

This document defines standards for COIL specification documentation to ensure consistency, clarity, and accessibility for both humans and language models.

## File Structure

### Naming Conventions
- Use lowercase with hyphens for file names (e.g., `binary-format.md`)
- Index files should be named `index.md` or `README.md`
- Reference files should include "reference" in the name (e.g., `instruction-reference.md`)

### Directory Organization
```
/
├── README.md                  # Project overview
├── CONTRIBUTING.md            # Contribution guidelines
├── LICENSE                    # License information
├── coil-docs/                 # Core COIL specification
│   ├── index.md               # Introduction to COIL
│   ├── core/                  # Core concepts
│   │   ├── binary-format.md   # Binary format specification
│   │   └── ...
│   ├── isa-u/                 # Universal instruction set
│   ├── isa-e/                 # Extended instruction set
│   ├── isa-c/                 # Compiler instructions
│   └── types/                 # Type system
├── casm-docs/                 # CASM documentation
├── cbc-docs/                  # CBC documentation
```

## Document Format

### Header Structure
Every document should begin with:

```markdown
# Document Title

## Overview
Brief description of the document's purpose (1-3 sentences).
```

### Standard Sections
Technical documents should include these sections as appropriate:

```markdown
## Purpose
Why this component exists and what problem it solves.

## Specification
Detailed technical specifications with clear requirements.

## Examples
Practical usage examples with explanations.

## Related Components
Links to related parts of the specification.
```

### Content Guidelines
- Keep paragraphs short (3-5 sentences maximum)
- Use numbered steps for procedures
- Use tables for structured information
- Include code examples for every major concept
- Include diagrams for complex relationships

## Code Examples

- Every code example should include comments
- Provide context before each code block
- Use consistent naming conventions
- Use syntax highlighting when possible

```markdown
The following example demonstrates variable declaration:

```c
// Declare an integer variable
VAR myVariable, INT32  // 32-bit integer
```
```

## Cross-References

- Use relative links for cross-references
- Include descriptive link text
- Group related links in "Related Components" sections

```markdown
## Related Components
- [Type System](../types/type-system.md)
- [Memory Operations](../isa-u/memory-operations.md)
```

## LLM Optimization

To make documentation more accessible to language models:

1. Begin documents with clear definitions of key terms
2. Explicitly state relationships between components
3. Use consistent terminology throughout
4. Include metadata tags for important content
5. Keep individual documents focused on a single topic
6. Use explicit numbering for requirements
7. List all possible values for enumerated types