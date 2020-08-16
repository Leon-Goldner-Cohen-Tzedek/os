#include "dma.h"
#include "ports.h"
#include "../libc/mem.c"
#include "../libc/bits.h"

int dma_init()
{
	//this dma init is just for the floppy disk right now
	byte_out(SINGLE_MASK_8, 0x06);
	byte_out(CLEAR_FLIPFLOP_8, 0xff);	
	byte_out(0x04, 0);
	byte_out(0x04, 0x10);
	byte_out(CLEAR_FLIPFLOP_8, 0xff);
	byte_out(0x05, 0xff);
	byte_out(0x05, 0x23);
	byte_out(CHNL_2, 0);
	byte_out(SINGLE_MASK_8, 0x02);
	return 0;
}

void set_channel(int channel, struct dma_profile profile)
{
	if (channel > 7)
	{
		kprint("DMA error: invalid channel\n");	
	}
	else
	{
		profile.chanel = channel;
	}
	set_ports(channel, profile);
}

void set_ports(int channel, struct dma_profile profile)
{
	switch(channel)
	{//8 bit channels
		case 0:
		{
			profile.address_port = 0x00;
			profile.count_port = 0x01;
			profile.page_port = 0x87;
		}
		case 1:
		{
			profile.address_port = 0x02;
			profile.count_port = 0x03;
			profile.page_port = 0x83;
		}
		case 2:
		{
			profile.address_port = 0x04;
			profile.count_port = 0x05;
			profile.page_port = 0x81;
		}			
		case 3:
		{
			profile.address_port = 0x06;
			profile.count_port = 0x07;
			profile.page_port = 0x82;
		}			
		//16 bit channels
		case 4:
		{
			profile.address_port = 0xc0;
			profile.count_port = 0xc2;
			profile.page_port = 0x8f;
		}			
		case 5:
		{
			profile.address_port = 0xc4;
			profile.count_port = 0xc6;
			profile.page_port = 0x8b;
		}			
		case 6:
		{
			profile.address_port = 0xc8;
			profile.count_port = 0xca;
			profile.page_port = 0x89;
		}			
		case 7:
		{
			profile.address_port = 0xcc;
			profile.count_port = 0xce;
			profile.page_port = 0x8a;
		}			
		default: kprint("DMA error: invalid channel\n");
	}
	if (channel < 4)
	{
		profile.mask_register = 0x0a;
		profile.clear_register = 0x0c;
		profile.mode_register = 0x0b;
	}
	else
	{
		profile.mask_register = 0xd4;
		profile.clear_register = 0xd8;
		profile.mode_register = 0xd6;
	}
}
