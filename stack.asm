;this program is an example of how the stack works 
mov ah, 0x0e ; tty mode 

mov bp, 0x8000 ; put the base pointer far enough from the beginning of boot sector memory at 0x7c00
mov sp, bp ; the stack pointer is the base pointer when initialized 

;push things onto the stack
push 'o'
push 'l'
push 'l'
push 'e'
push 'H'

; this shows how the stack moves downwards brackets [] are like dereferencing memory in c++/c
mov al, [0x7ffe] ; 0x8000 - 2
int 0x10

;recover the characters using pop
; We can only pop full words so we need an auxiliary register to manipulate
; the lower byte
; I think this will print 'Hello'
pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

; data that has been pop'd from the stack is now garbage, this'll print garbage
mov al, [0x7ffe]
int 0x10

;padding and infinite loop
jmp $

times 510-($-$$) db 0
dw 0xaa55 ;dw = define word (two bytes)
