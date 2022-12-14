#pragma once

#include <stdint.h>
#include <stddef.h>

void ata_id_drive();
void ata_read(uint32_t drive, uint32_t addr, uint16_t *data);
void ata_write(uint32_t drive, uint32_t addr, uint16_t *data, size_t size);
