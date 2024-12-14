#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER

#include <stdint.h>  // Make sure stdint.h is included here

// Multiboot magic number and flags
#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_HEADER_FLAGS 0x00010003

// Define the multiboot header structure
struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
};

#endif
