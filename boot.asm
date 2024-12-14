[org 0x7c00]          ; Boot sector starts at memory address 0x7C00

mov ah, 0x0E          ; BIOS Teletype output
mov al, 'H'           ; Character to print
int 0x10              ; BIOS interrupt for screen output

mov al, 'i'
int 0x10

jmp $

times 510-($-$$) db 0 ; Fill the rest of the 512-byte sector
dw 0xAA55             ; Boot signature
