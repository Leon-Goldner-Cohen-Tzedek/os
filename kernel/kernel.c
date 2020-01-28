#include "drivers/screen.c"

void main()
{
	clear_screen();
	kprint_at("XXXXXXXXXXX", 7, 3);
	kprint_at("This text spans multiple lines", 75, 10);
	kprint_at("There is a \nline\nbreak", 0, 15);
	kprint_at("There is another\nline\nbreak", 0, 20);
	kprint_at("What happens when we run out of space?", 45, 24);
	
}

