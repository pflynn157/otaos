
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/pic.h>
#include <cpu/io.h>
#include <video/tty.h>
#include <drivers/keyboard.h>
#include <drivers/software.h>
#include <drivers/ata.h>
 
void kernel_main(void) 
{
    asm volatile("cli");
    setupGDT();
    pic_init();
    idt_init();
    
    // Map the keyboard
    idt_set_descriptor(0x21, keyboard_handler_int, 0x8E);
    keyboard_init();
    
    idt_set_descriptor(0x30, software_handler_int, 0x8E);
    software_init();
    
	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
	
	uint16_t data[256];
	for (int i = 0; i<256; i++) {
	    data[i] = 0xABCD;
	}
	ata_id_drive();
	ata_read1(0, 0, 1);
	ata_write(0, 20, data, 256*sizeof(uint16_t));
	
	uint16_t data2[256];
	ata_read(0, 20, data2, 2);
	if (data2[0] == 0xABCD && data2[1] == 0xABCD) {
	    printf("Pass");
	}
	
	while (1) {}
}
