#ifndef ORIONOBJ_TARGET_H
#define ORIONOBJ_TARGET_H

#include "orionobj/types.h"

// Target architecture registry and management
orionobj_result_t orionobj_target_register_arch(const orionobj_arch_info_t* arch_info);
orionobj_result_t orionobj_target_unregister_arch(orionobj_arch_t arch);
const orionobj_arch_info_t* orionobj_target_get_arch_info(orionobj_arch_t arch);

// Architecture properties
const char* orionobj_target_get_arch_name(orionobj_arch_t arch);
const char* orionobj_target_get_arch_description(orionobj_arch_t arch);
uint32_t orionobj_target_get_pointer_size(orionobj_arch_t arch);
uint32_t orionobj_target_get_alignment(orionobj_arch_t arch);
bool orionobj_target_is_big_endian(orionobj_arch_t arch);
orionobj_abi_type_t orionobj_target_get_default_abi(orionobj_arch_t arch);

// Architecture classification
bool orionobj_target_is_orion_variant(orionobj_arch_t arch);
bool orionobj_target_is_x86_family(orionobj_arch_t arch);
bool orionobj_target_is_arm_family(orionobj_arch_t arch);
bool orionobj_target_is_riscv_family(orionobj_arch_t arch);
bool orionobj_target_is_64bit(orionobj_arch_t arch);
bool orionobj_target_is_32bit(orionobj_arch_t arch);
bool orionobj_target_is_16bit(orionobj_arch_t arch);

// Architecture compatibility
bool orionobj_target_is_compatible(orionobj_arch_t arch1, orionobj_arch_t arch2);
orionobj_arch_t orionobj_target_get_base_arch(orionobj_arch_t arch);
orionobj_arch_t orionobj_target_get_orion_variant(orionobj_arch_t native_arch);

// Relocation support
orionobj_result_t orionobj_target_validate_relocation(orionobj_arch_t arch,
                                                       orionobj_reloc_type_t type);

const char* orionobj_target_get_relocation_name(orionobj_arch_t arch,
                                                 orionobj_reloc_type_t type);

orionobj_result_t orionobj_target_apply_relocation(orionobj_arch_t arch,
                                                    void* data,
                                                    orionobj_reloc_type_t type,
                                                    uint64_t offset,
                                                    uint64_t value);

orionobj_result_t orionobj_target_get_relocation_info(orionobj_arch_t arch,
                                                       orionobj_reloc_type_t type,
                                                       uint32_t* size,
                                                       bool* is_relative,
                                                       bool* needs_addend);

// Architecture detection
orionobj_result_t orionobj_target_detect_from_string(const char* arch_string,
                                                      orionobj_arch_t* arch);

orionobj_result_t orionobj_target_detect_from_file(FILE* file,
                                                    orionobj_arch_t* arch);

// Architecture enumeration
orionobj_result_t orionobj_target_get_supported_architectures(orionobj_arch_t** archs,
                                                               uint32_t* count);

orionobj_result_t orionobj_target_get_orion_variants(orionobj_arch_t** archs,
                                                      uint32_t* count);

orionobj_result_t orionobj_target_get_native_architectures(orionobj_arch_t** archs,
                                                            uint32_t* count);

// Built-in architecture definitions
extern const orionobj_arch_info_t orionobj_arch_orion_x86;
extern const orionobj_arch_info_t orionobj_arch_orion_arm;
extern const orionobj_arch_info_t orionobj_arch_orion_riscv;
extern const orionobj_arch_info_t orionobj_arch_x86_32;
extern const orionobj_arch_info_t orionobj_arch_x86_64;
extern const orionobj_arch_info_t orionobj_arch_arm_32;
extern const orionobj_arch_info_t orionobj_arch_arm_64;
extern const orionobj_arch_info_t orionobj_arch_riscv_32;
extern const orionobj_arch_info_t orionobj_arch_riscv_64;

// Relocation type definitions for common architectures

// x86-64 relocations
#define ORIONOBJ_R_X86_64_NONE        0
#define ORIONOBJ_R_X86_64_64          1
#define ORIONOBJ_R_X86_64_PC32        2
#define ORIONOBJ_R_X86_64_GOT32       3
#define ORIONOBJ_R_X86_64_PLT32       4
#define ORIONOBJ_R_X86_64_COPY        5
#define ORIONOBJ_R_X86_64_GLOB_DAT    6
#define ORIONOBJ_R_X86_64_JUMP_SLOT   7
#define ORIONOBJ_R_X86_64_RELATIVE    8
#define ORIONOBJ_R_X86_64_GOTPCREL    9
#define ORIONOBJ_R_X86_64_32          10
#define ORIONOBJ_R_X86_64_32S         11
#define ORIONOBJ_R_X86_64_16          12
#define ORIONOBJ_R_X86_64_PC16        13
#define ORIONOBJ_R_X86_64_8           14
#define ORIONOBJ_R_X86_64_PC8         15

// ARM64 relocations
#define ORIONOBJ_R_AARCH64_NONE           0
#define ORIONOBJ_R_AARCH64_ABS64          257
#define ORIONOBJ_R_AARCH64_ABS32          258
#define ORIONOBJ_R_AARCH64_ABS16          259
#define ORIONOBJ_R_AARCH64_PREL64         260
#define ORIONOBJ_R_AARCH64_PREL32         261
#define ORIONOBJ_R_AARCH64_PREL16         262
#define ORIONOBJ_R_AARCH64_MOVW_UABS_G0   263
#define ORIONOBJ_R_AARCH64_MOVW_UABS_G1   264
#define ORIONOBJ_R_AARCH64_MOVW_UABS_G2   265
#define ORIONOBJ_R_AARCH64_MOVW_UABS_G3   266

// RISC-V relocations
#define ORIONOBJ_R_RISCV_NONE         0
#define ORIONOBJ_R_RISCV_32           1
#define ORIONOBJ_R_RISCV_64           2
#define ORIONOBJ_R_RISCV_RELATIVE     3
#define ORIONOBJ_R_RISCV_COPY         4
#define ORIONOBJ_R_RISCV_JUMP_SLOT    5
#define ORIONOBJ_R_RISCV_TLS_DTPMOD32 6
#define ORIONOBJ_R_RISCV_TLS_DTPMOD64 7
#define ORIONOBJ_R_RISCV_TLS_DTPREL32 8
#define ORIONOBJ_R_RISCV_TLS_DTPREL64 9
#define ORIONOBJ_R_RISCV_TLS_TPREL32  10
#define ORIONOBJ_R_RISCV_TLS_TPREL64  11

// Orion-specific relocations (extensible)
#define ORIONOBJ_R_ORION_NONE         0
#define ORIONOBJ_R_ORION_DIRECT       1   // Direct address
#define ORIONOBJ_R_ORION_RELATIVE     2   // PC-relative
#define ORIONOBJ_R_ORION_SYMBOL       3   // Symbol reference
#define ORIONOBJ_R_ORION_SECTION      4   // Section-relative
#define ORIONOBJ_R_ORION_GOT          5   // Global offset table
#define ORIONOBJ_R_ORION_PLT          6   // Procedure linkage table
#define ORIONOBJ_R_ORION_TLS          7   // Thread-local storage
#define ORIONOBJ_R_ORION_CUSTOM       8   // Custom relocation

// Architecture-specific section types
#define ORIONOBJ_SECTION_X86_UNWIND   0x80  // x86 unwind information
#define ORIONOBJ_SECTION_ARM_UNWIND   0x81  // ARM unwind information
#define ORIONOBJ_SECTION_ARM_ATTRIB   0x82  // ARM attributes
#define ORIONOBJ_SECTION_RISCV_ATTRIB 0x83  // RISC-V attributes
#define ORIONOBJ_SECTION_ORION_ISA    0x90  // Orion ISA metadata
#define ORIONOBJ_SECTION_ORION_ABI    0x91  // Orion ABI information

// Target-specific utilities
orionobj_result_t orionobj_target_encode_instruction(orionobj_arch_t arch,
                                                      const void* instruction,
                                                      uint8_t* buffer,
                                                      uint32_t buffer_size,
                                                      uint32_t* encoded_size);

orionobj_result_t orionobj_target_decode_instruction(orionobj_arch_t arch,
                                                      const uint8_t* buffer,
                                                      uint32_t buffer_size,
                                                      void* instruction,
                                                      uint32_t* decoded_size);

orionobj_result_t orionobj_target_disassemble(orionobj_arch_t arch,
                                               const uint8_t* code,
                                               uint64_t size,
                                               uint64_t base_address,
                                               FILE* output);

// Target validation
orionobj_result_t orionobj_target_validate_section_for_arch(orionobj_arch_t arch,
                                                             const orionobj_section_t* section);

orionobj_result_t orionobj_target_validate_symbol_for_arch(orionobj_arch_t arch,
                                                            const orionobj_symbol_t* symbol);

// Cross-compilation support
orionobj_result_t orionobj_target_convert_endianness(const uint8_t* source,
                                                      uint8_t* dest,
                                                      uint32_t size,
                                                      bool to_big_endian);

orionobj_result_t orionobj_target_convert_pointer_size(const uint8_t* source,
                                                        uint8_t* dest,
                                                        uint32_t source_size,
                                                        uint32_t dest_size);

// Target information queries
orionobj_result_t orionobj_target_get_calling_conventions(orionobj_arch_t arch,
                                                           orionobj_abi_type_t** abis,
                                                           uint32_t* count);

orionobj_result_t orionobj_target_get_register_info(orionobj_arch_t arch,
                                                     uint32_t* register_count,
                                                     uint32_t* register_size);

orionobj_result_t orionobj_target_get_instruction_alignment(orionobj_arch_t arch,
                                                             uint32_t* alignment);

orionobj_result_t orionobj_target_get_stack_alignment(orionobj_arch_t arch,
                                                       uint32_t* alignment);

#endif // ORIONOBJ_TARGET_H