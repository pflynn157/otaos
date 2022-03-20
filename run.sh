#!/bin/bash

qemu-system-i386 -drive format=raw,file=build/os.img -boot c -m 10 #-no-reboot -d int,cpu_reset
