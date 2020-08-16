#ifndef DISK_H
#define DISK_H

//floppy disk ports
#define STATUS_REGISTER_A 0x3F0 // read-only
#define STATUS_REGISTER_B 0x3F1 // read-only
#define DIGITAL_OUTPUT_REGISTER 0x3F2
#define TAPE_DRIVE_REGISTER 0x3F3
#define MAIN_STATUS_REGISTER 0x3F4 // read-only
#define DATARATE_SELECT_REGISTER 0x3F4 // write-only
#define DATA_FIFO 0x3F5
#define DIGITAL_INPUT_REGISTER 0x3F7 // read-only
#define CONFIGURATION_CONTROL_REGISTER 0x3F7  // write-only

//floppy disk commands  
#define READ_TRACK 2 //generates irq6
#define SPECIFY 3 // set driver params
#define SENSE_DRIVE_STATUS 4
#define WRITE_DATA 5 // write to the disk
#define READ_DATA 6 // read from the disk
#define RECALIBRATE 7 // seek to track 0
#define SENSE_INTERRUPT 8 // ack irq6 get status of last command
#define WRITE_DELETED_DATA 9
#define READ_ID 10 // generates irq6
#define READ_DELETED_DATA 12     
#define DUMPREG 14
#define SEEK 15 //seek both heads to track X
#define VERSION 16 //used during initialization once
#define SCAN_EQUAL 17
#define PERPENDICULAR_MODE 18 // used during init once, maybe
#define CONFIGURE 19 // set controller params
#define LOCK 20 // protects controller params from reset
#define VERIFY 22
#define SCAN_LOW_OR_EQUAL 25
#define SCAN_HIGH_OR_EQUAL 29

//option bits 
#define MT_BIT 0x80
#define MF_BIT 0x40
#define FDC_TIMEOUT 128 

int disk_init();
unsigned char fdc_byte_in(unsigned short port);
int fdc_byte_out(unsigned short port, unsigned char byte);
int kread(int blocks, int count, int track, int start_sector);
void kwrite(int sector, int track, int head, int data, int count);
void configure(int seek, int fifo, int polling, int threshold);
void recalibrate();
void reset();
void motor_on();
void motor_off();
void seek();
#endif
