#!/bin/bash

qemu-system-i386 -drive format=raw,file=build/os.img -boot c #-no-reboot -d int,cpu_reset
