#include <fs/fs.h>
#include <drivers/ata.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

static size_t fs_sector_start = 1;

// State variables
static uint8_t fs_current_data[512];
static size_t fs_current_sector = 0;
static size_t fs_current_chunk = 0;

// Init the file system
void fs_init() {
    // Init the drive
    ata_id_drive();
	ata_read(0, 1, NULL);
	
	// Read the drive for the file system start
	uint16_t data[256];
	for (size_t i = 1; i<100; i++) {
	    memset(data, 0, 256);
	    ata_read(0, i, data);
	    for (size_t j = 0; j<256; j++) {
	        if (data[j] == 0x0FAB) {
	            fs_sector_start = i;
	            break;
	        }
	    }
	}
	
	// DEBUG
	// Print the sector start
	kprintf("FS Start: %d\n", fs_sector_start);
}

// Opens a file on the disk
int fs_open(const char *path) {
    uint16_t data[256];
    int root_count = 0;
    int index = 0;
    for (size_t i = fs_sector_start; i<100; i++) {
        memset(data, 0, 256);
        ata_read(0, i, data);
        
        size_t j = 0;
        for (j = 0; j<256; j++) {
            if (data[j] == 0x0FAB) break;
        }
        root_count = data[j+1];
        index = j + 2;
        break;
    }
    
    // At this point, we're in the root directory and we know
    // how many objects there are
    //
    // Now, start reading files
    for (int i = 0; i<root_count; i++) {
        uint16_t next = data[index];
        ++index;
        
        // This first element should contain the type
        if (next != 0x0010) {
            // TODO: Something...
        }
        
        uint16_t content_size = data[index];
        uint16_t name_size = data[index+1];
        index += 2;
        
        char name[name_size];
        int name_index = 0;
        
        // Now, read the name
        for (int j = 0; j<name_size/2; j++) {
            uint16_t current = data[index];
            ++index;
            
            char c1 = current;
            char c2 = current >> 8;
            name[name_index] = c1;
            name[name_index+1] = c2;
            name_index += 2;
        }
        name[name_index] = 0;
        
        // TODO: Make this into strcmp
        int correct = 1;
        for (int j = 0; j<strlen(path); j++) {
            if (path[j] == 0 && name[j] == 0) break;
            if (path[j] != name[j]) {
                correct = 0;
                break;
            }
        }
        
        if (correct) {
            // Break up the current chunk into something readable,
            // and then set the filesystem state variables
            //
            for (int j = 0, k = 0; j<256; j++, k += 2) {
                uint16_t current = data[j];
                char c1 = current;
                char c2 = current >> 8;
                fs_current_data[k] = c1;
                fs_current_data[k+1] = c2;
            }
            fs_current_sector = i;
            fs_current_chunk = index * 2;
            return 1;
        }
    }

    return 0;
}

uint8_t fs_read() {
    uint8_t data = fs_current_data[fs_current_chunk];
    ++fs_current_chunk;
    return data;
}

