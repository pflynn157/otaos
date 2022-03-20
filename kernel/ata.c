#include <stdint.h>
#include <stddef.h>

#include <drivers/ata.h>
#include <cpu/io.h>

#include <stdio.h>

// This is temporary, more of a sanity check
void outw(uint16_t port, uint16_t dat) {			
				asm volatile(
					"outw %0, %1"
					:
					: "a"(dat), "d"(port)
				);
}

			uint8_t inb(uint16_t port) {				
				uint8_t dat;
				asm volatile(
					"inb %1, %0"
					: "=a"(dat)
					: "d"(port)
				);
				return dat;				
			}

			void outb(uint16_t port, uint8_t dat) {
				asm volatile(
					"outb %1, %0"
					:
					: "d"(port), "a"(dat)
				);
			}
			
						uint16_t inw(uint16_t port) {			
				uint16_t dat;
				asm volatile (
					"inw %1, %0"
					: "=a"(dat)
					: "d"(port)
				);
				return dat;
				}

// Port constants
const uint16_t ata_port_data = 0x1F0;
const uint16_t ata_port_err = 0x1F1;
const uint16_t ata_port_sc = 0x1F2;
const uint16_t ata_port_lba_low = 0x1F3;
const uint16_t ata_port_lba_mid = 0x1F4;
const uint16_t ata_port_lba_high = 0x1F5;
const uint16_t ata_port_dh = 0x1F6;         // Drive head select
const uint16_t ata_port_cmd = 0x1F7;        // Also use for status

// Drive types
const uint8_t ata_master_drive = 0xE0;
const uint8_t ata_slave_drive = 0xF0;

// Commands
const uint16_t ata_cmd_id = 0xEC;
const uint16_t ata_cmd_read = 0x20;
const uint16_t ata_cmd_write = 0x30;
const uint16_t ata_cmd_flush = 0xE7;

// Status bits
const uint16_t ata_status_drq = (uint8_t)8;
const uint16_t ata_status_bsy = (uint8_t)128;

void ata_check_error() {
    uint8_t err = inb(ata_port_err);
    if ((err & 1) == 1) {
        kprintf("SATA error: Address not found.\n");
    } else if ((err & 2) == 2) {
        kprintf("SATA error: Track zero not found.\n");
    } else if ((err & 4) == 4) {
        kprintf("SATA error: abort.\n");
    } else if ((err & 8) == 8) {
        kprintf("SATA error: change req.\n");
    } else if ((err & 16) == 16) {
        kprintf("SATA error: ID not found.\n");
    } else if ((err & 32) == 32) {
        kprintf("SATA error: media changed.\n");
    } else if ((err & 64) == 64) {
        kprintf("SATA error: UNC Block error.\n");
    } else if ((err & 128) == 128) {
        kprintf("SATA error: Bad block.\n");
    }
}

// Identify the drive
void ata_id_drive() {
    outb(ata_port_dh, ata_master_drive);
    for (uint16_t port = 0x1F2; port <= 0x1F5; port++) outb(port, 0);
    outb(ata_port_cmd, ata_cmd_id);
    
    uint16_t status = 0;
    for (int i = 0; i<10000; i++) {
        status = inb(ata_port_cmd);
        if ((status & ata_status_bsy) == 0) break;
        if ((status & 1) == 1) {
            status = 0;
            break;
        }
    }
    if (status == 0) {
        kprintf("No drive!\n");
        return;
    } else {
        kprintf("ATA found\n");
    }
    
    /*uint16_t identity_data[256];
	for (uint16_t i = 0; i < 256; i++)
		identity_data[i] = inw(ata_port_cmd);
    uint32_t lba1_max = identity_data[60];
    uint32_t lba2_max = identity_data[61] << 16;
    uint32_t max_sc_count = 0;
    max_sc_count |= lba1_max;
    max_sc_count |= lba2_max;
    if (max_sc_count == 0) {
        kprintf("LBA28 not supported.\n");
    }*/
}

/*void ata_read1(uint32_t drive, uint32_t addr, uint16_t num_sectors) {
    uint16_t num_read = num_sectors * 256;
    size_t bytes = num_read * sizeof(uint16_t);
    
    outb(ata_port_sc, num_sectors/256);
    outb(ata_port_lba_low, addr);
    outb(ata_port_lba_mid, addr >> 8);
    outb(ata_port_lba_high, addr >> 16);
    outb(ata_port_dh, ata_master_drive | (drive << 4) | ((addr >> 24) & 0xF));
    outb(ata_port_cmd, ata_cmd_read);
    
    // Poll until ready
    int ready = 0;
    for (int i = 0; i<1000; i++) {
        uint8_t status = inb(ata_port_cmd);
        if ((status & ata_status_bsy) == 0 && (status & ata_status_drq) !=0) {
            ready = 1;
            break;
        }
    }
    //if (ready) printf("Ready to write!\n");
    //else printf("Not ready to write");
    
    for (size_t i = 0; i<bytes/sizeof(uint16_t); i++) {
        inw(ata_port_data);
    }
}*/

void ata_read(uint32_t drive, uint32_t addr, uint16_t *data) {
    outb(ata_port_sc, 1);
    outb(ata_port_lba_low, addr);
    outb(ata_port_lba_mid, addr >> 8);
    outb(ata_port_lba_high, addr >> 16);
    outb(ata_port_dh, ata_master_drive | (drive << 4) | ((addr >> 24) & 0xF));
    outb(ata_port_cmd, ata_cmd_read);
    
    // Poll until ready
    int ready = 0;
    for (int i = 0; i<1000; i++) {
        uint8_t status = inb(ata_port_cmd);
        if ((status & ata_status_bsy) == 0 && (status & ata_status_drq) != 0) {
            ready = 1;
            break;
        }
    }
    
    for (size_t i = 0; i<256; i++) {
        uint16_t d = inw(ata_port_data);
        if (addr != 0) data[i] = d;
    }
}

void ata_write(uint32_t drive, uint32_t addr, uint16_t *data, size_t size) {
    // Start setting ports
    outb(0x1F1, 0);
    outb(ata_port_sc, size/512);
    outb(ata_port_lba_low, addr);
    outb(ata_port_lba_mid, addr >> 8);
    outb(ata_port_lba_high, addr >> 16);
    outb(ata_port_dh, ata_master_drive | (drive << 4) | ((addr >> 24) & 0xF));
    outb(ata_port_cmd, ata_cmd_write);
    
    // Poll until we're ready to write
    int ready = 0;
    for (int i = 0; i<10000; i++) {
        uint8_t status = inb(ata_port_cmd);
        if ((status & ata_status_bsy) == 0 && (status & ata_status_drq) !=0) {
            ready = 1;
            break;
        } else if ((status & 1) == 1) {
            break;
        }
    }
    //if (ready) kprintf("Ready to write!\n");
    //else kprintf("Not ready to write");
    
    // Now, write
    for (size_t i = 0; i<256; i++) {
        outw(ata_port_data, data[i]);
    }
    
    // Flush the cache
    outb(ata_port_cmd, ata_cmd_flush);
    int done = 0;
    for (int i = 0; i<10000; i++) {
        uint8_t status = inb(ata_port_cmd);
        if ((status & ata_status_bsy) == 0) {
            done = 1;
            break;
        } else if ((status & 1) == 1) {
            done = 0;
            break;
        }
    }
    
    //if (done) kprintf("Write done!\n");
    //else kprintf("Write failed.\n");
}

