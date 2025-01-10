# $@ = target file
# $< = first dependency
# $^ = all dependencies

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h  drivers/*.h cpu/*.h)
OBJ_FILES = ${addprefix $(BUILD_DIR)/, $(C_SOURCES:.c=.o cpu/interrupt.o)}

# Detect the operating system
OS := $(shell uname -s)

# Set the toolchain based on the OS
ifeq ($(OS), Darwin)
    GCC = x86_64-elf-gcc
    LD  = x86_64-elf-ld
    GDB = x86_64-elf-gdb
    NASM = nasm
else
    GCC = x86_64-linux-gnu-gcc
    LD  = x86_64-linux-gnu-ld
    GDB = gdb
    NASM = nasm
endif

BUILD_DIR = build

# First rule is the one executed when no parameters are fed to the Makefile
all: os-image.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/boot
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/cpu
	mkdir -p $(BUILD_DIR)/drivers

# Notice how dependencies are built as needed
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: $(BUILD_DIR)/boot/mbr.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-x86_64 -drive file=$<,format=raw,if=floppy -vga virtio

echo: os-image.bin
	xxd $<

# only for debug
$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot/kernel_entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin $(BUILD_DIR)/kernel.elf
	qemu-system-x86_64 -s -S -fda os-image.bin -d guest_errors,int &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

$(BUILD_DIR)/%.o: %.c ${HEADERS} | $(BUILD_DIR)
	$(GCC) -Wno-discarded-qualifiers -fno-pie -g -m32 -ffreestanding -c $< -o $@ # -g for debugging

$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.bin: %.asm | $(BUILD_DIR)
	nasm $< -f bin -o $@

$(BUILD_DIR)/%.dis: $(BUILD_DIR)/%.bin | $(BUILD_DIR)
	ndisasm -b 32 $< > $@

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) os-image.bin
