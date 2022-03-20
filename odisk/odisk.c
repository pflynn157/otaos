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
    uint16_t count = 3;
    fwrite(&count, sizeof(uint16_t), 1, file);
    
    // Write the first file
    char *content = "Hello!";
    char *name = "file1.txt";
    
    uint8_t type = 0b00010000;
    uint16_t size = strlen(content) + 1;
    fputc(type, file);
    fputc(0, file);
    fwrite(&size, sizeof(uint16_t), 1, file);
    uint16_t name_length = strlen(name) + 1;
    fwrite(&name_length, sizeof(uint16_t), 1, file);
    fputs(name, file);
    fputc(0, file);
    fputs(content, file);
    fputc(0, file);
    
    fclose(file);
    return 0;
}
