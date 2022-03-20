#pragma once

#include <stdint.h>

void fs_init();
int fs_open(const char *path);
uint8_t fs_read();
void fs_close();

