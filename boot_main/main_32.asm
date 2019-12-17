; this is a bootsector designed to enter 32bit protected mode
[org 0x7c00]
    mov bp, 0x9000 ; set the stack
    mov sp, bp
    
    mov bx, MSG_REAL_MODE
    call print ; this will be written after the BIOS messages 
    
    call switch_to_pm
    jmp $ ; this will never be executed

%include "print.asm"
%include "print_32.asm"
%include "global_descriptor_table.asm"
%include "switch_to_32.asm"

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    jmp $
    
MSG_REAL_MODE db "Started in 16bit real mode", 0
MSG_PROT_MODE db "Loaded 32bit protected mode", 0

; boot sector 

times 510-($-$$) db 0
dw 0xaa55
