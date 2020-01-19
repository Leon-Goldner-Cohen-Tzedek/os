[bits 32]
[extern main] ; define the calling point. Must have the same name as the kernel.c 'main' function
call main
jmp $
