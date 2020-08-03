; load dh sectors from drive dl into ES:BX
%ifndef DISK_READ_ASM
	%define DISK_READ_ASM
		disk_load:
			pusha
			; reading from the disk requires setting specific values in all registers
			; so overwrite our input parameters from dx to the stack
			push dx 
			
			mov ah, 0x02 ; ah is a int 0x13 function 0x02 = read
			mov al, dh ; al is the number of sectors to read (0x01 to 0x80)
			mov cl, 0x02 ; cl is sector 0x01 to 0x11
			
			mov ch, 0x00 ; ch cylinder 
			; dl is automatically set to the drive number by the BIOS
			mov dh, 0x00 ; dh is the head number of the drive 
			
			;[es:bx] <- pointer to buffer where the data will be stored
			; caller sets it up for us and is the standard location for int 0x13
			int 0x13 ; BIOS interrupt
			jc disk_error ; if error (stored in the carry bit)
			
			pop dx
			cmp al, dh ; BIOS also sets al to dh # of sectors read
			jne sectors_error
			popa 
			ret

		disk_error:
			mov bx, DISK_ERROR
			call print
			call print_nl
			mov dh, ah ; ah = error code, dl =  disk drive that dropped the error
			call print_hex ; the codes are here: http://stanislavs.org/helppc/int_13-1.html
			jmp disk_loop

		sectors_error:
			mov bx, SECTORS_ERROR
			call print

		disk_loop:
			jmp $

		DISK_ERROR:
			db "Disk read error", 0
		SECTORS_ERROR:
			db "Incorrect numbers of sectors read", 0
%endif
