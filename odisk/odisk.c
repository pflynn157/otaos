//
// odisk
// Generates a simple file system for testing our OS
//
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    FILE *file = fopen("build/fs.img", "w");
    
    // Write the signature
    fputc(0xAB, file);
    fputc(0x0F, file);
    
    // Root directory object count
    uint16_t count = 2;
    fwrite(&count, sizeof(uint16_t), 1, file);
    
    // Write the first file
    char *content = "Hello!";
    
    uint8_t type = 0b00010000;
    uint16_t size = strlen(content) + 1;
    fputc(type, file);
    fwrite(&size, sizeof(uint16_t), 1, file);
    fputs(content, file);
    fputc(0, file);
    
    fclose(file);
    return 0;
}
