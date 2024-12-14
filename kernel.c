#include "multiboot.h"
#include <stdint.h>
#include "keyboard.h"

// VGA text buffer to print the message to the screen
volatile unsigned short *vga_buffer = (unsigned short *)0xB8000;

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
    //start at row 2 because of previous prints
    static int row = 3, col = 0;

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

// Kernel entry point for GRUB (must have a specific signature)
void kernel_main(void) {
    // Clear the screen before printing anything
    clear_screen();

    // Print "Hello from the Kernel!" to the screen
    print_string("Hello from the kernel!", 0, 0);
    // Print a line of dashes, as wide as the console
    char dashes[VGA_WIDTH + 1];  // One extra space for null terminator
    for (int i = 0; i < VGA_WIDTH; i++) {
        dashes[i] = '-';  // Fill the array with dashes
    }
    dashes[VGA_WIDTH] = '\0';  // Null-terminate the string

    print_string(dashes, 1, 0);  // Print the dashes on the second line

    // Now ask the user to type something
    const char *message = "Type something: ";
    print_string(message, 2, 0);  // Print this on the 2nd line

    // Continuously read input and print it
    while (1) {
        handle_input();  // Call function from keyboard.c to read and handle input
    }
}

__attribute__((section(".multiboot_header"))) 
struct multiboot_header header = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS),
    0, 0, 0, 0, (unsigned int)kernel_main
};
