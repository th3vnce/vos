ENTRY(kernel_main)

SECTIONS {
    .multiboot_header : { *(.multiboot_header) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
