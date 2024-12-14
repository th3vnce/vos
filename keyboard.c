#include <stdint.h>
#include "keyboard.h"

// I/O port addresses for the keyboard
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

extern void print_string(const char *str, int row, int col);

// Hungarian QWERTZ scancode-to-ASCII table (CP437 encoding)
static const char scancode_map[128] = {
    0, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 27 , 0x93, 0x94, '\b', // 0x00-0x0E (ó=0x93, ö=0x94)
    '\t', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 0x8F, 0x81, '\n',    // 0x0F-0x1C (ő=0x8F, ú=0x81)
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0x82, 0x86, 0,   0,        // 0x1D-0x2B (é=0x82, á=0x86)
    0, 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,   '*', 0,   ' '         // 0x2C-0x39
    // Extend as needed for shifted or special keys
};

// Basic inb function (read from I/O port)
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Function to read a scancode from the keyboard data port
uint8_t read_keyboard() {
    // Wait for data to be available in the status port
    while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0) {}
    return inb(KEYBOARD_DATA_PORT); // Read scancode from the data port
}

// Function to convert a scancode to an ASCII character
char get_char_from_scancode(uint8_t scancode) {
    // Process only valid scancodes
    if (scancode > 0 && scancode < 128) {
        return scancode_map[scancode];
    }
    return 0; // Invalid scancode
}


void handle_input() {
    uint8_t scancode = read_keyboard();  // Read scancode

    // Convert scancode to ASCII
    char ch = get_char_from_scancode(scancode);

    // If a valid character, print it
    if (ch) {
        print_char(ch);
    }
    
}
