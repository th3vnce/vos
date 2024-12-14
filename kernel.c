#include "multiboot.h"
#include <stdint.h>  // Include for uint16_t

// VGA text buffer to print the message to the screen
volatile unsigned short *vga_buffer = (unsigned short *)0xB8000;

// Keyboard I/O port addresses
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// VGA text mode constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0F00  // White text on black background

// Function to clear the screen
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        vga_buffer[i] = (uint16_t) ' ' | VGA_COLOR;
    }
}

// Function to print a string starting from a specific position in the VGA buffer
void print_string(const char *str, int row, int col) {
    int offset = row * VGA_WIDTH + col; // 80 columns per row, so calculate offset based on row and column
    for (int i = 0; str[i] != '\0'; ++i) {
        vga_buffer[offset + i] = (uint16_t)(str[i] | VGA_COLOR);
    }
}

// Function to print a single character at the current cursor position
void print_char(char c) {
    static int row = 0, col = 0;

    if (c == '\n') {
        row++;
        col = 0;
    } else {
        vga_buffer[row * VGA_WIDTH + col] = (uint16_t)(c | VGA_COLOR);
        col++;
    }

    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
    }
    if (row >= VGA_HEIGHT) {
        row = 0;  // Scroll screen up when full
    }
}

// Basic inb function (read from I/O port)
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Function to read a byte from the keyboard
uint8_t read_keyboard() {
    // Wait for a byte to be available in the data port
    while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0) {}
    return inb(KEYBOARD_DATA_PORT); // Read the scancode from the data port
}

// Function to handle the keyboard input and print it to the screen
void handle_input() {
    uint8_t scancode = read_keyboard();  // Get the key press scancode

    // Basic ASCII keymap (You can expand this as needed)
    if (scancode >= 0x2 && scancode <= 0x1A) {  // Letters A-Z
        print_char(scancode - 0x02 + 'A');
    } else if (scancode >= 0x1E && scancode <= 0x26) { // Numbers 1-9
        print_char(scancode - 0x1E + '1');
    } else if (scancode == 0x1C) {  // Enter
        print_char('\n');
    } else if (scancode == 0x0E) {  // Backspace
        // Clear the last character printed
        // TODO: Handle backspace by moving the cursor back and replacing the character with a space
    }
}

// Kernel entry point for GRUB (must have a specific signature)
void kernel_main(void) {
    // Clear the screen before printing anything
    clear_screen();

    // Print "Hello from the Kernel!" to the screen
    print_string("Hello from the kernel!", 0, 0);

    // Now ask the user to type something
    const char *message = "Type something: ";
    print_string(message, 2, 0);  // Print this on the 2nd line

    // Continuously read input and print it
    while (1) {
        handle_input();  // Read and print user input
    }
}

__attribute__((section(".multiboot_header"))) 
struct multiboot_header header = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS),
    0, 0, 0, 0, (unsigned int)kernel_main
};
