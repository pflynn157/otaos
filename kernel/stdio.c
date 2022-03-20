#include <stdio.h>

void kprintf(const char *s, ...) {
    terminal_writestring(s);
}

