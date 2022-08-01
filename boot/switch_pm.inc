.code16
switch_to_pm:
	cli   #1. disable interrupts
	lgdt (gdt_descriptor)   #2. loag the GDT descriptor
	mov %cr0, %eax
	or $0x1, %eax    #3. set 32-bit mode in cr0
	mov %eax, %cr0
	jmp $CODE_SEG,$init_pm   #4. far jump by using different segment

.code32
init_pm:
	mov $DATA_SEG, %ax  #5. update the segment register
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov $0x90000, %ebp   #6. update the stack right at the top of the free space
	mov %ebp, %esp

	call BEGIN_PM   #7. call a well-known label with useful code
