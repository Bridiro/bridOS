[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel

ModeInfoBlock equ 0x8000   ; Buffer for info on VESA mode

mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
mov bp, 0x9000
mov sp, bp

mov bx, MSG_16BIT_MODE
call print16
call print16_nl

call load_kernel ; read the kernel from disk
call switch_to_32bit ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
jmp $ ; Never executed

%include "boot/print-16bit.asm"
%include "boot/print-32bit.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch-to-32bit.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print16
    call print16_nl

    ; Load kernel
    mov bx, KERNEL_OFFSET           ; Read from disk and store in 0x1000
    mov dh, 54                      ; Read 54 sectors (for some reason is the limit)
    mov dl, [BOOT_DRIVE]            ; Read from this disk
    call disk_load

    ; Set VESA mode
    mov ax, 0x4F02                  ; VESA function to set mode
    mov bx, 0x115 | 0x4000          ; Mode 800x600x24-bit
    int 0x10
    jc vesa_fail

    mov ax, 0x4F01                  ; VESA function to get info
    mov cx, 0x115                   ; Mode 800x600x24-bit
    mov di, ModeInfoBlock           ; ModeInfoBlock buffer pointer
    int 0x10
    jc vesa_fail

    ret

vesa_fail:
    mov bx, MSG_VESA_FAIL
    call print16
    hlt

[bits 32]
BEGIN_32BIT:
    mov eax, cr0          ; Read control register CR0
    and eax, 0xFFFFFFFB   ; Disable FPU emulation (bit 2 = 0)
    or eax, 0x2           ; Set co-processors monitoring
    mov cr0, eax          ; Re-write in CR0

    mov eax, cr4
    or eax, 0x200         ; Set OSFXSR bit (bit 9) to enable SSE instructions
    mov cr4, eax
    fninit                ; Initialize FPU
    
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_16BIT_MODE db "16-bit Real Mode", 0
MSG_32BIT_MODE db "32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
MSG_VESA_FAIL db "Failed to set VESA", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
