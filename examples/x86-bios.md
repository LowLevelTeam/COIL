# COIL x86 BIOS Boot Example

This example demonstrates how to create a simple x86 BIOS bootloader in COIL, transitioning through the different modes of the x86 architecture (16-bit real mode, 32-bit protected mode, and 64-bit long mode).

## Setup and Configuration

```
; Set processor to CPU
PROC 0x01

; Set architecture to x86 in 16-bit mode
ARCH 0x01, 0x01

; Define bootloader section
SECTION bootloader, 0x01 | 0x04  ; Executable and Readable
```

## 16-bit Real Mode Code

```
; Create bootloader entry point
SYM boot_start

; Ensure proper alignment for boot sector
ALIGN 0x200

; Boot signature magic numbers
DATA TYPE_UNT8, 0x55
DATA TYPE_UNT8, 0xAA

; Initialize segments
MOV TYPE_RS=DS, 0x0000
MOV TYPE_RS=ES, 0x0000
MOV TYPE_RS=SS, 0x0000
MOV TYPE_RGP=SP, 0x7C00

; Print welcome message
MOV TYPE_RGP=SI, welcome_msg
CALL print_string

; Check for A20 line and enable if needed
CALL check_a20
CMP TYPE_RGP=AX, 0
BR_EQ enable_a20

; Load GDT for protected mode
MOV TYPE_RGP=AX, gdt_ptr
CALL load_gdt

; Transition to protected mode
BR switch_to_pm

SYM welcome_msg
DATA TYPE_ARRAY=TYPE_UNT8, "COIL Bootloader starting...", 0

SYM print_string
; Simple routine to print null-terminated string pointed to by SI
PUSH TYPE_RGP=AX
PUSH TYPE_RGP=BX

SYM print_loop
; Load character
MOV TYPE_RGP=AL, [TYPE_RGP=SI]
; Check for null terminator
CMP TYPE_RGP=AL, 0
BR_EQ print_done
; BIOS teletype output
MOV TYPE_RGP=AH, 0x0E
MOV TYPE_RGP=BX, 0x0007
INT 0x10
; Next character
INC TYPE_RGP=SI
BR print_loop

SYM print_done
POP TYPE_RGP=BX
POP TYPE_RGP=AX
RET
```

## Transition to 32-bit Protected Mode

```
; Switch architecture mode for the next section
ARCH 0x01, 0x02  ; x86 in 32-bit protected mode

SYM switch_to_pm
; Disable interrupts
CLI

; Enable protected mode bit in CR0
MOV TYPE_RGP=EAX, TYPE_RGP=CR0
OR TYPE_RGP=EAX, 0x00000001
MOV TYPE_RGP=CR0, TYPE_RGP=EAX

; Far jump to flush pipeline and set CS
JMP 0x08:pm_start

SYM pm_start
; Set up segment registers
MOV TYPE_RGP=AX, 0x10
MOV TYPE_RS=DS, TYPE_RGP=AX
MOV TYPE_RS=ES, TYPE_RGP=AX
MOV TYPE_RS=FS, TYPE_RGP=AX
MOV TYPE_RS=GS, TYPE_RGP=AX
MOV TYPE_RS=SS, TYPE_RGP=AX

; Set up a proper stack
MOV TYPE_RGP=ESP, 0x90000

; Check for long mode support
CALL check_long_mode
CMP TYPE_RGP=EAX, 0
BR_EQ no_long_mode

; Set up paging for long mode
CALL setup_paging

; Transition to long mode
BR switch_to_lm
```

## Transition to 64-bit Long Mode

```
; Switch architecture mode for the next section
ARCH 0x01, 0x03  ; x86 in 64-bit long mode

SYM switch_to_lm
; Enable PAE
MOV TYPE_RGP=EAX, TYPE_RGP=CR4
OR TYPE_RGP=EAX, 0x00000020  ; Set PAE bit
MOV TYPE_RGP=CR4, TYPE_RGP=EAX

; Set long mode bit in EFER MSR
MOV TYPE_RGP=ECX, 0xC0000080  ; EFER MSR
RDMSR
OR TYPE_RGP=EAX, 0x00000100   ; Set LME bit
WRMSR

; Enable paging
MOV TYPE_RGP=EAX, TYPE_RGP=CR0
OR TYPE_RGP=EAX, 0x80000000  ; Set PG bit
MOV TYPE_RGP=CR0, TYPE_RGP=EAX

; Load 64-bit GDT
MOV TYPE_RGP=RAX, gdt64_ptr
CALL load_gdt64

; Jump to 64-bit code segment
JMP 0x08:lm_start

SYM lm_start
; Set up 64-bit segment registers
MOV TYPE_RGP=AX, 0x10
MOV TYPE_RS=DS, TYPE_RGP=AX
MOV TYPE_RS=ES, TYPE_RGP=AX
MOV TYPE_RS=FS, TYPE_RGP=AX
MOV TYPE_RS=GS, TYPE_RGP=AX
MOV TYPE_RS=SS, TYPE_RGP=AX

; Set up final stack
MOV TYPE_RGP=RSP, 0x200000

; Jump to the kernel
MOV TYPE_RGP=RAX, kernel_entry
BR TYPE_RGP=RAX
```

## Key Observations

1. The `ARCH` directive with a mode parameter is used to switch between different x86 modes.
2. Each mode section contains code appropriate for that mode:
   - 16-bit: BIOS calls, real mode setup
   - 32-bit: Protected mode initialization, checking for long mode
   - 64-bit: Long mode setup, transition to kernel

3. Register references use the same type system throughout, but the underlying registers change size based on the mode.

4. Mode-specific instructions (like RDMSR/WRMSR) are only available in their appropriate modes.

This example demonstrates how COIL can handle the complete boot process for an x86 system, from the initial BIOS handoff to the final kernel execution, with clean transitions between the architecture modes.