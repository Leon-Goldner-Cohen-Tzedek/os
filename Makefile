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

os-image.bin:  boot/bootsect.bin kernel/kernel.bin
	cat $^ > $@

# Notice how dependencies are built as needed

boot/bootsect.bin: 
	cd boot && make

kernel/kernel.bin:
	cd kernel && make

kernel/kernel.elf:
	cd kernel && make $@

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

debug: os-image.bin kernel/kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel/kernel.elf"
		
clean:
	rm -rf *.o *.dis *.bin
	rm -rf kernel/*.o kernel/*.bin  boot/*.bin drivers/*.o boot/*.o
