#include "dma.h"
#include "ports.h"
#include "../libc/mem.h"
#include "../libc/bits.h"

void dma_init(unsigned char mode, unsigned char increment, unsigned char cycle, unsigned char transfer, int channel, struct dma_profile profile)
{
	set_channel(channel, profile);
	enable_channel(profile);
	set_control_byte_mask(mode, increment, cycle, transfer, channel, profile);
	set_transfer_length(transfer, profile);
	set_control_byte(profile);
	allocate_buffer(profile);		
}

void allocate_buffer(struct dma_profile profile)
{
	profile.buffer_base_addr = kmalloc(DMA_BUFFER, 1, profile.buffer_phys_addr);
}		

void set_transfer_length(unsigned short length, struct dma_profile profile)
{
	profile.transfer_length = length;
	byte_out(profile.count_port, lobyte(length-1));
	byte_out(profile.count_port, hibyte(length-1));
}

void set_buffer_info(struct dma_profile profile)
{
	byte_out(profile.page_port, profile.buffer_base_addr);
	byte_out(profile.address_port, profile.buffer_phys_addr & 0xff);
	byte_out(profile.address_port, profile.buffer_phys_addr >> 8);
}	

void set_control_byte_mask(unsigned char mode, unsigned char increment, unsigned char cycle, unsigned char transfer, int channel, struct dma_profile profile)
{
	profile.control_byte_mask = (mode + increment + cycle + transfer);
	profile.control_byte_mask += profile.control_byte_mask;
}

void set_control_byte(struct dma_profile profile)
{
	profile.control_byte |= profile.control_byte_mask; 
	byte_out(profile.mode_register, profile.control_byte);
}

void set_channel(int channel, struct dma_profile profile)
{ 
	if (channel > 7)
	{
		kprint("DMA error: invalid channel\n");	
	}
	else
	{
		profile.channel = channel;
	}
	set_ports(channel, profile);
}

void enable_channel(struct dma_profile profile)
{
	unsigned char mask = 0;
	mask = profile.mask_register;
	byte_out(profile.mask_register, mask);
}

void disable_channel(struct dma_profile profile)
{
	unsigned char mask = 0;
	mask = profile.mask_register;
	mask |= 4;
	byte_out(profile.mask_register, mask);
}

void clear_fliplop(struct dma_profile profile)
{
	byte_out(profile.clear_register, 0x0000);
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
