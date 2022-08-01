gdt_start:
	.word 0x0000, 0x0000
	.byte 0x00, 0b00000000, 0b00000000, 0x00
gdt_code:
	.word 0xffff, 0x0000
	.byte 0x00, 0b10011010, 0b11001111, 0x00
gdt_data:
	.word 0xffff, 0x0000
	.byte 0x00, 0b10010010, 0b11001111, 0x00

gdt_end:

#GDT descriptor
gdt_descriptor:
	.word (gdt_end - gdt_start - 1)
	.long gdt_start
	
#define some constants for later use
.equ CODE_SEG, (gdt_code - gdt_start)
.equ DATA_SEG, (gdt_data - gdt_start)

