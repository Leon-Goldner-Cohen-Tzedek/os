;to assemble: nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
;0x0e turns on tty mode
mov ah, 0x0e
mov al, 'H'
;the 0x10 interupt prints whatever is in al to the screen
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10
mov al, 'o'
int 0x10

jmp $ ;infinite loop by calling the current adress forever

;fill with 510 zeros minus the size of the previous code 
times 510-($-$$) db 0
;magic number to tell BIOS to boot (BIOSes look at the last two bytes of the sector) 
dw 0xaa55
