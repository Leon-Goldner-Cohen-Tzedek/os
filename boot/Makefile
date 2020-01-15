bootsect.bin: bootsect.asm 
	nasm $^ -f bin -o $@

kernel_entry.o: kernel_entry.asm
	nasm $^ -f elf -o $@ 
