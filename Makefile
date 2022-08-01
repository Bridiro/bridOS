#  WILDCARDS
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}

#  COMPILERS
CC = /usr/bin/gcc
GDB = /usr/bin/gdb
AS = /usr/bin/as

all: run

run: os-image.bin
	qemu-system-x86_64 -fda os-image.bin
	
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin
	
kernel.bin: boot/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 --oformat binary $^

kernel.elf: boot/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-x86_64 -s -fda os-image.bin & ${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
	
# GENERIC RULES
%.o: %.c ${HEADERS}
	${CC} -ffreestanding -c $< -o $@
	
%.o: %.s
	as --64 -o $@ $<
	
%.bin: %.o
	ld -Ttext 0x7c00 --oformat binary -o $@ $<
	
clean:
	rm -rf *.bin *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
