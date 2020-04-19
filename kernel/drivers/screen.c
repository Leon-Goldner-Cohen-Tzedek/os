#include "screen.h"
#include "ports.c"
#include "util.c"

//declaration of private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// Public Kernel API functions 

// print a message in the specified location
// if col, row are negative, we will use the current offset 

void kprint_at(char *message, int col,  int row)
{	//set cursor if col/row are not negative
	int offset;
	if (col >= 0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	// loop through message print it 
	int i = 0;
	while (message[i] != 0)
	{
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		// compute row/col for next iteration
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kprint(char *message)
{
	kprint_at(message, -1, -1);
}

// Private kernel functions 
// these DIRECTLY access memory

int print_char(char c, int col, int row, char attr)
{
	unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
	if (!attr) attr = WHITE_ON_BLACK;
	
	// Error control print red E if the coords aren't right
	if (col >= MAX_COLS || row >= MAX_ROWS)
	{
		vidmem[2*(MAX_COLS)*(MAX_ROWS) - 2] = 'E';
		vidmem[2*(MAX_COLS)*(MAX_ROWS) - 1] =  RED_ON_WHITE;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >=0) {offset = get_offset(col, row); }
	else {offset = get_cursor_offset(); }
	
	if (c == '\n')
	{
		row = get_offset_row(offset);
		offset = get_offset(0, row + 1);
	}
		
	else
	{
		vidmem[offset] = c;
		vidmem[offset + 1] = attr;
		offset += 2;
	}

	/* SCROLLS */
	if (offset >= MAX_ROWS * MAX_COLS * 2)
	{
		int i;
		for (i = 1; i < MAX_ROWS; i++)
		{
			memory_copy(get_offset(0, i) + VIDEO_ADDRESS, get_offset(0, i-1) + VIDEO_ADDRESS, MAX_COLS * 2);
		}
		/*blank last line maker*/
		char* last_line = get_offset(0, MAX_ROWS-1)  + VIDEO_ADDRESS;
		for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;
		
		offset -= 2 * MAX_COLS;
	}
	
	set_cursor_offset(offset);
	return offset;
}

int get_cursor_offset()
{
	// use the VGA ports to get the current position
	// 1. ask for the high byte of the cursor offset (data 14)
	// 2. ask for the low byte of the cursor offset (data 15)

	byte_out(REG_SCREEN_CTL, 14);
	int offset = byte_in(REG_SCREEN_DATA) << 8; //high byte
	byte_out(REG_SCREEN_CTL, 15);
	offset += byte_in(REG_SCREEN_DATA);
	return offset * 2; // position * size of the character cell (2)
}

void set_cursor_offset(int offset)
{// similar to get_cursor_offset, but instead reading, we write data
	offset /= 2;	
	byte_out(REG_SCREEN_CTL, 14);
	byte_out(REG_SCREEN_DATA, (unsigned char)(offset << 8));
	byte_out(REG_SCREEN_CTL, 15);
	byte_out(REG_SCREEN_CTL, (unsigned char)(offset & 0xff));
}

void clear_screen()
{
	int screen_size = MAX_COLS * MAX_ROWS;
	int i;
	char *screen = VIDEO_ADDRESS;
	
	for (i = 0; i < screen_size; i++)
	{
		screen[i*2] = ' ';
		screen[i*2+1] = WHITE_ON_BLACK;
	}
	set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }
 
