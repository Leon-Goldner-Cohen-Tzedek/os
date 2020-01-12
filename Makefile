# $@ = target file
# $< = first dependency
# $^ = all dependencies
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

#Nice syntax for the file extension replacement 
OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/cross/bin/bin/i686-elf-gcc
GDB = /usr/local/cross/bin/bin/i686-elf-gdb
CFLAGS = -g -ffreestanding -c 

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# Notice how dependencies are built as needed
kernel.bin: boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@
%.bin: %.asm 
	nasm $< -f bin -o $@

run: os-image.bin
	qemu-system-i386 -nographic -fda $<

clean:
	rm -rf *.bin *.o *.dis
	rm -rf kernel/*.o boot/*,bin drviers/*.o boot/*,o
