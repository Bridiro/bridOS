.code32

.equ VIDEO_MEMORY, 0xb8000
.equ WHITE_ON_BLACK, 0x0f

print_string_pm:
	pusha
	mov $VIDEO_MEMORY, %edx

print_string_pm_loop:
	mov (%ebx), %al
	mov $WHITE_ON_BLACK, %ah

	cmp $0, %al
	je print_string_pm_done

	mov %ax, (%edx)
	add $1, %ebx
	add $2, %edx

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
