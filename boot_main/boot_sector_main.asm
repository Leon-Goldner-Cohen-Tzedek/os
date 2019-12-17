[org 0x7c00] ; the memory offset, where bootsector memory starts

mov bx, HELLO_WORLD 
call print

call print_nl

mov bx, this_is_cool
call print

HELLO_WORLD:
    db 'Hello World!', 0

this_is_cool:
    db 'this is cool', 0

jmp $

%include "print.asm"
%include "print_hex.asm"
%include "disk_read.asm"

; padding and magic number
times 510-($-$$) db 0
dw 0xaa55    
