#include <stdio.h>
#include <stdint.h>
#include <cpu/io.h>
#include <drivers/software.h>

void software_init() {
    uint8_t mask = read_port(0x30);
    write_port(0x30, mask & 0xFD);
}

void software_handler(int num) {
    printf("Software interrupt!\n");
    switch (num) {
        case 1: printf("Stdout\n"); break;
        case 2: printf("Stdin\n"); break;
        case 3: printf("Stderr\n"); break;
        case 4: printf("Open\n"); break;
        case 5: printf("Write\n"); break;
        default: printf("Idk\n");
    }
}
