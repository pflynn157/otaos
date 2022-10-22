#!/bin/bash

sudo dd if=build/os.img of=/dev/sdc && sync

echo ""
echo "Done"
echo ""

