#include "timer.h"
#include "../drivers/screen.h"
#include "../libc/function.h"
#include "isr.h"
#include "ports.h"
u32 tick = 0;

static void timer_callback(registers_t regs)
{
	tick++;
	UNUSED(regs);
}


void init_timer(u32 freq)
{
	//install the function we just wrote 
	register_interrupt_handler(IRQ0, timer_callback);
	
	//get the PIT value: hardware clock at 1193180 Hz
	u32 divisor = 1193180 / freq;
	u8 low = (u8)(divisor & 0xFF);
	u8 high = (u8)( (divisor >> 8) & 0xFF);
	//send the command
	byte_out(0x43, 0x36); //command port
	byte_out(0x40, low);
	byte_out(0x40, high);
}
