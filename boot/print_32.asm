; this program uses 32 bit real mode, it writes directly to the video memory that starts at 0xb8000
; there are then routines that are able to print chars on the screen from 0xb8000
%ifndef PRINT_32_ASM
	%define PRINT_32_ASM 
		[bits 32]

		; constants being defined 
		VIDEO_MEMORY equ 0xb8000
		WHITE_ON_BLACK equ 0x0f ; the color byte for each char 

		print_string_pm:
			pusha
			mov edx, VIDEO_MEMORY

		print_string_pm_loop:
			mov al, [ebx] ; ebx is the address of our character 
			mov ah, WHITE_ON_BLACK

			cmp al, 0 ; check if we are at the end of a string 
			je print_string_pm_done

			mov [edx], ax ; store character + attribute in video memory
			add ebx, 1 ; next char 
			add edx, 2 ; next video memory position

			jmp print_string_pm_loop


		print_string_pm_done:
			popa 
			ret
%endif
