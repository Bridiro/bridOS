.code16  #tell the assembler that we're in 16-bit mode
.global _start  #makes our label "_start" available to the outside

.equ KERNEL_OFFSET, 0x1000

_start:  #beginning of our binary
	mov $0x9000, %bp
	mov %bp, %sp

	mov $MSG_REAL_MODE, %si
	call print
	call print_nl

	call load_kernel
	call switch_to_pm
	jmp .

.include "boot/print.inc"
.include "boot/print_hex.inc"
.include "boot/disk.inc"
.include "boot/gdt.inc"
.include "boot/32bit_print.inc"
.include "boot/switch_pm.inc"

.code16
load_kernel:
	mov $MSG_LOAD_KERNEL, %si
	call print

	mov $KERNEL_OFFSET, %bx
	mov $31, %dh
	call disk_load	
	mov $MSG_LOAD_DONE, %si
	call print
	call print_nl
	ret

.code32
BEGIN_PM:
	mov $MSG_PROT_MODE, %ebx
	call print_string_pm
	call KERNEL_OFFSET
	jmp .

MSG_REAL_MODE: .asciz "Started in 16-bit Real Mode"
MSG_LOAD_KERNEL: .asciz "Loading Kernel into memory..."
MSG_LOAD_DONE: .asciz "  Done!"
MSG_PROT_MODE: .asciz "Landed in 32-bit Protected Mode"

.fill 510-(.-_start), 1, 0  #fill the rest of memory up to 510 bytes
.word 0xaa55   #magic bytes that tell the BIOS that this is bootable
