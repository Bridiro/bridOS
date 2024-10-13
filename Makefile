# $@ = target file
# $< = first dependency
# $^ = all dependencies

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h  drivers/*.h cpu/*.h)
OBJ_FILES = ${addprefix $(BUILD_DIR)/, $(C_SOURCES:.c=.o cpu/interrupt.o)}

$(info    OBJ_FILES is $(OBJ_FILES))

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
	x86_64-linux-gnu-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: $(BUILD_DIR)/boot/mbr.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-x86_64 -drive file=$<,format=raw,if=floppy

echo: os-image.bin
	xxd $<

# only for debug
$(BUILD_DIR)/kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	x86_64-linux-gnu-ld -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin $(BUILD_DIR)/kernel.elf
	qemu-system-x86_64 -s -S -fda os-image.bin -d guest_errors,int &
	gdb -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

$(BUILD_DIR)/%.o: %.c ${HEADERS} | $(BUILD_DIR)
	x86_64-linux-gnu-gcc -Wno-discarded-qualifiers -fno-pie -g -m32 -ffreestanding -c $< -o $@ # -g for debugging

$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.bin: %.asm | $(BUILD_DIR)
	nasm $< -f bin -o $@

$(BUILD_DIR)/%.dis: $(BUILD_DIR)/%.bin | $(BUILD_DIR)
	ndisasm -b 32 $< > $@

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) os-image.bin