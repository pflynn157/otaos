#include <stdio.h>
#include <stdint.h>
#include <cpu/io.h>
#include <drivers/software.h>

void software_init() {
    uint8_t mask = read_port(0x30);
    write_port(0x30, mask & 0xFD);
}

void software_handler(int num) {
    kprintf("Software interrupt!\n");
    switch (num) {
        case 1: kprintf("Stdout\n"); break;
        case 2: kprintf("Stdin\n"); break;
        case 3: kprintf("Stderr\n"); break;
        case 4: kprintf("Open\n"); break;
        case 5: kprintf("Write\n"); break;
        default: kprintf("Idk\n");
    }
}
