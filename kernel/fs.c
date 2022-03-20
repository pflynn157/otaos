#include <fs/fs.h>
#include <drivers/ata.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

static size_t fs_sector_start = 1;

// Init the file system
void fs_init() {
    // Init the drive
    ata_id_drive();
	ata_read(0, 1, NULL);
	
	// Read the drive for the file system start
	uint16_t data[256];
	for (size_t i = 1; i<30; i++) {
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
