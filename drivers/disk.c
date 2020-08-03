#include "disk.h"
#include "../kernel/kernel.h"
#include "../cpu/types.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
//private disk reading functions


// Obviously you'd have this return the data, start drivers or something.
void disk_callback(registers_t regs)
{
	//error handling i guess??
	kprint("irq6 happened");	

}
//returns 0 on success
int disk_init()
{
	register_interrupt_handler(IRQ6, disk_callback);
	
	//check the version, if the result byte is not 0x90, just abort
	byte_out(DATA_FIFO, VERSION);
	if (byte_in(DATA_FIFO) != 0x90)// this is the lowest common denominator controller, the osdev wiki says so, so it *must* be true . . .
	{
		kprint("floppy version incorrect, aborting disk subsystem initialization\n");
		return 1;
	}
	unsigned config_bit = /*enable implied seek*/ 1 << 6 | /*disable fifo*/ 0 << 5 | /*drive polling disable*/ 0 << 4 |/*threshold, how many bytes are read before interrupt*/ 15;
	//this part should set up the proper configuration			
	byte_out(DIGITAL_OUTPUT_REGISTER, CONFIGURE);//configure command
	byte_out(DIGITAL_OUTPUT_REGISTER, 0); // 0 for some reason
	byte_out(DIGITAL_OUTPUT_REGISTER, config_bit); 
	byte_out(DIGITAL_OUTPUT_REGISTER, 0);
	
	//lock the config so that it stays persistent through resets. i *think* this works, but i am really not sure. . . 	
	byte_out(DATA_FIFO, LOCK & MT_BIT);
	if (byte_in(DATA_FIFO) != LOCK & MT_BIT << 4)
	{
		kprint("failed to lock configuration, aborting disk subsystem initialization\n");
		return 1;
	}
	
	kprint("disk subsystem initialized\n");
	return 0;
	
		
}
/*
int kread(u32 blocks, u32 count, int track, int start_sector)
{
	//turn on drive motor
		// byte_out command to the DIGITAL_OUTPUT_REGISTER
	byte_out(DIGITAL_OUTPUT_REGISTER, etc etc 
	//wait a bit until the motor is up to speed
	//issue the command byte + parameter bytes to the fifo command io port
	//exchange data with the drive / seek the drive heads "execution phase" on the FIFO pot 	
	//get irq 6 at the end of the execution phase but only if the command as an execution phase 
	//read any "result bytes" produced by the command on the fifo port
	
}
void kwrite(int sector, int track, int head, u32 data, u32 count)
{
}

*/	
