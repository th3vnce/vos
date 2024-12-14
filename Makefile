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

# Source files
SRC = kernel.c keyboard.c
OBJ = $(SRC:.c=.o)

# Build the kernel binary
all: $(ISO)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJ)

# Compile C source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create the ISO
$(ISO): $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	echo 'set timeout=0\nset default=0\nmenuentry "My OS" {\n  multiboot /boot/kernel.bin\n}' > $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $(ISO) $(ISO_DIR)

# Clean up build files
clean:
	rm -f *.o $(KERNEL_BIN) $(ISO)
	rm -rf $(ISO_DIR)
