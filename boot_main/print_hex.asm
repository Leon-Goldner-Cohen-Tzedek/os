; recieving data from dx

print_hex:
    pusha ; push current register state to the stack
    
    mov cx, 0 ; the index variable
    
    jmp hex_loop

hex_loop: ; actually prinst out the hex

; Strategy: get the last char of 'dx', then convert to ASCII
; Numeric ASCII values: '0' (ASCII 0x30) to '9' (0x39), so just add 0x30 to byte N.
; For alphabetic characters A-F: 'A' (ASCII 0x41) to 'F' (0x46) we'll add 0x40
; Then, move the ASCII byte to the correct position on the resulting string

    cmp cx, 4
    je end
    
    ;1. convert last char of dx to ASCII
    mov ax, dx ; use ax as our working register
    and ax, 0x000f ; masks the first three to zeros
    add al, 0x30 ; add 0x30 to convert N to "N"
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle step2
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58 = 7

step2:
    ; 2. get the correct position in the string to put our ASCII char
    ; bx <-- base address + string length - index of char
    mov bx, HEX_OUT + 5 ; base + length
    sub bx, cx ; our index string 
    mov [bx], al ; copy the ASCII char on al to the position pointed to by bx 
    ror dx, 4 ; shifts the

    add cx, 1 
    jmp hex_loop

end:
    ; prepare the parameter and prepare to call the function
    ; remember that print recieves the parameter in bx
    mov bx, HEX_OUT
    call print
    
    popa 
    ret

HEX_OUT:
    db '0x0000', 0 ; reserve space for the hex for our new string
