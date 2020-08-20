#include "disk.h"
#include "screen.h"
#include "../libc/function.h"
#include "../cpu/types.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/dma.h"

// Obviously you'd have this return the data, start drivers or something.
static void disk_callback(registers_t regs)
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
	kprint("checking floppy disk version. . .\n");
	fdc_byte_out(DATA_FIFO, VERSION);
	if (fdc_byte_in(DATA_FIFO) != 0x90)// this is the lowest common denominator controller, the osdev wiki says so, so it *must* be true . . .
	{
		kprint("floppy version incorrect: not enhanced, aborting disk subsystem initialization\n");
	}

	//sets the configuration for the fdc, implied seek on, fifo off, drive polling off, threshold for polling?? 15 bytes	
	configure(1, 0, 0, 15);
	
	//lock the config so that it stays persistent through resets. i *think* this works, but i am really not sure. . . 	
	if (fdc_byte_in(DATA_FIFO) != (LOCK & MT_BIT << 4))
	{
		kprint("failed to lock configuration, aborting disk subsystem initialization\n");
		return 1;
	}
	
	reset();

	
	kprint("disk subsystem initialized\n");
	return 0;
}

void recalibrate()
{
	kprint("recalibrating controller . . .\n");
	//drive recalibration
	fdc_byte_out(DATA_FIFO, RECALIBRATE);
	fdc_byte_out(DATA_FIFO, 0);//drives being recalibrated
}
void reset()
{
	kprint("resetting controller. . .\n");
	//reset the controller 
	byte_out(DIGITAL_OUTPUT_REGISTER, 0);
	byte_out(DATARATE_SELECT_REGISTER, 0);
	fdc_byte_out(DATA_FIFO, SPECIFY);
	fdc_byte_out(DATA_FIFO, 0xdf); //SRT = 3ms, HUT = 240ms
	fdc_byte_out(DATA_FIFO, 0x02); //HLT = 16ms, ND = 0
	
	seek(1);
	recalibrate();

}
void configure(int seek, int fifo, int drive_polling, int threshold) 
{
	kprint("configuring controller. . .\n");
	/* recommended settings are: implied seek: on, fifo: off, drive polling: off, threshold (threshold is how many bytes are read 
	before an interrupt occurs): 15*/
	unsigned char config_bit = seek << 6 | fifo << 5 | drive_polling << 4 | threshold;
	//this part should set up the proper configuration			
	fdc_byte_out(DIGITAL_OUTPUT_REGISTER, CONFIGURE);
	fdc_byte_out(DIGITAL_OUTPUT_REGISTER, 0); // 0 for some reason
	byte_out(DIGITAL_OUTPUT_REGISTER, config_bit); 
	byte_out(DIGITAL_OUTPUT_REGISTER, 0);
}

int fdc_byte_out(unsigned short port, unsigned char byte)
{
	long timeout = FDC_TIMEOUT;
	while (timeout != 0)
	{
		unsigned char  msr = byte_in(MAIN_STATUS_REGISTER);
		if (msr == 0x80 || 0x90)//this might work through context, but i am not sure. . .might become a problem later
		{
			byte_out(port, byte);
			return 0;
		}
		timeout--;
	}
	kprint("Floppy Disk Controller Error: Main Status Register Time out\n");
	return 1;
}

unsigned char fdc_byte_in(unsigned short port)
{
	long timeout = FDC_TIMEOUT;
	while (timeout != 0)
	{
		unsigned char msr = byte_in(MAIN_STATUS_REGISTER);
		if (msr == 0x80 || 0x90)
		{
			return byte_in(port);
		}
		timeout--;
	}
	kprint("Floppy Disk Controller Error: Main Status Register Time out\n");
	return (unsigned char)1;
}

void motor_on()
{
	//simply turns on the motor for now, figuring out delay systems later (might not be necessary due to emulator
	fdc_byte_out(DIGITAL_OUTPUT_REGISTER, 0x10);//0x10 is the value for turning on motor a
} 

void seek(int track)
{
//bare minimum, no checks or anything . . .
	fdc_byte_out(DATA_FIFO, SEEK);
	fdc_byte_out(DATA_FIFO, 0);	
	fdc_byte_out(DATA_FIFO, track);
}
void lba_to_hst(int block, int *head, int *track, int *sector)
{
   *head = (block % (SECTORS_PER_TRACK * /*heads (always 2)*/2)) / (SECTORS_PER_TRACK);
   *track = block / (SECTORS_PER_TRACK * /*heads*/ 2);
   *sector = block % SECTORS_PER_TRACK + 1;
}
void kread_write(int block, unsigned long number_sectors, int read)
{
	int head, track, sector, copy_count = 0;
	
	lba_to_hst(block, &head, &track, &sector);
	
	struct dma_profile dma_profile;	
		//turn on drive motor
	motor_on();

	for (int tries = 0; tries < 3; tries++)
	{
		if(read) 
		{
			// byte_out command to the DIGITAL_OUTPUT_REGISTER
			//wait a bit until the motor is up to speed
			//issue the command byte + parameter bytes to the fifo command io port
			fdc_byte_out(DATA_FIFO, READ_DATA);
			fdc_byte_out(DATA_FIFO, head << 2);
			fdc_byte_out(DATA_FIFO, track);
			fdc_byte_out(DATA_FIFO, head);
			fdc_byte_out(DATA_FIFO, sector); //starting sector
			fdc_byte_out(DATA_FIFO, 2);//something about sector sizes (all use 512bytes, so 2 is the magic number
			fdc_byte_out(DATA_FIFO, 0x1b); //default gap between sectors
			fdc_byte_out(DATA_FIFO, 0xff); //something to do with sector sizes	
			return 0;
		}
	}

}


