disk_load:
	pusha
	push %dx

	mov $0x02, %ah
	mov %dh, %al
	mov $0x02, %cl

	mov $0x00, %ch
	mov $0x00, %dh

	int $0x13
	jc disk_error

	pop %dx
	cmp %dh, %al
	jne sectors_error
	popa
	ret

disk_error:
	mov $DISK_ERROR, %si
	call print_nl
	call print
	call print_nl
	mov %ah, %dh
	call print_hex
	jmp disk_loop

sectors_error:
	mov $SECTORS_ERROR, %si
	call print_nl
	call print
	call print_nl

disk_loop:
	jmp .

DISK_ERROR: .asciz "Disk read error"
SECTORS_ERROR: .asciz "Incorrect number of sectors read"
