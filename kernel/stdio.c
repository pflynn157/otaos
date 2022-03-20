#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void print_int(size_t num) {
    // First, we need the number of digits
    size_t size = 0;
    size_t num2 = num;
    while (num2 != 0) {
        num2 /= 10;
        ++size;
    }
    
    // Now, we can convert to string
    char buf[size];
    size_t index = size - 1;
    
    while (num != 0) {
        size_t digit = num % 10;
        num /= 10;
        buf[index] = digit + '0';
        --index;
    }
    
    // Finally, print
    terminal_writestring(buf);
}

void kprintf(const char *s, ...) {
    //terminal_writestring(s);
    va_list args;
    va_start(args, s);
    
    size_t length = strlen(s);
    char buf[length];
    memset(buf, 0, length);
    size_t index = 0;
    
    for (size_t i = 0; i<length; i++) {
        if (s[i] == '%') {
            char fmt = s[i+1];
            ++i;
            
            switch (fmt) {
                // Print an integer
                case 'd': {
                    if (index > 0) {
                        buf[index] = 0;
                        terminal_writestring(buf);
                        memset(buf, 0, length);
                        index = 0;
                    }
                    
                    size_t num = va_arg(args, size_t);
                    print_int(num);
                } break;
                
                // Unknown specifier- append the string
                default: {
                    buf[index] = '%';
                    buf[index+1] = fmt;
                    index += 2;
                }
            }
        } else {
            buf[index] = s[i];
            ++index;
        }
    }
    
    if (index > 0) {
        buf[index] = 0;
        terminal_writestring(buf);
    }
}

