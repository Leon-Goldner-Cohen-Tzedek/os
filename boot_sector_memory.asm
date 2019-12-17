;testing out the memory offset on a bios 

mov ah, 0x0e ;scrolling teletype routine 

;first attempt
mov al, the_secret
int 0x10

;second attempt
mov al, [the_secret]
int 0x10

;third attempt 
mov bx, the_secret
add bx, 0x7c00
mov al, [bx]
int 0x10

;fourth attempt 
mov al, [0x7c00]
int 0x10

jmp $



the_secret:
    db 'X'

;BIOS magic number and padding 
times 510-($-$$) db 0
dw 0xaa55
