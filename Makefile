CFLAGS=-I./include

KERNEL_SRC=$(wildcard kernel/*.c)
KERNEL_OBJS=$(addprefix build/,$(patsubst %.c, %.o, $(KERNEL_SRC)))
KERNEL_ASM_SRC=$(wildcard kernel/*.asm)
KERNEL_ASM_OBJS=$(addprefix build/,$(patsubst %.asm, %.o, $(KERNEL_ASM_SRC)))

BOOT_OBJS=build/boot.o

OBJS=$(BOOT_OBJS) \
	$(KERNEL_OBJS) \
	$(KERNEL_ASM_OBJS)

all: check build/kernel.elf

.PHONY: check
check:
	if [ ! -d ./build ]; then mkdir -p build; fi
	if [ ! -d ./build/kernel ] ; then mkdir -p build/kernel; fi
    
#build/os.img: build/os.bin
#	dd if=/dev/zero of=$@ bs=1024 count=1024 conv=notrunc
#	dd if=$^ of=$@ bs=512 conv=notrunc
	
#build/os.bin: build/boot.bin build/kernel.elf
#	cat build/boot.bin build/kernel.elf build/fs.img > $@
	
build/kernel.elf: $(OBJS)
	i686-elf-gcc -T kernel/linker.ld -o build/kernel.elf -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
	
build/boot.o: boot/boot.asm
	nasm -f elf32 $^ -o $@
	
build/kernel/%.o: kernel/%.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@
	
build/kernel/%.o: kernel/%.asm
	nasm -f elf32 $< -o $@
	
.PHONY: iso
iso:
	mkdir -p build/isodir/boot/grub
	cp build/kernel.elf build/isodir/boot/kernel.elf
	cp grub/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o otaos.iso build/isodir

#.PHONY: build/fs.img	
#build/fs.img:#
	#nasm -f bin odisk/disk.asm -o build/fs.img
	
#build/odisk: odisk/odisk.c
#	$(CC) odisk/odisk.c -o build/odisk
	
.PHONY: clean
clean:
	rm build/*.o; \
	rm build/*.bin; \
	rm build/*.img; \
	rm build/*.elf; \
	rm build/kernel/*;
	
