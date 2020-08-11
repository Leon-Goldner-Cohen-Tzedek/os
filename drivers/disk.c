#include "disk.h"
#include "screen.h"
#include "../libc/function.h"
#include "../cpu/types.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

// Obviously you'd have this return the data, start drivers or something.
void disk_callback(registers_t regs)
{
	//error handling i guess??
	kprint("irq6 happened");	
	UNUSED(regs);
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
	//sets the configuration for the fdc, implied seek on, fifo off, drive polling off, threshold for polling?? 15 bytes	
	configure(1, 0, 0, 15);
	
	//lock the config so that it stays persistent through resets. i *think* this works, but i am really not sure. . . 	
	byte_out(DATA_FIFO, LOCK & MT_BIT);
	if (byte_in(DATA_FIFO) != (LOCK & MT_BIT << 4))
	{
		kprint("failed to lock configuration, aborting disk subsystem initialization\n");
		return 1;
	}
	
	recalibrate();
	reset();

	kprint("disk subsystem initialized\n");
	
	return 0;
	
}
void recalibrate()
{
	//drive recalibration
	byte_out(DATA_FIFO, RECALIBRATE);
	byte_out(DATA_FIFO, 0);//drives being recalibrated
	byte_out(DATA_FIFO, 1);
	byte_out(DATA_FIFO, 2);
	byte_out(DATA_FIFO, 3);
}
void reset()
{
	//reset the controller 
	byte_out(DATARATE_SELECT_REGISTER, 0 | MT_BIT);
}
void configure(int seek, int fifo, int drive_polling, int threshold) 
{
	/* recommended settings are: implied seek: on, fifo: off, drive polling: off, threshold (threshold is how many bytes are read 
	before an interrupt occurs: 15*/
	unsigned config_bit = seek << 6 | fifo << 5 | drive_polling << 4 | threshold;
	//this part should set up the proper configuration			
	byte_out(DIGITAL_OUTPUT_REGISTER, CONFIGURE);//configure command
	byte_out(DIGITAL_OUTPUT_REGISTER, 0); // 0 for some reason
	byte_out(DIGITAL_OUTPUT_REGISTER, config_bit); 
	byte_out(DIGITAL_OUTPUT_REGISTER, 0);
}
/*
void motor_on()
void motor_off()
void seek()
	
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
