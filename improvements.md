# Documentation Issues and Update Recommendations

## Overview

This document identifies existing documentation files that contain issues, inaccuracies, or fail to follow the documentation standard. Each issue is categorized by severity and includes specific recommendations for updates.

## Critical Issues

These issues involve significant conceptual errors that could lead to implementation problems.

### 1. COIL Output Object Format Misrepresentation

**Affected Files:**
- `/coil-docs/core/binary-format.md`
- `/casm-docs/programming/best-practices.md` (contains COIL binary format information)

**Issues:**
- Incorrectly states that `.coilo` files are directly executable
- Fails to clarify that `.coilo` contains native opcodes for the target that must be processed by an OS-specific linker
- Does not explain that `.coilo` files can contain sections with COIL code or CBC for JIT purposes
- Creates confusion about the final stage of the toolchain

**Recommendations:**
- Replace `/coil-docs/core/binary-format.md` with updated content
- Move binary format information from `/casm-docs/programming/best-practices.md` to the appropriate file
- Clarify throughout documentation that `.coilo` files require OS-specific linking to create executables
- Update all toolchain diagrams to show the correct flow: CASM → COIL → COIL Processor → COILO → OS Linker → Executable

### 2. Incomplete Multi-Device Support

**Affected Files:**
- `/coil-docs/core/core-concepts.md`
- `/future-roadmap.md`
- `/README.md`

**Issues:**
- Multi-device support is mentioned as a Version 3 feature, but not adequately defined in current documentation
- ABI examples are exclusively CPU-focused despite COIL's claim of device-agnosticism
- No clear specification for targeting different processing units

**Recommendations:**
- Update core concepts to include multi-device framework
- Implement the new device architecture, configuration, memory model, and targeting documentation
- Revise roadmap to clarify which multi-device features are present in Version 1 vs. future versions
- Add examples for non-CPU devices in ABI documentation

### 3. CBC (COIL Byte Code) Underdevelopment

**Affected Files:**
- `/cbc-docs/overview.md`

**Issues:**
- Current CBC documentation is extremely minimal (only a brief overview)
- Lacks concrete instruction encoding and format
- Missing execution semantics and environment definition
- No clear relationship to COIL proper

**Recommendations:**
- Replace with the new CBC documentation files:
  - `/cbc-docs/cbc-overview.md`
  - `/cbc-docs/cbc-format.md`
  - `/cbc-docs/cbc-instruction-set.md`
  - `/cbc-docs/cbc-execution.md`

## Structural Issues

These issues involve documentation organization and structure that makes information difficult to find or understand.

### 1. Misplaced Content

**Affected Files:**
- `/casm-docs/programming/best-practices.md` (contains COIL binary format information)
- `/abstract-concepts.md` (contains implementation TODOs rather than formal specification)
- `/guide.md` (contains implementation guidance mixed with specification)

**Recommendations:**
- Move binary format information from best-practices to `/coil-docs/core/binary-format.md`
- Transform abstract-concepts into a formal specification gap analysis
- Separate guide into implementation recommendations and formal specification components

### 2. Inconsistent Documentation Length

**Affected Files:**
- Many documents exceed the recommended length in document-standard.md
- Some documents contain excessive detail while others lack critical information

**Recommendations:**
- Break long documents into multiple focused files following the document-standard
- Ensure each document addresses a single coherent topic
- Balance detail across documents to avoid some being overly detailed while others are sparse

### 3. Missing Links Between Documents

**Affected Files:**
- Across most documentation

**Issue:**
- Limited cross-references between related documents
- No clear navigation path through documentation

**Recommendations:**
- Add "Related Documents" section to each file
- Create a documentation map for navigation
- Implement consistent linking between related concepts

## Content Issues

These issues involve incomplete or inaccurate technical content.

### 1. Unspecified Error Handling

**Affected Files:**
- `/coil-docs/core/error-handling.md`

**Issues:**
- Error handling is described conceptually but lacks concrete error codes
- No standardized error reporting format
- Missing details on how errors propagate through the toolchain

**Recommendations:**
- Replace with the new error handling documentation:
  - `/coil-docs/core/error-classification.md`
  - `/coil-docs/core/error-detection.md`
  - `/coil-docs/core/handling-mechanisms.md`
  - `/coil-docs/core/device-error-handling.md`

### 2. Undefined Toolchain

**Affected Files:**
- No dedicated toolchain documentation exists

**Issues:**
- No clear definition of the standard toolchain components
- No specification of command-line interfaces
- Missing file format transitions between components

**Recommendations:**
- Add the new toolchain documentation:
  - `/coil-docs/implementation/toolchain-components.md`
  - `/coil-docs/implementation/file-formats.md`
  - `/coil-docs/implementation/command-interfaces.md`
  - `/coil-docs/implementation/compilation-profiles.md`

### 3. Build System References

**Affected Files:**
- Multiple files mention CMake or Makefiles

**Issue:**
- References to non-COIL build systems could create confusion
- Build system details should be implementation guides, not part of the specification

**Recommendations:**
- Remove all references to CMake and Makefiles
- Replace with COIL-specific toolchain information

## Formatting Issues

These issues involve deviations from the documentation standard format.

### 1. Inconsistent Section Naming

**Affected Files:**
- Various files use different naming conventions for similar sections

**Recommendations:**
- Standardize section names across all documents
- Follow document-standard.md recommendations

### 2. Missing Standardized Headers

**Affected Files:**
- Some files lack proper headers according to document-standard.md

**Recommendations:**
- Add consistent headers to all documents:
  - Overview section
  - Purpose section (where applicable)
  - Related Components section

### 3. Inconsistent Code Examples

**Affected Files:**
- Code examples vary in style, comments, and formatting

**Recommendations:**
- Standardize code example formatting
- Ensure all examples include comments
- Use consistent naming conventions

## Implementation Plan

To address these issues efficiently, we recommend the following phased approach:

### Phase 1: Critical Fixes
1. Correct COILO format misrepresentation
2. Implement CBC documentation
3. Add multi-device framework documentation

### Phase 2: Content Enhancements
1. Update error handling documentation
2. Add toolchain documentation
3. Remove build system references

### Phase 3: Structural Improvements
1. Reorganize misplaced content
2. Break long documents into focused components
3. Implement cross-linking between documents

### Phase 4: Formatting Standardization
1. Standardize section names
2. Add missing headers
3. Normalize code examples

## Conclusion

The COIL documentation requires significant updates to reach production readiness. By addressing the identified issues, the documentation can provide clear, accurate, and comprehensive guidance for implementers and users of the COIL specification.