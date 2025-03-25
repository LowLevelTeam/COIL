# COIL Version and Release Management

## 1. Versioning Scheme

COIL follows [Semantic Versioning](https://semver.org/) with three components:

```
MAJOR.MINOR.PATCH
```

- **MAJOR** version increases for incompatible changes
- **MINOR** version increases for backwards-compatible additions
- **PATCH** version increases for backwards-compatible fixes

### 1.1 Specification Versions

The COIL specification uses this versioning scheme to track significant changes:

- **COIL v1.0.0**: Initial stable specification with CPU focus
- **COIL v2.0.0**: Standard library addition
- **COIL v3.0.0**: Multi-device support

Between major versions, minor updates may add features while maintaining compatibility:

- **COIL v1.1.0**: Additional CPU architecture support
- **COIL v1.2.0**: Extended type system features

Patch versions address clarifications, corrections, and documentation improvements:

- **COIL v1.0.1**: Clarification of type system behavior
- **COIL v1.0.2**: Fix inconsistencies in instruction descriptions

### 1.2 Implementation Versions

COIL processors should maintain their own version numbers, indicating which COIL specification they support:

```
PROCESSOR-NAME v1.2.3 (COIL v1.0.0)
```

## 2. Document Metadata

Each specification document should include version metadata:

```
---
title: "COIL Type System"
version: "1.0.0"
date: "2025-03-25"
status: "Stable"
---
```

Valid status values include:
- **Draft**: Early development, subject to major changes
- **Review**: Soliciting feedback, relatively stable
- **Stable**: Officially released, changes only through versioning process
- **Deprecated**: Superseded by newer versions, maintained for compatibility

## 3. File Naming Conventions

File names should reflect the version they document:

```
spec/v1/core.md         # Part of COIL v1.x.x specification
spec/v2/stdlib.md       # Part of COIL v2.x.x specification
```

For documents that span multiple versions or apply to all versions, use:

```
common/glossary.md      # Terminology applicable to all versions
```

## 4. Change Management

### 4.1 Changelog

All changes should be documented in a changelog:

```
# Changelog

## [1.0.0] - 2025-03-25
- Initial stable release of COIL v1 specification
- Complete CPU instruction set
- Type system finalized
- ABI system implemented

## [0.9.0] - 2025-02-15
- Release candidate 1
- Complete instruction set documentation
- Added debugging support
```

### 4.2 Pull Request Workflow

Contributions to the specification should follow a formal process:
1. Issue creation to discuss proposed changes
2. Pull request with implementation of agreed changes
3. Review by at least two core team members
4. Version number update according to change impact
5. Merge and changelog update

## 5. Binary Format Versioning

### 5.1 Object Format Version

The COIL object format includes a version field in its header:

```
struct CoilHeader {
    char     magic[4];     // "COIL"
    uint8_t  major;        // Major version
    uint8_t  minor;        // Minor version
    uint8_t  patch;        // Patch version
    uint8_t  flags;        // Format flags
    // ...
};
```

### 5.2 Version Compatibility Checks

COIL processors must check version compatibility:

```
if (header.major > SUPPORTED_MAJOR) {
    error("Unsupported COIL version");
} else if (header.major == SUPPORTED_MAJOR) {
    if (header.minor > SUPPORTED_MINOR) {
        warning("Some features may not be supported");
    }
}
```

## 6. API Stability Guarantees

### 6.1 Stability Levels

Different parts of the COIL specification have different stability guarantees:

- **Core ISA**: Stable across all minor versions of a major release
- **Type System**: Stable across all minor versions of a major release
- **Object Format**: Minor additions allowed, but core structure stable
- **ABI System**: Stable within a major version
- **Configuration Format**: May evolve with minor versions

### 6.2 Deprecation Policy

Features may be deprecated before removal:
1. Mark as deprecated in a minor version
2. Keep functionality with warnings for at least one minor version
3. Remove only in a subsequent major version

## 7. Version Adoption

COIL processors should clearly indicate their level of specification support:

- **Full Compliance**: Implements all features of a specific version
- **Partial Compliance**: Implements core features with documented limitations
- **Extended Implementation**: Implements all features plus vendor extensions

## 8. Versioning Tools

The COIL ecosystem includes tools to help with version management:

- **Version Validator**: Checks compliance with a specific version
- **Compatibility Checker**: Verifies compatibility between different versions
- **Update Assistant**: Helps migrate code between major versions