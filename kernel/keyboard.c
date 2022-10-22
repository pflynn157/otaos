#include <stdio.h>

#include <drivers/keyboard.h>
#include <cpu/io.h>

unsigned char keymap[128] = {
    0, 0,
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    0 /* backspace */, 0 /* tab */,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    0 /* enter */, 0 /* left-ctrl */,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0 /* left shift */,
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0 /* right shift */, '*', 0 /*left alt*/, ' '
};

void keyboard_init() {
    write_port(0x21, 0xFD);
}

extern void software_int_test1();
extern void software_int_test2();
extern void software_int_test3();

void keyboard_handler() {
    signed char keycode = read_port(0x60);
    if (keycode >= 0 && keymap[keycode]) {
        char buf[2];
        buf[0] = keymap[keycode];
        buf[1] = '\0';
        kprintf(buf);
        
        char c = keymap[keycode];
        
        if (c == '1') {
            software_int_test1();
        } else if (c == '2') {
            software_int_test2();
        } else if (c == '3') {
            software_int_test3();
        }
    }
    
    // Acknowledge the interrupt
    write_port(0x20, 0x20);
    write_port(0xA0, 0x20);
}
