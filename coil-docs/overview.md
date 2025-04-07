# COIL Structure Overview

The core files include everything needed for a standard COIL implementation under the core conformance levels a compliant COIL processor, linker, interpreter or other tool has to implement all core features to be considered compilant.

COIL is then further split into COIL extensions beyond the standard format. There is support for preprocessing and macros through directives, ABIs, architecture and processing unit specific code and much more. These are counted as COIL features and can be checked through a feature list in the file which documents all features used by the file for a tool to decide weather it is compliant with the requested features.

A current list of COIL extensions:
- Multi-Dimensional Types (Comes with an extended instruction set and new types to be used with existing instructions)
- PreProcessor (Under the compiler instruction set extensions it comes with compiler conditionals, definitions and compiler constants)
- ABI (A complex system for creating abstractly compatible code, This also comes with a platform default standard ABI and a C ABI that has to be implemented)
- Processor Specific (Processor Specific instructions should be checked for each processing unit being targeted where an overridable instruction set can be utilized)
- Architecture Specific (Similar to Processor Specific when checking for a processor with specific instructions being implemented it can then be followed on by architecture specifics to that processing unit)
- Composite Types (Similar to Multi-Dimensional types composite types bring a good way to inherintly modify multiple types stored under one memory location organised in some structure extends some type operations)


When reading through COIL you may notice that there are almost no examples in anything remotely readable by humans, this is on purpose. COIL is a binary langauge and the only examples needed to explain COIL would encoding examples. A human variation of COIL will exist for direct writing of COIL but will still not be used for examples in the coil-docs.

This text examples are mostly useless and will just end up adding needless complexity.



