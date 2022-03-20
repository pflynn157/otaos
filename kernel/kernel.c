
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/pic.h>
#include <cpu/io.h>
#include <video/tty.h>
#include <drivers/keyboard.h>
#include <drivers/software.h>
#include <drivers/ata.h>
#include <fs/fs.h>

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t attr;
} __attribute__((packed)) MemoryMap;
 
void kernel_main(MemoryMap **mem_map) 
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
	
	if (mem_map == NULL) {
	    kprintf("NULL MEM\n");
	} else {
	    size_t i = 0;
	    while (mem_map[i]) {
	        MemoryMap *map = mem_map[i];
	        kprintf("Memory: %d MB\n", map->length / 1024 / 1024);
	        i += sizeof(MemoryMap);
	    }
	}
	
	fs_init();
	if (fs_open("file1.txt")) {
	    kprintf("file1.txt is open!\n");
	    
	    char c = fs_read();
	    char buf[256];
	    int index = 0;
	    while (c != 0) {
	        buf[index] = c;
	        ++index;
	        c = fs_read();
	    }
	    kprintf(buf);
	}
	
	/*uint16_t data[256];
	for (int i = 0; i<256; i++) {
	    data[i] = 0x00AB;
	}*/
	
	
	/*ata_id_drive();
	ata_read(0, 1, NULL);
	ata_write(0, 25, data, 256*sizeof(uint16_t));
	
	int16_t data2[256];
	ata_read(0, 25, data2);
	if (data2[0] == 0x00AB) {
	    kprintf("Pass\n");
	} else {
	    kprintf("FAIL: %d\n", data2[0]);
	}
	
	memset(data2, 0, 256);
	ata_read(0, 25, data2);
	if (data2[0] == 0x00AB) {
	    kprintf("Pass\n");
	} else {
	    kprintf("FAIL: %d\n", data2[0]);
	}
	
	//
	//
	//
	for (int i = 0; i<256; i++) {
	    data[i] = 0xCD00;
	}
	ata_write(0, 26, data, 256*sizeof(uint16_t));
	
	//
	//
	//
	for (size_t i = 1; i<30; i++) {
	    memset(data2, 0, 256);
	    ata_read(0, i, data2);
	    for (size_t j = 0; j<256; j++) {
	        if (data2[j] == 0x0FAB) {
	            kprintf("Found at: %d\n", i);
	            break;
	        }
	    }
	}*/
	
	while (1) {}
}
