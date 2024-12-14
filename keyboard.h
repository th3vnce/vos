#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Function declarations for keyboard handling
uint8_t read_keyboard();
void handle_input();
void print_char(char c);

#endif
