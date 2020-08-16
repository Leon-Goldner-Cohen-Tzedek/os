#include "../cpu/isr.h"
#include "../cpu/dma.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/strings.h"
#include "../libc/mem.h"
#include "../drivers/disk.h"
void main()
{
	
	isr_install();
	irq_install();
	dma_init();
	disk_init();
	kprint("welcome to LeonOS: \"basically just copied from a tutorial\"\n^ ");
}

void user_input(char* input)
{
	if (strcmp(input, "END") == 0)
	{
		kprint("stopping the CPU\n");
		asm volatile("hlt");
	}
	else if	(strcmp(input, "PAGE") == 0)
	{
		u32 phys_addr;
		u32 page = kmalloc(1000, 1, &phys_addr);
		char page_str[16] = "";
		hex_to_ascii(page, page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr, phys_str);
		kprint("Page: ");
		kprint(page_str);
		kprint(", physical address: ");
		kprint(phys_str);
	}
	else if (strcmp(input, "CLEAR") == 0)
	{
		clear_screen();
	}
	else if (strcmp(input, "") != 0)
	{
		kprint("that is not a valid command");
	}
	
	kprint("\n^ ");
	
}
