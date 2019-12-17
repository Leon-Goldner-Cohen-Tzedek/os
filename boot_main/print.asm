; this is going to be the basic print subroutine for the bootsector
; it also demonstrates how functions and strings work in assembly
; use:
; mov bx, what_you_want_to_print
; call print

print:
    pusha ; saves the current state of the registers
    jmp start

; while (string[i] != 0) {print string[i]; i++}
start:
    mov al, [bx] ; bx is the base address for the string
    ; if al is 0, jump to done
    cmp al, 0
    je done 

    ; print with BIOS help
    mov ah, 0x0e
    int 0x10
    
    ; increment pointer and start next loop
    add bx, 1
    jmp start

done:
    popa ; restore register states
    ret ; return to the address called from

;prints a newline character
print_nl:
    pusha 
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return 
    int 0x10

    popa 
    ret 
