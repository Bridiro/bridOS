#receiving the data in 'dx'

print_hex:
	pusha
	mov $0, %cx

hex_loop:
	cmp $4, %cx
	je end

	#1. convert last char of 'dx' to ascii
	mov %dx, %ax
	and $0x000f, %ax
	add $0x30, %al
	cmp $0x39, %al
	jle step2
	add $7, %al

step2:
	#2. get the correct position of the string to place our ascii char
	mov $HEX_OUT + 5, %bx
	sub %cx, %bx
	mov %al, (%bx)
	ror $4, %dx

	add $1, %cx
	jmp hex_loop

end:
	mov $HEX_OUT, %si
	call print

	popa 
	ret


HEX_OUT:
	.asciz "0x0000"

