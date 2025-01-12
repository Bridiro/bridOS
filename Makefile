# $@ = target file
# $< = first dependency
# $^ = all dependencies

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

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/src/*.c drivers/src/*.c cpu/src/*.c kernel/libs/sw-lib-font/src/*c)
INCLUDES = -Ikernel/inc -Idrivers/inc -Icpu/inc -Ikernel/libs/sw-lib-font/inc
OBJ_FILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(C_SOURCES)))
OPT_FLAGS = -Os

vpath %.c kernel/src drivers/src cpu/src kernel/libs/sw-lib-font/src

ASM_SPECIAL_SOURCE = cpu/interrupt.asm
ASM_SPECIAL_OBJ = $(BUILD_DIR)/interrupt.o

ASM_SOURCES = $(wildcard boot/*.asm cpu/%.asm)
ASM_BIN_FILES = $(patsubst %.asm,$(BUILD_DIR)/%.bin,$(notdir $(ASM_SOURCES)))

vpath %.asm boot cpu

# First rule is the one executed when no parameters are fed to the Makefile
all: os-image.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Notice how dependencies are built as needed
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o ${OBJ_FILES} ${ASM_SPECIAL_OBJ}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: $(BUILD_DIR)/mbr.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -drive file=$<,format=raw,if=floppy -vga virtio

echo: os-image.bin
	xxd $<

# only for debug
$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o ${OBJ_FILES} ${ASM_SPECIAL_OBJ}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: OPT_FLAGS = -O0 -g
debug: os-image.bin $(BUILD_DIR)/kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(GCC) -Wno-discarded-qualifiers -fno-pie $(OPT_FLAGS) -m32 -msoft-float -mfpmath=sse -msse -msse2 -mno-red-zone -ffreestanding -c $< -o $@ $(INCLUDES) # -g for debugging

$(BUILD_DIR)/%.o: %.asm | $(BUILD_DIR)
	nasm $< -f elf -o $@

$(BUILD_DIR)/%.bin: %.asm | $(BUILD_DIR)
	nasm $< -f bin -o $@

$(BUILD_DIR)/%.dis: $(BUILD_DIR)/%.bin | $(BUILD_DIR)
	ndisasm -b 32 $< > $@

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) os-image.bin
