# Variables
CC = i686-elf-gcc
ASM = nasm
LD = i686-elf-ld
GRUB = grub-mkrescue

# Output files
KERNEL_BIN = kernel.bin
ISO = os.iso
ISO_DIR = iso

# Compiler flags
CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

# Assembly files
ASMFILES = boot.asm

# Source files
SRC = kernel.c
OBJ = $(SRC:.c=.o)

# Build the kernel binary
all: $(KERNEL_BIN) $(ISO)

$(KERNEL_BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(KERNEL_BIN)

# Assemble the bootloader
boot.asm: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

# Link the object files to create the binary
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Create the ISO
$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	echo 'set timeout=0\nset default=0\nmenuentry "My OS" {\n  multiboot /boot/kernel.bin\n}' > $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $(ISO) $(ISO_DIR)

# Clean up build files
clean:
	rm -f *.o $(KERNEL_BIN) boot.bin $(ISO)
	rm -rf $(ISO_DIR)
