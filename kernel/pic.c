#include <cpu/pic.h>
#include <cpu/io.h>

void pic_init() {
    write_port(PIC_1_CTRL, 0x11);
    write_port(PIC_2_CTRL, 0x11);
    
    // Remap the PIC's
    write_port(PIC_1_DATA, 0x20);
    write_port(PIC_2_DATA, 0x28);
    
    // Setup cascading
    write_port(PIC_1_DATA, 0x00);
    write_port(PIC_2_DATA, 0x00);
    
    // Setup environment info
    write_port(PIC_1_DATA, 0x01);
    write_port(PIC_2_DATA, 0x01);
    
    // Mask interrupts
    write_port(0x21, 0xFF);
    write_port(0xA1, 0xFF);
}

